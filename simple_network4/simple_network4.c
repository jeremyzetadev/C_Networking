#include <netinet/in.h>
#include <stdio.h>
#include <string.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <time.h>
#include <inttypes.h>

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

    // loop reading items from client
    clock_t start, diff;
    int32_t time_nanosec = 0;
    int iRunning = 1;
    while(iRunning){
    start = clock();
        // read from client with recv
        char buffer[1024];
        recv(cfd, buffer, sizeof(buffer), 0);
        char *response =  "The server says Hi";
        write(cfd, response, strlen(response));
    diff = clock() - start;
    time_nanosec = diff * 1000000 / CLOCKS_PER_SEC;
        // print without looking
    printf("client responds text [1024] bytes:  %s", buffer);
    printf("| client responds nanoseconds:  %" PRId32 "\n", time_nanosec);
    }

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

    clock_t start, diff;
    int32_t time_nanosec = 0;
    int iRunning = 1;
    while(iRunning){
    start = clock();
        // say hey with send!
        const char *msg = "The client says Hello";
        send(sfd, msg, strlen(msg) +1, 0);
        char buffer[1024];
        recv(sfd, buffer, 1024, 0);
    diff = clock() - start;
    time_nanosec = diff * 1000000 / CLOCKS_PER_SEC;
        // print without looking
    printf("server responds text [1024] bytes:  %s", buffer);
    printf("| server responds nanoseconds:  %" PRId32 "\n", time_nanosec);
    }
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

