#pragma once
#include "BxDF.h"

class SpecularReflection : public BxDF
{
public:
	SpecularReflection(const Spectrum &R);
	Spectrum f(const XMFLOAT3 &wo, const XMFLOAT3 &wi) const;
	Spectrum sample_f(const XMFLOAT3 &wo, XMFLOAT3 *wi, const XMFLOAT2 &sample, float *pdf, BxDFType *sampledType = nullptr) const;
protected:
	const Spectrum R;
};