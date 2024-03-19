#ifndef SOCKETS_COMMON_H
#define SOCKETS_COMMON_H

#define BUFSIZE 255

void DieWithUserMessage(char * msg, char * detail);
void DieWithSystemMessage(char * msg);

void HandleTcpClient(int clntSock);

#endif