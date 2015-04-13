#ifndef PARSER_H
#define PARSER_H


enum{
    ASSIGN_STMT = 1, DECLARE_STMT,
    WHILE_STMT, IF_STMT, BLOCK_STMT, ACTION_STMT,
    NUM_EXPR, VAR_EXPR, ADD_EXPR, SUB_EXPR, MULT_EXPR, DIV_EXPR, NEG_EXPR, 
    GE_EXPR, LE_EXPR, NE_EXPR, EQ_EXPR, GT_EXPR, LT_EXPR, NOT_EXPR, OR_EXPR, AND_EXPR
};


typedef struct Expr {
    int type;  //NUM_EXPR, VAR_EXPR, ADD_EXPR,
    union {
        float num;      //NUM_EXPR
        char* sym;    //VAR_EXPR
        struct Expr *unary;  //unary operation (NEG_EXPR, ... );
        struct {struct Expr *left, *right;} binary;  //binary operation (ADD_EXPR,...);
    } op;
} Expr;

typedef struct Stmt {
    int type;  //WHILE_, IF_, ASSIGN_, BLOCK_
    union {
        struct {char* sym;} declare_;
        struct {char* lval; Expr *rval;} assign_;
        struct {Expr *cond; struct Stmt *body;} while_;
        struct {struct Stmt *list;} block_; //The next pointer is used for the block statements
        struct {Expr *cond; struct Stmt *body; struct Stmt *elseBody; /*struct Stmt *elseIfbody*/} if_;
        struct {int action; Expr *rval;} action_;
    } s;
    struct Stmt *next;
} Stmt;


// Prototype list
Stmt* stmt();
Expr* expr();
Expr* boolExpr();
Stmt* elseIf_stmt();
Stmt* declare_stmt();
void ExecDeclareStmt(Stmt* stmt);
Stmt* block();

#endif /* PARSER_H */