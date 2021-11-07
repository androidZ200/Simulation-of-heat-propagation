#ifndef ICOLLECTOR_H
#define ICOLLECTOR_H


class ICollector
{
protected:
	double* array;
	int N = 0;
	double right_border;
	double point;
	int index;

public:
	ICollector(double slice);
	virtual ~ICollector();

	virtual void init(double lenght, int pooints_in_lenght, int N);
	int get_index() const;
	double get_point() const;
	const double* get_array() const;
};

#endif // ICOLLECTOR_H
