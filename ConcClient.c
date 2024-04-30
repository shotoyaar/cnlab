#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<sys/types.h>
#include<netinet/in.h>
void primes(char* sendmsg) {
unsigned n = 0;
printf("Enter the value of n to check: ");
scanf("%d", &n);
sprintf(sendmsg, "%u", n);
}
void parenthesis(char* sendmsg) {
unsigned n = 0;
printf("Enter the number of characters: ");
scanf("%d", &n);
sprintf(sendmsg, "%u", n);
}
void LPS(char* sendmsg) {
printf("Enter string: ");
scanf("%s", sendmsg);
}
void (*funcs[4])(char*) = { NULL, &primes, &parenthesis, &LPS };
int main() {
int csd, cport, len, nsd;
char sendmsg[101] = { 0 }, rcvmsg[101] = { 0 };
struct sockaddr_in servaddr;
printf("(Client)Enter the server port: ");
scanf("%d", &cport);
csd = socket(AF_INET, SOCK_STREAM, 0);
if (csd < 0) puts("can't create");
else puts("Socket is created");
servaddr.sin_family = AF_INET;
servaddr.sin_port = htons(cport);
servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
len = sizeof(servaddr);
nsd = connect(csd, (struct sockaddr*) &servaddr, len);
if (nsd < 0) puts("Cant connect");else puts("Connected successfully");
int choice = 0;
puts("Enter\n1. Check if a number is prime\n2. Number of ways to put parenthesis across n
characters\n3. Length of Longest Palindromic Subsequence");
while (choice > 3 || choice < 1) {
printf("Enter Choice: "); scanf("%d", &choice);
}
sprintf(sendmsg, "%d", choice);
send(csd, sendmsg, 101, 0);
while (1) {
funcs[choice](sendmsg);
if (sendmsg[0] == '\n') continue;
send(csd, sendmsg, 101, 0);
recv(csd, rcvmsg, 101, 0);
printf("%s", rcvmsg);
}
return 0;
}
