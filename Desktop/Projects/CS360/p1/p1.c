#include <stdio.h>
#include <stdlib.h>
#include <getWord.h>
#include <string.h>
#include <assert.h>
#include "Hashtable.h"
#define INIT_SIZE 100

/* increments value if the word pair is already in the table, otherwise creates a new Node */
void insertWordPair(Hashtable** myHash, char* newWordPair){
	Node* exists;
	exists = search(*myHash, newWordPair);

	if (exists) {
		int *iptr = (int*) (exists);
		(*iptr)++;
		return;
	} else { 
		void* value = (void*) malloc(sizeof(int));
		*((int*)value) = 1;
		insert(myHash, newWordPair, value);
		return;
	}
}	

/* formatting text for human readability "word word"*/
void assemble(char* word1, char* word2, char* theWord) {
	strcpy(theWord, word1);
	strcat(theWord, " ");
	strcat(theWord, word2); 
}

int main(int argc, char* argv[]){


	char* wordPair[2];
	char* newword;
	Hashtable* myHash;
	myHash = createH(INIT_SIZE);
	int arg = 1;
	int count = 0;

	if (!argv[arg]){
		fprintf(stderr, "Error: This program requires command line arguments. \nPlease retry in the form Wordpairs <-count> filename1 <filename2> ... <filenameN>.\n");
		return -1; 
	}

	if (argv[arg] && *argv[arg] == '-') {
		/* strip out count */
		/* set count */
		count = atoi(argv[arg]) * -1;
		arg++;
	}

	if (!argv[arg]){
		fprintf(stderr, "Error: No files specified.\nPlease retry in the form Wordpairs <-count> filename1 <filename2> ... <filenameN>.\n");
		return -1; 
	}

	while (argv[arg]) {
		FILE *fp;
		fp = fopen(argv[arg], "r");
		if (!fp){
			fprintf(stderr, "Error: File not found.\n");
		return -1; 
		}
		/* insert first word pair into table */
		wordPair[0] = getNextWord(fp);
		//printf("%s \n", wordPair[0]);
		wordPair[1] = getNextWord(fp);
		//printf("%s \n", wordPair[1]);
		newword = calloc(strlen(wordPair[0]) + strlen(wordPair[1]) + 2, sizeof(char));
		assemble(wordPair[0], wordPair[1], newword);
		//printf("%s \n", newword);
		insertWordPair(&myHash, newword);
	
		/* read all the words from the file*/
		while (!feof(fp)){
			free(wordPair[0]);
			wordPair[0] = wordPair[1];
			wordPair[1] = getNextWord(fp);
			if (!wordPair[1]){
				break;
			}
			newword = calloc(strlen(wordPair[0]) + strlen(wordPair[1]) + 2, sizeof(char));
			assemble(wordPair[0], wordPair[1], newword);
			//printf("%s \n", newword);
			insertWordPair(&myHash, newword);
		}
		/* close the file and move on to the next*/
		fclose(fp);
		arg++;
	}
	KV* sortedbykey = sort(myHash);	
	displayKV(sortedbykey, myHash, count);	

	//display(myHash);
	destroyH(myHash);	
	return 0;
}
