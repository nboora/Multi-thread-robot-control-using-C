# Multi-thread-robot-control-using-C
Developed a multi thread robot control protocol using TCP/IP.



Programs execution: -
The execution of the code is mainly intended to meet and show that the project has met the requirements of
the course project using a multi-threaded programming. Since the libraries that are used are windows and
Linux based, in order to show the output and to enable an individual to run in any compiler to check the
functionality of the programs the make file is created in order to generate the executable file. The make is
performed in the raspberry pi in order to support the Linux libraries used in the program. Once the make is
done the executable files for the main server, sub servers and the client servers are generated in the raspberry
pi. These executable files can be run in any terminal to show the working of the code. Since the course work
emphasizes on using the Ch command shell. I executable is ran on the Ch terminal in the raspberry pi to
show the functioning of the code.

Creating the Makefile: -
The make file is created using the gcc compiler to compile the code and generate the executable files for the
servers. The make commands use -pthread at the end this will enable the gcc to use the pthread library while
compiling generating the executable file. Use make or make all command in raspberry pi terminal.

Commands to run the executable files: -
The make file should be run in the raspberry pi to get the executable files. After that following commands
should be used in individual terminals and open ch in them to run the executable in the ch terminal. Once the
connection is established and if there is an error after entering either kinematics, path planning or pick and
place in the client terminal there should be a change that needs to be made in client code (line 101
“ip[strlen(ip)-1 = ‘\0’]”). When I run this in the windows Linux virtual linux terminal this line should be
uncommented and if it is running in the raspberry pi it should be commented.
./main_server
./sub_1
./sub_2
./sub_3
./client


