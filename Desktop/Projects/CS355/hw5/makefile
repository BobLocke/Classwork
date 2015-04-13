CC=gcc
COPTS=-g -std=c99 -Wall
LIB=-L$(GET_WORD)/lib
INCL=-I$(GET_WORD)/include

All: listaxioms 3sat

listaxioms: listaxioms.c list.c list.h
	$(CC) $(COPTS) listaxioms.c list.c -o listaxioms

3sat: 3sat.c list.c list.h getword.c getword.h
	$(CC) $(COPTS) 3sat.c list.c getword.c -o 3sat