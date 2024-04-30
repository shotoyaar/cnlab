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

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    printf("Socket not created.\n");
  else
    printf("Socket created.\n");

  memset(&servaddr, 0, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(PORT);
  servaddr.sin_addr.s_addr = INADDR_ANY;

  int n, len;

  while (1) {
    printf("Send to server: ");
    fgets(buffer, MAXLINE, stdin);
    sendto(sockfd, (const char *)buffer, strlen(buffer), MSG_CONFIRM,
           (const struct sockaddr *)&servaddr, sizeof(servaddr));

    if (strncmp(buffer, "exit", 4) == 0)
      break;

    len = sizeof(servaddr);

    n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL,
                 (struct sockaddr *)&servaddr, &len);
    buffer[n] = '\0';
    printf("Recieved from server: %s\n", buffer);
  }

  close(sockfd);
  return 0;
}
