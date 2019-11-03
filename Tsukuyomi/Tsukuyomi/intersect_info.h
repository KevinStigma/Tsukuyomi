#pragma once
#include "common.h"
#include "PbrMat/BxDF.h"
#include "Objects/Object.h"

struct IntersectInfo
{
	XMFLOAT3 normal;
	XMFLOAT3 pos;
	BxDF* bxdf;
	Object* obj=nullptr;
};
