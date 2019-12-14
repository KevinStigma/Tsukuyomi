#pragma once
#include "../common.h"
#include "../MathHelper/MathHelper.h"
#include "tiny_obj_loader.h"
#include <string>
#include <windows.h>
#include <d3d11_1.h>
#include <DirectXMath.h>

class VisibilityTester
{
public:
	VisibilityTester() {}
	void setup(XMFLOAT3 a, XMFLOAT3 b) { p1 = a; p2 = b; }
	bool unoccluded();
protected:
	XMFLOAT3 p1, p2;
};
