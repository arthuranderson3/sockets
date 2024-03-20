CC      = cc
CFLAGS  = -g
RM      = rm -f

default: all

all: echo-client echo-server addr-info

echo-client: echo-client-ipv4

echo-server: echo-server-ipv4 echo-server-ipv6

addr-info: get-addr-info.c common.c common.h
	$(CC) $(CFLAGS) -o addr-info get-addr-info.c common.c

echo-client-ipv4: echo-client-ipv4.c common.c common.h
	$(CC) $(CFLAGS) -o echo-client-ipv4 echo-client-ipv4.c common.c

echo-server-ipv4: echo-serv-ipv4.c common.c common.h
	$(CC) $(CFLAGS) -o echo-server-ipv4 echo-serv-ipv4.c common.c

echo-server-ipv6: echo-serv-ipv6.c common.c common.h
	$(CC) $(CFLAGS) -o echo-server-ipv6 echo-serv-ipv6.c common.c

clean:
	rm -f echo-client-ipv4 echo-server-ipv4 echo-server-ipv6 addr-info
	rm -rf echo-client-ipv4.dSYM echo-server-ipv4.dSYM echo-server-ipv6.dSYM addr-info.dSYM