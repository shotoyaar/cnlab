#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
bool is_even(int num) {
 return num % 2 == 0;
}
bool is_prime(int num) {
 if (num < 2) {
 return false;
 }
 for (int i = 2; i * i <= num; i++) {
 if (num % i == 0) {
 return false;
 }
 }
 return true;
}
bool is_armstrong(int num) {
 int temp = num;
 int sum = 0;
 while (temp > 0) {
int digit = temp % 10;
 sum += digit * digit * digit;
 temp /= 10;
 }
 return num == sum;
}
void handle_client(int client_socket, int client_number) {
 char buffer[1024];
 int n;
 while (true) {
 n = read(client_socket, buffer, sizeof(buffer));
 if (n <= 0) {
 break;
 }
 buffer[n] = '\0';
 char* action = strtok(buffer, ",");
 int num = atoi(strtok(NULL, ","));
 bool result;
 if (strcmp(action, "even") == 0) {
 result = is_even(num);
 } else if (strcmp(action, "prime") == 0) {
 result = is_prime(num);
 } else if (strcmp(action, "armstrong") == 0) {
 result = is_armstrong(num);
 } else {
 result = false;
 }
 char response[1024];
 snprintf(response, sizeof(response), "Client %d: %s\n", client_number, 
(result ? "true" : "false"));
write(client_socket, response, strlen(response));
 }
 close(client_socket);
}
int main() {
 int server_socket, client_socket;
 struct sockaddr_in server_address, client_address;
 socklen_t client_address_length;
 server_socket = socket(AF_INET, SOCK_STREAM, 0);
 if (server_socket == -1) {
 perror("Socket creation failed");
 exit(EXIT_FAILURE);
 }
 server_address.sin_family = AF_INET;
 server_address.sin_addr.s_addr = INADDR_ANY;
 server_address.sin_port = htons(12345);
 if (bind(server_socket, (struct sockaddr*)&server_address, 
sizeof(server_address)) == -1) {
 perror("Binding failed");
 exit(EXIT_FAILURE);
 }
 if (listen(server_socket, 5) == -1) {
 perror("Listening failed");
 exit(EXIT_FAILURE);
 }
 printf("Server started. Listening for connections...\n");
 int client_number = 1; // To keep track of connected clients
while (true) {
 client_address_length = sizeof(client_address);
 client_socket = accept(server_socket, (struct 
sockaddr*)&client_address, &client_address_length);
 if (client_socket == -1) {
 perror("Accepting connection failed");
 exit(EXIT_FAILURE);
 }
 printf("Client %d connected\n", client_number);
 if (fork() == 0) {
 handle_client(client_socket, client_number);
 exit(EXIT_SUCCESS);
 }
 client_number++;
 }
 close(server_socket);
 return 0;
}
