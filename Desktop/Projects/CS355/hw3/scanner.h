#ifndef SCANNER_H
#define SCANNER_H

/* this enum holds all token types */
enum {   /* non-single char tokens */
  IDENT_ = 256,
  ASSIGN_, VAR_,
  REAL_,
  NE_, /* 260*/ LE_, GE_,
  OR_, AND_, NOT_,
  /* 265 */ WHILE_, DO_, OD_,
  IF_, THEN_, /* 270 */ ELSIF_, ELSE_, FI_,
  HOME_, PENUP_,  /* 275 */ PENDOWN_, FORWARD_, RIGHT_, LEFT_, PUSHSTATE_,  /* 280 */ POPSTATE_
};

/* LVAL can either be a real number or an identifier */
typedef union {     /* lexeme associated with certain tokens */
  float f;          /* REAL_ */
  char *s;          /* IDENT_ */
} LVAL;

extern int lineno;  /* current source code line number */

/*
 * Returns the next token/lexeme read from stdin.
 * Returns 0 when there are no more tokens.
 */
int nextToken(LVAL *lval);

#endif /* SCANNER_H */
