#include "RenderWidget.h"
#include "Globalsys.h"
#include "common.h"
#include <iostream>
#include <QMouseEvent>
#include <QListWidget>

RenderWidget::RenderWidget(QWidget* parent) : QWidget(parent)
{
	setAttribute(Qt::WA_PaintOnScreen, true);
	setAttribute(Qt::WA_NativeWindow, true);
};

RenderWidget::~RenderWidget()
{
	if (renderer)
	{
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
	else if (mouse_event->button() == Qt::RightButton)
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
		float diff_x = (cur_pos.x() - lastMousePos.x()) * 0.001;
		float diff_y = (lastMousePos.y() - cur_pos.y()) * 0.001;
		camera.rotateRight(-diff_y);
		camera.rotateY(diff_x);
	}
	else if (touchType == 1)
	{
		float diff_x = (cur_pos.x() - lastMousePos.x()) * 0.001;
		float diff_y = (lastMousePos.y() - cur_pos.y()) * 0.001;
		camera.walkRight(diff_x);
		camera.walkUp(diff_y);
	}
	lastMousePos = cur_pos;
}

void RenderWidget::wheelEvent(QWheelEvent *event)
{
	float delta = event->delta();
	Camera& camera = renderer->getCamera();
	camera.walkForward(delta * g_pGlobalSys->getMoveSpeed());
}
