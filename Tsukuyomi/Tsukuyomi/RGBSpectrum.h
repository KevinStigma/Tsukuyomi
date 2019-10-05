#pragma once
#include "common.h"

struct RGBSpectrum
{
	RGBSpectrum(float r1=0.0, float g1=0.0, float b1=0.0):r(r1), g(g1), b(b1){}
	RGBSpectrum operator+(RGBSpectrum s)
	{
		return RGBSpectrum(r+s.r, g+s.g, b+s.b);
	}
	RGBSpectrum operator-(RGBSpectrum s)
	{
		return RGBSpectrum(r - s.r, g - s.g, b - s.b);
	}
	RGBSpectrum operator*(RGBSpectrum s)
	{
		return RGBSpectrum(r * s.r, g * s.g, b * s.b);
	}
	RGBSpectrum operator/(RGBSpectrum s)
	{
		return RGBSpectrum(r / s.r, g / s.g, b / s.b);
	}
	RGBSpectrum operator+(float n)
	{
		return RGBSpectrum(n + r, n + g, b + n);
	}
	RGBSpectrum operator-(float n)
	{
		return RGBSpectrum(r - n, g - n, b - n);
	}
	RGBSpectrum operator*(float n)
	{
		return RGBSpectrum(r * n, g * n, b * n);
	}
	RGBSpectrum operator/(float n)
	{
		return RGBSpectrum(r / n, g / n, b / n);
	}
	friend RGBSpectrum operator+(float n, RGBSpectrum s)
	{
		return RGBSpectrum(n+ s.r, n + s.g, n + s.b);
	}
	friend RGBSpectrum operator*(float n, RGBSpectrum s)
	{
		return RGBSpectrum(n * s.r, n * s.g, n * s.b);
	}
	friend RGBSpectrum operator-(float n, RGBSpectrum s)
	{
		return RGBSpectrum(n - s.r, n - s.g, n - s.b);
	}

	float r, g, b;
};

typedef RGBSpectrum Spectrum;