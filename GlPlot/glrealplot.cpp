#include "glrealplot.h"

GlRealPlot::GlRealPlot(double x, double y, double scale, QWidget* parent)
	: GlPlot(x, y, scale, parent)
{
}

GlRealPlot::GlRealPlot(QRectF position, QWidget* parent)
	: GlPlot(position, parent)
{
}

GlRealPlot::GlRealPlot(QWidget* parent)
	: GlPlot(parent)
{
}

GlRealPlot::~GlRealPlot()
{
}

void GlRealPlot::drawGrpg(int index)
{
	if(graphics[index].style.hiden) return;
	glLineWidth(graphics[index].style.weight);
	QColor& col = graphics[index].style.color;
	glColor4f(col.red() / 255.0f, col.green() / 255.0f, col.blue() / 255.0f, 1.0f);
	glBegin(GL_LINE_STRIP);
	for(auto &x : graphics[index].graphic) {
		glVertex3f(x.first, x.second, 0);
	}
	glEnd();
}

void GlRealPlot::addGraphics(double (*f)(double), double start, double end, double step)
{
	QVector<QPair<double, double>> graph;

	while(start <= end) {
		graph.push_back(QPair<double, double>(start, f(start)));
		start += step;
	}

	graphics.push_back({graph, defGraphStyle});
	update();
}


void GlRealPlot::addGraphics(const double *f, int N, double start, double end)
{
	QVector<QPair<double, double>> graph;
	double h = (end - start) / (N - 1);
	for(int i = 0; i < N; ++i)
		graph.push_back(QPair<double, double>(start + h*i, f[i]));

	graphics.push_back({graph, defGraphStyle});
	update();
}

void GlRealPlot::deleteGraphics(int index)
{
	if(index < graphics.size() && index >= 0) graphics.erase(graphics.begin() + index);
}


int GlRealPlot::countGraphics() const
{
	return graphics.size();
}


void GlRealPlot::setGraphicStyle(GlGraphicStyle style, int index)
{
	graphics[index].style = style;
}


GlGraphicStyle GlRealPlot::getGraphicStyle(int index) const
{
	return graphics[index].style;
}


GlGraphicStyle& GlRealPlot::graphicStyle(int index)
{
	return graphics[index].style;
}


void GlRealPlot::ClearAll()
{
	graphics.clear();
}
