#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

int main() {
  char sendmsg[100], recvmsg[100];
  struct sockaddr_in servaddr, cliaddr;
  int sockid, sockid2, nsd, clilen, sport, len;

  printf("Enter the server port: \n");
  scanf("%d", &sport);

  sockid = socket(AF_INET, SOCK_STREAM, 0);

  if (sockid < 0)
    printf("Socket creation failed\n");
  else
    printf("Socket created successfully\n");

  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(sport);
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

  sockid2 = bind(sockid, (struct sockaddr *)&servaddr, sizeof(servaddr));

  if (sockid2 < 0)
    printf("Binding failed\n");
  else
    printf("Binding successful\n");

  listen(sockid, 5);

  clilen = sizeof(cliaddr);
  nsd = accept(sockid, (struct sockaddr *)&cliaddr, &clilen);

  if (nsd < 0)
    printf("Accept failed\n");
  else
    printf("Accept successful\n");

  while (1) {
    recv(nsd, recvmsg, 100, 0);
    printf("Message recieved from client: %s\n", recvmsg);

    printf("Enter the message to send to client: \n");
    fgets(sendmsg, 100, stdin);
    send(nsd, sendmsg, strlen(sendmsg) + 1, 0);
  }

  return 0;
}

