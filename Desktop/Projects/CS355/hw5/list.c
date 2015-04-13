/* 	Ryan "Bob" Dean
	CS 355 - Assignment 5
	3-SAT problem */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

List* createList(){
	List* newList = malloc(sizeof(List));
	newList->data = NULL;
	newList->next = NULL;
	return newList;
}

int isEmpty(List* l){
	if (l == NULL || l->data == NULL) return 1;
	else return 0;
}

List* prepend(List* l, void* value){
	if (isEmpty(l)){
		l->data = value;
		return l;
	} else {
		List* newList = createList();
		newList->data = value;
		newList->next = l;
		return newList;
	}
}

List* append(List* l, void* value){
	if (isEmpty(l)){
		l->data = value;
		return l;
	} else {
		List* newList = createList();
		newList->data = value;
		newList->next = NULL;
		List* temp = l;
		while (l->next){
			l = l->next;
		}
		l->next = newList;
		l = temp;
		return l;
	}
}

void* head(List* l){
	if (!isEmpty(l)) return l->data;
		printf("Error: list is empty, returning NULL.\n");
		return NULL;	
}

List* tail(List* l){
	if (!isEmpty(l) && l->next) { 
		return l->next;
	} else if (!isEmpty(l)) {
		return NULL;
	}
	printf("Error: List is empty. Returning empty list.\n");
	return l;
}

int length(List* l){
	if (isEmpty(l)) return 0;
	int length = 1;
	while(!isEmpty(l)){
		l = l->next;
		length++;
	}
	return length-1;
}

List* insert(List* l, void* value, int loc){
	List* temp = l;
	List* newList = createList();
	newList = append(newList, value);

	if (loc == 1){
		newList->next = l;
		return newList;
	}
	if (isEmpty(l)) {
		l = append(l, value);
		return l;
	}
	for (int i = loc - 2; i > 0; i--){
		if (!l->next) {
			printf("Error: Insert location out of bounds.\n");
			return temp;
		}
		l = l->next;
	}	
	if (l->next){
		newList->next = l->next;
		l->next = newList;
		l = temp;
		return l;
	} else {
		l->next = newList;
		return temp;
	}
}

List* removeFromList(List* l, int loc){
	List* temp = l;

	if (loc == 1){
		l = l->next;
		return l;	
	}
	if (isEmpty(l)) {
		printf("Error: Cannot remove from empty list.\n");
		return l;
	}
	for (int i = loc - 2; i > 0; i--){
		if (!l->next) {
			printf("Error: Removal location out of bounds. Returning initial list.\n");
			return temp;
		}
		l = l->next;
	}
	if(l->next){
		List* temp2 = l->next;
		l->next = temp2->next;
		free(temp2);
		return temp;
	}else {
		free(l);
		if (!isEmpty(temp)) return temp;
		else return createList();
	}
}	

void* retrieve(List* l, int loc){

	if (loc == 0){
		if (isEmpty(l)) {
			printf("Error: Cannot retrieve from empty list.\n");
			return NULL;
		} else {
			return l->data;
		}
	}
	for (int i = loc - 1; i > 0; i--){
		if (!l->next) {
			printf("Error: Retrieval location out of bounds.\n");
			return NULL;
		}
		l = l->next;
	}
		return l->data;
}