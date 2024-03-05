#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE 255
void DieWithUserMessage(char * msg, char * detail);
void DieWithSystemMessage(char * msg);

int main(int argc, char* argv[]) {
  if (argc < 3 || argc > 4) {
    DieWithUserMessage("Parameters", "<server address> <echo word> [<server port>]");
  }

  char *servIP = argv[1];
  char *echoStr = argv[2];
  in_port_t servPort = (argc == 4) ? atoi(argv[3]) : 7; // 7 is well known echo port

  int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (sock < 0) {
    DieWithSystemMessage("socket() failed");
  }

  // construct the server address structure
  struct sockaddr_in servAddr;
  memset(&servAddr, 0, sizeof(servAddr));
  servAddr.sin_family = AF_INET;
  int retval = inet_pton(AF_INET, servIP, &servAddr.sin_addr.s_addr);
  if (retval < 0) {
    DieWithUserMessage("inet_pton() failed", "invalid address string");
  }
  servAddr.sin_port = htons(servPort);

  if (connect(sock, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
    DieWithSystemMessage("connect() failed");
  }

  size_t echoLen = strlen(echoStr);

  ssize_t numBytes = send(sock, echoStr, echoLen, 0);
  if (numBytes < 0) {
    DieWithSystemMessage("send() failed");
  } else if (numBytes != echoLen) {
    DieWithUserMessage("send()", "unexpected number of bytes");
  }

  unsigned int totalBytesRcvd = 0;
  fputs("received: ", stdout);
  while (totalBytesRcvd < echoLen) {
    char buffer[BUFSIZE];
    numBytes = recv(sock, buffer, BUFSIZE - 1, 0);
    if (numBytes < 0) {
      DieWithSystemMessage("recv() failed");
    } else if (numBytes == 0) {
      DieWithUserMessage("recv failed", "connection closed prematurely");
    }
    totalBytesRcvd += numBytes;
    buffer[numBytes] = '\0';
    fputs(buffer, stdout);
  }

  fputc('\n', stdout);
  return 0;
}

void DieWithUserMessage(char * msg, char * detail) {
  printf("%s:\t%s\n", msg, detail);
  exit(1);
}

void DieWithSystemMessage(char *msg) {
  printf("%s\n", msg);
  exit(2);
}