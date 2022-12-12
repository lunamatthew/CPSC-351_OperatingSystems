#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <mqueue.h>
#include <unistd.h>
#include <signal.h>

#define MSQ_NAME "/cpsc351queue"

int getFileSize(const char* fileName) {
	struct stat statBuff;
	int fileSize = -1;
	int status = stat(fileName, &statBuff);
	if (status == 0) {
		fileSize = statBuff.st_size;
	}
	else {
		perror("stat");
	}
	return fileSize;
}

int main(int argc, char** argv)
{	

	int fileSize = -1;
	char buff[8192];
	int msqid = -1;
	int totalBytesRead = 0;
	int bytesRead = 0;
	bool finishedReading = false;
	mqd_t myQueue, retVal;
	mq_attr attr;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = 4096;
	unsigned int priority = 1;

	//myQueue = mq_open(MSQ_NAME, O_RDWR, 0600, &attr);
	//if (myQueue < 0) {
	//	perror("Error with mq_open!");
	//	exit(1);
	//}
	
	if(argc < 2)
	{
		fprintf(stderr, "USAGE: %s <FILE NAME>\n", argv[0]);
		exit(1);
	}

	int fd = open(argv[1], O_RDONLY);
	
	if(fd < 0)
	{
		perror("open");
		exit(1);
	}
	
	myQueue = mq_open(MSQ_NAME, O_RDWR, 0600, &attr);
	if (myQueue < 0) {
		perror("Error with mq_open!");
		exit(1);
	}
	
	fileSize = getFileSize(argv[1]);
	while((totalBytesRead < fileSize) && !finishedReading)
	{
		bytesRead = read(fd, buff, 4096);
		if(bytesRead < 0)
		{
			perror("read");
			exit(1);
		}

		else if (bytesRead > 0) {
			retVal = mq_send(myQueue, buff, bytesRead, 1);
			if (retVal < 0) {
				perror("mq_send");
				exit(1);
			}
		}

		else if (bytesRead == 0)
		{
			finishedReading = true;		
		}
		
		totalBytesRead += bytesRead;
	}

	mq_getattr(myQueue, &attr);
	while (attr.mq_curmsgs != 0) {
		mq_getattr(myQueue, &attr);
	}
	retVal = mq_send(myQueue, buff, 0, 2);
	if (retVal < 0) {
		perror("mq_send");
		exit(1);
	}
	fprintf(stderr, "Sent a total of %d bytes\n", totalBytesRead);
	
	fprintf(stderr, "bool: %d\n", (int)finishedReading);
	if (close(fd)) {
		perror("close");
		exit(1);
	}
	
	return 0;
}
