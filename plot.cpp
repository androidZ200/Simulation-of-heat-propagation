#include "plot.h"
#include <qmath.h>

const double pi = acos(-1);

Plot::Plot(double x, double y, double scale, QWidget *parent)
    : QGLWidget(parent)
{
    rec = QRectF(-3+x, 2+y, 6/scale, -4/scale);
    win = QSize(900, 600);

    QGLFormat format;
    format.setSamples(4);
    this->setFormat(format);
}

Plot::Plot(QWidget *parent) : Plot(0, 0, 1, parent)
{
}

void Plot::Reboot()
{
    rec = QRectF(-3, 2, 6, -4);

    update();
}

void Plot::addComplexGraphics(std::complex<double> (*f)(double), double start, double end, double step)
{
    QVector<QPair<double, std::complex<double>>> graph;

    while(start <= end) {
        graph.push_back(QPair<double, std::complex<double>>(start, f(start)));
        start += step;
    }

    complexGraphics.push_back(graph);
    update();
}

void Plot::addComplexGraphics(std::complex<double> *f, int N, double start, double end)
{
    QVector<QPair<double, std::complex<double>>> graph;
    double h = (end - start) / (N - 1);
    for(int i = 0; i < N; ++i)
        graph.push_back(QPair<double, std::complex<double>>(start + h*i, f[i]));

    complexGraphics.push_back(graph);
    update();
}

void Plot::addRealGraphics(double (*f)(double), double start, double end, double step)
{
    QVector<QPair<double, double>> graph;

    while(start <= end) {
        graph.push_back(QPair<double, double>(start, f(start)));
        start += step;
    }

    realGraphics.push_back(graph);
    update();
}

void Plot::addRealGraphics(double *f, int N, double start, double end)
{
    QVector<QPair<double, double>> graph;
    double h = (end - start) / (N - 1);
    for(int i = 0; i < N; ++i)
        graph.push_back(QPair<double, double>(start + h*i, f[i]));

    realGraphics.push_back(graph);
    update();
}

void Plot::deleteComplexGraphics(int index)
{
    if(index < complexGraphics.size() && index >= 0) complexGraphics.erase(complexGraphics.begin() + index);
}

void Plot::deleteRealGraphics(int index)
{
    if(index < realGraphics.size() && index >= 0) realGraphics.erase(realGraphics.begin() + index);
}

void Plot::turnedCircle(bool TurnedON)
{
    isDrawCircle = TurnedON;
}

void Plot::showGrid(bool on)
{
	isDrawGrid = on;
}

void Plot::ClearAll()
{
	complexGraphics.clear();
	realGraphics.clear();
}

void Plot::SetPositin(QRect rect)
{
	rec = rect;
}

void Plot::AllowMove(bool allow)
{
	isMoving = allow;
}

void Plot::Move(QPointF offset)
{
    rec.moveTopLeft(rec.topLeft() + offset);
    update();
}

void Plot::Zoom(double zoom, QPointF mouse)
{
    if(abs(mouse.x() * zoomX()) > 20) {
        rec.setLeft((rec.left() - mouse.x()) * zoom + mouse.x());
        rec.setRight((rec.right() - mouse.x()) * zoom + mouse.x());
    }

    if(abs(mouse.y() * zoomY()) > 20) {
        rec.setTop((rec.top() - mouse.y()) * zoom + mouse.y());
        rec.setBottom((rec.bottom() - mouse.y()) * zoom + mouse.y());
    }

    update();
}

void Plot::initializeGL()
{
    glClearColor(1, 1, 1, 1);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
}

void Plot::resizeGL(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    double dw = (w - win.width()) / 2.0 / zoomX();
    double dh = (h - win.height()) / 2.0 / zoomY();

    win.setWidth(w);
    win.setHeight(h);

    rec.setLeft(rec.left() - dw);
    rec.setRight(rec.right() + dw);
    rec.setTop(rec.top() + dh);
    rec.setBottom(rec.bottom() - dh);

    glOrtho(rec.left(), rec.right(), rec.bottom(), rec.top(), -1, 1);
    glViewport(0, 0, (GLint)w, (GLint)h);
}

void Plot::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(rec.left(), rec.right(), rec.bottom(), rec.top(), -1, 1);

    if(isDrawGrid) drawGrid();
    drawAxis();
    for(int i = 0; i < complexGraphics.size(); ++i) drawComGrpg(i);
    for(int i = 0; i < realGraphics.size(); ++i) drawRealGraph(i);
    if(isDrawCircle) drawCircle();
}

void Plot::mousePressEvent(QMouseEvent *pe)
{
    mouse = pe->pos();
}

void Plot::mouseMoveEvent(QMouseEvent *pe)
{
	if(!isMoving) return;
    QPointF diff = pe->pos() - mouse;
    diff.setX(-diff.x() / zoomX());
    diff.setY(diff.y() / zoomY());
    Move(diff);
    mouse = pe->pos();
    emit MoveSig(diff);
}

void Plot::wheelEvent(QWheelEvent *pe)
{
    double t = 1;
    if(pe->angleDelta().y() > 0) t /= 1.1;
    else if(pe->angleDelta().y() < 0) t *= 1.1;

    QPointF m = rec.topLeft();
    m.setX(m.x() + pe->position().x() * rec.width() / win.width());
    m.setY(m.y() + pe->position().y() * rec.height() / win.height());

    Zoom(t, m);
    emit ZoomSig(t, m);
}

void Plot::drawGrid()
{
    glLineWidth(0.8f);
    glColor4f(0.80f, 0.80f, 0.80f, 1.0f);

    glBegin(GL_LINES);
    for(int i = rec.left(); i < rec.right(); i++) {
        glVertex3f( i,   rec.top(), 0.0f);
        glVertex3f( i,  rec.bottom(), 0.0f);
    }

    for(int i = rec.bottom(); i < rec.top(); i++) {
        glVertex3f(rec.left(),  i,  0.0f);
        glVertex3f(rec.right(),  i,  0.0f);
    }
    glEnd();
}

void Plot::drawAxis()
{
    glLineWidth(1.0f);
    glColor4f(0.00f, 0.00f, 0.00f, 1.0f);
    glBegin(GL_LINES);
        glVertex3f(rec.left(),  0.0f,  0.0f);
        glVertex3f(rec.right(),  0.0f,  0.0f);
        glVertex3f( 0.0f,   rec.top(), 0.0f);
        glVertex3f( 0.0f,  rec.bottom(), 0.0f);
    glEnd();
}

void Plot::drawComGrpg(int index)
{
    glLineWidth(3.0f);
    QColor col;
    glBegin(GL_LINE_STRIP);
    for(auto &x : complexGraphics[index]) {
        double mod = sqrt(x.second.real() * x.second.real() + x.second.imag() * x.second.imag());
        double arg = atan2(x.second.imag(), x.second.real());
        col = QColor::fromHsv(arg / pi * 180 + 180, 255, 255);
        glColor4f(col.red() / 255.0, col.green() / 255.0, col.blue() / 255.0, 1);
        glVertex3f(x.first, mod, 0);
    }
    glEnd();
}

void Plot::drawRealGraph(int index)
{
    glLineWidth(1.0f);
    glColor4f(0.40f, 0.00f, 0.00f, 1.0f);
    glBegin(GL_LINE_STRIP);
    for(auto &x : realGraphics[index]) {
        glVertex3f(x.first, x.second, 0);
    }
    glEnd();
}

void Plot::drawCircle()
{
    const double A = 30;
    const double Shift = 1.1;
    glLineWidth(2.0f);
    QColor col;
    glBegin(GL_LINE_LOOP);
    for(int i = 0; i <= 360; ++i) {
        col = QColor::fromHsv((i + 180) % 360, 255, 255);
        glColor4f(col.red() / 255.0, col.green() / 255.0, col.blue() / 255.0, 1);
        glVertex3f(A*(cos(i * pi / 180) + Shift) / zoomX() + rec.left(),
                   A*(sin(i * pi / 180) - Shift) / zoomY() + rec.top(), 0);
    }
    glEnd();
}

double Plot::zoomX()
{
    return abs(win.width() / rec.width());
}

double Plot::zoomY()
{
    return abs(win.height() / rec.height());
}
