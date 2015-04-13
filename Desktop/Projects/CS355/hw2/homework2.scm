#lang racket

;Ryan "Bob Dean;
;CS 355;
;Assignment 2;

(define state '((A #f)(B #t)(C #f)(D #t)))

(define clause '(A (not B) C))

(define clauses '(((not B) C) ((not B) C) (C B)))

;#1 eval_var will return the true/false status of a variable in a state;

(define eval_var (lambda (var state) 
        (if (equal? (car (car state)) var) 
           (car (cdr (car state))) 
        (eval_var var (cdr state)))))

;#2 eval_clause will determine if a clause is satisfied by a given state;

(define eval_clause(lambda (clause state)
        (if (empty? clause) 
            #f
            (if (list? (car clause)) 
                (if (not (eval_var (car (cdr (car clause))) state))
                    #t
                    (eval_clause (rest clause) state))                    
                (if 
                    (equal? (eval_var (car clause) state) #t) 
                        #t 
                        (eval_clause (rest clause) state))))))

;#3 get_vars will return all variables from a single clause;

(define get_vars(lambda (clause)
               (if (empty? clause)
                   '()   
                   (if (list? (car clause)) 
                       (cons (car (cdr (car clause))) (get_vars (cdr clause)))                  
                       (cons (car clause) (get_vars (cdr clause)))
                   )
               )))

;#4 get_all_vars will return a set of all variables used in a set of clauses;

(define get_all_vars (lambda (clauses)  
                       (if (empty? clause)
                           '()
                           (if (empty? clauses)
                           '()    
                           (remove-duplicates (flatten 
                                               (cons 
                                                (get_vars (car clauses)) 
                                                (get_all_vars (rest clauses)))))
                       ))))

;#5 unsat_clauses will determine the number of unsatisfied clauses for the given state;

(define unsat_clauses (lambda (clauses state)
                        (if (empty? clauses)
                            '()
                            (if (not (eval_clause (car clauses) state))
                                (cons (car clauses) (unsat_clauses (cdr clauses) state))
                                (append (unsat_clauses (cdr clauses) state))
                            )
                        )
                    ))

;#6 flip_var will toggle the true/false value of a variable in a state;

(define flip_var (lambda (var state)
               (if (empty? state)
                   '()
                   (if (equal? var (car (car state)))
                       (cons (list (car (car state)) (not (car (cdr (car state))))) (rest state)) 
                       (cons (car state) (flip_var var (rest state))) 
                   )
               )))

;#7 get_better_neighbor will determine if by toggling one of the varibles in the given state, more arguments in the clause can be satisfied, and if so will reurn the new state;

(define get_better_neighbor (lambda (clauses state vars num_unsat)
               (if (empty? vars)
                   #f
                   (if (< (length (unsat_clauses clauses (flip_var (car vars) state))) num_unsat)
                       (flip_var (car vars) state)
                       (get_better_neighbor clauses state (rest vars) (length (unsat_clauses clauses state)))
                              
               ))))

;#8 simple_hill_climb will find the set of states that satisfies the most arguments in the given clause;

(define simple_hill_climb (lambda (clauses state dist unsat)
                   (if (equal? 0 dist)
                       state
                       (if (equal? #f (get_better_neighbor clauses state (get_all_vars clauses) (length (unsat_clauses clauses state))))
                           state
                           (simple_hill_climb clauses 
                                              (get_better_neighbor clauses state (get_all_vars clauses) (length (unsat_clauses clauses state)))
                                              (- dist 1)
                                              (unsat_clauses clauses state))))))
                       
                                
                           
                   