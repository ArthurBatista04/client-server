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


#pragma pack()
sem_t semaphore;
typedef struct person_t {
    char nome[1000];
    uint32_t id;
} person;

int createSocket(int port)
{
    
    int sock, err;
    struct sockaddr_in server;
    

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("ERRO: falha na hora de criar o Socket!\n");
        exit(1);
    }
    printf("Socket criado!\n");

    bzero((char *) &server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);
    if (bind(sock, (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        printf("ERRO: Bind falhou\n");
        exit(1);
    }
    printf("Bind concluído\n");

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
        printf("\nMensagem não enviada!\n");
        closeSocket(sock);
        exit(1);
    }
    printf("\nMensagem enviada com (%d bytes).\n", msgsize);
    return;
}

int main()
{
    struct sockaddr_in client;
    pid_t pid;
    int PORT = 38105;
    int BUFFSIZE = 512;
    char newUserBuffer[BUFFSIZE];
    char oldUserBuffer[BUFFSIZE];
    int ssock, csock;
    int nread;
    char *message;
    int clilen = sizeof(client);
    int fd0, fd1;
    char *myfifo = "/tmp/myfifo"; 
    int temp;
    person *database[100];
    mkfifo(myfifo, 0666); 
    ssock = createSocket(PORT);
    printf("Server ativo no port %d...\n", PORT);
    message="Criado com sucesso!";
    fd0 = open(myfifo, O_WRONLY);
    write(fd0,  message, sizeof(message));
    close(fd0);
    while (csock = accept(ssock, (struct sockaddr *)&client, &clilen)) // aguarandando cliente se concectar
    {
        if (csock < 0)
        {
            printf("Erro: accept() falhou\n");
            continue;
        }
        if((pid = fork()) < 0) // pid = 0 -> filho pid = 1 -> pai
        { 
            printf("Erro: fork() falhou!\n");
            return -1;
        }
        if (pid <= 0) //
        { 
            printf("Cliente ativo no endereço %s\n", inet_ntoa(client.sin_addr));
            bzero(newUserBuffer, BUFFSIZE);
            while (nread = read(csock, newUserBuffer, BUFFSIZE)>0) // esperando resposta do cliente
            {
                 printf("\nReceived %d bytes\n", nread);
                person *newUser = (person*) newUserBuffer;
                printf("Conteúdo recebido: id=%d\n",
                        newUser->id);

                fd1 = open(myfifo, O_RDONLY);
                read(fd1, database, sizeof(database));
                close(fd1);
                database[newUser->id] = newUser;
                printf("\nDado inserido!");
                printf("\nEnviando resposta ao cliente.. ");
                
                sendMsg(csock, message , strlen(message));
               
            }
        }        
    }

    closeSocket(ssock);
    printf("Finalizado!");
    return 0;
}