#pragma once
#include "../common.h"

class RotAxis
{
public:
	RotAxis();
	float getSmallRadius() { return small_radius; }
	float getLargetRadius() { return large_radius; }
	float getScale() { return scale; }
protected:
	float small_radius;
	float large_radius;
	float scale;
};