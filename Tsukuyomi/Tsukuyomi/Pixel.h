#pragma once
#include "common.h"

struct Pixel
{
	XMFLOAT3 xyz = XMFLOAT3(0.0, 0.0, 0.0);
	float filterWeightSum = 0.0;
};