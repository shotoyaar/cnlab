#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>

#define PORT 12345
#define SERVER_IP "127.0.0.1"

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char message[1024]; // Increase buffer size to accommodate larger messages
    char buffer[1024];

    // Create client socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Error in socket creation");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(PORT);

    // Connect to server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error in connection");
        exit(EXIT_FAILURE);
    }

    // Prompt user to enter message
    printf("Enter message to send: ");
    fgets(message, sizeof(message), stdin);

    // Remove trailing newline character from message
    message[strcspn(message, "\n")] = '\0';

    // Send message to server
    if (send(client_socket, message, strlen(message), 0) < 0) {
        perror("Error in sending data");
        exit(EXIT_FAILURE);
    }

    // Close socket
    close(client_socket);

    return 0;
}