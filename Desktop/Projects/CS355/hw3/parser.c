/* Ryan "Bob" Dean */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"
#include "symtab.h"
#include "turtle.h"
#include "parser.h"

// lookahead keeps track of the current symbol taken by the scanner. lval is the value 
//SymTab *symtab = NULL;
int lookahead = 0;
LVAL *lval;

/**
 *  * Executes an action statement from the turtle API.
 *   */

float evalExpr( Expr *expr )
{
    switch(expr->type)
    {
        case NUM_EXPR:
            return expr->op.num;
        case VAR_EXPR:
           // printf("%f\n", expr->op.sym->val);
            return symLookup(expr->op.sym)->val;                                                                   /// SYMBOL STUFF
        case ADD_EXPR:
            return (evalExpr(expr->op.binary.left) + evalExpr(expr->op.binary.right));
        case SUB_EXPR:
            return (evalExpr(expr->op.binary.left) - evalExpr(expr->op.binary.right));
        case MULT_EXPR:
            return (evalExpr(expr->op.binary.left) * evalExpr(expr->op.binary.right));
        case DIV_EXPR:
            return (evalExpr(expr->op.binary.left) / evalExpr(expr->op.binary.right));
        case NEG_EXPR:
            return (-1 * evalExpr(expr->op.unary));
        case NE_EXPR:
            return (evalExpr(expr->op.binary.left) != evalExpr(expr->op.binary.right));
        case GE_EXPR:
            return (evalExpr(expr->op.binary.left) >= evalExpr(expr->op.binary.right));
        case LE_EXPR:
            return (evalExpr(expr->op.binary.left) <= evalExpr(expr->op.binary.right));
        case GT_EXPR:
            return (evalExpr(expr->op.binary.left) > evalExpr(expr->op.binary.right));
        case LT_EXPR:
            return (evalExpr(expr->op.binary.left) < evalExpr(expr->op.binary.right));
        case EQ_EXPR:
            return (evalExpr(expr->op.binary.left) == evalExpr(expr->op.binary.right));     
        case OR_EXPR:
            return (evalExpr(expr->op.binary.left) || evalExpr(expr->op.binary.right));
        case AND_EXPR:
            return (evalExpr(expr->op.binary.left) && evalExpr(expr->op.binary.right));
        case NOT_EXPR:
            return (!evalExpr(expr->op.unary));  
        default:  
            return 0;
    }
}

void execActionStmt(Stmt *stmt)
{
    //printf("excecute action stmt \n");
    int action = stmt->s.action_.action;
    float param;
    switch(action){
        case FORWARD_:
            param = evalExpr(stmt->s.action_.rval);
            turtleForward(param);
            break;
         case LEFT_:
            param = evalExpr(stmt->s.action_.rval);
            turtleRotate(param);
            break;
        case RIGHT_:
            param = evalExpr(stmt->s.action_.rval);
            turtleRotate(-param);
            break;
        case PENUP_:
            turtlePenUp(1);
            break;
        case PENDOWN_:
            turtlePenUp(0);
            break;
        case PUSHSTATE_:
            turtlePushState();
            break;
        case POPSTATE_:
            turtlePopState();
            break;
        case HOME_:
            turtleHome();
            break;       
        default:
            break;
    }
    //execute action statement by calling turtle API
}

void executeStmt(Stmt *stmt_)
{
    //printf("execute stmt %d\n", stmt_->type);

    Stmt *temp;
    switch(stmt_->type){
        case ACTION_STMT:
            execActionStmt(stmt_);
            break;
        case IF_STMT:
            
            if(evalExpr(stmt_->s.if_.cond) != 0){
                pushStack();
                executeStmt(stmt_->s.if_.body);
                popStack();
            } else if(stmt_->s.if_.elseBody){
                pushStack();
                executeStmt(stmt_->s.if_.elseBody);
                popStack();
            }           
            break;
        case WHILE_STMT:
            while(evalExpr(stmt_->s.while_.cond) != 0){
            pushStack();
            executeStmt(stmt_->s.while_.body);
            popStack();
            }
        
            break;
        case BLOCK_STMT:
            temp = stmt_->s.block_.list;
            while(temp){
                executeStmt(temp);
                temp = temp->next;
            }
            break;           
        case ASSIGN_STMT:
            symLookup(stmt_->s.assign_.lval)->val = evalExpr(stmt_->s.assign_.rval);
            break;
        case DECLARE_STMT:
            ExecDeclareStmt(stmt_);
            break;
        default:
            break; 
    }
    // freeStmt(stmt_);
    //execute some logics or call turtle API

}

Stmt* createActionStmt(int action, Expr *rval){

    //printf("create action stmt\n");

    Stmt *s = (Stmt*) malloc(sizeof(Stmt));
    s->type = ACTION_STMT;
    s->s.action_.action = action;
    s->s.action_.rval = rval;
    return s;
}

/* enforce match of current token, fetch next token*/
void match(int tok)
{
    if(tok == lookahead)
        lookahead = nextToken(lval);
    else
        printf("token matching error \n Token: %i, Expected: %i", lookahead, tok);
}

Expr* createBinaryExpression(int type, Expr* left, Expr* right) {

        Expr* e = malloc(sizeof(Expr));
        e->type = type;
        e->op.binary.left = left;
        e->op.binary.right = right;
        return e;
}

Expr* factor(){

    Expr *e;
    e = (Expr*) malloc(sizeof(Expr));
        switch(lookahead){
            case REAL_:
                e->type = NUM_EXPR;
                e->op.num = lval->f;
                lookahead = nextToken(lval);
                break;      
            case '+':
                match('+');
                e = factor();
                break;
            case '-':
                match('-');
                e->type = NEG_EXPR;
                e->op.unary = factor();
                break;
            case IDENT_:
                e->type = VAR_EXPR;
                e->op.sym = lval->s;
                lookahead = nextToken(lval);
                break;
            case '(':
                match('(');
                e = expr();
                match(')');
                break;
                       
        }   
    return e;
}

Expr* term(){
    Expr* e = factor();
    while (1){
        if(lookahead == '*'){ 
            match('*');
            e = createBinaryExpression(MULT_EXPR, e, term());
        } else if (lookahead ==  '/'){
            match('/');
            e = createBinaryExpression(DIV_EXPR, e, term());
        } else break;          
    }
    return e;    
}

Expr* expr(){
    Expr* e = term();
    while (1){
        if(lookahead == '+'){ 
            match('+');
            e = createBinaryExpression(ADD_EXPR, e, term());
        } else if (lookahead ==  '-'){
            match('-');
            e = createBinaryExpression(SUB_EXPR, e, term());
        } else break;          
    }
    return e;
}


Expr* compare(){
    Expr *b = expr();
    switch(lookahead){
        case GE_:
            match(GE_); 
            b = createBinaryExpression(GE_EXPR, b, expr());
            break;
        case LE_:
            match(LE_); 
            b = createBinaryExpression(LE_EXPR, b, expr());
            break;
        case NE_: 
            match(NE_);  
            b = createBinaryExpression(NE_EXPR, b, expr());
            break;
        case '=':
            match('='); 
            b = createBinaryExpression(EQ_EXPR, b, expr());
            break; 
        case '>':
            match('>');
            b = createBinaryExpression(GT_EXPR, b, expr());
            break;  
        case '<':
            match('<');
            b = createBinaryExpression(LT_EXPR, b, expr());
            break; 
    }
    return b;
}

Expr* boolFactor(){
    Expr *b;
    switch(lookahead){
        case NOT_:
            b = malloc(sizeof(Expr));
            b->type = NOT_EXPR;
            b->op.unary = boolFactor();
            break;
        case '(':
            match('(');
            b = boolExpr();
            match(')');
            break;
        default:
           b = compare();
           break; 
    }
    return b;
}

Expr* boolTerm(){
    Expr *b = boolFactor();
        while (1){
        if(lookahead == AND_){ 
            match(AND_);
            b = createBinaryExpression(AND_EXPR, b, boolExpr());
        } else break;          
    }
    return b;
}

Expr* boolExpr(){
    Expr *b = boolTerm();
    while (1){
        if(lookahead == OR_){ 
            match(OR_);
            b = createBinaryExpression(OR_EXPR, b, boolExpr());
        } else break;          
    }
    return b;
}




Stmt* assign_stmt(){
    Stmt *s = (Stmt*) malloc(sizeof(Stmt));
    s->type = ASSIGN_STMT;
    s->s.assign_.lval = lval->s;
    if(!s->s.assign_.lval){
        printf("Error: Assigning Value to non-existant Symbol\n");
        exit(1);
    }
    match(IDENT_);
    match(ASSIGN_);
    s->s.assign_.rval = expr();
    return s;
}

Stmt* block(){
    Stmt* s = (Stmt*) malloc(sizeof(Stmt));
    s->type = BLOCK_STMT;
    s->s.block_.list = stmt();
    while(lookahead != OD_ && lookahead != FI_ && lookahead != ELSE_ && lookahead != ELSIF_){
        s->s.block_.list->next = block();
    }
    return s;
}

Stmt* while_stmt(){
    match(WHILE_);
    Stmt* s = (Stmt*) malloc(sizeof(Stmt));
    s->type = WHILE_STMT;
    s->s.while_.cond = boolExpr();       
    match(DO_);    
    s->s.while_.body = block();
    match(OD_);
    return s;
}

Stmt* if_stmt(){
    match(IF_);
    Stmt* s = (Stmt*) malloc(sizeof(Stmt));
    s->type = IF_STMT;
    s->s.if_.cond = boolExpr();
    match(THEN_);
    s->s.if_.body = block();
    if(lookahead == ELSIF_){
        match(ELSIF_);
        s->s.if_.body = elseIf_stmt();
    } 
    if(lookahead == ELSE_){
        match(ELSE_);
        s->s.if_.elseBody = block();
    }
    match(FI_);
    return s;
}

 Stmt* elseIf_stmt(){
        Stmt* s = (Stmt*) malloc(sizeof(Stmt));
    s->type = IF_STMT;
    s->s.if_.cond = boolExpr();
    match(THEN_);
    s->s.if_.body = block();
    if(lookahead == ELSIF_){
        match(ELSIF_);
    }
    return s;
} 

Stmt* action_stmt(){
    //printf("action_stmt\n");
    int action = lookahead;
    lookahead = nextToken(lval);

    if ((action == FORWARD_) || (action == LEFT_) || (action == RIGHT_)){
        return createActionStmt(action, expr());
    }
    return createActionStmt(action, NULL);
}

Stmt* stmt(){
    //printf("stmt\n");
    switch(lookahead)
    {
        case FORWARD_: case LEFT_: case RIGHT_: case PENUP_: case PENDOWN_: case PUSHSTATE_: case POPSTATE_: case HOME_:
            //printf("stmt_foward\n");
            return action_stmt();
        case IDENT_:
            return assign_stmt();
        case IF_:
            return if_stmt();
        case WHILE_:
            return while_stmt();
        case VAR_:
            return declare_stmt();
        default:
            break;
    }

    return NULL;

}

void stmt_seq()
{
    do{
        Stmt *s = stmt(); //Build our AST
        if(s == NULL)
            printf("execute stmt null \n");

        executeStmt( s ); //Eval our stmt
    }
    while (lookahead != 0);
}

void ExecDeclareStmt(Stmt* stmt){
    symAdd(stmt->s.declare_.sym);
}

Stmt* declare_stmt() {
    match(VAR_);
    Stmt* s = (Stmt*) malloc(sizeof(Stmt));
    s->type = DECLARE_STMT;
    s->s.declare_.sym = lval->s;
    match(IDENT_);
    return s;
}


/* program -> stmt_list */
void program() {
    pushStack();
    while(lookahead == VAR_) {
        Stmt* temp = declare_stmt();
        executeStmt(temp);
    }
    stmt_seq();
    popStack();
}

/* stmt_list -> stmt {stmt}*/
int main(void) {
    
    lval = (LVAL*) malloc( sizeof(LVAL) );
    turtleInit();
    lookahead = nextToken( lval );
    if( !lookahead )
        fprintf(stderr, "Nothing to read");
    else
        program();
    turtleDumpImage( stdout );
    /* turtleInit();
    turtlePenUp(1); // penup
    turtleForward(10);
    turtlePenUp(0); // pendown
    turtleRotate(90);
    turtleForward(10);
    turtleRotate(90);
    turtleForward(20);
    turtleRotate(90);
    turtleForward(20);
    turtleRotate(90);
    turtleForward(20);
    turtleRotate(90);
    turtleForward(10);
    turtleDumpImage( stdout ); */
    return 0;

}


