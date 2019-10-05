#include "GlobalSys.h"
#include "Objects/Mesh.h"
#include "common.h"
#include "D3DRenderer.h"
#include "RenderWidget.h"
#include "RGBSpectrum.h"
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
