#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define MAXLINE 1024

int main() {
  int sockfd, PORT;
  char buffer[MAXLINE];
  struct sockaddr_in servaddr, cliaddr;

  printf("Enter server port: \n");
  scanf("%d", &PORT);

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
    printf("Socket creation failed. \n");
    exit(EXIT_FAILURE);
  }

  else
    printf("Socket creation successful.\n");

  memset(&servaddr, 0, sizeof(servaddr));
  memset(&cliaddr, 0, sizeof(cliaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(PORT);
  servaddr.sin_addr.s_addr = INADDR_ANY;

  if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    printf("Binding failed.\n");
  else
    printf("Binding successful.\n");

  int len, n;

  while (1) {
    len = sizeof(cliaddr);
    n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL,
                 (struct sockaddr *)&cliaddr, &len);
    buffer[n] = '\0';
    printf("Message from client: %s\n", buffer);

    printf("Enter message to send to client: ");
    fgets(buffer, MAXLINE, stdin);
    sendto(sockfd, (const char *)buffer, strlen(buffer) + 1, MSG_CONFIRM,
           (const struct sockaddr *)&cliaddr, len);

    if (strncmp(buffer, "exit", 4) == 0)
      break;
  }

  close(sockfd);
  return 0;
}
