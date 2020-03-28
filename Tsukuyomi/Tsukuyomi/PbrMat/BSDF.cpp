#include "BSDF.h"

BSDF::~BSDF()
{
	for (int i = 0; i < nBxDFs; i++)
		SAFE_DELETE(bxdfs[i]);
}

int BSDF::NumComponents(BxDFType flags) const
{
	int num = 0;
	for (int i = 0; i < nBxDFs; ++i)
		if (bxdfs[i]->MatchesFlags(flags)) 
			num++;
	return num;
}

XMFLOAT3 BSDF::WorldToLocal(const XMFLOAT3 &v) const
{
	return XMFLOAT3(MathHelper::DotFloat3(v, ss), MathHelper::DotFloat3(v, ts), MathHelper::DotFloat3(v, normal));
}

XMFLOAT3 BSDF::LocalToWorld(const XMFLOAT3 &v) const
{
	return XMFLOAT3(ss.x * v.x + ts.x * v.y + normal.x * v.z,
		ss.y * v.x + ts.y * v.y + normal.y * v.z,
		ss.z * v.x + ts.z * v.y + normal.z * v.z);
}

Spectrum BSDF::f(const XMFLOAT3 &woW, const XMFLOAT3 &wiW, BxDFType flags) const
{
	XMFLOAT3 wi = WorldToLocal(wiW), wo = WorldToLocal(woW);
	if (wo.z == 0) return Spectrum(0);
	bool reflect = MathHelper::DotFloat3(wiW, normal) *  MathHelper::DotFloat3(woW, normal) > 0.0;
	Spectrum f(0.f);
	for (int i = 0; i < nBxDFs; ++i)
		if (bxdfs[i]->MatchesFlags(flags) &&
			((reflect && (bxdfs[i]->type & BSDF_REFLECTION)) ||
			(!reflect && (bxdfs[i]->type & BSDF_TRANSMISSION))))
			f += bxdfs[i]->f(wo, wi);
	return f;
}

Spectrum BSDF::Sample_f(const XMFLOAT3 &woWorld, XMFLOAT3 *wiWorld, const XMFLOAT2 &u, float *pdf, BxDFType type, BxDFType *sampledType) const
{
	// Choose which _BxDF_ to sample
	int matchingComps = NumComponents(type);
	if (matchingComps == 0) {
		*pdf = 0;
		if (sampledType) *sampledType = BxDFType(0);
		return Spectrum(0);
	}
	int comp = std::min<int>((int)std::floor(u.x * matchingComps), matchingComps - 1);

	// Get _BxDF_ pointer for chosen component
	BxDF *bxdf = nullptr;
	int count = comp;
	for (int i = 0; i < nBxDFs; ++i)
		if (bxdfs[i]->MatchesFlags(type) && count-- == 0) {
			bxdf = bxdfs[i];
			break;
		}

	// Remap _BxDF_ sample _u_ to $[0,1)^2$
	XMFLOAT2 uRemapped(std::min<float>(u.x * matchingComps - comp, 0x1.fffffep-1), u.y);

	// Sample chosen _BxDF_
	XMFLOAT3 wi, wo = WorldToLocal(woWorld);
	if (wo.z == 0) 
		return Spectrum(0);
	*pdf = 0;
	if (sampledType) *sampledType = bxdf->type;
	Spectrum f = bxdf->sample_f(wo, &wi, uRemapped, pdf, sampledType);
	if (*pdf == 0) {
		if (sampledType) *sampledType = BxDFType(0);
		return Spectrum(0);
	}
	*wiWorld = LocalToWorld(wi);

	// Compute overall PDF with all matching _BxDF_s
	if (!(bxdf->type & BSDF_SPECULAR) && matchingComps > 1)
		for (int i = 0; i < nBxDFs; ++i)
			if (bxdfs[i] != bxdf && bxdfs[i]->MatchesFlags(type))
				*pdf += bxdfs[i]->Pdf(wo, wi);
	if (matchingComps > 1) *pdf /= matchingComps;

	// Compute value of BSDF for sampled direction
	if (!(bxdf->type & BSDF_SPECULAR)) {
		bool reflect = MathHelper::DotFloat3(*wiWorld, normal) * MathHelper::DotFloat3(woWorld, normal) > 0;
		f = 0.;
		for (int i = 0; i < nBxDFs; ++i)
			if (bxdfs[i]->MatchesFlags(type) &&
				((reflect && (bxdfs[i]->type & BSDF_REFLECTION)) ||
				(!reflect && (bxdfs[i]->type & BSDF_TRANSMISSION))))
				f += bxdfs[i]->f(wo, wi);
	}
	return f;
}

float BSDF::Pdf(const XMFLOAT3 &woWorld, const XMFLOAT3 &wiWorld, BxDFType flags) const
{
	if (nBxDFs == 0.f) return 0.f;
	XMFLOAT3 wo = WorldToLocal(woWorld), wi = WorldToLocal(wiWorld);
	if (wo.z == 0) return 0.;
	float pdf = 0.f;
	int matchingComps = 0;
	for (int i = 0; i < nBxDFs; ++i)
		if (bxdfs[i]->MatchesFlags(flags)) {
			++matchingComps;
			pdf += bxdfs[i]->Pdf(wo, wi);
		}
	float v = matchingComps > 0 ? pdf / matchingComps : 0.f;
	return v;
}

std::string BSDF::ToString() const
{
	return "BSDF";
}
