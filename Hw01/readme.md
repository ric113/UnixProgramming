### Howework 01 - Implement a 'netstat -nap'-like program
In this homework, we have to implement a 'netstat -nap' tool by yourself. You have to list all the existing TCP and UDP connections. <br><br>

To provide more flexibilities, your program have to accept several predefined options, including : <br>

* -t or --tcp: list only TCP connections.
* -u or --udp: list only UDP connections.
* An optional string to list only command lines that containing the string.

#### How to Run ?
``` shell
$ make
$ ./hw1 [-t|--tcp] [-u|--udp] [filter-string]
```
<br>Samples result :<br>
``` shell
$ ./hw1
$ ./hw1
List of TCP connections:
Proto Local Address           Foreign Address         PID/Program name and arguments
tcp   0.0.0.0:993             0.0.0.0:*               2472/dovecot    
tcp   0.0.0.0:60546           0.0.0.0:*               764/rpc.statd   
tcp   0.0.0.0:995             0.0.0.0:*               2472/dovecot    
tcp   127.0.0.1:3306          0.0.0.0:*               2684/mysqld     
tcp   0.0.0.0:110             0.0.0.0:*               2472/dovecot    
tcp   0.0.0.0:143             0.0.0.0:*               2472/dovecot    
tcp   0.0.0.0:111             0.0.0.0:*               742/rpcbind     
tcp   0.0.0.0:465             0.0.0.0:*               2827/master     
tcp   192.168.1.254:53        0.0.0.0:*               30495/named     
tcp   127.0.0.1:53            0.0.0.0:*               30495/named     
tcp   10.0.3.1:53             0.0.0.0:*               2580/dnsmasq    
tcp   127.0.0.1:631           0.0.0.0:*               27868/cupsd     
tcp   127.0.0.1:953           0.0.0.0:*               30495/named     
tcp   0.0.0.0:25              0.0.0.0:*               2827/master     
tcp   0.0.0.0:1723            0.0.0.0:*               2845/pptpd      
tcp   140.113.1.1:50275       140.112.172.4:23        16993/telnet ptt.cc
tcp6  :::993                  :::*                    2472/dovecot    
tcp6  :::995                  :::*                    2472/dovecot    
tcp6  :::2122                 :::*                    2475/sshd       
tcp6  :::110                  :::*                    2472/dovecot    
tcp6  :::143                  :::*                    2472/dovecot    
tcp6  :::111                  :::*                    742/rpcbind     
tcp6  :::80                   :::*                    4358/apache2    
tcp6  :::465                  :::*                    2827/master     
tcp6  :::53                   :::*                    30495/named     
tcp6  :::33173                :::*                    764/rpc.statd   
tcp6  ::1:631                 :::*                    27868/cupsd     
tcp6  ::1:953                 :::*                    30495/named     
tcp6  :::25                   :::*                    2827/master     
tcp6  :::443                  :::*                    4358/apache2    
tcp6  ::1:53859               ::1:631                 2608/cups-browsed

List of UDP connections:
Proto Local Address           Foreign Address         PID/Program name and arguments
udp   0.0.0.0:5353            0.0.0.0:*               1071/avahi-daemon: 
udp   0.0.0.0:56307           0.0.0.0:*               764/rpc.statd   
udp   0.0.0.0:41678           0.0.0.0:*               1071/avahi-daemon: 
udp   0.0.0.0:12491           0.0.0.0:*               2559/dhcpd      
udp   10.0.3.1:53             0.0.0.0:*               30495/named     
udp   192.168.1.254:53        0.0.0.0:*               30495/named     
udp   127.0.0.1:53            0.0.0.0:*               30495/named     
udp   10.0.3.1:53             0.0.0.0:*               2580/dnsmasq    
udp   0.0.0.0:67              0.0.0.0:*               2559/dhcpd      
udp   0.0.0.0:67              0.0.0.0:*               2580/dnsmasq    
udp   0.0.0.0:111             0.0.0.0:*               742/rpcbind     
udp   0.0.0.0:631             0.0.0.0:*               2608/cups-browsed
udp   0.0.0.0:913             0.0.0.0:*               742/rpcbind     
udp   127.0.0.1:940           0.0.0.0:*               764/rpc.statd   
udp6  :::5353                 :::*                    1071/avahi-daemon: 
udp6  :::59492                :::*                    764/rpc.statd   
udp6  :::53                   :::*                    30495/named     
udp6  :::111                  :::*                    742/rpcbind     
udp6  :::913                  :::*                    742/rpcbind     
udp6  :::34193                :::*                    1071/avahi-daemon: 
udp6  :::18161                :::*                    2559/dhcpd  
```
<br>

#### Note about this homework

###### Map 
可以存 Key:Value的Table : [Reference](http://mropengate.blogspot.tw/2015/12/cc-map-stl.html)

###### 取得程式參數(短參數 e.g. -v -la -al,或是長參數 e.g. --tcp , --udp) 
getopt_long : [Reference](http://blog.csdn.net/ast_224/article/details/3861625)

###### 以stream形式讀file 
ifstream : [Reference](http://kwcheng0119.pixnet.net/blog/post/43828154-cpp---ifstream-(read-file)---%E9%96%8B%E6%AA%94-%26-%E8%AE%80%E6%AA%94)

###### 開啟某個目錄/探索該目錄所有檔案/關閉目錄 
``` C
	...
	DIR* dir = opendir("/proc");	/* 開啟"/proc"這個目錄 */
	struct dirent* ent;				/* 紀錄目錄中entry的info. */
	while(end = readdir(dir))		/* 探訪所有該目錄底下的檔案/目錄 */
	{
		...
	}
	closedir(dir);
	...
```

###### long int strtol([target String],NULL,[base]) 
將各形式字串轉成以base為基底的long int : [Reference](http://blog.xuite.net/tzeng015/twblog/113272225-strtol%EF%BC%88%E5%B0%87%E5%AD%97%E7%AC%A6%E4%B8%B2%E8%BD%89%E6%8F%9B%E6%88%90%E9%95%B7%E6%95%B4%E5%9E%8B%E6%95%B8%EF%BC%89)


###### sscanf() 
[Reference](http://pisces.ck.tp.edu.tw/~peng/index.php?action=showfile&file=fa6dc86f5b337e2c7b8fdc11d1c32b41990d2f08b)
<br>formatting : [Reference](http://edisonx.pixnet.net/blog/post/35305668-%5Bc%5D-printf-%E5%BC%95%E6%95%B8%E8%AA%AA%E6%98%8E)

###### ipv6
* 有128個bit = 16 Bytes.
* 資料結構 : 
``` C
	struct in6_addr {
            unsigned char   s6_addr[16];   /* IPv6 address , 1 char = 1 byte  */
       };
```
* 由[Linux source code](http://lxr.free-electrons.com/source/net/ipv6/tcp_ipv6.c#L1779)知道,/proc/net/[tcp6/udp6]中,localIp和remoteIp的排列格式 .
* 先將各in6_addr.s6_addr[16]依上面的格式存入 .
* inet_ntop(AFINET6,&in6_addr.s6_addr,buf,sizeof(buf))