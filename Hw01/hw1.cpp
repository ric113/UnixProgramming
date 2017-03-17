#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include <string>
#include <string.h>
#include <iterator>
#include <cstdlib>

#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>

#include <iomanip>

#include <getopt.h>


using namespace std;

#define BUF_LENGTH 1024

enum ConnectionType
{
	TCP,
	TCPV6,
	UDP,
	UDPV6
};

struct Connection{
	ConnectionType type; 			
	char localIp[INET_ADDRSTRLEN];
	unsigned int localPort;
	string localIpAndPort;
	char remoteIp[INET_ADDRSTRLEN];
	unsigned int remotePort;
	string remoteIpAndPort;
	int pid;
	char* cmdline;
	unsigned int inodeIndex;
};

void parseConnection(map<unsigned int,Connection>&,ConnectionType,string);
vector<string> &splitString(const string&,char,vector<string>&);
vector<string> splitAnySpace(const string&);
void printVector(vector<string>);
void printConnectionTable(map<unsigned int,Connection>);
void splitIpAndPort(string,unsigned int&,unsigned int&);
string intToString(int);

void parseCurrentProcess(map<unsigned int,Connection>&,map<unsigned int,Connection>&);
char* getProcessCmdlineInfo(const int);
char* ipv6IpConvert(string);
bool passFilter(string,vector<string>);

void printTCPConnection(map<unsigned int,Connection>&,bool,vector<string>&);
void printUDPConnection(map<unsigned int,Connection>&,bool,vector<string>&);

int main(int argc, char **argv)
{
	/* code */

	struct option long_options[] = {
    	{"tcp",0,NULL,'t'},
    	{"udp",0,NULL,'u'},
    	{0,0,0,0}
	};

	bool printBoth = true , useTcpArg = false , useUdpArg = false , hasfilterString = false ;
	vector<string> filterStrings ;

	int c;  
    while((c = getopt_long(argc, argv, "tu", long_options, NULL)) != -1)  
    {  
        switch (c)  
        {  
        case 't':  
        	useTcpArg = true;
        	printBoth = false;
            break;  
         case 'u':  
            useUdpArg = true;
            printBoth = false;
            break;  
        default:
        	cout << "Usage : sudo ./hw1 [-t|--tcp] [-u|--udp] [option]" << endl;
         	return 0;
        }  
    }  

     argc -= optind;
     argv += optind;

    int i;
    for(i = 0; i < argc; i++) {
     	filterStrings.push_back(string(argv[i]));
	}

	if(filterStrings.size() > 0)
		hasfilterString = true;

	map<unsigned int,Connection> tcpMap;
	map<unsigned int,Connection> udpMap;

	parseConnection(tcpMap,TCP,"/proc/net/tcp");
	parseConnection(tcpMap,TCPV6,"/proc/net/tcp6");
	parseConnection(udpMap,UDP,"/proc/net/udp");
	parseConnection(udpMap,UDPV6,"/proc/net/udp6");

	parseCurrentProcess(tcpMap,udpMap);

	if(printBoth)
	{
		printTCPConnection(tcpMap,hasfilterString,filterStrings);
		printUDPConnection(udpMap,hasfilterString,filterStrings);
	}
	else
	{
		if(useTcpArg)
			printTCPConnection(tcpMap,hasfilterString,filterStrings);
		if(useUdpArg)
			printUDPConnection(udpMap,hasfilterString,filterStrings);
	}
	
	return 0;
}

void printTCPConnection(map<unsigned int,Connection> &tcpMap,bool hasfilterString,vector<string> &filterStrings)
{
	cout << "List of TCP connecions:" << endl;
	cout << left << setw(20) << "Proto" << left << setw(20) << "Local Address" << left << setw(30) << "Foreign Address" << left << setw(30) << "PID/Program name and arguments" << endl;

	char buf[BUF_LENGTH];
	string typeStr;
	map<unsigned int,Connection>::iterator it = tcpMap.begin();
	while(it != tcpMap.end())
	{
		Connection currentConnt = it->second;

		if(currentConnt.type == TCPV6)
			typeStr = "tcp6";
		else
			typeStr = "tcp";

		sprintf(buf,"%-20s%-20s%-30s%d/%s\n",typeStr.c_str(),currentConnt.localIpAndPort.c_str(),currentConnt.remoteIpAndPort.c_str(),currentConnt.pid,currentConnt.cmdline);
		
		if(passFilter(string(buf),filterStrings))
			cout << buf ;
		
		it ++;
	}
	cout << endl ;
}

void printUDPConnection(map<unsigned int,Connection> &udpMap,bool hasfilterString,vector<string> &filterStrings)
{
	cout << "List of UDP connecions:" << endl;
	cout << left << setw(20) << "Proto" << left << setw(20) << "Local Address" << left << setw(30) << "Foreign Address" << left << setw(30) << "PID/Program name and arguments" << endl;
	
	char buf[BUF_LENGTH];
	string typeStr;
	map<unsigned int,Connection>::iterator it = udpMap.begin();
	while(it != udpMap.end())
	{
		Connection currentConnt = it->second;

		if(currentConnt.type == UDPV6)
			typeStr = "udp6";
		else
			typeStr = "udp";

		sprintf(buf,"%-20s%-20s%-30s%d/%s\n",typeStr.c_str(),currentConnt.localIpAndPort.c_str(),currentConnt.remoteIpAndPort.c_str(),currentConnt.pid,currentConnt.cmdline);
		
		if(passFilter(string(buf),filterStrings))
			cout << buf ;

		it ++;
	}
}

bool passFilter(string target,vector<string> filterStrings)
{
	vector<string>::iterator it = filterStrings.begin();
	while(it != filterStrings.end())
	{
		if(target.find(*it) != string::npos)
			return false;
		it ++;
	}
	return true;
}

void parseCurrentProcess(map<unsigned int,Connection> &tcpMap,map<unsigned int,Connection> &udpMap)
{
	DIR* proc = opendir("/proc");
	struct dirent* ent;
	long tgid;

	if(proc == NULL)
		perror("Can't open /proc!");

	while(ent = readdir(proc))
	{
		if(!isdigit(*ent->d_name))
			continue;
		tgid = strtol(ent->d_name,NULL,10);
		//print_status(tgid);
		//cout << tgid << endl;
		
		char buf[100];
		sprintf(buf,"/proc/%ld/fd", tgid);
		//cout << buf << endl;
		
		DIR* inProc = opendir(buf);
		struct dirent* procEnt;
		
		if(inProc == NULL)
		{
			perror("Can't open /proc/[pid]/fd!");
			exit(errno);
		}
		else
		{
			while(procEnt = readdir(inProc))
			{
				char buf2[100];
				strcpy(buf2,buf);
				strcat(buf2,"/");
				strcat(buf2,procEnt->d_name);
				//cout << buf2 << endl;
				
				
				struct stat sb;
				stat(buf2, &sb);
			
				 	
				map<unsigned int,Connection>::iterator it ;
				
				it = tcpMap.find((long)sb.st_ino);
				if(it != tcpMap.end()) /* it's tcp connection */
				{	
					(it->second).pid = tgid;
					(it->second).cmdline = getProcessCmdlineInfo(tgid);
				}
				else
				{
					it = udpMap.find((long)sb.st_ino);
					if( it != udpMap.end()) /* it's udp connection */
					{
						(it->second).pid = tgid;
						(it->second).cmdline = getProcessCmdlineInfo(tgid);		
					}
					
				}
				
						
			}
		}
		
		closedir(inProc);
	}
	closedir(proc);

}

char* ipv6IpConvert(string ipHex)
{
	struct in6_addr tmp_ip;
	char *ipStr = (char*)malloc(128 * sizeof(char)) ;

	if (sscanf(ipHex.c_str(),
		"%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx",
		&tmp_ip.s6_addr[3], &tmp_ip.s6_addr[2], &tmp_ip.s6_addr[1], &tmp_ip.s6_addr[0],
		&tmp_ip.s6_addr[7], &tmp_ip.s6_addr[6], &tmp_ip.s6_addr[5], &tmp_ip.s6_addr[4],
		&tmp_ip.s6_addr[11], &tmp_ip.s6_addr[10], &tmp_ip.s6_addr[9], &tmp_ip.s6_addr[8],
		&tmp_ip.s6_addr[15], &tmp_ip.s6_addr[14], &tmp_ip.s6_addr[13], &tmp_ip.s6_addr[12]) == 16) 
	{
		inet_ntop(AF_INET6, &tmp_ip, ipStr, sizeof(ipStr));
	}

	return ipStr;
}

char* getProcessCmdlineInfo(const int pid)
{
	char* name = (char*)calloc(1024,sizeof(char));
	char* result;
	if(name)
	{
		sprintf(name, "/proc/%d/cmdline",pid);
		FILE* f = fopen(name,"r");
		if(f)
		{
			size_t size;
			size = fread(name, sizeof(char), 1024, f);
			if(size > 0)
			{
				if('\n' == name[size-1])
					name[size-1] = '\0';
				
				int i;
				result = (char*)malloc(size * sizeof(char)); 
				for(i=0 ; i < size ;i ++)
				{
					if(name[i] == '\0')
						result[i] = ' ';
					else
						result[i] = name[i];
				}
				result[size-1] = '\0';
				
					
			}
			fclose(f);
		}
	}
	free(name);
	return result;
}

void parseConnection(map<unsigned int,Connection> &connectionMap,ConnectionType connectionType,string path)
{
	bool isFirstLine = true;
	ifstream connection(path.c_str());
	string line;
	vector<string> tempVector;

	while(getline(connection, line))
	{
		if(isFirstLine)
		{
			isFirstLine = false;
		}
		else
		{
			tempVector = splitAnySpace(line);
			//printVector(tempVector);
			
			Connection newConnection;
			unsigned int newLocalIp,newRemoteIp;
			string newLocalPort,newRemotePort;
			vector<string> localIpAndPort,remoteIpAndPort;		
	
			newConnection.type = connectionType;
			newConnection.inodeIndex = atoi(tempVector.at(9).c_str());
			
			localIpAndPort = splitString(tempVector.at(1), ':', localIpAndPort);
			remoteIpAndPort = splitString(tempVector.at(2), ':', remoteIpAndPort);


			//newConnection.localPort = (int)strtol(localIpAndPort.at(1).c_str(),NULL,16);
			//newConnection.remotePort = (int)strtol(remoteIpAndPort.at(1).c_str(),NULL,16);
			
			newLocalPort = ((int)strtol(localIpAndPort.at(1).c_str(),NULL,16) == 0)? "*" : string(intToString((int)strtol(localIpAndPort.at(1).c_str(),NULL,16)));
			newRemotePort = ((int)strtol(remoteIpAndPort.at(1).c_str(),NULL,16) == 0)? "*" : string(intToString((int)strtol(localIpAndPort.at(1).c_str(),NULL,16)));

			newLocalIp = strtol(localIpAndPort.at(0).c_str(),NULL,16);
			newRemoteIp = strtol(remoteIpAndPort.at(0).c_str(),NULL,16);

			if(connectionType == TCP || connectionType == UDP)
			{
				inet_ntop(AF_INET, &newLocalIp, newConnection.localIp, sizeof(newConnection.localIp));
				inet_ntop(AF_INET, &newRemoteIp, newConnection.remoteIp, sizeof(newConnection.remoteIp));
			}
			else
			{
				
				strcpy(newConnection.localIp,ipv6IpConvert(localIpAndPort.at(0)));
				strcpy(newConnection.remoteIp,ipv6IpConvert(remoteIpAndPort.at(0)));
			}

			newConnection.localIpAndPort = string(newConnection.localIp) + ":" + newLocalPort ;
			newConnection.remoteIpAndPort = string(newConnection.remoteIp) + ":" +  newRemotePort ;

			//
            //newConnection.remoteIp = atoi(tempIpAndPort.at(0).c_str());
            

			connectionMap[newConnection.inodeIndex] = newConnection;	
		}
	}
	//printConnectionTable(connectionMap);		
}

vector<string> &splitString(const string &source, char delim, vector<string> &elmts)
{
	stringstream ss(source);
	string item;
	while(getline(ss, item,delim))
		elmts.push_back(item);
	return elmts;
}

vector<string> splitAnySpace(const string &source)
{
	stringstream ss(source);
	vector<string> vec((istream_iterator<string>(ss)), istream_iterator<string>() );
	return vec;
}

string intToString(int i)
{
	stringstream ss;
	ss << i;
	return ss.str();
}


void printVector(vector<string> vec)
{
	vector<string>::iterator it = vec.begin();
	while(it != vec.end())
	{
		cout << *it << endl;
		it ++ ;
	}
}

void printConnectionTable(map<unsigned int,Connection> table)
{
	map<unsigned int,Connection>::iterator it = table.begin();
	while(it != table.end()){
		
		cout << "type:" << (it->second).type << endl;
		cout << "localIP:" <<(it->second).localIp << endl;
		cout << "localPort:" <<(it->second).localPort << endl;
		cout << "remoteIp:"<< (it->second).remoteIp << endl;
		cout << "remotePort:" <<(it->second).remotePort << endl;
		cout << "pid:"<<(it->second).pid << endl;
		cout << "cmdline:"<<(it->second).cmdline << endl;
		cout << "inode:"<<(it->second).inodeIndex << endl;
		
		//cout << (it->second) << endl;
		it++;
	}
}




