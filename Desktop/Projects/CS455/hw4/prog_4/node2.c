#include <stdio.h>
#include <stdlib.h>
#include "dv.h"

int TRACE;             /* for debugging */
int YES;
int NO;

struct distance_table dt2;
int costs2[4];


/* ========= Begin of student's work                      ========== */
/* ========= students to write the following two routines ========== */
void rtinit2() {
	int id = 2;
	dt2.costs[2][0] = 15;
 	dt2.costs[2][1] = 1;
 	dt2.costs[2][2] = 0;
 	dt2.costs[2][3] = 12;
 	for (int x = 0; x < 4; x++){
		if (x != id){
			dt2.costs[x][0] = 999;
			dt2.costs[x][1] = 999;
			dt2.costs[x][2] = 999;
			dt2.costs[x][3] = 999;
		}
	}
 	costs2[0] = 15;
 	costs2[1] = 1;
 	costs2[2] = 0;
 	costs2[3] = 12;
 	struct rtpkt *packet;
	packet = malloc(sizeof(struct rtpkt));
	for(int x = 0; x < 4; x++){
	 	if (x != id && dt2.costs[id][x] != 999){	
	 		creatertpkt(packet, id, x, dt2.costs[id]);
	 		tolayer2(*packet);
	 	}
	}
}

void rtupdate2(struct rtpkt* rcvdpkt){
	int change = 0;
	int id = 2;
	int temp = 0;
	for(int x = 0; x < 4; x++){
		dt2.costs[rcvdpkt->sourceid][x] = rcvdpkt->mincost[x];
	}
	for (int x = 0; x < 4; x++){
		//if (x != id){
			temp = costs2[x];
			for (int y = 0; y < 4; y++){

				if (y != id){
					//printf("(%d, %d) Old temp: %d, new temp: %d\n", x, y, temp, costs2[y] + dt2.costs[y][x]);
					if (temp > costs2[y] + dt2.costs[y][x]){
						
						temp = costs2[y] + dt2.costs[y][x];						
					}
					//printf("NEW: %d\n", temp);
				}
			}
			if (temp != dt2.costs[id][x]){
				change = 1;
				dt2.costs[id][x] = temp;
			}
		//}		
	}

	if (change == 1){
		struct rtpkt *packet;
		packet = malloc(sizeof(struct rtpkt));
		for(int x = 0; x < 4; x++){
		 	if (x != id && costs2[x] != 999){	
		 		creatertpkt(packet, id, x, dt2.costs[id]);
		 		tolayer2(*packet);
		 	}
		}
	}
	printf("\nTime: %.2f", clocktime);
	printdt(&dt2, id);
}
/* ========= End of student's work                      ========== */

