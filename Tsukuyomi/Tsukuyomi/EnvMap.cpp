#include "EnvMap.h"
#include "Vertex.h"
#include "common.h"
#include "Effects\Effects.h"
#include "Objects\Camera.h"
#include <QFileInfo>
#include <DDSTextureLoader.h>
#include <GeometryGenerator\GeometryGenerator.h>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include <stb_image.h>

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#endif
#include <stb_image_write.h>

EnvironmentMap::EnvironmentMap(std::string path, ID3D11Device* d, ID3D11DeviceContext* dc): device(d), context(dc)
{
	stbi_set_flip_vertically_on_load(true);
	int nrComponents;
	data = stbi_loadf(path.c_str(), &width, &height, &nrComponents, 0);

	createBuffers();
	createEnvironmentMapSRV();

	mat.albedo = XMFLOAT3(1.0, 1.0, 1.0);
	mat.metallic = 0.0;
	mat.roughness = 1.0;

	hdr_path = path;
	ira_path = EnvironmentMap::genIrradianceMapPath(path);
	pre_filter_path = EnvironmentMap::genPreFilterMapPath(path);
	createIrradianceMapResource(false);
	createPreFilterEnvironmentMapResource();

	CreateDDSTextureFromFile(device, L"./Data/ibl_brdf_lut.dds", nullptr, &brdfLUTSRV);
}

EnvironmentMap::~EnvironmentMap()
{
	stbi_image_free(data);
	data = nullptr;
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
	SAFE_RELEASE(environmentSRV);
	SAFE_RELEASE(irradianceSRV);
	SAFE_RELEASE(irradianceRTV);
	SAFE_RELEASE(preFileterSRV);
	SAFE_RELEASE(brdfLUTSRV);
	clearBakedPreFilterMaps();
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
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA initData = { 0 };
	initData.SysMemPitch = width * sizeof(XMFLOAT3);

	initData.pSysMem = data;

	ID3D11Texture2D* tex = 0;
	device->CreateTexture2D(&texDesc, &initData, &tex);

	HRESULT hr = device->CreateShaderResourceView(tex, 0, &environmentSRV);
	ReleaseCOM(tex);

	initData.SysMemPitch = width * sizeof(XMFLOAT3);
	initData.pSysMem = data;
}

void EnvironmentMap::createPreFilterMaps()
{
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = 512;
	texDesc.Height = 512;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;
	
	ID3D11Texture2D* tex = 0;
	bakedPreFilterMaps.resize(max_mipmap_levels);

	for(int i=0; i<max_mipmap_levels; i++)
	{
		texDesc.Width /= 2;
		texDesc.Height /= 2;
		device->CreateTexture2D(&texDesc, 0, &tex);
		device->CreateShaderResourceView(tex, 0, &bakedPreFilterMaps[i].first);
		device->CreateRenderTargetView(tex, 0, &bakedPreFilterMaps[i].second);
		SAFE_RELEASE(tex);
	}

	preFilterMapsViewPort.TopLeftX = 0.0f;
	preFilterMapsViewPort.TopLeftY = 0.0f;
	preFilterMapsViewPort.Width = texDesc.Width;
	preFilterMapsViewPort.Height = texDesc.Height;
	preFilterMapsViewPort.MinDepth = 0.0f;
	preFilterMapsViewPort.MaxDepth = 1.0f;
}

void EnvironmentMap::clearBakedPreFilterMaps()
{
	for (int i = 0; i < bakedPreFilterMaps.size(); i++)
	{
		SAFE_RELEASE(bakedPreFilterMaps[i].first);
		SAFE_RELEASE(bakedPreFilterMaps[i].second);
	}
	bakedPreFilterMaps.clear();
}

void EnvironmentMap::createPreFilterEnvironmentMapResource()
{
	QFileInfo f(pre_filter_path.c_str());
	if (!f.isFile())
		return;

	wchar_t * w_file_name = new wchar_t[pre_filter_path.size() + 2];
	wmemset(w_file_name, 0, pre_filter_path.size() + 2);
	MultiByteToWideChar(CP_ACP, 0, pre_filter_path.c_str(), pre_filter_path.size(), w_file_name, pre_filter_path.size());

	auto hr = CreateDDSTextureFromFile(device, w_file_name, nullptr, &preFileterSRV);
	SAFE_DELETE(w_file_name);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"create pre filter map failed!", L"error", MB_OK);
		return;
	}


}

void EnvironmentMap::createIrradianceMapResource(bool is_baking)
{
	float* d = nullptr;
	int w = width * 0.1, h = height * 0.1;

	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = w;
	texDesc.Height = h;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	ID3D11Texture2D* tex = 0;
	if (!is_baking)
	{
		QFileInfo f(ira_path.c_str());
		if (!f.isFile())
			return;
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		texDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		int nrComponents;
		d = stbi_loadf(ira_path.c_str(), &w, &h, &nrComponents, 0);
		texDesc.Width = w;
		texDesc.Height = h;
		D3D11_SUBRESOURCE_DATA initData = { 0 };
		initData.SysMemPitch = w * sizeof(XMFLOAT3);
		initData.pSysMem = d;
		device->CreateTexture2D(&texDesc, &initData, &tex);
		device->CreateShaderResourceView(tex, 0, &irradianceSRV);
		ReleaseCOM(tex);
		stbi_image_free(d);
	}
	else
	{
		irradianceViewPort.TopLeftX = 0.0f;
		irradianceViewPort.TopLeftY = 0.0f;
		irradianceViewPort.Width = w;
		irradianceViewPort.Height = h;
		irradianceViewPort.MinDepth = 0.0f;
		irradianceViewPort.MaxDepth = 1.0f;

		device->CreateTexture2D(&texDesc, 0, &tex);
		device->CreateShaderResourceView(tex, 0, &irradianceSRV);
		device->CreateRenderTargetView(tex, 0, &irradianceRTV);
		ReleaseCOM(tex);
	}
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

void EnvironmentMap::bakeIrradiance(ID3D11Buffer* quadVertexBuffer, ID3D11Buffer* quadIndexBuffer)
{
	if (irradianceSRV)
		return;

	BakeIrradianceEffect* bakeEffect = Effects::BakeIrradianceFX;

	createIrradianceMapResource(true);

	ID3D11RenderTargetView* renderTargets[1] = { irradianceRTV };
	context->OMSetRenderTargets(1, renderTargets, 0);
	context->ClearRenderTargetView(irradianceRTV, reinterpret_cast<const float*>(&Colors::Black));
	context->RSSetViewports(1, &irradianceViewPort);

	ID3DX11EffectTechnique* tech = bakeEffect->BakeIrradianceTech;

	UINT stride = sizeof(Basic32);
	UINT offset = 0;

	context->IASetInputLayout(InputLayouts::PosNorTex);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetVertexBuffers(0, 1, &quadVertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(quadIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	bakeEffect->SetEnvironmentMap(environmentSRV);

	D3DX11_TECHNIQUE_DESC techDesc;
	tech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		tech->GetPassByIndex(p)->Apply(0, context);
		context->DrawIndexed(6, 0, 0);
	}
}

void EnvironmentMap::bakePreFilterMap(ID3D11Buffer* quadVertexBuffer, ID3D11Buffer* quadIndexBuffer)
{
	createPreFilterMaps();

	ID3D11Resource* res = nullptr;
	ID3D11Texture2D* tex = nullptr;

	for (int i = 0; i < max_mipmap_levels; i++)
	{
		BakePreFilterEnvMapEffect* bakeEffect = Effects::BakePreFilterMapFX;

		auto preFilterSRV = bakedPreFilterMaps[i].first;
		preFilterSRV->GetResource(&res);

		res->QueryInterface(&tex);
		D3D11_TEXTURE2D_DESC desc;
		tex->GetDesc(&desc);

		preFilterMapsViewPort.Width = desc.Width;
		preFilterMapsViewPort.Height = desc.Height;

		ID3D11RenderTargetView* renderTargets[1] = { bakedPreFilterMaps[i].second };
		context->OMSetRenderTargets(1, renderTargets, 0);
		context->ClearRenderTargetView(bakedPreFilterMaps[i].second, reinterpret_cast<const float*>(&Colors::Black));
		context->RSSetViewports(1, &preFilterMapsViewPort);

		ID3DX11EffectTechnique* tech = bakeEffect->BakePreFileterEnvMapTech;

		UINT stride = sizeof(Basic32);
		UINT offset = 0;

		context->IASetInputLayout(InputLayouts::PosNorTex);
		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		context->IASetVertexBuffers(0, 1, &quadVertexBuffer, &stride, &offset);
		context->IASetIndexBuffer(quadIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

		bakeEffect->SetEnvironmentMap(environmentSRV);
		bakeEffect->SetRoughness((float)i / (float)(max_mipmap_levels - 1));

		D3DX11_TECHNIQUE_DESC techDesc;
		tech->GetDesc(&techDesc);
		for (UINT p = 0; p < techDesc.Passes; ++p)
		{
			tech->GetPassByIndex(p)->Apply(0, context);
			context->DrawIndexed(6, 0, 0);
		}
	}
	exportPreFilterEnvMaps();
	clearBakedPreFilterMaps();
}

void EnvironmentMap::exportIrradianceMap()
{
	ID3D11Resource* res = nullptr;
	ID3D11Texture2D* tex = nullptr;

	irradianceSRV->GetResource(&res);

	res->QueryInterface(&tex);
	D3D11_TEXTURE2D_DESC desc;
	tex->GetDesc(&desc); 

	ID3D11Texture2D* copy_tex;
	desc.Usage = D3D11_USAGE_STAGING;
	desc.BindFlags = 0;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	device->CreateTexture2D(&desc, 0, &copy_tex);
	context->CopyResource(copy_tex, res);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	context->Map(copy_tex, 0, D3D11_MAP_READ, 0, &mappedResource);
	
	int index;
	float*img_data = new float[desc.Width * desc.Height * 3];
	char* begin_data = reinterpret_cast<char*>(mappedResource.pData);
	for(int i=0;i< desc.Height;i++)
		for (int j = 0; j < desc.Width; j++)
		{
			int y = desc.Height -1 - i;
			index = y * desc.Width + j;
			img_data[index * 3] = *reinterpret_cast<float*>(begin_data + i * mappedResource.RowPitch + j * 16);
			img_data[index * 3 + 1] = *reinterpret_cast<float*>(begin_data + i * mappedResource.RowPitch + j * 16 + 4);
			img_data[index * 3 + 2] = *reinterpret_cast<float*>(begin_data + i * mappedResource.RowPitch + j * 16 + 8);
		}

	context->Unmap(copy_tex, 0);
	
	int r = stbi_write_hdr(ira_path.c_str(), desc.Width, desc.Height, 3, img_data);
	if(r)
		std::cout << "export irradiance map:" << ira_path << std::endl;
	SAFE_DELETE(img_data);
	SAFE_RELEASE(res);
	SAFE_RELEASE(tex);
	SAFE_RELEASE(copy_tex);
}

void EnvironmentMap::exportPreFilterEnvMaps()
{
	ID3D11Resource* res = nullptr;
	ID3D11Texture2D* tex = nullptr;
	std::string path = "";

	for (int i = 0; i < max_mipmap_levels; i++)
	{
		auto preFilterSRV = bakedPreFilterMaps[i].first;
		preFilterSRV->GetResource(&res);

		res->QueryInterface(&tex);
		D3D11_TEXTURE2D_DESC desc;
		tex->GetDesc(&desc);

		ID3D11Texture2D* copy_tex;
		desc.Usage = D3D11_USAGE_STAGING;
		desc.BindFlags = 0;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		device->CreateTexture2D(&desc, 0, &copy_tex);
		context->CopyResource(copy_tex, res);

		D3D11_MAPPED_SUBRESOURCE mappedResource;
		context->Map(copy_tex, 0, D3D11_MAP_READ, 0, &mappedResource);

		int index;
		float*img_data = new float[desc.Width * desc.Height * 3];
		char* begin_data = reinterpret_cast<char*>(mappedResource.pData);
		for (int i = 0; i < desc.Height; i++)
			for (int j = 0; j < desc.Width; j++)
			{
				int y = desc.Height - 1 - i;
				index = y * desc.Width + j;
				img_data[index * 3] = *reinterpret_cast<float*>(begin_data + i * mappedResource.RowPitch + j * 16);
				img_data[index * 3 + 1] = *reinterpret_cast<float*>(begin_data + i * mappedResource.RowPitch + j * 16 + 4);
				img_data[index * 3 + 2] = *reinterpret_cast<float*>(begin_data + i * mappedResource.RowPitch + j * 16 + 8);
			}

		context->Unmap(copy_tex, 0);
		path = "./Data/PreFilterEnvMaps/" + std::to_string(i) + ".hdr";
		int r = stbi_write_hdr(path.c_str(), desc.Width, desc.Height, 3, img_data);

		if (r)
			std::cout << "export pre filter environment map:" << path << std::endl;

		SAFE_DELETE(img_data);
		SAFE_RELEASE(tex);
		SAFE_RELEASE(copy_tex);
	}
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

std::string EnvironmentMap::genIrradianceMapPath(std::string path)
{
	std::vector<std::string> irradiance_path;
	int dot_index = path.rfind('.');
	int underline_index = path.rfind('_');
	return path.replace(path.begin() + underline_index +1 , path.begin() + dot_index, "Env");
}

std::string EnvironmentMap::genPreFilterMapPath(std::string path)
{
	std::vector<std::string> prefilter_path;
	int dot_index = path.rfind('.');
	int underline_index = path.rfind('_');
	path = path.replace(path.begin() + dot_index, path.end(), ".dds");
	return path.replace(path.begin() + underline_index + 1, path.begin() + dot_index, "PreFilter");
}