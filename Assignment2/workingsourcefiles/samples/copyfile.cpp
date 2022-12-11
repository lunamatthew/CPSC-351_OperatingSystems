#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

#define MAX_READ_SIZE 4096

int main(int argc, char** argv)
{
	// The input and output file descriptors
	int inFd = -1, outFd = -1;
	
	// To keep track of the number of bytes read/written
	int bytesRead = -1, bytesWritten = -1;
	
	// The buffer to hold the data read from the file
	char buff[MAX_READ_SIZE];
	
	// Sanity checks
	if(argc < 3)
	{
		fprintf(stderr, "%s <SOURCE FILE> <DESTINATION FILE>\n", argv[0]);
		exit(1);
	}
	
	// Open the source file for reading (O_RDONLY)
	inFd = open(argv[1], O_RDONLY);
	
	// Sanity check
	if(inFd < 0)
	{
		perror("open");
		exit(1);
	}
	
	// Open the destination file for writing (O_WRONLY)
	// If does not exist, create it (O_CREAT)
	// If exists, delete its contents (O_TRUNC)
	// Use permissions 0600
	outFd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0600);
	
	// Sanity checks
	if(outFd < 0)
	{
		perror("open");
		exit(1);
	}
	
	// Keep reading until we reach the end of file
	while(bytesRead != 0)
	{
		// Read at most MAX_READ_SIZE.
		// Will return how many bytes were actually
		// read (could be less than MAX_READ_SIZE).
		// Will return 0 if the end of file was reached
		bytesRead = read(inFd, buff, MAX_READ_SIZE);
		
		// Sanity check
		if(bytesRead < 0)
		{
			perror("read");
			exit(1);
		}
		// Not at the end of the source file?
		// Write the data to the destination file
		else if(bytesRead != 0)
		{
			// Write the bytes to the destination file.
			// will return the number of bytes read
			bytesWritten = write(outFd, buff, bytesRead);
			
			// Sanity checks
			if(bytesWritten < 0)
			{
				perror("write");
				exit(1);
			}
		}
		
	}
	
	// Close both files
	close(inFd);
	close(outFd);
	
	return 0;
}
