#include <fstream>
#include <iostream>


int main(int argc, char const *argv[])
{
	/* code */
	std::ifstream tcp("/proc/net/tcp");
	printf("Hi");
    std::string line;
    while(std::getline(tcp, line))
        std::cout << line << '\n';

	return 0;
}