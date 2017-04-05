#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <dlfcn.h>
#include <dirent.h>

FILE* logOutput;

static uid_t (*origin_getuid)(void) = NULL;
static ssize_t (*origin_write)(int fd,const void *buf,size_t count) = NULL;
static int (*origin_closedir)(DIR *dirp) = NULL;
static DIR* (*origin_fdopendir)(int fd) = NULL;
static DIR* (*origin_opendir)(const char *name) = NULL;
static struct dirent* (*origin_readdir)(DIR *dirp) = NULL;
static int (*origin_readdir_r)() = NULL;




static __attribute__((constructor)) void beforeMain()
{
        if(strcmp(getenv("MONITOR_OUTPUT"),"stderr") == 0)
                logOutput = stderr;
        else
                logOutput = fopen(getenv("MONITOR_OUTPUT"),"w");

        void *handle = dlopen("libc.so.6", RTLD_LAZY);
        if(handle != NULL)
        {
                origin_getuid = dlsym(handle,"getuid");
                origin_write = dlsym(handle,"write");
        }
}

static __attribute__((destructor)) void afterMain()
{
        fclose(logOutput);
}

#define log(format,...) \
        fprintf(logOutput,"[monitor] " format "\n", ##__VA_ARGS__); 

void getFileNameByFd(int fd,char fileName[])
{
	// dir use dirfd
	int bufSize = 1024;
	char procLink[bufSize];
	ssize_t size;	

	sprintf(procLink,"/proc/self/fd/%d",fd);
	size = readlink(procLink, fileName, bufSize);
	fileName[size] = '\0';		
}

uid_t getuid(void)
{
        log("getuid(%s) = %d","void",origin_getuid());
        return origin_getuid();
}

ssize_t write(int fd,const void *buf,size_t count)
{
       int result = origin_write(fd,buf,count);
       log("write(%d,%s,%d) = %d",fd,buf,count,result);
       return result;
}

