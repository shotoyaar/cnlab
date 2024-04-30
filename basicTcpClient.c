#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>

int main() {
    int clisockid, cliport, len;
    char sendmsg[100], recvmsg[100];
    struct sockaddr_in servaddr;

    printf("Enter server port:\n");
    scanf("%d", &cliport);

    clisockid = socket(AF_INET, SOCK_STREAM, 0);

    if (clisockid < 0) {
        printf("Socket creation failed.\n");
    }

    else {
        printf("Socket successfully created.\n");
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(cliport);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (connect(clisockid, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        printf("Couldn't connect.\n");
    }

    else {
        printf("Connected.\n");
    }

    recv(clisockid, recvmsg, 100, 0);

    printf("Received message: %s\n", recvmsg);
    return 0;
}
