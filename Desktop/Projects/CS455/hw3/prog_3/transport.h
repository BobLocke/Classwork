#ifndef _TRANSPORTH_
#define _TRANSPORTH_

#include "emulator.h"

/* Implementation for A */
/* called from layer 5, passed the data to be sent to other side */
void A_rdtsend(struct msg message);

/* called from layer 3, when a packet arrives for layer 4 */
void A_rdtrcv(struct pkt packet);

/* called when A's timer goes off */
void A_timerinterrupt(); 

/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init();


/* Implementation for B */
/* called from layer 5, passed the data to be sent to other side */
void B_rdtsend(struct msg message);

/* called from layer 3, when a packet arrives for layer 4 at B*/
void B_rdtrcv(struct pkt packet);

/* called when B's timer goes off */
void B_timerinterrupt();

/* the following rouytine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init();

#endif