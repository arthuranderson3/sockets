#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "common.h"

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