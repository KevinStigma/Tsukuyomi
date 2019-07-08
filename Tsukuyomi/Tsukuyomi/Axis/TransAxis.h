#pragma once
#include "MathHelper\MathHelper.h"
#include "../Camera.h"
#include "Objects\Object.h"
#include "../common.h"

class TransAxis
{
public:
	TransAxis();
	float get_cylinder_length() { return cylinder_length; }
	float get_cone_length() { return cone_length; }
	float get_cylinder_radius() { return cylinder_radius; }
	float get_cone_radius() { return cone_radius; }
	int rayIntersectDectect(const Ray& ray, Object* obj);
	XMFLOAT2 getAxisDirectionProj(const Camera& cam, AXIS axis_type);
	XMMATRIX computeWorldMatrix(Object * obj, AXIS axis_type);
	XMMATRIX getAxisLocalTransform(AXIS axis_type);
protected:
	float cylinder_length;
	float cone_length;
	float cylinder_radius;
	float cone_radius;
	float scale;
};