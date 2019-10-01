#ifndef SERVERCUSTOMER_H
#define SERVERCUSTOMER_H
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <pthread.h>
#include <semaphore.h>

typedef struct DRINK_t
{
    int id;
    float price;
    char brand[100];
    char name[100];
} DRINK;

typedef struct DATABASE_t
{
    DRINK drinks[100];
} DATABASE;

typedef struct STATUS_t
{
    int code;
    char message[100];
} STATUS;

typedef struct DATA_t
{
    int method;
    DRINK drink;
    STATUS status;
} DATA;

sem_t mutex;
void initDatabase(DATABASE *database);
DRINK searchDrinkById(int id, DATABASE *database);
DATABASE database;
DRINK drink;
#endif
