#include "loadwindow.h"
#include "ui_loadwindow.h"
#include <QMessageBox>

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

bool LoadWindow::on_pushButton_cancel_clicked()
{
	if(QMessageBox::warning(this, "Внимание", "При закрытие, результаты будут потеряны.\n Продолжить?",
							QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No) == QMessageBox::StandardButton::Yes) {
		isStarting = false;
		solver->Stop();
		solver->deleteLater();
		delete hor_coll;
		delete vert_coll;
		this->close();
		return true;
	}
	return false;
}

void LoadWindow::GetResult()
{
	isStarting = false;
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

void LoadWindow::closeEvent(QCloseEvent* event)
{
	if(isStarting && !on_pushButton_cancel_clicked())
		event->ignore();
}

