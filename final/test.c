#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <semaphore.h>
#include "Reservation.h"
#include "Backend.h"


int main(){
    // Create semaphores and shared memory object
    sem_t *file_write;
    sem_t *file_read;
    int shm_fd;
    int *ptrReaders;
    init_sync(file_write, file_read, shm_fd, ptrReaders);

    // create a bunch of processes to read at the same time
    char name = 'B';
    ServerX(name);
    // int parentid = getpid();
    // for (int i = 0; i < 5; i++){
    //     if (fork() == 0){
    //         ServerX(name + i);
    //         return 0;
    //     }
    // }

    // for(int j = 0; j < 5; j++){
    //     wait(NULL);
    // }

    // // desync
    desync(file_write, file_read, shm_fd, ptrReaders);

    // printf("%ld\n", sizeof(Reservation)*27);

    return 0;
}