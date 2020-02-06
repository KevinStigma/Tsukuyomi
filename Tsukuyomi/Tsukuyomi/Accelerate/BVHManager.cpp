#include "BVHManager.h"
#include "GlobalSys.h"

void BVHManager::generateBoundingVolumeHieratchies()
{
	destroyBoundingVolumeHieratches();
	auto objs = g_pGlobalSys->objectManager.getAllObjects();
	
	//allocate all primivies
	for (int i = 0; i < objs.size(); i++)
	{
		Object* obj = objs[i];
		Mesh* mesh = getMesh(obj);
		if (!mesh)
			continue;
		std::vector<Primitive*> mesh_ps = mesh->getAllPrimitives();
		primitives.insert(primitives.end(), mesh_ps.begin(), mesh_ps.end());
	}
	updatePrimitivesBoundingBoxes();

	// generate BVH node
	std::vector<Primitive*> ordered_prims;
	int totalnodes = 0;
	BVHBuildNode* root = recursiveBuild(0, primitives.size(), &totalnodes, ordered_prims);
	primitives.assign(ordered_prims.begin(), ordered_prims.end());

	//flatten BVH tree
	linear_nodes.resize(totalnodes);
	int offset = 0;
	flattenBVHTree(root, &offset);

	destroyBVHBuildNodes(root);
}

void BVHManager::destroyBoundingVolumeHieratches()
{
	for (int i = 0; i < primitives.size(); i++)
	{
		SAFE_DELETE(primitives[i]);
	}
	primitives.clear();
	linear_nodes.clear();
}


void BVHManager::updatePrimitivesBoundingBoxes()
{
	for (int i = 0; i < primitives.size(); i++)
		primitives[i]->generateBoundingBox();
}

int BVHManager::partitionPrimitivesWithEquallySizedSubsets(int start, int end, int dim)
{
	int mid = (start + end) / 2;
	std::nth_element(&primitives[start], &primitives[mid], &primitives[end - 1] + 1,
		[dim](const Primitive* a, const Primitive* b) {
		return a->getBoundingBox().getCenterValFromDim(dim) < b->getBoundingBox().getCenterValFromDim(dim);
	});
	return mid;
}

int BVHManager::partitionPrimitivesWithSAH(int start, int end, int dim, BoundingBox& bounds)
{
	int nPrimitives = end - start;
	int nBuckets = BVHManager::nBuckets;
	if (nPrimitives <= 4)
		return partitionPrimitivesWithEquallySizedSubsets(start, end, dim);
	else
	{
		for (int i = start; i < end; i++)
		{
			BoundingBox prim_bounds = primitives[i]->getBoundingBox();
			int b = nBuckets * 
				(prim_bounds.getCenterValFromDim(dim) - bounds.getBottomFromDim(dim)) /
				(bounds.getTopFromDim(dim) - bounds.getBottomFromDim(dim));
			if (b == nBuckets)
				b--;
			buckets[b].count++;
			buckets[b].bounds = BoundingBox::Union(buckets[b].bounds, prim_bounds);
		}
		float cost[BVHManager::nBuckets - 1];
		for (int i = 0; i < nBuckets - 1; i++)
		{
			BoundingBox b0, b1;
			int count0 = 0, count1 = 0;
			for (int j = 0; j <= i; j++)
			{
				b0 = BoundingBox::Union(b0, buckets[j].bounds);
				count0 += buckets[j].count;
			}

			for (int j = i+1; j < BVHManager::nBuckets; j++)
			{
				b1 = BoundingBox::Union(b1, buckets[j].bounds);
				count1 += buckets[j].count;
			}
			cost[i] = 0.125f + (count0*b0.surfaceArea() + count1 * b1.surfaceArea()) / bounds.surfaceArea();
		}

		float min_cost = cost[0];
		int min_ind = 0;
		for (int i = 0; i < BVHManager::nBuckets; i++)
		{
			if (cost[i] < min_cost)
			{
				min_cost = cost[i];
				min_ind = i;
			}
		}
		if (nPrimitives > maxPrimsInNode || min_cost < nPrimitives)
		{
			Primitive** p = std::partition(&primitives[start], &primitives[end - 1] + 1,
				[=](const Primitive* pi) {
				int b = nBuckets *
					(pi->getBoundingBox().getCenterValFromDim(dim) - bounds.getBottomFromDim(dim)) /
					(bounds.getTopFromDim(dim) - bounds.getBottomFromDim(dim));
				if (b == nBuckets)
					b--;
				return b <= min_ind;
			});
			return p - &primitives[0];
		}
		else
			return -1;
	}
}

void BVHManager::destroyBVHBuildNodes(BVHBuildNode* root)
{
	if (!root)
		return;
	if (root->nPrimitives > 0)
	{
		SAFE_DELETE(root);
	}
	else
	{
		destroyBVHBuildNodes(root->children[0]);
		destroyBVHBuildNodes(root->children[1]);
		SAFE_DELETE(root);
	}

}

BVHBuildNode* BVHManager::recursiveBuild(int start, int end, int* totalnodes, std::vector<Primitive*>& ordered_prims)
{
	BVHBuildNode* node = nullptr;
	(*totalnodes)++;
	int nPrimitives = end - start;
	BoundingBox bounds;
	for (int i = start; i < end; i++)
		bounds = BoundingBox::Union(bounds, primitives[i]->getBoundingBox());
	if (nPrimitives == 1)
		node = createLeafNode(start, end, totalnodes, ordered_prims, bounds);
	else if(nPrimitives > 1)
	{
		int dim = bounds.maximumExtent();
		if(bounds.getTopFromDim(dim)==bounds.getBottomFromDim(dim))
			node = createLeafNode(start, end, totalnodes, ordered_prims, bounds);
		else
		{
			int mid = partitionPrimitivesWithSAH(start, end, dim, bounds);
			if(mid < 0)
				node = createLeafNode(start, end, totalnodes, ordered_prims, bounds);
			else {
				node = new BVHBuildNode;
				node->initInterior(dim,
					recursiveBuild(start, mid, totalnodes, ordered_prims),
					recursiveBuild(mid, end, totalnodes, ordered_prims));
			}
		}
	}
	return node;
}

BVHBuildNode* BVHManager::createLeafNode(int start, int end, int* totalnodes, std::vector<Primitive*>& ordered_prims, BoundingBox& bounds)
{
	int firstPrimOffset = ordered_prims.size();
	for (int i = start; i < end; i++)
		ordered_prims.push_back(primitives[i]);
	BVHBuildNode*node = new BVHBuildNode;
	node->initLeaf(firstPrimOffset, end - start, bounds);
	return node;
}

int BVHManager::flattenBVHTree(BVHBuildNode* node, int* offset)
{
	LinearBVHNode& lnode = linear_nodes[*offset];
	lnode.boundingbox = node->boundingbox;
	int myOffset = (*offset)++;

	if (node->nPrimitives > 0)
	{
		lnode.primitivesOffset = node->firstPrimeOffset;
		lnode.nPrimitives = node->nPrimitives;
	}
	else
	{
		lnode.axis = node->splitAxis;
		lnode.nPrimitives = 0;
		flattenBVHTree(node->children[0], offset);
		lnode.secondChildOfset = flattenBVHTree(node->children[1], offset);
	}
	return myOffset;
}