#include "socketutil.h"
#include <netdb.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

#include <stdio.h>

void startListeningAndPrintMessagesOnNewThread(int fd);
void *listenAndPrint(void* socketFD);
void readConsoleEntriesAndSendToServer(int socketFD);

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

    // thisclient send data to server by threading
    startListeningAndPrintMessagesOnNewThread(s_fd);
    // waiting msg of server responds by threading
    readConsoleEntriesAndSendToServer(s_fd);
    
    close(s_fd);

    return 0;
}

void startListeningAndPrintMessagesOnNewThread(int socketFD) {
    pthread_t id ;
    pthread_create(&id,NULL,listenAndPrint,(void *)&socketFD);
}

void readConsoleEntriesAndSendToServer(int socketFD) {
    char *name = NULL;
    size_t nameSize= 0;
    printf("please enter your name?\n");
    ssize_t  nameCount = getline(&name,&nameSize,stdin);
    name[nameCount-1]=0;

    char *line = NULL;
    size_t lineSize= 0;
    printf("type and we will send(type exit)...\n");

    char buffer[1024];
    while(true)
    {
        ssize_t charCount = getline(&line,&lineSize,stdin);
        line[charCount-1]=0;
        sprintf(buffer,"%s:%s",name,line);
        if(charCount>0)
        {
            if(strcmp(line,"exit")==0)
                break;

            ssize_t amountWasSent =  send(socketFD, buffer, strlen(buffer), 0);
        }
    }
}

void *listenAndPrint(void *socketFD) {
    int *socketFD_casted = (int *)socketFD;
    char buffer[1024];
    while (true)
    {
        ssize_t amountReceived = recv(*socketFD_casted,buffer,1024,0);
        if(amountReceived>0)
        {
            buffer[amountReceived] = 0;
            printf("Response was %s\n ",buffer);
        }
        if(amountReceived==0)
            break;
    }
    close(*socketFD_casted);
    return NULL;
}



