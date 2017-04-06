#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <dlfcn.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdarg.h>
#include <fcntl.h>

#define BUF_SIZE 512
#define log(format,...) \
        fprintf(logOutput,"[monitor] " format , ##__VA_ARGS__); 
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
static int (*origin_open)(const char *pathname, int flags) = NULL;
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
static int (*origin_execve)(const char *path, char *const argv[], char *const envp[]) = NULL;
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
static unsigned int (*origin_sleep)(unsigned int seconds) = NULL;
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
        // Bind to origin lib .
        void *handle = dlopen("libc.so.6", RTLD_LAZY);
        if(handle != NULL)
        {
                bindOrigin(getuid);
                bindOrigin(write);
                bindOrigin(fdopendir);
                bindOrigin(opendir);
                bindOrigin(readdir);
                bindOrigin(readdir_r);
                bindOrigin(closedir);
                bindOrigin(rewinddir);
                bindOrigin(seekdir);
                bindOrigin(telldir);
                bindOrigin(open);
                bindOrigin(remove);
                bindOrigin(rename);
                bindOrigin(setbuf);
                bindOrigin(setvbuf);
                bindOrigin(tempnam);
                bindOrigin(tmpfile);
                bindOrigin(open);
                bindOrigin(exit);
                bindOrigin(getenv);
                bindOrigin(mkdtemp);
                bindOrigin(mkstemp);
                bindOrigin(putenv);
                bindOrigin(rand);
                bindOrigin(srand);
                bindOrigin(system);
                bindOrigin(chdir);
                bindOrigin(chown);
                bindOrigin(close);
                bindOrigin(dup);
                bindOrigin(dup2);
                bindOrigin(execl);
                bindOrigin(execle);
                bindOrigin(execlp);
                bindOrigin(execv);
                bindOrigin(execve);
                bindOrigin(execvp);
                bindOrigin(fchdir);
                bindOrigin(fork);
                bindOrigin(fsync);
                bindOrigin(ftruncate);
                bindOrigin(getcwd);
                bindOrigin(getegid);
                bindOrigin(geteuid);
                bindOrigin(getgid);
                bindOrigin(link);
                bindOrigin(pipe);
                bindOrigin(pread);
                bindOrigin(pwrite);
                bindOrigin(read);
                bindOrigin(readlink);
                bindOrigin(rmdir);
                bindOrigin(seteuid);
                bindOrigin(setgid);
                bindOrigin(setuid);
                bindOrigin(sleep);
                bindOrigin(symlink);
                bindOrigin(unlink);
                bindOrigin(fchmod);
                bindOrigin(mkdir);
                bindOrigin(mkfifo);
                bindOrigin(umask);
                bindOrigin(__lxstat);
                bindOrigin(__xstat);
                bindOrigin(__fxstat);
        }

        // Set log output .
        char *outpusDes = origin_getenv("MONITOR_OUTPUT");
        if(outpusDes)
        {
            if(strcmp(outpusDes,"stderr") == 0)
                logOutput = stderr;
            else
                logOutput = fopen(origin_getenv("MONITOR_OUTPUT"),"w");
        }
        else
        {
            // default to stderr .
            logOutput = stderr;
        }
        
}

static __attribute__((destructor)) void afterMain()
{
        fclose(logOutput);
}

void getFileNameByFd(int fd,char fileName[])
{
	char procLink[BUF_SIZE];
	ssize_t size;	

	sprintf(procLink,"/proc/self/fd/%d",fd);
	size = origin_readlink(procLink, fileName, BUF_SIZE);
	fileName[size] = '\0';		
}

void getUsernameByUid(uid_t uid,char userName[])
{
    struct passwd *user;
    user = getpwuid(uid);
    userName = user->pw_name;
}

void getGroupnameByGid(gid_t gid, char groupName[])
{
    struct group *grp;
    grp = getgrgid(gid);
    groupName = grp->gr_name;
}

uid_t getuid(void)
{
        log("getuid(%s) = %d\n","void",origin_getuid());
        return origin_getuid();
}

ssize_t write(int fd,const void *buf,size_t count)
{
       int result = origin_write(fd,buf,count);
       char fileName[BUF_SIZE];
       getFileNameByFd(fd,fileName);

       if(fileName)
       {
            log("write(\"%s\",%s,%zu) = %d\n",fileName,(char*)buf,count,result);
       }
       else
       {
            log("write(%d,%s,%zu) = %d\n",fd,(char*)buf,count,result);
       }

       return result;
}

int closedir(DIR *dirp)
{
    int result = origin_closedir(dirp);

    int dirFd = dirfd(dirp);
    char dirName[BUF_SIZE];
    getFileNameByFd(dirFd,dirName);

    log("closedir(\"%s\") = %d\n",dirName,result);

    return result;
}

DIR* fdopendir(int fd)
{
    DIR* result = origin_fdopendir(fd);
    
    if(result)
    {
        int dirFd = dirfd(result);
        char dirName[BUF_SIZE];
        getFileNameByFd(dirFd,dirName);

        log("fdopendir(%d) = \"%s\"\n",fd,dirName);
    }
    else
    {
        log("fdopendir(%d) = (nil)\n",fd);
    }

    return result;
}

DIR* opendir(const char *name)
{
    DIR* result = origin_opendir(name);

    if(result)
    {
        int dirFd = dirfd(result);
        char dirName[BUF_SIZE];
        getFileNameByFd(dirFd,dirName);

        log("fdopendir(\"%s\") = \"%s\"\n",name,dirName);
    }
    else
    {
        log("fdopendir(\"%s\") = (nil)\n",name);
    }

    return result;
}

struct dirent* readdir(DIR *dirp)
{
    struct dirent *result = origin_readdir(dirp);
    int dirFd = dirfd(dirp);
    char dirName[BUF_SIZE];
    getFileNameByFd(dirFd,dirName);

    if(result)
    {
        ino_t inode = result->d_ino;
        char *dName = result->d_name;

        log("readdir(\"%s\") = (inode = %zu,dir Name = \"%s\")\n",dirName,inode,dName);
    }
    else
    {
        log("readdir(\"%s\") = (nil)\n",dirName);
    }

    return result;
}

int readdir_r(DIR *dirp, struct dirent *entry, struct dirent **result)
{
    int resultValue = origin_readdir_r(dirp,entry,result);

    int dirFd = dirfd(dirp);
    char dirName[BUF_SIZE];
    getFileNameByFd(dirFd,dirName);

    ino_t inode = entry->d_ino;
    char *dName = entry->d_name;

    log("readdir_r(\"%s\",(inode = %zu , dir Name = \"%s\"), %p) = %d\n", dirName, inode, dName,result,resultValue);

    return resultValue;
}

void rewinddir(DIR *dirp)
{
    origin_rewinddir(dirp);

    int dirFd = dirfd(dirp);
    char dirName[BUF_SIZE];
    getFileNameByFd(dirFd,dirName);

    log("rewinddir(\"%s\")\n",dirName);
}


void seekdir(DIR *dirp, long loc)
{
    origin_seekdir(dirp,loc);

    int dirFd = dirfd(dirp);
    char dirName[BUF_SIZE];
    getFileNameByFd(dirFd,dirName);

    log("seekdir(\"%s\", %ld)\n",dirName,loc);
}

long telldir(DIR *dirp)
{
    long result = origin_telldir(dirp);

    int dirFd = dirfd(dirp);
    char dirName[BUF_SIZE];
    getFileNameByFd(dirFd,dirName);

    log("telldir(\"%s\") = %ld\n", dirName, result);

    return result;
}
int creat(const char *pathname, mode_t mode)
{
    int result = origin_creat(pathname, mode);

    log("creat(\"%s\", %o) = %d\n",pathname, mode, result);

    return result;
}

int open(const char *pathname, int flags)
{
    int result = origin_open(pathname, flags);

    log("open(\"%s\", %d) = %d\n", pathname, flags, result);

    return result;  
}

int remove(const char *pathname)
{
    int result = origin_remove(pathname);

    log("remove(\"%s\") = %d\n", pathname, result);

    return result;
}

int rename(const char *oldpath, const char *newpath)
{
    int result = origin_rename(oldpath, newpath);
    log("rename(\"%s\", \"%s\") = %d\n", oldpath, newpath, result);

    return result;
}

void setbuf(FILE *stream, char *buf)
{
    origin_setbuf(stream, buf);

    int fileFd = fileno(stream);
    char fileName[BUF_SIZE];
    getFileNameByFd(fileFd,fileName);

    log("setbuf(\"%s\", \"%s\")\n", fileName, buf);
}

int setvbuf(FILE *stream, char *buf, int mode, size_t size)
{
    int result = origin_setvbuf(stream, buf, mode, size);

    int fileFd = fileno(stream);
    char fileName[BUF_SIZE];
    getFileNameByFd(fileFd,fileName);

    log("setvbuf(\"%s\", \"%s\", %d, %zd) = %d\n", fileName, buf, mode, size, result);

    return result;
}

char* tempnam(const char *dir, const char *pfx)
{
    char *result = origin_tempnam(dir, pfx);

    if(result)
    {
        log("tempnam(\"%s\", \"%s\") = \"%s\"\n", dir, pfx, result);
    }
    else
    {
        log("tempnam(\"%s\", \"%s\") = (nil)\n", dir, pfx);
    }

    return result;
}


FILE* tmpfile(void)
{
    FILE *result = origin_tmpfile();

    if(result)
    {
        int fileFd = fileno(result);
        char fileName[BUF_SIZE];
        getFileNameByFd(fileFd,fileName);

         log("tmpfile(void) = \"%s\"\n", fileName);
    }
    else
    {
        log("tmpfile(void) = (nil)\n");
    }
    
    return result;
}


char* tmpnam(char *s)
{
    char *result = origin_tmpnam(s);

    if(result)
    {
        log("tmpnam(\"%s\") = \"%s\"\n", s, result);
    }
    else
    {
        log("tmpnam(\"%s\") = (nil)\n", s);
    }

    return result;
}

void exit(int status)
{
    
    log("exit(%d)\n", status);

    origin_exit(status);
}

char* mkdtemp(char *_template)
{
    char *result = origin_mkdtemp(_template);

    if(result)
    {
        log("mkdtemp(\"%s\") = \"%s\"\n", _template, result);
    }
    else
    {
        log("mkdtemp(\"%s\") = (nil)\n", _template);
    }

    return result;
}

int mkstemp(char *_template)
{
    int result = origin_mkstemp(_template);

    log("mkdtemp(\"%s\") = %d\n", _template, result);

    return result;
}


int putenv(char *string)
{
    int result = origin_putenv(string);

    log("putenv(\"%s\") = %d\n", string, result);

    return result;
}

int rand(void)
{
    int result = origin_rand();

    log("rand(void) = %d\n", result);

    return result;
}

int rand_r(unsigned int *seedp)
{
    int result = origin_rand_r(seedp);

    log("rand_r(%u) = %d\n", *seedp, result);

    return result;
}

int setenv(const char *name, const char *value, int overwrite)
{
    int result = origin_setenv(name, value, overwrite);

    log("setenv(\"%s\", \"%s\", %d) = %d\n", name, value, overwrite, result);

    return result;
}

void srand(unsigned int seed)
{
    origin_srand(seed);

    log("srand(%u)\n", seed);
}

int system(const char *command)
{
    int result = origin_system(command);

    log("system(\"%s\") = %d\n", command, result);

    return result;
}

int chdir(const char *path)
{
    int result = origin_chdir(path);

    log("chdir(\"%s\") = %d\n", path, result);

    return result;
}

int chown(const char *path, uid_t owner, gid_t group)
{
    int result = origin_chown(path, owner, group);

    char userName[BUF_SIZE];
    char groupName[BUF_SIZE];

    getUsernameByUid(owner, userName);
    getGroupnameByGid(group, groupName);

    log("chown(\"%s\", \"%s\", \"%s\") = %d\n", path, userName, groupName, result);

    return result;
}

int close(int fd)
{
    int result = origin_close(fd);

    char fileName[BUF_SIZE];
    getFileNameByFd(fd, fileName);

    log("close(\"%s\") = %d\n", fileName, result);

    return result;
}

int dup(int oldfd)
{
    int result = origin_dup(oldfd);
    
    log("dup(%d) = %d\n", oldfd, result);

    return result;   
}

int dup2(int oldfd, int newfd)
{
    int result = origin_dup2(oldfd, newfd);

    log("dup2(%d, %d) = %d\n", oldfd, newfd, result);

    return result;
}

void _exit(int status)
{

    log("_exit(%d)\n", status);
    
    fflush(logOutput);

    origin__exit(status);
}


// exec() : 當exec後的program有error時才會返回原program .
int execl(const char *path, const char *arg, ...)
{
    // Get arguments amount .
    va_list v1;
    va_start(v1,arg);
    int argc = 0 ;
    char *str ;
    do{
        str = va_arg(v1,char*);
        argc ++ ;
    } while( str != NULL);
    va_end(v1);

    // Create argv's array .
    char *argv[argc + 1] ;
    int i;
    argv[0] = (char*)arg;
    va_start(v1,arg);
    for(i = 1 ; i < argc + 1 ; i ++)
    {
        argv[i] = va_arg(v1,char*);
    }
    va_end(v1);

    log("execl(\"%s\", argv = %s", path, argv[0]);

    for(i = 1 ; i < argc ; i ++)
    {
        log(", %s",argv[i]);
    }

    log(")\n");

    fflush(logOutput);
    unsetenv("LD_PRELOAD");

    int result = origin_execv(path, argv);
    log("execl return = %d\n", result);

    return result;
}

int execle(const char *path, const char *arg, ...)
{
    // Get arguments amount .
    va_list v1;
    va_start(v1,arg);
    int argc = 0 ;
    char *str ;
    do{
        str = va_arg(v1,char*);
        argc ++ ;
    } while( str != NULL);
    va_end(v1);

    // Create argv's array .
    char *argv[argc + 1] ;
    int i;
    argv[0] = (char*)arg;
    va_start(v1,arg);
    for(i = 1 ; i < argc + 1 ; i ++)
    {
        argv[i] = va_arg(v1,char*);
    }

    char **envp = va_arg(v1,char **);
    va_end(v1);

    log("execle(\"%s\", argv = %s", path, argv[0]);

    for(i = 1 ; i < argc ; i ++)
    {
        log(", %s",argv[i]);
    }

    log(", envp = %s", envp[0]);
    for(i = 1 ; envp[i] != NULL ; i ++)
    {
        log(", %s",envp[i]);
    }
    log(")\n");


    fflush(logOutput);
    unsetenv("LD_PRELOAD");

    int result = origin_execve(path, argv, envp);
    log("execle return = %d\n", result);

    return result;    
}

int execlp(const char *file, const char *arg, ...)
{
    // Get arguments amount .
    va_list v1;
    va_start(v1,arg);
    int argc = 0 ;
    char *str ;
    do{
        str = va_arg(v1,char*);
        argc ++ ;
    } while( str != NULL);
    va_end(v1);

    // Create argv's array .
    char *argv[argc + 1] ;
    int i;
    argv[0] = (char*)arg;
    va_start(v1,arg);
    for(i = 1 ; i < argc + 1 ; i ++)
    {
        argv[i] = va_arg(v1,char*);
    }
    va_end(v1);

    log("execlp(\"%s\", argv = %s", file, argv[0]);

    for(i = 1 ; i < argc ; i ++)
    {
        log(", %s",argv[i]);
    }

    log(")\n");

    fflush(logOutput);
    unsetenv("LD_PRELOAD");

    int result = origin_execvp(file, argv);
    log("execlp return = %d\n", result);

    return result;


}

int execv(const char *path, char *const argv[])
{
    
    log("execv(\"%s\", argv = %s", path, argv[0]);

    int i;
    for(i = 1; argv[i] != NULL; i++) 
    {
        log(", %s",argv[i]);
    }
    log(")\n");

    fflush(logOutput);
    unsetenv("LD_PRELOAD");

    int result = origin_execv(path,argv);
    log("execv return = %d\n", result);

    return result;

}
int execve(const char *path, char *const argv[], char *const envp[])
{

    log("execve(\"%s\", argv = %s", path, argv[0]);

    int i;
    for(i = 1; argv[i] != NULL; i++) 
    {
        log(", %s",argv[i]);
    }
    log(", envp = %s", envp[0]);
    for(i = 1 ; envp[i] != NULL ; i ++)
    {
        log(", %s",envp[i]);
    }
    log(")\n");

    fflush(logOutput);
    unsetenv("LD_PRELOAD");

    int result = origin_execve(path,argv,envp);
    log("execve return = %d\n", result);

    return result;
}

int execvp(const char *file, char *const argv[])
{

    log("execvp(\"%s\", argv = %s", file, argv[0]);

    int i;
    for(i = 1; argv[i] != NULL; i++) 
    {
        log(", %s",argv[i]);
    }
    log(")\n");

    fflush(logOutput);
    unsetenv("LD_PRELOAD");

    int result = origin_execvp(file,argv);
    log("execvp return = %d\n", result);

    return result;
}

int fchdir(int fd)
{
    int result = origin_fchdir(fd);

    char fileName[BUF_SIZE];
    getFileNameByFd(fd, fileName); 

    log("fchdir(\"%s\") = %d\n", fileName, result);

    return result;

}

int fchown(int fd, uid_t owner, gid_t group)
{
    int result = origin_fchown(fd, owner, group);

    char fileName[BUF_SIZE];
    getFileNameByFd(fd, fileName); 

    char userName[BUF_SIZE];
    char groupName[BUF_SIZE];
    getUsernameByUid(owner, userName);
    getGroupnameByGid(group, groupName);

    log("fchown(\"%s\", \"%s\", \"%s\") = %d\n", fileName, userName, groupName,result);

    return result;
}

pid_t fork(void)
{
    fflush(logOutput);

    int result = origin_fork();

    if(result == 0)
        fclose(logOutput);
    else if(result > 0)
    {
        log("fork(void) = %d\n", result);
    }  

    return result;
}

// 確保fd的資料一更新後馬上寫到硬碟上 .
int fsync(int fd)
{
    int result = origin_fsync(fd); 

    log("fsync(%d) = %d\n", fd, result);

    return result;    
}


// 指定的file大小改為参数length指定的大小 .
int ftruncate(int fd, off_t length)
{
    int result = origin_ftruncate(fd, length);

    char fileName[BUF_SIZE];
    getFileNameByFd(fd, fileName); 

    log("ftruncate(\"%s\", %ld) = %d\n", fileName, length, result);

    return result;
}

char* getcwd(char *buf, size_t size)
{
    char *result = origin_getcwd(buf, size);

    if(result)
    {
        log("getcwd(\"%s\", %zu) = \"%s\"\n", buf, size, result);
    }
    else
    {
        log("getcwd(\"%s\", %zu) = (nil)\n", buf, size);
    }

    return result;
}

gid_t getegid(void)
{
    gid_t result = origin_getegid();

    char groupName[BUF_SIZE];
    getGroupnameByGid(result, groupName);

    log("getegid(void) = (egid = %u, gName = %s)\n",result, groupName);

    return result;
}

uid_t geteuid(void)
{
    uid_t result = origin_geteuid();

    char userName[BUF_SIZE];
    getUsernameByUid(result, userName); 

    log("geteuid(void) = (euid = %u, uName = %s)\n",result, userName);

    return result;       
}

gid_t getgid(void)
{
    gid_t result = origin_getgid();

    char groupName[BUF_SIZE];
    getGroupnameByGid(result, groupName);

    log("getgid(void) = (egid = %u, gName = %s)\n",result, groupName);

    return result;
}

int link(const char *oldpath, const char *newpath)
{
    int result = origin_link(oldpath, newpath);

    log("link(\"%s\", \"%s\") = %d\n", oldpath, newpath, result);

    return result;
}


int pipe(int pipefd[2])
{
    int result = origin_pipe(pipefd);

    log("pipe((pipe in fd = %d, pipe out fd = %d)) = %d\n", pipefd[1], pipefd[0], result);

    return result;
}

// read from or write to a file descriptor at a given offset
ssize_t pread(int fd, void *buf, size_t count, off_t offset)
{
    ssize_t result = origin_pread(fd, buf, count, offset);

    char fileName[BUF_SIZE];
    getFileNameByFd(fd, fileName);

    if(fileName)
    {
        log("pread(\"%s\", %p, %zu, %ld) = %zu\n", fileName, buf, count, offset, result);
    }
    else
    {
        log("pread(%d, %p, %zu, %ld) = %zu\n", fd, buf, count, offset, result);
    }

    return result;
}

ssize_t pwrite(int fd, const void *buf, size_t count, off_t offset)
{
    ssize_t result = origin_pwrite(fd, buf, count, offset);

    char fileName[BUF_SIZE];
    getFileNameByFd(fd, fileName);

    if(fileName)
    {
        log("pwrite(\"%s\", %p, %zu, %ld) = %zu\n", fileName, buf, count, offset, result);
    }
    else
    {
        log("pwrite(%d, %p, %zu, %ld) = %zu\n", fd, buf, count, offset, result);
    }
    
    return result;
}

ssize_t read(int fd, void *buf, size_t count)
{
    ssize_t result = origin_read(fd, buf, count);

    char fileName[BUF_SIZE];
    getFileNameByFd(fd, fileName);

    if(fileName)
    {
        log("read(\"%s\",%s,%zd) = %zd\n",fileName,buf,count,result);
    }
    else
    {
        log("read(%d,%s,%zd) = %zd\n",fd,buf,count,result);
    }

    return result;
}

ssize_t readlink(const char *path, char *buf, size_t bufsiz)
{
    ssize_t result = origin_readlink(path, buf, bufsiz);

    log("readlink(\"%s\", \"%s\", %zu) = %zu\n", path, buf, bufsiz, result);

    return result;
}

int rmdir(const char *pathname)
{
    int result = origin_rmdir(pathname);

    log("rmdir(\"%s\") = %d\n", pathname, result);

    return result;
}

int setegid(gid_t egid)
{
    int result = origin_setegid(egid);

    char groupName[BUF_SIZE];
    getGroupnameByGid(egid, groupName);

    log("setegid((egid = %d, gName = %s)) = %d\n", egid, groupName, result);

    return result;
}

int seteuid(uid_t euid)
{
    int result = origin_seteuid(euid);

    char userName[BUF_SIZE];
    getUsernameByUid(euid, userName);

    log("seteuid((euid = %d, uName = %s)) = %d\n", euid, userName, result);

    return result;
}

int setgid(gid_t gid)
{
    int result = origin_setgid(gid);

    char groupName[BUF_SIZE];
    getGroupnameByGid(gid, groupName);

    log("setgid((gid = %d, gName = %s)) = %d\n", gid, groupName, result);

    return result;
}

int setuid(uid_t uid)
{
    int result = origin_setuid(uid);

    char userName[BUF_SIZE];
    getUsernameByUid(uid, userName);

    log("setuid((uid = %d, uName = %s)) = %d\n", uid, userName, result);

    return result;
}

unsigned int sleep(unsigned int seconds)
{
    unsigned int result = origin_sleep(seconds);

    log("sleep(%u) = %u\n", seconds, result);

    return result;
}

int symlink(const char *oldpath, const char *newpath)
{
    int result = origin_symlink(oldpath, newpath);

    log("symlink(\"%s\", \"%s\") = %d\n", oldpath, newpath, result);

    return result;
}

int unlink(const char *path)
{
    int result = origin_unlink(path);

    log("unlink(\"%s\") = %d\n", path, result);

    return result;
}

int chmod(const char *path, mode_t mode)
{
    int result = origin_chmod(path, mode);

    log("chmod(\"%s\", %o) = %d\n", path, mode, result);

    return result;
}

int fchmod(int fd, mode_t mode)
{
    int result = origin_fchmod(fd, mode);

    char fileName[BUF_SIZE];
    getFileNameByFd(fd, fileName);

    log("fchmod(\"%s\", %o) = %d\n", fileName, mode, result);

    return result;
}


int mkdir(const char *pathname, mode_t mode)
{
    int result = origin_mkdir(pathname,mode);

    log("mkdir(\"%s\", %o) = %d\n", pathname, mode, result);

    return result;
}

int mkfifo(const char *pathname, mode_t mode)
{
    int result = origin_mkfifo(pathname, mode);

    log("mkfifo(\"%s\", %o) = %d\n", pathname, mode, result);

    return result;
}

mode_t umask(mode_t mask)
{
    mode_t result = origin_umask(mask);

    log("umask(%o) = %o\n", mask, result);

    return result;
}


char* getenv(const char *name)
{
    char *result = origin_getenv(name);
    if(result)
    {
        log("getenv(\"%s\") = %s\n",name,result);
    }
    else
    {
        log("getenv(\"%s\") = \'(null)\'\n",name);
    }

    return result;
}

int __xstat(int ver, const char *path, struct stat *buf)
{
    int result = origin___xstat(ver, path,buf);

    ino_t     ino = buf->st_ino;         
    mode_t    mode = buf->st_mode;
    off_t     size = buf->st_size; 

    log("__xstat(%d, %s, (inode = %ju , file type and mode = %o , file size = %jd)) = %d\n", ver, path, ino, mode, size,result);

    return result;
}

int __fxstat(int ver, int fildes, struct stat *buf)
{
    int result = origin___fxstat(ver, fildes,buf);

    ino_t     ino = buf->st_ino;         
    mode_t    mode = buf->st_mode;
    off_t     size = buf->st_size; 

    log("__fxstat(%d, %d, (inode = %ju , file type and mode = %o , file size = %jd)) = %d\n", ver, fildes, ino, mode, size, result);

    return result;
}

int __lxstat(int ver, const char *path, struct stat *buf)
{
    int result = origin___lxstat(ver, path,buf);

    ino_t     ino = buf->st_ino;         
    mode_t    mode = buf->st_mode;
    off_t     size = buf->st_size; 

    log("__lxstat(%d, %s, (inode = %ju , file type and mode = %o , file size = %jd)) = %d\n", ver, path, ino, mode, size, result);

    return result;
}


