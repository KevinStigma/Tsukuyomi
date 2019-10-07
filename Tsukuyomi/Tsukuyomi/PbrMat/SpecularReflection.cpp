#include "SpecularReflection.h"

SpecularReflection::SpecularReflection(const Spectrum &R):BxDF(BxDFType(BSDF_REFLECTION|BSDF_SPECULAR)), R(R)
{}

Spectrum SpecularReflection::f(const XMFLOAT3 &wo, const XMFLOAT3 &wi) const
{
	return Spectrum();
}

Spectrum SpecularReflection::sample_f(const XMFLOAT3 &wo, XMFLOAT3 *wi, const XMFLOAT2 &sample, float *pdf, BxDFType *sampledType = nullptr) const
{
	*wi = XMFLOAT3(-wo.x, -wo.y, wo.z);
	*pdf = 1.0f;
	// TODO: fix Frenel parameter
	return R / AbsCosTheta(*wi);
}