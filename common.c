#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include "common.h"

void DieWithUserMessage(const char * msg, const char * detail) {
  printf("%s:\t%s\n", msg, detail);
  exit(1);
}

void DieWithSystemMessage(const char * msg) {
  perror(msg);
  exit(2);
}

void HandleTcpClient(int clntSock) {
  char buffer[BUFSIZE];
  ssize_t numBytesRcvd = recv(clntSock, buffer, BUFSIZE, 0);
  if (numBytesRcvd < 0) {
    DieWithSystemMessage("recv() failed");
  }

  while (numBytesRcvd > 0) {
    ssize_t numBytesSent = send(clntSock, buffer, numBytesRcvd, 0);
    if (numBytesSent < 0) {
      DieWithSystemMessage("send() failed");
    } else if (numBytesRcvd != numBytesSent) {
      DieWithUserMessage("send()", "sent unexpected number of bytes");
    }

    numBytesRcvd = recv(clntSock, buffer, BUFSIZE, 0);
    if (numBytesRcvd < 0) {
      DieWithSystemMessage("recv() failed");
    }
  }
  close(clntSock);
}

void PrintSocketAddress(const struct sockaddr * addr, FILE *stream) {
  if (addr == NULL || stream == NULL) {
    return;
  }

  void *binAddr;
  char addrBuff[INET6_ADDRSTRLEN];
  in_port_t port;
  switch(addr->sa_family) {
  case AF_INET:
    binAddr = &((struct sockaddr_in *) addr)->sin_addr;
    port = ntohs(((struct sockaddr_in *) addr)->sin_port);
    break;
  case AF_INET6:
    binAddr = &((struct sockaddr_in6 *) addr)->sin6_addr;
    port = ntohs(((struct sockaddr_in6 *) addr)->sin6_port);
    break;
  default:
    fputs("[unknown type]", stream);
    return;
  }

  if (inet_ntop(addr->sa_family, binAddr, addrBuff, sizeof(addrBuff)) == NULL) {
    fputs("[invalid address]", stream);
  } else {
    fprintf(stream, "%s", addrBuff);
    if (port != 0) {
      fprintf(stream, " -%u", port);
    }
  }
}

int SetupTCPClientSocket(const char *host, const char *service) {
  struct addrinfo addrCriteria;
  memset(&addrCriteria, 0, sizeof(addrCriteria));
  addrCriteria.ai_family = AF_UNSPEC;
  addrCriteria.ai_socktype = SOCK_STREAM;
  addrCriteria.ai_protocol = IPPROTO_TCP;

  struct addrinfo *srvAddr;
  int rtv = getaddrinfo(host, service, &addrCriteria, &srvAddr);
  if (rtv != 0) {
    DieWithUserMessage("getaddrinfo() failed", gai_strerror(rtv));
  }

  int sock = -1;
  for (struct addrinfo * addr = srvAddr; addr != NULL;  addr = addr->ai_next) {
    sock = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
    if (sock < 0) {
      continue; // try again with next address
    }

    if (connect(sock, addr->ai_addr, addr->ai_addrlen) == 0) {
      break;
    }

    close(sock);
    sock = -1;
  }
  freeaddrinfo(srvAddr);
  return sock;
}
