#pragma once
#include "../Objects/Camera.h"
class OfflineRenderer
{
public:
	OfflineRenderer() {};
	virtual ~OfflineRenderer() {};
	virtual void start_render(Camera* camera, int height) = 0;
};