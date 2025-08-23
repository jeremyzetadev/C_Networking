#ifndef SOCKETUTIL_H
#define SOCKETUTIL_H

#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <malloc.h>


int createTCPIPv4Socket();
struct sockaddr_in *createIPv4Address(char *ip, int port);


#endif // SOCKETUTIL_H
