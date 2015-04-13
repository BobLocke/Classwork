CC=gcc
COPTS=-g -std=c99 -Wall
LIB=-L$(GET_WORD)/lib
INCL=-I$(GET_WORD)/include

WordPairs: p1.c Hashtable.c Hashtable.h
	$(CC) $(COPTS) $(INCL) p1.c Hashtable.c -o WordPairs $(LIB) -lget