#include <stdio.h>
#include <stdlib.h>
#include "dv.h"

int TRACE;             /* for debugging */
int YES;
int NO;

struct distance_table dt3;
int costs3[4];

/* ========= Begin of student's work                      ========== */
/* ========= students to write the following two routines ========== */
void rtinit3() {
	int id = 3;
	dt3.costs[3][0] = 10;
 	dt3.costs[3][1] = 999;
 	dt3.costs[3][2] = 12;
 	dt3.costs[3][3] = 0;
 	for (int x = 0; x < 4; x++){
		if (x != id){
			dt3.costs[x][0] = 999;
			dt3.costs[x][1] = 999;
			dt3.costs[x][2] = 999;
			dt3.costs[x][3] = 999;
		}
	}
 	costs3[0] = 10;
 	costs3[1] = 999;
 	costs3[2] = 12;
 	costs3[3] = 0;
 	struct rtpkt *packet;
	packet = malloc(sizeof(struct rtpkt));
	for(int x = 0; x < 4; x++){
	 	if (x != id && dt3.costs[id][x] != 999){	
	 		creatertpkt(packet, id, x, dt3.costs[id]);
	 		tolayer2(*packet);
	 	}
	}
}

void rtupdate3(struct rtpkt* rcvdpkt){
		int change = 0;
	int id = 3;
	int temp = 0;
	for(int x = 0; x < 4; x++){
		dt3.costs[rcvdpkt->sourceid][x] = rcvdpkt->mincost[x];
	}
	for (int x = 0; x < 4; x++){
		//if (x != id){
			temp = costs3[x];
			for (int y = 0; y < 4; y++){

				if (y != id){
					if (temp > costs3[y] + dt3.costs[y][x]){
						temp = costs3[y] + dt3.costs[y][x];						
					}
					//printf("NEW: %d\n", temp);
				}
			}
			if (temp != dt3.costs[id][x]){
				change = 1;
				dt3.costs[id][x] = temp;
			}
		//}		
	}

	if (change == 1){
		struct rtpkt *packet;
		packet = malloc(sizeof(struct rtpkt));
		for(int x = 0; x < 4; x++){
		 	if (x != id && costs3[x] != 999){	
		 		creatertpkt(packet, id, x, dt3.costs[id]);
		 		tolayer2(*packet);
		 	}
		}
	}
	printf("\nTime: %.2f", clocktime);
	printdt(&dt3, id);
}
/* ========= End of student's work                      ========== */
