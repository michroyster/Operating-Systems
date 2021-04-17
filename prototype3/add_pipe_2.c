// Author: Dr. Shital Joshi
// Email: Shital.Joshi@okstate.edu

// Description: Using named pipe to pass data between the two processes

// Run this second.

#include <stdio.h> 	// Baisc standard I/O like printf
#include <string.h> 	// required for strlen
#include <unistd.h> 	// defines standard symbolic constants and types
#include <sys/types.h> 	// basic derived types like pid_t
#include <sys/stat.h> 	//required for mkfifo (i.e. defines data returned by the stat() function: get file status)
#include <fcntl.h> 	//required for open (i.e. file control options)

#define MAX 9000000000 // 9 billion times

int main(){
	int fd; // file descriptor

	long long int result = 0;	// computing results for addition
	long long int finalResult;	// computing final results for addition

	long long int i;			// loop counter
	
	// Perform addition
	for (i = MAX/2+1; i <= MAX; i++){
		result = result + i;
	}
	
	char myPipe[] = "./NamedPipe"; 	// FIFO file path 
	mkfifo(myPipe, 0666);	// Creating the named file(FIFO) 
	

	if ((fd = open(myPipe, O_RDONLY)) < 0) 	// Open FIFO for Read only
		printf("2: Error opening..");
		
	if (read(fd, &finalResult, sizeof(long long int)) < 0)	// Read from FIFO 
		printf("2: Error reading..");

	// Display the results
	printf("\nThe final result is: %lld\n", (result + finalResult)); 	// Print the read message 

	close(fd); 			// Close the pipe
	
	return 0;

}
