#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

bool checkParity(int num) {
  int n = num;
  int cnt = 0;
  while (n > 0) {
    int lastDigit = n % 10;
    if (lastDigit % 2 != 0)
      cnt++;
    n /= 10;
  }
  return (cnt % 2 == 0);
}

void checkVRC(int arr, int csd) {
  char strArr[4][7];
  int k = 0;

  if (!checkParity(arr)) {
    sprintf(strArr[k], "%d1", arr);
    send(csd, strArr[k], 6, 0);
    k++;
  }

  else {
    sprintf(strArr[k], "%d0", arr);
    send(csd, strArr[k], 6, 0);
    k++;
  }

  for (int i = 0; i < k; i++) {
    printf("The corrected message is: %s\n", strArr[i]);
  }
}

int main() {
  int csd, cport, len, nsd, clilen;
  char recvmsg[100], sendmsg[100];

  struct sockaddr_in servaddr, cliaddr;

  printf("Enter the server port: \n");
  scanf("%d", &cport);

  csd = socket(AF_INET, SOCK_STREAM, 0);

  if (csd < 0)
    printf("Socket not created.\n");
  else
    printf("Socket created.\n");

  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(cport);
  servaddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

  if (connect(csd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    printf("Can't connect.\n");
  else
    printf("Connection successful.\n");

  while (true) {
    int cnt = 0;

    printf("Enter a message of binary numbers to send to the server: ");
    int arr[4];

    for (int i = 0; i < 4; i++) {
      scanf("%d", &arr[i]);
    }

    for (int i = 0; i < 4; i++) {
      checkVRC(arr[i], csd);
    }
  }
}
