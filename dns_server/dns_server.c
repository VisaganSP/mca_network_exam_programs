//server
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    char *ip = "127.0.0.1";
    int port = 5566;
    int server_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char buffer1[1024], str[1024], buffer2[1024];
    int n;

    server_sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_sock < 0) {
        perror("Socket error");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY; // Bind to any available interface

    n = bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (n < 0) {
        perror("Bind Error");
        exit(1);
    }

    addr_size = sizeof(client_addr);

    while (1) {
        FILE *fp;
        char *name = "dns.txt";
        fp = fopen(name, "r");
        if (fp == NULL) {
            perror("Error opening file");
            exit(1);
        }

        bzero(str, 1024);
        ssize_t received_bytes = recvfrom(server_sock, str, sizeof(str), 0, (struct sockaddr *)&client_addr, &addr_size);
        if (received_bytes == -1) {
            perror("Error receiving data");
            exit(1);
        }

        int flag = 0;

        while (fscanf(fp, "%s%s", buffer1, buffer2) != EOF) {
            if (strcmp(buffer1, str) == 0 || strcmp(buffer2, str) == 0) {
                ssize_t sent_bytes = sendto(server_sock, buffer1, strlen(buffer1), 0, (struct sockaddr *)&client_addr, addr_size);
                if (sent_bytes == -1) {
                    perror("Error sending data");
                    exit(1);
                }

                usleep(100000); // Introduce a small delay

                sent_bytes = sendto(server_sock, buffer2, strlen(buffer2), 0, (struct sockaddr *)&client_addr, addr_size);
                if (sent_bytes == -1) {
                    perror("Error sending data");
                    exit(1);
                }

                flag = 1;
                break;
            }
        }

        if (flag == 0) {
            bzero(buffer1, 1024);
            strcat(buffer1, "unknown");
            ssize_t sent_bytes = sendto(server_sock, buffer1, strlen(buffer1), 0, (struct sockaddr *)&client_addr, addr_size);
            if (sent_bytes == -1) {
                perror("Error sending data");
                exit(1);
            }

            usleep(100000); // Introduce a small delay

            bzero(buffer2, 1024);
            strcat(buffer2, "unknown");
            sent_bytes = sendto(server_sock, buffer2, strlen(buffer2), 0, (struct sockaddr *)&client_addr, addr_size);
            if (sent_bytes == -1) {
                perror("Error sending data");
                exit(1);
            }
        }

        fclose(fp);
    }

    return 0;
}
