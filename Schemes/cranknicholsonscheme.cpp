#include "cranknicholsonscheme.h"

CrankNicholsonScheme::~CrankNicholsonScheme()
{
	delete[] a;
	delete[] b;

	delete[] A;
	delete[] C;
}

void CrankNicholsonScheme::Init(double R, double T, double l, double k,
						  double c, double alpha, int I, int K)
{
	ISolver::Init(R, T, l, k, c, alpha, I, K);

	a = new double[I-1];
	b = new double[I-1];

	A = new double[I-1];
	C = new double[I-1];

	B0 = (2*k_c/h_r/h_r + alpha_cl/2)*h_t;
	Bi = (  k_c/h_r/h_r + alpha_cl/2)*h_t;

	C[0] = -2*k_c*h_t/h_r/h_r;
	a[0] = -C[0] /(1 + B0);

	for(int i = 1; i < I-1; i++) {
		C[i] =   -k_c*h_t/h_r/h_r/2*(1 + 0.5/i);
		A[i-1] = -k_c*h_t/h_r/h_r/2*(1 - 0.5/i);
		a[i] = -C[i]/(A[i-1]*a[i-1] + 1 + Bi);
	}
	A[I-2] = -k_c*h_t/h_r/h_r/2*(1 - 0.5/(I-1));
}

void CrankNicholsonScheme::step()
{
	second_array[I] = 0;

	b[0] = ((1 - B0)*first_array[0] - C[0]*first_array[1])/(1 + B0);
	double Fi;
	for(int i = 1; i < I-1; i++) {
		Fi = -A[i-1]*first_array[i-1] + (1-Bi)*first_array[i] -
				C[i]*first_array[i+1];
		b[i] = (Fi - A[i-1]*b[i-1])/(A[i-1]*a[i-1] + 1 + Bi);
	}

	Fi = -A[I-2]*first_array[I-2] + (1-Bi)*first_array[I-1];
	second_array[I-1] = (Fi - A[I-2]*b[I-2])/(A[I-2]*a[I-2] + 1 + Bi);
	for(int i = I-2; i >= 0; i--)
		second_array[i] = a[i]*second_array[i+1] + b[i];
}
