CC=gcc
COPTS=-g -std=c99 -Wall
LIB=-L$(GET_WORD)/lib
INCL=-I$(GET_WORD)/include

WordMatch: p3.c
	$(CC) $(COPTS) -D_BSD_SOURCE p3.c -o readable