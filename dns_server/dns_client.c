//client
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    char *ip = "127.0.0.1";
    int port = 5566;

    int sock;
    struct sockaddr_in addr;
    socklen_t addr_size;

    char buffer[1024];
    char str1[1024], str2[1024];
    int n;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("Socket error");
        exit(1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(ip);

    addr_size = sizeof(addr);

    while (1) {
        bzero(buffer, 1024);
        printf("Enter the ip address/domain name : ");
        scanf("%s", buffer);

        ssize_t sent_bytes = sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *)&addr, addr_size);
        if (sent_bytes == -1) {
            perror("Error sending data");
            exit(1);
        }

        bzero(str1, 1024);
        ssize_t received_bytes = recvfrom(sock, str1, sizeof(str1), 0, (struct sockaddr *)&addr, &addr_size);
        if (received_bytes == -1) {
            perror("Error receiving data");
            exit(1);
        }
        printf("SERVER : %s\n", str1);

        bzero(str2, 1024);
        received_bytes = recvfrom(sock, str2, sizeof(str2), 0, (struct sockaddr *)&addr, &addr_size);
        if (received_bytes == -1) {
            perror("Error receiving data");
            exit(1);
        }
        printf("ADDRESS : %s\n", str2);
    }

    return 0;
}
