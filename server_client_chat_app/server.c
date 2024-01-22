#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define PORT 5555
#define MAX_BUFFER_SIZE 1024

int main()
{
    int server_socket, new_socket;
    struct sockaddr_in server_addr, new_addr;
    socklen_t address_size;
    char buffer[MAX_BUFFER_SIZE];

    // Create Socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server_socket < 0)
    {
        perror("[-] Error in socket creation!...\n");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = PORT;
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the Socket to the specified IP and PORT
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("[-] Error while binding!...\n");
        exit(1);
    }

    // Listen for new connections upto 10 maximum
    if (listen(server_socket, 10) == 0)
    {
        printf("[+] Listening...\n");
    }
    else
    {
        printf("[-] Error in listening!...\n");
    }

    // Accepting new connections
    address_size = sizeof(new_addr);
    new_socket = accept(server_socket, (struct sockaddr *)&new_addr, &address_size);
    if (new_socket < 0)
    {
        perror("[-] Error in accepting client!...\n");
        exit(1);
    }

    while (1)
    {

        bzero(buffer, 1024);

        // Recieve msg from the client
        recv(new_socket, buffer, MAX_BUFFER_SIZE, 0);
        if (strcmp(buffer, "exit") == 0)
        {
            printf("Connection closed by client!...\n");
            
            close(new_socket);
            close(server_socket);
            return 0;
        }
        printf("Client: %s\n", buffer);

        bzero(buffer, 1024);

        // Sending msg from the server
        printf("Server: ");
        fgets(buffer, MAX_BUFFER_SIZE, stdin);
        send(new_socket, buffer, strlen(buffer), 0);
    }
    close(new_socket);
    close(server_socket);

    return 0;
}