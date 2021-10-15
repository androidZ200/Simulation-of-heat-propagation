#include "loadwindow.h"
#include "ui_loadwindow.h"


LoadWindow::LoadWindow(ISolver* solver, QWidget *parent) :
	QWidget(parent),
	ui(new Ui::loadwindow)
{
	this->solver = solver;
	hor_coll = solver->getHorizontalCollectors();
	vert_coll = solver->getVerticalCollectors();
	ui->setupUi(this);

	solver->moveToThread(&thread);
	connect(solver, &ISolver::Finish, this, &LoadWindow::GetResult);
	connect(solver, &ISolver::Process, this, &LoadWindow::Progress);
	connect(&thread, &QThread::started, solver, &ISolver::Start);

	thread.start();
}

LoadWindow::~LoadWindow()
{
	delete ui;
}

void LoadWindow::on_pushButton_cancel_clicked()
{
	solver->Stop();
	solver->deleteLater();
	delete hor_coll;
	delete vert_coll;
	this->close();
}

void LoadWindow::GetResult()
{
	ShowWindow *win = new ShowWindow(solver->getI(), solver->getK(),
			solver->getR(), solver->getT(), hor_coll, vert_coll);
	solver->deleteLater();
	win->show();
	this->close();
}

void LoadWindow::Progress(double percent)
{
	ui->progressBar->setValue(percent);
}


void LoadWindow::on_pushButton_start_stop_clicked()
{
	if(isStarting) {
		isStarting = false;
		solver->Stop();
		ui->pushButton_start_stop->setText("Старт");
	}
	else {
		isStarting = true;
		QMetaObject::invokeMethod(solver, "Start");
		ui->pushButton_start_stop->setText("Пауза");
	}
}

