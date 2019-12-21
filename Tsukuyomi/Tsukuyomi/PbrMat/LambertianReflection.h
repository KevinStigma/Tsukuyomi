#pragma once
#include "BxDF.h"
#include "Fresnel.h"

class LambertianReflection :public BxDF
{
public:
	LambertianReflection(const Spectrum &R);
	Spectrum f(const XMFLOAT3 &wo, const XMFLOAT3 &wi)const;
	std::string toString() const;
protected:
	const Spectrum R;
};