#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

int main(){
    int s_fd = socket(AF_INET, SOCK_STREAM, 0);

    //Enter server ip
    char *s_ip = "0.0.0.0";
    struct sockaddr_in s_addr;
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(80);
    inet_pton(AF_INET, s_ip, &s_addr.sin_addr.s_addr);

    int result = connect(s_fd, (struct sockaddr *)&s_addr, sizeof(s_addr));
    if(result==0)
        printf("connection was successful\n");

    char *message;
    message = "Hello world";
    send(s_fd, message, strlen(message), 0);

    char buffer[1024];
    recv(s_fd, buffer, 1024, 0);

    printf("Server respond is: %s\n", buffer);

    return 0;
}
