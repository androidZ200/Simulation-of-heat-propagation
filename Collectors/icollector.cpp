#include "icollector.h"

ICollector::ICollector(double slice)
{
	point = slice;
}

ICollector::~ICollector()
{
	if(N) delete[] array;
}

void ICollector::init(double lenght, int pooints_in_lenght, int N)
{
	double h = lenght/pooints_in_lenght;
	int i = point/h;
	index = point - i*h <= i*h + h - point ? i : i+1;
	array = new double[N];
	this->N = N;
}

int ICollector::get_index() const
{
	return index;
}

double ICollector::get_point() const
{
	return point;
}

const double* ICollector::get_array() const
{
	return array;
}
