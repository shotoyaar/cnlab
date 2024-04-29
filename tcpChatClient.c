#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>

int main() {
    int csd, cport, len;
    char sendmsg[100], recvmsg[100];

    struct sockaddr_in servaddr;

    printf("Enter the server port: \n");
    scanf("%d", &cport);

    csd = socket(AF_INET, SOCK_STREAM, 0);

    if (csd < 0) printf("Socket creation failed\n");
    else printf("Socket created successfully\n");

    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(cport);
    servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (connect(csd, (struct sockaddr*)&servaddr, sizeof(servaddr)) > 0) printf("Can't connect to server\n");
    else printf("Connected to server\n");

    while (1) {
        printf("Enter the message to send to server: \n");
        fgets(sendmsg, 100, stdin);
        send(csd, sendmsg, strlen(sendmsg)+1, 0);

        recv(csd, recvmsg, 100, 0);
        printf("Message recieved from server: %s\n", recvmsg);
    }

    return 0;
}