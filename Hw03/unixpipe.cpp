#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

#include "unixpipe.h"

void createPipe(UnixPipe &upipe)
{
	int pipefd[2];
	if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

   upipe.pipeExitFd = pipefd[0];
   upipe.pipeEnterFd = pipefd[1];
}

void closePipe(UnixPipe &pipe)
{
	close(pipe.pipeExitFd);
	close(pipe.pipeEnterFd);
}