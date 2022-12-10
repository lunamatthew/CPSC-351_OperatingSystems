#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <signal.h>

#define SHARED_MEM_NAME "cpsc351sharedmem"
#define MAX_READ_SIZE 4096

int getFileSize(char* fileName)
{
	struct stat statBuff;
	int fileSize = -1;
	int status = stat(fileName, &statBuff);

	if(status == 0) 
	{
		fileSize = statBuff.st_size;
	}
	else
	{
		perror("stat");
	}
	
	return fileSize;
}

int main(int argc, char** argv)
{	
	int fileSize = -1;
	int shmid = -1;
	int totalBytesRead = 0;
	int bytesRead = -1;
	int bytesWritten = 0;
	bool finishedReading = false;
	void* sharedMemPtr = NULL;
	if(argc < 3)
	{
		fprintf(stderr, "USAGE: %s <FILE NAME> <PID OF THE RECEIVER PROCESS>\n", argv[0]);
		exit(1);
	}

	int infd = open(argv[1], O_RDONLY); // O_RDONLY ?
	
	if(infd < 0)
	{
		perror("open");
		exit(1);
	}
	
	fileSize = getFileSize(argv[1]);
	
	if(fileSize < 0)
	{
		fprintf(stderr, "The file %s does not exist\n", argv[1]);
	}
	
	char charBuff[MAX_READ_SIZE];
	int recvPid = atoi(argv[2]);
	shmid = shm_open(SHARED_MEM_NAME, O_CREAT | O_RDWR, 0600);
	if (shmid < 0) {
		perror("shm_open");
		exit(1);
	}
	if (ftruncate(shmid, fileSize) < 0) {
		perror("ftruncate");
		exit(1);
	}
	sharedMemPtr = mmap(0, fileSize, PROT_WRITE, MAP_SHARED, shmid, 0);
	if (sharedMemPtr == MAP_FAILED) {
		perror("mmap");
		exit(1);
	}

	int i = -1;
	while(bytesRead != 0) {
		bytesRead = read(infd, charBuff, MAX_READ_SIZE);
		if (bytesRead < 0) {
			perror("read");
			exit(1);
		}
		else if (bytesRead != 0) {
			++i;
			strncpy((char*)sharedMemPtr + (MAX_READ_SIZE * i), charBuff, bytesRead);

			if (bytesWritten < 0) {
				perror("write");
				exit(1);
			}
		}
	}

	close(infd);
	int pid = atoi(argv[2]);
	int retVal = kill(pid, SIGUSR1);
	if (retVal < 0) {
		perror("Kill failed");
	}
	
	return 0;
}
