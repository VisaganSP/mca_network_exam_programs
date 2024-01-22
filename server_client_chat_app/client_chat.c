#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[MAX_BUFFER_SIZE];

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Error in socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = PORT;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Change this to the server's IP address

    // Connect to the server
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error in connection");
        exit(1);
    }

    while (1) {
	bzero(buffer, 1024);

        // Send data to the server
        printf("Client: ");
        fgets(buffer, MAX_BUFFER_SIZE, stdin);
        send(client_socket, buffer, strlen(buffer), 0);

	bzero(buffer, 1024);

        // Receive data from the server
        recv(client_socket, buffer, MAX_BUFFER_SIZE, 0);
        printf("Server: %s\n", buffer);

        if (strcmp(buffer, "exit") == 0) {
            printf("Connection closed by server.\n");
            break;
        }
    }

    close(client_socket);
    return 0;
}
