#pragma once
#include "../common.h"

// MicrofacetDistribution Declarations
class MicrofacetDistribution {
public:
	// MicrofacetDistribution Public Methods
	virtual ~MicrofacetDistribution();
	virtual Float D(const XMFLOAT3 &wh) const = 0;
	virtual Float Lambda(const XMFLOAT3 &w) const = 0;
	Float G1(const XMFLOAT3 &w) const {
		//    if (Dot(w, wh) * CosTheta(w) < 0.) return 0.;
		return 1 / (1 + Lambda(w));
	}
	virtual Float G(const XMFLOAT3 &wo, const XMFLOAT3 &wi) const {
		return 1 / (1 + Lambda(wo) + Lambda(wi));
	}
	virtual XMFLOAT3 Sample_wh(const XMFLOAT3 &wo, const XMFLOAT2 &u) const = 0;
	Float Pdf(const XMFLOAT3 &wo, const XMFLOAT3 &wh) const;

protected:
	// MicrofacetDistribution Protected Methods
	MicrofacetDistribution(bool sampleVisibleArea)
		: sampleVisibleArea(sampleVisibleArea) {}

	// MicrofacetDistribution Protected Data
	const bool sampleVisibleArea;
};


class TrowbridgeReitzDistribution : public MicrofacetDistribution {
public:
	// TrowbridgeReitzDistribution Public Methods
	static inline Float RoughnessToAlpha(Float roughness);
	TrowbridgeReitzDistribution(Float alphax, Float alphay,
		bool samplevis = true)
		: MicrofacetDistribution(samplevis), alphax(alphax), alphay(alphay) {}
	Float D(const XMFLOAT3 &wh) const;
	XMFLOAT3 Sample_wh(const XMFLOAT3 &wo, const XMFLOAT2 &u) const;

private:
	// TrowbridgeReitzDistribution Private Methods
	Float Lambda(const XMFLOAT3 &w) const;

	// TrowbridgeReitzDistribution Private Data
	const Float alphax, alphay;
};

// MicrofacetDistribution Inline Methods
inline Float TrowbridgeReitzDistribution::RoughnessToAlpha(Float roughness) {
	roughness = std::max<float>(roughness, (Float)1e-3);
	Float x = std::log(roughness);
	return 1.62142f + 0.819955f * x + 0.1734f * x * x + 0.0171201f * x * x * x +
		0.000640711f * x * x * x * x;
}