#include "Plastic.h"
#include "BSDF.h"
#include "../BxDFs/LambertianReflection.h"
#include "../BxDFs/MicrofacetReflection.h"
#include "../intersect_info.h"

void PlasticMaterial::ComputeScatteringFunctions(IntersectInfo *it, bool allowMultipleLobes) const
{
	if (!Kd.isBlack())
		it->bsdf->Add(new LambertianReflection(Kd));

	// Initialize specular component of plastic material
	if (!Ks.isBlack()) {
		Fresnel *fresnel = new FresnelDielectric(1.5f, 1.f);
		// Create microfacet distribution _distrib_ for plastic material
		float rough = roughness;
		if (remapRoughness)
			rough = TrowbridgeReitzDistribution::RoughnessToAlpha(rough);
		MicrofacetDistribution *distrib = new TrowbridgeReitzDistribution(rough, rough);
		BxDF *spec = new MicrofacetReflection(Ks, distrib, fresnel);
		it->bsdf->Add(spec);
	}
}

void PlasticMaterial::exportToXML(tinyxml2::XMLElement* elm, RenderLightHelper::Material mat)
{
	std::string str = std::to_string(Kd.r) + "," + std::to_string(Kd.g) + "," + std::to_string(Kd.b);
	elm->SetAttribute("type", toString().c_str());
	elm->SetAttribute("kd", str.c_str());
	str = std::to_string(Ks.r) + "," + std::to_string(Ks.g) + "," + std::to_string(Ks.b);
	elm->SetAttribute("ks", str.c_str());
	elm->SetAttribute("roughness", std::to_string(roughness).c_str());
	elm->SetAttribute("remapRoughness", std::to_string(remapRoughness).c_str());
	elm->SetAttribute("metallic", std::to_string(mat.metallic).c_str());
}
