*** README ***

Name: Matthew Luna
Email: Luna.Matthew@yahoo.com OR Luna.Matthew@csu.fullerton.edu
Description of Contribution: I did this assignment alone, and worked on and researched all the problems myself.
        All code and effort that went into this assignment was done by me alone. I really enjoyed working on this.
Statement: I, Matthew Luna, truthfully and faithfully completed this assignment using my own knowledge of these topics,
        what I have learned in class regarding these topics, and utilized the skeleton code provided where I saw fit.
        I researched the Linux manual of the system calls used being, wait(), fork(), execlp(), getpid(), getppid(), getgid(), and getuid().
        This was also my introduction into using, albeit lightly, .json files for the first time, and can see some very cool applications
        for using said files that I found doing this assignment and in my research for it.
Programming Language Used: C++
How to execute programs:
        For the shell program:
        Once shell.cpp is compiled, running the executable will prompt the user for a command that will be ran as if
        through any other shell. The shell process will repeat prompting the user to allow for more commands, this will occur until the user
        inputs the exit command, at which point the process will end.

        For the serial & parallel proggrams:
        Once both serial.cpp and parallel.cpp have been compiled, each program should run through the created executable just
        fine as any other program. For the functionality of each of the programs there should be a text file included inside the same directory
        as the executable code itself named "input.txt". This text file should include coordinates (latitude and longitude) seperated by white space
        between the two decimal numericals, and then seperated by new lines for new locations. This is EXACTLY how the "input.txt" file was provided
        for the assignment on Canvas as I have not edited the input file in any way. Assuming this format remains, any data should work for my
        program, including more or less data as the program dynamically parses the data for fetching the weather information into the .json files.
        The .json files that will be saved should save into the same directory as the executable code and will be numerically organized as described
        in the assignment prompt. (i.e. file1.json, file2.json, etc etc) Each .json file represents the data fetched for one location, each .json
        file will be for a different location as originally provided for in the "input.txt" file.

        Run the executable, wait for the parent process to exit, and each .json will be inside the same directory. Each program works properly and
        as expected for both executables following the prompt.
Extra Credit?: I did not implement the extra credit.
Additional Notes: No additional notes. Thank you for the very fun and interesting assignment!

Performance:
time ./serial
	real 0m3.579s
	user 0m0.323s
	sys  0m0.027s

time ./parallel
	real 0m1.278s
	user 0m0.291s
	sys  0m0.016s

Answers to Questions:
1. In the output of the time program, real represents the "wall clock time" of the process once
	executed. This is the time from start to finish of the call. This time includes the total
	time the process took including context switching, waiting for I/O to complete, etc etc.
   user represents the amount of time the process spent with the CPU in user-mode (Non-kernel mode).
	This would be used to perform actions where the intervention of the kernel is not necessary.
   sys represents the amount of time the process spent with the CPU in kernel-mode. This would be
	for any segments of the process that must have the kernel intervene in order to accomplish
	instructions in the code. An example of this would be all system calls.
2. As seen in the above outputs of time with the serial and parallel programs, the user time is 
	longer than the sys time in both serial and parallel. This is because both the programs 
	spend more time needing to execute in user-mode and not needing intervention from the kernel.
	If the programs made use of more system calls or more priviliged instructions then we could
	see a potential extension of the sys times in both programs possibly reaching past the user
	time. However, as the code is currently written, the bulk of the programs are ran NOT in
	kernel-mode. I would assume that the user time is much longer than the sys time because
	of the many loops and boolean checks (if statements, conditionals, etc) that are included in
	each program.
3. Looking back at the time outputs for both serial and parallel, we can see that the parallel
	process ran faster than the serial process, and thus parallel finishes faster. I believe 
	this to be due to the fact that the parallel program makes more children at once than
	the serial program does. And because of this, it allows for the CPU to be shared to a 
	wider group of processes that stem from parallel. This means that when the CPU ultimately
	does do a context switch in order to share its threads with other processes, the CPU 
	probably makes more than a few context switches to children of the parallel parent. This
	would allow for the children and the parallel process as a whole to finish faster than
	serial. Serial on the other hand only creates one child at a time, and does not create 
	another until the previous child has been terminated. This leaves no room for the CPU
	to possibly context switch to another child of parallel, because there is only a singular
	child that exists at any one time. Parallel allows all children to run their processes
	at the same time, while serial only allows one child to be created, run their process, and
	finally terminate one at a time sequentially. 
4. In each of the fetcher programs, if a child crashes, this will thankfully not affect the other
	children in any way. This is due to the fact that nothing that the child process needs or
	uses comes from the other children of the fetcher parent process. For all intensive
	purposes, the other children are individual from the other children. This crash could 
	affect the wait() system call from the parent however, due to the fact that the child is
	termiated abnormally, however, this itself does not affect the children though. The benefit
	of isolating each fetch task within a difference process allows for the behavior that 
	I just spoke about. No child crashing should affect the other children in our scenario,
	however, if you were to say, allow the child to fork() into other children that would
	perhaps fetch data as well, this could indeed affect any children that had a child-parent
	that terminated abnormally. So in conclusion, it seems very safe and more stable and
	dependable to isolate each fetch within a different process. This could even save a
	potential crash of the entire main process if these fetch calls were not isolated from 
	the parent process. So all in all, it seems that fetching in isolated processes seems 
	very helpful and more secure than alternatives.


External References used: 
stackoverflow.com/questions/556405/what-do-real-user-and-sys-mean-in-the-output-of-time1 for information on the time program.

*** END README ***
