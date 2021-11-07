#include "showwindow.h"
#include "ui_showwindow.h"
#include <QFile>
#include <QMessageBox>

ShowWindow::ShowWindow(int I, int K, double R, double T, QVector<HorizontalCollector> *hor_coll,
				QVector<VerticalCollector> *vert_coll, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ShowWindow)
{
	ui->setupUi(this);

	this->hor_coll = hor_coll;
	this->vert_coll = vert_coll;
	this->I = I;
	this->K = K;
	this->R = R;
	this->T = T;

	if(vert_coll->size() == 0) ui->radioButton_t->setEnabled(false);
	else if(hor_coll->size() == 0) {
		ui->radioButton_t->setChecked(true);
		ui->radioButton_r->setEnabled(false);
	}
	updateList();
	connect(ui->radioButton_r, &QRadioButton::clicked, this, &ShowWindow::updateList);
	connect(ui->radioButton_t, &QRadioButton::clicked, this, &ShowWindow::updateList);
	ui->openGLWidget->StylePlane().show_grid = true;
	ui->openGLWidget->StylePlane().is_moving = false;
	ui->openGLWidget->StylePlane().Y_name = "u";
	ui->openGLWidget->StylePlane().show_coord = true;

	ui->lineEdit->setValidator(new QDoubleValidator(0, 1e9, 9, this));
}

ShowWindow::~ShowWindow()
{
	delete ui;
	delete hor_coll;
	delete vert_coll;
}

void ShowWindow::setCheckedItems()
{
	for(int i = 0; i < ui->listWidget->count(); i++) {
		auto item = ui->listWidget->item(i);
		item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
		item->setCheckState(Qt::Checked);
	}
}

void ShowWindow::updateList()
{
	ui->listWidget->clear();
	ui->openGLWidget->ClearAll();
	if(ui->radioButton_r->isChecked()) {
		ui->openGLWidget->SetPositin(QRectF(-R*0.1, 1.1, R*1.2, -1.2));
		for(int i = 0; i < hor_coll->size(); i++)
			ui->listWidget->addItem("t = " + QString::number((*hor_coll)[i].get_point()));
		for(int i = 0; i < hor_coll->size(); i++)
			ui->openGLWidget->addGraphics((*hor_coll)[i].get_array(), I+1, 0, R);
		setStepHorizontalGrid(R);
		ui->label->setText("r = ");
		ui->openGLWidget->StylePlane().X_name = "r";
	}
	else {
		ui->openGLWidget->SetPositin(QRectF(-T*0.1, 1.1, T*1.2, -1.2));
		for(int i = 0; i < vert_coll->size(); i++)
			ui->listWidget->addItem("r = " + QString::number((*vert_coll)[i].get_point()));
		for(int i = 0; i < vert_coll->size(); i++)
			ui->openGLWidget->addGraphics((*vert_coll)[i].get_array(), K+1, 0, T);
		setStepHorizontalGrid(T);
		ui->label->setText("t = ");
		ui->openGLWidget->StylePlane().X_name = "t";
	}
	setCheckedItems();
	redrawgraphics();
	on_lineEdit_textChanged(ui->lineEdit->text());
}
void ShowWindow::redrawgraphics()
{
	auto s_items = ui->listWidget->selectedItems();
	for(int i = 0; i < ui->listWidget->count(); i++) {
		if(ui->listWidget->item(i)->checkState())
			ui->openGLWidget->graphicStyle(i).hiden = false;
		else
			ui->openGLWidget->graphicStyle(i).hiden = true;
		if(s_items.contains(ui->listWidget->item(i))) {
			ui->openGLWidget->graphicStyle(i).color = QColor::fromRgb(0, 0, 180);
			ui->openGLWidget->graphicStyle(i).weight = 2.0f;
		}
		else {
			ui->openGLWidget->graphicStyle(i).color = QColor::fromRgb(100, 0, 0);
			ui->openGLWidget->graphicStyle(i).weight = 1.0f;
		}
	}
	ui->openGLWidget->update();
}

void ShowWindow::setStepHorizontalGrid(double X)
{
	double x = 1;
	while(x*20 < X) x *= 10;
	ui->openGLWidget->StylePlane().steps_grid = QSizeF(x, 0.1);
}



void ShowWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
	redrawgraphics();
	on_lineEdit_textChanged(ui->lineEdit->text());
}


void ShowWindow::on_pushButton_save_clicked()
{
	auto s_items = ui->listWidget->selectedItems();
	for(int i = 0; i < ui->listWidget->count(); i++) {
		if(s_items.contains(ui->listWidget->item(i))) {
			QFile file("bin data " + ui->listWidget->item(i)->text() + ".dat");
			file.open(QIODevice::WriteOnly);
			QDataStream out(&file);
			if(ui->radioButton_r->isChecked()) {
				out << I+1 << R;
				out.writeBytes((char*)(*hor_coll)[i].get_array(), (I+1)*sizeof(double));
			}
			else {
				out << K+1 << T;
				out.writeBytes((char*)(*vert_coll)[i].get_array(), (K+1)*sizeof(double));
			}
			QMessageBox msg;
			msg.setText("Graphic has saved");
			msg.exec();
		}
	}
}


void ShowWindow::on_lineEdit_textChanged(const QString &arg1)
{
	QLocale lang(QLocale::German);
	if(ui->listWidget->selectedItems().size() == 0 || ui->lineEdit->text() == "") {
		ui->label_2->setText("u = NaN");
		return;
	}
	if(ui->radioButton_r->isChecked()) {

		double r = lang.toDouble(ui->lineEdit->text());
		if(r < 0 || r > R)
			ui->label_2->setText("u = NaN");
		else ui->label_2->setText("u = " + QString::number(Value(getArray(), I, r/R)));
	}
	else {

		double t = lang.toDouble(ui->lineEdit->text());
		if(t < 0 || t > T)
			ui->label_2->setText("u = NaN");
		else ui->label_2->setText("u = " + QString::number(Value(getArray(), K, t/T)));
	}
}

double ShowWindow::Value(const double* arr, int N, double x)
{
	return arr[(int)(N*x)];
}

const double* ShowWindow::getArray()
{
	auto items = ui->listWidget->selectedItems();
	if (items.size() == 0) return 0;
	if(ui->radioButton_r->isChecked()) {
		for(int i = 0; i < hor_coll->size(); i++)
			if(ui->listWidget->item(i) == items[0])
				return (*hor_coll)[i].get_array();
	}
	else {
		for(int i = 0; i < vert_coll->size(); i++)
			if(ui->listWidget->item(i) == items[0])
				return (*vert_coll)[i].get_array();
	}
	return 0;
}

