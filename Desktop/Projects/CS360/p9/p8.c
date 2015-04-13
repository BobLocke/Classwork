#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <time.h>

#define PORTNO 1620
/* 	Ryan "Bob" Dean
	CS 360 - Assignment 8 - Network
*/

char* getTime(){
	time_t currentTime;
  	struct tm * timeinfo;
  	time (&currentTime);
  	timeinfo = localtime (&currentTime);
  	return asctime(timeinfo);
  }


int main(int argc, char** argv){

	int listenfd;
	socklen_t length = sizeof(struct sockaddr_in);
	struct sockaddr_in clientAddr;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd < 0) {
        fprintf(stderr, "ERROR opening socket");
        return -1;
    }
    struct sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(PORTNO);
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind( listenfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
		perror("bind");
		exit(1);
	}

	listen(listenfd, 1);

	int connectfd;
	connectfd = accept(listenfd, (struct sockaddr *) &clientAddr, &length);
		if (connectfd < 0){
			fprintf(stderr, "Connection Error");
			return -1;
		}

	struct hostent* hostEntry;
	char* hostName;
	hostEntry = gethostbyaddr(&(clientAddr.sin_addr), sizeof(struct in_addr), AF_INET);
	hostName = hostEntry->h_name;


	char* currentTime = getTime();
	printf("%s \n", currentTime);

    return 0;
}
