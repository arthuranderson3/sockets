#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include "common.h"

int main(int argc, char* argv[]) {
  if (argc != 3) {
    DieWithUserMessage("Parameter(s)", "<Address/Name> <Port/Service>");
  }

  char *addrStr = argv[1];
  char *portStr = argv[2];

  // Tell the system what kind of address info we got.
  struct addrinfo addrCriteria;
  memset(&addrCriteria, 0, sizeof(addrCriteria));
  addrCriteria.ai_family = AF_UNSPEC;
  addrCriteria.ai_socktype = SOCK_STREAM;
  addrCriteria.ai_protocol = IPPROTO_TCP;

  // Get addresses associated with the specified name/service
  struct addrinfo *addrList;
  int rtv = getaddrinfo(addrStr, portStr, &addrCriteria, &addrList);
  if (rtv != 0) {
    DieWithUserMessage("getaddrinfo() failed", gai_strerror(rtv));
  }

  for (struct addrinfo *addr = addrList; addr != NULL; addr = addr->ai_next) {
    PrintSocketAddress(addr->ai_addr, stdout);
    fputs("\n", stdout);
  }

  freeaddrinfo(addrList);
  exit(0);
}
