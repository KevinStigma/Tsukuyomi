#pragma once
#include "../intersect_info.h"
#include "tinyxml2/tinyxml2.h"
#include "LightHelper.h"
class PbrMat
{
public:
	// Material Interface
	virtual void ComputeScatteringFunctions(IntersectInfo *it,
		bool allowMultipleLobes) const = 0;
	virtual ~PbrMat() {}
	virtual std::string toString()=0;
	virtual void exportToXML(tinyxml2::XMLElement* elm)=0;
	virtual RenderLightHelper::Material generateRenderMaterial()=0;
};