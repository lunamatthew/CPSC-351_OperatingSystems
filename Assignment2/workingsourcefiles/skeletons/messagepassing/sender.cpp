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

	// The file size
	int fileSize = -1;

	// The buffer used to store the message copied
	// copied from the shared memory
	char buff[8192];

	// The variable to hold the message queue ID
	int msqid = -1;

	// The total number of bytes written
	int totalBytesRead = 0;

	// The number of bytes 
	int bytesRead = 0;

	// Whether we are done reading	
	bool finishedReading = false;
	
	// TODO: Define a data structure
	// of type mq_attr to specify a
	// queue that can hold up to 10
	// messages with the maximum message
	// size being 4096 bytes
	mqd_t myQueue, retVal;
	mq_attr attr;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = 4096;
	unsigned int priority = 1;

	myQueue = mq_open(MSQ_NAME, O_RDWR, 0600, &attr);
	if (myQueue < 0) {
		perror("Error with mq_open!");
		exit(1);
	}
	
	// Sanity checks -- make sure the user has provided a file
	if(argc < 2)
	{
		fprintf(stderr, "USAGE: %s <FILE NAME>\n", argv[0]);
		exit(1);
	}

	// Open the file for reading
	int fd = open(argv[1], O_RDONLY);
	
	// Make sure the file was opened
	if(fd < 0)
	{
		perror("open");
		exit(1);
	}
	
	
	// TODO: Gain access to the message queue
	// whose name is defined by the macro
	// MSQ_NAME macro above. We assume that
	// the receiver has allocated the message queue.

	
	//TODO: Loop below attempts to read the 
	// file 4096 bytes at a time.
	// Modify the loop as necessary to send
	// each chunk of data read as message
	// through the message queue. You can use
	// 1 for the priority of the message.
	fileSize = getFileSize(argv[1]);
	// Keep writing until all data has been written
	//fprintf(stderr, "\nqueue msg size: %d\n", (int)attr.mq_msgsize);
	//fprintf(stderr, "size: %d\n", fileSize);
	while((totalBytesRead < fileSize) && !finishedReading)
	//while(!finishedReading)
	{
		// Read from file to the shared memory
		bytesRead = read(fd, buff, 4096);
		//fprintf(stderr, "bytesRead: %d\n", bytesRead);
		// Something went wrong
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

		// We are at the end of file
		else if (bytesRead == 0)
		{
			fprintf(stderr, "TEST!!!!%d\n\n", bytesRead);
			// We are at the end of file
			finishedReading = true;		
		}
		
		//fprintf(stderr, "bool: %d\n", (int)finishedReading);
		totalBytesRead += bytesRead;
	}
	
	// TODO: Send a message with size of 0
	// to the receiver to tell it that the
	// transmission is done

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
	// TODO: Close the file		
	if (close(fd)) {
		perror("close");
		exit(1);
	}
	
	return 0;
}
