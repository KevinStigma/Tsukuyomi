#pragma once
#include "../RGBSpectrum.h"

class Fresnel {
public:
	// Fresnel Interface
	virtual ~Fresnel() {}
	virtual Spectrum Evaluate(float cosI) const = 0;
	virtual std::string ToString() const = 0;
};


class FresnelDielectric : public Fresnel {
public:
	// FresnelDielectric Public Methods
	Spectrum Evaluate(float cosThetaI) const;
	FresnelDielectric(float etaI, float etaT) : etaI(etaI), etaT(etaT) {}
	std::string ToString() const;

private:
	float etaI, etaT;
};