#ifndef VERTICALCOLLECTOR_H
#define VERTICALCOLLECTOR_H

class VerticalCollector
{
	double* array = 0;
	double r;
	int index;

	int localIndex = 0;
public:
	VerticalCollector(double r);
	~VerticalCollector();

	void init(double R, int I, int size);
	int getIndex() const;
	void setItem(double u);

	double* getArray() const;
};

#endif // VERTICALCOLLECTOR_H
