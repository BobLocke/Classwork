all: rdt30_gbn

network.o: network.h
	gcc  -c -std=c99 -Wall network.c -o network.o

transport.o: transport.h
	gcc  -c -std=c99 -Wall transport.c -o transport.o

application.o: application.h
	gcc  -c -std=c99 -Wall application.c -o application.o

emulator.o: emulator.h
	gcc  -c -std=c99 -Wall emulator.c -o emulator.o

timer.o: timer.h
	gcc  -c -std=c99 -Wall timer.c -o timer.o

rdt30_gbn: application.o transport.o network.o emulator.o timer.o
	gcc -std=c99 -Wall application.o transport.o network.o emulator.o timer.o -lm -o rdt30_gbn


clean: 
	rm -rf rdt30_gbn *.o
