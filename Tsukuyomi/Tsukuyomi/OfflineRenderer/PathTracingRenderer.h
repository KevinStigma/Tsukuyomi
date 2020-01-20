#pragma once
#include "OfflineRenderer.h"

class PathTracingRenderer :public OfflineRenderer
{
public:
	PathTracingRenderer();
	virtual ~PathTracingRenderer();
	virtual void start_render(Camera* camera, int height);
protected:
	Spectrum Li(const Ray& ray);
	Spectrum sample_pixel(Camera* camera, int x, int y, int width, int height);
	int sample_count = 16;
	int max_bounce = 5;
};