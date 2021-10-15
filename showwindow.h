#ifndef SHOWWINDOW_H
#define SHOWWINDOW_H

#include <QWidget>
#include "plot.h"
#include <QVector>
#include <QListWidgetItem>
#include "horizontalcollector.h"
#include "verticalcollector.h"

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

	void on_listWidget_itemClicked(QListWidgetItem *item);

private:
	Ui::ShowWindow *ui;
	QVector<HorizontalCollector> *hor_coll;
	QVector<VerticalCollector> *vert_coll;
	int I, K;
	double R, T;
};

#endif // SHOWWINDOW_H
