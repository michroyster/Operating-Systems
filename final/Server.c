#include <stdio.h>
#include <string.h> 	
#include <unistd.h> 
#include <sys/types.h> 	
#include <sys/stat.h> 	
#include <fcntl.h> 		
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <arpa/inet.h> 	// client
#include <sys/socket.h>
#include <netinet/in.h> // server
#include <pthread.h>
#include <semaphore.h>
#include <ctype.h>

#define IP "127.0.0.1"
#define PORT 8019
 
#define BUFFER_SIZE 1024
#define THREAD_POOL_SIZE 1

sem_t semaphore, ready;
int num_seats = 27;
int thread_in_use[THREAD_POOL_SIZE] = {0};

void test(int purchased_seats)
{
	if (purchased_seats > num_seats)
	{
		// do something
		puts("not enough seats left");
	}
	num_seats -= purchased_seats;
	char out[128];
	sprintf(out, "Number of seats remaining: %d", num_seats);
	puts(out);
	sleep(4);
}

void *server_thread(void *arg)
{
	int purchased_seats, sock, client_choice;										// variables
	char buffer[BUFFER_SIZE] = {0};													// variable for receiving FROM client 
	char message[BUFFER_SIZE-24];													// variable for sending TO client 
	
	char *temp = (char *)arg;														// convert the void * argument into a char array
	char *t = strtok(temp, ",");													// get the first string token
	char *tt = strtok(NULL, ",");													// get the second string token 
	
	sock = atoi(t);																	// convert the first string token into an integer (sock)
	int id = atoi(tt);																// convert the second string token into an integer (thread_id)
	
	//sock = *((int *)arg);															// old int (socket number) argument only for thread 
	
	recv(sock, buffer, BUFFER_SIZE, 0);
	purchased_seats = atoi(buffer);													// example user input 
	
	//sem_wait(&semaphore);
	test(purchased_seats);															// example semaphore access control
	//sem_post(&semaphore);
	
	sprintf(message, "You bought %d tickets, there are now %d seats remaining\n", purchased_seats, num_seats);
	send(sock, message, BUFFER_SIZE, 0);											// example send method
	/*
	// TODO - Fully flesh out the menu transmission between Client/Server
	while(1)
	{
		// send_menu();
		recv(sock, buffer, BUFFER_SIZE, 0);
		client_choice = atoi(buffer);
		if (buffer == 1) make_reservation();										// these functions will need to communicate with the user according to their specifications; and parse their input into variables
		else if (buffer == 2) make_inquiry();										// whether they will call someone else's functions; or be implemented into them; I don't know
		else if (buffer == 3) modify_reservation();
		else if (buffer == 4) cancel_reservation();
		else if (buffer == 5) break;												// breaks while loop if user enters 5
		else send_error(); 															// tells the user to enter a correct choice
	}	*/
	
	//send_exit_message();
	
	//strcpy(buffer, "");
	//strcpy(message, "");
	
	thread_in_use[id] = 0;
	
	//puts(tt);																		// print the thread_id (debugging purposes)

	close(sock);																	// close the socket with the current client 
	sem_post(&ready);																// signal that there is a thread open for a new client
	//puts("subthread exited");
	pthread_exit(NULL);																// exit the thread 
}

int Server(char name, int port)
{
	int server_fd, sock, thread_id = 0;
	struct sockaddr_in address;
	struct sockaddr_storage storage;
	socklen_t addrlen;																// declare the address length
	pthread_t thread_pool[THREAD_POOL_SIZE];										// declare the thread pool array
	
	sem_init(&semaphore, 0, 1);														// initialize an example semaphore used to limit access to a variable
	sem_init(&ready, 0, THREAD_POOL_SIZE);											// initialize the semaphore used for enforcing the limit on the threadpool; and waiting 
																						// threads to be available
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)							// try to create the server_fd socket
	{
		puts("Server socket creation failed\n");									// server_fd socket creation failed
		return 1;																	// exit program
	}
	else puts("Server socket creation success\n");									// server_fd socket creation succeeded
	
	address.sin_family = AF_INET;													// set ipv4
	address.sin_port = htons(port);													// set the port
	address.sin_addr.s_addr = INADDR_ANY;											// set the IP? address(es) that the socket can connect to(from?)
	
	if ((bind(server_fd, (struct sockaddr *)&address, sizeof(address))) < 0)		// try to bind the server_fd socket
	{
		puts("Server socket bind failed\n");										// binding failed
		return 1;																	// exit program
	}
	else puts("Server socket bind success");										// binding succeeded
	
	if (listen(server_fd, THREAD_POOL_SIZE * 3) < 0)								// try to listen on the server_fd socket
	{
		puts("Server socket listen failed");										// listening failed
		return 1;																	// exit program
	}
	else puts("Listening...");														// listening did not fail
	
	while(1)																		// infinite loop
	{
		sem_wait(&ready); 															// wait for a thread to be available
		
		addrlen = sizeof(storage);													// set the address length
		if ((sock = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0) 	// create a socket for the thread and new client
		{
			puts("Server socket accept failed");
			return 1;
		}
		//else puts("Server socket accept success");
		
		while (thread_in_use[thread_id])
		{
			thread_id++;
			if (thread_id >= THREAD_POOL_SIZE) thread_id = 0;
			
		}
		thread_in_use[thread_id] = 1; 		
		//		
		char temp[10];
		sprintf(temp, "%d,%d", sock, thread_id);
		puts(temp);
		pthread_create(&thread_pool[thread_id], NULL, server_thread, (void *)temp);
		//pthread_create(&thread_pool[thread_id], NULL, server, &sock); // call/create the thread with the new socket
		//pthread_create(&thread_pool[thread_id++], NULL, server, &sock); // call/create the thread with the new socket
	}
	
	close(server_fd);
	sem_destroy(&semaphore);
	sem_destroy(&ready);
	puts("server exited");
	return 0;
}

