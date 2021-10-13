#ifndef ISOLVER_H
#define ISOLVER_H

#include <QObject>
#include <QVector>
#include "horizontalcollector.h"
#include "verticalcollector.h"

class ISolver : public QObject
{
	Q_OBJECT

protected:
	int I, K;
	double k_c, alpha_cl;
	double h_r, h_t;
	QVector<HorizontalCollector>* hor_coll = 0;
	QVector<VerticalCollector>* vert_coll = 0;
	double *first_array = 0;
	double *second_array = 0;
	int cur_index = 0;
	bool isWork;

public:
	virtual ~ISolver();

	virtual void Init(double R, double T, double l, double k,
			  double c, double alpha, int I, int K);
	void setHorCollectors(QVector<HorizontalCollector> *coll);
	void setVertCollectors(QVector<VerticalCollector> *coll);
	void setBeginingLayer(double (*psi)(double x));
	void setBeginingLayer(double* arr);
	double hr() const;
	double ht() const;
	QVector<HorizontalCollector>* getHorizontalCollectors() const;
	QVector<VerticalCollector>* getVerticalCollectors() const;
public slots:
	virtual void Start();
	virtual void Stop();

protected:
	virtual void step() = 0;

private:
	void saveVertColl();
	void solv_to(int end, int& ind);

signals:
	void Finish();
	void Process(double percent);
	void newLayer(const HorizontalCollector& layer);
};

#endif // ISOLVER_H
