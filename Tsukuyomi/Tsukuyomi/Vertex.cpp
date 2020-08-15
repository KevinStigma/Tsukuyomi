#include "Vertex.h"
#include "Effects/Effects.h"
#include "common.h"

const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::Basic32[3] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

const D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::SimpleColor[2] =
{
	{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};


ID3D11InputLayout* InputLayouts::PosNorTex = 0;
ID3D11InputLayout* InputLayouts::PosColor = 0;

void InputLayouts::initAll(ID3D11Device* device)
{
	D3DX11_PASS_DESC passDesc;
	Effects::BasicFX->CustomLightTech->GetPassByIndex(0)->GetDesc(&passDesc);
	device->CreateInputLayout(InputLayoutDesc::Basic32, 3, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &PosNorTex);

	Effects::BasicFX->SimpleColorTech->GetPassByIndex(0)->GetDesc(&passDesc);
	device->CreateInputLayout(InputLayoutDesc::SimpleColor, 2, passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &PosColor);
}

void InputLayouts::destroyAll()
{
	SAFE_DELETE(PosColor);
	SAFE_DELETE(PosNorTex);
}