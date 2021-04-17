#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    printf("Hello world from other file.\n");
    sleep(3);
    printf("More text.\n");
    sleep(3);
    return 0;
}