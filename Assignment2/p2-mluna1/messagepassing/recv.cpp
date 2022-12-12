
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

#define MSQ_NAME "/cpsc351queue"
#define RECV_FILE_NAME "file__recv"

#define MQ_MSGSIZE 4096

void recvFile()
{
	mqd_t myQueue;
	unsigned int prio = 0;	
	mq_attr attr;
	attr.mq_maxmsg = 10;
	attr.mq_msgsize = MQ_MSGSIZE;
	attr.mq_curmsgs = 0;

	char buff[MQ_MSGSIZE];

	myQueue = mq_open(MSQ_NAME, O_CREAT | O_RDWR, 0600, &attr);
	if (myQueue < 0) {
		perror("mq_open");
		exit(-1);
	}

	int totalBytesWritten = 0;
	int bytesWritten = 0;
	int msqBytesRecv = -1;
	int fd = open(RECV_FILE_NAME, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if(fd < 0)
	{
		perror("open");
		exit(1);
	}

	int tmpFileOps = 0;
	while(msqBytesRecv != 0)
	{
		msqBytesRecv = mq_receive(myQueue, buff, 8192, NULL);
		if (msqBytesRecv < 0) {
			perror("mq_receive");
			exit(-1);
		}
		if (msqBytesRecv > 0) {
			tmpFileOps = write(fd, buff, msqBytesRecv);
		}
	}
	tmpFileOps = close(fd);
	if (tmpFileOps < 0) {
		perror("close");
		exit(1);
	}

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
