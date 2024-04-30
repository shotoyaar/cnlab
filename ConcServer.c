#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <math.h>
void primes(char *rcvmsg, char *sendmsg)
{
    int n;
    sscanf(rcvmsg, "%d", &n);
    int status = n % 2 != 0;
    int sq = ceil(sqrt(n) + 1);
    for (int i = 3; status && i <= sq; i += 2)
    {
        status = status & (n % i != 0);
    }
    if (status)
        sprintf(sendmsg, "Is Prime\n");
    else
        sprintf(sendmsg, "Is not Prime\n");
}
void parenthesis(char *rcvmsg, char *sendmsg)
{
    int n;
    sscanf(rcvmsg, "%d", &n);
    int DP[n + 1][n + 1];
    memset(DP, 0, (n + 1) * (n + 1) * sizeof(int));
    DP[0][0] = 1;
    for (int i = 0; i <= n; i++)
        DP[i][i] = 1;
    for (int i = n - 1; i >= 0; i--)
    {
        for (int j = i + 1; j <= n; j++)
        {
            for (int k = i; k < j; k++)
            {
                DP[i][j] += DP[i][k] * DP[k + 1][j];
            }
        }
    }
    sprintf(sendmsg, "%d\n", DP[0][n]);
}
int LCS(const char *s1, const char *s2, int n)
{
    int dp[n + 1], ndp[n + 1];
    memset(dp, 0, (n + 1) * sizeof(int));
    memset(ndp, 0, (n + 1) * sizeof(int));
    for (int i = 1; i <= n; i++)
    {
        for (int j = 1; j <= n; j++)
        {
            if (s1[i - 1] == s2[j - 1])
                ndp[j] = 1 + dp[j - 1];
            else
                ndp[j] = dp[j] > ndp[j - 1] ? dp[j] : ndp[j - 1];
        }
        memcpy(dp, ndp, (n + 1) * sizeof(int));
    }
    return dp[n];
}
void LPS(char *rcvmsg, char *sendmsg)
{
    int n = strlen(rcvmsg);
    char rev[n];
    for (int i = 0; i < n; i++)
        rev[i] = rcvmsg[n - i - 1];
    sprintf(sendmsg, "%d\n", LCS(rcvmsg, rev, n));
}
void (*funcs[3])(char *, char *) = {&primes, &parenthesis, &LPS};
int main()
{
    int sd, socketBind, nsd, cliLen, servPort, len, port;
    struct sockaddr_in servAddr, cliAddr;
    printf("Enter the server port:");
    scanf("%d", &servPort);
    sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0)
        puts("Creation failed");
    else
        puts("Socket Created");
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(servPort);
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    socketBind = bind(sd, (struct sockaddr *)&servAddr, sizeof(servAddr));
    if (socketBind < 0)
        puts("Can't bind");
    else
        puts("Binded");
    listen(sd, 5);
    cliLen = sizeof(cliAddr);
    while (1)
    {
        nsd = accept(sd, (struct sockaddr *)&cliAddr, &cliLen);
        if (nsd < 0)
            puts("Can't accept");
        else
        {
            pid_t pid = fork();
            if (pid == 0)
            {
                char sendmsg[101], rcvmsg[101];
                int choice;
                recv(nsd, rcvmsg, 101, 0);
                sscanf(rcvmsg, "%d", &choice);
                void (*func)(char *, char *);
                if (choice == 1)
                    func = &primes;
                else if (choice == 2)
                    func = &parenthesis;
                else
                    func = &LPS;
                while (1)
                {
                    recv(nsd, rcvmsg, 101, 0);
                    printf("%s\n", rcvmsg);
                    func(rcvmsg, sendmsg);
                    send(nsd, sendmsg, 101, 0);
                }
            }
        }
    }
}
