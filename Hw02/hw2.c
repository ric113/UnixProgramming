#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <dlfcn.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>

#define log(format,...) \
        fprintf(logOutput,"[monitor] " format "\n", ##__VA_ARGS__); 
#define bindOrigin(name) \
        origin_##name = dlsym(handle, #name);


FILE* logOutput;

static uid_t (*origin_getuid)(void) = NULL;
static ssize_t (*origin_write)(int fd,const void *buf,size_t count) = NULL;
static int (*origin_closedir)(DIR *dirp) = NULL;
static DIR* (*origin_fdopendir)(int fd) = NULL;
static DIR* (*origin_opendir)(const char *name) = NULL;
static struct dirent* (*origin_readdir)(DIR *dirp) = NULL;
static int (*origin_readdir_r)(DIR *dirp, struct dirent *entry, struct dirent **result) = NULL;
static void (*origin_rewinddir)(DIR *dirp) = NULL;
static void (*origin_seekdir)(DIR *dirp, long loc) = NULL;
static long (*origin_telldir)(DIR *dirp) = NULL;
static int (*origin_creat)(const char *pathname, mode_t mode) = NULL;
static int (*origin_open)(const char *pathname, int flags, ...) = NULL;
static int (*origin_remove)(const char *pathname) = NULL;
static int (*origin_rename)(const char *oldpath, const char *newpath) = NULL;
static void (*origin_setbuf)(FILE *stream, char *buf) = NULL;
static int (*origin_setvbuf)(FILE *stream, char *buf, int mode, size_t size) = NULL;
static char *(*origin_tempnam)(const char *dir, const char *pfx) = NULL;
static FILE *(*origin_tmpfile)(void) = NULL;
static char *(*origin_tmpnam)(char *s) = NULL;
static void (*origin_exit)(int status) = NULL;
static char *(*origin_getenv)(const char *name) = NULL;
static char *(*origin_mkdtemp)(char *_template) = NULL;
static int (*origin_mkstemp)(char *_template) = NULL;
static int (*origin_putenv)(char *string) = NULL;
static int (*origin_rand)(void) = NULL;
static int (*origin_rand_r)(unsigned int *seedp) = NULL;
static int (*origin_setenv)(const char *name, const char *value, int overwrite) = NULL;
static void (*origin_srand)(unsigned int seed) = NULL;
static int (*origin_system)(const char *command) = NULL;
static int (*origin_chdir)(const char *path) = NULL;
static int (*origin_chown)(const char *path, uid_t owner, gid_t group) = NULL;
static int (*origin_close)(int fd) = NULL;
static int (*origin_dup)(int oldfd) = NULL;
static int (*origin_dup2)(int oldfd, int newfd) = NULL;
static void (*origin__exit)(int status) = NULL;
static int (*origin_execl)(const char *path, const char *arg, ...) = NULL;
static int (*origin_execle)(const char *path, const char *arg, ...) = NULL;
static int (*origin_execlp)(const char *file, const char *arg, ...) = NULL;
static int (*origin_execv)(const char *path, char *const argv[]) = NULL;
static int (*origin_execve)(const char *filename, char *const argv[], char *const envp[]) = NULL;
static int (*origin_execvp)(const char *file, char *const argv[]) = NULL;
static int (*origin_fchdir)(int fd) = NULL;
static int (*origin_fchown)(int fd, uid_t owner, gid_t group) = NULL;
static pid_t (*origin_fork)(void) = NULL;
static int (*origin_fsync)(int fd) = NULL;
static int (*origin_ftruncate)(int fd, off_t length) = NULL;
static char *(*origin_getcwd)(char *buf, size_t size) = NULL;
static gid_t (*origin_getegid)(void) = NULL;
static uid_t (*origin_geteuid)(void) = NULL;
static gid_t (*origin_getgid)(void) = NULL;
static int (*origin_link)(const char *oldpath, const char *newpath) = NULL;
static int (*origin_pipe)(int pipefd[2]) = NULL;
static ssize_t (*origin_pread)(int fd, void *buf, size_t count, off_t offset) = NULL;
static ssize_t (*origin_pwrite)(int fd, const void *buf, size_t count, off_t offset) = NULL;
static ssize_t (*origin_read)(int fd, void *buf, size_t count) = NULL;
static ssize_t (*origin_readlink)(const char *path, char *buf, size_t bufsiz) = NULL;
static int (*origin_rmdir)(const char *pathname) = NULL;
static int (*origin_setegid)(gid_t egid) = NULL;
static int (*origin_seteuid)(uid_t euid) = NULL;
static int (*origin_setgid)(gid_t gid) = NULL;
static int (*origin_setuid)(uid_t uid) = NULL;
static unsigned int (*sleep)(unsigned int seconds) = NULL;
static int (*origin_symlink)(const char *oldpath, const char *newpath) = NULL;
static int (*origin_unlink)(const char *path) = NULL;
static int (*origin_chmod)(const char *path, mode_t mode) = NULL;
static int (*origin_fchmod)(int fd, mode_t mode) = NULL;
static int (*origin___fxstat)(int ver, int fildes, struct stat *buf) = NULL;
static int (*origin___lxstat)(int ver, const char *path, struct stat *buf) = NULL;
static int (*origin_mkdir)(const char *pathname, mode_t mode) = NULL;
static int (*origin_mkfifo)(const char *pathname, mode_t mode) = NULL;
static int (*origin___xstat)(int ver, const char *path, struct stat *buf) = NULL;
static mode_t (*origin_umask)(mode_t mask) = NULL;


static __attribute__((constructor)) void beforeMain()
{
        if(strcmp(getenv("MONITOR_OUTPUT"),"stderr") == 0)
                logOutput = stderr;
        else
                logOutput = fopen(getenv("MONITOR_OUTPUT"),"w");

        void *handle = dlopen("libc.so.6", RTLD_LAZY);
        if(handle != NULL)
        {
                bindOrigin(getuid);
                bindOrigin(write);
                /*
                bindOrigin(getuid);
                bindOrigin(getuid);
                bindOrigin(getuid);
                bindOrigin(getuid);
                bindOrigin(getuid);
                bindOrigin(getuid);
                bindOrigin(getuid);
                */
                
                
        }
}

static __attribute__((destructor)) void afterMain()
{
        fclose(logOutput);
}

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

