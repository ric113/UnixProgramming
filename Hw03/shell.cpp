#include <string>
#include <vector>
#include <iostream>


#include "lib.h"



int main()
{
	string line;
	vector<string> cmds;
	vector<Cmd> cmdTable;
	bool isBackground = false;

	while(getline(cin,line))
	{

		if(line.find("&") != string::npos){
			isBackground = true;
			cout << "Is bg line!" << endl;
		}

		parseInputLine(line,"|",cmds);
		// showCmdsVector(cmds);
		initCmdTable(cmdTable,cmds);
		showCmdTable(cmdTable);

		cmds.clear();
		cmdTable.clear();
		isBackground = false;
	}
}