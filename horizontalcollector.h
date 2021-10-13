#ifndef HORIZONTALCOLLECTOR_H
#define HORIZONTALCOLLECTOR_H

class HorizontalCollector
{
	double* array;
	double t;
	int index;

public:
	HorizontalCollector(double t);

	void init(double T, int K, int size);
	int getIndex() const;
	double* setArray(double* arr);

	double* getArray() const;
};

#endif // HORIZONTALCOLLECTOR_H
