#include "BVHBuildNode.h"

void BVHBuildNode::initLeaf(int first, int n, const BoundingBox&b)
{
	firstPrimeOffset = first;
	nPrimitives = n;
	boundingbox = b;
	children[0] = children[1] = nullptr;
}

void BVHBuildNode::initInterior(int axis, BVHBuildNode*c0, BVHBuildNode*c1)
{
	splitAxis = axis;
	children[0] = c0;
	children[1] = c1;
	nPrimitives = 0;
	boundingbox = BoundingBox::Union(c0->boundingbox, c1->boundingbox);
}