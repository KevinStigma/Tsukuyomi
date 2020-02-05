#pragma once
#include "../Objects/Object.h"
struct BVHBuildNode {
	BVHBuildNode* children[2];
	BoundingBox boundingbox;
	int splitAxis, firstPrimeOffset, nPrimitives;
	void initLeaf(int first, int n, const BoundingBox&b);
	void initInterior(int axis, BVHBuildNode*c0, BVHBuildNode*c1);
};