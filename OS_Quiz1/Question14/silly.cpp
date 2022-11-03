#include <iostream>
#include <fstream>
#include <unistd.h>

int main()
{
	std::ofstream file("file.txt");
	file << "Hello" << std::endl;
	while(true) { sleep(1); }
	
	return 0;
}

