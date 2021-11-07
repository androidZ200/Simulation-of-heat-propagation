#include "verticalcollector.h"

VerticalCollector::VerticalCollector(double r) :
	ICollector(r)
{}

VerticalCollector::~VerticalCollector()
{

}

void VerticalCollector::setItem(double u)
{
	array[localIndex++] = u;
}
