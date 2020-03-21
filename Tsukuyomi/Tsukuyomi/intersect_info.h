#pragma once
#include "RGBSpectrum.h"
#include "common.h"
#include "Objects/Object.h"

class BSDF;
struct IntersectInfo
{
	XMFLOAT3 normal;
	XMFLOAT3 pos;
	XMFLOAT3 wo;
	float tri_area;
	BSDF* bsdf=nullptr;
	Object* obj=nullptr;
	~IntersectInfo();
	void ComputeScatteringFunctions();
	bool isSurfaceInteraction()const { return obj != nullptr; }
	Ray spawnRay(const XMFLOAT3& d)const;
	Ray spawnRayTo(const XMFLOAT3& p1, const XMFLOAT3& p2)const;
	Spectrum Le(const XMFLOAT3& w)const;
};
