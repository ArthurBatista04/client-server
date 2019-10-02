#include "includes/socket.h"
#include "includes/serverCustomer.h"

#pragma pack()

void initDatabase(DATABASE *database)
{
    for (int i = 0; i < 100; i++)
    {
        database->customers[i].id = -1;
    }
}

CUSTOMER findCustomerByID(uint32_t id, DATABASE *database)
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
    int PORT = 38210;
    int ssock, csock;
    int nread;
    int fd;
    struct sockaddr_in client;
    int clilen = sizeof(client);
    pid_t pid;
    char *myfifo = "/tmp/hello";
    int mkFIFO;

    remove(myfifo);
    mkFIFO = mkfifo(myfifo, 0666);
    if (mkFIFO < 0)
    {
        perror("Error: mkfifo() failed = ");
        exit(1);
    }
    ssock = createSocket(PORT);
    printf("Server listening on port %d\n", PORT);
    fd = open(myfifo, O_RDWR | O_TRUNC);
    initDatabase(&database);
    write(fd, &database, sizeof(DATABASE));

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
                read(fd, &database, sizeof(DATABASE));
                if (buff.method == POST)
                {
                    if (database.customers[buff.customer.id].id != -1)
                    {
                        strcpy(buff.status.message, "This id has aldready been registered!");
                        buff.status.code = 500;
                        buff.customer.id = -1;
                    }
                    else
                    {
                        database.customers[buff.customer.id] = buff.customer;
                        strcpy(buff.status.message, "Registered data");
                        buff.status.code = 200;
                    }
                }
                else if (buff.method == GET)
                {
                    customer = findCustomerByID(buff.customer.id, &database);
                    strcpy(buff.status.message, "Customer's data");
                    buff.status.code = 200;
                    if (customer.id == -1)
                    {
                        strcpy(buff.status.message, "Customer not found!");
                        buff.status.code = 404;
                    }
                    buff.customer = customer;
                }
                printf("\nSending message back to client.. ");
                sendMsg(csock, &buff, sizeof(DATA));
                write(fd, &database, sizeof(DATABASE));
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
