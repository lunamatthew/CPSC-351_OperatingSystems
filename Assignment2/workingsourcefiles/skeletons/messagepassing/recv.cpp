
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <signal.h>

// The name of the shared memory segment
#define MSQ_NAME "/cpsc351queue"

// The name of the file where to save the received file
#define RECV_FILE_NAME "file__recv"

#define MQ_MSGSIZE 4096


/**
 * Receive the file from the message queue 
 */
void recvFile()
{
	// TODO: Define a data structure
	// of type mq_attr to specify a
	// queue that can hold up to 10
	// messages with the maximum message
	// size being 4096 bytes
	mqd_t myQueue;
	unsigned int prio = 0;	
	mq_attr attr;
	attr.mq_maxmsg = 10;	// possibly not necessary?
	attr.mq_msgsize = MQ_MSGSIZE; // possibly not necessary?
	attr.mq_curmsgs = 0;

	// The buffer used to store the message copied
	// copied from the shared memory
	char buff[MQ_MSGSIZE];

	//unsigned int priority = 1;

	myQueue = mq_open(MSQ_NAME, O_CREAT | O_RDWR, 0600, &attr);
	// mq_setattr() ? use here ?
	if (myQueue < 0) {
		perror("mq_open");
		exit(-1);
	}

	// The total number of bytes written
	int totalBytesWritten = 0;

	// The number of bytes written 
	int bytesWritten = 0;
	
	// The return value
	int msqBytesRecv = -1;
	
	// Open the file for writing
	int fd = open(RECV_FILE_NAME, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	
	// Make sure the file was opened
	if(fd < 0)
	{
		perror("open");
		exit(1);
	}


	
	// TODO: Create the message queue
	// whose name is defined by the macro
	// MSQ_NAME macro above. 
	// The queue should be configured with
	// parameters defined by the data 
	// message queue data structure defined above.
	// The queue should have permissions of 0600.
	int tmpFileOps = 0;
	int i = 0;
	while(msqBytesRecv != 0)
	{
		// TODO: Receive the message.
		// Write the data from the message
		// into the file (represented by the 
		// descriptor fd). Please note:
		// the mq_receive() function
		// you will use will return
		// the size of the message
		// received (in bytes).
		// If the size of the message
		// is 0, it is the sender telling
		// us that it has no more data to send.
		// Please see the sender's code for the
		// corresponding logic
		++i;
		msqBytesRecv = mq_receive(myQueue, buff, 8192, &prio);
		printf("Bytes received: %d Iterations: %d\n Priority: %d ", (int)msqBytesRecv, i, prio);
		if (msqBytesRecv < 0) {
			perror("mq_receive");
			exit(-1);
		}
		if (msqBytesRecv > 0) {
			//tmpFileOps = write(fd, buff, msqBytesRecv);
			tmpFileOps = write(fd, buff, msqBytesRecv);
		}
	}
	tmpFileOps = close(fd);
	if (tmpFileOps < 0) {
		perror("close");
		exit(1);
	}

	// TODO: Deallocate the message queue
	if (mq_unlink(MSQ_NAME) < 0) {
		perror("mq_unlink");
		exit(1);
	}
	if (mq_close(myQueue) < 0) {
		perror("mq_close");
		exit(1);
	}
}


int main(int argc, char** argv)
{	
	
	recvFile();
		
	return 0;
}
