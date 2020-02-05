#pragma once
#include "../Objects/Sphere.h"
#include "Primitive.h"
#include "BVHBuildNode.h"

struct BucketInfo {
	int count = 0;
	BoundingBox bounds;
};

class BVHManager
{
public:
	static const int nBuckets = 12;
	void generateBoundingVolumeHieratchies();
	void destroyBoundingVolumeHieratches();
	void updatePrimitivesBoundingBoxes();
protected:
	int partitionPrimitivesWithSAH(int start, int end, int dim, BoundingBox& bounds);
	int partitionPrimitivesWithEquallySizedSubsets(int start, int end,int dim);
	BVHBuildNode* recursiveBuild(int start, int end, int* totalnodes, std::vector<Primitive*>& ordered_prims);
	BVHBuildNode* createLeafNode(int start, int end, int* totalnodes, std::vector<Primitive*>& ordered_prims, BoundingBox& bounds);
	std::vector<Primitive*> primitives;
	BucketInfo buckets[nBuckets];
	int maxPrimsInNode = 20;
};