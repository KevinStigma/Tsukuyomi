#include "GlobalSys.h"
#include "Objects/Mesh.h"
#include "common.h"
#include "D3DRenderer.h"
#include "RenderWidget.h"
#include "RGBSpectrum.h"
#include "OfflineRenderer\NormalRenderer.h"
#include "OfflineRenderer\PathTracingRenderer.h"
#include <iostream>

GlobalSys *g_pGlobalSys = NULL;

GlobalSys::GlobalSys()
{
}

GlobalSys::~GlobalSys()
{
}

float GlobalSys::getMoveSpeed()
{
	return 0.001 / 20.0 * moveSpeedSlider->value();
}

OfflineRenderer* GlobalSys::generateOfflineRenderer()
{
	OfflineRenderer* renderer=nullptr;
	if (render_paras.offline_render_type == OfflineRenderType::NORMAL_DEBUG)
		renderer = new NormalRenderer();
	else if (render_paras.offline_render_type == OfflineRenderType::PATH_TRACING)
		renderer = new PathTracingRenderer();
	return renderer;
}