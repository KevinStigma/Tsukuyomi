#include "Matte.h"
#include "BSDF.h"
#include "../BxDFs/LambertianReflection.h"
#include "../BxDFs/OrenNayar.h"

void MatteMaterial::ComputeScatteringFunctions(IntersectInfo *it, bool allowMultipleLobes) const
{
	float sig = MathHelper::Clamp<float>(sigma, 0.0, 90.0);
	if (sig == 0)
		it->bsdf->Add(new LambertianReflection(Kd));
	else
		it->bsdf->Add(new OrenNayar(Kd, sig));
}

void MatteMaterial::exportToXML(tinyxml2::XMLElement* elm)
{
	std::string str = std::to_string(Kd.r) + "," + std::to_string(Kd.g) + "," + std::to_string(Kd.b);
	elm->SetAttribute("type", "Matte");
	elm->SetAttribute("kd", str.c_str());
	elm->SetAttribute("sigma", std::to_string(sigma).c_str());
}