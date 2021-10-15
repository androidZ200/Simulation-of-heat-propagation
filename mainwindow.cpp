#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "explictschema.h"
#include "implicitschema.h"
#include <cmath>

QLocale l(QLocale::German);

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	ui->lineEdit_I->setValidator(new QIntValidator(1, 1<<30, this));
	ui->lineEdit_K->setValidator(new QIntValidator(1, 1<<30, this));

	ui->lineEdit_R->setValidator(new QDoubleValidator(1e-9, 1e9, 9, this));
	ui->lineEdit_k->setValidator(new QDoubleValidator(0, 1e9, 9, this));
	ui->lineEdit_l->setValidator(new QDoubleValidator(1e-9, 1e9, 9, this));
	ui->lineEdit_c->setValidator(new QDoubleValidator(1e-9, 1e9, 9, this));
	ui->lineEdit_alpha->setValidator(new QDoubleValidator(-1e9, 1e9, 9, this));
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
		QListWidgetItem *item = new QListWidgetItem();
		item->setData(Qt::DisplayRole, l.toDouble(ui->lineEdit_new_r->text()));
		ui->listWidget_r->addItem(item);
	}
	ui->listWidget_r->sortItems();
	checkListR();
}


void MainWindow::on_pushButton_add_t_clicked()
{
	if(ui->lineEdit_new_t->text() != "") {
		for(int i = 0; i < ui->listWidget_t->count(); i++)
			if(ui->listWidget_t->item(i)->text() == ui->lineEdit_new_t->text())
				return;
		QListWidgetItem *item = new QListWidgetItem();
		item->setData(Qt::DisplayRole, l.toDouble(ui->lineEdit_new_t->text()));
		ui->listWidget_t->addItem(item);
	}
	ui->listWidget_t->sortItems();
	checkListT();
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

	ui->lineEdit_I->setText("4000");
	ui->lineEdit_K->setText("6000");

	ui->lineEdit_R->setText("5");
	ui->lineEdit_k->setText("0,01");
	ui->lineEdit_l->setText("1");
	ui->lineEdit_c->setText("1,84");
	ui->lineEdit_alpha->setText("0,001");
	ui->lineEdit_T->setText("150");

	ui->lineEdit_new_r->clear();
	ui->lineEdit_new_t->clear();
}

double R;
double psi(double r) {
	return std::cyl_bessel_j(0, r*2.4048255/R);
}
void MainWindow::on_pushButton_start_clicked()
{
	if(!check_inruts()) {
		QMessageBox::warning(this, "Внимание!", "Не корректно введены параметры");
		return;
	}
	ISolver* sol;
	QVector<HorizontalCollector>* hor_coll = new QVector<HorizontalCollector>;
	QVector<VerticalCollector>* vert_coll = new QVector<VerticalCollector>;
	for(int i = 0; i < ui->listWidget_t->count(); i++) {
		double t = l.toDouble(ui->listWidget_t->item(i)->text());
		if(t <= l.toDouble(ui->lineEdit_T->text()))
			hor_coll->push_back(HorizontalCollector(t));
	}
	for(int i = 0; i < ui->listWidget_r->count(); i++) {
		double r = l.toDouble(ui->listWidget_r->item(i)->text());
		if(r <= l.toDouble(ui->lineEdit_R->text()))
			vert_coll->push_back(VerticalCollector(r));
	}

	if(ui->radioButton_1->isChecked())
		sol = new ExplictSchema();
	else if(ui->radioButton_2->isChecked())
		sol = new ImplicitSchema();

	sol->setHorCollectors(hor_coll);
	sol->setVertCollectors(vert_coll);
	sol->Init(l.toDouble(ui->lineEdit_R->text()),
			  l.toDouble(ui->lineEdit_T->text()),
			  l.toDouble(ui->lineEdit_l->text()),
			  l.toDouble(ui->lineEdit_k->text()),
			  l.toDouble(ui->lineEdit_c->text()),
			  l.toDouble(ui->lineEdit_alpha->text()),
			  ui->lineEdit_I->text().toInt(),
			  ui->lineEdit_K->text().toInt());
	R = l.toDouble(ui->lineEdit_R->text());
	sol->setBeginingLayer(psi);
	LoadWindow* loading = new LoadWindow(sol);
	loading->show();
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

void MainWindow::checkListR()
{

	double R = l.toDouble(ui->lineEdit_R->text());
	for(int i = 0; i < ui->listWidget_r->count(); i++) {
		auto item = ui->listWidget_r->item(i);
		if(l.toDouble(item->text()) > R)
			item->setTextColor(Qt::GlobalColor::red);
		else
			item->setTextColor(QApplication::palette().text().color());


	}
}
void MainWindow::checkListT()
{
	double T = l.toDouble(ui->lineEdit_T->text());
	for(int i = 0; i < ui->listWidget_t->count(); i++) {
		auto item = ui->listWidget_t->item(i);
		if(l.toDouble(item->text()) > T)
			item->setTextColor(Qt::GlobalColor::red);
		else
			item->setTextColor(QApplication::palette().text().color());
	}
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
	checkListR();
}


void MainWindow::on_lineEdit_T_textChanged(const QString &arg1)
{
	update_lable_ht();
	checkListT();
}


void MainWindow::on_lineEdit_new_r_returnPressed()
{
	ui->pushButton_add_r->clicked();
}


void MainWindow::on_lineEdit_new_t_returnPressed()
{
	ui->pushButton_add_t->clicked();
}

