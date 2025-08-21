#include "socketutil.h"
#include <sys/socket.h>

int main(){
    // server -- file descriptor
    int s_fd = createTCPIPv4Socket();
    struct sockaddr_in *s_addr = createIPv4Address("", 8000); //self IP
    int result = bind(s_fd, (struct sockaddr *)s_addr, sizeof(*s_addr));                                                                
    if(result==0){
        printf("socket was bind successfully\n");
    }

    // waiting for client to connect : 10 variable is => up to 10 connections
    result = listen(s_fd, 10);

    struct sockaddr_in c_addr;
    socklen_t c_addr_size = sizeof(struct sockaddr_in);
    // client -- file descriptor
    int c_fd = accept(s_fd, (struct sockaddr *) &c_addr, &c_addr_size);

    char buffer[1024];
    result = recv(c_fd, buffer, 1024, 0);
    printf("Client message is: %s\n", buffer);

    return 0;
}
