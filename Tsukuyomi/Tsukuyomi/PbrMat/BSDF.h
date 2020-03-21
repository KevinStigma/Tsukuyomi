#pragma once
#include "../intersect_info.h"
#include "../BxDFs/BxDF.h"

class BSDF
{
public:
	// BSDF Public Methods
	BSDF(const IntersectInfo &it, float eta = 1)
		: eta(eta),
		ns(0.0, 0.0, 0.0),
		ng(0.0, 0.0, 0.0),
		ss(0.0, 0.0, 0.0),
		ts(0.0, 0.0, 0.0)
	{}

	void Add(BxDF *b) {
		bxdfs[nBxDFs++] = b;
	}
	int NumComponents(BxDFType flags = BSDF_ALL) const;
	XMFLOAT3 WorldToLocal(const XMFLOAT3 &v) const;
	XMFLOAT3 LocalToWorld(const XMFLOAT3 &v) const;
	Spectrum f(const XMFLOAT3 &woW, const XMFLOAT3 &wiW,
		BxDFType flags = BSDF_ALL) const;
	Spectrum rho(int nSamples, const XMFLOAT2 *samples1, const XMFLOAT2 *samples2,
		BxDFType flags = BSDF_ALL) const;
	Spectrum rho(const XMFLOAT2 &wo, int nSamples, const XMFLOAT2 *samples,
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
	~BSDF() {}

	// BSDF Private Data
	const XMFLOAT3 ns, ng;
	const XMFLOAT3 ss, ts;
	int nBxDFs = 0;
	static const int MaxBxDFs = 8;
	BxDF *bxdfs[MaxBxDFs];
};