#ifndef PLOT_H
#define PLOT_H

#include <QGLWidget>
#include <complex>
#include <QMouseEvent>
#include <QPair>
#include <QVector>

struct GLPlaneStyle {
	enum class ResizeStyle {
		fixed_zoom,
		stick_border
	};

	QColor background;
	QColor color_axis;
	QColor color_grid;

	bool show_axis;
	bool show_circle;
	bool show_grid;
	QSizeF steps_grid;
	float weight_axis;

	bool is_moving;
	ResizeStyle resize_style;
};
struct GlGraphicStyle {
	QColor color;
	float weight;
	bool hiden;
};

class Plot : public QGLWidget
{
    Q_OBJECT

	struct RealGraphic {
		QVector<QPair<double, double>> graphic;
		GlGraphicStyle style;
	};
	struct RealComplex {
		QVector<QPair<double, std::complex<double>>> graphic;
		GlGraphicStyle style;
	};

public:
    Plot(double x = 0, double y = 0, double scale = 1, QWidget *parent = nullptr);
    Plot(QWidget *parent = nullptr);

    void addComplexGraphics(std::complex<double> (*f)(double), double start, double end, double step);
    void addComplexGraphics(std::complex<double>* f, int N, double start, double end);
    void addRealGraphics(double (*f)(double), double start, double end, double step);
    void addRealGraphics(double *f, int N, double start, double end);
    void deleteComplexGraphics(int index);
    void deleteRealGraphics(int index);
	int countRealGraphics() const;
	int countComplexGraphics() const;
	void setComplexGraphicStyle(GlGraphicStyle style, int index);
	void setRealGraphicStyle(GlGraphicStyle style, int index);
	GlGraphicStyle getComplexGraphicStyle(int index) const;
	GlGraphicStyle getRealGraphicStyle(int index) const;
	GlGraphicStyle& ComplexGraphicStyle(int index);
	GlGraphicStyle& RealGraphicStyle(int index);

	GLPlaneStyle getStylePlane() const;
	GLPlaneStyle& StylePlane();
	void setStylePlane(GLPlaneStyle style);

public slots:
    void Move(QPointF offset);
    void Zoom(double zoom, QPointF mouse);
	void ClearAll();
	void SetPositin(QRectF rect);
	void StartPosition();

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

private:
    void drawGrid();
    void drawAxis();
    void drawComGrpg(int index);
    void drawRealGraph(int index);
    void drawCircle();
	void updateBackground();
	void setDefaultPlaneStyle();
	void setDefaultGraphicStyle();

    QSize win;          // размер окна отрисовки
    QRectF rec;         // прямоугоник в плоскости, который отображается
    QPoint mouse;
	GLPlaneStyle plStyle;
	GlGraphicStyle defGraphStyle;

    double zoomX();
    double zoomY();

	QVector<RealComplex> complexGraphics;
	QVector<RealGraphic> realGraphics;
};

#endif // PLOT_H
