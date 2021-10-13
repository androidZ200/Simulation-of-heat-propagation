#ifndef IMPLICITSCHEMA_H
#define IMPLICITSCHEMA_H

#include "isolver.h"

class ImplicitSchema : public ISolver
{
	double *a, *b;
	double *A;
	double B0, Bi;

public:
	~ImplicitSchema() override;
	void Init(double R, double T, double l, double k,
			  double c, double alpha, int I, int K) override;

protected:
	void step() override;
};

#endif // IMPLICITSCHEMA_H
