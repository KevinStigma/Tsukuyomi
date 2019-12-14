#pragma once
#include "common.h"

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
	void operator+=(const RGBSpectrum & s)const
	{
		r + s.r;g + s.g; b + s.b;
	}
	void operator-=(const RGBSpectrum & s)const
	{
		r - s.r; g - s.g; b - s.b;
	}
	void operator*=(const RGBSpectrum & s)const
	{
		r * s.r; g * s.g; b * s.b;
	}
	void operator/=(const RGBSpectrum & s)const
	{
		r / s.r; g / s.g; b / s.b;
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
	void operator+=(float v)const
	{
		r + v; g + v; b + v;
	}
	void operator-=(float v)const
	{
		r - v; g - v; b - v;
	}
	void operator*=(float v)const
	{
		r * v; g * v; b * v;
	}
	void operator/=(float v)const
	{
		r / v; g / v; b / v;
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

	bool isBlack()
	{
		return r == 0.0 && b == 0.0&&g == 0.0;
	}

	float y() const {
		const float YWeight[3] = { 0.212671f, 0.715160f, 0.072169f };
		return YWeight[0] * r + YWeight[1] * g + YWeight[2] * b;
	}

	float r, g, b;
};

typedef RGBSpectrum Spectrum;