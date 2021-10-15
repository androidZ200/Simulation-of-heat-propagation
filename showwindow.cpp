#include "showwindow.h"
#include "ui_showwindow.h"

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
	redrawgraphics();
	connect(ui->radioButton_r, &QRadioButton::clicked, this, &ShowWindow::updateList);
	connect(ui->radioButton_t, &QRadioButton::clicked, this, &ShowWindow::updateList);
	ui->openGLWidget->showGrid(true);
	ui->openGLWidget->AllowMove(false);
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
	if(ui->radioButton_r->isChecked()) {
		for(int i = 0; i < hor_coll->size(); i++)
			ui->listWidget->addItem("t = " + QString::number((*hor_coll)[i].getT()));
	}
	else {
		for(int i = 0; i < vert_coll->size(); i++)
			ui->listWidget->addItem("r = " + QString::number((*vert_coll)[i].getR()));
	}
	setCheckedItems();
	redrawgraphics();
}
void ShowWindow::redrawgraphics()
{
	ui->openGLWidget->ClearAll();
	if(ui->radioButton_r->isChecked()) {
		for(int i = 0; i < hor_coll->size(); i++)
			if(ui->listWidget->item(i)->checkState())
				ui->openGLWidget->addRealGraphics((*hor_coll)[i].getArray(), I+1, 0, R);
	}
	else {
		for(int i = 0; i < vert_coll->size(); i++)
			if(ui->listWidget->item(i)->checkState())
				ui->openGLWidget->addRealGraphics((*vert_coll)[i].getArray(), K+1, 0, T/50);
	}
	ui->openGLWidget->update();
}



void ShowWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
	redrawgraphics();
}

