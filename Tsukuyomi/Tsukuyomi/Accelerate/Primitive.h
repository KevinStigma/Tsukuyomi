#pragma once
#include "../Objects/Mesh.h"
class Primitive
{
public:
	Primitive(Mesh*p);
	~Primitive() {}
	virtual void generateBoundingBox()=0;
	virtual bool is_intersect(const Ray& ray, float&t, IntersectInfo& it) { return false; };
	BoundingBox getBoundingBox()const { return boundingbox; }
	Mesh* getParent() { return parent; }
protected:
	BoundingBox boundingbox;
	Mesh* parent=nullptr;
};