#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include <time.h>

int main() {
    char sendmsg[100], recvmsg[100];
    struct sockaddr_in servaddr, cliaddr;
    int sockid, sockid2, nsd, clilen, sport, len;
    time_t ticks;

    printf("Enter the server port:\n");
    scanf("%d", &sport);

    sockid = socket(AF_INET, SOCK_STREAM, 0);

    if (sockid < 0) {
        printf("Socket creation failed.\n");
    }

    else {
        printf("Socket successfully created.\n");
    }

    ticks = time(NULL);
    strcpy(sendmsg, ctime(&ticks));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(sport);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    sockid2 = bind(sockid, (struct sockaddr*)&servaddr, sizeof(servaddr));

    if (sockid < 0) {
        printf("Binding failed.\n");
    }

    else {
        printf("Binding successful.\n");
    }

    listen(sockid, 5);
    clilen = sizeof(cliaddr);

    nsd = accept(sockid, (struct sockaddr*)&cliaddr, &clilen);
    
    if (nsd < 0) {
        printf("Cannot accept.\n");
    }

    else {
        printf("Accepted.\n");
    }

    send(nsd, sendmsg, strlen(sendmsg), 0);

    return 0;
}