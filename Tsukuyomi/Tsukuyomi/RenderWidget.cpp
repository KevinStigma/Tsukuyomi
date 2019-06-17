#include "RenderWidget.h"
#include "Globalsys.h"
#include "common.h"
#include <iostream>
#include <QMouseEvent>


RenderWidget::RenderWidget(QWidget* parent) : QWidget(parent)
{
	g_pGlobalSys = new GlobalSys;
	setAttribute(Qt::WA_PaintOnScreen, true);
	setAttribute(Qt::WA_NativeWindow, true);
};

RenderWidget::~RenderWidget()
{
	if (renderer)
	{
		renderer->cleanup();
		SAFE_DELETE(renderer);
	}
	SAFE_DELETE(g_pGlobalSys);
}

void RenderWidget::resizeEvent(QResizeEvent *event)
{
	if (!renderer)
	{
		renderer = new D3DRenderer;
		renderer->initD3D((HWND)winId(), width(), height());
		renderer->initScene();
	}
}

void RenderWidget::paintEvent(QPaintEvent *event)
{
	renderer->renderScene();
	update();
}

void RenderWidget::keyPressEvent(QKeyEvent *event)
{

}

void RenderWidget::keyReleaseEvent(QKeyEvent *event)
{

}

void RenderWidget::mousePressEvent(QMouseEvent *mouse_event)
{
	QPoint pos = mouse_event->pos();
	if (touchType != -1)
		return;
	if (mouse_event->button() == Qt::LeftButton)
	{
		lastMousePos = pos;
		touchType = 0;
	}
	else if (mouse_event->button == Qt::RightButton)
	{
		lastMousePos = pos;
		touchType = 1;
	}
}

void RenderWidget::mouseReleaseEvent(QMouseEvent *mouse_event)
{
	touchType = -1;
}

void RenderWidget::mouseMoveEvent(QMouseEvent *mouse_event)
{
	QPoint cur_pos = mouse_event->pos();
	Camera& camera = renderer->getCamera();
	if (touchType == 0)
	{
		camera.computeArcballRotation(lastMousePos.x(), height() - lastMousePos.y(), cur_pos.x(), height() - cur_pos.y(), width(), height());
	}
	else if (touchType == 1)
	{

	}
	lastMousePos = cur_pos;
}

void RenderWidget::wheelEvent(QWheelEvent *event)
{

}
