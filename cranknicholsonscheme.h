#ifndef CRANKNICHOLSONSCHEME_H
#define CRANKNICHOLSONSCHEME_H

#include "isolver.h"

class CrankNicholsonScheme : public ISolver
{
	double *a, *b;
	double *A, *C;
	double B0, Bi;

public:
	~CrankNicholsonScheme() override;
	void Init(double R, double T, double l, double k,
			  double c, double alpha, int I, int K) override;

protected:
	void step() override;
};

#endif // CRANKNICHOLSONSCHEME_H
