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
	int bytesRead = -1;
	void* sharedMemPtr = NULL;
	if(argc < 3)
	{
		fprintf(stderr, "USAGE: %s <FILE NAME> <PID OF THE RECEIVER PROCESS>\n", argv[0]);
		exit(1);
	}

	int infd = open(argv[1], O_RDONLY);
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

	bytesRead = read(infd, sharedMemPtr, fileSize);
	if (bytesRead < 0) {
		perror("read");
		exit(1);
	}

	int tmpFileOp = 0;
	tmpFileOp = close(infd);
	if (tmpFileOp < 0) {
		perror("close");
		exit(1);
	}
	int pid = atoi(argv[2]);
	int retVal = kill(pid, SIGUSR1);
	if (retVal < 0) {
		perror("Kill");
		exit(1);
	}
	
	return 0;
}
