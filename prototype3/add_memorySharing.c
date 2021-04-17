// Author: Dr. Shital Joshi
// Email: shital.joshi@okstate.edu
// Description: POSIX Interprocess communication using memory sharing

// Run: Use gcc add_messagePassing.c -lrt

#include <stdio.h>		// for printf
#include <time.h>		// for clock()
#include <unistd.h>		// for fork
#include <stdlib.h>		// for exit(1)
#include <sys/ipc.h> 	// Used by IPC maechanisms: messages, shared memory and semaphores
#include <string.h>		// for string operation: strlen
#include<sys/wait.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

#define QUEUE_NAME   "/addition"
#define PERMISSIONS 0660
#define MAX_MESSAGES 10
#define MAX_MSG_SIZE 256
#define MSG_BUFFER_SIZE MAX_MSG_SIZE + 10

#define MAX 9000000000 // 9 billion times

int main() 
{ 
	clock_t t;
	t = clock();
	double time;

	/* size (in bytes) of shared memory object */
	const int SIZE = 1024;
		
	/* name of the shared memory object */
	const char *name = "CS4323";
	
    int pid = fork();	// make two processes

	/* shared memory file descriptor */
	int shm_fd;
		
	/* pointer to shared memory object */
	long long int * ptr;

	/* Variables to hold the result after computation */
	long long int result = 0, i;
		
	if (pid < 0){	// fork failed
		fprintf(stderr, "fork failed..\n");
		exit(1);
	}
	else if(pid == 0){ // Child process
	
		/* Create or open a shared memory object */
		shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666); 

		/* Set the shared memory object size */
		ftruncate(shm_fd, SIZE);

		/* Map the shared memory object into the current address space */
		ptr = mmap(0, SIZE,  PROT_WRITE, MAP_SHARED, shm_fd, 0);

		/* Add the numbers */
		for (i = 1; i <= MAX/2; i++){
			result = result + i;
		}

		/* Write to the shared memory object */
		*ptr = result;
	
	}
	else{	// Parent process

		long long int finalResult; // Final result
		
		for (i = MAX/2 + 1; i <= MAX; i++){
			result = result + i;
		}
		/* open the shared memory object */
		shm_fd = shm_open(name, O_RDONLY, 0666);

		wait(NULL);

		/* memory map the shared memory object */
		ptr = (long long int *) mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);

		/* read from the shared memory object and compute the final answer */
		finalResult = result + *ptr;
		
		printf("The final result is: %lld\n", finalResult);
	
		/* Unmap the shared memory */
		munmap(ptr, SIZE);
		
		/* Close the shared memory object */ 
		close(shm_fd);

		/* Delete the shared memory object */
		shm_unlink(name);
		
		t = clock() - t;
		time = (double)t/CLOCKS_PER_SEC;
		
		printf("\nTime taken: %f sec\n", time);
	}
    			
	return 0; 
} 