#include "includes/socket.h"

#pragma pack()

int main()
{
    int portClient = 38110;
    int portCustomer = 38210;
    int portDrink = 38310;
    int portFood = 38410;
    int ssock, csock;
    int customerSocket, drinkSocket, foodSocket;
    int nread;
    int fd;
    struct sockaddr_in client;
    int clilen = sizeof(client);
    pid_t pid;

    ssock = createSocket(portClient);
    customerSocket = connectServer(portCustomer);
    drinkSocket = connectServer(portDrink);
    foodSocket = connectServer(portFood);
    printf("Proxy listening on port %d\n", portClient);

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
            void *buf;
            printf("Accepted connection from %s\n", inet_ntoa(client.sin_addr));
            while ((nread = read(csock, &buf, 416))) // 216 bytes for customer | 416 bytes for food | 316 bytes for drink
            {
                void *serverMsg;
                printf("\nReceived %d bytes\n", nread);
                int serverSize;

                switch (nread)
                {
                case 216:
                    printf("Customer object identified. Sending object to customer server..\n");
                    sendMsg(customerSocket, &buf, 216);
                    serverSize = read(customerSocket, &serverMsg, 216);
                    break;
                case 316:
                    printf("Drink object identified. Sending object to drink server..\n");
                    sendMsg(drinkSocket, &buf, 316);
                    serverSize = read(drinkSocket, &serverMsg, 316);
                    break;
                case 416:
                    printf("Food object identified. Sending object to food server..\n");
                    sendMsg(foodSocket, &buf, 416);
                    serverSize = read(foodSocket, &serverMsg, 416);
                    break;
                // number of bytes doesn't match any server
                default:
                    printf("Error! Not a valid data object.");
                    return -1;
                }
                sendMsg(csock, &serverMsg, serverSize);
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
