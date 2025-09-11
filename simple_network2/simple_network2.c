#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

static void server_mypublicIP(){
    int server_fd;
    struct sockaddr_in s_address;
    int port = 8080; // The port you want to bind to
    const char *public_ip = "YOUR_PUBLIC_IP_ADDRESS"; // Replace with your actual public IP

    // 1. Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 2. Prepare the sockaddr_in structure
    s_address.sin_family = AF_INET;
    s_address.sin_addr.s_addr = inet_addr(public_ip); // Convert IP string to network address
    s_address.sin_port = htons(port); // Convert port to network byte order
    
    // 3. Bind the socket to the specified IP and port
    if (bind(server_fd, (struct sockaddr *)&s_address, sizeof(s_address)) < 0) {
        perror("bind failed");
        close(server_fd); // Close the socket on error
        exit(EXIT_FAILURE);
    }

    printf("Socket successfully bound to %s:%d\n", public_ip, port);

    // Further server logic (listen, accept, etc.) setup here
    // 
    // 
    // 
    // Further server logic (listen, accept, etc.) setup here

    close(server_fd); // Close the socket when done
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
    char buffer[1024];
    recv(cfd, buffer, sizeof(buffer), 0);

    // print without looking
    printf("client responds:\n     %s\n", buffer);

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
    const char *msg = "the client says Hello";
    send(sfd, msg, strlen(msg) +1, 0);

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

