#pragma once
#include "Primitive.h"
class PrimSphere : public Primitive
{
public:
	PrimSphere(Mesh*p, float r, XMFLOAT3 c): Primitive(p), radius(r), center(c) {}
	void generateBoundingBox();
protected:
	float radius;
	XMFLOAT3 center;
};