#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

Stack *stack = NULL;    /* global symbol table */

/*
 * symLookup()
 * Looks for 'sym' in 'symtab' (case sensitive search) and
 * returns the corresponding symbol table entry if found.
 * If the symbol is not found, is creates a new symbol table
 * entry and inserts it into the table and return the new entry.
 */

void pushStack() {
  Stack* temp = malloc(sizeof(Stack));
  temp->symtab = NULL;
  temp->next = stack;
  stack = temp;
}

void popStack() {
  Stack* temp = stack;
  stack = stack->next;
  while(temp->symtab){
    SymTab* tempTab = temp->symtab->next;
    free(temp->symtab->sym);
    free(temp->symtab);
    temp->symtab = tempTab;
  }
  free(temp);
}

SymTab* symLookup(char* sym) {
  SymTab *s;
  Stack* temp;
  for (temp = stack; temp != NULL; temp = temp->next) { 
    for (s = temp->symtab; s != NULL; s = s->next) {
      if (strcmp(s->sym, sym) == 0) {
        return s;
      }
    } 
  }
  return NULL;
}

void symAdd(char* sym) {
  SymTab *s;
  s = (SymTab *) malloc(sizeof(SymTab));
  if (s == NULL) {
    perror("malloc()");
    exit(-1);
  }
  s->sym = strdup(sym);
  s->val = 0.0;
  s->next = stack->symtab;
  stack->symtab = s;
}