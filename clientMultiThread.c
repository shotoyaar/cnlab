#include <stdio.h>
#include <sys/types.h> 
#include <netinet/in.h> 
#include <string.h>
#include <pthread.h>

void *sendThread(void *arg) {
    int csd = *(int *)arg;
    char sendmsg[100];
    while (1) {
        printf("Enter the message to be sent: ");
        scanf("%s", sendmsg);
        send(csd, sendmsg, 100, 0);
    }
    return NULL;
}

void *receiveThread(void *arg) {
    int csd = *(int *)arg;
    char revmsg[100];
    while (1) {
        recv(csd, revmsg, 100, 0);
        printf("Message read: %s\n", revmsg);
    }
    return NULL;
}

int main() {
    int csd, cport;
    struct sockaddr_in servaddr;
    printf("Enter the port: ");
    scanf("%d", &cport);

    csd = socket(AF_INET, SOCK_STREAM, 0);
    if (csd < 0) {
        printf("Can't create socket\n");
        return 1;
    }
    printf("Socket is created\n");

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(cport);

    if (connect(csd, (struct sockaddr *)&servaddr, sizeof(servaddr))) {
        printf("Can't connect\n");
        return 1;
    }
    printf("Connected successfully\n");

    pthread_t send_tid;
    pthread_create(&send_tid, NULL, sendThread, &csd);

    pthread_t receive_tid;
    pthread_create(&receive_tid, NULL, receiveThread, &csd);

    pthread_join(send_tid, NULL);
    pthread_join(receive_tid, NULL);

    return 0;
}