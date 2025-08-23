#include "socketutil.h"
#include <sys/socket.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>

struct AcceptedSocket {
    int acceptSocketFd;
    struct sockaddr_in address;
    int error;
    bool acceptedSuccessfully;
};

struct AcceptedSocket *acceptIncomingConnection(int s_fd);
void startAcceptingIncomingConnections(int s_fd);
void acceptNewConnectionAndSendRecv(int s_fd);
void receiveAndPrintIncomingDataOnSeparateThread(struct AcceptedSocket *clientSocket);
void *receiveAndPrintIncomingData(void *s_fd);
void sendReceivedMessageToTheOtherClients(char *buffer, int s_fd);

int maxSocketCount  = 10;
int acceptedSocketsCount = 0;
struct AcceptedSocket acceptedSockets[10];

int main(){
    // server -- file descriptor
    int s_fd = createTCPIPv4Socket();
    struct sockaddr_in *s_addr = createIPv4Address("", 8000); //self IP
    int result = bind(s_fd, (struct sockaddr *)s_addr, sizeof(*s_addr));                                                                
    if(result==0){
        printf("socket was bind successfully\n");
    }

    // waiting for client to connect : 10 variable is => up to 10 connections
    result = listen(s_fd, maxSocketCount);

    // infinitely accepting connections
    startAcceptingIncomingConnections(s_fd);

    shutdown(s_fd, SHUT_RDWR);

    return 0;
}

void startAcceptingIncomingConnections(int s_fd){
    while(true){
        struct AcceptedSocket *clientSocket = acceptIncomingConnection(s_fd);
        acceptedSockets[acceptedSocketsCount++] = *clientSocket;
        receiveAndPrintIncomingDataOnSeparateThread(clientSocket);
    }
}

struct AcceptedSocket *acceptIncomingConnection(int s_fd){
    struct sockaddr_in c_addr;
    socklen_t c_addr_size = sizeof(struct sockaddr_in);
    int c_fd = accept(s_fd, (struct sockaddr *)&c_addr, &c_addr_size);

    struct AcceptedSocket *acceptedSocket = malloc(sizeof (struct AcceptedSocket));
    acceptedSocket->address = c_addr;
    acceptedSocket->acceptSocketFd = c_fd;
    acceptedSocket->acceptedSuccessfully = (c_fd >0);

    if(!acceptedSocket->acceptedSuccessfully)
        acceptedSocket->error = c_fd;  

    return acceptedSocket;
}

void receiveAndPrintIncomingDataOnSeparateThread(struct AcceptedSocket *clientSocket){
        pthread_t pth_id;
        pthread_create(&pth_id, NULL, receiveAndPrintIncomingData, (void*) &clientSocket->acceptSocketFd);
}

void *receiveAndPrintIncomingData(void *c_fd){
    char buffer[1024];
    int *c_fd_casted = (int *)(c_fd);
    while(true){
        ssize_t amountReceived = recv(*c_fd_casted, buffer, 1024, 0);
        if(amountReceived>0){
            buffer[amountReceived] = 0; // assign 0 at end of length to stop the read
            printf("Client%i message is: %s\n", *c_fd_casted, buffer);
            sendReceivedMessageToTheOtherClients(buffer,*c_fd_casted);
        }

        if(amountReceived==0)
            break;
    }
    close(*c_fd_casted);
    return NULL;
}

void sendReceivedMessageToTheOtherClients(char *buffer, int c_fd){
    for(int i=0; i<acceptedSocketsCount ; i++){
        // send all chats to all clients
        send(acceptedSockets[i].acceptSocketFd, buffer, strlen(buffer), 0);

        // this is only when you want to see other chat
        // if(acceptedSockets[i].acceptSocketFd != c_fd){
        //     send(acceptedSockets[i].acceptSocketFd, buffer, strlen(buffer), 0);
        // }
    }
}
