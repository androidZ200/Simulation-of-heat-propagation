#include "glplot.h"
#include <qmath.h>
#include <QSurfaceFormat>

GlPlot::GlPlot(double x, double y, double scale, QWidget *parent)
	: QOpenGLWidget(parent)
{
    rec = QRectF(-3+x, 2+y, 6/scale, -4/scale);
    win = QSize(900, 600);

	QSurfaceFormat format;
    format.setSamples(4);
	this->setFormat(format);
}

GlPlot::GlPlot(QRectF position, QWidget* parent)
	: QOpenGLWidget(parent)
{
	rec = position;
	win = QSize(900, 600);

	QSurfaceFormat format;
	format.setSamples(4);
	this->setFormat(format);
}

GlPlot::GlPlot(QWidget *parent)
	: GlPlot(0, 0, 1, parent)
{
	setDefaultPlaneStyle();
	setDefaultGraphicStyle();
}

GlPlot::~GlPlot()
{
}

void GlPlot::StartPosition()
{
    rec = QRectF(-3, 2, 6, -4);

    update();
}

void GlPlot::SetPositin(QRectF rect)
{
	rec = rect;
}


GLPlaneStyle GlPlot::getStylePlane() const
{
	return plStyle;
}


GLPlaneStyle& GlPlot::StylePlane()
{
	return plStyle;
}


void GlPlot::setStylePlane(GLPlaneStyle style)
{
	plStyle = style;
	updateBackground();
}


void GlPlot::Move(QPointF offset)
{
    rec.moveTopLeft(rec.topLeft() + offset);
    update();
}


void GlPlot::Zoom(double zoom, QPointF mouse)
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


void GlPlot::initializeGL()
{
	updateBackground();
	glDisable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
}


void GlPlot::resizeGL(int w, int h)
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


void GlPlot::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(rec.left(), rec.right(), rec.bottom(), rec.top(), -1, 1);

	if(plStyle.show_grid) drawGrid();
	if(plStyle.show_axis) drawAxis();
	for(int i = 0; i < countGraphics(); ++i)
		drawGrpg(i);

}


void GlPlot::mousePressEvent(QMouseEvent *pe)
{
    mouse = pe->pos();
}


void GlPlot::mouseMoveEvent(QMouseEvent *pe)
{
	if(!plStyle.is_moving) return;
    QPointF diff = pe->pos() - mouse;
    diff.setX(-diff.x() / zoomX());
    diff.setY(diff.y() / zoomY());
    Move(diff);
    mouse = pe->pos();
	//emit MoveSig(diff);
	MoveSig(diff);
}


void GlPlot::wheelEvent(QWheelEvent *pe)
{
	if(!plStyle.is_moving) return;
	double t;
	if(pe->angleDelta().y() > 0) t = 1.1;
	else if(pe->angleDelta().y() < 0) t = 1.1;

    QPointF m = rec.topLeft();
    m.setX(m.x() + pe->position().x() * rec.width() / win.width());
    m.setY(m.y() + pe->position().y() * rec.height() / win.height());

    Zoom(t, m);
	//emit ZoomSig(t, m);
	ZoomSig(t, m);
}


void GlPlot::drawGrid()
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


void GlPlot::drawAxis()
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


void GlPlot::updateBackground()
{
	QColor &col = plStyle.background;
	glClearColor(col.red() / 255.0f, col.green() / 255.0f, col.blue() / 255.0f, 1.0f);
}


void GlPlot::setDefaultPlaneStyle()
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


void GlPlot::setDefaultGraphicStyle()
{
	defGraphStyle.color = QColor::fromRgb(100, 0, 0);
	defGraphStyle.weight = 1.0f;
	defGraphStyle.hiden = false;
}


double GlPlot::zoomX()
{
    return abs(win.width() / rec.width());
}


double GlPlot::zoomY()
{
    return abs(win.height() / rec.height());
}
