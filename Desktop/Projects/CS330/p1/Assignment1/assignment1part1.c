// Ryan "Bob" Dean
// CS 330 Assignment 1
// In which I realize I forgot everything from Calc 2 and Intro to C

#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#ifndef M_LN2
#define M_LN2 0.6931471805599453094172321214581765680755001343602552
#endif

int main(int argc, char** argv)
{
	int n = 0;
	long double nPlusOneFactorial = 1;
	long double error = 0;


	for(n=1; n <= 15; n++){
		nPlusOneFactorial *= (n + 1);
		error = (2/(nPlusOneFactorial)) * powl((M_LN2/2), (n + 1));		
		printf("N = %i, error = %Le\n", n, error);
	}
return 0;
}
