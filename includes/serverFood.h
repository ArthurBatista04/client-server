#ifndef SERVERCUSTOMER_H
#define SERVERCUSTOMER_H
#include <arpa/inet.h>

typedef struct FOOD_t
{
    int id;
    float price;
    char category[100];
    char name[100];
    char origin[100];
} FOOD;

typedef struct DATABASE_t
{
    FOOD foods[100];
} DATABASE;

typedef struct STATUS_t
{
    int code;
    char message[100];
} STATUS;

typedef struct DATA_t
{
    int method;
    FOOD food;
    STATUS status;
} DATA;

void initDatabase(DATABASE *database);
FOOD searchFoodById(int id, DATABASE *database);
DATABASE database;
FOOD food;
#endif
