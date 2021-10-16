#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include "Collectors/horizontalcollector.h"
#include "Collectors/verticalcollector.h"
#include "loadwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:
	void on_pushButton_add_r_clicked();
	void on_pushButton_add_t_clicked();
	void on_pushButton_remove_r_clicked();
	void on_pushButton_remove_t_clicked();
	void on_pushButton_clear_clicked();
	void on_pushButton_start_clicked();
	void on_lineEdit_I_textChanged(const QString &arg1);
	void on_lineEdit_K_textChanged(const QString &arg1);
	void on_lineEdit_R_textChanged(const QString &arg1);
	void on_lineEdit_T_textChanged(const QString &arg1);
	void on_lineEdit_new_r_returnPressed();
	void on_lineEdit_new_t_returnPressed();

private:
	void update_lable_hr();
	void update_lable_ht();
	bool check_inruts();
	void checkListR();
	void checkListT();

	Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
