#pragma once
#include "OfflineRenderer.h"

class NormalRenderer :public OfflineRenderer
{
public:
	NormalRenderer();
	virtual ~NormalRenderer();
	virtual void start_render(Camera* camera, int height);
};