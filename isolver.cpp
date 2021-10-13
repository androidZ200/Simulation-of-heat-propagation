#include "isolver.h"

ISolver::~ISolver()
{
	if(first_array)
		delete[] first_array;
	if(second_array)
		delete[] second_array;
}

void ISolver::Init(double R, double T, double l, double k,
				   double c, double alpha, int I, int K)
{
	this->I = I;
	this->K = K;
	k_c = k/c;
	alpha_cl = 2*alpha/c/l;
	h_r = R/I;
	h_t = T/K;
	first_array = new double[I+1];
	second_array = new double[I+1];

	for(auto &x : *hor_coll)
		x.init(T, K, I+1);
	for(auto &x : *vert_coll)
		x.init(R, I, K+1);
}

void ISolver::setHorCollectors(QVector<HorizontalCollector>* coll)
{
	hor_coll = coll;
}

void ISolver::setVertCollectors(QVector<VerticalCollector>* coll)
{
	vert_coll = coll;
}

void ISolver::setBeginingLayer(double (*psi)(double))
{
	for(int i = 0; i <= I; i++)
		first_array[i] = psi(i*h_r);
}

void ISolver::setBeginingLayer(double* arr)
{
	for(int i = 0; i <= I; i++)
		first_array[i] = arr[i];
}

double ISolver::hr() const
{
	return h_r;
}

double ISolver::ht() const
{
	return h_t;
}

QVector<HorizontalCollector>* ISolver::getHorizontalCollectors() const
{
	return hor_coll;
}

QVector<VerticalCollector>* ISolver::getVerticalCollectors() const
{
	return vert_coll;
}

void ISolver::Start()
{
	int end;
	isWork = true;
	QVector<HorizontalCollector>::iterator iter = hor_coll->begin();
	while(iter != hor_coll->end() && iter->getIndex() < cur_index) ++iter;

	while(iter != hor_coll->end()) {
		end = iter->getIndex();
		solv_to(end, cur_index);
		if(!isWork) return;
		second_array = iter->setArray(second_array);
		emit newLayer(*iter);
		++iter;
	}
	end = K;
	solv_to(end, cur_index);
	if(!isWork) return;

	emit Finish();
}

void ISolver::Stop()
{
	isWork = false;
}

void ISolver::saveVertColl()
{
	for(auto &x : *vert_coll)
		x.setItem(first_array[x.getIndex()]);
}

void ISolver::solv_to(int end, int& ind)
{
	for(;ind<=end && isWork;++ind) {
		saveVertColl();
		step();
		qSwap(first_array, second_array);
		emit Process(ind*100.0/K);
	}
}
