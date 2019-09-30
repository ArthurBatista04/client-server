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
    int id;
    int age;
    char* nome;
} CUSTOMER;

typedef struct DATABASE_t {
    CUSTOMER customers[100];
} DATABASE;

typedef struct STATUS_t {
    int code;
} STATUS;

typedef struct DATA_t {
    int method;
    CUSTOMER customer;
    STATUS status;
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
    for(int i=0; i<100; i++)
    {
        database->customers[i].id = -1;
    }
}

CUSTOMER findCustomerByID(uint32_t id, DATABASE *database )
{
    CUSTOMER fake;
    fake.id = -1;
    if (database->customers[id].id != -1)
    {

        return database->customers[id];
    }
    return fake;
    
}

int main()
{
    int PORT = 38109;
    int ssock, csock;
    int nread;
    int fd;
    struct sockaddr_in client;
    int clilen = sizeof(client);
    pid_t pid;
    DATABASE *database;
    CUSTOMER customer;
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
    initDatabase(database);
    write(fd,  database, sizeof(DATABASE));
    
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
            DATA buff;
            printf("Accepted connection from %s\n", inet_ntoa(client.sin_addr));
            while ((nread=read(csock, &buff, sizeof(DATA))) > 0)
            {
                printf("\nReceived %d bytes\n", nread);
                read(fd, database, sizeof(DATABASE));
                if (buff.method == POST)
                {
                    if(database->customers[buff.customer.id].id != -1)
                    {
                        printf("\nThis id has aldready been registered!");
                        buff.status.code = 500;
                        buff.customer.id = -1;
                    } else
                    {
                        database->customers[buff.customer.id] = buff.customer;
                        printf("\nInserted!");
                        buff.status.code = 200;
                    }
                    printf("\nSending response back to client.. ");
                   
                    
                }else
                {
                    customer = findCustomerByID(buff.customer.id, database);
                    if (customer.id == -1){
                        printf("\nCustomer not found!");
                        printf("\nSending message back to client.. ");
                        buff.status.code = 404;
                    }
                    buff.customer = customer;
                }
                sendMsg(csock, &buff, sizeof(DATA));
                write(fd,  database, sizeof(DATABASE));
               
            
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
