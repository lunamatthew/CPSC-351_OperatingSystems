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
#define FULL_SHARED_MEMORY_PATH "/dev/shm/cpsc351sharedmem"
#define RECV_FILE_NAME "file__recv"

bool terminate = false;

int getFileSize(const char* fileName)
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

void recvFile(int signNum)
{
	int memSize = getFileSize(FULL_SHARED_MEMORY_PATH);
	char buff[memSize];
	int totalBytesWritten = 0;
	int bytesWritten = 0;
	void* sharedMemPtr = NULL;
	int fd = open(RECV_FILE_NAME, O_CREAT | O_TRUNC | O_WRONLY, 0600);

	if(fd < 0)
	{
		perror("open");
		exit(1);
	}

	int shmid = -1;
	shmid = shm_open(SHARED_MEM_NAME, O_RDONLY, 0666); // This may need to be altered
	if (shmid < 0) {
		perror("Missing shared memory segment!");
		exit(1);
	}
	sharedMemPtr = mmap(0, memSize, PROT_READ, MAP_SHARED, shmid, 0);
	if (sharedMemPtr == MAP_FAILED) {
		perror("mmap");
		exit(1);
	}
	
	int tmpFileOps = 0;
	tmpFileOps = write(fd, (char*)sharedMemPtr, memSize);
	if (tmpFileOps < 0) {
		perror("write");
		exit(1);
	}
	tmpFileOps = close(fd);
	if (tmpFileOps < 0) {
		perror("close");
		exit(1);
	}
	tmpFileOps = shm_unlink(SHARED_MEM_NAME);
	if (tmpFileOps < 0) {
		perror("shm_unlink");
		exit(1);
	}
	terminate = true;
}


int main(int argc, char** argv)
{	
	signal(SIGUSR1, recvFile);
	while(!terminate)
	{
		sleep(1);
	}
			
	return 0;
}
