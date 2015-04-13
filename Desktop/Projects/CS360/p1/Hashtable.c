#include "HashTable.h"
#include <assert.h>
#include "/cs_Share/class/cs360/src/crc64.c"

Hashtable* createH(int initSize){

	Hashtable* newHashTable;
	newHashTable = malloc(sizeof(Hashtable));

	newHashTable->Nodes = calloc(initSize, sizeof(Node*));
	newHashTable->numBuckets = initSize;
	newHashTable->numNodes = 0;

	return newHashTable;
}

Node* createNode(char* key, void* value){
	Node* newNode;
	newNode = calloc(1, sizeof(Node));
	newNode->key = key;
	newNode->value = value;
	newNode->next = NULL;
	return newNode;
}

void destroyNode(Node* Node){
	if (Node->next){
		destroyNode(Node->next);
	}
	free(Node->value);
	// free(Node->count);
	free(Node);
}

void destroyH(Hashtable* hashtable){
	for (int i = 0; i <= hashtable->numBuckets; i++){
		if (hashtable->Nodes[i]){
			destroyNode(hashtable->Nodes[i]);
		}
	}
	free(hashtable->Nodes);
	free(hashtable);
}

/* If, after insertion, the number of Nodes (stored values) is
greater than a third of the total number of buckets in the hashtable, 
it creates a new hashtable at triple the size of the old one.*/

void rehash(Hashtable** myHash){
	Node* tempNode;
	Node* current;
	Hashtable* newHashTable = createH((*myHash)->numBuckets * 3);
	for(int i = 0; i < (*myHash)->numBuckets; i++){
		current = (*myHash)->Nodes[i];
		while (current) {
			tempNode = current->next;
			unsigned long long hashedKey = crc64(current->key) % newHashTable->numBuckets;
			current->next = newHashTable->Nodes[hashedKey];
			newHashTable->Nodes[hashedKey] = current;			 
			current = tempNode;
		}
	}
	int temp = (*myHash)->numBuckets * 3;
	int temp2 =(*myHash)->numNodes; 
	free((*myHash)->Nodes);
	*myHash = newHashTable;
	(*myHash)->numBuckets = temp;
	(*myHash)->numNodes = temp2;

	return;
}

/* 
The insert function should only run if a 
Node for the new Value doesn't exist. 
For already existing Nodes, key modifications should 
be handled by the external functions.
*/

void insert(Hashtable** myHash, char* newKey, void* newValue){

	Node* current;
	unsigned long long hashedKey = crc64(newKey) % (*myHash)->numBuckets;
	current = search(*myHash, newKey);
	assert(current == NULL);
	current = createNode(newKey, newValue);
	current->next = (*myHash)->Nodes[hashedKey];
	(*myHash)->Nodes[hashedKey] = current;
	(*myHash)->numNodes++;

	//rehash if table is filling up 
	if ((*myHash)->numNodes > ((*myHash)->numBuckets/3)){
		rehash(myHash);
	}
	return;
} 

/*Returns NULL if the key does not already exist within the Hashtable.
Otherwise, returns a pointer to the Node containing the key. */

void* search(Hashtable* myHash, char* newKey){
	unsigned long long key = crc64(newKey) % myHash->numBuckets;
	if (!myHash->Nodes[key]){
		return NULL;
	}
	Node* current;
	current = myHash->Nodes[key];
	while (current) {
		if (!strcmp(current->key, newKey)){
			return current->value;
		}
		current = current->next;
	}
	return NULL;
}


/* This function displays all key/value 
pairs in the Hashtable, for debugging purposes */

void displayH(Hashtable* myHash){
	Node* current;
	int count = 0;
	for(int i = 0; i <= myHash->numBuckets; i++){
		current = myHash->Nodes[i];
		if (current){
			count++;
			printf("%d key: %s, value: %d \n", count, current->key, *(int*) current->value);
			while (current->next) {
				current = current->next;
				count++;
			printf("%d key: %s, value: %d \n", count, current->key, *(int*) current->value);			
			}
		}
	}
	printf("Number of Nodes = %d, numNodes = %d", count, myHash->numNodes);
}


int compare(const void *a, const void *b){
	KV* node1 = (KV*) a;
	KV* node2 = (KV*) b;
	int val1 = node1->value;
	int val2 = node2->value;
    //printf("Stop! \n");
	if(val1 < val2){
		return 1;
	}else if(val1 > val2){
		return -1;
	}
	return 0;
}

/*Creates and sorts populated table by value in descending order */

KV* sort(Hashtable* myHash){
	// create/allocate new sorted structure of key/value pairs
	KV* sorted = calloc(myHash->numNodes, sizeof(KV));
	Node* current;
	int count = 0;
	for(int i = 0; i < myHash->numBuckets; i++){
		current = myHash->Nodes[i];
		while (current) {
			sorted[count].key = current->key;
			sorted[count].value = *(int*) current->value;
			count++;
			current = current->next;
		}
		assert(count <= myHash->numNodes);
	}
	qsort(sorted, myHash->numNodes, sizeof(KV), compare);
	return sorted;		
}


void displayKV(KV* kv, Hashtable* myHash, int count){
	if((count < 1) | (count > myHash->numNodes)){
		count = myHash->numNodes;
		printf("\nNo count specified. Displaying all Keys.\n");
	}
		printf("\nDisplaying the first %d keys, sorted by value.\n---------------------------------------------------\n\n", count);
	for(int i = 0; i < count; i++){
		printf("WordPair: %s | Occurances: %d \n", kv[i].key, kv[i].value);
	}
}