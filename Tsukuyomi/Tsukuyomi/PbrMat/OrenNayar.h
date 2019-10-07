#pragma once
#include "BxDF.h"

class OrenNayar : public BxDF
{
public:
	OrenNayar(const Spectrum &R, float sigma);
	Spectrum f(const XMFLOAT3 &wo, const XMFLOAT3 &wi) const;
protected:
	const Spectrum R;
	float A, B;
};