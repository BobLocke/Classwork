#ifndef _DVH_
#define _DVH_

/* a rtpkt is the packet sent from one routing update process to
 *    another via the call tolayer3() */
struct rtpkt {
    int sourceid;       /* id of sending router sending this pkt */
    int destid;         /* id of router to which pkt being sent 
                           (must be an immediate neighbor) */
    int mincost[4];    /* min cost to node 0 ... 3 */
};  

/* data structure for distance table */
struct distance_table {
    int costs[4][4];
};

float clocktime;

/***************  Emulator API *************************/

/* send packet to link layer (layer 2) */
void tolayer2(struct rtpkt packet);

/* students to implement the following functions. These 
 * functions will be called by the emulator */

int min(int a, int b);

/* initialize functions for nodes */
void rtinit0();
void rtinit1();
void rtinit2();
void rtinit3();

/* link layer will deliver a packet via routing update functions*/
void rtupdate0(struct rtpkt* rcvdpkt);
void rtupdate1(struct rtpkt* rcvdpkt);
void rtupdate2(struct rtpkt* rcvdpkt);
void rtupdate3(struct rtpkt* rcvdpkt);

/* handle the case when the cost of link changes. Only need to implement
 * the handlers for node 0 and 1 for this program. */
void linkcostchange0(int nodeid,int  newcost);
void linkcostchange1(int nodeid,int  newcost);

/*************** END Emulator API *********************/

/* Helper functions */
/* pretty print out distance table for a node */
/* Parameters
 *  dtptr: pointer to node's distance_table
 *  nodeid: id of the node that has the dtptr
 **/
void printdt(struct distance_table *dtptr, int nodeid);

/* construct a packet to send down to layer 2*/
void creatertpkt( struct rtpkt* initrtpkt, int srcid, int destid, int mincosts[]);


/* for debugging purpose only */
int TRACE;             /* for debugging */
int YES;
int NO;

#endif
