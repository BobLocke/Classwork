The whole emulator consists of four components
 1. Timer: provides start/stop timer. Implementation is in timer.h and timer.c
 2. Network layer: provides implementation of tolayer3 functionality. This is the function that the transport layer can call to "push" a packet to the network. The implementation is in network.h and network.c
 3. Application layer: provides implementation of tolayer5 functionality. This is the function that the transport layer can call to "deliver" data to an application. The implementation is in application.h and application.c
 4. Transport layer: the template code is in transport.h and transport.c, which will be completed as part of programming 3. 
 5. Emulator: A program that put together the whole thing and simulate two nodes A and B. The implementation is in emulator.h and emulator.c

 To complile the code, type make from a terminal. 
 