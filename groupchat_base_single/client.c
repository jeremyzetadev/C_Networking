#include "socketutil.h"
#include <netdb.h>
#include <unistd.h>

int main(){
    int s_fd = createTCPIPv4Socket();
    struct sockaddr_in *s_addr = createIPv4Address("127.0.0.1", 8000);
    int result = connect(s_fd, (struct sockaddr *)s_addr, sizeof(*s_addr));
    if(result==0)
        printf("connection was successful\n");
    else{
        printf("connection has failed %d \n", result);
        perror("connect error");
    }

    char *message;
    message = "Hello world";
    send(s_fd, message, strlen(message), 0);

    char buffer[1024];
    recv(s_fd, buffer, 1024, 0);

    printf("Server respond is: %s\n", buffer);

    return 0;
}

