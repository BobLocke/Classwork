% Ryan "Bob" Dean
% Assignment 4: Prolog Hell

:- use_module(library(lists)).


% given a set of t/f values, find truth value of X and return as R
% X : variable input
% A, B, C : input states
% R : output boolean

eval-var(X,[[A|[C]]|B], R) :- X = A, R = C.
eval-var(X,[[A|[C]]|B], R) :- X \= A, eval-var(X, B, R).

% example use:
% eval-var(a, [[a,true],[b,false]],X).


% evaluates a set of ORed statements, reutrn t/f value
% C: input states
% A, B: 
% R: output boolean

eval-clause(C, [], R) :- R = false.
eval-clause(C, [[not, A]|B], R) :- eval-var(A, C, X), X = false, R = true.
eval-clause(C, [[not, A]|B], R) :- eval-var(A, C, X), X = true, eval-clause(C,B,R).
eval-clause(C, [A|B], R) :- eval-var(A, C, X), X = true, R = true.
eval-clause(C, [A|B], R) :- eval-var(A, C, X), X = false, eval-clause(C,B,R).

% example use:
% eval-clause([[a,false],[b,false],[c,false],[d,false]],[a,[not, b], c],X).


% pulls all variables from a clause and returns a list
% [A,B] : input clause
% X: returned list
% Q: temporary variable used to derermine if A is a list 
% Z: temporary list

get-vars([], []).
get-vars([[not, A]|B], X) :- get-vars(B, Z), append([A], Z, X).
get-vars([A|B], X) :- Q = is_list(A), not(Q), append([A], Z, X), get-vars(B,Z).

% example use:
% get-vars([a, [not, b], c], X).


% gets all variables from a group of clauses and returns them in a set
% [C,R] : list of input clauses
% X: return set
% Z: temporary list
% Y: temporary return list from get-vars
% H: temporary pre-sorted return set

get-all-vars([], []).
get-all-vars([C|R], X) :- get-all-vars(R, Z), get-vars(C,Y), append(Y, Z, H), sort(H, X).

% example use:
% get-all-vars([[a,[not,b],c],[c,[not,d],e],[a,[not,e],f]],X).


% gets all unsatisfied clauses from a list of clauses
% [C,R]: input list of clauses
% S: input states
% X: output list of unsatisfied clauses
% Y: temporary return value from eval-clause
% Z: temporary list of unsatisfied clauses

unsat-clauses([],_,[]).
unsat-clauses([C|R], S, X) :- eval-clause(S, C, Y), Y = true, unsat-clauses(R, S, X).
unsat-clauses([C|R], S, X) :- eval-clause(S, C, Y), Y = false, unsat-clauses(R, S, Z), append([C], Z, X).

% example use:
% unsat-clauses([[a,[not,b],c],[a,[not,b],[not,c]],[a,[not,b],d]],[[a,false],[b,true],[c,true],[d,false]],X).


% changes true/false value of a variable in a state
% X : variable input
% [A,B],T : state input
% R : output state

flip-var(X,[],[]).
flip-var(X, [[A|B]|T], R) :- X \= A, flip-var(X, T, Y), append([[A|B]], Y, R).
flip-var(X, [[A|B]|T], R) :- X = A, B = [true], flip-var(X, T, Y), C = false, append([[A,C]], Y, R).
flip-var(X, [[A|B]|T], R) :- X = A, B = [false], flip-var(X, T, Y), C = true, append([[A,C]], Y, R).

% example use:
% flip-var(a,[[a,false],[b,true],[c,true],[d,false]],R]).