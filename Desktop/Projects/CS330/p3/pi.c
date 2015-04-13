#include<math.h>
#include<stdio.h>
#include<stdlib.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

long double f(long double x){
  return 4.0L/(1.0L + x*x);
}

long double trap(long double (*f) (long double), long double a, long double b, int n){
  const long double h = (b - a)/n;
  long double sum = .5*(f(a) + f(b));
  for (int i = 1; i < n; i++){
    long double x = a + i * h;
    sum += f(x);   
  }
  return sum * h;
}

long double simpsons(long double (*f) (long double), long double a, long double b, int n){
 const long double h = (b - a)/n;
  long double sum = (f(a) + f(b));
  for (int i = 2; i < n;i+=2){
    long double x = a + i * h;
    sum += 2*f(x);
  }
  for (int i = 1; i < n;i+=2){
    long double x = a + i * h;
    sum += 4*f(x);
  }
  return sum * (h/3);
}

long double simpsons38(long double (*f) (long double), long double a, long double b, int n){
 const long double h = (b - a)/n;
  long double sum = (f(a) + f(b));
for (int i = 1; i < n;i+=3){
    long double x = a + i * h;
    sum += 3*f(x);
  }
  for (int i = 2; i < n;i+=3){
    long double x = a + i * h;
    sum += 3*f(x);
  }
  for (int i = 3; i < n;i+=3){
    long double x = a + i * h;
    sum += 2*f(x);
  }
  return sum * ((3*h)/8);
}

long double booles(long double (*f) (long double), long double a, long double b, int n){
  const long double h = (b - a)/n;
  long double sum = 7*(f(a) + f(b));
  for (int i = 2; i < n;i+=4){
    long double x = a + i * h;
    sum += 12*f(x);
  }
  for (int i = 4; i < n;i+=4){
    long double x = a + i * h;
    sum += 14*f(x);
  }
  for (int i = 1; i < n;i+=2){
    long double x = a + i * h;
    sum += 32*f(x);
  }
  return sum * ((2*h)/45);
}

int main(void) {
  const unsigned maxn = 800000;

  for (unsigned n = 12; n <= maxn; n *= 2) {

    const long double I_trap = trap(f, 0, 1, n);

    const long double error_trap = fabsl(M_PI - I_trap);

    const long double I_simp = simpsons(f, 0, 1, n);

    const long double error_simp = fabsl(M_PI - I_simp);

    const long double I_simp38 = simpsons38(f, 0, 1, n);

    const long double error_simp38 = fabsl(M_PI - I_simp38);

    const long double I_booles = booles(f, 0, 1, n);

    const long double error_booles = fabsl(M_PI - I_booles);

    printf("%5d %0.10Le %0.10Le %0.10Le %0.10Le\n", n, 
	   error_trap, error_simp, error_simp38, error_booles);

  }
  return 0;
}
