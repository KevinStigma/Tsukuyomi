#pragma once
#include "PbrMat.h"
class PlasticMaterial : public PbrMat {
public:
	// PlasticMaterial Public Methods
	PlasticMaterial(const Spectrum &Kd, const Spectrum &Ks, float roughness, bool remapRoughness)
		: Kd(Kd), Ks(Ks), roughness(roughness),  remapRoughness(remapRoughness) 
	{}
	void ComputeScatteringFunctions(IntersectInfo *it, bool allowMultipleLobes) const;
	void exportToXML(tinyxml2::XMLElement* elm);
	std::string toString() { return "Plastic"; }

private:
	// PlasticMaterial Private Data
	Spectrum Kd, Ks;
	float roughness;
	const bool remapRoughness;
};