#ifndef SYMTAB_H
#define SYMTAB_H

typedef struct SymTab {
  char *sym;
  float val;
  struct SymTab *next;
} SymTab;

typedef struct Stack {
	struct SymTab* symtab;
	struct Stack* next;  
} Stack;

//extern SymTab *symtab;  /* global symbol table */

void pushStack();

void popStack();

SymTab* symLookup(char *sym);

void symAdd(char *sym);

#endif /* SYMTAB_H */
