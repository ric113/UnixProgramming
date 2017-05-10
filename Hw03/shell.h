#include <signal.h>

#include "lib.h"


sigset_t initShell();
void sigcldHandler(int);
void unblockSigsBeforeExecChild(sigset_t&);
void processCmds(vector<Cmd>&, sigset_t&);
void removeJobFromList(vector<Job>&, Job&);
void waitForForegroundJob(Job&);
void executeCmd(Cmd&);
Job changeProcessStatus(pid_t,Job&,int);
bool jobIsStopped(Job&);
bool jobIsCompleted(Job&);