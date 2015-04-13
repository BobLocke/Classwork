#include "sort.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <getWord.h>
#include <assert.h>

#define SORT_THRESHOLD      40

typedef struct _sortParams {
	char** array;
	int left;
	int right;
} SortParams;

static int maximumThreads;
pthread_mutex_t CT; 
int currentThreads = 0;
//TEST ARRAY
//char* sortable[1000000];

	int isSorted(char** array, int wordcount){
		for (int i = 1; i <= wordcount; i++){
			if (strcmp(array[i-1], array[i]) > 0)
				return 0;
		}
		return 1;
	}

/* This is an implementation of insert sort, which although it is */
/* n-squared, is faster at sorting short lists than quick sort,   */
/* due to its lack of recursive procedure call overhead.          */

static void insertSort(char** array, int left, int right) {
	int i, j;
	for (i = left + 1; i <= right; i++) {
		char* pivot = array[i];			
		j = i - 1;
		while (j >= left && (strcmp(array[j],pivot) > 0)) {
			array[j + 1] = array[j];
			j--;
		}
		array[j + 1] = pivot;
	}
}

/* Recursive quick sort, but with a provision to use */
/* insert sort when the range gets small.            */

static void quickSort(void* p) {
	SortParams* params = (SortParams*) p;
	char** array = params->array;
	int left = params->left;
	int right = params->right;
	int i = left, j = right;
	
	if (j - i > SORT_THRESHOLD) {			/* if the sort range is substantial, use quick sort */

		int m = (i + j) >> 1;				/* pick pivot as median of         */
		char* temp, *pivot;					/* first, last and middle elements */
		if (strcmp(array[i],array[m]) > 0) {
			temp = array[i]; array[i] = array[m]; array[m] = temp;
		}
		if (strcmp(array[m],array[j]) > 0) {
			temp = array[m]; array[m] = array[j]; array[j] = temp;
			if (strcmp(array[i],array[m]) > 0) {
				temp = array[i]; array[i] = array[m]; array[m] = temp;
			}
		}
		pivot = array[m];

		for (;;) {
			while (strcmp(array[i],pivot) < 0) i++;	/* move i down to first element greater than or equal to pivot */
			while (strcmp(array[j],pivot) > 0) j--;	/* move j up to first element less than or equal to pivot      */
			if (i < j) {
				char* temp = array[i];		/* if i and j have not passed each other */
				array[i++] = array[j];		/* swap their respective elements and    */
				array[j--] = temp;			/* advance both i and j                  */
			} else if (i == j) {
				i++; j--;
			} else break;					/* if i > j, this partitioning is done  */
		}

		

		
	SortParams first;  first.array = array; first.left = left; first.right = j;
    					/* sort the left partition	*/

	SortParams second; second.array = array; second.left = i; second.right = right;
			/* sort the right partition */

		pthread_mutex_lock(&CT);
		if(currentThreads < maximumThreads) {
			currentThreads++;
			pthread_mutex_unlock(&CT);
			pthread_t firstT; 

			pthread_create(&firstT, NULL, (void *) quickSort, (void *) &first);
			quickSort(&second);
			
			pthread_join(firstT, NULL);

			pthread_mutex_lock(&CT);
			currentThreads--;
			pthread_mutex_unlock(&CT);

		} else { 
			pthread_mutex_unlock(&CT);
			quickSort(&first);
			quickSort(&second);		
	}
				
	} else insertSort(array,i,j);			/* for a small range use insert sort */
}

/* user interface routine to set the number of threads sortT is permitted to use */

void setSortThreads(int count) {
    maximumThreads = count;
    pthread_mutex_init(&CT, NULL);
}

/* user callable sort procedure, sorts array of count strings, beginning at address array */

void sortThreaded(char** array, unsigned int count) {
	SortParams parameters;
    parameters.array = array; parameters.left = 0; parameters.right = count - 1;
	quickSort(&parameters);
}


//INSERT MAIN HERE






//TEST MAIN

/*
int main(int argc, char* argv[]) {
	int wordcount;


	setSortThreads(8);

	if (!argv[1]) {
		fprintf(stderr, "Please include a filename as an argument.\n");
		return -1;
	}
	FILE *fp;
	fp = fopen(argv[1], "r");
	if (!fp){
		fprintf(stderr, "Error: File not found.\n");
	return -1; 
	}
	wordcount = 0;
	while(!feof(fp)){ 
		sortable[wordcount] = getNextWord(fp);	
		wordcount++;
	}
	wordcount--;
	fclose(fp);	

	/*for (int i = 0; i < wordcount; i++){
			printf("%s | ", sortable[i]);
		} 

	printf("\n\n\n");

	SortParams* test = (SortParams*) malloc(sizeof(SortParams));
	test->array = sortable;
	test->left = 0;
	test->right = wordcount;

	quickSort(test);

	if (isSorted(sortable, wordcount)){
		printf("Sorting Successful!\n");
	} else {
		printf("Sorting Failed!\n");
	}


	return 0;
	}
*/

