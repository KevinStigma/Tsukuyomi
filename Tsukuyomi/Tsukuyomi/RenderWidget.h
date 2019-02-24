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

	virtual void resizeEvent(QResizeEvent *event);
	virtual void paintEvent(QPaintEvent *event);

	/*
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void keyReleaseEvent(QKeyEvent *event);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void wheelEvent(QWheelEvent *event);
	*/
private:

	D3DRenderer * renderer = nullptr;
};
