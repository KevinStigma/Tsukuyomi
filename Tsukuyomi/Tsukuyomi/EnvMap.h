#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <LightHelper.h>

class Camera;
class EnvironmentMap
{
public:
	EnvironmentMap(std::string path, ID3D11Device* d, ID3D11DeviceContext* dc);
	~EnvironmentMap();

	void createIrradianceMapResource(bool is_baking);
	void renderEnvironmentMap(Camera*);
	void bakeIrradiance(ID3D11Buffer* quadVertexBuffer, ID3D11Buffer* quadIndexBuffer);
	void bakePreFilterMap(ID3D11Buffer* quadVertexBuffer, ID3D11Buffer* quadIndexBuffer);
	void exportIrradianceMap();
	void exportPreFilterEnvMaps();
	ID3D11ShaderResourceView* environmentSRV2 = nullptr;
	ID3D11ShaderResourceView* environmentSRV = nullptr;
	ID3D11ShaderResourceView* irradianceSRV = nullptr;
	std::vector<std::pair<ID3D11ShaderResourceView*, ID3D11RenderTargetView*>> bakedPreFilterMaps;
protected:
	static std::string genIrradianceMapPath(std::string);
	void createBuffers();
	void createEnvironmentMapSRV();
	void createPreFilterMaps();
	void clearBakedPreFilterMaps();

	std::string             hdr_path;
	std::string             ira_path;
	D3D11_VIEWPORT          irradianceViewPort;
	D3D11_VIEWPORT          preFilterMapsViewPort;
	ID3D11Device*           device = nullptr;
	ID3D11RenderTargetView* irradianceRTV = nullptr;
	
	ID3D11DeviceContext*    context = nullptr;
	ID3D11Buffer*	        vertexBuffer = nullptr;
	ID3D11Buffer*           indexBuffer = nullptr;
	
	int width=0, height=0, sphereIndexCount=0;
	int max_mipmap_levels = 5;
	float* data = nullptr;
	RenderLightHelper::Material mat;
};