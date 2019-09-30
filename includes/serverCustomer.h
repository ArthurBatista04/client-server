#ifndef SERVERCUSTOMER_H
#define SERVERCUSTOMER_H
#define POST 1
#define GET 2
#include <arpa/inet.h>

typedef struct CUSTOMER_t
{
    int id;
    int age;
    char nome[100];
} CUSTOMER;

typedef struct DATABASE_t
{
    CUSTOMER customers[100];
} DATABASE;

typedef struct STATUS_t
{
    int code;
    char message[100];
} STATUS;

typedef struct DATA_t
{
    int method;
    CUSTOMER customer;
    STATUS status;
} DATA;
void initDatabase(DATABASE *database);
CUSTOMER findCustomerByID(uint32_t id, DATABASE *database);
DATABASE database;
CUSTOMER customer;
#endif
