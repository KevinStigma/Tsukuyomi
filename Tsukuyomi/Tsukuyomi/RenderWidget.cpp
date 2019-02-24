#include "RenderWidget.h"
#include "common.h"

RenderWidget::RenderWidget(QWidget* parent) : QWidget(parent)
{
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
}

void RenderWidget::resizeEvent(QResizeEvent *event)
{
	if (!renderer)
	{
		renderer = new D3DRenderer;
		renderer->initD3D((HWND)winId(), width(), height());
	}
}

void RenderWidget::paintEvent(QPaintEvent *event)
{
	renderer->renderScene();
	update();
}