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

int connect_to_server()
{
	int sock;
	struct sockaddr_in address;
	socklen_t addrlen;
	
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		puts("Failed to create socket");
		return -1;
	}
	
	address.sin_family = AF_INET;
	address.sin_port = htons(PORT);
	
	if (inet_pton(AF_INET, IP, &address.sin_addr) <= 0)
	{
		puts("Invalid address");
		return -1;
	}
	
	while (1)
	{
		addrlen = sizeof(address);
		if (connect(sock, (struct sockaddr *)&address, addrlen) < 0)
		{
			puts("Failed to connect");
			sleep(5); // wait 5 seconds before attempting to connect again
			// NOTE: I THINK THAT THE WAY THIS WILL WORK IS THAT IF YOU ARE BELOW THE BUFFER SIZE ON listen()
			// THEN IT WILL ''SUCCEED'' AND JUST WAIT FOR A THREAD TO BE OPEN; IF THERE ARE TOO MANY REQUESTS FOR 
			// THE BUFFER THEN IT WILL ENTER THIS CONDITION AND WAIT 5 SECONDS BEFORE TRYING AGAIN
		}
		else 
		{
			//puts("Connection successful");
			return sock;
		}
	}
}

int main()
{
	char message[BUFFER_SIZE-24];
	char buffer[BUFFER_SIZE];
	
	int sock = connect_to_server();
	if (sock < 0) puts("There was an error creating the client socket");
	
	strcpy(message, "4");
	send(sock, &message, strlen(message), 0);
	recv(sock, &buffer, BUFFER_SIZE, 0);
	puts(buffer);/*
	
	// TODO - Handle menu transmission back and forth
	while (1)
	{
		recv(sock, buffer, BUFFER_SIZE, 0);
		puts(buffer); // menu should be formatted properly for printing; unless someone wants to deal with parsing it
		
		// get user input here (should be an integer menu choice) (sprintf() the input into message)
		send(sock, message, strlen(message), 0);
		
		// check the value of user input here; enter a different condition for user input 
		// IE; if user wants to create a reservation; need to get user input many more times than for other options
		// not sure if should be one line at a time or one subject at a time (like name, then birthdate or whatever)
		// something to consider is that the server could send back commands telling the client which function
		// to enter; but that seems like unnecessary work; I believe the important part is that the server sends the menu 
		// and actually does something with the options
		// client is used only for printing output and entering data
		
		// anyways enter the relevant input loop for the option selected
		// if 5 selected; send it to the server and break;
		// if anything besides 1-5 selected; print complaint and restart loop
		// once an option is completed you should restart the loop
		
	}*/
	
	
	close(sock);
	puts("client exited");
	return 0;
}