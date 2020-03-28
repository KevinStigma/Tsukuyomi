#pragma once
#include "Primitive.h"
class Triangle: public Primitive
{
public:
	Triangle(Mesh*p, XMFLOAT3 v1, XMFLOAT3 v2, XMFLOAT3 v3, XMFLOAT3 n1, XMFLOAT3 n2, XMFLOAT3 n3, int index):Primitive(p), a(v1), b(v2), c(v3), na(n1), nb(n2), nc(n3), index(index) {}
	void generateBoundingBox();
	bool is_intersect(const Ray& ray, float&t, IntersectInfo& it);
protected:
	void GetUVs(XMFLOAT2 uv[3]) const;
	int index;
	XMFLOAT3 a, b, c;
	XMFLOAT3 na, nb, nc;
};