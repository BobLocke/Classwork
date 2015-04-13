/*	Ryan "Bob" Dean
	Assignment 2
	CS 360	*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#define WORDLEN	16

int checkDict(char* userWord, int fp){
	char checkWord[WORDLEN];
	int bottom = 0;
	int top = lseek(fp, -WORDLEN, SEEK_END) / 16;
	int midpoint = (top + bottom) / 2;
	lseek(fp, midpoint * 16, SEEK_SET);
	while (top >= bottom){
		read (fp, checkWord, 16);
		checkWord[15] = '\0';
		int wordMatch = strcmp(userWord, checkWord);
		if (wordMatch == 0){
			return 0;
		}
		else if (wordMatch < 0){
			top = midpoint-1;
		}
		else {
			bottom = midpoint+1;
		}
		midpoint = (top + bottom) / 2;
		lseek(fp, midpoint*16, SEEK_SET);	
	}
		return 1;
}

int main(int argc, char** argv){
	char userWord[WORDLEN];

	if(argc != 2){
		fprintf(stderr, "Incorrect input: Please enter one word to search for.\n");
		return -1;
	}	

	strcpy(userWord, argv[1]);
	for(int i = strlen(userWord); i < WORDLEN - 1; i++){
			userWord[i] = ' ';
	}
	userWord[WORDLEN-1] = '\0';

	int fp;
	fp = open("/cs_Share/class/CS360/lib/webster", O_RDONLY, 0);
	if (fp < 0){
		fprintf(stderr, "File not found or is empty.\n");
		return -1;
	}
	int result = checkDict(userWord, fp);
	close(fp); 
	if (result == 0){
		printf("ok!\n");
	}
	else printf("not ok!\n");
	return 0;

}

