#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char **argv)
{
    int n1 = atoi(argv[1]), n2 = atoi(argv[2]), res = 0;

    char operator = *argv[3];
    int sock_id = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_id < 0)
    {
        printf("Error in getting socket\n");
        return 0;
    }

    struct sockaddr_in clientstruct;
    clientstruct.sin_family = AF_INET;
    clientstruct.sin_addr.s_addr = INADDR_ANY;
    clientstruct.sin_port = 6666;

    int ret = connect(sock_id, (struct sockaddr *)&clientstruct, sizeof(clientstruct));

    write(sock_id, &n1, sizeof(n1));
    write(sock_id, &n2, sizeof(n2));
    write(sock_id, &operator, sizeof(operator));

    int ret1 = read(sock_id, &res, sizeof(res));
    printf("FROM SERVER Calculated Value: %d \nBytes = %d\n", res, ret1);
    close(sock_id);

    return 0;
}
