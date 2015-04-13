#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>

#define PORTNO 49999

int main(int argc, char** argv){

	if(!argv[1]){
		fprintf(stderr, "ERROR: Please enter server IP.\n");
		return -1;
	}

char* serverID = argv[1];
int socketfd;
struct sockaddr_in servAddr;
struct hostent* hostEntry;
struct in_addr **pptr;
char buf [100];

socketfd = socket(AF_INET, SOCK_STREAM, 0);
if (socketfd < 0) {
    fprintf(stderr, "ERROR opening socket");
    return -1;
}

memset(&servAddr, 0, sizeof(servAddr));
servAddr.sin_family = AF_INET;
servAddr.sin_port = htons(PORTNO);

hostEntry = gethostbyname(serverID);
if (!hostEntry){
	herror("ERROR");
	return -1;
}
/* test for error using herror() */

/* this is magic, unless you want to dig into the man pages */
pptr = (struct in_addr **) hostEntry->h_addr_list;
memcpy(&servAddr.sin_addr, *pptr, sizeof(struct in_addr));

if (connect(socketfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0){
	fprintf(stderr, "Error: %s", strerror(errno));
	return -1;
}

read(socketfd, buf, 100);
printf("The current time and date is: %s \n", buf);
return 0;
}