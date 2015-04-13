#include <stdio.h>
#include "emulator.h"

void data_delivery(int AorB,char datasent[20]){
  int i;  
  if (TRACE>2) {
     printf("          Application: data received: ");
     for (i=0; i<20; i++)  
        printf("%c",datasent[i]);
     printf("\n");
   }
  
}