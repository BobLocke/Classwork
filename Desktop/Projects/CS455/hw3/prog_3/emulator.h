#ifndef _EMULATORH_
#define _EMULATORH_

#define BIDIRECTIONAL 1    /* 1 for bidirectional */
extern int TRACE;          /* for debugging: 2 for verbal */

struct event {
   float evtime;           /* event time */
   int evtype;             /* event type code */
   int eventity;           /* entity where event occurs */
   struct pkt *pktptr;     /* ptr to packet (if any) assoc w/ this event */
   struct event *prev;
   struct event *next;
 };

extern struct event *evlist;   /* the event list */
/* a "msg" is the data unit passed from layer 5 (Application layer ) to layer  */
/* 4 (Transport layer).  It contains the data (characters) to be delivered */
/* to applications via the students transport level protocol entities.         */
struct msg {
  char data[20];
};

/* a packet is the data unit passed from layer 4 (Transport layer) to layer */
/* 3 (Network layer).  Note the pre-defined packet structure, which all   */
/* students must follow. */

struct pkt {
   int seqnum;
   int acknum;
   int checksum;
   char payload[20];
};


/* possible events: */
#define  TIMER_INTERRUPT 0  
#define  FROM_LAYER5     1
#define  FROM_LAYER3     2

#define  OFF             0
#define  ON              1
#define   A              0
#define   B              1



extern int nsim;              /* number of messages from 5 to 4 so far */ 
extern int nsimmax;           /* number of msgs to generate, then stop */
extern float time;
extern float lossprob;            /* probability that a packet is dropped  */
extern float corruptprob;         /* probability that one bit is packet is flipped */
extern float lambda;              /* arrival rate of messages from layer 5 */   
extern int   ntolayer3;           /* number sent into layer 3 */
extern int   nlost;               /* number lost in media */
extern int ncorrupt;              /* number corrupted by media*/

/* simulator internal methods */
void init() ;
float jimsrand();
void generate_next_arrival();
void insertevent(struct event *p);

#endif
