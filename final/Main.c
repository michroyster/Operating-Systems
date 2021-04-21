#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <semaphore.h>
#include "Reservation.h"
#include "Server.h"


int main(){
    sem_t *file_semaphore = sem_open("/file_semaphore", O_CREAT, 0666, 1);
    printf("the value of semaphore is: %ld\n", file_semaphore->__align);
    // if ((file_semaphore = sem_open("/file_semaphore", O_CREAT, 0666, 1)) == SEM_FAILED)
    // {
    //     perror("Error opening semaphore!\n");
    // }
    printf("hi\n");
    if(fork() == 0){
        Server('B');
        if(fork() == 0){
            Server('C');
            if (fork () == 0){
                Server('D');
            }
        }
    }else{
        Server('A');
    }
    wait(NULL);
    printf("bye\n");
    return 0;
}