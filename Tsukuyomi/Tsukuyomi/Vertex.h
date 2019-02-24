#pragma once
#include <directxmath.h>
#include <d3d11_1.h>
using namespace DirectX;

struct SimpleVertex
{
	XMFLOAT3 Pos;
	XMFLOAT4 Color;
};

// Basic 32-byte vertex structure.
struct Basic32
{
	XMFLOAT3 pos;
	XMFLOAT3 normal;
	XMFLOAT2 tex;
	Basic32(float px = 0, float py = 0, float pz = 0, float nx = 0, float ny = 0, float nz = 0, float tu = 0, float tv = 0)
	{
		pos = XMFLOAT3(px, py, pz);
		normal = XMFLOAT3(nx, ny, nz);
		tex = XMFLOAT2(tu, tv);
	}
};

class InputLayoutDesc
{
public:
	static const D3D11_INPUT_ELEMENT_DESC Basic32[3];
};


class InputLayouts
{
public:
	static void initAll(ID3D11Device* device);
	static void destroyAll();
	static ID3D11InputLayout* Pos;
	static ID3D11InputLayout* PosNorTex;
};