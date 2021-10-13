#include "implicitschema.h"


ImplicitSchema::~ImplicitSchema()
{
	delete[] a;
	delete[] b;

	delete[] A;
}

void ImplicitSchema::Init(double R, double T, double l, double k,
						  double c, double alpha, int I, int K)
{
	ISolver::Init(R, T, l, k, c, alpha, I, K);
	a = new double[I];
	b = new double[I];

	A = new double[I];

	B0 = 1 + (4*k_c/h_r/h_r + alpha_cl)*h_t;
	Bi = 1 + (2*k_c/h_r/h_r + alpha_cl)*h_t;

	double C0 = -4*k_c*h_t/h_r/h_r;
	a[0] = C0 / B0;

	for(int i = 1; i < I; i++) {
		double Ci = k_c*h_t/h_r/h_r*(1+0.5/i);
		A[i] = k_c*h_t/h_r/h_r*(1-0.5/i);
		a[i] = -Ci/(A[i]*a[i-1] + Bi);
	}
}

void ImplicitSchema::step()
{
	second_array[I] = 0;

	b[0] = first_array[0]/B0;
	for(int i = 1; i < I; i++)
		b[i] = (first_array[i] - A[i]*b[i-1])/(A[i]*a[i-1] + Bi);

	second_array[I-1] = (first_array[I-1] - A[I-1]*b[I-1])/(A[I-1]*a[I-1] + Bi);
	for(int i = I-2; i >= 0; i--)
		second_array[i] = a[i]*second_array[i+1] + b[i];
}
