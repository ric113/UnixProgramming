#ifndef __SHELL_H__
#define __SHELL_H__

#include <signal.h>

#include "lib.h"


sigset_t initShell();
void sigcldHandler(int);
void processCmds(vector<Cmd>&, sigset_t&);
void waitForForegroundJob(Job&);
void executeCmd(Cmd&);
void unblockSigsBeforeExecChild(sigset_t&);
void removeJobFromList(vector<Job>&, Job&);
Job changeProcessStatus(pid_t,int);
bool jobIsStopped(Job&);
bool jobIsCompleted(Job&);
void addJobToActiveList(Job&);
void addJobToStoppedList(Job&);
void removeJobFromActiveList(Job&);
void removeJobFromStoppedList(Job&);
void updateAllJobLists(Job&);

#endif 	/* Def shell.h */