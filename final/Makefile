main:
	gcc -c -o server.o Server.c -pthread 
	gcc -c -o main.o Main.c
	gcc -o main main.o server.o -pthread

clean:
	rm *.o

remove:
	rm /dev/shm/sem.file_semaphore