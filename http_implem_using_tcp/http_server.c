#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define WWWROOT "./wwwroot"

void handle_get_request(int client_socket, const char* path) {
    char file_path[256];
    snprintf(file_path, sizeof(file_path), "%s%s", WWWROOT, path);

    // Open the requested file
    int file_fd = open(file_path, O_RDONLY);
    if (file_fd == -1) {
        // If the file is not found, send a 404 response
        const char* response_404 = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n404 Not Found";
        send(client_socket, response_404, strlen(response_404), 0);
        return;
    }

    // Send the file as the response
    const char* response_header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    send(client_socket, response_header, strlen(response_header), 0);

    char buffer[BUFFER_SIZE];
    int bytes_read;
    while ((bytes_read = read(file_fd, buffer, BUFFER_SIZE)) > 0) {
        send(client_socket, buffer, bytes_read, 0);
    }

    close(file_fd);
}

void handle_post_request(int client_socket, const char* data) {
    printf("Received POST data: %s\n", data);

    // Process the data and print it to the server terminal
    printf("Processed data: %s\n", data);

    // Send a response to the client
    const char* response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello, this is a POST response!";
    send(client_socket, response, strlen(response), 0);
}


void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);

    recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
    printf("Received request:\n%s\n", buffer);

    // Extract the request method and path
    char method[10], path[256];
    sscanf(buffer, "%s %s", method, path);

    // Check if it's a GET or POST request
    if (strcmp(method, "GET") == 0) {
        handle_get_request(client_socket, path);
    } else if (strcmp(method, "POST") == 0) {
        // Find the start of the POST data
        printf("POST triggered\n");
        char* data_start = strstr(buffer, "\r\n\r\n");
        if (data_start != NULL) {
            data_start += 4; // Move past the '\r\n\r\n'
            handle_post_request(client_socket, data_start);
        }
    }

    close(client_socket);
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    socklen_t client_address_len = sizeof(client_address);

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Set up server address struct
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    // Bind socket
    if (bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("Error binding socket");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 10) == -1) {
        perror("Error listening for connections");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        // Accept a connection
        client_socket = accept(server_socket, (struct sockaddr*)&client_address, &client_address_len);
        if (client_socket == -1) {
            perror("Error accepting connection");
            continue;
        }

        // Handle the client request
        handle_client(client_socket);
    }

    // Close the server socket
    close(server_socket);

    return 0;
}
