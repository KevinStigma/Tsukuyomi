#include "GlobalSys.h"
#include "Objects/Mesh.h"
#include "common.h"
#include "D3DRenderer.h"
#include "RenderWidget.h"
#include "RGBSpectrum.h"
#include "OfflineRenderer\NormalRenderer.h"
#include "OfflineRenderer\PathTracingRenderer.h"
#include <iostream>

GlobalSys *g_pGlobalSys = NULL;

GlobalSys::GlobalSys(){}

GlobalSys::~GlobalSys(){}

float GlobalSys::getMoveSpeed()
{
	return 0.001 / 20.0 * moveSpeedSlider->value();
}

OfflineRenderer* GlobalSys::generateOfflineRenderer()
{
	OfflineRenderer* renderer=nullptr;
	if (render_paras.offline_render_type == OfflineRenderType::NORMAL_DEBUG)
		renderer = new NormalRenderer();
	else if (render_paras.offline_render_type == OfflineRenderType::PATH_TRACING)
		renderer = new PathTracingRenderer();
	return renderer;
}

float GlobalSys::cast_ray_to_get_intersection(const Ray& ray, IntersectInfo& info, bool use_accel)
{
	if (!use_accel)
	{
		auto objs = objectManager.getAllObjects();
		float min_t = -1.0f;
		for (int i = 0; i < objs.size(); i++)
		{
			Object* obj = objs[i];
			Mesh* mesh = getMesh(obj);
			if (!mesh)
				continue;
			float t;
			IntersectInfo it;
			if (mesh->is_intersect(ray, t, it) && (min_t < 0.0f || t < min_t))
			{
				min_t = t;
				info = it;
				if (obj->getType() == AREA_LIGHT)
					info.obj = obj;
			}
		}
		return min_t;
	}
	else
	{
		float min_t = -1.0f;
		auto bvh_nodes = objectManager.getBVHManager()->getBvhNodes();
		std::stack<int> nodesToVisit;
		int cur_node_index = 0;
		while (true)
		{
			LinearBVHNode node = bvh_nodes[cur_node_index];
			if (node.boundingbox.isIntersect(ray, XMMatrixIdentity()))
			{
				if (node.nPrimitives > 0)
				{
					for (int i = 0; i < node.nPrimitives; i++)
					{
						float t=-1.0f;
						Primitive* p = objectManager.getBVHManager()->getPrimitive(node.primitivesOffset + i);
						IntersectInfo it;
						if (p->is_intersect(ray, t, it) && (min_t < 0.0f || t < min_t))
						{
							min_t = t;
							info = it;
							if (p->getParent()->getAreaLight())
								info.obj = p->getParent()->getAreaLight();
						}
					}
					if (nodesToVisit.empty())
						break;
					cur_node_index = nodesToVisit.top();
					nodesToVisit.pop();
				}
				else
				{
					if(ray.dirIsNeg(node.axis))
					{
						nodesToVisit.push(cur_node_index + 1);
						cur_node_index = node.secondChildOfset;

					}
					else
					{
						nodesToVisit.push(node.secondChildOfset);
						cur_node_index++;
					}
				}
			}
			else
			{
				if (nodesToVisit.empty())
					break;
				cur_node_index = nodesToVisit.top();
				nodesToVisit.pop();
			}
		}
		return min_t;
	}
}

void GlobalSys::generateBVH()
{
	objectManager.getBVHManager()->generateBoundingVolumeHieratchies();
}