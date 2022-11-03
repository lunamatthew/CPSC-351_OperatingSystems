#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/syscall.h>   /* For SYS_xxx definitions */
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>

int main()
{
	std::cout << SYS_open << '\n' << SYS_close << '\n' << SYS_write << '\n' << SYS_getpid << '\n';
	std::cout << syscall(SYS_getpid) << '\n';
	int fileDescriptor2 = syscall(SYS_creat, "myfile.txt", S_IRWXU);
	if (fileDescriptor2 < 0) {
		perror("syscall");
		exit(-1);
	}
	
	char buffer2[64] = {"Hello OS and hello system calls!"};
	if (syscall(SYS_write, fileDescriptor2, buffer2, 32) < 0) {
		perror("syscall");
		exit(-1);
	}
	
	if (syscall(SYS_close, fileDescriptor2) < 0) {
		perror("syscall");
		exit(-1);
	}
	
	
	/* Issue a system call asking the operating system to open file file.txt.
 	 * The first parameter, 5, tells the OS to execute a routine for opening
 	 * the file. The rest of the arguments are parameters to the OS routine:
 	 * the name of the file and the mode in which the file should be ipened 
 	 * (i.e. for reading, writing, and the file should be created if it does
 	 * not exist.
 	 *
 	 * If successful, the system call will return the file descriptor,
 	 * which is like a reference to the file opened file. It can be
 	 * passed as parameter to system calls for reading and writing files,
 	 * in order to read/write data to/from files. 
 	 */
	int fileDescriptor = syscall(SYS_open, "file.txt", O_RDONLY); 
	
	/* The buffer to store the read data */
	char buffer[100];
	
	/* Error checks (syscall returns a -1 on faliure) */
	if(fileDescriptor < 0)
	{
		perror("syscall");
		exit(-1);
	}
	
	/* Issue a system call to read 12 bytes of data from the file */
	/* The calling convention for sys_read is
	 * @param fileDescriptor - the file descriptor representing
	 * an open file that we would like to ask the OS to read
	 * @param buffer - the buffer where the read data should be stored
	 * @param 12 - represents the number of bytes to read from the file.
	 * in this example, we would like to read 12 bytes of data.
	 * @return - sys_read will return the number of bytes that have
	 * actually been successfully read, 0 if we are at the end of the file,
	 * or -1 on error.
	 */
	if(syscall(SYS_read, fileDescriptor, buffer, 12) < 0)
	{
		perror("syscall");
		exit(-1);
	}
	
	/* Null terminate the string in the buffer */
	buffer[12] = '\0';
	
	fprintf(stderr, "Read: %s\n", buffer);
	
	/* Issue a system call to close the file */	
	/* The calling conventions of SYS_close() are:
	 * @param fileDescriptor - the file descriptor representing
	 * the file we would like to close
	 */
	if(syscall(SYS_close, fileDescriptor) < 0)
	{
		perror("syscall");
		exit(-1);
	}
	
	return 0;
}
