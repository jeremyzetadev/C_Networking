#include "socketutil.h"
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>

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

    char *line;
    size_t lineSize = 0;
    printf("type and we will send(type exit)...\n");

    while(true){
        ssize_t charCount = getline(&line, &lineSize, stdin);
        if(charCount>0){
            if(strcmp(line,"exit\n")==0)
                break;
            else{
                ssize_t amountWasSent = send(s_fd, line, charCount, 0);
            }
        }
    }

    return 0;
}

