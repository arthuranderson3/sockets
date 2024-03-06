#include <stdio.h>
#include <stdlib.h>
#include "common.h"

void DieWithUserMessage(char * msg, char * detail) {
  printf("%s:\t%s\n", msg, detail);
  exit(1);
}

void DieWithSystemMessage(char * msg) {
  perror(msg);
  exit(2);
}