#ifndef GLREALPLOT_H
#define GLREALPLOT_H

#include "glplot.h"
#include <QObject>

class GlRealPlot : public GlPlot
{
	Q_OBJECT

	struct GraphicAndStyle {
		QVector<QPair<double, double>> graphic;
		GlGraphicStyle style;
	};

public:
	GlRealPlot(double x = 0, double y = 0, double scale = 1, QWidget *parent = nullptr);
	GlRealPlot(QRectF position, QWidget *parent = nullptr);
	GlRealPlot(QWidget *parent = nullptr);
	~GlRealPlot() override;

	void addGraphics(double (*f)(double), double start, double end, double step);
	void addGraphics(const double* f, int N, double start, double end);
	void deleteGraphics(int index) override;
	int countGraphics() const override;
	void setGraphicStyle(GlGraphicStyle style, int index) override;
	GlGraphicStyle getGraphicStyle(int index) const override;
	GlGraphicStyle& graphicStyle(int index) override;

public slots:
	void ClearAll();

protected:
	void drawGrpg(int index) override;

	QVector<GraphicAndStyle> graphics;
};

#endif // GLREALPLOT_H
