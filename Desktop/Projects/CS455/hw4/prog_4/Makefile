all: dv

dv: dv.o dv.h node0.o node1.o node2.o node3.o
	gcc -std=c99  -Wall dv.o node0.o node1.o node2.o node3.o -o dv

node0.o: dv.h node0.c
	gcc -std=c99  -Wall -c node0.c

node1.o: dv.h node1.c
	gcc -std=c99  -Wall -c node1.c

node2.o: dv.h node2.c
	gcc -std=c99  -Wall -c node2.c

node3.o: dv.h node3.c
	gcc -std=c99  -Wall -c node3.c

clean: 
	rm -rf node*.o dv
