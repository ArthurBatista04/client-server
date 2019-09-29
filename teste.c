#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char* argv[])
{
    int fds[2];
    char *myfifo = "/tmp/serwer";
    char *myfifo2 = "/tmp/client";
    int x=mkfifo(myfifo2,0666);
    printf("%d",x);
    // fds[0]=open(myfifo,O_RDONLY);
    // fds[1]=open(myfifo2,O_WRONLY);

    // char tab[BUFSIZ];
    // memset(tab, 0, sizeof(tab));

    // write(fds[1],"klient",6);
    // perror("Write:"); //Very crude error check
    // read(fds[0],tab,sizeof(tab));
    // perror("Read:"); // Very crude error check

    // printf("Odebrano od serwera: %s\n",tab);

    // close(fds[0]);
    // close(fds[1]);
    // unlink(myfifo2);
    return 0;
}