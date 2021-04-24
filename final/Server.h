#ifndef SERVER_H
#define SERVER_H

void test(int purchased_seats);
void *server_thread(void *arg);
int Server(char name, int port);

#endif