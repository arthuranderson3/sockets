CC      = cc
CFLAGS  = -g
RM      = rm -f

default: all

all: echo-client echo-server

echo-client: echo-client-ipv4

echo-server: echo-server-ipv4

echo-client-ipv4: echo-client-ipv4.c die-with-message.c common.h
	$(CC) $(CFLAGS) -o echo-client-ipv4 echo-client-ipv4.c die-with-message.c

echo-server-ipv4: echo-serv-ipv4.c die-with-message.c common.h
	$(CC) $(CFLAGS) -o echo-server-ipv4 echo-serv-ipv4.c die-with-message.c

clean:
	rm -f echo-client-ipv4 echo-server-ipv4
	rm -rf echo-client-ipv4.dSYM echo-server-ipv4.dSYM