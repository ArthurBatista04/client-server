#ifndef SOCKET_H
#define SOCKET_H
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#pragma pack(1)
#define POST 1
#define GET 2
int createSocket(int port);
void closeSocket(int sock);
void sendMsg(int sock, void *msg, uint32_t msgsize);
#endif
