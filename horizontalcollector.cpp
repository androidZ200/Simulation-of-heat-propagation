#include "horizontalcollector.h"

HorizontalCollector::HorizontalCollector(double t)
{
	this->t = t;
}

HorizontalCollector::~HorizontalCollector()
{
	if(array)
		delete[] array;
}

void HorizontalCollector::init(double T, int K, int size)
{
	double h = T/K;
	int i = t/h;
	index = t - i*h <= i*h + h - t ? i : i+1;
	array = new double[size];
}

int HorizontalCollector::getIndex() const
{
	return index;
}

double* HorizontalCollector::setArray(double* arr)
{
	double* t = array;
	array = arr;
	return t;
}

double* HorizontalCollector::getArray() const
{
	return array;
}
