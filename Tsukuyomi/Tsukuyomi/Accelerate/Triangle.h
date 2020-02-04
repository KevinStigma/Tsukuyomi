#pragma once
#include "Primitive.h"
class Triangle: public Primitive
{
public:
	Triangle(Mesh*p, XMFLOAT3 v1, XMFLOAT3 v2, XMFLOAT3 v3):Primitive(p), a(v1), b(v2), c(v3) {}
	void generateBoundingBox();
protected:
	XMFLOAT3 a, b, c;
};