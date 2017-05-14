### Homework 03 - Simple Shell

In this homework, we are going to practice process relationships, descriptors, signals, and simple IPC with pipes. Please implement a simple shell that fulfills the following requirements:

* Execute a single command.
* Properly block or unblock signals.
* Replace standard input/output of a process using the redirection operators (< and >).
* Setup foreground process group and background process groups.
* Create pipeline for commands separated by the pipe operator (|), and put the commands into the same process group.
* Manipulate environment variables: provide commands like export and unset to add and remove environment variables, respectively.
* Expand of the * and ? operators.
* Job control: support process suspension using Ctrl-Z, and jobs, fg and bg command.

#### Sample Test

* Execute a single command:

```shell
myshell$ ls -la
``` 
* Properly block or unblock signals: Sending SIGINT or SIGQUIT should not terminate the current running shell:

``` shell
myshell$ sleep 10
```

* Replace standard output/input of a process:

``` shell
myshell$ ls -la > /tmp/x
myshell$ cat < /etc/passwd
```

* Setup foreground process group and background process groups:

``` shell
myshell$ less /etc/passwd
```

* Create pipelines:

``` shell
myshell$ cat /etc/passwd | cat | less
```

* Put processes into the same process group:

``` shell
myshell$ ps -o pid,sid,pgid,ppid,cmd | cat | cat | tr A-Z a-z
```

* Manipulate environment variables: You can use the relevant commands to add and remove environment variables and then use the printenv command to list all the available environment variables.

``` shell
myshell$ printenv // no env var yet .
myshell$ export TEST=test
myshell$ export TEST2=test2
myshell$ printenv TEST
TEST=test
myshell$ printenv
TEST=test
TEST2=test2
myshell$ unset TEST
myshell$ printenv 
TEST2=test2
```

* Expand * and ? characters:

``` shell
myshell$ ls *
```
* Job Control :

``` shell
myshell$ sleep 10 &
myshell$ fg
// sleep 10 to foreground, wait ...
myshell$ sleep 10
// Ctrl-Z, suspend sleep 10
myshell$ bg
// wake up sleep 10 and move sleep 10 run in background
```




#### 程式流程
* 1. 初始化Shell
	- 將某些signals block .
		- SIGTTOU : 當該Process在bg, 又嘗試寫入control terminal Output時會收到的訊號 .
		- SIGTSTP : Process suspend , Crtl-Z 時會收到的訊號 .
		- SIGTTIN : 當該Process在bg, 又嘗試讀取control terminal Input時會收到的訊號 .
		- SIGINT : Ctrl-C 時會收到的訊號 .
	- 設定SIGCHLD的Handler (本意是用來處理「background」job中porcess的exit status).

* 2. Print Prompt & Read Input Line .

* 3. 判斷是否要Run在背景, 設定isBackground flag .

* 4. Parse Inpt Line
	- 以 | 作為分割的對象 .
	- 把結果存在一個vector<string>裡面 .

* 5. 初始化這次的Command Table
	- 初始化各個Command的相關資訊 
		- string command = ""
		- bool isRedirectBracket = false
		- int pid = 0
		- int incomeFd = 0 (default stdin)
		- int outgoFd = 1 (default stdout)
		- bool isStopped = false
		- bool isCompleted = false
		- vector<string> args = empty 
	- 設定好「目前可得知」各個Command的相關資訊 :
		- string command  
		- bool isRedirectBracket
		- vector<string> args

* 6. 處理各個Commands 
	- Control Process(Parent)
		- 1. Create Pipe, 並設定Command資訊(incomeFd,outgoFd) .
		- 2. 檢查是否為需要特殊處理的Command, 處理完畢後直接return .
			- Job Control command (fg,bg)
			- enviroment command (printenv,export,unset)
			- exit
		- 3. fork Children
		- 4. Set children's gid
		- 5. Close Redirect's fd (if any)
		- 6. Push Job(Group) to Active Job List
		- 7. Close pipe's fd .
		- 8. 若這次的Job是run在foreground, 則進入「waitForForegroundJob()」函數 .
	- Children
		- 1. 設定自己的 gid .
		- 2. unblock那些被Parent block的signals .
		- 3. 恢復SIGCHLD handler為default .
		- 4. 設定好stdin,stdout(依據前面設定好的Cmd資訊) .
		- 5. 關閉那些沒用到的Pipe(必須！否則program會block住) .
		- 6. executeCmd() .

* (Control Process) waitForForegroundJob(Job)
	- 1. 先把SIGCHLD的Handler設定回default .
		- 因為需要特別處理 .
	- 2. 將該Job移至foreground .
		- tcsetpgrp(STDIN_FILENO,job.groupId), 也就是將terminal的STDIN_FILENO交給該Job .
	- 3. 使用一個迴圈不停地接收該Job中process的exit status, 終止條件為: 當該Job Stop了或該Job Complete了 .

``` C++
do{
	...
}while(!jobStopped && !jobCompleted);
```

	- 4. 每當接收到一個Process的exit status, 會依據Pid去查找這個Process是屬於哪個Job, 並且Uddate該Process的Command資訊(isStopped/isCompleted)
		- 這邊要特別注意, 因為是設定wait for all prcoess(也就是waitpid的第一個參數是 -1), 因此每次接收到一個process的exit status, 需要判斷是不是目前正在監控的Job(也就是和參數中的Job相同).
		- 為何要要設成接收所有Process ?
			- 因為已經把SIGCHLD設回default, 過程中如果有其他run在背景的Job的Process完成, 就會沒有人去處理它, 最後會變成Zombie .
	- 5. 最後記得更新Active Job List以及Stopped Job List .
	- 6. 當跳出迴圈後, 需要把Control terminal交還給Control Process, tcsetpgrp(STDIN_FILENO,getpgid(0)) .

#### Reference

* glob Library : 用來實現 * , ? 的filename expands
	- [GLOB(3) - Linux Programmer's Manual](http://man7.org/linux/man-pages/man3/glob.3.html)
* Job Control之Foreground & background概念
	- [阿旺的 Linux 開竅手冊 - Advanced Chapter 6 : Process (程序)](http://wanggen.myweb.hinet.net/ach6/ach6.html?MywebPageId=201751493987558650&MywebPageId=201751493987569034&MywebPageId=201751493993563019&MywebPageId=201761494046201738&MywebPageId=201761494046245306#bg)
* 一些Signal的討論(INT,QUIT,TERM) 
	- [SIGINT,SIGQUIT,SIGTERM这些停止进程的信号有什么不同](http://bbs.csdn.net/topics/330100297)
* 詳細Job Control介紹以及實作方法 
	- [Job Control](https://ftp.gnu.org/old-gnu/Manuals/glibc-2.2.3/html_chapter/libc_27.html#SEC582)










