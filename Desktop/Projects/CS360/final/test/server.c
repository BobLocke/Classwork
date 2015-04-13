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
#include <sys/wait.h>
#include <fcntl.h>

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

int listenserv(){
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

char* getPort(int datafd, struct sockaddr_in servAddr){

	if (bind(datafd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
		perror("bind");
		exit(1);
	}

	char* portno = malloc(16);
	struct sockaddr_in getPort;
	unsigned int len = sizeof(getPort);
	if (getsockname(datafd, (struct sockaddr *) &getPort, &len) == -1) {
      perror("getsockname() failed");
      return "E\0";
    }
    int port = ntohs(getPort.sin_port);
    sprintf(portno, "%d", port);
	return portno;
}

int dataConn(int listenfd){
	struct sockaddr_in clientAddr;
	unsigned int length = sizeof(struct sockaddr_in);
	int connectfd = accept(listenfd, (struct sockaddr *) &clientAddr, &length);
	return connectfd; 
	}

void rls(int datafd){
	int x;
	if(!(x = fork())){
		close(1); dup(datafd);
		execlp("ls", "ls", "-l", NULL);
		perror("execlp");
		exit(1);
	}
	wait(&x);
	return;
}

int rcd(char* path){
	strtok(path, "\n");
	if(chdir(path)) {
		printf("Error: Path not found or not accessable.\n");
		return 1;
	}
	printf("Directory changed.\n");
	return 0;
}

void get(int datafd,int connectfd, char* path){
	//strtok(path, "\n");
	int fd;
	int bytes;
	char buf [512];
	if((fd = open(path, O_RDONLY, 0600)) < 0) {
		perror("Get Error");
		write(connectfd, "EError: Get Failed.\n", 20);
		return;
	}
	write(connectfd, "A\n", 2);

	while ((bytes = read(fd, buf, 512)) > 0){

		write(datafd, buf, bytes);
	}

	close(fd);
	return;
}

int put(int datafd, int connectfd, char* path){
	//strtok(path, "\n");
	int fd;
	int bytes;
	char buf [512];
		if((fd = open(path, O_WRONLY | O_CREAT | O_EXCL, 0600)) < 0) {
			perror("Put Error");
			write(connectfd, "EError: Put Failed.\n", 20);
			return 1;
		}
		write(connectfd, "A\n", 2);
		while ((bytes = read(datafd, buf, 512)) > 0){
			write(fd, buf, bytes);
		}
		close(fd);
	return 0;
}
	



int main(int argc, char** argv){

	char buf[4096];

	if(!argv[1]){
		fprintf(stderr, "ERROR: Please enter port number.\n");
		return -1;
	}

	int PORTNO = atoi(argv[1]);
	int listenfd = listenserv();
	int datafd = 0;
	int i;
	int listendatafd = 0;
	char* dataport;
	struct sockaddr_in servAddr = makeServAddr(PORTNO);
	struct sockaddr_in dataAddr;

	if (bind(listenfd, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0) {
		perror("bind");
		exit(1);
	}

	listen(listenfd, 4);

	socklen_t length = sizeof(struct sockaddr_in);
	struct sockaddr_in clientAddr;

	int connectfd;
	struct hostent* hostEntry;
	char* hostName;
	char reply[512];
	char b;

	while (1) { 
		connectfd = accept(listenfd, (struct sockaddr *) &clientAddr, &length);
		if (connectfd < 0){
			fprintf(stderr, "Connection Error");
			return -1;
		}

		if (!fork()){				
			hostEntry = gethostbyaddr(&(clientAddr.sin_addr), sizeof(struct in_addr), AF_INET);
			hostName = hostEntry->h_name;
			printf("Child %i: Connection from %s at %s \n", getpid(), hostName, getTime());
			while(1){
				if (!read(connectfd, &b, 1)){
					printf("Child %i: Error: Connection dropped.\n", getpid());
					exit(1);
				}
				i = 0;
				while(b != '\n'){
					buf[i] = b;
					i++;
					read(connectfd, &b, 1);
				}
				buf[i] = '\0';
				switch(buf[0]){
					case 'Q':
						printf("Child %i: Client from %s disconnected.\n", getpid(), hostName);
						write(connectfd, "A\n", 2);
						return 0;
					case 'D':
						listendatafd = listenserv();
						dataAddr = makeServAddr(0);
						dataport = getPort(listendatafd, dataAddr);
						strcpy(reply, "A");
						strcat(reply, dataport);
						strcat(reply, "\n");
						write(connectfd, reply, (int) strlen(reply));	
						listen(listendatafd, 1);
						datafd = dataConn(listendatafd);
						continue;
					case 'L':
						if(!listendatafd){
							printf("Child %i: Error: Data connection not established.\n", getpid());
						} else {
							rls(datafd);
							write(connectfd, "A\n", 2);
							close(datafd);
							close(listendatafd);
							datafd = 0;
							listendatafd = 0;
						}
						continue;
					case 'C':
							if (!rcd(buf+1)) {
							write(connectfd, "A\n", 2);
							} else write(connectfd, "ERCD failed.\n", 13);
							continue;
					case 'G':
					if(!listendatafd){
							printf("Child %i: Error: Data connection not established.\n", getpid());
						} else {
							get(datafd, connectfd, buf+1);
							close(datafd);
							close(listendatafd);
							datafd = 0;
							listendatafd = 0;
						}
						continue;
					case 'P':
					if(!listendatafd){
							printf("Child %i: Error: Data connection not established.\n", getpid());
						} else {
							put(datafd, connectfd, buf+1);
							close(datafd);
							close(listendatafd);
							datafd = 0;
							listendatafd = 0;
						}
						continue;
					default:
						continue;
				}
			}
		} else {
			close(connectfd);
		}
	}
	

    return 0;
}
