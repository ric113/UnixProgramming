#include <string>
#include <vector>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>


#include "lib.h"
#include "shell.h"
#include "unixpipe.h"

using namespace std;

void unblockSigsBeforeExecChild(sigset_t &oldmask)
{
	sigprocmask(SIG_SETMASK, &oldmask, NULL);
}

sigset_t initShell()
{
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
	if(cmd.command == "fg")
	{

	}
	else if(cmd.command == "bg")
	{

	}
	else if(cmd.command == "exit")
	{
		exit(0);
	}
	else
	{
		char **args = TranVecToCharArr(cmd.args, cmd.command);
		execvp((cmd.command).c_str(), args);
	}
}


void processCmds(vector<Cmd> &cmdTable, bool isBackground,sigset_t &oldmask)
{
	bool hasPipe = (cmdTable.size() > 1)? true:false;
	int pgid = 0 ;
	vector<UnixPipe> pipeList;

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

			// cout << "Cmd " << i << " : stdin - " <<  cmdTable[i].incomeFd << ", stdout - " << cmdTable[i].outgoFd << endl;
		}
	}

	for(int i = 0 ; i < cmdTable.size() ; i ++)
	{
		pid_t pid = fork();

		if(pid == 0)
		{
			setpgid(0,pgid);

      		unblockSigsBeforeExecChild(oldmask);
      		// set stdin stdout .
      		
      		dup2(cmdTable[i].incomeFd,STDIN_FILENO);
      		dup2(cmdTable[i].outgoFd,STDOUT_FILENO);
      			
      		// 若該Line有Pipe, 關閉那些沒用到的Pipe fd => 關掉整個PipeList中紀錄的FD(因為已經把要用的dup到stdin/stdout了!) .
      		for(int i = 0 ; i < pipeList.size() ; i ++)
			{
				closePipe(pipeList[i]);
			}
			pipeList.clear();
      		
      		executeCmd(cmdTable[i]);

      		exit(127);	// error with exec (Command not found in $PATH).

		}
		else if(pid > 0)
		{
			if(pgid == 0){
        		pgid = pid;
      		}
      		// set pgid for child .
      		setpgid(pid,pgid);
		}
	}

	// close pipes , reset pipe List .
	for(int i = 0 ; i < pipeList.size() ; i ++)
	{
		closePipe(pipeList[i]);
	}
	pipeList.clear();
	
	if(!isBackground)
	{
		tcsetpgrp(STDIN_FILENO,pgid);

		int waitedProcessAmount = cmdTable.size();
	    while(waitedProcessAmount > 0)
	    {
	      int status = 0;
		  pid_t pid = waitpid(pgid * -1, &status ,WUNTRACED);
	      if(WIFSTOPPED(status))
	      {
	        // stopped_jobs.emplace_back(pgid,wait_count);
	        
	        break;
	      }
	     
	     	waitedProcessAmount--;
	    }
	    // change foreground to my pgid
	    tcsetpgrp(STDIN_FILENO,getpgid(0));
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
		
		if(line.find("&") != string::npos){
			isBackground = true;
			cout << "Is bg line!" << endl;
		}

		parseInputLine(line,"|",cmds);
		// showCmdsVector(cmds);
		initCmdTable(cmdTable,cmds);
		// showCmdTable(cmdTable);
		processCmds(cmdTable, isBackground, oldmask);

		cmds.clear();
		cmdTable.clear();
		isBackground = false;

		printPrompt();


	}
}