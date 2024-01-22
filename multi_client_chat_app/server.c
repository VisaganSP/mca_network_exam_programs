// ----------------------- MULTI CLIENT CHATTING SERVER PROGRAM ----------------------------- //

#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

pthread_mutex_t mutex;
int clients[20];
int n=0;

void sendtoall(char *msg,int curr){
    int i;
    pthread_mutex_lock(&mutex);
    for(i = 0; i < n; i++) {
        if(clients[i] != curr) {
            if(send(clients[i],msg,strlen(msg),0) < 0) {
                printf("sending failure \n");
                continue;
            }
        }
    }
    pthread_mutex_unlock(&mutex);
}

void *recvmg(void *client_sock){
    int sock = *((int *)client_sock);
    char msg[500];
    int len;
    while((len = recv(sock,msg,500,0)) > 0) {
        msg[len] = '\0';
        sendtoall(msg,sock);
    }

}

int main(){
    struct sockaddr_in ServerIp;
    pthread_t recvt;
    int sock=0 , Client_sock=0;

    ServerIp.sin_family = AF_INET;
    ServerIp.sin_port = htons(1234);
    // ServerIp.sin_addr.s_addr = inet_addr("127.0.0.1");
    ServerIp.sin_addr.s_addr = INADDR_ANY;
    sock = socket( AF_INET , SOCK_STREAM, 0 );
    if( bind( sock, (struct sockaddr *)&ServerIp, sizeof(ServerIp)) == -1 )
        printf("cannot bind, error!! \n");
    else
        printf("Server Started\n");

    if( listen( sock ,20 ) == -1 )
        printf("listening failed \n");

    while(1){
        if( (Client_sock = accept(sock, (struct sockaddr *)NULL,NULL)) < 0 )
            printf("accept failed  \n");
        pthread_mutex_lock(&mutex);
        clients[n]= Client_sock;
        n++;
        // creating a thread for each client 
        pthread_create(&recvt,NULL,(void *)recvmg,&Client_sock);
        pthread_mutex_unlock(&mutex);
    }
    return 0; 

}