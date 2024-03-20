#ifndef SOCKETS_COMMON_H
#define SOCKETS_COMMON_H
#include <stdio.h>
#include <sys/socket.h>

#define BUFSIZE 255

void DieWithUserMessage(const char * msg, const char * detail);
void DieWithSystemMessage(const char * msg);

void HandleTcpClient(int clntSock);

/* Utility functions */
void PrintSocketAddress(const struct sockaddr * addr, FILE *stream);
int SetupTCPClientSocket(const char *host, const char *service);

#endif