// Author: Dr. Shital Joshi
// Email: Shital.Joshi@okstate.edu

// Description: Using named pipe to pass data between the two processes

// Run this first.

#include <stdio.h> 	// Baisc standard I/O like printf
#include <unistd.h> 	// defines standard symbolic constants and types
#include <sys/types.h> 	// basic derived types like pid_t
#include <sys/stat.h> 	//required for mkfifo (i.e. defines data returned by the stat() function: get file status)
#include <fcntl.h> 	//required for open (i.e. file control options)

#define MAX 9000000000 // 9 billion times

int main(){
	int fd; // file descriptor
	
	long long int result = 0;
	long long int i;

	for (i = 1; i <= MAX/2; i++){
		result = result + i;
	}
	
	printf("Computation done...\n");
	
	char myPipe[] = "./NamedPipe"; 	// FIFO file path 
	mkfifo(myPipe, 0666);	// Creating the named file(FIFO)
	
	
   	if ((fd = open(myPipe,O_WRONLY | O_CREAT)) < 0) 	// Open FIFO for write only
		printf("1: Error opening..");
		
	if (write(fd, &result, sizeof(long long int)) < 0) // Write on the FIFO
		printf("1: Error writing..");
		
    close(fd); 			// Close the pipe

	return 0;

}
