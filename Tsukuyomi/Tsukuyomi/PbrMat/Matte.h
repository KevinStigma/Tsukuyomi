#pragma once
#include "PbrMat.h"

class MatteMaterial : public PbrMat {
public:
	// MatteMaterial Public Methods
	MatteMaterial(const Spectrum &Kd,
		float sigma)
		: Kd(Kd), sigma(sigma) {}
	void ComputeScatteringFunctions(IntersectInfo *it, bool allowMultipleLobes) const;
	void exportToXML(tinyxml2::XMLElement* elm);
	RenderLightHelper::Material generateRenderMaterial();
	std::string toString()
	{
		return "Matte";
	}

private:
	// MatteMaterial Private Data
	Spectrum Kd;
	float sigma;
};
