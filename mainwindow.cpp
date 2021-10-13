#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

QLocale l(QLocale::German);

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	ui->lineEdit_I->setValidator(new QIntValidator(1, 1<<30, this));
	ui->lineEdit_K->setValidator(new QIntValidator(1, 1<<30, this));

	ui->lineEdit_R->setValidator(new QDoubleValidator(1e-9, 1e9, 9, this));
	ui->lineEdit_k->setValidator(new QDoubleValidator(1e-9, 1e9, 9, this));
	ui->lineEdit_l->setValidator(new QDoubleValidator(1e-9, 1e9, 9, this));
	ui->lineEdit_c->setValidator(new QDoubleValidator(1e-9, 1e9, 9, this));
	ui->lineEdit_alpha->setValidator(new QDoubleValidator(0, 1e9, 9, this));
	ui->lineEdit_T->setValidator(new QDoubleValidator(1e-9, 1e9, 9, this));

	ui->lineEdit_new_r->setValidator(new QDoubleValidator(0, 1e9, 9, this));
	ui->lineEdit_new_t->setValidator(new QDoubleValidator(0, 1e9, 9, this));

	update_lable_hr();
	update_lable_ht();
}

MainWindow::~MainWindow()
{
	delete ui;
}


void MainWindow::on_pushButton_add_r_clicked()
{
	if(ui->lineEdit_new_r->text() != "") {
		for(int i = 0; i < ui->listWidget_r->count(); i++)
			if(ui->listWidget_r->item(i)->text() == ui->lineEdit_new_r->text())
				return;
		ui->listWidget_r->addItem(ui->lineEdit_new_r->text());
	}
	ui->listWidget_r->sortItems();
}


void MainWindow::on_pushButton_add_t_clicked()
{
	if(ui->lineEdit_new_t->text() != "") {
		for(int i = 0; i < ui->listWidget_t->count(); i++)
			if(ui->listWidget_t->item(i)->text() == ui->lineEdit_new_t->text())
				return;
		ui->listWidget_t->addItem(ui->lineEdit_new_t->text());
	}
	ui->listWidget_r->sortItems();
}


void MainWindow::on_pushButton_remove_r_clicked()
{
	QList<QListWidgetItem*> items = ui->listWidget_r->selectedItems();
	foreach(QListWidgetItem * item, items)
	{
		delete ui->listWidget_r->takeItem(ui->listWidget_r->row(item));
	}
}


void MainWindow::on_pushButton_remove_t_clicked()
{
	QList<QListWidgetItem*> items = ui->listWidget_t->selectedItems();
	foreach(QListWidgetItem * item, items)
	{
		delete ui->listWidget_t->takeItem(ui->listWidget_t->row(item));
	}
}


void MainWindow::on_pushButton_clear_clicked()
{
	ui->listWidget_r->clear();
	ui->listWidget_t->clear();

	ui->lineEdit_I->setText("400");
	ui->lineEdit_K->setText("600");

	ui->lineEdit_R->setText("5");
	ui->lineEdit_k->setText("0,01");
	ui->lineEdit_l->setText("1");
	ui->lineEdit_c->setText("1,84");
	ui->lineEdit_alpha->setText("0,001");
	ui->lineEdit_T->setText("150");

	ui->lineEdit_new_r->clear();
	ui->lineEdit_new_t->clear();
}


void MainWindow::on_pushButton_start_clicked()
{
	if(!check_inruts()) {
		QMessageBox::warning(this, "Внимание!", "Не корректно введены параметры");
		return;
	}
}

void MainWindow::update_lable_hr() {
	if(ui->lineEdit_I->text() != "" && ui->lineEdit_R->text() != 0) {
		double R = l.toDouble(ui->lineEdit_R->text());
		int I = ui->lineEdit_I->text().toInt();
		ui->label_hr->setText(QString::number(R / I));
	}
	else
		ui->label_hr->setText("NaN");
}
void MainWindow::update_lable_ht() {
	if(ui->lineEdit_K->text() != "" && ui->lineEdit_T->text() != 0) {
		double T = l.toDouble(ui->lineEdit_T->text());
		int K = ui->lineEdit_K->text().toInt();
		ui->label_ht->setText(QString::number(T / K));
	}
	else
		ui->label_ht->setText("NaN");
}

bool MainWindow::check_inruts()
{
	if(ui->lineEdit_I->text() == "") return false;
	if(ui->lineEdit_K->text() == "") return false;

	if(ui->lineEdit_R->text() == "") return false;
	if(l.toDouble(ui->lineEdit_R->text()) == 0) return false;
	if(ui->lineEdit_k->text() == "") return false;
	if(l.toDouble(ui->lineEdit_k->text()) == 0) return false;
	if(ui->lineEdit_l->text() == "") return false;
	if(l.toDouble(ui->lineEdit_l->text()) == 0) return false;
	if(ui->lineEdit_c->text() == "") return false;
	if(l.toDouble(ui->lineEdit_c->text()) == 0) return false;
	if(ui->lineEdit_alpha->text() == "") return false;
	if(ui->lineEdit_T->text() == "") return false;
	if(l.toDouble(ui->lineEdit_T->text()) == 0) return false;

	if(ui->listWidget_r->count() == 0 && ui->listWidget_t->count() == 0) return false;
	return true;
}
void MainWindow::on_lineEdit_I_textChanged(const QString &arg1)
{
	update_lable_hr();
}


void MainWindow::on_lineEdit_K_textChanged(const QString &arg1)
{
	update_lable_ht();
}


void MainWindow::on_lineEdit_R_textChanged(const QString &arg1)
{
	update_lable_hr();
}


void MainWindow::on_lineEdit_T_textChanged(const QString &arg1)
{
	update_lable_ht();
}

