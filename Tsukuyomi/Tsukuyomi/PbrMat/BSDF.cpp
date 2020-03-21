#include "BSDF.h"

int BSDF::NumComponents(BxDFType flags) const
{
	return 0;
}

XMFLOAT3 BSDF::WorldToLocal(const XMFLOAT3 &v) const
{
	return XMFLOAT3();
}

XMFLOAT3 BSDF::LocalToWorld(const XMFLOAT3 &v) const
{
	return XMFLOAT3();
}

Spectrum BSDF::f(const XMFLOAT3 &woW, const XMFLOAT3 &wiW, BxDFType flags) const
{
	return Spectrum();
}

Spectrum BSDF::rho(int nSamples, const XMFLOAT2 *samples1, const XMFLOAT2 *samples2, BxDFType flags) const
{
	return Spectrum();
}

Spectrum BSDF::rho(const XMFLOAT2 &wo, int nSamples, const XMFLOAT2 *samples, BxDFType flags) const
{
	return Spectrum();
}

Spectrum BSDF::Sample_f(const XMFLOAT3 &wo, XMFLOAT3 *wi, const XMFLOAT2 &u, float *pdf, BxDFType type, BxDFType *sampledType) const
{
	return Spectrum();
}

float BSDF::Pdf(const XMFLOAT3 &wo, const XMFLOAT3 &wi, BxDFType flags) const
{
	return 0.0f;
}

std::string BSDF::ToString() const
{
	return "";
}
