#pragma once
#include "MathHelper\MathHelper.h"
#include "Objects\Object.h"

class TransAxis
{
public:
	TransAxis();
	float get_cylinder_length() { return cylinder_length; }
	float get_cone_length() { return cone_length; }
	
	// axis_type 0 x, 1 y, 2 z
	XMMATRIX computeWorldMatrix(Object * obj, int axis_type);
protected:
	float cylinder_length;
	float cone_length;
	float scale;
};