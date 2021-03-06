#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "Schemes/explictschema.h"
#include "Schemes/implicitschema.h"
#include "Schemes/cranknicholsonscheme.h"
#include <cmath>

QLocale lang(QLocale::German);

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



	connect(ui->lineEdit_I, &QLineEdit::textChanged, this, &MainWindow::checkInputs);
	connect(ui->lineEdit_K, &QLineEdit::textChanged, this, &MainWindow::checkInputs);
	connect(ui->lineEdit_R, &QLineEdit::textChanged, this, &MainWindow::checkInputs);
	connect(ui->lineEdit_T, &QLineEdit::textChanged, this, &MainWindow::checkInputs);
	connect(ui->lineEdit_k, &QLineEdit::textChanged, this, &MainWindow::checkInputs);
	connect(ui->lineEdit_l, &QLineEdit::textChanged, this, &MainWindow::checkInputs);
	connect(ui->lineEdit_c, &QLineEdit::textChanged, this, &MainWindow::checkInputs);
	connect(ui->lineEdit_alpha, &QLineEdit::textChanged, this, &MainWindow::checkInputs);

	connect(ui->pushButton_add_r, &QPushButton::clicked, this, &MainWindow::checkInputs);
	connect(ui->pushButton_add_t, &QPushButton::clicked, this, &MainWindow::checkInputs);
	connect(ui->pushButton_remove_r, &QPushButton::clicked, this, &MainWindow::checkInputs);
	connect(ui->pushButton_remove_t, &QPushButton::clicked, this, &MainWindow::checkInputs);
	connect(ui->pushButton_clear, &QPushButton::clicked, this, &MainWindow::checkInputs);
	checkInputs();
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
		item->setData(Qt::DisplayRole, lang.toDouble(ui->lineEdit_new_r->text()));
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
		item->setData(Qt::DisplayRole, lang.toDouble(ui->lineEdit_new_t->text()));
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
	if(!check_steadiness()) {
		int ret = QMessageBox::warning(this, "????????????????", "?????????? ???? ??????????????????, ???????????????????? ?????????????????????",
							 QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No);
		if(ret == QMessageBox::StandardButton::No) return;
	}
	ISolver* sol;
	QVector<HorizontalCollector>* hor_coll = new QVector<HorizontalCollector>;
	QVector<VerticalCollector>* vert_coll = new QVector<VerticalCollector>;
	for(int i = 0; i < ui->listWidget_t->count(); i++) {
		double t = lang.toDouble(ui->listWidget_t->item(i)->text());
		if(t <= lang.toDouble(ui->lineEdit_T->text()))
			hor_coll->push_back(HorizontalCollector(t));
	}
	for(int i = 0; i < ui->listWidget_r->count(); i++) {
		double r = lang.toDouble(ui->listWidget_r->item(i)->text());
		if(r <= lang.toDouble(ui->lineEdit_R->text()))
			vert_coll->push_back(VerticalCollector(r));
	}

	if(ui->radioButton_1->isChecked())
		sol = new ExplictSchema();
	else if(ui->radioButton_2->isChecked())
		sol = new ImplicitSchema();
	else if(ui->radioButton_3->isChecked())
		sol = new CrankNicholsonScheme();

	sol->setHorCollectors(hor_coll);
	sol->setVertCollectors(vert_coll);
	sol->Init(lang.toDouble(ui->lineEdit_R->text()),
			  lang.toDouble(ui->lineEdit_T->text()),
			  lang.toDouble(ui->lineEdit_l->text()),
			  lang.toDouble(ui->lineEdit_k->text()),
			  lang.toDouble(ui->lineEdit_c->text()),
			  lang.toDouble(ui->lineEdit_alpha->text()),
			  ui->lineEdit_I->text().toInt(),
			  ui->lineEdit_K->text().toInt());
	R = lang.toDouble(ui->lineEdit_R->text());
	sol->setBeginingLayer(psi);
	LoadWindow* loading = new LoadWindow(sol);
	loading->show();
}

void MainWindow::update_lable_hr() {
	if(ui->lineEdit_I->text() != "" && ui->lineEdit_R->text() != 0) {
		double R = lang.toDouble(ui->lineEdit_R->text());
		int I = ui->lineEdit_I->text().toInt();
		ui->label_hr->setText(QString::number(R / I));
	}
	else
		ui->label_hr->setText("NaN");
}
void MainWindow::update_lable_ht() {
	if(ui->lineEdit_K->text() != "" && ui->lineEdit_T->text() != 0) {
		double T = lang.toDouble(ui->lineEdit_T->text());
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
	if(lang.toDouble(ui->lineEdit_R->text()) == 0) return false;
	if(ui->lineEdit_k->text() == "") return false;
	if(ui->lineEdit_l->text() == "") return false;
	if(lang.toDouble(ui->lineEdit_l->text()) == 0) return false;
	if(ui->lineEdit_c->text() == "") return false;
	if(lang.toDouble(ui->lineEdit_c->text()) == 0) return false;
	if(ui->lineEdit_alpha->text() == "") return false;
	if(ui->lineEdit_T->text() == "") return false;
	if(lang.toDouble(ui->lineEdit_T->text()) == 0) return false;

	if(!check_list_points()) return false;
	return true;
}

bool MainWindow::check_steadiness()
{
	if(ui->radioButton_2->isChecked()) return true;
	double k = lang.toDouble(ui->lineEdit_k->text());
	double c = lang.toDouble(ui->lineEdit_c->text());
	double l = lang.toDouble(ui->lineEdit_l->text());
	double alpha = lang.toDouble(ui->lineEdit_alpha->text());
	double hr = lang.toDouble(ui->lineEdit_R->text()) / lang.toInt(ui->lineEdit_I->text());
	double ht = lang.toDouble(ui->lineEdit_T->text()) / lang.toInt(ui->lineEdit_K->text());
	double gama = k*ht/c/hr/hr;
	double beta = 2*alpha*ht/c/l;

	if(ui->radioButton_1->isChecked())
		return 1 - 4*gama - beta >= 0;
	else
		return 1 - 2*gama - beta/2 >= 0;
}

bool MainWindow::check_list_points()
{
	double R = lang.toDouble(ui->lineEdit_R->text());
	for(int i = 0; i < ui->listWidget_r->count(); i++) {
		auto item = ui->listWidget_r->item(i);
		if(lang.toDouble(item->text()) <= R) return true;
	}

	double T = lang.toDouble(ui->lineEdit_T->text());
	for(int i = 0; i < ui->listWidget_t->count(); i++) {
		auto item = ui->listWidget_t->item(i);
		if(lang.toDouble(item->text()) <= T) return true;
	}

	return false;
}

void MainWindow::checkListR()
{

	double R = lang.toDouble(ui->lineEdit_R->text());
	for(int i = 0; i < ui->listWidget_r->count(); i++) {
		auto item = ui->listWidget_r->item(i);
		if(lang.toDouble(item->text()) > R)
			item->setTextColor(Qt::GlobalColor::red);
		else
			item->setTextColor(QApplication::palette().text().color());


	}
}
void MainWindow::checkListT()
{
	double T = lang.toDouble(ui->lineEdit_T->text());
	for(int i = 0; i < ui->listWidget_t->count(); i++) {
		auto item = ui->listWidget_t->item(i);
		if(lang.toDouble(item->text()) > T)
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

void MainWindow::checkInputs()
{
	ui->pushButton_start->setEnabled(check_inruts());
}

