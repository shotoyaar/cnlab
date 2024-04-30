#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
void send_request(const char* action, int num) {
 int client_socket = socket(AF_INET, SOCK_STREAM, 0);
 if (client_socket == -1) {
 perror("Socket creation failed");
 exit(EXIT_FAILURE);
 }
struct sockaddr_in server_address;
 server_address.sin_family = AF_INET;
 server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
 server_address.sin_port = htons(12345);
 if (connect(client_socket, (struct sockaddr*)&server_address, 
sizeof(server_address)) == -1) {
 perror("Connection failed");
 exit(EXIT_FAILURE);
 }
 char request[1024];
 snprintf(request, sizeof(request), "%s,%d", action, num);
 if (write(client_socket, request, strlen(request)) == -1) {
 perror("Write failed");
 exit(EXIT_FAILURE);
 }
 char response[1024];
 int n = read(client_socket, response, sizeof(response));
 if (n == -1) {
 perror("Read failed");
 exit(EXIT_FAILURE);
 }
response[n] = '\0';
 printf("Server response: %s\n", response);
 close(client_socket);
}
int main() {
 char action[20];
 int num;
 printf("Enter the action (even, prime, armstrong): ");
 scanf("%s", action);
 printf("Enter the number: ");
 scanf("%d", &num);
 send_request(action, num);
 return 0;
}