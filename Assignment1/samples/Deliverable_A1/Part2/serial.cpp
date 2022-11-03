// File: serial.cpp
// Programmed by: Matthew Luna 

#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdio.h>

#define BUFF_SIZE 200

struct location {
	double latitude = 0.0;
	double longitude = 0.0;
};

std::fstream openFile(std::string fileName) {
	std::fstream file;
	file.open(fileName, std::ios::in);
	return file;
}

void closeFile(std::fstream& fileStream) {
	fileStream.close();
}

bool isFileNull(std::fstream& file) {
	return file.fail();
}

bool getCoordinatesFromFile(std::string fileName, std::vector<location>& vector) {
	std::fstream file = openFile(fileName);
	if (isFileNull(file)) 
		return false;
	std::string line;
	double val = 0.0;
	while (std::getline(file, line)) {
		std::stringstream lineStream(line);
		location *temp = new location;
		vector.push_back(*temp);
		lineStream >> vector.back().latitude;
		lineStream >> vector.back().longitude;
		temp = nullptr;
	}
	closeFile(file);
	return true;
}

const std::string INPUT_FILE_NAME = "input.txt";

int main() {
	std::vector<location> locations;
	if (!getCoordinatesFromFile(INPUT_FILE_NAME, locations)) {
		printf("\nError Parsing Data from File... Exiting.\n");
		return -1;
	}
	
	for (int i = 0; i < locations.size(); ++i) {
		pid_t pid;
		pid = fork();

		if (pid < 0) { // Error check
			perror("fork");
			exit(1);
		}
		if (pid == 0) { // If pid == 0 then is child
			printf("\nChild PID: %d\n\tChilds-Parent PID: %d\n\t\tChild UID: %d\n\t\t\tChild GID: %d\n\n", getpid(), getppid(), getuid(), getgid());
			std::string saveFileLocStr = "file";
			saveFileLocStr += std::to_string(i+1);
			saveFileLocStr += ".json";
			char urlCString[BUFF_SIZE];
			snprintf(urlCString, BUFF_SIZE, "https://api.open-meteo.com/v1/forecast?latitude=%f&longitude=%f&current_weather=True", locations[i].latitude, locations[i].longitude);
			if (execlp("/usr/bin/curl", "curl", "-o", saveFileLocStr.c_str(), urlCString, NULL) < 0) {
				perror("execlp");
				exit(1);
			}
		} else  { // Parent
			printf("\n\nParent UID: %d\n\tParent GID: %d\n", getuid(), getgid());
			int childStatus;
			int waitReturn = wait(&childStatus);
			if (waitReturn < 0) {
				perror("wait");
				exit(1);
			}
		}
	}

	return 0;
}
