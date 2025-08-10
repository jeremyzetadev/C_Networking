#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while(0)

int main(){
    int result;
    struct sockaddr_in server_addr, client_addr;

    printf("Initializing socket, server log\n");
    // server -- file descriptor
    int sfd = socket(AF_INET, SOCK_STREAM,0);
    if(sfd==-1){
        handle_error("socket");
    }

    // 1.) Create Server by binding
    printf("Server start, server log\n");
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8000);
    inet_pton(AF_INET, "0.0.0.0", &server_addr.sin_addr);
    result = bind(sfd, (struct sockaddr *) &server_addr, sizeof(server_addr)); 
    if(result == -1){
        handle_error("bind");
    }

    // 2.) Server is listening
    printf("Server listening, server log\n");
    result = listen(sfd,10);
    if(result == -1){
        handle_error("listen");
    }

    // 3.) Accepting client
    socklen_t client_addr_size;
    client_addr_size = sizeof(client_addr);
    // client -- file descriptor
    int cfd = accept(sfd, (struct sockaddr *) &client_addr, &client_addr_size);
    if(result == -1){
        handle_error("accept");
    }

    //Send message from server to client
    result = write(cfd, "server message to client - hello\n", 33);
    if(result == -1){
        handle_error("write");
    }

    //Read message from client sent to server - ONE MESSAGE
    char buffer[1024] = {0};
    result = read(cfd, buffer, 1024);
    printf("from client: %s\n", buffer);

    // 4.) closing client
    result = close(cfd);
    if(result == -1){
        handle_error("close");
    }

    // 5.) closing server
    result = close(sfd);
    if(result == -1){
        handle_error("close");
    }

    printf("Server close, server log\n");
    return 1;
}
