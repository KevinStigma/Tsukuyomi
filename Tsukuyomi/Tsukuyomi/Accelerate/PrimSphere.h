#pragma once
#include "Primitive.h"
class PrimSphere : public Primitive
{
public:
	PrimSphere(Mesh*p, float r, XMFLOAT3 c): Primitive(p), radius(r), center(c) {}
	void generateBoundingBox();
	bool is_intersect(const Ray& ray, float&t, IntersectInfo& it);
protected:
	float radius;
	XMFLOAT3 center;
};