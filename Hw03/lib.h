#ifndef __LIB_H__
#define __LIB_H__

#include <string>
#include <vector>

using namespace std;

typedef struct _Cmd{
	string command;
	bool isRedirectBracket ;
	int pid;
	int incomeFd;
	int outgoFd;
	bool isStopped;
	bool isCompleted;
	vector<string> args;
}Cmd ;

typedef struct _Job{
	int groupId;
	int processCount;
	vector<Cmd> processList ;
}Job;

void parseInputLine(const string&, string, vector<string>&);
void initCmdTable(vector<Cmd>&, vector<string>&);
void showCmdsVector(vector<string>&);
void showCmdTable(vector<Cmd>&);
vector<string> splitWithSpace(const string&);
char** TranVecToCharArr(vector<string>,string);
vector<string> expandArgs(vector<string>&);
bool hasSpecialCmd(string);
void processRedirectBracket(Cmd&,vector<string>&);
void printPrompt();

void unblockSigsBeforeExecChild(sigset_t&);
void removeJobFromList(vector<Job>&, Job&);
Job changeProcessStatus(pid_t,Job&,int);
bool jobIsStopped(Job&);
bool jobIsCompleted(Job&);

#endif 	/* Def lib.h */

