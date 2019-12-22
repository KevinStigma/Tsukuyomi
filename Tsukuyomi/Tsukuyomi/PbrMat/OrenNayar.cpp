#include <algorithm>
#include "OrenNayar.h"
#include "../MathHelper\MathHelper.h"

OrenNayar::OrenNayar(const Spectrum &R, float sigma):BxDF(BxDFType(BSDF_REFLECTION|BSDF_DIFFUSE)), R(R)
{
	sigma = MathHelper::Degree2Radian(sigma);
	float sigma2 = sigma * sigma;
	A = 1.0f - (sigma2 / 2.0f*(sigma2 + 0.33f));
	B = 0.45f * sigma2 / (sigma2 + 0.09f);
}

Spectrum OrenNayar::f(const XMFLOAT3 &wo, const XMFLOAT3 &wi) const
{
	if (!(SameHemisphere(wo, wi)&&SameHemisphereWithNormal(wo)))
		return Spectrum();
	float sinThetaI = SinTheta(wi);
	float sinThetaO = SinTheta(wo);
	float max_cos = 0.0;
	if (sinThetaI > 1e-4 && sinThetaO > 1e-4)
	{
		float sinPhiI = SinPhi(wi), cosPhiI = CosPhi(wi);
		float shiPhiO = SinPhi(wo), cosPhiO = CosPhi(wo);
		float dcos = cosPhiI * cosPhiO + sinPhiI * shiPhiO;
		max_cos = std::max<float>(0.0f, dcos);
	}

	float sinAlpha, tanBeta;
	if (AbsCosTheta(wi) > AbsCosTheta(wo))
	{
		sinAlpha = sinThetaO;
		tanBeta = sinThetaI / AbsCosTheta(wi);
	}
	else
	{
		sinAlpha = sinThetaI;
		tanBeta = sinThetaO / AbsCosTheta(wo);
	}
	return R * MathHelper::InvPi * (A + B * max_cos * sinAlpha * tanBeta);
}