#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timer.h"
#include "application.h"
#include "network.h"
#include "emulator.h"

#define TIME 100.0
/* ******************************************************************
 GO-BACK-N NETWORK EMULATOR: 
 Network properties:
   - one way network delay averages five time units (longer if there
     are other messages in the channel for GBN), but can be larger
   - packets can be corrupted (either the header or the data portion)
     or lost, according to user-defined probabilities
   - packets will be delivered in the order in which they were sent
     (although some can be lost).
**********************************************************************/

/* Helper function - calculate checksum for a packet.
struct pkt is defined in emulator.h */

int Abase;
int Bbase;
int Anextseqnum;
int Bnextseqnum;
int Aexpectedseqnum;
int Bexpectedseqnum;
int cwnd = 5;
struct pkt Abuffer[100];
struct pkt Bbuffer[100];
struct pkt Aackpkt;
struct pkt Backpkt;



unsigned int calc_checksum(struct pkt packet)
{
  unsigned int checksum = 0;
  unsigned int i = 0;

  checksum = packet.seqnum + packet.acknum;
  for (i=0;i<20;i++) {
    checksum = checksum + (int)(packet.payload[i]);
  }
  return checksum;
}

/********* PROGRAMING 3 - COMPLETE THE FOLLOWING FUNCTIONS *********
     - You can add helper functions if its necessary 
     - Complete code for the emulator, application layer, and network layer
     are provided. However, you don't need to understand all of them. Indeed,
     all you need to know is what the following functions do and how to use them.
      + void stoptimer(int AorB);
      + void starttimer(int AorB,float increment);
      + void udt_send(int AorB, struct pkt packet);
      + void data_delivery(int AorB, char datasent[20]);
     - struct pkt is defined in emulator.h 

*********************************************************************/

/* Implementation for A */
/* called from layer 5 (application), passed the data to be sent to other side */
void A_rdtsend(struct msg message)
{
  if(Anextseqnum < Abase + cwnd){
      struct pkt sndpkt;
      sndpkt.seqnum = Anextseqnum;
      sndpkt.acknum = -1;
      strncpy(sndpkt.payload, message.data, 20);
      sndpkt.checksum = calc_checksum(sndpkt);
      Abuffer[Anextseqnum] = sndpkt;
      udt_send(0, sndpkt);
    if(Abase == Anextseqnum) {
      starttimer(0, TIME);
    }
    Anextseqnum = (Anextseqnum + 1) % 100;
  }   
}

/* called from layer 3(network), when a packet arrives for layer 4 (transport) at A */
void A_rdtrcv(struct pkt packet){
  if (packet.acknum >= 0 && packet.seqnum == -1 && packet.checksum == calc_checksum(packet)){
    Abase = (packet.acknum+1)%100;
    if (Abase == Anextseqnum)
      stoptimer(0);
    else 
      starttimer(0, TIME);
  }else if(packet.acknum == -1 && packet.seqnum >= 0){
    if (packet.seqnum == Aexpectedseqnum && packet.checksum == calc_checksum(packet)){
      data_delivery(0, packet.payload);
      //printf("\nMESSAGE RECIEVED on A: %s\n", packet.payload);
      Aackpkt.acknum = packet.seqnum;
      Aackpkt.seqnum = -1;
      strncpy(Aackpkt.payload, packet.payload, 20);
      Aackpkt.checksum = calc_checksum(Aackpkt);
      udt_send(0, Aackpkt);
      Aexpectedseqnum = (Aexpectedseqnum + 1) % 100;
    }else {
      udt_send(0, Aackpkt);
    }
  }
}

/* called when A's timer goes off */
void A_timerinterrupt()
{

  starttimer(0, TIME);
  if (Anextseqnum - Abase >= 0) {
    for (int i = Abase; i < Anextseqnum; i++){
      udt_send(0, Abuffer[i]);
    }
  } else {
    for (int i = Abase; i < 100 + Anextseqnum; i++){
        udt_send(0, Abuffer[i%100]);
    }
  }
}  

/* the following routine will be called once (only) before any other */
/* entity A routines are called. You can use it to do any initialization */
void A_init()
{
  Abase = 1;
  Anextseqnum = 1;
  Aexpectedseqnum = 1;
}

/*****************************************************************/

/* Implementation for B */
/* called from layer 5 (application), passed the data to be sent to other side */
void B_rdtsend(struct msg message)
{
  if(Bnextseqnum < Bbase + cwnd){
      struct pkt sndpkt;
      sndpkt.seqnum = Bnextseqnum;
      sndpkt.acknum = -1;
      strncpy(sndpkt.payload, message.data, 20);
      sndpkt.checksum = calc_checksum(sndpkt);
      Bbuffer[Bnextseqnum] = sndpkt;
      udt_send(1, sndpkt);
    if(Bbase == Bnextseqnum) {
      starttimer(1, TIME);
    }
    Bnextseqnum = (Bnextseqnum + 1) % 100;
  }   
}

/* called from layer 3(network), when a packet arrives for layer 4 (transport) at B */
void B_rdtrcv(struct pkt packet){
  if (packet.acknum >= 0 && packet.seqnum == -1 && packet.checksum == calc_checksum(packet)){
    Bbase = (packet.acknum+1)%100;
    if (Bbase == Bnextseqnum)
      stoptimer(1);
    else 
      starttimer(1, TIME);
  }else if(packet.acknum == -1 && packet.seqnum >= 0){
    if (packet.seqnum == Bexpectedseqnum && packet.checksum == calc_checksum(packet)){
      data_delivery(1, packet.payload);
      //printf("\nMESSAGE RECIEVED on B: %s\n", packet.payload);
      Backpkt.acknum = packet.seqnum;
      Backpkt.seqnum = -1;
      strncpy(Backpkt.payload, packet.payload, 20);
      Backpkt.checksum = calc_checksum(Backpkt);
      udt_send(1, Backpkt);
      Bexpectedseqnum = (Bexpectedseqnum + 1) % 100;
    }else {
      udt_send(1, Backpkt);
    }
  }
}

/* called when B's timer goes off */
void B_timerinterrupt()
{

  starttimer(1, TIME);
  if (Bnextseqnum - Bbase >= 0) {
    for (int i = Bbase; i < Bnextseqnum; i++){
      udt_send(1, Bbuffer[i]);
    }
  } else {
    for (int i = Bbase; i < 100 + Bnextseqnum; i++){
        udt_send(1, Bbuffer[i%100]);
    }
  }
}  

/* the following routine will be called once (only) before any other */
/* entity B routines are called. You can use it to do any initialization */
void B_init()
{
  Bbase = 1;
  Bnextseqnum = 1;
  Bexpectedseqnum = 1;
}
