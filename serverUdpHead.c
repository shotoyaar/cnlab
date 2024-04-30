#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#define MAX_BUFFER_SIZE 1024
#define SERVER_PORT 8888
int main() {
int sockfd;
struct sockaddr_in server_addr, client_addr;
char buffer[MAX_BUFFER_SIZE];
char head_numbers[MAX_BUFFER_SIZE];
int client_addr_len, recv_len, i;
// Create UDP socket
sockfd = socket(AF_INET, SOCK_DGRAM, 0);
if (sockfd < 0) {
perror("Socket creation failed");
exit(EXIT_FAILURE);
}
// Bind socket to address
server_addr.sin_family = AF_INET;
server_addr.sin_addr.s_addr = INADDR_ANY;
server_addr.sin_port = htons(SERVER_PORT);
if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
perror("Socket bind failed");
exit(EXIT_FAILURE);
}
printf("Server listening on port %d\n", SERVER_PORT);
while (1) {
// Receive list of numbers from client
client_addr_len = sizeof(client_addr);
recv_len = recvfrom(sockfd, buffer, MAX_BUFFER_SIZE, 0, (struct
sockaddr*)&client_addr, &client_addr_len);
if (recv_len < 0) {
perror("Receive failed");
exit(EXIT_FAILURE);
}
// Process the list of numbers
int n = atoi(buffer); // Get the number of elements in the list
head_numbers[0] = '\0'; // Initialize head_numbers as an empty string
// Split the received buffer into individual numbers
char* token = strtok(buffer, " ");
int* numbers = malloc(n * sizeof(int));
i = 0;
while (token != NULL) {
numbers[i++] = atoi(token);
token = strtok(NULL, " ");
}
for (i = 1; i < n; i++) {
if (numbers[i-1] < numbers[i] && numbers[i+1] < numbers[i]) {
char num_str[4];
sprintf(num_str, "%d ", numbers[i]);
strcat(head_numbers, num_str);
}
}
free(numbers);
// Print the head numbers in the server terminal
printf("Head Numbers: %s\n", head_numbers);
// Send the head numbers back to the client
if (sendto(sockfd, head_numbers, strlen(head_numbers), 0, (struct
sockaddr*)&client_addr, client_addr_len) < 0) {
perror("Send failed");
exit(EXIT_FAILURE);
}
}
// Close the socket
close(sockfd);
return 0;
}
