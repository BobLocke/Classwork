#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif 

//
//   X(t+h) = X(t) + h*F(t,X) 
//

void RK4(int N, double t, double h, const double X[],
	 void (*F)(double t, const double X[], double R[]),
	 double R[]) {
  double K1[N];
  F(t,X,K1);

  double T[N], K2[N];
  for (int i = 0; i < N; i++)
    T[i] = X[i] + h/2*K1[i];
  F(t+h/2,T,K2);

  double K3[N];
  for (int i = 0; i < N; i++)
    T[i] = X[i] + h/2*K2[i];
  F(t+h/2,T,K3);

  double K4[N];
  for (int i = 0; i < N; i++)
    T[i] = X[i] + h*K3[i];
  F(t+h,T,K4);

  for (int i = 0; i < N; i++)
    R[i] = X[i] + h/6*(K1[i] + 2*(K2[i] + K3[i]) + K4[i]);
}

void Fbaseball(double t, const double X[], double R[]) {
  const double k = 0.002;
  const double g = 32.0;
  R[0] = X[1];
  R[1] = -k * X[1] * sqrt((X[1]*X[1]) + (X[3] * X[3]));
  R[2] = X[3];
  R[3] = -k * X[3] * sqrt((X[1]*X[1]) + (X[3] * X[3])) - g;
}

void Fch11(double t, const double X[], double R[]){
  R[0] = X[1];
  R[1] = t * X[0];
}

void FNewton(double t, const double X[], double R[]) {
  long double m = 0.3301 * pow(10, 24);
  long double M = 1.989 * pow(10, 30);
  long double G = 6.67 * pow(10, -11);
  long double r = sqrt(X[0] * X[0] + X[1] * X[1]);
  long double F = M * G / (r * r);
  R[0] = X[2];
  R[1] = X[3];
  R[2] = -F * (X[0]/r);
  R[3] = -F * (X[1]/r);
}

// code for Mercury
int main () {
  double t = 0.0;
  double h = 87.969/100;
  double X[4];
  X[0] = 46001200;
  X[1] = 0;
  X[2] = 0;
  X[3] = 58.98; 

  for (int i = 0; i < 101; i++, t += h) {
    printf("%0.10f %0.10f\n",t, X[0]);
    RK4(4, t, h, X, FNewton, X);
  }
}


// code for 11.2.2
/*
int main() {
  double t = 0.0;
  double h = 0.045;
  double X[3];
  X[0] = 0.355028053887817;
  X[1] = -0.258819403792807;
  for (int i = 0; i < 101; i++, t += h) {
    printf("%0.10f %0.10f\n",t, X[0]);
    RK4(2, t, h, X, Fch11, X);
  }

}
 */

 /* 

 // Code for Baseball Problem

int main() {
  double t = 0.0;
  double h = 6.47859/100;
  double v = 208.0;
  double theta = 43*M_PI/180;
  double X[4];
  X[0] = 0.0; X[1] = v*cos(theta);
  X[2] = 3.0; X[3] = v*sin(theta);
  for (int i = 0; i < 101; i++, t += h) {
    printf("%0.10f %0.10f\n", X[0], X[2]);
    // XXX Euler(4, t, h, X, Fbaseball, X);
    RK4(4, t, h, X, Fbaseball, X);
  }
  return 0;
}
*/


