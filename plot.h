#ifndef PLOT_H
#define PLOT_H

#include <QGLWidget>
#include <complex>
#include <QMouseEvent>
#include <QPair>
#include <QVector>

class Plot : public QGLWidget
{
    Q_OBJECT

public:
    Plot(double x = 0, double y = 0, double scale = 1, QWidget *parent = nullptr);
    Plot(QWidget *parent = nullptr);

    void Reboot();
    void addComplexGraphics(std::complex<double> (*f)(double), double start, double end, double step);
    void addComplexGraphics(std::complex<double>* f, int N, double start, double end);
    void addRealGraphics(double (*f)(double), double start, double end, double step);
    void addRealGraphics(double *f, int N, double start, double end);
    void deleteComplexGraphics(int index);
    void deleteRealGraphics(int index);
    void turnedCircle(bool TurnedON);
    void showGrid(bool on);
	void ClearAll();
	void SetPositin(QRect rect);
	void AllowMove(bool allow);

public slots:
    void Move(QPointF offset);
    void Zoom(double zoom, QPointF mouse);

signals:
    void MoveSig(QPointF offset);
    void ZoomSig(double zoom, QPointF mouse);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent* pe) override;
    void mouseMoveEvent(QMouseEvent* pe) override;
    void wheelEvent(QWheelEvent* pe) override;

    void drawGrid();
    void drawAxis();
    void drawComGrpg(int index);
    void drawRealGraph(int index);
    void drawCircle();
private:
    QSize win;          // размер окна отрисовки
    QRectF rec;         // прямоугоник в плоскости, который отображается
    QPoint mouse;
    bool isDrawCircle = false;
    bool isDrawGrid = false;
	bool isMoving = true;

    double zoomX();
    double zoomY();

    QVector<QVector<QPair<double, std::complex<double>>>> complexGraphics;
    QVector<QVector<QPair<double, double>>> realGraphics;
};

#endif // PLOT_H
