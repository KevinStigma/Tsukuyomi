#pragma once
#include "Object.h"
class Light:public Object
{
public:
	Light(std::string name, XMFLOAT3 t = XMFLOAT3(0.0, 0.0, 0.0), XMFLOAT3 s = XMFLOAT3(1.0, 1.0, 1.0), XMFLOAT3 r = XMFLOAT3(0.0, 0.0, 0.0), XMFLOAT3 color = XMFLOAT3(1.0, 1.0, 1.0));
	~Light();
	XMFLOAT3 getColor() { return color; }
	void setColor(XMFLOAT3 c) { color = c; }
	std::string getColorText() { return std::to_string(color.x) + "," + std::to_string(color.y) + "," + std::to_string(color.z); }
protected:
	XMFLOAT3 color;
};