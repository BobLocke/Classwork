/* 	Ryan "Bob" Dean
	CS 355 - Assignment 5
	3-SAT problem */

typedef struct List {
	void* data;
	struct List* next;
	} List;

List* createList();

int isEmpty(List* l);

List* prepend(List* l, void* value);

List* append(List* l, void* value);

void* head(List* l);

List* tail(List* l);

int length(List* l);

List* insert(List* l, void* value, int loc);

List* removeFromList(List* l, int loc);

void* retrieve(List* l, int loc);

