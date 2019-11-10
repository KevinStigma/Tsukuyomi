#pragma once
#include "OfflineRenderer.h"

class PathTracingRenderer :public OfflineRenderer
{
public:
	PathTracingRenderer();
	virtual ~PathTracingRenderer();
	virtual void start_render(Camera* camera, int height);
};