#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

char buffer[100];
char input[100];
int fd[2];
int rdr, wtr, pid;
int delay;
int status;
char delaystr[100];
int offset;

void parentaction() {
	signal(SIGINT, SIG_IGN);
	kill(pid, SIGFPE);
	printf("\n\nWaiting for user input: ");
	fgets(input, 100, stdin);
	if (!strcmp(input, "exit\n")){
		write(wtr, input, strlen(input));
		kill(pid, SIGFPE);
		wait(&status);
		exit(0);
	}
	//printf("%s", input);
	write(wtr, input, strlen(input)+1);
	return;
}

void childaction(){
	signal(SIGALRM, SIG_IGN);
	read(rdr, buffer, 100); 
	//printf("child read string %s\n", buffer);
	if (!strcmp(buffer, "exit\n")){
		printf("Exit command recieved, closing...\n");
		exit (0);
	}
	if (isdigit(buffer[0])){
		for (offset = 0; isdigit(buffer[offset]); offset++){
			delaystr[offset] = buffer[offset];
		}

	} else {
		offset = 0;
		strcpy(delaystr, "5");	
	} 
	delay = atoi(delaystr);
    //printf("%s", buffer+i);
	//printf("child action!\n");
	return;
}

void printfunction(){
	return;
}

int main(int argc, char** argv){

	pipe(fd);
	rdr = fd[0];
	wtr = fd[1];

	if ((pid = fork())){
		close(rdr);		
		while (1){
			signal(SIGINT, parentaction);
			pause();
			//printf("Parent out of pause\n");	
		}
	} else {
		close(wtr);
		signal(SIGINT, SIG_IGN);;
		while (1) {
			signal(SIGFPE, childaction);
			signal(SIGALRM, printfunction);		
			alarm(delay);
			pause();
			printf("%s\n", buffer + offset);		
		}		
	}		
		//printf("%s", input);

		/* }
		else { //child
			signal (SIGINT, SIG_IGN);

		} */
	return 0;
}