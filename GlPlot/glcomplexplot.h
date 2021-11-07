#ifndef GLCOMPLEXPLOT_H
#define GLCOMPLEXPLOT_H

#include "glplot.h"
#include <QObject>
#include <complex>

class GlComplexPlot : public GlPlot
{
	Q_OBJECT

	struct GraphicAndStyle {
		QVector<QPair<double, std::complex<double>>> graphic;
		GlGraphicStyle style;
	};

public:
	GlComplexPlot(double x = 0, double y = 0, double scale = 1, QWidget *parent = nullptr);
	GlComplexPlot(QRectF position, QWidget *parent = nullptr);
	GlComplexPlot(QWidget *parent = nullptr);
	~GlComplexPlot() override;

	void addGraphics(std::complex<double> (*f)(double), double start, double end, double step);
	void addGraphics(const std::complex<double>* f, int N, double start, double end);
	void deleteGraphics(int index) override;
	int countGraphics() const override;
	void setGraphicStyle(GlGraphicStyle style, int index) override;
	GlGraphicStyle getGraphicStyle(int index) const override;
	GlGraphicStyle& graphicStyle(int index) override;

public slots:
	void ClearAll();

protected:
	void paintGL() override;
	void drawGrpg(int index) override;
	void drawCircle();

	QVector<GraphicAndStyle> graphics;
};

#endif // GLCOMPLEXPLOT_H
