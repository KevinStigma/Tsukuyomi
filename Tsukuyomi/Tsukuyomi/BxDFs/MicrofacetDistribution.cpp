#include "../MathHelper/MathHelper.h"
#include "MicrofacetDistribution.h"
#include "BxDF.h"

MicrofacetDistribution::~MicrofacetDistribution() {}

Float MicrofacetDistribution::Pdf(const XMFLOAT3 &wo, const XMFLOAT3 &wh) const {
	if (sampleVisibleArea)
		return D(wh) * G1(wo) * fabs(MathHelper::DotFloat3(wo, wh)) / AbsCosTheta(wo);
	else
		return D(wh) * AbsCosTheta(wh);
}

Float TrowbridgeReitzDistribution::Lambda(const XMFLOAT3 &w) const {
	Float absTanTheta = std::abs(TanTheta(w));
	if (std::isinf(absTanTheta)) return 0.;
	// Compute _alpha_ for direction _w_
	Float alpha =
		std::sqrt(Cos2Phi(w) * alphax * alphax + Sin2Phi(w) * alphay * alphay);
	Float alpha2Tan2Theta = (alpha * absTanTheta) * (alpha * absTanTheta);
	return (-1 + std::sqrt(1.f + alpha2Tan2Theta)) / 2;
}

Float TrowbridgeReitzDistribution::D(const XMFLOAT3 &wh) const {
	Float tan2Theta = Tan2Theta(wh);
	if (std::isinf(tan2Theta)) return 0.;
	const Float cos4Theta = Cos2Theta(wh) * Cos2Theta(wh);
	Float e =
		(Cos2Phi(wh) / (alphax * alphax) + Sin2Phi(wh) / (alphay * alphay)) *
		tan2Theta;
	return 1 / (MathHelper::Pi * alphax * alphay * cos4Theta * (1 + e) * (1 + e));
}

static void TrowbridgeReitzSample11(Float cosTheta, Float U1, Float U2,
	Float *slope_x, Float *slope_y) {
	// special case (normal incidence)
	if (cosTheta > .9999) {
		Float r = sqrt(U1 / (1 - U1));
		Float phi = 6.28318530718 * U2;
		*slope_x = r * cos(phi);
		*slope_y = r * sin(phi);
		return;
	}

	Float sinTheta =
		std::sqrt(std::max((Float)0, (Float)1 - cosTheta * cosTheta));
	Float tanTheta = sinTheta / cosTheta;
	Float a = 1 / tanTheta;
	Float G1 = 2 / (1 + std::sqrt(1.f + 1.f / (a * a)));

	// sample slope_x
	Float A = 2 * U1 / G1 - 1;
	Float tmp = 1.f / (A * A - 1.f);
	if (tmp > 1e10) tmp = 1e10;
	Float B = tanTheta;
	Float D = std::sqrt(
		std::max(Float(B * B * tmp * tmp - (A * A - B * B) * tmp), Float(0)));
	Float slope_x_1 = B * tmp - D;
	Float slope_x_2 = B * tmp + D;
	*slope_x = (A < 0 || slope_x_2 > 1.f / tanTheta) ? slope_x_1 : slope_x_2;

	// sample slope_y
	Float S;
	if (U2 > 0.5f) {
		S = 1.f;
		U2 = 2.f * (U2 - .5f);
	}
	else {
		S = -1.f;
		U2 = 2.f * (.5f - U2);
	}
	Float z =
		(U2 * (U2 * (U2 * 0.27385f - 0.73369f) + 0.46341f)) /
		(U2 * (U2 * (U2 * 0.093073f + 0.309420f) - 1.000000f) + 0.597999f);
	*slope_y = S * z * std::sqrt(1.f + *slope_x * *slope_x);
}

static XMFLOAT3 TrowbridgeReitzSample(const XMFLOAT3 &wi, Float alpha_x,
	Float alpha_y, Float U1, Float U2) {
	// 1. stretch wi
	XMFLOAT3 wiStretched =
		MathHelper::NormalizeFloat3(XMFLOAT3(alpha_x * wi.x, alpha_y * wi.y, wi.z));

	// 2. simulate P22_{wi}(x_slope, y_slope, 1, 1)
	Float slope_x, slope_y;
	TrowbridgeReitzSample11(CosTheta(wiStretched), U1, U2, &slope_x, &slope_y);

	// 3. rotate
	Float tmp = CosPhi(wiStretched) * slope_x - SinPhi(wiStretched) * slope_y;
	slope_y = SinPhi(wiStretched) * slope_x + CosPhi(wiStretched) * slope_y;
	slope_x = tmp;

	// 4. unstretch
	slope_x = alpha_x * slope_x;
	slope_y = alpha_y * slope_y;

	// 5. compute normal
	return MathHelper::NormalizeFloat3(XMFLOAT3(-slope_x, -slope_y, 1.));
}

XMFLOAT3 TrowbridgeReitzDistribution::Sample_wh(const XMFLOAT3 &wo,
	const XMFLOAT2 &u) const {
	XMFLOAT3 wh;
	float pi = MathHelper::Pi;
	if (!sampleVisibleArea) {
		Float cosTheta = 0, phi = (2 * pi) * u.y;
		if (alphax == alphay) {
			Float tanTheta2 = alphax * alphax * u.x / (1.0f - u.x);
			cosTheta = 1 / std::sqrt(1 + tanTheta2);
		}
		else {
			phi =
				std::atan(alphay / alphax * std::tan(2 * pi * u.y + .5f * pi));
			if (u.y > .5f) phi += pi;
			Float sinPhi = std::sin(phi), cosPhi = std::cos(phi);
			const Float alphax2 = alphax * alphax, alphay2 = alphay * alphay;
			const Float alpha2 =
				1 / (cosPhi * cosPhi / alphax2 + sinPhi * sinPhi / alphay2);
			Float tanTheta2 = alpha2 * u.x / (1 - u.x);
			cosTheta = 1 / std::sqrt(1 + tanTheta2);
		}
		Float sinTheta =
			std::sqrt(std::max((Float)0., (Float)1. - cosTheta * cosTheta));
		wh = MathHelper::SphericalDirection(sinTheta, cosTheta, phi);
		if (!SameHemisphere(wo, wh)) wh = MathHelper::NegativeFloat3(wh);
	}
	else {
		bool flip = wo.z < 0;
		wh = TrowbridgeReitzSample(flip ? MathHelper::NegativeFloat3(wo) : wo, alphax, alphay, u.x, u.y);
		if (flip) 
			wh = MathHelper::NegativeFloat3(wh);
	}
	return wh;
}
