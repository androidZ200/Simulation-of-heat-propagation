#ifndef LOADWINDOW_H
#define LOADWINDOW_H

#include <QWidget>
#include "Schemes/isolver.h"
#include "showwindow.h"
#include <QThread>
#include <QCloseEvent>

namespace Ui {
class loadwindow;
}

class LoadWindow : public QWidget
{
	Q_OBJECT

public:
	explicit LoadWindow(ISolver* solver, QWidget *parent = nullptr);
	~LoadWindow();

private slots:
	bool on_pushButton_cancel_clicked();
	void GetResult();
	void Progress(double percent);
	void on_pushButton_start_stop_clicked();

private:
	Ui::loadwindow *ui;
	ISolver* solver;
	QThread thread;
	QVector<HorizontalCollector>* hor_coll;
	QVector<VerticalCollector>* vert_coll;
	bool isStarting = true;

protected:
	void closeEvent(QCloseEvent* event) override;
};

#endif // LOADWINDOW_H
