#pragma once
#include "../common.h"
#include "../Objects/Object.h"

class RotAxis
{
public:
	RotAxis();
	float getSmallRadius() { return small_radius; }
	float getLargetRadius() { return large_radius; }
	float getScale() { return scale; }
	XMMATRIX getAxisLocalTransform(AXIS axis_type);
	XMMATRIX computeWorldMatrix(Object * obj, AXIS axis_type);
	int rayIntersectDectect(const Ray& ray, Object* obj);
	XMFLOAT2 getAxisDirectionProj(const Camera& cam, Object*sel_obj, AXIS axis_type);
protected:
	float small_radius;
	float large_radius;
	float scale;
};