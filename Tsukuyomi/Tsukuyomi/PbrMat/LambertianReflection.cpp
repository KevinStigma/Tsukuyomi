#include "LambertianReflection.h"
#include "../MathHelper/MathHelper.h"

LambertianReflection::LambertianReflection(const Spectrum &R):BxDF(BxDFType(BSDF_REFLECTION|BSDF_DIFFUSE)), R(R)
{
}

Spectrum LambertianReflection::f(const XMFLOAT3 &wo, const XMFLOAT3 &wi)
{
	return R * MathHelper::InvPi;
}