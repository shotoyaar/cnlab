#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define MAX_BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8888

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[MAX_BUFFER_SIZE];
    int send_len, recv_len;

    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_IP, &(server_addr.sin_addr)) <= 0) {
        perror("Invalid server address");
        exit(EXIT_FAILURE);
    }

    // Get the list of numbers from the user
    printf("Enter the number of elements: ");
    fgets(buffer, MAX_BUFFER_SIZE, stdin);
    int n = atoi(buffer); // Get the number of elements in the list

    printf("Enter the list of numbers: ");
    fgets(buffer, MAX_BUFFER_SIZE, stdin);

    // Send the list of numbers to the server
    send_len = sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (send_len < 0) {
        perror("Send failed");
        exit(EXIT_FAILURE);
    }

    // Receive the head numbers from the server
    recv_len = recvfrom(sockfd, buffer, MAX_BUFFER_SIZE, 0, NULL, NULL);
    if (recv_len < 0) {
        perror("Receive failed");
        exit(EXIT_FAILURE);
    }

    // Print the head numbers in the client terminal
    printf("Head Numbers: %.*s\n", recv_len, buffer);

    // Close the socket
    close(sockfd);

    return 0;
}
