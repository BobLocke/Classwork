//Ryan "Bob" Dean
//assignment1part2.c

#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#ifndef M_LN2
#define M_LN2 0.6931471805599453094172321214581765680755001343602552
#define M_LOG2E 1.4426950408889634073599246810018921374266459541529859
#endif

float myexp(float x){
	
	float z = x * M_LOG2E;
	int m = round(z);
	float w = z - m;
	float u = w * M_LN2;
	
	return ldexp(1, m) * (1 + u + u*(u/2 + u*(u/6 + u*(u/24 + u*(u/120 + u/720)))));	
}

int main(int argc, char** argv)
{
	float x = -20.0;
	int n;
	float y1, y2, rerr;
	
	for(n=1; n <= 30; n++){
	y1 = myexp(x);
	y2 = expl(x);
	rerr = (y1 - y2) / y2;	
	printf("%f || %e || %e || %e \n", x, y1, y2, rerr);
		x += 1.264;
	}
return 0;
}
