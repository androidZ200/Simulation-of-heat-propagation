#include "glcomplexplot.h"

const double pi = acos(-1);

GlComplexPlot::GlComplexPlot(double x, double y, double scale, QWidget* parent)
	: GlPlot(x, y, scale, parent)
{
}

GlComplexPlot::GlComplexPlot(QRectF position, QWidget* parent)
	: GlPlot(position, parent)
{
}

GlComplexPlot::GlComplexPlot(QWidget* parent)
	: GlPlot(parent)
{
}

GlComplexPlot::~GlComplexPlot()
{
}

void GlComplexPlot::paintGL()
{
	GlPlot::paintGL();
	if(plStyle.show_circle) drawCircle();
}

void GlComplexPlot::drawGrpg(int index)
{
	if(graphics[index].style.hiden) return;
	glLineWidth(graphics[index].style.weight);
	QColor col;
	glBegin(GL_LINE_STRIP);
	for(auto &x : graphics[index].graphic) {
		double mod = sqrt(x.second.real() * x.second.real() + x.second.imag() * x.second.imag());
		double arg = atan2(x.second.imag(), x.second.real());
		col = QColor::fromHsv(arg / pi * 180 + 180, 255, 255);
		glColor4f(col.red() / 255.0, col.green() / 255.0, col.blue() / 255.0, 1);
		glVertex3f(x.first, mod, 0);
	}
	glEnd();
}

void GlComplexPlot::drawCircle()
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

void GlComplexPlot::addGraphics(std::complex<double> (*f)(double), double start, double end, double step)
{
	QVector<QPair<double, std::complex<double>>> graph;

	while(start <= end) {
		graph.push_back(QPair<double, std::complex<double>>(start, f(start)));
		start += step;
	}

	graphics.push_back({graph, defGraphStyle});
	update();
}


void GlComplexPlot::addGraphics(const std::complex<double> *f, int N, double start, double end)
{
	QVector<QPair<double, std::complex<double>>> graph;
	double h = (end - start) / (N - 1);
	for(int i = 0; i < N; ++i)
		graph.push_back(QPair<double, std::complex<double>>(start + h*i, f[i]));

	graphics.push_back({graph, defGraphStyle});
	update();
}

void GlComplexPlot::deleteGraphics(int index)
{
	if(index < graphics.size() && index >= 0) graphics.erase(graphics.begin() + index);
}


int GlComplexPlot::countGraphics() const
{
	return graphics.size();
}


void GlComplexPlot::setGraphicStyle(GlGraphicStyle style, int index)
{
	graphics[index].style = style;
}


GlGraphicStyle GlComplexPlot::getGraphicStyle(int index) const
{
	return graphics[index].style;
}


GlGraphicStyle& GlComplexPlot::graphicStyle(int index)
{
	return graphics[index].style;
}


void GlComplexPlot::ClearAll()
{
	graphics.clear();
}
