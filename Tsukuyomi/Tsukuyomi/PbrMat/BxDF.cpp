#include "BxDF.h"
#include "../MathHelper/MathHelper.h"

Spectrum BxDF::sample_f(const XMFLOAT3 &wo, XMFLOAT3 *wi, const XMFLOAT2 &u, float *pdf, BxDFType *sampledType)const
{
	// Cosine-sample the hemisphere, flipping the direction if necessary
	*wi = MathHelper::CosineSampleHemisphere(u);
	if (wo.z < 0) wi->z *= -1;
	*pdf = Pdf(wo, *wi);
	*sampledType = BxDFType::BSDF_REFLECTION;
	return f(wo, *wi);
}

float BxDF::Pdf(const XMFLOAT3 &wo, const XMFLOAT3 &wi) const
{
	return SameHemisphere(wo, wi) ? AbsCosTheta(wi) * MathHelper::InvPi : 0;
}