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
    Server('A');
    sleep(2);
    Server('B');
    return 0;
}