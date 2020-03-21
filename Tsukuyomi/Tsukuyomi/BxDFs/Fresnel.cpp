#include "Fresnel.h"
#include <iostream>
#include <algorithm>
#include <../MathHelper/MathHelper.h>

// BxDF Utility Functions
Spectrum FrDielectric(float cosThetaI, float etaI, float etaT) {
	cosThetaI = MathHelper::Clamp(cosThetaI, -1.0f, 1.0f);
	// Potentially swap indices of refraction
	bool entering = cosThetaI > 0.f;
	if (!entering) {
		std::swap(etaI, etaT);
		cosThetaI = std::abs(cosThetaI);
	}

	// Compute _cosThetaT_ using Snell's law
	float sinThetaI = std::sqrt(std::max<float>(0.0f, 1.0f - cosThetaI * cosThetaI));
	float sinThetaT = etaI / etaT * sinThetaI;

	// Handle total internal reflection
	if (sinThetaT >= 1) return Spectrum(1.0, 1.0, 1.0);
	float cosThetaT = std::sqrt(std::max<float>(0.0f, 1 - sinThetaT * sinThetaT));
	float Rparl = ((etaT * cosThetaI) - (etaI * cosThetaT)) /
		((etaT * cosThetaI) + (etaI * cosThetaT));
	float Rperp = ((etaI * cosThetaI) - (etaT * cosThetaT)) /
		((etaI * cosThetaI) + (etaT * cosThetaT));
	float val = (Rparl * Rparl + Rperp * Rperp) / 2;
	return Spectrum(val, val, val);
}

Spectrum FresnelDielectric::Evaluate(float cosThetaI) const {
	return FrDielectric(cosThetaI, etaI, etaT);
}

std::string FresnelDielectric::ToString() const {
	return std::to_string(etaI) + " " + std::to_string(etaT);
}