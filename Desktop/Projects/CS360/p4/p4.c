#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char** argv){
	
	int j = 2;
	int fd[2];
	pipe(fd);
	int rdr = fd[0], wtr = fd[1];

	while (strcmp(argv[j], ":")){
		j++;
	} 
		argv[j] = NULL;

		if (fork()){
			close(wtr);
			close (0); dup (rdr); 
			execvp(argv[j+1], argv+j+1); 
			close (rdr);
		} else {
			close(rdr);
			close(1); dup(wtr); 
			execvp(argv[1], argv+1);
			close(wtr);			
		}

	return 0;
}