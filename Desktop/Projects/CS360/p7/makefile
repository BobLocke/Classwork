CC=gcc
COPTS=-g -std=c99 -Wall
LIB=-L$(GET_WORD)/lib
INCL=-I$(GET_WORD)/include

Philo: sample.c random_r.c
	$(CC) $(COPTS) -D_XOPEN_SOURCE sample.c random_r.c -o Philo -lpthread -lm