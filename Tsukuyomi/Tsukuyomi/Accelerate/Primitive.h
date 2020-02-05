#pragma once
#include "../Objects/Mesh.h"
class Primitive
{
public:
	Primitive(Mesh*p);
	~Primitive() {}
	virtual void generateBoundingBox()=0;
	BoundingBox getBoundingBox()const { return boundingbox; }
protected:
	BoundingBox boundingbox;
	Mesh* parent=nullptr;
};