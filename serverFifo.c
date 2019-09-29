#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h> 
#include <semaphore.h>
#include <sys/stat.h>
#include <fcntl.h>
#pragma pack(1)

#define POST 1


typedef struct CUSTOMER_t {
    uint32_t id;
    uint32_t age;
    char* nome;
    char* cpf;
} CUSTOMER;

typedef struct DATABASE_t {
    CUSTOMER customers[100];
} DATABASE;

typedef struct ERROR_t {
    char* message;
    uint32_t code;
} ERROR;

typedef struct DATA_t {
    uint32_t method;
    CUSTOMER customer;
    ERROR error;
} DATA;




#pragma pack()


int createSocket(int port)
{
    int sock, err;
    struct sockaddr_in server;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("ERROR: Socket creation failed\n");
        exit(1);
    }
    printf("Socket created\n");

    bzero((char *) &server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);
    if (bind(sock, (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        printf("ERROR: Bind failed\n");
        exit(1);
    }
    printf("Bind done\n");

    listen(sock , 3);

    return sock;
}

void closeSocket(int sock)
{
    close(sock);
    return;
}

void sendMsg(int sock, void* msg, uint32_t msgsize)
{
    if (write(sock, msg, msgsize) < 0)
    {
        printf("Can't send message.\n");
        closeSocket(sock);
        exit(1);
    }
    printf("Message sent (%d bytes).\n", msgsize);
    return;
}

void initDatabase(DATABASE* database)
{
    for(int i=1; i<100; i++)
    {
        database->customers[i].id = -1;
    }
}

CUSTOMER* findCustomerByCpf(char* cpf, DATABASE* database )
{
     for(int i=1; i<100; i++)
    {
        if(strcmp(database->customers[i].cpf, cpf) == 0) return &database->customers[i];
    }
    return NULL;
}

int main()
{
    int PORT = 38107;
    int BUFFSIZE = 512;
    DATA buff;
    int ssock, csock;
    int nread;
    int fd;
    struct sockaddr_in client;
    int clilen = sizeof(client);
    pid_t pid;
    DATABASE database;
    CUSTOMER *customer;
    char *myfifo = "/tmp/hello";
    char *message; 
    int mkFIFO;

    remove(myfifo); 
    mkFIFO = mkfifo(myfifo, 0666);
    if (mkFIFO < 0){
        perror("Error: mkfifo() failed = ");
        exit(1);
    } 
    ssock = createSocket(PORT);
    printf("Server listening on port %d\n", PORT);
    fd = open(myfifo, O_RDWR | O_TRUNC);
    write(fd,  database.customers, sizeof(DATABASE));
    initDatabase(&database);
    while (csock = accept(ssock, (struct sockaddr *)&client, &clilen))
    {
        if (csock < 0)
        {
            perror("Error: accept() failed = ");
            continue;
        }
        if((pid = fork()) < 0) 
        { 
            perror("Error: fork() failed = ");
            return -1;
        }
        if (pid <= 0)
        {
            printf("Accepted connection from %s\n", inet_ntoa(client.sin_addr));
            while ((nread=read(csock, &buff, BUFFSIZE)) > 0)
            {
                printf("\nReceived %d bytes\n", nread);
                read(fd, database.customers, sizeof(DATABASE));
                if (buff.method == POST)
                {
                    database.customers[buff.customer.id] = buff.customer;
                    printf("\nInserted!");
                    printf("\nSending message back to client.. ");
                    sendMsg(csock, &buff, sizeof(DATA));
                    write(fd,  database.customers, sizeof(DATABASE));
                }else
                {
                    customer = findCustomerByCpf(buff.customer.cpf, &database);   
                    if (customer == NULL){
                        printf("\nCustomer not found!");
                        printf("\nSending message back to client.. ");
                        buff.error.message = "Customer not found!";
                        buff.error.code = 404;
                    }
                }
               
            
            }
            printf("Closing connection to client\n");
            printf("----------------------------\n");
            closeSocket(csock);
        }
       
    }

    closeSocket(ssock);
    printf("bye");
    return 0;
}
