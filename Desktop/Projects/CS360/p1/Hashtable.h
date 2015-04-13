#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "/cs_Share/class/cs360/src/crc64.c"

typedef struct Node {
	char* key;
	void* value;
	struct Node* next;
} Node;

typedef struct Hashtable {
	Node** Nodes;
	int numBuckets; // number of buckets
	int numNodes; // number of unique values	
} Hashtable; 

typedef struct KV {
	int value;
	char* key;
} KV;

void insert(Hashtable** myHash, char* newKey, void* newValue);

Hashtable* createH(int);

void destroyH(Hashtable*);

void* search(Hashtable*, char* newKey);

void displayH(Hashtable*);

void displayKV(KV*, Hashtable*, int);

KV* sort(Hashtable*);