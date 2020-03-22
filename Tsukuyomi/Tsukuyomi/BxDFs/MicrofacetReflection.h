#pragma once
#include "BxDF.h"
#include "Fresnel.h"
#include "MicrofacetDistribution.h"

class MicrofacetReflection : public BxDF {
public:
	// MicrofacetReflection Public Methods
	MicrofacetReflection(const Spectrum &R,
		MicrofacetDistribution *distribution, Fresnel *fresnel)
		: BxDF(BxDFType(BSDF_REFLECTION | BSDF_GLOSSY)),
		R(R),
		distribution(distribution),
		fresnel(fresnel) {}
	~MicrofacetReflection();
	Spectrum f(const XMFLOAT3 &wo, const XMFLOAT3 &wi) const;
	Spectrum sample_f(const XMFLOAT3 &wo, XMFLOAT3 *wi, const XMFLOAT2 &u,
		Float *pdf, BxDFType *sampledType) const;
	Float Pdf(const XMFLOAT3 &wo, const XMFLOAT3 &wi) const;
	std::string toString() const { return "MicrofacetReflection"; }

private:
	// MicrofacetReflection Private Data
	const Spectrum R;
	const MicrofacetDistribution *distribution;
	const Fresnel *fresnel;
};
