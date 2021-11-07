#ifndef VERTICALCOLLECTOR_H
#define VERTICALCOLLECTOR_H

#include "icollector.h"

class VerticalCollector : public ICollector
{
	int localIndex = 0;
public:
	VerticalCollector(double r);
	~VerticalCollector() override;

	void setItem(double u);
};

#endif // VERTICALCOLLECTOR_H
