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

#define BUF_SIZE 512
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
	// dir use dirfd
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
        log("getuid(%s) = %d","void",origin_getuid());
        return origin_getuid();
}

ssize_t write(int fd,const void *buf,size_t count)
{
       int result = origin_write(fd,buf,count);
       char fileName[BUF_SIZE];
       getFileNameByFd(fd,fileName);

       if(fileName)
       {
            log("write(\"%s\",%s,%zd) = %zd",fileName,buf,count,result);
       }
       else
       {
            log("write(%d,%s,%zd) = %zd",fd,buf,count,result);
       }

       return result;
}

int closedir(DIR *dirp)
{
    int result = origin_closedir(dirp);

    int dirFd = dirfd(dirp);
    char dirName[BUF_SIZE];
    getFileNameByFd(dirfd,dirName);

    log("closedir(\"%s\") = %d",dirName,result);

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

        log("fdopendir(%d) = \"%s\"",fd,dirName);
    }
    else
    {
        log("fdopendir(%d) = (nil)",fd);
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

        log("fdopendir(\"%s\") = \"%s\"",name,dirName);
    }
    else
    {
        log("fdopendir(\"%s\") = (nil)",name);
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

        log("readdir(\"%s\") = (inode = %zu,dir Name = \"%s\")",dirName,inode,dName);
    }
    else
    {
        log("readdir(\"%s\") = (nil)",dirName);
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

    log("readdir_r(\"%s\",(inode = %zu , dir Name = \"%s\"), %p) = %d", dirName, inode, dName,result,resultValue);

    return resultValue;
}

void rewinddir(DIR *dirp)
{
    origin_rewinddir(dirp);

    int dirFd = dirfd(dirp);
    char dirName[BUF_SIZE];
    getFileNameByFd(dirFd,dirName);

    log("rewinddir(\"%s\")",dirName);
}


void seekdir(DIR *dirp, long loc)
{
    origin_seekdir(dirp,loc);

    int dirFd = dirfd(dirp);
    char dirName[BUF_SIZE];
    getFileNameByFd(dirFd,dirName);

    log("seekdir(\"%s\", %ld)",dirName,loc);
}

long telldir(DIR *dirp)
{
    long result = origin_telldir(dirp);

    int dirFd = dirfd(dirp);
    char dirName[BUF_SIZE];
    getFileNameByFd(dirFd,dirName);

    log("telldir(\"%s\") = %ld", dirName, result);

    return result;
}
int creat(const char *pathname, mode_t mode)
{
    int result = origin_creat(pathname, mode);

    log("creat(\"%s\", %o) = %d",pathname, mode, result);

    return result;
}


/*
int open(const char *pathname, int flags, ...)
{
  int i;
  va_list vl;
  va_start(vl,pathname);
  for (i=0;i<n;i++)
  {
    val=va_arg(vl,double);
    printf (" [%.2f]",val);
  }
  va_end(vl);
  printf ("\n");
}
*/


int remove(const char *pathname)
{
    int result = origin_remove(pathname);

    log("remove(\"%s\") = %d", pathname, result);

    return result;
}

int rename(const char *oldpath, const char *newpath)
{
    int result = origin_rename(oldpath, newpath);
    log("rename(\"%s\", \"%s\") = %d", oldpath, newpath, result);

    return result;
}

void setbuf(FILE *stream, char *buf)
{
    origin_setbuf(stream, buf);

    int fileFd = fileno(stream);
    char fileName[BUF_SIZE];
    getFileNameByFd(fileFd,fileName);

    log("setbuf(\"%s\", \"%s\")", fileName, buf);
}

int setvbuf(FILE *stream, char *buf, int mode, size_t size)
{
    int result = origin_setvbuf(stream, buf, mode, size);

    int fileFd = fileno(stream);
    char fileName[BUF_SIZE];
    getFileNameByFd(fileFd,fileName);

    log("setvbuf(\"%s\", \"%s\", %d, %zd) = %d", fileName, buf, mode, size, result);

    return result;
}

char* tempnam(const char *dir, const char *pfx)
{
    char *result = origin_tempnam(dir, pfx);

    if(result)
    {
        log("tempnam(\"%s\", \"%s\") = \"%s\"", dir, pfx, result);
    }
    else
    {
        log("tempnam(\"%s\", \"%s\") = (nil)", dir, pfx);
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

         log("tmpfile(void) = \"%s\"", fileName);
    }
    else
    {
        log("tmpfile(void) = (nil)");
    }
    
    return result;
}


char* tmpnam(char *s)
{
    char *result = origin_tmpnam(s);

    if(result)
    {
        log("tmpnam(\"%s\") = \"%s\"", s, result);
    }
    else
    {
        log("tmpnam(\"%s\") = (nil)", s);
    }

    return result;
}

void exit(int status)
{
    
    log("exit(%d)", status);

    origin_exit(status);
}

char* mkdtemp(char *_template)
{
    char *result = origin_mkdtemp(_template);

    if(result)
    {
        log("mkdtemp(\"%s\") = \"%s\"", _template, result);
    }
    else
    {
        log("mkdtemp(\"%s\") = (nil)", _template);
    }

    return result;
}

int mkstemp(char *_template)
{
    int result = origin_mkstemp(_template);

    log("mkdtemp(\"%s\") = %d", _template, result);

    return result;
}


int putenv(char *string)
{
    int result = origin_putenv(string);

    log("putenv(\"%s\") = %d", string, result);

    return result;
}

int rand(void)
{
    int result = origin_rand();

    log("rand(void) = %d", result);

    return result;
}

int rand_r(unsigned int *seedp)
{
    int result = origin_rand_r(seedp);

    log("rand_r(%u) = %d", *seedp, result);

    return result;
}

int setenv(const char *name, const char *value, int overwrite)
{
    int result = origin_setenv(name, value, overwrite);

    log("setenv(\"%s\", \"%s\", %d) = %d", name, value, overwrite, result);

    return result;
}

void srand(unsigned int seed)
{
    origin_srand(seed);

    log("srand(%u)", seed);
}

int system(const char *command)
{
    int result = origin_system(command);

    log("system(\"%s\") = %d", command, result);

    return result;
}

int chdir(const char *path)
{
    int result = origin_chdir(path);

    log("chdir(\"%s\") = %d", path, result);

    return result;
}

int chown(const char *path, uid_t owner, gid_t group)
{
    int result = origin_chown(path, owner, group);

    char userName[BUF_SIZE];
    char groupName[BUF_SIZE];

    getUsernameByUid(owner, userName);
    getGroupnameByGid(group, groupName);

    log("chown(\"%s\", \"%s\", \"%s\") = %d", path, userName, groupName, result);

    return result;
}

int close(int fd)
{
    int result = origin_close(fd);

    char fileName[BUF_SIZE];
    getFileNameByFd(fd, fileName);

    log("close(\"%s\") = %d", fileName, result);

    return result;
}

int dup(int oldfd)
{
    int result = origin_dup(oldfd);

    char fileName[BUF_SIZE];
    getFileNameByFd(oldfd, fileName); 
    
    log("dup(\"%s\") = %d", fileName, result);

    return result;   
}

int dup2(int oldfd, int newfd)
{
    int result = origin_dup2(oldfd, newfd);

    char oldFileName[BUF_SIZE];
    getFileNameByFd(oldfd, oldFileName); 
    char newFileName[BUF_SIZE];
    getFileNameByFd(newfd, newFileName);

    log("dup2(\"%s\", \"%s\") = %d", oldFileName, newFileName, result);

    return result;
}

void _exit(int status)
{

    log("_exit(%d)", status);
    
    fflush(outpusDes);

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
    argv[0] = arg;
    va_start(v1,arg);
    for(i = 1 ; i < argc + 1 ; i ++)
    {
        argv[i] = va_arg(v1,char*);
    }
    va_end(v1);

    log("execl(\"%s\", argv list pointer = %p)", path, arg);

    fflush(outpusDes);
    unsetenv("LD_PRELOAD");

    int result = origin_execv(path, argv);
    log("execl return = %d", result);

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
    argv[0] = arg;
    va_start(v1,arg);
    for(i = 1 ; i < argc + 1 ; i ++)
    {
        argv[i] = va_arg(v1,char*);
    }

    char **envp = va_arg(v1,char **);
    va_end(v1);

    log("execle(\"%s\", argv list pointer = %p, envp array pointer = %p)",path, arg, envp);

    fflush(outpusDes);
    unsetenv("LD_PRELOAD");

    int result = origin_execve(path, argv, envp);
    log("execle return = %d", result);

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
    argv[0] = arg;
    va_start(v1,arg);
    for(i = 1 ; i < argc + 1 ; i ++)
    {
        argv[i] = va_arg(v1,char*);
    }
    va_end(v1);

    log("execlp(\"%s\", argv list pointer = %p)", file, arg);

    fflush(outpusDes);
    unsetenv("LD_PRELOAD");

    int result = origin_execvp(file, argv);
    log("execlp return = %d", result);

    return result;


}

int execv(const char *path, char *const argv[])
{
    log("execv(\"%s\", argv array pointer = %p)", path, argv);

    fflush(outpusDes);
    unsetenv("LD_PRELOAD");

    int result = origin_execv(path,argv);
    log("execv return = %d", result);

    return result;

}
int execve(const char *path, char *const argv[], char *const envp[])
{
    log("execve(\"%s\", argv array pointer = %p, envp array pointer = %p)", path, argv, envp);

    fflush(outpusDes);
    unsetenv("LD_PRELOAD");

    int result = origin_execve(path,argv,envp);
    log("execve return = %d", result);

    return result;
}

int execvp(const char *file, char *const argv[])
{
    log("execvp(\"%s\", argv array pointer = %p)", file, argv);

    fflush(outpusDes);
    unsetenv("LD_PRELOAD");

    int result = origin_execvp(file,argv);
    log("execvp return = %d", result);

    return result;
}


int fchdir(int fd)
{
    int result = origin_fchdir(fd);

    char fileName[BUF_SIZE];
    getFileNameByFd(fd, fileName); 

    log("fchdir(\"%s\") = %d", fileName, result);

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

    log("fchown(\"%s\", \"%s\", \"%s\") = %d", fileName, userName, groupName);

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
        log("fork(void) = %d", result);
    }  

    return result;
}

int fsync(int fd)
{
    int result = origin_fsync(fd);

    char fileName[BUF_SIZE];
    getFileNameByFd(fd, fileName); 

    log("fsync(\"%s\") = %d", fileName, result);

    return result;    
}

int ftruncate(int fd, off_t length)
{
    int result = origin_ftruncate(fd, length);

    char fileName[BUF_SIZE];
    getFileNameByFd(fd, fileName); 

    log("ftruncate(\"%s\", %d) = %d", fileName, length, result);

    return result;
}

char* getcwd(char *buf, size_t size)
{
    char *result = origin_getcwd(buf, size);

    if(result)
    {
        log("getcwd(\"%s\", %zu) = \"%s\"", buf, size, result);
    }
    else
    {
        log("getcwd(\"%s\", %zu) = (nil)", buf, size);
    }

    return result;
}

gid_t getegid(void)
{
    gid_t result = origin_getegid();

    char groupName[BUF_SIZE];
    getGroupnameByGid(result, groupName);

    log("getegid(void) = (egid = %zd, gName = %s)",result, groupName);

    return result;
}

uid_t geteuid(void)
{
    uid_t result = origin_geteuid();

    char userName[BUF_SIZE];
    getUsernameByUid(result, userName); 

    log("geteuid(void) = (euid = %zd, uName = %s)",result, userName);

    return result;       
}

gid_t getgid(void)
{
    gid_t result = origin_getgid();

    char groupName[BUF_SIZE];
    getGroupnameByGid(result, groupName);

    log("getgid(void) = (egid = %zd, gName = %s)",result, groupName);

    return result;
}

int link(const char *oldpath, const char *newpath)
{
    int result = origin_link(oldpath, newpath);

    log("link(\"%s\", \"%s\") = %d", oldpath, newpath, result);

    return result;
}


int pipe(int pipefd[2])
{
    int result = origin_pipe(pipefd);

    log("pipe((pipe in fd = %d, pipe out fd = %d)) = %d", pipefd[1], pipefd[0], result);

    return result;
}

ssize_t pread(int fd, void *buf, size_t count, off_t offset)
{
    ssize_t result = origin_pread(fd, buf, count, offset);

    char fileName[BUF_SIZE];
    getFileNameByFd(fd, fileName);

    log("pread(\"%s\", %p, %zu, %d) = %zu", fileName, buf, count, offset, result);

    return result;

}

ssize_t pwrite(int fd, const void *buf, size_t count, off_t offset)
{
    ssize_t result = origin_pwrite(fd, buf, count, offset);

    char fileName[BUF_SIZE];
    getFileNameByFd(fd, fileName);

    log("pwrite(\"%s\", %p, %zu, %d) = %zu", fileName, buf, count, offset, result);

    return result;
}

ssize_t read(int fd, void *buf, size_t count)
{
    ssize_t result = origin_read(fd, buf, count);

    char fileName[BUF_SIZE];
    getFileNameByFd(fd, fileName);

    log("read(\"%s\", %p, %zu) = %zu", fileName, buf, count, result);

    return result;
}

ssize_t readlink(const char *path, char *buf, size_t bufsiz)
{
    ssize_t result = origin_readlink(path, buf, bufsiz);

    log("readlink(\"%s\", \"%s\", %zu) = %zu", path, buf, bufsiz, result);

    return result;
}

int rmdir(const char *pathname)
{
    int result = origin_rmdir(pathname);

    log("rmdir(\"%d\") = %d", pathname, result);

    return result;
}

int setegid(gid_t egid)
{
    int result = origin_setegid(egid);

    char groupName[BUF_SIZE];
    getGroupnameByGid(egid, groupName);

    log("setegid((egid = %d, gName = %s)) = %d", egid, groupName, result);

    return result;
}

int seteuid(uid_t euid)
{
    int result = origin_seteuid(euid);

    char userName[BUF_SIZE];
    getUsernameByUid(euid, userName);

    log("seteuid((euid = %d, uName = %s)) = %d", euid, userName, result);

    return result;
}

int setgid(gid_t gid)
{
    int result = origin_setgid(gid);

    char groupName[BUF_SIZE];
    getGroupnameByGid(gid, groupName);

    log("setgid((gid = %d, gName = %s)) = %d", gid, groupName, result);

    return result;
}

int setuid(uid_t uid)
{
    int result = origin_setuid(uid);

    char userName[BUF_SIZE];
    getUsernameByUid(uid, userName);

    log("setuid((uid = %d, uName = %s)) = %d", uid, userName, result);

    return result;
}

unsigned int sleep(unsigned int seconds)
{
    unsigned int result = origin_sleep(seconds);

    log("sleep(%u) = %u", seconds, result);

    return result;
}

int symlink(const char *oldpath, const char *newpath)
{
    int result = origin_symlink(oldpath, newpath);

    log("symlink(\"%s\", \"%s\") = %d", oldpath, newpath, result);

    return result;
}

int unlink(const char *path)
{
    int result = origin_unlink(path);

    log("unlink(\"%s\") = %d", path, result);

    return result;
}

int chmod(const char *path, mode_t mode)
{
    int result = origin_chmod(path, mode);

    log("chmod(\"%s\", %o) = %d", path, mode, result);

    return result;
}

int fchmod(int fd, mode_t mode)
{
    int result = origin_fchmod(fd, mode);

    char fileName[BUF_SIZE];
    getFileNameByFd(fd, fileName);

    log("fchmod(\"%s\", %o) = %d", fileName, mode, result);

    return result;
}


int mkdir(const char *pathname, mode_t mode)
{
    int result = origin_mkdir(pathname,mode);

    log("mkdir(\"%s\", %o) = %d", pathname, mode, result);

    return result;
}

int mkfifo(const char *pathname, mode_t mode)
{
    int result = origin_mkfifo(pathname, mode);

    log("mkfifo(\"%s\", %o) = %d", pathname, mode, result);

    return result;
}

mode_t umask(mode_t mask)
{
    mode_t result = origin_umask(mask);

    log("umask(%o) = %o", mask, result);

    return result;
}


char* getenv(const char *name)
{
    char *result = origin_getenv(name);
    if(result)
    {
        log("getenv(\"%s\") = %s",name,result);
    }
    else
    {
        log("getenv(\"%s\") = \'(null)\'",name);
    }

    return result;
}

int __xstat(int ver, const char *path, struct stat *buf)
{
    int result = origin___xstat(ver, path,buf);

    ino_t     ino = buf.st_ino;         
    mode_t    mode = buf.st_mode;
    off_t     size = buf.st_size; 

    log("__xstat(%d, %s, (inode = %ju , file type and mode = %o , file size = %jd)) = %d", ver, path, (uintmax_t)ino, mode, (intmax_t)size);

    return result;
}

int __fxstat(int ver, int fildes, struct stat *buf)
{
    int result = origin___fxstat(ver, fildes,buf);

    ino_t     ino = buf.st_ino;         
    mode_t    mode = buf.st_mode;
    off_t     size = buf.st_size; 

    log("__fxstat(%d, %d, (inode = %ju , file type and mode = %o , file size = %jd)) = %d", ver, fildes, (uintmax_t)ino, mode, (intmax_t)size);

    return result;
}

int __lxstat(int ver, const char *path, struct stat *buf)
{
    int result = origin___lxstat(ver, path,buf);

    ino_t     ino = buf.st_ino;         
    mode_t    mode = buf.st_mode;
    off_t     size = buf.st_size; 

    log("__lxstat(%d, %s, (inode = %ju , file type and mode = %o , file size = %jd)) = %d", ver, path, (uintmax_t)ino, mode, (intmax_t)size);

    return result;
}


