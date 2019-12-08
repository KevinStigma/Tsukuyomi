#pragma once
#include "Object.h"
#include "../intersect_info.h"

class Light:public Object
{
public:
	Light(std::string name, XMFLOAT3 t = XMFLOAT3(0.0, 0.0, 0.0), XMFLOAT3 s = XMFLOAT3(1.0, 1.0, 1.0), XMFLOAT3 r = XMFLOAT3(0.0, 0.0, 0.0), XMFLOAT3 color = XMFLOAT3(1.0, 1.0, 1.0));
	~Light();
	bool isLight() { return true; }
	virtual bool isDelta() { return false; }
	XMFLOAT3 getColor() { return color; }
	void setColor(XMFLOAT3 c) { color = c; }
	virtual Spectrum Le(Ray& ray) { return Spectrum(); }
	virtual Spectrum sample_li(const IntersectInfo & ref, XMFLOAT2 uSample, XMFLOAT3* wi, float* pdf, bool visibility) { return Spectrum(); };
	std::string getColorText() { return std::to_string(color.x) + "," + std::to_string(color.y) + "," + std::to_string(color.z); }
protected:
	XMFLOAT3 color;
};