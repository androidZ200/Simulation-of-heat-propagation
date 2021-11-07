#include "horizontalcollector.h"

HorizontalCollector::HorizontalCollector(double t) :
	ICollector(t)
{}

HorizontalCollector::~HorizontalCollector()
{
}

double* HorizontalCollector::setArray(double* arr)
{
	double* t = array;
	array = arr;
	return t;
}
