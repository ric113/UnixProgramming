#include <signal.h>

#include "lib.h"

sigset_t initShell();
void unblockSigsBeforeExecChild(sigset_t&);
void processCmds(vector<Cmd>&, sigset_t&);
void executeCmd(Cmd&);