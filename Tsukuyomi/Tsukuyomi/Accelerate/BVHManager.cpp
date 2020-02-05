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


}


void BVHManager::destroyBoundingVolumeHieratches()
{
	for (int i = 0; i < primitives.size(); i++)
	{
		SAFE_DELETE(primitives[i]);
	}
	primitives.clear();
}


void BVHManager::updatePrimitivesBoundingBoxes()
{

}