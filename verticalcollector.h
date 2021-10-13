#ifndef VERTICALCOLLECTOR_H
#define VERTICALCOLLECTOR_H


class VerticalCollector
{
	double* array;
	double r;
	int index;

	int localIndex = 0;
public:
	VerticalCollector(double r);

	void init(double R, int I, int size);
	int getIndex() const;
	void setItem(double u);

	double* getArray() const;
};

#endif // VERTICALCOLLECTOR_H
