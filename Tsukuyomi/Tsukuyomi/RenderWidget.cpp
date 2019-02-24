#include "RenderWidget.h"
#include "Globalsys.h"
#include "common.h"


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