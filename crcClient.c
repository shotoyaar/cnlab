#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string.h>


int main() {
    int csock, PORT;
    struct sockaddr_in servadder;
    char message[1024], buffer[1024];

    printf("Enter server port: \n");
    scanf("%d", &PORT);

    if ((csock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {printf("Socket not created.\n"); exit(EXIT_FAILURE);}
    else printf("Socket created.\n");

    memset(&servadder, 0, sizeof(servadder));

    servadder.sin_family = AF_INET;
    servadder.sin_port = htons(PORT);
    servadder.sin_addr.s_addr = htonl(INADDR_ANY);

    if (connect(csock, (struct sockaddr*)&servadder, sizeof(servadder)) < 0) printf("Connection failed.\n");
    else printf("Connection successful.\n");

    while (1) {
        printf("Enter message: \n");
        fgets(message, 1024, stdin);

        send(csock, message, sizeof(message), 0);
        printf("Message sent.\n");\
    }

    return 0;
}