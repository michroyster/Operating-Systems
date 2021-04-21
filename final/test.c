#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>

void func(char *str){
    printf("%s\n", str);
}


int main(){
    // char arr[3][9][3];
    // char temp[3];
    // char first = 'A';
    // int num = 1;

    // for (int i = 0; i < 3; i ++){
    //     for (int j = 0; j < 9; j++){
    //         sprintf(temp, "%c%d", first + i, num + j);
    //         strcpy(arr[i][j], temp);
    //     }
    // }

    //     for (int i = 0; i < 3; i ++){
    //         for (int j = 0; j < 9; j++){
    //             printf("%s\n", arr[i][j]);
    //     }
    // }
    sem_unlink("/file_semaphore");

    return 0;    
}