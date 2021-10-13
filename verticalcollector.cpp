#include "verticalcollector.h"

VerticalCollector::VerticalCollector(double r)
{
	this->r = r;
}

VerticalCollector::~VerticalCollector()
{
	if(array)
		delete[] array;
}

void VerticalCollector::init(double R, int I, int size)
{
	double h = R/I;
	int i = r/h;
	index = r - i*h <= i*h + h - r ? i : i+1;
	array = new double[size];
}

int VerticalCollector::getIndex() const
{
	return index;
}

void VerticalCollector::setItem(double u)
{
	array[localIndex++] = u;
}

double* VerticalCollector::getArray() const
{
	return array;
}
