#pragma once
#include "OfflineRenderer.h"

class PathTracingRenderer :public OfflineRenderer
{
public:
	PathTracingRenderer();
	virtual ~PathTracingRenderer();
	virtual void start_render(Camera* camera, int height);
protected:
	Spectrum Li();
	Spectrum sample_pixel(Camera* camera, int x, int y, int width, int height);
	int sample_count = 100;
	int max_bounce = 5;
};