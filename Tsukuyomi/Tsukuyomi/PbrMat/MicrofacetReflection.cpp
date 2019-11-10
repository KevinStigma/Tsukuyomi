#include "MicrofacetReflection.h"

MicrofacetReflection::MicrofacetReflection(const Spectrum &R, const Fresnel*f, float ax, float ay):BxDF(BxDFType(BSDF_REFLECTION|BSDF_GLOSSY)), R(R)
{
	alphax = ax;
	alphay = ay;
	fresnel = f;
}

float MicrofacetReflection::D(const XMFLOAT3&wh)const
{
	float tan2Theta = Tan2Theta(wh);
	if (std::isinf(tan2Theta)) 
		return 0.0;
	float cos2Theta = Cos2Theta(wh);
	float cos4Theta = cos2Theta * cos2Theta;
	float e = (Cos2Phi(wh) / (alphax*alphax) + Sin2Phi(wh) / (alphay*alphay))*tan2Theta;
	return 1.0f / (MathHelper::Pi*alphax * alphay * cos4Theta * (1 + e) * (1 + e));
}

float MicrofacetReflection::Lambda(const XMFLOAT3& w)const
{
	float absTanTheta = std::abs(TanTheta(w));
	if (std::isinf(absTanTheta))
		return 0;
	float alpha = std::sqrtf(Cos2Phi(w) * alphax * alphax + Sin2Phi(w) * alphay * alphay);
	float alpha2Tan2Theta = (alpha * absTanTheta) * (alpha * absTanTheta);
	return (-1.0f + std::sqrt(1.0f + alpha2Tan2Theta)) * 0.5f;
}

float MicrofacetReflection::G(const XMFLOAT3&wi, const XMFLOAT3&wo)const
{
	return 1.0f / (1.0f + Lambda(wo) + Lambda(wi));
}

float MicrofacetReflection::G1(const XMFLOAT3&w)const
{
	return 1.0f / (1.0f + Lambda(w));
}

Spectrum MicrofacetReflection::f(const XMFLOAT3 &wo, const XMFLOAT3 &wi)const
{
	float cosThetaO = AbsCosTheta(wo), cosThetaI = AbsCosTheta(wi);
	XMFLOAT3 wh(wi.x + wo.x, wi.y + wo.y, wi.z + wo.z);
	if ((cosThetaI == 0.0f || cosThetaO == 0.0f)||(wh.x==0.0&&wh.y == 0.0 &&wh.z == 0.0))
		return Spectrum();
	wh = MathHelper::NormalizeFloat3(wh);
	Spectrum F = fresnel->Evaluate(MathHelper::DotFloat3(wi, wh));
	return R * D(wh)* G(wi, wo) * F / (4.0f * cosThetaI * cosThetaO);
}

float MicrofacetReflection::Pdf(const XMFLOAT3 &wo, const XMFLOAT3 &wi) const
{
	if (!SameHemisphere(wo, wi))
		return 0.0;
	XMFLOAT3 wh = MathHelper::NormalizeFloat3(XMFLOAT3(wo.x+wi.x, wo.y + wi.y, wo.z + wi.z));
	float pdf = 0.0f;
	float dot_wo_wh = MathHelper::DotFloat3(wo, wh);
	if (sampleVisibleArea)
		pdf = D(wh) * G1(wo) * abs(dot_wo_wh) / AbsCosTheta(wo);
	else
		pdf = D(wh) * AbsCosTheta(wh);
	return pdf / (4.0 * dot_wo_wh);
}
