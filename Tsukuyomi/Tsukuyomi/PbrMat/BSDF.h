#pragma once
#include "../intersect_info.h"
#include "../BxDFs/BxDF.h"
#include "LightHelper.h"

class BSDF
{
public:
	// BSDF Public Methods
	BSDF(const IntersectInfo &it, float eta = 1)
		: eta(eta), normal(it.normal), ss(it.dpdu)
	{
		ts = MathHelper::NormalizeFloat3(MathHelper::Cross(normal, ss));
	}
	~BSDF();

	void Add(BxDF *b) {
		bxdfs[nBxDFs++] = b;
	}
	int NumComponents(BxDFType flags = BSDF_ALL) const;
	XMFLOAT3 WorldToLocal(const XMFLOAT3 &v) const;
	XMFLOAT3 LocalToWorld(const XMFLOAT3 &v) const;
	Spectrum f(const XMFLOAT3 &woW, const XMFLOAT3 &wiW,
		BxDFType flags = BSDF_ALL) const;
	Spectrum Sample_f(const XMFLOAT3 &wo, XMFLOAT3 *wi, const XMFLOAT2 &u,
		float *pdf, BxDFType type = BSDF_ALL,
		BxDFType *sampledType = nullptr) const;
	float Pdf(const XMFLOAT3 &wo, const XMFLOAT3 &wi,
		BxDFType flags = BSDF_ALL) const;
	std::string ToString() const;

	// BSDF Public Data
	const float eta;

private:
	// BSDF Private Methods
	XMFLOAT3 normal, ss, ts;;
	int nBxDFs = 0;
	static const int MaxBxDFs = 8;
	BxDF *bxdfs[MaxBxDFs];
};