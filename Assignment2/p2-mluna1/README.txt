*** README ***

Name: Matthew Luna
Email: Luna.Matthew@yahoo.com OR Luna.Matthew@csu.fullerton.edu
Programming language used: C++

How to execute programs:
	
	For the sharedmem programs:
	Once both recv.cpp and sender.cpp are compiled, first run the recv executable. Inside another terminal use "pidof <recv executable name>" to get the pid of the currently running recv process. With this information run the sender executable with the arguments on the command line of a file to read for input and the pid of the recv process. This would look something like this: "./<name of sender executable> <file for input> <pidof recv process that is running>" This will transfer the file contents (file for input) using shared memory between the two processes and output the data from the file for input to a new file within the directory. This new file will be named "file__recv".
	
	For the message passing programs:
	Once both recv.cpp and sender.cpp are compiled, first run the recv executable. Next, inside a seperate terminal run the sender executable alongside a file for input. This will look like: "./<sender executable> <file for input>". The processes will then write to a new file within the directory the contents of the file for input, this file named file__recv. Both processes should then terminate.
	
	For the pipe programs:
	Once pipefile.cpp is compiled, run the pipefile executable alongisde a file for input argument on the command line. This will look something like: "./<pipefile executable> <file for input>". The process, using pipes, will write the data of the file for input to a new file within the directory. The new file will be named "file__recv". 
	
	For the bonus pipe program:
	Once pipefileBonus.cpp is compiled, you will run it exactly as the pipe program (see For the pipe programs section of the README). This will result in the same output as the normal pipe program however it will make use of the sendfile() system call instead of using write() in the parent.

Extra Credit Implemented?: Yes, the extra credit program is within the pipes directory and named pipefileBonus.cpp.

Anything special about submission?: No additional notes. Thank you for the assignment!

*** END README ***
