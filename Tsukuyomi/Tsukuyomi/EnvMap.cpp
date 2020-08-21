#include "EnvMap.h"
#include "Vertex.h"
#include "common.h"
#include "Effects\Effects.h"
#include "Objects\Camera.h"
#include <GeometryGenerator\GeometryGenerator.h>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include <stb_image.h>

EnvironmentMap::EnvironmentMap(std::string path, ID3D11Device* d, ID3D11DeviceContext* dc): device(d), context(dc)
{
	stbi_set_flip_vertically_on_load(true);
	int nrComponents;
	data = stbi_loadf(path.c_str(), &width, &height, &nrComponents, 0);
	std::cout << sizeof(data) << " " << width << " " << height << std::endl;

	createBuffers();
	createEnvironmentMapSRV();

	mat.albedo = XMFLOAT3(1.0, 1.0, 1.0);
	mat.metallic = 0.0;
	mat.roughness = 1.0;
}

EnvironmentMap::~EnvironmentMap()
{
	stbi_image_free(data);
	data = nullptr;
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
	SAFE_RELEASE(environmentSRV);
}

void EnvironmentMap::createEnvironmentMapSRV()
{
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = width;
	texDesc.Height = height;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_IMMUTABLE;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = { 0 };
	initData.SysMemPitch = width * sizeof(XMFLOAT3);

	initData.pSysMem = data;

	ID3D11Texture2D* tex = 0;
	device->CreateTexture2D(&texDesc, &initData, &tex);

	device->CreateShaderResourceView(tex, 0, &environmentSRV);
	ReleaseCOM(tex);
}

void EnvironmentMap::renderEnvironmentMap(Camera* camera)
{
	EnvMapEffect* envMapEffect = Effects::EnvMapFX;

	UINT stride = sizeof(Basic32);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetInputLayout(InputLayouts::PosNorTex);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ID3DX11EffectTechnique* activeTech = envMapEffect->EnvMapTech;

	D3DX11_TECHNIQUE_DESC techDesc;
	activeTech->GetDesc(&techDesc);

	XMVECTOR v;
	XMMATRIX worldMat = XMMatrixScaling(1000.0, 1000.0, 1000.0);
	XMMATRIX inv_world_mat = XMMatrixTranspose(XMMatrixInverse(&v, worldMat));
	XMMATRIX WVP = worldMat * camera->getViewMatrix() * camera->getProjMatrix();
	XMMATRIX WVPT = WVP;
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		envMapEffect->SetWorld(worldMat);
		envMapEffect->SetWorldInvTranspose(inv_world_mat);
		envMapEffect->SetWorldViewProj(WVP);
		envMapEffect->SetMaterial(mat);
		envMapEffect->SetEnvMap(environmentSRV);
		activeTech->GetPassByIndex(p)->Apply(0, context);
		context->DrawIndexed(sphereIndexCount, 0, 0);
	}
	context->RSSetState(0);
}

void EnvironmentMap::createBuffers()
{
	GeometryGenerator geoGen;
	GeometryGenerator::MeshData sphere_data;
	geoGen.CreateSphere(1.0, 25, 25, sphere_data);

	D3D11_BUFFER_DESC buffDesc = {};
	D3D11_SUBRESOURCE_DATA initData = {};

	std::vector<Basic32> vertices;
	for (int i = 0; i < sphere_data.Vertices.size(); i++)
	{
		GeometryGenerator::Vertex v = sphere_data.Vertices[i];
		vertices.push_back(Basic32(v.Position.x, v.Position.y, v.Position.z, v.Normal.x, v.Normal.y, v.Normal.z, v.TexC.x, v.TexC.y));
	}

	ZeroMemory(&buffDesc, sizeof(buffDesc));
	buffDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	buffDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	buffDesc.CPUAccessFlags = 0;
	buffDesc.ByteWidth = sizeof(Basic32) * vertices.size();

	initData.pSysMem = vertices.data();

	if (FAILED(device->CreateBuffer(&buffDesc, &initData, &vertexBuffer)))
		return;

	sphereIndexCount = sphere_data.Indices.size();
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(unsigned int)* sphere_data.Indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &sphere_data.Indices[0];
	if (FAILED(device->CreateBuffer(&ibd, &iinitData, &indexBuffer)))
		return;
}