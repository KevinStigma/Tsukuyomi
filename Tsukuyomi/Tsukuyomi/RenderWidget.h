#pragma once
#include <QWidget>
#include "D3DRenderer.h"

class RenderWidget :public QWidget
{
	Q_OBJECT
public:
	RenderWidget(QWidget *parent = 0);
	~RenderWidget();
	virtual QPaintEngine *paintEngine() const
	{
		return NULL;
	}

	void resizeEvent(QResizeEvent *event);
	void paintEvent(QPaintEvent *event);

	void keyPressEvent(QKeyEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
	D3DRenderer * getRenderer() { return renderer; }
private:

	D3DRenderer * renderer = nullptr;
	QPoint lastMousePos;
	int touchType = -1;
};
