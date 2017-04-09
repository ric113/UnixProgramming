### Homework 02 - Implement a "library call monitor" (LCM) program

In this homework, we are going to practice library injection and API hijacking.We have to implement your LCM as a shared library and <br> inject the shared library into a process using using LD_PRELOAD.The result should be stored into a filename, e.g., "monitor.out".<br> We may write the output to a filename specified by an environment variable "MONITOR_OUTPUT". If the value of MONITOR_OUTPUT is stderr, output the messages to standart error instead of a file.<br><br>

參考上課PDF : [Process Environment](https://github.com/ric113/UnixProgramming/blob/master/Hw02/07-procenv.pdf)

#### Monitored library calls :<br>

closedir fdopendir opendir readdir readdir_r rewinddir seekdir telldir <br>
creat open remove rename setbuf setvbuf tempnam tmpfile <br>
tmpnam exit getenv mkdtemp mkstemp putenv rand rand_r <br> 
setenv srand system chdir chown close dup dup2 <br>
_exit execl execle execlp execv execve execvp fchdir <br>
fchown fork fsync ftruncate getcwd getegid geteuid <br>
getgid getuid link pipe pread pwrite read readlink <br>
rmdir setegid seteuid setgid setuid sleep symlink <br>
unlink write chmod fchmod fstat lstat mkdir mkfifo stat umask


#### Notes 

###### Marcos 
在「compile之前」, 會先對Marcos進行處理, 也就是字串取代 . <br>
可利用Marcos來 : <br>
	* 定義常數 , 函式 .
	* 進行一些環境的判斷 . e.g . #ifdef ...  

Reference : <br>

[Introdution Slide](https://www.slideshare.net/itembedded/c-15114885) 
[About '##' in Marcos(用在變數得取用In Marcos)](https://www.complete-concrete-concise.com/programming/c/preprocessor-the-token-pasting-operator)

###### \__attribute\__((constructor/destructor))
\__attribute\__ : 用來給函數加上一些屬性 . <br>
constructor/destructor就是分別運行在main()之前以及main之後的屬性 <br>

Reference : <br>

[Some example](http://www.cnblogs.com/respawn/archive/2012/07/09/2582078.html)

	
###### 擷取'數量不固定'的參數 - stdarg.h 

``` C
int execl(const char *path, const char *arg, ...)
	...
	va_list v1;				// 存放參數們的list .
	va_start(v1,arg);		// 將目前指針指向arg這個參數的下一個參數 , 也就是 "..." 的第一個參數 .
	char *str ;
	do{
	  	str = va_arg(v1,char*);	// 取出當下指針指向的value, 並將指針向下移一個 .
	        
	} while( str != NULL);
	va_end(v1);			// 關閉指針 .
	...
}
```

__注意__ : 在exec系列中, 當使用可變數量的參數時, 必須以NULL當作最後參數結尾 . <br>

Reference :<br>

[Example](http://www.cnblogs.com/haoyuanyuan/p/3221463.html)

###### \__VA_ARGS\__
[Eample](http://www.cash.idv.tw/wordpress/?p=1531)

