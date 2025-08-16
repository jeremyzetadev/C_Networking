#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define handle_error(msg) \
    do {perror(msg); exit(EXIT_FAILURE);} while(0)

int main(){
    int result;
    struct sockaddr_in s_addr, c_addr;

    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(8000);
    inet_pton(AF_INET, "0.0.0.0", &s_addr.sin_addr);

    result = bind(sfd, (struct sockaddr *) &s_addr, sizeof(s_addr));

    result = listen(sfd, 10);

    socklen_t c_addr_size;
    c_addr_size = sizeof(c_addr);
    int cfd = accept(sfd, (struct sockaddr *) &c_addr, &c_addr_size);

    result = write(cfd, "server message to client - hello\n", 33);

    char buffer[1024] = {0};
    result = read(cfd, buffer, 1024);
    printf("from client: %s\n",buffer);

    return 0;
}
