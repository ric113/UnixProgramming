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


using namespace std;

struct Connection{
	int type ; /* 0:tcp , 1:udp*/
	unsigned int localIp;
	unsigned int localPort;
	unsigned int remoteIp;
	unsigned int remotePort;
	int pid;
	string cmdline;
	unsigned int inodeIndex;	
};

void parseConnection(map<unsigned int,Connection>&,int,string);
vector<string> &splitString(const string&,char,vector<string>&);
vector<string> splitAnySpace(const string&);
void printVector(vector<string>);
void printConnectionTable(map<unsigned int,Connection>);
void splitIpAndPort(string,unsigned int&,unsigned int&);

void parseCurrentProcess();


int main(int argc, char const *argv[])
{
	/* code */
	map<unsigned int,Connection> tcpMap;
	map<unsigned int,Connection> udpMap;

	//parseConnection(tcpMap,0,"/proc/net/tcp");
	parseCurrentProcess();
	//std::ifstream tcp("/proc/net/tcp");
	//std::string line;
    	//while(std::getline(tcp, line))
        // 	std::cout << line << '\n';
	
	return 0;
}

void parseCurrentProcess()
{
	DIR* proc = opendir("/proc");
	struct dirent* ent;
	long tgid;

	if(proc == NULL)
	{
		cout << "error!" << endl;
	}

	while(ent = readdir(proc))
	{
		if(!isdigit(*ent->d_name))
			continue;
		tgid = strtol(ent->d_name,NULL,10);
		//print_status(tgid);
		//cout << tgid << endl;
		
		char buf[100];
		sprintf(buf,"/proc/%d/fd", tgid);
		cout << buf << endl;
		
		DIR* inProc = opendir(buf);
		struct dirent* procEnt;
		
		if(inProc == NULL)
			cout << "error! permission denied!" << endl;
		else
		{
			while(procEnt = readdir(inProc))
			{
				char buf2[100];
				strcpy(buf2,buf);
				strcat(buf2,"/");
				strcat(buf2,procEnt->d_name);
				cout << buf2 << endl;
				
				//char buf3[10];
				//readlink(buf2,buf3,sizeof(buf3));
				//cout << buf3 << endl;
				struct stat sb;
				stat(buf2, &sb);
				if((long)sb.st_ino == 34870)
					cout << (long)sb.st_ino << endl;						
			}
		}
		
		
	}

}

void parseConnection(map<unsigned int,Connection> &connectionMap,int connectionType,string path)
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
			unsigned int newIp;
			unsigned int newPort;
			vector<string> tempIpAndPort;		
	
			newConnection.type = connectionType;
			newConnection.inodeIndex = atoi(tempVector.at(9).c_str());
			
			tempIpAndPort =	splitString(tempVector.at(1), ':', tempIpAndPort);
			newConnection.localIp = atoi(tempIpAndPort.at(0).c_str());
			newConnection.localPort = atoi(tempIpAndPort.at(1).c_str());
			
			tempIpAndPort.clear();			

			tempIpAndPort = splitString(tempVector.at(2), ':', tempIpAndPort);
                        newConnection.remoteIp = atoi(tempIpAndPort.at(0).c_str());
                        newConnection.remotePort = atoi(tempIpAndPort.at(1).c_str());

			connectionMap[newConnection.inodeIndex] = newConnection;	
		}
	}
	printConnectionTable(connectionMap);		
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
		
		cout << (it->second).type << endl;
		cout << (it->second).localIp << endl;
		cout << (it->second).localPort << endl;
		cout << (it->second).remoteIp << endl;
		cout << (it->second).remotePort << endl;
		cout << (it->second).pid << endl;
		cout << (it->second).cmdline << endl;
		cout << (it->second).inodeIndex << endl;
		
		//cout << (it->second) << endl;
		it++;
	}
}




