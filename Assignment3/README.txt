*** README ***

Name: Matthew Luna
Email: Luna.Matthew@yahoo.com OR Luna.Matthew@csu.fullerton.edu
Programming Language used: C++

How to execute program:

	Once threads.cpp is compiled, run the program executable on the command line with an argument of some number (INTEGER) to the effect of "./<EXECUTABLE NAME> <INTEGER VALUE>". This will run the executable with the integer value as an argument to create that integer value specified amount of threads. The threads will randomly generate 100 integers to store inside a binary search tree, which (as a critical section) will be protected by a posix mutex to only allow one thread to insert into the tree at a time. When a thread is finished storing the 100 randome integers into the binary search tree it will then close the thread. Meanwhile the main thread will await for all threads to terminate and close. Once all threads are finished with their tasks and closed, the main thread will display the binary search tree in order to the terminal. Once this has completed, the main thread will then destroy the binary search tree and terminate.

Anything special about submission?: No additional notes. Thank you for the very fun assignment!

*** END README ***
