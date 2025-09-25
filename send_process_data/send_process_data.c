#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

typedef struct{
    int id;
    char name[20];
    float value;
} Packet;

static void sendpacketsameple(){
    const int s_fd = socket(PF_INET, SOCK_STREAM, 0);

    // server send
    Packet packet_send = {1, "John", 12.34f};
    write(s_fd, &packet_send, sizeof(packet_send));

    // client send
    Packet packet_receive;
    read(s_fd, &packet_receive, sizeof(packet_receive));
}

static void server(){
    // create socket (file descriptor)
    const int sfd = socket(PF_INET, SOCK_STREAM, 0);

    // bind to open port
    struct sockaddr_in s_addr;
    s_addr.sin_family = AF_INET;
    s_addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sfd, (struct sockaddr*) &s_addr, sizeof(s_addr))){
        perror("bind error:");  
        return;
    }

    // read port
    socklen_t s_addr_len = sizeof(s_addr);
    getsockname(sfd, (struct sockaddr*) &s_addr, &s_addr_len);
    printf("server is on port %d\n", (int) ntohs(s_addr.sin_port));

    if (listen(sfd,1)){
        perror("listen error");
        return;
    }

    // accept incoming connection
    struct sockaddr_in c_addr;
    socklen_t c_addr_len = sizeof(c_addr);
    const int cfd  = accept(sfd, (struct sockaddr*) &c_addr,  &c_addr_len);

    // read from client with recv
    // char buffer[1024];
    // recv(cfd, buffer, sizeof(buffer), 0);

    // ======================= PACKET RECEIVE STRUCT ============================ //
    Packet packet_receive;
    read(cfd, &packet_receive, sizeof(packet_receive));
    // ======================= PACKET RECEIVE STRUCT ============================ //

    // print without looking
    printf("client responds:\n     %s\n", packet_receive.name);

    close(sfd);
    close(cfd);
}

static void client(int port){

    // create socket to connect to (file descriptor)
    // sfd server socket
    const int sfd = socket(PF_INET, SOCK_STREAM, 0);

    struct sockaddr_in s_addr = { 0 };
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons((short) port);

    // connect to local machine at specified port
    char addrstr[NI_MAXHOST + NI_MAXSERV + 1];
    snprintf(addrstr, sizeof(addrstr), "127.0.0.1:%d",port);

    // parse into address
    inet_pton(AF_INET, addrstr, &s_addr.sin_addr);

    // connect to server
    if (connect(sfd, (struct sockaddr*) &s_addr, sizeof(s_addr))){
        perror("connect error");
        return;
    }

    // say hey with send!
    // const char *msg = "the client says Hello";
    // send(sfd, msg, strlen(msg) +1, 0);

    // ======================= PACKET SEND STRUCT ============================ //
    Packet packet_send = {1, "John", 12.34f};
    write(sfd, &packet_send, sizeof(packet_send));
    // ======================= PACKET SEND STRUCT ============================ //


    close(sfd);
}

int main(int argc, char *argv[]){
    if (argc > 1 && !strcmp(argv[1], "client")){
        if (argc !=3){
            fprintf(stderr, "not enought args!");
            return -1;
        }

        int port;
        sscanf(argv[2], "%d", &port);

        client(port);
    } else {
        server();
    }

    return 0;
}

