#include "includes/socket.h"
#include "includes/serverFood.h"

#pragma pack()

void initDatabase(DATABASE *database)
{
    for (int i = 0; i < 100; i++)
    {
        database->foods[i].id = -1;
    }
}

FOOD searchFoodById(int id, DATABASE *database)
{
    FOOD fake;
    fake.id = -1;
    if (database->foods[id].id != -1)
    {

        return database->foods[id];
    }
    return fake;
}

int main()
{
    int PORT = 38110;
    int ssock, csock;
    int nread;
    int fd[2];
    struct sockaddr_in client;
    int clilen = sizeof(client);
    pid_t pid;
    if (pipe(fd) == -1)
    {
        perror("Error: pipe() failed = ");
        return 1;
    }
    ssock = createSocket(PORT);
    printf("Server listening on port %d\n", PORT);
    initDatabase(&database);
    write(fd[1], &database, sizeof(DATABASE));
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
                printf("\nReceived %d bytes\n", nread);
                read(fd[0], &database, sizeof(DATABASE));
                if (buff.method == POST)
                {
                    if (database.foods[buff.food.id].id != -1)
                    {
                        strcpy(buff.status.message, "This id has aldready been registered!");
                        buff.status.code = 500;
                        buff.food.id = -1;
                    }
                    else
                    {
                        database.foods[buff.food.id] = buff.food;
                        strcpy(buff.status.message, "Registered data");
                        buff.status.code = 200;
                    }
                }
                else if (buff.method == GET)
                {
                    food = searchFoodById(buff.food.id, &database);
                    strcpy(buff.status.message, "Food's data");
                    buff.status.code = 200;
                    if (food.id == -1)
                    {
                        strcpy(buff.status.message, "Food not found!");
                        buff.status.code = 404;
                    }
                    buff.food = food;
                }
                printf("\nSending message back to client.. ");
                sendMsg(csock, &buff, sizeof(DATA));
                write(fd[1], &database, sizeof(DATABASE));
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
