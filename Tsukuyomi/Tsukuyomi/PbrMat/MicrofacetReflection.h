#pragma once
#include "BxDF.h"
#include "Fresnel.h"

class MicrofacetReflection :public BxDF
{
public:
	static inline float RoughnessToAlpha(float roughness)
	{
		roughness = std::max<float>(roughness, (float)1e-3);
		float x = std::log(roughness);
		return 1.62142f + 0.819955f * x + 0.1734f * x * x + 0.0171201f * x * x * x +
			0.000640711f * x * x * x * x;
	}
	MicrofacetReflection(const Spectrum &R, const Fresnel*f, float alphax, float alphay);
	Spectrum f(const XMFLOAT3 &wo, const XMFLOAT3 &wi)const;
	float Pdf(const XMFLOAT3 &wo, const XMFLOAT3 &wi) const;

protected:
	float Lambda(const XMFLOAT3& w)const;
	virtual float D(const XMFLOAT3&wh)const;
	virtual float G(const XMFLOAT3&wi, const XMFLOAT3&wo)const;
	virtual float G1(const XMFLOAT3&w)const;
	const Spectrum R;
	const Fresnel* fresnel;
	float alphax, alphay;
	bool sampleVisibleArea = false;
};