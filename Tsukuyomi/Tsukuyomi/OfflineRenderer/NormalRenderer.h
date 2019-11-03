#pragma once
#include "OfflineRenderer.h"

class NormalRenderer :public OfflineRenderer
{
public:
	NormalRenderer();
	~NormalRenderer();
	void start_renderer(Camera* camera, int height);
};