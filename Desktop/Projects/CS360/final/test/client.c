/* 	
Ryan "Bob" Dean
	CS 360 
	Assignment 8 - mftp Client
*/

#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <ctype.h>

#define PORTNO 49999

int connect4(char* serverID, int portno){
	struct sockaddr_in servAddr;
	struct hostent* hostEntry;
	struct in_addr **pptr;

	int socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketfd < 0) {
	    fprintf(stderr, "ERROR opening socket");
	    return -1;
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(portno);

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
		fprintf(stderr, "Connect Error: %s\n", strerror(errno));
		return -1;
	}
	return socketfd;
}

void ls(){
	int x = 1;
	if((x = fork())){
		wait(&x);
		printf("\n\n-------------------------------------\n\n");								
	} else {
		int fd[2];
		pipe(fd);
		int rdr = fd[0], wtr = fd[1];
		int status = 1;
		if((status = fork())){
			close(wtr);
			close(0); dup(rdr);
			close(rdr);
			wait(&status);
			execlp("more", "more", "-20", NULL);				
		} else {
			close(rdr);
			close(1); dup(wtr);
			close(wtr);
			execlp("ls", "ls", "-l", NULL); 
		}
	}
	return;
}

void cd(char* path){
	if(chdir(path) < 0) 
		printf("Error: path not found or not accessable.\n");
	return;
}

void rls(int datafd){
	int x;
	if((x = fork())){
		wait(&x);
	} else {
		close(0); dup(datafd);
		execlp("more", "more", "-20", NULL);		
	}
	return;
}

void getfile(int datafd, char* path){
	int fd;
	int bytes;
	char buf [512];
	if((fd = open(path, O_WRONLY | O_CREAT | O_EXCL, 0600)) < 0) {
		perror("Getfile Error");
		return;
	}
	while ((bytes = read(datafd, buf, 512)) > 0){
		write(fd, buf, bytes);
	}
	close(fd);
	return;
}

char* readM(int socketfd){
	char* buf = malloc(512);
	int i= 0;
	char b;
	read(socketfd, &b, 1);
	while (b != '\n'){
		buf[i] = b;
		i++;
		read(socketfd, &b, 1);
	}
	buf[i] = '\0';
	return buf;
}

int putfile(int datafd, int socketfd, char* command, char* path){
	int fd;
	int bytes;
	char* buf;
		if((fd = open(path, O_RDONLY, 0600)) < 0) {
			perror("Putfile Error");
			return 1;
		}
		write(socketfd, command, strlen(command));
		buf = readM(socketfd);
		if (buf[0] == 'E'){
			printf("%s\n", strcat(buf+1, "\0"));
			return 1;
		} else {
			while ((bytes = read(fd, buf, 512)) > 0){
				write(datafd, buf, bytes);
			}
		}
	close(fd);
	return 0;
}

int main(int argc, char** argv){

	if(!argv[1]){
		fprintf(stderr, "ERROR: Please enter server IP.\n");
		return -1;
	}

	char* serverID = argv[1];
	int socketfd = connect4(serverID, PORTNO);
	char* buf;
	char* command = NULL;
	char servCommand[512];
	char *path;
	char input[512];
	int datafd = 0;

	if (socketfd == -1) return -1;

	while(1){
		printf("Enter command: "); 
		fgets(input, 512, stdin);
		printf("\n");

		if (input[0] == '\n') continue;
		printf("%s", input);
		command = strtok(input, " \n\t\v\f\r");

		if (!strcmp(command, "exit") || !strcmp(command, "quit")) {
			write(socketfd, "Q\n", 2);
			buf = readM(socketfd);
			if(buf[0] == 'A'){
			printf("Disconnected from server normally.\n");
			return 0;
			};

		// CD // 
		} else if (!strcmp(command, "cd")){
			path = strtok(NULL, " \n\t\v\f\r");
			cd(path);
			continue; 


		// LS //
		} else if (!strcmp(command, "ls")){
			ls();
			continue;



		} else if (!strcmp(command, "rcd")){
			//printf("%s", path);
			path = strtok(NULL, " \n\t\v\f\r");
			strcpy(servCommand, "C");
			strcat(servCommand, path);
			strcat(servCommand, "\n");	
			printf("%s",servCommand);
			write(socketfd, servCommand, strlen(servCommand));
			buf = readM(socketfd);
			printf("%s", buf);
			if (buf[0] == 'E'){
				printf("%s\n", strcat(buf+1, "\0"));
			}
			continue;


		
		} else if (!strcmp(command, "rls")){
			write(socketfd, "D\n", 2);
			buf = readM(socketfd);
			if (buf[0] == 'E'){
				printf("%s\n", strcat(buf+1, "\0"));
			} else {
				datafd = connect4(serverID, atoi(buf+1));
				if (datafd == -1) return -1;
				write(socketfd, "L\n", 2);
				buf = readM(socketfd);
				if(buf[0] == 'A'){
					rls(datafd);
				} else printf("Error: %s\n", strcat(buf+1, "\0"));
			continue;
		}



		} else if (!strcmp(command, "get")){
			write(socketfd, "D\n", 2);
			buf = readM(socketfd);	
			if (buf[0] == 'E'){
				printf("%s\n", strcat(buf+1, "\0"));
			} else {
				datafd = connect4(serverID, atoi(buf+1));
				path = strtok(NULL, " \n\t\v\f\r");
				strcpy(servCommand, "G");
				strcat(servCommand, path);
				strcat(servCommand, "\n");	
				write(socketfd, servCommand, strlen(servCommand));
				buf = readM(socketfd);
				if(buf[0] == 'A'){
					getfile(datafd, path);
					close(datafd);
				} else printf("Error: %s\n", strtok(buf+1, "\n"));
			}
			continue;



		} else if (!strcmp(command, "show")){
			write(socketfd, "D\n", 2);
			buf = readM(socketfd);	
			if (buf[0] == 'E'){
				printf("%s\n", strcat(buf+1, "\0"));
			} else {
				datafd = connect4(serverID, atoi(buf+1));
				path = strtok(NULL, " \n\t\v\f\r");
				strcpy(servCommand, "G");
				strcat(servCommand, path);
				strcat(servCommand, "\n");	
				write(socketfd, servCommand, strlen(servCommand));				
				buf = readM(socketfd);			
				if(buf[0] == 'A'){
					getfile(datafd, path);
					rls(datafd);
				} else printf("Error: %s\n", strtok(buf+1, "\n"));
			}
			continue;



		} else if (!strcmp(command, "put")){
			printf("%s", buf+1);
			fflush(stdout);
			write(socketfd, "D\n", 2);
			printf("%s", buf+1);
			fflush(stdout);
			buf = readM(socketfd);	
			if (buf[0] == 'E'){
				printf("%s\n", strcat(buf+1, "\0"));
			} else {

				datafd = connect4(serverID, atoi(buf+1));
				path = strtok(NULL, " \n\t\v\f\r");
				strcpy(servCommand, "P");
				strcat(servCommand, path);
				strcat(servCommand, "\n");	
				putfile(datafd, socketfd, servCommand, path);
				close(datafd);
			}
			continue;



		} else {
			printf("Invalid option.\n");
			continue;
		}	
	}
	return 0;
}