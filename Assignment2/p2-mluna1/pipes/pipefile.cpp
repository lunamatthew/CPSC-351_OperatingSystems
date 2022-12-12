#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

#define RECV_FILE_NAME "file__recv"

void parent(const int& writePipeEndFd, const int& readFileFd)
{
	#define READ_SIZE 4096

	int bytesRead = -1;	
	char buff[READ_SIZE];
	
	while(bytesRead != 0)
	{
		bytesRead = read(readFileFd, buff, READ_SIZE);
		if (bytesRead < 0) {
			perror("read");
			exit(1);
		}
		if (write(writePipeEndFd, buff, bytesRead) < 0) {
			perror("write");
			exit(1);
		}
	}
}

void child(const int& readPipeEndFd, const int& writeFileFd)
{
	#define READ_SIZE 4096

	int bytesRead = -1;	
	char buff[READ_SIZE];
	
	while(bytesRead != 0)
	{
		bytesRead = read(readPipeEndFd, buff, READ_SIZE);
		if (bytesRead < 0) {
			perror("read");
			exit(1);
		}
		if (write(writeFileFd, buff, bytesRead) < 0) {
			perror("write");
			exit(1);	
		}
	}
}

int main(int argc, char** argv)
{
	int fd[2];
	pid_t pid;
	char readBuff[4096];
	char writeBuff[4096];
	int inFd = -1, outFd = -1;
	if(argc < 2)
	{
		fprintf(stderr, "%s <FILE NAME>\n", argv[0]);
		exit(1); 
	}

	if (pipe(fd) < 0) {
		perror("pipe");
		exit(-1);
	}	
	pid = fork();
	if(pid < 0)
	{
		perror("fork");
		exit(1);
	}
	if(pid > 0) // Parent
	{
		if (close(fd[0]) < 0) {
			perror("close");
			exit(1);
		}	
		inFd = open(argv[1], O_RDONLY);
		if (inFd < 0) {
			perror("open");
			exit(1);
		}
		
		parent(fd[1], inFd);

		if (close(fd[1]) < 0) {
			perror("close");
			exit(1);
		}
		if (close(inFd) < 0) {
			perror("close");
			exit(1);
		}
		int childStatus;
		int waitReturn = wait(&childStatus);
		if (waitReturn < 0) {
			perror("wait");
			exit(1);
		}
	}
	else // Child
	{

		if (close(fd[1])) {
			perror("close");
			exit(1);
		}
		
		outFd = open(RECV_FILE_NAME, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		if(outFd < 0)
		{
			perror("open");
			exit(1);
		}
		
		child(fd[0], outFd);
		
		if (close(fd[0] < 0)) {
			perror("close");
			exit(1);
		}
		if (close(outFd) < 0) {
			perror("close");
			exit(1);
		}
	}
}
