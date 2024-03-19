#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "common.h"

// Max pending connection requests
static const int MAX_PENDING = 5;

int main(int argc, char *argv[]) {
  if (argc != 2) {
    DieWithUserMessage("Parameter(s)", "<Server Port>");
  }

  in_port_t srvPort = atoi(argv[1]);

  // Create socket for incoming connections
  int srvSock = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
  if (srvSock < 0) {
    DieWithSystemMessage("socket() failed");
  }

  struct sockaddr_in6 srvAddr;
  memset(&srvAddr, 0, sizeof(srvAddr));
  srvAddr.sin6_family = AF_INET6;
  srvAddr.sin6_addr = in6addr_any;
  srvAddr.sin6_port = htons(srvPort);

  if (bind(srvSock, (struct sockaddr*) &srvAddr, sizeof(srvAddr)) < 0) {
    DieWithSystemMessage("bind() failed");
  }

  if (listen(srvSock, MAX_PENDING) < 0) {
    DieWithSystemMessage("listen() failed");
  }

  for (;;) {
    struct sockaddr_in6 clntAddr;  // Client address
    socklen_t clntAddrLen = sizeof(clntAddr);
    int clntSock = accept(srvSock, (struct sockaddr *)&clntAddr, &clntAddrLen);
    if (clntSock < 0) {
      DieWithSystemMessage("accept() failed");
    }
    char clntName[INET6_ADDRSTRLEN];
    if (inet_ntop(AF_INET6, &clntAddr.sin6_addr.s6_addr, clntName, sizeof(clntName)) != NULL) {
      printf("Handling client %s/%d\n", clntName, ntohs(clntAddr.sin6_port));
    } else {
      puts("Unable to get client address");
    }
    HandleTcpClient(clntSock);
  }
}

