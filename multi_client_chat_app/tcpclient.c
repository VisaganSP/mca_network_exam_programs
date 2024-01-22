#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT 4444
#define BUF_SIZE 2000

int main(int argc, char **argv)
{
    int len;
    char client_name[100];
    struct sockaddr_in addr, cl_addr;
    int sockfd, ret;
    char buffer[BUF_SIZE];
    struct hostent *server;
    char *serverAddr;

    if (argc < 2)
    {
        printf("usage: client < ip address >\n");
        exit(1);
    }

    serverAddr = argv[1];

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        printf("Error creating socket!\n");
        exit(1);
    }
    printf("Socket created...\n");

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(serverAddr);
    addr.sin_port = PORT;

    ret = connect(sockfd, (struct sockaddr *)&addr, sizeof(addr));
    if (ret < 0)
    {
        printf("Error connecting to the server!\n");
        exit(1);
    }
    printf("Connected to the server...\n");

    memset(buffer, 0, BUF_SIZE);
    printf("Enter your message(s): ");

    while (fgets(buffer, BUF_SIZE, stdin) != NULL)
    {
        ret = sendto(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr *)&addr, sizeof(addr));
        // ret = send(sockfd, buffer, BUF_SIZE, 0);
        if (ret < 0)
        {
            printf("Error sending data!\n\t-%s", buffer);
        }
        ret = recvfrom(sockfd, buffer, BUF_SIZE, 0, NULL, NULL);
        // ret = recv(sockfd, buffer, BUF_SIZE, 0);
        if (ret < 0)
        {
            printf("Error receiving data!\n");
        }
        else
        {
        //     printf("Received: ");
        //     strcpy(buffer,client_name);
        // strcat(buffer,":");
        // strcat(buffer,buffer);
        // len = write(sockfd,buffer,strlen(buffer));
            fputs(buffer, stdout);
            printf("\n");
        }
    }

    return 0;
}