#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

// String Slice can use ds.h
#define DS_SS_IMPLEMENTATION
// String Builder can use ds.h
#define DS_SB_IMPLEMENTATION
// Input Output can use ds.h
#define DS_IO_IMPLEMENTATION
#include "ds.h"


#define MAX_LEN 1024

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while(0)

int main(){
    int result, sfd, cfd;
    struct sockaddr_in server_addr, client_addr;

    // printf("Initializing socket, server log\n");
    // server -- file descriptor
    sfd = socket(AF_INET, SOCK_STREAM,0);
    if(sfd==-1){
        handle_error("socket");
    }

    // 1.) Create Server by binding
    // printf("Server start, server log\n");
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8000);
    inet_pton(AF_INET, "0.0.0.0", &server_addr.sin_addr);
    result = bind(sfd, (struct sockaddr *) &server_addr, sizeof(server_addr)); 
    if(result == -1){
        handle_error("bind");
    }

    // 2.) Server is listening
    // printf("Server listening, server log\n");
    result = listen(sfd,10);
    if(result == -1){
        handle_error("listen");
    }

    // 3.) Accepting client
    while(1){
        socklen_t client_addr_size;
        client_addr_size = sizeof(client_addr);
        // client -- file descriptor
        cfd = accept(sfd, (struct sockaddr *) &client_addr, &client_addr_size);
        if(result == -1){
            handle_error("accept");
        }

        //Read message from client sent to server - ONE MESSAGE
        char buffer[MAX_LEN] = {0};
        result = read(cfd, buffer, MAX_LEN);
        if (result == -1){
            handle_error("read");
        }
        unsigned int buffer_len = result;
        // printf("client (%u): %s\n", buffer_len, buffer);

        // GET /hello HTTP/1.1
        // HOST: localhost:8000
        // User-Agent: curl/8.8.0
        // Accept: */*
        
        ds_string_slice request, token;
        ds_string_slice_init(&request, buffer, buffer_len);

        ds_string_slice_tokenize(&request, ' ', &token);
        char *verb = NULL;
        ds_string_slice_to_owned(&token, &verb); 
        if (strcmp(verb, "GET")!=0){
            printf("not get request");
            // TODO respond with 400
            continue;
        }

        ds_string_slice_tokenize(&request, ' ', &token);
        char *path;
        ds_string_slice_to_owned(&token, &path);
        // printf("path requested: %s\n", path);

        ds_string_slice_tokenize(&request, '\n', &token);
        char *protocol = NULL;
        ds_string_slice_to_owned(&token, &protocol);
        // printf("protocol: %s\n", protocol);
        
        // TODO if file does not exist return 404
        char *content = NULL;
        int content_len = ds_io_read_file(path + 1, &content);

        ds_string_builder response_builder;
        ds_string_builder_init(&response_builder);
        ds_string_builder_append(&response_builder, "HTTP/1.1 200 ok\nContent-Type: text/html\nContent-Length: %d\n\n%s", content_len, content);
        char *response = NULL;
        ds_string_builder_build(&response_builder, &response);
        int response_len = strlen(response);

        // int response_len = snprintf(NULL, 0, "HTTP/1.1 200 ok\nContent-Type: text/html\nContent-Length: %d\n\n%s" , protocol, 6, "Hello\n");
        // char *response = calloc(response_len, sizeof(char));
        // snprintf(response, response_len, "%s 200 ok\nContent-Type: text/html\nContent-Length: %d\n\n%s", protocol, 6, "Hello\n");
        
        printf("response: '%s'",response);
        
        //send one message to client..
        // write(cfd,"200 ok\nContent-Type: text/html\nContent-Length: %d\n\n%s", 8);
        write(cfd, response, response_len);
    }

    // 4.) closing client
    result = close(cfd);
    if(result == -1){
        handle_error("close");
    }

    // 5.) closing server
    result = close(sfd);
    if(result == -1){
        handle_error("close");
    }

    printf("Server close, server log\n");
    return 1;
}
