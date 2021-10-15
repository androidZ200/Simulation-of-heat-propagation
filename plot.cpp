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
	setDefaultPlaneStyle();
	setDefaultGraphicStyle();
}

void Plot::StartPosition()
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

	complexGraphics.push_back({graph, defGraphStyle});
    update();
}

void Plot::addComplexGraphics(std::complex<double> *f, int N, double start, double end)
{
    QVector<QPair<double, std::complex<double>>> graph;
    double h = (end - start) / (N - 1);
    for(int i = 0; i < N; ++i)
        graph.push_back(QPair<double, std::complex<double>>(start + h*i, f[i]));

	complexGraphics.push_back({graph, defGraphStyle});
    update();
}

void Plot::addRealGraphics(double (*f)(double), double start, double end, double step)
{
    QVector<QPair<double, double>> graph;

    while(start <= end) {
        graph.push_back(QPair<double, double>(start, f(start)));
        start += step;
    }

	realGraphics.push_back({graph, defGraphStyle});
    update();
}

void Plot::addRealGraphics(double *f, int N, double start, double end)
{
    QVector<QPair<double, double>> graph;
    double h = (end - start) / (N - 1);
    for(int i = 0; i < N; ++i)
        graph.push_back(QPair<double, double>(start + h*i, f[i]));

	realGraphics.push_back({graph, defGraphStyle});
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

int Plot::countRealGraphics() const
{
	return realGraphics.size();
}

int Plot::countComplexGraphics() const
{
	return complexGraphics.size();
}

void Plot::setComplexGraphicStyle(GlGraphicStyle style, int index)
{
	complexGraphics[index].style = style;
}

void Plot::setRealGraphicStyle(GlGraphicStyle style, int index)
{
	realGraphics[index].style = style;
}

GlGraphicStyle Plot::getComplexGraphicStyle(int index) const
{
	return complexGraphics[index].style;
}

GlGraphicStyle Plot::getRealGraphicStyle(int index) const
{
	return realGraphics[index].style;
}

GlGraphicStyle& Plot::ComplexGraphicStyle(int index)
{
	return complexGraphics[index].style;
}

GlGraphicStyle& Plot::RealGraphicStyle(int index)
{
	return realGraphics[index].style;
}

void Plot::ClearAll()
{
	complexGraphics.clear();
	realGraphics.clear();
}

void Plot::SetPositin(QRectF rect)
{
	rec = rect;
}

GLPlaneStyle Plot::getStylePlane() const
{
	return plStyle;
}

GLPlaneStyle& Plot::StylePlane()
{
	return plStyle;
}

void Plot::setStylePlane(GLPlaneStyle style)
{
	plStyle = style;
	updateBackground();
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
	updateBackground();
	glDisable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
}

void Plot::resizeGL(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    win.setWidth(w);
    win.setHeight(h);

	if(plStyle.resize_style == GLPlaneStyle::ResizeStyle::fixed_zoom) {
		double dw = (w - win.width()) / 2.0 / zoomX();
		double dh = (h - win.height()) / 2.0 / zoomY();

		rec.setLeft(rec.left() - dw);
		rec.setRight(rec.right() + dw);
		rec.setTop(rec.top() + dh);
		rec.setBottom(rec.bottom() - dh);
	}

    glOrtho(rec.left(), rec.right(), rec.bottom(), rec.top(), -1, 1);
    glViewport(0, 0, (GLint)w, (GLint)h);
}

void Plot::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(rec.left(), rec.right(), rec.bottom(), rec.top(), -1, 1);

	if(plStyle.show_grid) drawGrid();
	if(plStyle.show_axis) drawAxis();
	for(int i = 0; i < complexGraphics.size(); ++i)
		if(!complexGraphics[i].style.hiden) drawComGrpg(i);
	for(int i = 0; i < realGraphics.size(); ++i)
		if(!realGraphics[i].style.hiden) drawRealGraph(i);
	if(plStyle.show_circle) drawCircle();
}

void Plot::mousePressEvent(QMouseEvent *pe)
{
    mouse = pe->pos();
}

void Plot::mouseMoveEvent(QMouseEvent *pe)
{
	if(!plStyle.is_moving) return;
    QPointF diff = pe->pos() - mouse;
    diff.setX(-diff.x() / zoomX());
    diff.setY(diff.y() / zoomY());
    Move(diff);
    mouse = pe->pos();
    emit MoveSig(diff);
}

void Plot::wheelEvent(QWheelEvent *pe)
{
	if(!plStyle.is_moving) return;
	double t;
	if(pe->angleDelta().y() > 0) t = 1.1;
	else if(pe->angleDelta().y() < 0) t = 1.1;

    QPointF m = rec.topLeft();
    m.setX(m.x() + pe->position().x() * rec.width() / win.width());
    m.setY(m.y() + pe->position().y() * rec.height() / win.height());

    Zoom(t, m);
    emit ZoomSig(t, m);
}

void Plot::drawGrid()
{
    glLineWidth(0.8f);
	QColor& col = plStyle.color_grid;
	glColor4f(col.red() / 255.0f, col.green() / 255.0f, col.blue() / 255.0f, 1.0f);

    glBegin(GL_LINES);
	for(float i = 0; i < rec.right(); i += plStyle.steps_grid.width()) {
        glVertex3f( i,   rec.top(), 0.0f);
        glVertex3f( i,  rec.bottom(), 0.0f);
	}
	for(float i = 0; i > rec.left(); i -= plStyle.steps_grid.width()) {
		glVertex3f( i,   rec.top(), 0.0f);
		glVertex3f( i,  rec.bottom(), 0.0f);
	}
	for(float i = 0; i < rec.top(); i += plStyle.steps_grid.height()) {
        glVertex3f(rec.left(),  i,  0.0f);
        glVertex3f(rec.right(),  i,  0.0f);
    }
	for(float i = 0; i > rec.bottom(); i -= plStyle.steps_grid.height()) {
		glVertex3f(rec.left(),  i,  0.0f);
		glVertex3f(rec.right(),  i,  0.0f);
	}
    glEnd();
}

void Plot::drawAxis()
{
	glLineWidth(plStyle.weight_axis);
	QColor& col = plStyle.color_axis;
	glColor4f(col.red() / 255.0f, col.green() / 255.0f, col.blue() / 255.0f, 1.0f);
    glBegin(GL_LINES);
        glVertex3f(rec.left(),  0.0f,  0.0f);
        glVertex3f(rec.right(),  0.0f,  0.0f);
        glVertex3f( 0.0f,   rec.top(), 0.0f);
        glVertex3f( 0.0f,  rec.bottom(), 0.0f);
    glEnd();
}

void Plot::drawComGrpg(int index)
{
	glLineWidth(complexGraphics[index].style.weight);
    QColor col;
    glBegin(GL_LINE_STRIP);
	for(auto &x : complexGraphics[index].graphic) {
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
	glLineWidth(realGraphics[index].style.weight);
	QColor& col = realGraphics[index].style.color;
	glColor4f(col.red() / 255.0f, col.green() / 255.0f, col.blue() / 255.0f, 1.0f);
    glBegin(GL_LINE_STRIP);
	for(auto &x : realGraphics[index].graphic) {
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

void Plot::updateBackground()
{
	QColor &col = plStyle.background;
	glClearColor(col.red() / 255.0f, col.green() / 255.0f, col.blue() / 255.0f, 1.0f);
}

void Plot::setDefaultPlaneStyle()
{
	plStyle.background = Qt::GlobalColor::white;
	plStyle.color_axis = Qt::GlobalColor::black;
	plStyle.color_grid = QColor::fromRgb(200, 200, 200);

	plStyle.show_axis = true;
	plStyle.show_circle = false;
	plStyle.show_grid = false;
	plStyle.steps_grid = QSize(1, 1);
	plStyle.weight_axis = 2.0f;

	plStyle.is_moving = true;
	plStyle.resize_style = GLPlaneStyle::ResizeStyle::fixed_zoom;
}

void Plot::setDefaultGraphicStyle()
{
	defGraphStyle.color = QColor::fromRgb(100, 0, 0);
	defGraphStyle.weight = 1.0f;
	defGraphStyle.hiden = false;
}

double Plot::zoomX()
{
    return abs(win.width() / rec.width());
}

double Plot::zoomY()
{
    return abs(win.height() / rec.height());
}
