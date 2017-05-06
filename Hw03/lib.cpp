#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <string.h>

#include "lib.h"

using namespace std;

void showCmdsVector(vector<string> &cmds)
{
	vector<string>::iterator it = cmds.begin();
	while(it != cmds.end())
	{
		cout << *it << endl;
		it ++;
	}
}

void showCmdTable(vector<Cmd> &cmdTable)
{
	cout << "Yo!" << endl;
	vector<Cmd>::iterator it = cmdTable.begin();
	while(it != cmdTable.end())
	{
		cout << "Com : "<<(*it).command << endl;
		if((*it).isRedirectBracket)
			cout << "Is bracket" << endl;
		cout << "Args :" << endl;

		vector<string>::iterator itt = (*it).args.begin();
		while(itt != (*it).args.end())
		{
			cout << *itt << endl;
			itt ++;
		}
		it ++;
	}

	cout << "Command Amount : " << cmdTable.size() << endl;
}

vector<string> splitWithSpace(const string &source)
{
    stringstream ss(source);
    vector<string> vec( (istream_iterator<string>(ss)), istream_iterator<string>() );
    return vec;
}


void parseInputLine(const string &s, string delim,vector<string> &cmds)
{

    char *src = strdup(s.c_str());
    char *ptr = strtok(src, delim.c_str()); // split source string with delimiter, and return the first sub-string to *ptr
    while (ptr != NULL) {
        cmds.push_back(string(ptr));
        ptr = strtok(NULL, delim.c_str()); // with the first argument of strtok() being NULL,it would continue on splitting the remaining source string from previous strtok
    }

}

void initCmdTable(vector<Cmd>& cmdTable,vector<string>& cmds)
{
	int cmdAmount = cmds.size();
	string currentCmd;


	for(int i = 0 ; i < cmdAmount ; i ++)
	{
		currentCmd = cmds[i];
		Cmd tempCmdEntry;
		vector<string> tokens = splitWithSpace(currentCmd);


		if(currentCmd.find("<") != string::npos || currentCmd.find(">") != string::npos)
			tempCmdEntry.isRedirectBracket = true;
		else 
			tempCmdEntry.isRedirectBracket = false;

		tempCmdEntry.command = tokens[0];
		vector<string> args;
		for(int j = 0 ; j < tokens.size() ; j ++)
		{
			if(tokens[j] != "<" && tokens[j] != ">" && tokens[j] != "&")
				args.push_back(tokens[j]);
		}
		tempCmdEntry.args = args;

		cmdTable.push_back(tempCmdEntry);
	}
}



