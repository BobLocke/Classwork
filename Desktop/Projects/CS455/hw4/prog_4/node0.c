#include <stdio.h>
#include <stdlib.h>
#include "dv.h"

extern int TRACE;             /* for debugging */
extern int YES;
extern int NO;

struct distance_table dt0;
int costs0[4];



/* ========= Begin of student's work                      ========== */
/* ========= students to write the following three routines ========== */

/* initialization */
void rtinit0() {
 //intialize node's own distance table based on the network graph given in the prog_4 description
	int id = 0;
	dt0.costs[0][0] = 0;
	dt0.costs[0][1] = 5;
	dt0.costs[0][2] = 15;
	dt0.costs[0][3] = 10;
	for (int x = 0; x < 4; x++){
		if (x != id){
			dt0.costs[x][0] = 999;
			dt0.costs[x][1] = 999;
			dt0.costs[x][2] = 999;
			dt0.costs[x][3] = 999;
		}
	}
	costs0[0] = 0;
	costs0[1] = 5;
 	costs0[2] = 15;
 	costs0[3] = 10;
	struct rtpkt *packet;
	packet = malloc(sizeof(struct rtpkt));
	for(int x = 0; x < 4; x++){
	 	if (x != id && dt0.costs[id][x] != 999){	
	 		creatertpkt(packet, id, x, dt0.costs[id]);
	 		tolayer2(*packet);
	 	}
	}

 //any other neccessary initialization
 
}

/* get called when there is a packet from link layer (layer 2) (e.g., receive a packet from neighbor) */
void rtupdate0(struct rtpkt* rcvdpkt){
	int change = 0;
	int id = 0;
	int temp = 0;
	for(int x = 0; x < 4; x++){
		dt0.costs[rcvdpkt->sourceid][x] = rcvdpkt->mincost[x];
	}
	for (int x = 0; x < 4; x++){
		//if (x != id){
			temp = costs0[x];
			for (int y = 0; y < 4; y++){
				if (y != id){
					if (temp > costs0[y] + dt0.costs[y][x]){
						temp = costs0[y] + dt0.costs[y][x];						
					}
					//printf("NEW: %d\n", temp);
				}
			}
			if (temp != dt0.costs[id][x]){
				change = 1;
				dt0.costs[id][x] = temp;
			}
		//}		
	}

	if (change == 1){
		struct rtpkt *packet;
		packet = malloc(sizeof(struct rtpkt));
		for(int x = 0; x < 4; x++){
		 	if (x != id && costs0[x] != 999){	
		 		creatertpkt(packet, id, x, dt0.costs[id]);
		 		tolayer2(*packet);
		 	}
		}
	}
	printf("\nTime: %.2f", clocktime);
	printdt(&dt0, id);
}

/* get called when link cost changes  
 *  nodeid: link from this node to node id 
 *  newcost: c(0, nodeid) = newcost 
 *  */

void linkcostchange0(int nodeid, int newcost){
	int temp;
	int id = 0;
    printf("\n Link cost c(0, %d) is changed to %d \n", nodeid, newcost);
    costs0[nodeid] = newcost;
    //dt0.costs[0][nodeid] = newcost;
    for(int x = 0; x < 4; x++){
    	dt0.costs[0][x] = costs0[x];
    }

    for (int x = 0; x < 4; x++){
		//if (x != id){
			temp = costs0[x];
			for (int y = 0; y < 4; y++){
				//if (x != y){
					if (temp > costs0[y] + dt0.costs[y][x]){
						temp = costs0[y] + dt0.costs[y][x];						
					}
					//printf("NEW: %d\n", temp);
				//}
			}
			dt0.costs[id][x] = temp;
		//}		
	}
	printf("\nTime: %.2f", clocktime);
	printdt(&dt0, id);
    
    struct rtpkt *packet;
	packet = malloc(sizeof(struct rtpkt));
	for(int x = 0; x < 4; x++){
		 if (x != 0 && costs0[x] != 999){	
		 	creatertpkt(packet, 0, x, dt0.costs[0]);
		 	tolayer2(*packet);
		 }
	}
}

/* ========= End of student's work                        ========== */
