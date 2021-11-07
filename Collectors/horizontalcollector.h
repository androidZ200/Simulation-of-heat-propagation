#ifndef HORIZONTALCOLLECTOR_H
#define HORIZONTALCOLLECTOR_H

#include "icollector.h"

class HorizontalCollector : public ICollector
{
public:
	HorizontalCollector(double t);
	~HorizontalCollector() override;

	double* setArray(double* arr);
};

#endif // HORIZONTALCOLLECTOR_H
