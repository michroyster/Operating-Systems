// Group: E
// Name: Jareth Harmon
// Email: jareth.harmon@okstate.edu
// Date: April 18, 2021

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
#define PORT 8018
#define BUFFER_SIZE 1024
#define THREAD_POOL_SIZE 1

sem_t semaphore;
int num_seats = 27;

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

void *server(void *arg)
{
	int purchased_seats, sock;
	char buffer[BUFFER_SIZE] = {0};
	char message[BUFFER_SIZE-24];
	
	sock = *((int *)arg);
	
	recv(sock, buffer, BUFFER_SIZE, 0);
	purchased_seats = atoi(buffer);
	
	sem_wait(&semaphore);
	test(purchased_seats);
	sem_post(&semaphore);
	
	sprintf(message, "You bought %d tickets, there are now %d seats remaining\n", purchased_seats, num_seats);
	send(sock, message, BUFFER_SIZE, 0);
	
	strcpy(buffer, "");
	strcpy(message, "");
	close(sock);
	//puts("subthread exited");
	pthread_exit(NULL);
}

int main()
{
	int server_fd, sock, thread_id = 0;
	struct sockaddr_in address;
	struct sockaddr_storage storage;
	socklen_t addrlen;
	pthread_t thread_pool[THREAD_POOL_SIZE];
	
	sem_init(&semaphore, 0, 1); // sem_t, thread(0) || process(not 0), value (max simultaneous entries to restricted section)
	
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		puts("Server socket creation failed\n");
		return 1;
	}
	else puts("Server socket creation success");
	
	address.sin_family = AF_INET;
	address.sin_port = htons(PORT);
	address.sin_addr.s_addr = INADDR_ANY;
	
	if ((bind(server_fd, (struct sockaddr *)&address, sizeof(address))) < 0)
	{
		puts("Server socket bind failed\n");
		return 1;
	}
	else puts("Server socket bind success");
	
	if (listen(server_fd, THREAD_POOL_SIZE) < 0)
	{
		puts("Server socket listen failed");
		return 1;
	}
	else puts("Listening...");
	
	while(1)
	{
		addrlen = sizeof(storage);
		if ((sock = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0)
		{
			puts("Server socket accept failed");
			return 1;
		}
		//else puts("Server socket accept success");
		
		pthread_create(&thread_pool[thread_id++], NULL, server, &sock);
		if (thread_id >= THREAD_POOL_SIZE)
		{
			thread_id = 0;
			while (thread_id < THREAD_POOL_SIZE)
			{
				pthread_join(thread_pool[thread_id++], NULL);
			}
			thread_id = 0;
		}
	}
	
	close(server_fd);
	sem_destroy(&semaphore);
	puts("server exited");
	return 0;
}