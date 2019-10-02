#include "includes/socket.h"
#include "includes/serverDrink.h"

#pragma pack()

void initDatabase(DATABASE *database)
{
    for (int i = 0; i < 100; i++)
    {
        database->drinks[i].id = -1;
    }
}

DRINK searchDrinkById(int id, DATABASE *database)
{
    DRINK fake;
    fake.id = -1;
    if (database->drinks[id].id != -1)
    {

        return database->drinks[id];
    }
    return fake;
}

int main()
{
    int PORT = 38310;
    int ssock, csock;
    int nread;
    struct sockaddr_in client;
    int clilen = sizeof(client);
    pid_t pid;
    key_t key = ftok("shmfiles", 68);
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    DATABASE *shmem = (DATABASE *)shmat(shmid, NULL, 0);
    memcpy(shmem, &database, sizeof(DATABASE *));
    ssock = createSocket(PORT);
    printf("Server listening on port %d\n", PORT);
    initDatabase(&database);
    sem_init(&mutex, 0, 1);
    while (csock = accept(ssock, (struct sockaddr *)&client, &clilen))
    {
        if (csock < 0)
        {
            perror("Error: accept() failed = ");
            continue;
        }
        if ((pid = fork()) < 0)
        {
            perror("Error: fork() failed = ");
            return -1;
        }
        if (pid <= 0)
        {
            DATA buff;
            printf("Accepted connection from %s\n", inet_ntoa(client.sin_addr));
            while ((nread = read(csock, &buff, sizeof(DATA))) > 0)
            {
                sem_wait(&mutex);
                printf("\nReceived %d bytes\n", nread);

                if (buff.method == POST)
                {
                    if (shmem->drinks[buff.drink.id].id != -1)
                    {
                        strcpy(buff.status.message, "This id has aldready been registered!");
                        buff.status.code = 500;
                        buff.drink.id = -1;
                    }
                    else
                    {
                        shmem->drinks[buff.drink.id] = buff.drink;
                        strcpy(buff.status.message, "Registered data");
                        buff.status.code = 200;
                    }
                }
                else if (buff.method == GET)
                {
                    drink = searchDrinkById(buff.drink.id, shmem);
                    strcpy(buff.status.message, "Drink's data");
                    buff.status.code = 200;
                    if (drink.id == -1)
                    {
                        strcpy(buff.status.message, "Drink not found!");
                        buff.status.code = 404;
                    }
                    buff.drink = drink;
                }
                printf("\nSending message back to client.. ");
                sendMsg(csock, &buff, sizeof(DATA));
                sem_post(&mutex);
            }
            printf("Closing connection to proxy\n");
            printf("----------------------------\n");
            closeSocket(csock);
        }
    }
    closeSocket(ssock);
    printf("bye");
    return 0;
}
