#include <string>
#include <vector>

using namespace std;

typedef struct _Cmd{
	string command;
	bool isRedirectBracket ;
	int incomeFd, outgoFd;
	vector<string> args;
}Cmd;

typedef struct _Job{
	int groupId;
	int processCount;
}Job;

void parseInputLine(const string&, string, vector<string>&);
void initCmdTable(vector<Cmd>&, vector<string>&);
void showCmdsVector(vector<string>&);
void showCmdTable(vector<Cmd>&);
vector<string> splitWithSpace(const string&);
