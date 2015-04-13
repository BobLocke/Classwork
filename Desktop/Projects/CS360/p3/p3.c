#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>

//struct DIR;


int isDirectory (char *pathname) {

struct stat area, *s = &area;

return (stat (pathname, s) == 0) &&
    S_ISDIR (s->st_mode);

}

int isRegular (char *pathname) {

struct stat area, *s = &area;

return (stat (pathname, s) == 0) &&
    S_ISREG (s->st_mode);

}

void readable(char* path){
	if ((access(path, R_OK))){return;}
	DIR* current;
	current = opendir(path);	
	char* nextPath;
	struct dirent *d;
	d = readdir(current);

		
		while(d){	

			if(!(strcmp(d->d_name, ".")) || !(strcmp(d->d_name, ".."))){
				d = readdir(current);
				continue;
			}

			nextPath = calloc(4096, sizeof(char));
			strcpy(nextPath, path);
			strcat(nextPath, "/");
			strcat(nextPath, d->d_name);
			if ((access(nextPath, R_OK))){
				d = readdir(current);
				continue;
			}

				if(isDirectory(nextPath)){
					//printf("%s/%s is a directory \n", path, d->d_name);
					readable(nextPath);
					d = readdir(current);
					continue;

				}else if(isRegular(nextPath)){
					if (access(d->d_name, R_OK));
					printf("%s/%s is readable\n", path, d->d_name);
				}
			
			d = readdir(current);
			free(nextPath);
		}	
	return;
}

int main(int argc, char** argv){

	if(argc > 2){
		fprintf(stderr, "Too many command line arguments, expected 0..1");
		return -1;
	}

	char* path;
	path = calloc(4096, sizeof(char));

	if (argc == 2){		
		path = argv[1];
	} else {
		getcwd(path, 4096);
	}

	readable(path);

	//printf("%s \n", path);
	//if (isDirectory(path)){
	//	printf("Is a directory!");
	//}

	return 0;
}