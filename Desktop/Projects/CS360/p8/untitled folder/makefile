CC=gcc
COPTS=-g -std=c99 -Wall
LIB=-L$(GET_WORD)/lib
INCL=-I$(GET_WORD)/include

Sort: sort.c
	$(CC) $(COPTS) $(INCL) -D_XOPEN_SOURCE sort.c -o Sort -lpthread $(LIB) -lget -lm