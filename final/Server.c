// Author: Michael Royster
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include "Reservation.h"
#include "Server.h"

#define MAX_SEATS 27
#define BUFFER_SIZE 512

// need a semaphore for writing to file

void Server(char name){
    char server_name = name; // this should later be a parameter of Server(char name)

    Reservation reservation = {"Michael", "1-1-1900", "M", 12345, "4-16-2021", 1, "D2"};

    make_reservation(server_name, reservation);
    update_train_seats("4172021-3", "B3");

    char info[512];
    inquiry("4172021-3", info);
    printf("%s\n", info);

    cancel_reservation("4162021-2");
    
    char arr[3*9*4+1];
    available_seats(4162021, arr);
    printf("%s\n", arr);

}

// Get date for today and tomorrow
void get_date(char* today, char* tomorrow){
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(today, sizeof(char)*10, "%d-%d-%d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    snprintf(tomorrow, sizeof(char)*10, "%d-%d-%d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday+1);
}

// return 1 if filename is in directory
int file_exists(char *filename){
    DIR *d;
    struct dirent *dir;
    d = opendir(".");
    if (d){
        while ((dir =readdir(d)) != NULL){
            if (strcmp(filename, dir->d_name) == 0){
                closedir(d);
                return 1;
            }
        }
    }
    closedir(d);
    return 0;
}

// Has a critical section - need to use reservation date instead of current date
void make_reservation(char server, Reservation reservation){  
    
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char today[9];
    snprintf(today, sizeof(char) * 8, "%d%d%d", tm.tm_mon + 1, tm.tm_mday, tm.tm_year + 1900);
    
    char filename[24];
    sprintf(filename, "%s.txt", today);
    char buffer[BUFFER_SIZE];
    int tick = 1;
    FILE *file;
    

    if (file_exists(filename)){
        file = fopen(filename, "r");
        while(fgets(buffer, sizeof(buffer), file)){
            tick++;
        }
        fclose(file);
    }
    
    char ticket[24];
    sprintf(ticket, "%s-%d", today, tick);
    file = fopen(filename, "a");
    char buffer_out[BUFFER_SIZE];
    sprintf(buffer_out, "%s\t%c\t%s\t%s\t%s\t%d\t%s\t%s\tOG\n", ticket, server, reservation.customerName, reservation.dob, reservation.gender, reservation.govID, reservation.travelDate, reservation.seat);
    fprintf(file, "%s", buffer_out);
    fclose(file);
}

// Has critical section - Need to pack up the data to send over socket
void inquiry(char *ticket, char* info){
    // find ticket
    char date[9];
    for (int i = 0; i < 7; i++) *(date+i) = *(ticket+i);
    char filename[16];
    sprintf(filename, "%s.txt", date);
    if (file_exists(filename)){
        char buffer[512];
        char temp[512];
        char *token;
        int flag = 0;
        FILE *file = fopen(filename, "r");
        fgets(buffer, sizeof(buffer), file);
        while(!feof(file)){
            strcpy(temp, buffer);
            token = strtok(temp, "\t");
            if (strcmp(ticket, token) == 0){
                flag = 1;
                for (int i = 0; i < sizeof(buffer); i++){
                    *(info + i) = *(buffer + i);
                }
                fclose(file);
                return;
            }
            fgets(buffer, sizeof(buffer), file);
        }
        if (!flag) printf("Ticket not found.\n");
        fclose(file);
    }else {
        printf("Ticket not found!");
    }
}

// Has critical section
void update_train_seats(char* ticket, char* seat){
    // create filename from date
    char date[9];
    for (int i = 0; i < 7; i++) *(date+i) = *(ticket+i);
    char filename[16];
    sprintf(filename, "%s.txt", date);

    char buffer_in[MAX_SEATS][BUFFER_SIZE];
    char line[BUFFER_SIZE];
    char temp[BUFFER_SIZE];
    char buffer_out[BUFFER_SIZE];
    char *token;
    int count = 0;
    int flag = 0;
    FILE *file;
    if (file_exists(filename)){
        file = fopen(filename, "r");
        fgets(line, sizeof(line), file);
        while(!feof(file)){
            strcpy(temp, line);
            token = strtok(temp, "\t");
            if (strcmp(token, ticket) == 0){
                flag = 1;
                strcpy(buffer_out, token);
                strcat(buffer_out, "\t");
                for (int i = 0; i < 6; i++){
                    token = strtok(NULL, "\t");
                    strcat(buffer_out, token);
                    strcat(buffer_out, "\t");
                }
                strcat(buffer_out, seat);
                strcat(buffer_out, "\tMD");
                strcat(buffer_out, "\n");
                strcpy(buffer_in[count], buffer_out);
            }else{
                strcpy(buffer_in[count], line);
            }
            fgets(line, sizeof(buffer_in), file);
            count++;
        }
        if (!flag) printf("Ticket not found!\n");
        fclose(file);

        file = fopen(filename, "w");
        for (int i = 0; i < count; i++){
            fprintf(file, "%s", buffer_in[i]);
        }
        fclose(file);
    }else{
        printf("Ticket not found!\n");
    }
}

// Has critical section
void cancel_reservation(char* ticket){
        // create filename from date
    char date[9];
    for (int i = 0; i < 7; i++) *(date+i) = *(ticket+i);
    char filename[16];
    sprintf(filename, "%s.txt", date);

    char buffer_in[MAX_SEATS][BUFFER_SIZE];
    char line[BUFFER_SIZE];
    char temp[BUFFER_SIZE];
    char buffer_out[BUFFER_SIZE];
    char *token;
    int count = 0;
    int flag = 0;
    FILE *file;
    if (file_exists(filename)){
        file = fopen(filename, "r");
        fgets(line, sizeof(line), file);
        while(!feof(file)){
            strcpy(temp, line);
            token = strtok(temp, "\t");
            if (strcmp(token, ticket) == 0){
                flag = 1;
                count--;
            }else{
                strcpy(buffer_in[count], line);
            }
            fgets(line, sizeof(buffer_in), file);
            count++;
        }
        if (!flag) printf("Ticket not found!\n");
        fclose(file);

        file = fopen(filename, "w");
        for (int i = 0; i < count; i++){
            fprintf(file, "%s", buffer_in[i]);
        }
        fclose(file);
    }else{
        printf("Ticket not found!\n");
    }
}

// Has critical section
void available_seats(int date, char* options){
    char all_seats[3][9][4];
    char x[4];
    char first = 'A';
    int num = 1;
    for (int i = 0; i < 3; i ++){
        for (int j = 0; j < 9; j++){
            sprintf(x, "%c%d", first + i, num + j);
            strcpy(all_seats[i][j], x);
        }
    }
    char taken[MAX_SEATS][3];

    char filename[16];
    char buffer[BUFFER_SIZE];
    char temp[BUFFER_SIZE];
    char available[100];
    char *token;
    int count = 0;
    sprintf(filename, "%d.txt", date);
    FILE *file;
    if (file_exists(filename)){
        file = fopen(filename, "r");
        fgets(buffer, sizeof(buffer), file);
        strcpy(temp, buffer);
        while(!feof(file)){
            token = strtok(temp, "\t");
            for (int i = 0; i < 7; i++) token = strtok(NULL, "\t");
            strcpy(taken[count], token);
            fgets(buffer, sizeof(buffer), file);
            strcpy(temp, buffer);
            count++;
        }
    }else{
        printf("Ticket not found!\n");
    }
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 9; j++){
            for (int k = 0; k < count; k++){
                if (strcmp(all_seats[i][j], taken[k]) == 0){
                    strcpy(all_seats[i][j], "XX");
                }
            }
        }
    }
    char row[37];
    for (int i = 0; i < 3; i ++){
        for (int j = 0; j < 9; j++){
            if (strcmp(all_seats[i][j], "XX") != 0){
                strcat(options, all_seats[i][j]);
                strcat(options, " ");
            }
        }
    }
}