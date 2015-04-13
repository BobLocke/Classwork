CC=gcc
COPTS=-g -std=c99 -Wall
LIB=-L$(GET_WORD)/lib
INCL=-I$(GET_WORD)/include

All: Server Client

Server: server.c
	$(CC) $(COPTS) server.c -o Server

Client: client.c
	$(CC) $(COPTS) client.c -o Client