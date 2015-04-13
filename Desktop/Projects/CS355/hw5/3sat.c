#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "list.h"
//#include <stdbool.h>
#include <assert.h>
#include "getword.h"
#include <assert.h>

typedef struct state {
	char* name;
	char* status;
} state;

int evalVar(List* l, char* var);

//
//
// PRINT FUNCTIONS
//
//

void StatesToString(List* l){
	while(1){
	state* pulled = (state*) head(l);		
	if (isEmpty(tail(l))) {
		printf("%s: %s\n", pulled->name, pulled->status);
		return;
	} else printf("%s: %s, ", pulled->name, pulled->status);
	l = tail(l);
	}
}

void VarsToString(List* l){
	while(1){
	char* pulled = (char*) head(l);		
	if (isEmpty(tail(l))) {
		printf("%s\n", pulled);
		return;
	} else printf("%s, ", pulled);
	fflush(stdout);
	l = tail(l);
	}
}

void ClauseToString(List* l){
	while(!isEmpty(l)){
		for (int i = 1; i <= 3; i++) {
		state* pulled = (state*) head(l);
			if(pulled->status) printf("(%s %s) ", pulled->status, pulled->name);
			else printf("%s ", pulled->name);
			l = tail(l);
		}	
		printf("\n");		
	}
	return;
}

void ClauselistToString(List* l){
	printf("Clauses:\n");
	while(!isEmpty(l)){
		ClauseToString(head(l));
		l = tail(l);
	}
	return;
}

//
//
// HELPER FUNCTIONS
//
//

List* flatten(List* l){
	List* list = createList();
	char* var;
	while(!isEmpty(l)){
		var = (char*) head(l);
		if (!isEmpty(list)) {
			List* temp = list;
			while(!isEmpty(temp)){
				if (strcmp(head(temp), var) != 0){
					temp = tail(temp);
					if (isEmpty(temp)) append(list, var);
				} else break;
			}

		} else append(list, var); 
		l = tail(l);
	}
	return list;
}

int findVar(List* states, char* var){
	state* pulled;
	int count = 1;
	while(!isEmpty(states)){
		pulled = (state*) head(states);
		if (strcmp(pulled->name, var) != 0){
			states = tail(states);
			count++;
		} else break;
	}
	return count;
}

//
//
// 3SAT FUNCTIONS
//
//

int evalVar(List* l, char* var){
	state* pulled = (state*) head(l);
	if (isEmpty(l)){
		printf("Error: %s is not in list, returning -1\n", var);
		return -1;
	} else {
		if (strcmp(pulled->name, var) == 0) {
			if (strcmp(pulled->status, "t") == 0) return 1;
			else return 0; 
		} else return evalVar(tail(l), var);
	}	
}

int evalClause(List* clause, List* states){
	// DEBUG if (!(isEmpty(tail(tail(clause))))) ClauseToString(clause);
	state* clauseVar;
	if(!isEmpty(clause)){
		clauseVar = head(clause);
		if (((!clauseVar->status) && evalVar(states, clauseVar->name)) || ((clauseVar->status) && !evalVar(states, clauseVar->name))) return 1;
		else return evalClause(tail(clause), states);
	} else return 0;
}

List* getVars(List* clause) {
	List* vars = createList();
	state* clauseVar;
	while(!isEmpty(clause)){
		clauseVar = head(clause);
		append(vars, clauseVar->name);
		clause = tail(clause);
	}
	return vars;
}

List* getAllVars(List* clauses){
	List* vars = createList();
	List* temp = createList();
	while (!isEmpty(clauses)) {
		temp = getVars(head(clauses));
		while (!isEmpty(temp)){
		append(vars, head(temp));
		temp = tail(temp);
		}
		clauses = tail(clauses);
	}
	vars = flatten(vars);
	return vars;
	}

int unsatClauses(List* clauseList, List* stateList){
	int unsat = 0;
	while(!isEmpty(clauseList)){		
		if (!evalClause(head(clauseList), stateList)) unsat++;
		clauseList = tail(clauseList);
	}	
	return unsat;
}

List* flipVar(List* states, char* var){
	state* s = malloc(sizeof(state));
	List* temp = states;
	if(evalVar(states, var) == 1){
		s->name = var;
		s->status = "f";	
	} else if (evalVar(states, var) == 0){
		s->name = var;
		s->status = "t";		
	} else {
		printf("Error: %s is not in list, returning original list\n", var);
		return states;
	}
	int count = findVar(states, var);
	temp = removeFromList(temp, count);
	temp = insert(temp, s, count);
	return temp;
}

List* getBetterNeighbor(List* clauses, List* states){
	int numUnsat = unsatClauses(clauses, states);
	List* vars = getAllVars(clauses);
	char* var;
	while (!isEmpty(vars)){
		var = (char*) head(vars);
		if (unsatClauses(clauses, flipVar(states, var)) < numUnsat) return states;
		else {
			flipVar(states, var);
			vars = tail(vars);
		}
	}
	return 0;
}

List* simpleHillClimb(List* clauses, List* states, int limit){

	while (limit){
		if(getBetterNeighbor(clauses, states)){
			states = getBetterNeighbor(clauses, states);
		} else return states;
		limit--;
	}
	return states;
}


int main(int argc, char** argv){

	if (argc < 3){
		printf("Arguments Expected. Include files containing states and clauses to run 3sat.\n");
		exit(-1);
	} 


	List* stateList = createList();
	char* current;

	FILE *states;
	states = fopen(argv[1], "r");
	if (!states){
		fprintf(stderr, "Error: File not found.\n");
	return -1; 
	}

	while (!feof(states)){
		state* newState = malloc(sizeof(state));
		char* name = getNextWord(states);
		char* status = getNextWord(states);
		newState->name = name;
		newState->status = status;
		append(stateList, newState);
	}
	fclose(states);
	
	//toString(stateList);
	
	FILE *clauses;
	clauses = fopen(argv[2], "r");
	if (!clauses){
		fprintf(stderr, "Error: File not found.\n");
		return -1; 
	}

	List* clauseList = createList();
	current = getNextWord(clauses);
	while (current){	
		List* clause = createList();	
		for (int i = 1; i <= 3; i++) {
			state* element = malloc(sizeof(state));
			if (strcmp(current, "not") == 0){
				current = getNextWord(clauses);
				char* name = current;
				char* status = "NOT";
				element->name = name;
				element->status = status;
			} else {
				char* name = current;
				char* status = NULL;
				element->name = name;
				element->status = status;
			}
			current = getNextWord(clauses);
			append(clause, element);
		}
		append(clauseList, clause);
	}
	fclose(clauses);



	//StatesToString(stateList);
	//ClauselistToString(clauseList);


	// prints 1 if g is true, 0 if false
	printf("evalVar: %i\n", evalVar(stateList, "g"));


	/// prints 1 if first clause in list passes, 0 if unsatisfied

	printf("evalClause: %i\n", evalClause(head(clauseList), stateList));

	// prints number of unsatisfied clauses

	printf("unsatClauses: %i\n", unsatClauses(clauseList, stateList));

	printf("getVars: ");
	VarsToString(getVars(head(clauseList)));

	printf("getAllVars: ");
	VarsToString(getAllVars(clauseList));

	// flipVar needs to be called twice

	printf("stateList before flipVar(j): ");
	StatesToString(stateList);
	printf("stateList after flipVar(j): ");
	StatesToString(flipVar(stateList, "j"));
	flipVar(stateList, "j");

	printf("Finding better neighbor...   ");
	StatesToString(getBetterNeighbor(clauseList, stateList));
	printf("unsatClauses after getBetterNeighbor: %i\n", unsatClauses(clauseList, stateList));
	flipVar(stateList, "h");


	printf("Performing simpleHillClimb: ");
	StatesToString(simpleHillClimb(clauseList, stateList, 50));
	printf("unsatClauses after simpleHillClimb: %i\n", unsatClauses(clauseList, stateList));
	
	return 0;
	
}