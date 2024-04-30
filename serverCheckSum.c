#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>

#define PORT 12345
#define POLYNOMIAL 0x1021 // CRC-CCITT (16-bit)

// CRC function
unsigned short crc_remainder(char *message, int len) {
    unsigned int remainder = 0;
    int byte, bit;

    for (byte = 0; byte < len; ++byte) {
        remainder ^= (message[byte] << 8);
        for (bit = 0; bit < 8; ++bit) {
            if (remainder & 0x8000)
                remainder = (remainder << 1) ^ POLYNOMIAL;
            else
                remainder = (remainder << 1);
        }
    }

    return remainder;
}

// LRC function
unsigned char lrc(char *message, int len) {
    unsigned char result = 0;
    for (int i = 0; i < len; ++i)
        result ^= message[i];
    return result;
}

// VRC function
int vrc(char *message, int len) {
    int count = 0;
    for (int i = 0; i < len; ++i)
        if (message[i] == '1')
            count++;
    return count % 2;
}

// Simple Checksum function
unsigned char checksum(char *message, int len) {
    unsigned char sum = 0;
    for (int i = 0; i < len; ++i)
        sum += message[i];
    return sum;
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len;
    char buffer[1024];

    // Create server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Error in socket creation");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind server socket
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error in binding");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_socket, 5) < 0) {
        perror("Error in listening");
        exit(EXIT_FAILURE);
    }

    printf("Server is ready to receive.\n");

    // Accept client connection
    client_len = sizeof(client_addr);
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
    if (client_socket < 0) {
        perror("Error in accepting connection");
        exit(EXIT_FAILURE);
    }

    // Receive data from client
    ssize_t bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytes_received < 0) {
        perror("Error in receiving data");
        exit(EXIT_FAILURE);
    }

    // Calculate CRC for received message
    unsigned short crc = crc_remainder(buffer, bytes_received);

    // Calculate LRC for received message
    unsigned char lrc_result = lrc(buffer, bytes_received);

    // Calculate VRC for received message
    int vrc_result = vrc(buffer, bytes_received);

    // Calculate checksum for received message
    unsigned char sum = checksum(buffer, bytes_received);

    printf("Received Message: %s\n", buffer);
    printf("CRC: %hu\n", crc);
    printf("LRC: %hhu\n", lrc_result);
    printf("VRC: %d\n", vrc_result);
    printf("Checksum: %hhu\n", sum);

    // Close sockets
    close(client_socket);
    close(server_socket);

    return 0;
}
