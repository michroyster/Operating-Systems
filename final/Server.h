// Author: Michael Royster
#ifndef SERVER_H
#define SERVER_H

void    Server(char name);
void    get_date(char* date, char* tomorrow);
int     file_exists(char *filename);
void    make_reservation(char server, Reservation reservation);
void    inquiry(char* ticket, char* info);
void    update_train_seats(char* ticket, char* seats); // modify
void    cancel_reservation(char* ticket);
void    available_seats(int date, char* options);

#endif