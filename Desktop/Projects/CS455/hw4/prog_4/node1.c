#include <stdio.h>
#include <stdlib.h>
#include "dv.h"

extern int TRACE;             /* for debugging */
extern int YES;
extern int NO;

struct distance_table dt1;
int costs1[4];


/* ========= Begin of student's work                      ========== */
/* ========= students to write the following three routines ========== */
void rtinit1() {
	int id = 1;
	dt1.costs[1][0] = 5;
	dt1.costs[1][1] = 0;
 	dt1.costs[1][2] = 1;
 	dt1.costs[1][3] = 999;
 	for (int x = 0; x < 4; x++){
		if (x != id){
			dt1.costs[x][0] = 999;
			dt1.costs[x][1] = 999;
			dt1.costs[x][2] = 999;
			dt1.costs[x][3] = 999;
		}
	}
 	costs1[0] = 5;
	costs1[1] = 0;
 	costs1[2] = 1;
 	costs1[3] = 999;
 	struct rtpkt *packet;
	packet = malloc(sizeof(struct rtpkt));
	for(int x = 0; x < 4; x++){
	 	if (x != id && dt1.costs[id][x] != 999){	
	 		creatertpkt(packet, id, x, dt1.costs[id]);
	 		tolayer2(*packet);
	 	}
	}

}

void rtupdate1(struct rtpkt* rcvdpkt){
	int change = 0;
	int id = 1;
	int temp = 0;
	for(int x = 0; x < 4; x++){
		dt1.costs[rcvdpkt->sourceid][x] = rcvdpkt->mincost[x];
	}
	for (int x = 0; x < 4; x++){
		if (x != id){
			temp = costs1[x];
			for (int y = 0; y < 4; y++){

				if (y != id){
					//printf("(%d, %d) Old temp: %d, new temp: %d\n", y, x, temp, costs1[y] + dt1.costs[y][x]);
					if (temp > costs1[y] + dt1.costs[y][x]){
						temp = costs1[y] + dt1.costs[y][x];						
					}
					//printf("NEW: %d\n", temp);
				}
			}
			if (temp != dt1.costs[id][x]){
				change = 1;
				dt1.costs[id][x] = temp;
			}
		}		
	}

	if (change == 1){
		struct rtpkt *packet;
		packet = malloc(sizeof(struct rtpkt));
		for(int x = 0; x < 4; x++){
		 	if (x != id && costs1[x] != 999){	
		 		creatertpkt(packet, id, x, dt1.costs[id]);
		 		tolayer2(*packet);
		 	}
		}
	}
	printf("\nTime: %.2f", clocktime);
	printdt(&dt1, id);
}

/* get called when link cost changes
 * nodeid: node that associate with the link (1, nodeid) 
 * newcost: c(1, nodeid) = newcost 
 * */

void linkcostchange1(int nodeid, int newcost)   {
	int temp;
	int id = 1;
    printf("\n Link cost c(1, %d) is changed to %d \n", nodeid, newcost);
    costs1[nodeid] = newcost;
    //dt1.costs[1][nodeid] = newcost;
    for(int x = 0; x < 4; x++){
    	dt1.costs[1][x] = costs1[x];
    }
    for (int x = 0; x < 4; x++){
		//if (x != id){
			temp = costs1[x];
			for (int y = 0; y < 4; y++){

				if (y != id){
					if (temp > costs1[y] + dt1.costs[y][x]){
						temp = costs1[y] + dt1.costs[y][x];						
					}
					//printf("NEW: %d\n", temp);
				}
			}

		dt1.costs[id][x] = temp;		
		//}		
	}
	printf("\nTime: %.2f", clocktime);
	printdt(&dt1, id);

    struct rtpkt *packet;
	packet = malloc(sizeof(struct rtpkt));
	for(int x = 0; x < 4; x++){
		 if (x != 1 && costs1[x] != 999){	
		 	creatertpkt(packet, 1, x, dt1.costs[1]);
		 	tolayer2(*packet);
		 }
	}
}
/* ========= End of student's work                      ========== */
