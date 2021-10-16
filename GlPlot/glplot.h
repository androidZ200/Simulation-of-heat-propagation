#ifndef GLPLOT_H
#define GLPLOT_H

#include <QOpenGLWidget>
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

class GlPlot : public QOpenGLWidget
{
	Q_OBJECT

public:
	GlPlot(double x = 0, double y = 0, double scale = 1, QWidget *parent = nullptr);
	GlPlot(QRectF position, QWidget *parent = nullptr);
	GlPlot(QWidget *parent = nullptr);
	virtual ~GlPlot();

	virtual void deleteGraphics(int index) = 0;
	virtual int countGraphics() const = 0;
	virtual void setGraphicStyle(GlGraphicStyle style, int index) = 0;
	virtual GlGraphicStyle getGraphicStyle(int index) const = 0;
	virtual GlGraphicStyle& graphicStyle(int index) = 0;

	GLPlaneStyle getStylePlane() const;
	GLPlaneStyle& StylePlane();
	void setStylePlane(GLPlaneStyle style);

public slots:
	void Move(QPointF offset);
	void Zoom(double zoom, QPointF mouse);
	void SetPositin(QRectF rect);
	void StartPosition();

signals:
    void MoveSig(QPointF offset);
    void ZoomSig(double zoom, QPointF mouse);

protected:
	virtual void initializeGL() override;
	virtual void resizeGL(int w, int h) override;
	virtual void paintGL() override;

	void mousePressEvent(QMouseEvent* pe) override;
	void mouseMoveEvent(QMouseEvent* pe) override;
	void wheelEvent(QWheelEvent* pe) override;

	virtual void drawGrid();
	virtual void drawAxis();
	virtual void drawGrpg(int index) = 0;
	void updateBackground();
	void setDefaultPlaneStyle();
	void setDefaultGraphicStyle();
	int findIndexX(int index, double x);

	QSize win;          // размер окна отрисовки
	QRectF rec;         // прямоугоник в плоскости, который отображается
	QPoint mouse;
	GLPlaneStyle plStyle;
	GlGraphicStyle defGraphStyle;

	double zoomX();
	double zoomY();
};

#endif // GLPLOT_H
