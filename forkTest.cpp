#include <iostream>
#include <unistd.h>

int main() {
	for (int i = 0; i < 3; ++i) {
		int pid = fork();
		if (pid == 0)
			std::cout << "\nI am a child!\n";
		std::cout << i;
	}
	std::cout << '\n';
	while (true) { sleep(1); }
	return 0;
}
