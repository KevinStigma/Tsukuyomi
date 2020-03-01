#pragma once
#include "common.h"
#include "PbrMat/BxDF.h"
#include "Objects/Object.h"

struct IntersectInfo
{
	XMFLOAT3 normal;
	XMFLOAT3 pos;
	XMFLOAT3 wo;
	float tri_area;
	BxDF* bxdf=nullptr;
	Object* obj=nullptr;
	bool isSurfaceInteraction()const { return obj != nullptr; }
	Ray spawnRay(const XMFLOAT3& d)const;
	Ray spawnRayTo(const XMFLOAT3& p1, const XMFLOAT3& p2)const;
	Spectrum Le(const XMFLOAT3& w)const;
};
