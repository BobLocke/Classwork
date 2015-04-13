#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

int main(){

	List* testList = createList();

	//createList tests

	assert (isEmpty(testList));
	assert (!head(testList));
	assert (testList = tail(testList));
	assert (!length(testList));
	assert (!retrieve(testList, 2));

	//append construction [a, b]

	append(testList, "a");
	append(testList, "b");
	List* appendTest = createList();
	append(appendTest, "b");

	// append tests

	assert(!isEmpty(testList));
	assert(!strcmp(head(testList), "a"));
	assert(tail(testList)->data == appendTest->data);
	assert(length(testList) == 2);
	assert(!strcmp(retrieve(testList, 1), "a")); 

	free(appendTest);
	free(testList);

	// prepend construction [1, 2]

	List* prependList = createList();
	List* pList = createList();
	prependList = prepend(prependList, "2");
	prependList = prepend(prependList, "1");
	pList = prepend(pList, "2");

	// prepend tests

	assert(!isEmpty(prependList));
	assert(!strcmp(head(prependList), "1"));
	assert(tail(prependList)->data == pList->data);
	assert(tail(prependList)->next == pList->next);
	assert(length(prependList) == 2);
	assert(!strcmp(retrieve(prependList, 2), "2")); 
	assert(!strcmp(retrieve(prependList, 1), "1")); 

	free(prependList);
	free(pList);

	// insert construction [a, 2, z]

	List* insList = createList();
	List* iList = createList();
	insList = insert(insList, "z", 1);
	insList = insert(insList, "a", 1);
	insList = insert(insList, "2", 2);
	iList = insert(iList, "z", 1);
	iList = insert(iList, "2", 1);

	//insert tests

	assert(!isEmpty(insList));
	assert(!strcmp(head(insList), "a"));
	assert(tail(insList)->data == iList->data);
	assert(tail(insList)->next->data == iList->next->data);
	assert(length(insList) == 3);
	assert(!strcmp(retrieve(insList, 2), "2")); 
	assert(!strcmp(retrieve(insList, 1), "a")); 

	free(iList);

	// remove tests [2] (removing a and z from insList)

	insList = removeFromList(insList, 1);
	insList = removeFromList(insList, 2);

	assert(!isEmpty(insList));
	assert(!strcmp(head(insList), "2"));
	assert(tail(insList)->data == NULL); // tail of insList is empty
	assert(length(insList) == 1);
	assert(!strcmp(retrieve(insList, 1), "2")); 

	printf("All axioms passed!\n");

	return 0;
}