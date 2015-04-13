CC=gcc
COPTS=-g -std=c99 -Wall
LIB=-L$(GET_WORD)/lib
INCL=-I$(GET_WORD)/include

Server: dayserve.c
	$(CC) $(COPTS) dayserve.c -o Server

Client: daytime.c
	$(CC) $(COPTS) daytime.c -o Client