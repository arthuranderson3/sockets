CC      = cc
CFLAGS  = -g
RM      = rm -f

default: all

all: echo-client

echo-client: echo-client-ipv4

echo-client-ipv4: echo-client-ipv4.c
	$(CC) $(CFLAGS) -o echo-client-ipv4 echo-client-ipv4.c

clean:
	rm -f echo-client-ipv4 
	rm -rf echo-client-ipv4.dSYM