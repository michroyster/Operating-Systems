// Author: Michael Royster
#ifndef BACKEND_H
#define BACKEND_H

void    init_sync(sem_t *file_write, sem_t *file_read, int shm_fd, int *ptrReaders);
void    desync(sem_t *file_write, sem_t *file_read, int shm_fd, int *ptrReaders);
void    ServerX(char name);
void    get_date(char* date, char* tomorrow);
int     file_exists(char *filename);
void    make_reservation(char server, Reservation* reservation, int numberTravelers);
void    inquiry(char* ticket, Reservation* info);
void    update_train_seats(char* ticket,char* name, char* seats); // modify
void    cancel_reservation(char* ticket);
void    available_seats(int date, char* options);
void    receipt(Reservation *reservations, int numberTravelers, char server);
int     check_seat(int date, char *seat);
void    testX(char name);
#endif