#pragma once
#include "../Objects/Sphere.h"
#include "Primitive.h"

class BVHManager
{
public:
	void generateBoundingVolumeHieratchies();
	void destroyBoundingVolumeHieratches();
	void updatePrimitivesBoundingBoxes();
protected:
	std::vector<Primitive*> primitives;
};