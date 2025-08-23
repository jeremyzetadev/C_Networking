#include "socketutil.h"
#include <sys/socket.h>
#include <stdbool.h>
#include <unistd.h>

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
    while(true){
        ssize_t amountReceived = recv(c_fd, buffer, 1024, 0);
        if(amountReceived>0){
            printf("Client amount received is: %zd\n", amountReceived);
            buffer[amountReceived] = 0; // assign 0 at end of length to stop the read
            printf("Client message is: %s\n", buffer);
        }

        if(amountReceived==0)
            break;
    }
    
    close(c_fd);
    shutdown(s_fd, SHUT_RDWR);

    return 0;
}
