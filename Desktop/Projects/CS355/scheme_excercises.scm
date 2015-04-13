#lang racket
;;example scheme code 
;; Thanh Dang

(define append (lambda (L M)
  (if (null? L) M
      (cons (car L) (append(cdr L) M)))))
; TEST: (append '(1 2) '(3 4))

;;List operations
;get the length of a list
(define getlen (lambda (l) 
                 (if (empty? l) 0
                     (+ 1 (getlen (cdr l)))
                            ) ))

;TEST: (getlen '(1 2 3 4))

;;reverse a list
(define reverse (lambda (L)
                  (if (null? L) '()
                      (append (reverse (cdr L)) (list (car L))))))

;TEST: (reverse '(1 2 3 4))

; get the first element
(define getFirst (lambda (L) (car L)))
; get n-th elment
(define findnth (lambda (l n) 
                  (if (= 0 n) 
                      (car l)
                      (findnth (cdr l) (- n 1) ))
                  )) 
; find last element
; short solution, reverse the list and get the first element
(define findlast (lambda (l) (car (reverse l)) ) )
;long solution, calculate the length and to get the index of the last element
;(define findlast (lambda (l) (findnth l (getlen l)))

; remove n-th element
(define removenth (lambda (l n) 
                    (if (null? l) '() 
                        (if (not (= n 0)) (cons (car l)  (removenth (cdr l) (- n 1))) (cdr l))
                        ) ))

;;square list
(define square-list (lambda (L)
                      (if (null? L) '()
                          (cons (* (car L) (car L))
                                (square-list (cdr L))))))


