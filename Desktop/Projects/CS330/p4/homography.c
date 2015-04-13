#include "LUdecomp.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>


int main(void){
	int N;
	scanf("%d", &N);
	double target[N][2];
	double source[N][2];
	
	for (int i = 0; i < N; i++){
		scanf("%lf %lf\n", &source[i][0], &source[i][1]);
	}
	for (int i = 0; i < N; i++){
		scanf("%lf %lf\n", &target[i][0], &target[i][1]);
	}

	double **A = malloc(2*N*sizeof(double *));
	for (int i = 0; i < 2*N; i++)
  		A[i] = calloc(8, sizeof(double));

	for (int i = 0; i < N; i++){
		A[2*i][0] = source[i][0];
		A[2*i][1] = source[i][1];
		A[2*i][2] = 1;
		A[2*i][3] = 0;
		A[2*i][4] = 0;
		A[2*i][5] = 0;
		A[2*i][6] = -source[i][0] * target[i][0];
		A[2*i][7] = -source[i][1] * target[i][0];
		A[2*i+1][0] = 0;
		A[2*i+1][1] = 0;
		A[2*i+1][2] = 0;
		A[2*i+1][3] = source[i][0];
		A[2*i+1][4] = source[i][1];
		A[2*i+1][5] = 1;
		A[2*i+1][6] = -source[i][0] * target[i][1];
		A[2*i+1][7] = -source[i][1] * target[i][1];
	}

	double *b = malloc(2 * N * sizeof(double));
	for (int i = 0; i < N; i++){
		b[2*i] = target[i][0];
		b[2*i+1] = target[i][1];
	}

	double **AtA = malloc(8*sizeof(double *));
	for (int i = 0; i < 8; i++)
  		AtA[i] = calloc(8, sizeof(double));

  	for (int i = 0; i < 8; i++){
		for (int j = i; j < 8; j++){
			double sum = 0;
				for (int k = 0; k < 2 * N; k++){
					sum += A[k][i] * A[k][j];
				}
			AtA[i][j] = sum;
		}
}
	
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < i; j++){
			AtA[i][j] = AtA[j][i];
		}
	}

	double *Atb = malloc(8 * sizeof(double));

	for (int i = 0; i < 8; i++){
		double sum = 0;
		for (int j = 0; j < 2 * N; j++){
			sum += A[j][i] * b[j];
		}
		Atb[i] = sum;
	}


	LUdecomp *decomp;
	decomp = LUdecompose(8, (const double**)AtA);
	assert (decomp != NULL);
	double *x = calloc(8, sizeof(double));
	LUsolve(decomp, Atb, x);






for (int i = 0; i < 8; i++){
		printf("%f \n", x[i]);
}

/*for (int i = 0; i < 2*N; i++){
		for (int j = 0; j < 8; j++){
			printf("%8.0f ",A[i][j]);
		}
		printf("\n");
	}
	
for (int i = 0; i < N; i++){
	printf("%f %f\n", target[i][0], target[i][1]);
}
for (int i = 0; i < N; i++){
	printf("%f %f\n", source[i][0], source[i][1]);
}*/






	return 0; 
}
