#include <string>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <map>

#include "lib.h"
#include "shell.h"
#include "unixpipe.h"

#define CHAR_BUF_SIZE 256

using namespace std;

// Global Variables .
vector<Job> activeJobList;
vector<Job> stoppedJobList;
map<string,string> envVariables;

void sigcldHandler(int signum)
{
	pid_t pid = 0;
	int status = 0 ;

	do{
		pid = waitpid ((pid_t)(-1), &status, WNOHANG | WUNTRACED);

		for(int i = 0 ; i < activeJobList.size() ; i ++)
		{
			for(int j = 0 ; j <  activeJobList[i].processList.size() ; j ++)
			{
				if(activeJobList[i].processList[j].pid == pid)
				{
					if(WIFSTOPPED(status))
			    		activeJobList[i].processList[j].isStopped = true;
			    	else
			    		activeJobList[i].processList[j].isCompleted = true;

			    	updateAllJobLists(activeJobList[i]);
			    	
					
			    	break;
				}
			}
		}
		
	}while(pid <= 0);
}

void updateAllJobLists(Job &job)
{
	
	if(jobIsStopped(job))
	{
		addJobToStoppedList(job);
	}

	if(jobIsCompleted(job))
	{
		// free job .
		job.processList.clear();
		removeJobFromActiveList(job);
		removeJobFromStoppedList(job);
  	}
}

void addJobToActiveList(Job &job)
{
	activeJobList.push_back(job);
}

void addJobToStoppedList(Job &job)
{
	stoppedJobList.push_back(job);
}
void removeJobFromActiveList(Job &job)
{
	removeJobFromList(activeJobList,job);
}
void removeJobFromStoppedList(Job &job)
{
	removeJobFromList(stoppedJobList,job);
}

void unblockSigsBeforeExecChild(sigset_t &oldmask)
{
	sigprocmask(SIG_SETMASK, &oldmask, NULL);
}

sigset_t initShell()
{
	// set SIGCHLD handler .
	signal(SIGCHLD, sigcldHandler);
	// block some signals .
	sigset_t newmask, oldmask;
  	sigemptyset(&newmask);
  	sigaddset(&newmask,SIGTTOU);
  	sigaddset(&newmask,SIGTSTP);
  	sigaddset(&newmask,SIGTTIN);
  	sigaddset(&newmask,SIGINT);
  	sigprocmask(SIG_BLOCK, &newmask, &oldmask);

  	return oldmask;
}

void executeCmd(Cmd& cmd)
{
	vector<string> expandedArgs = expandArgs(cmd.args);
	char **args = TranVecToCharArr(expandedArgs, cmd.command);
	execvp((cmd.command).c_str(), args);
}

void removeJobFromList(vector<Job> &list, Job &job)
{
	for(int i = 0 ; i < list.size() ; i ++)
	{
		if(list[i].groupId == job.groupId)
		{
			list.erase(list.begin() + i);
			break;
		}
	}
}


// Change [pid]'s status(isStopped / isCompleted), and return the job it belongs to .
Job changeProcessStatus(pid_t pid,int status)
{
	Job tempJob ;
	
	if(pid > 0)
	{

		for(int i = 0 ; i < activeJobList.size() ; i ++)
		{
			for(int j = 0 ; j <  activeJobList[i].processList.size() ; j ++)
			{
				if(activeJobList[i].processList[j].pid == pid)
				{
					
					if(WIFSTOPPED(status))
			    		activeJobList[i].processList[j].isStopped = true;
			    	else
			    		activeJobList[i].processList[j].isCompleted = true;


			    	tempJob = activeJobList[i];
			    	break;
				}
			}
		}

	}

	return tempJob;
}

bool jobIsStopped(Job &job)
{
	for(int i = 0 ; i < job.processList.size() ; i++)
	{
		if(!job.processList[i].isStopped && !job.processList[i].isCompleted)
			return false;
	}
	return true;
}

bool jobIsCompleted(Job &job)
{
	for(int i = 0 ; i < job.processList.size() ; i++)
	{
		if(!job.processList[i].isCompleted)
			return false;
	}
	return true;
}


void waitForForegroundJob(Job &job)
{

	signal(SIGCHLD, SIG_DFL);

	pid_t pid;
	tcsetpgrp(STDIN_FILENO,job.groupId);
	int status = 0;
	bool jobStopped = false;
	bool jobCompleted = false;
	Job tempJob;

	do{
		// Wait for "ALL" pid, 因為此Func第一行有SIG_DFL, 若有先後上執行順序的問題可能會造成非這個[job]的process沒人處理,
		// 所以只好由這邊來處理 . 
		pid = waitpid((pid_t) -1, &status ,WUNTRACED);	
		// tempJob 即這個Process所屬的Job .
		tempJob = changeProcessStatus(pid, status);

		// [job]等於這個Process所屬的Job(tempJob) , 並存下回傳的結果(jobStopped/jobCompleted) .
		if(tempJob.groupId == job.groupId)
		{
			jobStopped = jobIsStopped(tempJob);
			jobCompleted = jobIsCompleted(tempJob);
		}
		else // 處理非目前focus的[job] .
		{
			jobIsStopped(tempJob);
			jobIsCompleted(tempJob);
		}

		updateAllJobLists(tempJob);
	
	}while(!jobStopped && !jobCompleted);
	
	// change foreground to my pgid .
	tcsetpgrp(STDIN_FILENO,getpgid(0));
}


void processCmds(vector<Cmd> &cmdTable, bool isBackground,sigset_t &oldmask)
{
	signal(SIGCHLD, sigcldHandler);

	bool hasPipe = (cmdTable.size() > 1)? true:false;
	int pgid = 0 ;
	vector<UnixPipe> pipeList;
	Job newJob;

	// Press return key only .
	if(cmdTable.size() == 0)
		return;


	if(hasPipe)
	{
		// 處理Pipes .
		// 1. 設定各個Cmd Entry的incomeFd & outgoFd .
		// 2. Create Pipe .
		for(int i = 0 ; i < cmdTable.size() - 1 ; i ++)
		{
			UnixPipe newPipe;
			createPipe(newPipe);
			pipeList.push_back(newPipe);

			cmdTable[i].outgoFd = newPipe.pipeEnterFd;
			cmdTable[i+1].incomeFd = newPipe.pipeExitFd;
		}
	}

	for(int i = 0 ; i < cmdTable.size() ; i ++)
	{
		if(hasSpecialCmd(cmdTable[i].command))
		{
			if(cmdTable[i].command == "exit")
			{
				cout << "[Exit My Shell!]" << endl;
				exit(0);
			}
			else if(cmdTable[i].command == "fg")
			{
				if(activeJobList.size() > 0)
				{
					if(cmdTable[i].args.size() > 0)
					{
						int jobNum ;
						sscanf(cmdTable[i].args[0].c_str(),"%%%d",&jobNum);
						waitForForegroundJob(activeJobList[jobNum - 1]);
					}
					else
						waitForForegroundJob(activeJobList[0]);
				}
			}
			else if(cmdTable[i].command == "bg")
			{
				if(stoppedJobList.size() > 0)
				{
					Job wakedJob = stoppedJobList[stoppedJobList.size() - 1];
					stoppedJobList.pop_back();
					kill(wakedJob.groupId * -1,SIGCONT);
				}
			}
			else if(cmdTable[i].command == "export")
			{
				char name[CHAR_BUF_SIZE],value[CHAR_BUF_SIZE];
				sscanf(cmdTable[i].args[0].c_str(),"%[^=]=%s",name,value);
				setenv(name, value, 1);
				envVariables[name] = value;

			}
			else if(cmdTable[i].command == "unset")
			{
				unsetenv(cmdTable[i].args[0].c_str());
				envVariables.erase(cmdTable[i].args[0].c_str());
			}
			else if(cmdTable[i].command == "printenv")
			{
				if(cmdTable[i].args.size() > 0)
				{
					cout << cmdTable[i].args[0] << "=" << envVariables[cmdTable[i].args[0]] << endl;
				}
				else
				{
					for (map<string,string>::iterator it=envVariables.begin(); it!=envVariables.end(); ++it)
    					cout << it->first << "=" << it->second << endl;
				}
			}

			return;
		}
		
		pid_t pid = fork();

		if(pid == 0)
		{
			// 設定自己的group id = pgid .
			setpgid(0,pgid);

      		// Reset一些Parent(shell process)的設定 .
      		unblockSigsBeforeExecChild(oldmask);
      		signal(SIGCHLD, SIG_DFL);
      		
      		dup2(cmdTable[i].incomeFd,STDIN_FILENO);
      		dup2(cmdTable[i].outgoFd,STDOUT_FILENO);
      			
      		// 若該Line有Pipe, 關閉那些沒用到的Pipe fd => 關掉整個PipeList中紀錄的FD(因為已經把要用的dup到stdin/stdout了!) .
      		for(int i = 0 ; i < pipeList.size() ; i ++)
			{
				closePipe(pipeList[i]);
			}
			pipeList.clear();
      		
      		executeCmd(cmdTable[i]);

      		cout << "[Unknown Command : "<< cmdTable[i].command << "]" << endl;
      		exit(127);	// error with exec (Command not found in $PATH).

		}
		else if(pid > 0)
		{
			if(pgid == 0){
        		pgid = pid;
      		}

      		// set pgid for child .
      		cmdTable[i].pid = pid;
      		setpgid(pid,pgid);

      		// if has open file(Redirect), close .
      		if(cmdTable[i].isRedirectBracket)
      		{
      			if(cmdTable[i].outgoFd != STDOUT_FILENO)
      				close(cmdTable[i].outgoFd);
      			if(cmdTable[i].incomeFd != STDIN_FILENO)
      				close(cmdTable[i].incomeFd);
      		}

		}
	}

	newJob.groupId = pgid;
	newJob.processCount = cmdTable.size();
	newJob.processList = cmdTable;
	activeJobList.push_back(newJob);

	// close pipes , reset pipe List .
	for(int i = 0 ; i < pipeList.size() ; i ++)
	{
		closePipe(pipeList[i]);
	}
	pipeList.clear();
	
	if(!isBackground)
	{
		waitForForegroundJob(newJob);
	}
}


int main()
{
	string line;
	vector<string> cmds;		
	vector<Cmd> cmdTable;		// Store All Cmds info. in this line .
	bool isBackground = false;

	sigset_t oldmask = initShell();	// block some signals .
	printPrompt();

	while(getline(cin,line))
	{
		
		/*
		cout << "active Job # : "<<activeJobList.size() << endl;
		cout << "suspend Job # : "<<stoppedJobList.size() << endl;
		for(int i = 0 ; i < stoppedJobList.size() ; i ++)
		{
			cout << stoppedJobList[i].groupId << endl;
		}
		*/
		
	
		if(line.find("&") != string::npos)
			isBackground = true;
		

		parseInputLine(line,"|",cmds);
		initCmdTable(cmdTable,cmds);
		processCmds(cmdTable, isBackground, oldmask);
		

		cmds.clear();
		cmdTable.clear();
		isBackground = false;

		printPrompt();
	}
}