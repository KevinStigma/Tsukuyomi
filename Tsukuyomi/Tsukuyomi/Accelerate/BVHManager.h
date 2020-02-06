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
	void destroyBVHBuildNodes(BVHBuildNode* root);
	BVHBuildNode* recursiveBuild(int start, int end, int* totalnodes, std::vector<Primitive*>& ordered_prims);
	BVHBuildNode* createLeafNode(int start, int end, int* totalnodes, std::vector<Primitive*>& ordered_prims, BoundingBox& bounds);
	int flattenBVHTree(BVHBuildNode* node, int* offset);
	std::vector<Primitive*> primitives;
	BucketInfo buckets[nBuckets];
	std::vector<LinearBVHNode> linear_nodes;
	int maxPrimsInNode = 255;
};