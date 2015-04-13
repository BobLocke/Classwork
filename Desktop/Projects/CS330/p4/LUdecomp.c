/*
 * Assignement 4: LU Decomposition
 * Ryan "Bob" Dean
 * With help from: Adam (Helped me with learning memory allocation and pointers, checked for errors) 
 */


#include "LUdecomp.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>

/*
 * Given NxN matrix A (stored as an array of N row ptrs),
 * returns LU decomposition information.
 * If A is singluar, NULL is returned.
 */
LUdecomp *LUdecompose(int N, const double **A){
  LUdecomp* decomp = malloc(sizeof(LUdecomp)); 
  decomp->mutate = calloc(N, sizeof(short));
  decomp->N = N;
  decomp->d = 1;
  decomp->LU = calloc(N, sizeof(double *));
  for (int i = 0; i < N; i++){
    decomp->LU[i] = calloc(N, sizeof(double));
    decomp->mutate[i] = i;
    memcpy(decomp->LU[i], A[i], N * sizeof(double));
  }
  for (int j = 0; j < N; j++){
    for (int i = 0; i <= j; i++){
      double sum = 0;
      for (int k = 0; k < i; k++) sum += decomp->LU[i][k] * decomp->LU[k][j];
      decomp->LU[i][j] -= sum;
    }
    double p = fabs(decomp->LU[j][j]);
    int n = j;
    for (int i = j + 1; i < N; i++){
      double sum = 0;
      for (int k = 0; k < j; k++) sum += decomp->LU[i][k] * decomp->LU[k][j];
      decomp->LU[i][j] -= sum;
      if (fabs(decomp->LU[i][j]) > p){
	p = fabs(decomp->LU[i][j]);
	n = i;
      }
    }
      if (p == 0){   //  if p = 0, abort!
	LUdestroy(decomp);
	return NULL;
      }
      if (n != j){
	//swap rows
	double *tempArr = decomp->LU[n];
	decomp->LU[n] = decomp->LU[j];
	decomp->LU[j] = tempArr;
	//swap mutate
	double temp = decomp->mutate[n];
	decomp->mutate[n] = decomp->mutate[j];
	decomp->mutate[j] = temp;
	decomp->d *= -1;
      }
      for (int i = j+1; i<N; i++) decomp->LU[i][j] /= decomp->LU[j][j];	  
  }
    
  /*  TESTING PRINT
   for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            printf("%f ", decomp->LU[i][j]);
        }
    }
  */
return decomp;
}

/*
 * Deallocate decomposition info created by LUdecompose.
 */
void LUdestroy(LUdecomp* decomp){
  int N = decomp->N;
  for (int i = 0; i < N; i++) free(decomp->LU[i]);
  free(decomp->LU);
  free(decomp->mutate);
  free(decomp);
}

/*
 * Given LU decomposition info for A, 
 * solves linear system Ax = b for x.
 * x and b vectors are assumed to be length N.
 */
void LUsolve(LUdecomp *decomp, const double *b, double *x){
  double *y;
  int N = decomp->N;
  y = calloc(N, sizeof(double));
  y[0] = b[decomp->mutate[0]];
  for (int i = 1; i < N; i++){
    double sum = 0;
    for (int j = 0; j < i; j++){
      sum += decomp->LU[i][j] * y[j];
    }
    y[i] = b[decomp->mutate[i]] - sum;
  }
  x[N-1] = y[N-1]/decomp->LU[N-1][N-1];
  for (int i = N-2; i >= 0; i--){
     double sum = 0;
    for (int j = i + 1; j < N; j++){
      sum += decomp->LU[i][j] * x[j];
    }
    x[i] = (1/decomp->LU[i][i])*(y[i] - sum);
  }
  free(y);	     
}
