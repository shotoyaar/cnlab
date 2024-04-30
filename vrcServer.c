#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

int main() {

  int sd, sd2, nsd, clilen, sport, len;
  int port;

  char sendmsg[100], recvmsg[100];
  struct sockaddr_in servaddr, cliaddr;

  printf("Enter server port: \n");
  scanf("%d", &sport);

  sd = socket(AF_INET, SOCK_STREAM, 0);

  if (sd < 0)
    printf("Can't create socket.\n");
  else
    printf("Socket is created.\n");

  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(sport);
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

  sd2 = bind(sd, (struct sockaddr *)&servaddr, sizeof(servaddr));

  if (sd2 < 0)
    printf("Can't bind.\n");
  else
    printf("Binding successful.\n");

  listen(sd, 5);

  clilen = sizeof(cliaddr);
  nsd = accept(sd, (struct sockaddr *)&cliaddr, &clilen);

  if (nsd < 0)
    printf("Can't accept.\n");
  else
    printf("Socket accepted.\n");

  while (true) {
    recv(nsd, &recvmsg, 200, 0);
    printf("Corrected msg is: %s", recvmsg);
    printf("\n");
  }

  return 0;
}
