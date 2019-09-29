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



typedef struct CUSTOMER_t {
    uint32_t id;
    char* nome;
} CUSTOMER;

typedef struct dataBase_t {
    CUSTOMER* customers[100];
} DATABASE;


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

void initDatabase(DATABASE database)
{
    for(int i=1; i<100; i++)
    {
        database.customers[i] = NULL;
    }
}



int main()
{
    int PORT = 38106;
    int BUFFSIZE = 512;
    char buff[BUFFSIZE];
    int ssock, csock;
    int nread;
    int fd;
    struct sockaddr_in client;
    int clilen = sizeof(client);
    pid_t pid;
    DATABASE database;
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
    initDatabase(database);
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
            bzero(buff, BUFFSIZE);
            while ((nread=read(csock, buff, BUFFSIZE)) > 0)
            {
                printf("\nReceived %d bytes\n", nread);
                CUSTOMER *p = (CUSTOMER*) buff;
                printf("Received contents: id=%d, counter=%s",
                        p->id, p->nome);
                
                read(fd, database.customers, sizeof(DATABASE));
                database.customers[p->id] = p;
                printf("\nInserted!");
                printf("\nSending message back to client.. ");
                message="Registered!";
                sendMsg(csock, message, sizeof(message));
                write(fd,  database.customers, sizeof(DATABASE));
            
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
