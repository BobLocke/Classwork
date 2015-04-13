CC=gcc
COPTS=-g -std=c99 -pedantic -Wall -lm

all: turtle clean

turtle: parser.o scanner.o turtle.o symtab.o
	$(CC) $(COPTS) -o turtle parser.o scanner.o turtle.o symtab.o

scanner.o: scanner.c scanner.h
	$(CC) $(COPTS) -c scanner.c

turtle.o: turtle.c turtle.h
	$(CC) $(COPTS) -c turtle.c

parser.o: parser.c
	$(CC) $(COPTS) -c parser.c

symtab.o: symtab.c symtab.h
	$(CC) $(COPTS) -c symtab.c

clean: 
	rm -f *.o