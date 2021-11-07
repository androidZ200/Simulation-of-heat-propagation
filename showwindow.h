#ifndef SHOWWINDOW_H
#define SHOWWINDOW_H

#include <QWidget>
#include "GlPlot/glplot.h"
#include <QVector>
#include <QListWidgetItem>
#include "Collectors/horizontalcollector.h"
#include "Collectors/verticalcollector.h"

namespace Ui {
class ShowWindow;
}

class ShowWindow : public QWidget
{
	Q_OBJECT

public:
	explicit ShowWindow(int I, int K, double R, double T, QVector<HorizontalCollector> *hor_coll,
				QVector<VerticalCollector> *vert_coll, QWidget *parent = nullptr);
	~ShowWindow();


private slots:
	void setCheckedItems();
	void updateList();

private slots:
	void redrawgraphics();
	void setStepHorizontalGrid(double X);
	void on_listWidget_itemClicked(QListWidgetItem *item);
	void on_pushButton_save_clicked();
	void on_lineEdit_textChanged(const QString &arg1);

private:
	double Value(const double* arr, int N, double x);
	const double* getArray();

	Ui::ShowWindow *ui;
	QVector<HorizontalCollector> *hor_coll;
	QVector<VerticalCollector> *vert_coll;
	int I, K;
	double R, T;
};

#endif // SHOWWINDOW_H
