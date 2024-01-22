// ----------------------- SERVER PROGRAM ----------------------------- //
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_BUFFER_SIZE 1024

int main()
{
    int server_socket, new_socket;
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;
    char buffer[MAX_BUFFER_SIZE];

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0)
    {
        perror("Error in socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = PORT;
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the specified IP and port
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Error in binding");
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_socket, 10) == 0)
    {
        printf("Listening...\n");
    }
    else
    {
        printf("Error in listening\n");
        exit(1);
    }

    addr_size = sizeof(new_addr);
    new_socket = accept(server_socket, (struct sockaddr *)&new_addr, &addr_size); // Accept a new connection
    if (new_socket < 0)
    {
        perror("Error in accepting");
        exit(1);
    }

    while (1)
    {
        bzero(buffer, 1024);

        // Receive data from the client
        recv(new_socket, buffer, MAX_BUFFER_SIZE, 0);
        if (strcmp(buffer, "exit") == 0)
        {
            printf("Connection closed by client.\n");
            break;
        }
        printf("Client: %s\n", buffer);

        // Send a response to the client
        printf("Server: ");
        fgets(buffer, MAX_BUFFER_SIZE, stdin);
        send(new_socket, buffer, strlen(buffer), 0);

        bzero(buffer, 1024);
    }

    close(new_socket);
    close(server_socket);
    return 0;
}
