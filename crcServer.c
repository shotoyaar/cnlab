#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>

#define POLYNOMIAL 0x1021

unsigned short crc_rem(char* message, int len) {
    unsigned int remainder = 0;
    int byte, bit;

    for (byte = 0; byte < len; byte++) {
        remainder ^= (message[byte] << 8);
        for (bit = 0; bit < 8; bit++) {
            if (remainder & 0x8000) {
                remainder = (remainder << 1)^POLYNOMIAL;
            }

            else {
                remainder = remainder << 1;
            }
        }
    }

    return remainder;
}

int main() {
    int sock, csock, clilen, PORT;
    struct sockaddr_in servadder, cliadder;
    char buffer[1024];

    printf("Enter server port: \n");
    scanf("%d", &PORT);

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {printf("Socket not created.\n"); exit(EXIT_FAILURE);}
    else printf("Socket created.\n");

    memset(&servadder, 0, sizeof(servadder));

    servadder.sin_family = AF_INET;
    servadder.sin_port = htons(PORT);
    servadder.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock, (struct sockaddr*)&servadder, sizeof(servadder)) < 0) printf("Binding unsuccessful.\n");
    else printf("Binding successful.\n");

    listen(sock, 5);
    clilen = sizeof(cliadder);

    csock = accept(sock, (struct sockaddr*)&cliadder, &clilen);

    if (csock < 0) printf("Not accepted.\n");
    else printf("Accepted.\n");

    while (1) {
        recv(csock, &buffer, sizeof(buffer)+1, 0);
        printf("Received message: %s\n", buffer);
        printf("CRC remainder: %hu\n", crc_rem(buffer, strlen(buffer)));
    }

    return 0;
}