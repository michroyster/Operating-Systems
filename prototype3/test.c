#define _XOPEN_SOURCE 600 
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <libgen.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>


int main(){
    system("gnome-terminal -e ./hello");
    printf("Hello world");
    return 0;
}