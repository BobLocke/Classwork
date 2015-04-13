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
#include <assert.h>

//#define PORTNO 1620

/* 	Ryan "Bob" Dean
	CS 360 - Assignment 8 - Date/Time Server/Client
*/

char* getTime(){
	time_t currentTime;
  	struct tm* timeinfo;
  	time(&currentTime);
  	timeinfo = localtime(&currentTime);
  	return asctime(timeinfo);
  }


// Sets up the listen socket

int listenserv(int PORTNO){
  	int listenfd;
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd < 0) {
        fprintf(stderr, "ERROR opening socket");
        return -1;
    }

/* 
Code snippet below taken from http://ubuntuforums.org/showthread.php?t=1351359

Without it, rerunning the server fails because the port number is still "in use" even after the server closes.

*/

// Begin 
    const int optVal = 1;
	const socklen_t optLen = sizeof(optVal);
	int rtn = setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (void*) &optVal, optLen);
	assert(rtn == 0);
// End

	return listenfd;
}

struct sockaddr_in makeServAddr(int PORTNO){
    struct sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(PORTNO);
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	return servAddr;
}

int main(int argc, char** argv){

	if(!argv[1]){
		fprintf(stderr, "ERROR: Please enter port number.\n");
		return -1;
	}

	int PORTNO = atoi(argv[1]);
	int listenfd = listenserv(PORTNO);
	struct sockaddr_in servAddr = makeServAddr(PORTNO);

	if (bind(listenfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
		perror("bind");
		exit(1);
	}

	listen(listenfd, 1);

	socklen_t length = sizeof(struct sockaddr_in);
	struct sockaddr_in clientAddr;

	int connectfd;
	struct hostent* hostEntry;
	char* hostName;
	while (1) { 
		connectfd = accept(listenfd, (struct sockaddr *) &clientAddr, &length);
		if (connectfd < 0){
			fprintf(stderr, "Connection Error");
			return -1;
		}

		if (fork()){				
			hostEntry = gethostbyaddr(&(clientAddr.sin_addr), sizeof(struct in_addr), AF_INET);
			hostName = hostEntry->h_name;
			printf("Connection from %s at %s \n", hostName, getTime());
			char* currentTime = getTime();
			write(connectfd, currentTime, 100);
		} else {}
	}
	

    return 0;
}
