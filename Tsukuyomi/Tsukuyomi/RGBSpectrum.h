#pragma once
#include "common.h"
#include "MathHelper/MathHelper.h"

struct RGBSpectrum
{
	RGBSpectrum(float r1=0.0, float g1=0.0, float b1=0.0):r(r1), g(g1), b(b1){}
	RGBSpectrum operator+(const RGBSpectrum & s)const
	{
		return RGBSpectrum(r+s.r, g+s.g, b+s.b);
	}
	RGBSpectrum operator-(const RGBSpectrum & s)const
	{
		return RGBSpectrum(r - s.r, g - s.g, b - s.b);
	}
	RGBSpectrum operator*(const RGBSpectrum & s)const
	{
		return RGBSpectrum(r * s.r, g * s.g, b * s.b);
	}
	RGBSpectrum operator/(const RGBSpectrum & s)const
	{
		return RGBSpectrum(r / s.r, g / s.g, b / s.b);
	}
	void operator+=(const RGBSpectrum & s)
	{
		r = r + s.r;
		g = g + s.g;
		b = b + s.b;
	}
	void operator-=(const RGBSpectrum & s)
	{
		r = r - s.r;
		g = g - s.g;
		b = b - s.b;
	}
	void operator*=(const RGBSpectrum & s)
	{
		r = r * s.r; 
		g = g * s.g; 
		b = b * s.b;
	}
	void operator/=(const RGBSpectrum & s)
	{
		r = r / s.r;
		g = g / s.g;
		b = b / s.b;
	}
	RGBSpectrum operator+(const float & n)const
	{
		return RGBSpectrum(n + r, n + g, b + n);
	}
	RGBSpectrum operator-(const float & n)const
	{
		return RGBSpectrum(r - n, g - n, b - n);
	}
	RGBSpectrum operator*(const float & n)const
	{
		return RGBSpectrum(r * n, g * n, b * n);
	}
	RGBSpectrum operator/(const float & n)const
	{
		return RGBSpectrum(r / n, g / n, b / n);
	}
	void operator+=(float v)
	{
		r = r + v;
		g = g + v;
		b = b + v;
	}
	void operator-=(float v)
	{
		r = r - v;
		g = g - v;
		b = b - v;
	}
	void operator*=(float v)
	{
		r = r * v;
		g = g * v;
		b = b * v;
	}
	void operator/=(float v)
	{
		r = r / v;
		g = g / v;
		b = b / v;
	}
	friend RGBSpectrum operator+(const float & n,const RGBSpectrum & s)
	{
		return RGBSpectrum(n+ s.r, n + s.g, n + s.b);
	}
	friend RGBSpectrum operator*(const float & n, const RGBSpectrum & s)
	{
		return RGBSpectrum(n * s.r, n * s.g, n * s.b);
	}
	friend RGBSpectrum operator-(const float & n, const RGBSpectrum & s)
	{
		return RGBSpectrum(n - s.r, n - s.g, n - s.b);
	}

	bool isBlack()const
	{
		return r == 0.0 && b == 0.0&&g == 0.0;
	}

	float y() const {
		const float YWeight[3] = { 0.212671f, 0.715160f, 0.072169f };
		return YWeight[0] * r + YWeight[1] * g + YWeight[2] * b;
	}

	XMFLOAT3 getFloat3() { return XMFLOAT3(r, g, b); }

	static RGBSpectrum Clamp(RGBSpectrum c)
	{
		return RGBSpectrum(MathHelper::Clamp(c.r, 0.0f, 1.0f), MathHelper::Clamp(c.g, 0.0f, 1.0f), MathHelper::Clamp(c.b, 0.0f, 1.0f));
	}
	float r, g, b;

	static XMFLOAT3 XYZToRGB(const XMFLOAT3 xyz) {
		return XMFLOAT3(3.240479f * xyz.x - 1.537150f * xyz.y - 0.498535f * xyz.z,
			-0.969256f * xyz.x + 1.875991f * xyz.y + 0.041556f * xyz.z, 
			0.055648f * xyz.x - 0.204043f * xyz.y + 1.057311f * xyz.z);
	}

	static XMFLOAT3 RGBToXYZ(const XMFLOAT3 rgb) {
		return XMFLOAT3(0.412453f * rgb.x + 0.357580f * rgb.y + 0.180423f * rgb.z,
		0.212671f * rgb.x + 0.715160f * rgb.y + 0.072169f * rgb.z,
		0.019334f * rgb.x + 0.119193f * rgb.y + 0.950227f * rgb.z);
	}

	static float GammaCorrect(float value) {
		if (value <= 0.0031308f) 
			return 12.92f * value;
		return 1.055f * std::pow(value, (float)(1.f / 2.4f)) - 0.055f;
	}
};

typedef RGBSpectrum Spectrum;