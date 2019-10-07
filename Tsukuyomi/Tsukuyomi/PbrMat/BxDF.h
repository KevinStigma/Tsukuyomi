#pragma once
#include "../common.h"
#include "../RGBSpectrum.h"
#include "../MathHelper/MathHelper.h"

enum BxDFType {
	BSDF_REFLECTION = 1 << 0,
	BSDF_TRANSMISSION = 1 << 1,
	BSDF_DIFFUSE = 1 << 2,
	BSDF_GLOSSY = 1 << 3,
	BSDF_SPECULAR = 1 << 4,
	BSDF_ALL = BSDF_DIFFUSE | BSDF_GLOSSY | BSDF_SPECULAR | BSDF_REFLECTION | BSDF_TRANSMISSION,
};

inline bool SameHemisphere(const XMFLOAT3 &w, const XMFLOAT3 &wp) {
	return w.z * wp.z > 0;
}

// BSDF Inline Functions
inline float CosTheta(const XMFLOAT3 &w) { return w.z; }
inline float Cos2Theta(const XMFLOAT3 &w) { return w.z * w.z; }
inline float AbsCosTheta(const XMFLOAT3 &w) { return std::abs(w.z); }
inline float Sin2Theta(const XMFLOAT3 &w) {
	return std::max<float>((float)0, (float)1 - Cos2Theta(w));
}

inline float SinTheta(const XMFLOAT3 &w) { return std::sqrt(Sin2Theta(w)); }

inline float TanTheta(const XMFLOAT3 &w) { return SinTheta(w) / CosTheta(w); }

inline float Tan2Theta(const XMFLOAT3 &w) {
	return Sin2Theta(w) / Cos2Theta(w);
}

inline float CosPhi(const XMFLOAT3 &w) {
	float sinTheta = SinTheta(w);
	return (sinTheta == 0) ? 1 : MathHelper::Clamp<float>(w.x / sinTheta, -1, 1);
}

inline float SinPhi(const XMFLOAT3 &w) {
	float sinTheta = SinTheta(w);
	return (sinTheta == 0) ? 0 : MathHelper::Clamp<float>(w.y / sinTheta, -1, 1);
}

inline float Cos2Phi(const XMFLOAT3 &w) { return CosPhi(w) * CosPhi(w); }

inline float Sin2Phi(const XMFLOAT3 &w) { return SinPhi(w) * SinPhi(w); }

class BxDF {
public:
	virtual ~BxDF() {}
	BxDF(BxDFType type) : type(type) {}
	bool MatchesFlags(BxDFType t) const { return (type & t) == type; }
	virtual Spectrum f(const XMFLOAT3 &wo, const XMFLOAT3 &wi) const = 0;
	virtual Spectrum sample_f(const XMFLOAT3 &wo, XMFLOAT3 *wi, const XMFLOAT2 &sample, float *pdf, BxDFType *sampledType = nullptr) const;
	virtual float Pdf(const XMFLOAT3 &wo, const XMFLOAT3 &wi) const;
	virtual std::string toString() const = 0;

	const BxDFType type;
};