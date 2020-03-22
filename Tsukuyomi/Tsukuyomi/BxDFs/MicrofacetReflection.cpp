#include "MicrofacetReflection.h"

XMFLOAT3 Faceforward(const XMFLOAT3 &v, const XMFLOAT3 &v2) {
	return (MathHelper::DotFloat3(v, v2) < 0.f) ? MathHelper::NegativeFloat3(v) : v;
}

MicrofacetReflection::~MicrofacetReflection()
{
	SAFE_DELETE(fresnel);
	SAFE_DELETE(distribution);
}

Spectrum MicrofacetReflection::f(const XMFLOAT3 &wo, const XMFLOAT3 &wi) const {
	Float cosThetaO = AbsCosTheta(wo), cosThetaI = AbsCosTheta(wi);
	XMFLOAT3 wh = MathHelper::AddFloat3(wi, wo);
	// Handle degenerate cases for microfacet reflection
	if (cosThetaI == 0 || cosThetaO == 0) 
		return Spectrum();
	if (wh.x == 0 && wh.y == 0 && wh.z == 0) 
		return Spectrum();
	wh = MathHelper::NormalizeFloat3(wh);
	// For the Fresnel call, make sure that wh is in the same hemisphere
	// as the surface normal, so that TIR is handled correctly.
	Spectrum F = fresnel->Evaluate(MathHelper::DotFloat3(wi, Faceforward(wh, XMFLOAT3(0, 0, 1))));
	return R * distribution->D(wh) * distribution->G(wo, wi) * F / (4 * cosThetaI * cosThetaO);
}

Float MicrofacetReflection::Pdf(const XMFLOAT3 &wo, const XMFLOAT3 &wi) const {
	if (!SameHemisphere(wo, wi)) return 0;
	XMFLOAT3 wh = MathHelper::NormalizeFloat3(MathHelper::AddFloat3(wo, wi));
	return distribution->Pdf(wo, wh) / (4 * MathHelper::DotFloat3(wo, wh));
}

Spectrum MicrofacetReflection::sample_f(const XMFLOAT3 &wo, XMFLOAT3 *wi, const XMFLOAT2 &u, Float *pdf, BxDFType *sampledType) const
{
	// Sample microfacet orientation $\wh$ and reflected direction $\wi$
	if (wo.z == 0) return 0.;
	XMFLOAT3 wh = distribution->Sample_wh(wo, u);
	if (MathHelper::DotFloat3(wo, wh) < 0) return 0.;   // Should be rare
	*wi = MathHelper::Reflect(wo, wh);
	if (!SameHemisphere(wo, *wi)) return Spectrum(0.f);

	// Compute PDF of _wi_ for microfacet reflection
	*pdf = distribution->Pdf(wo, wh) / (4 * MathHelper::DotFloat3(wo, wh));
	return f(wo, *wi);
}