#ifndef __UNIXPIPE_H__
#define __UNIXPIPE_H__ 

typedef struct _unixpipe{
	int pipeEnterFd;
	int pipeExitFd;
}UnixPipe;

void createPipe(UnixPipe&);
void closePipe(UnixPipe&);
#endif