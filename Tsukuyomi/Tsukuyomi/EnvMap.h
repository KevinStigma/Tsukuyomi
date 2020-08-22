#pragma once
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

	void createIrradianceMapSRV();
	void renderEnvironmentMap(Camera*);
	ID3D11ShaderResourceView* environmentSRV = nullptr;
	ID3D11ShaderResourceView* irradianceSRV = nullptr;
protected:
	static std::string genIrradianceMapPath(std::string);
	void createBuffers();
	void createEnvironmentMapSRV();
	std::string             hdr_path;
	std::string             ira_path;
	ID3D11Device*           device = nullptr;
	ID3D11DeviceContext*    context = nullptr;
	ID3D11Buffer*	        vertexBuffer = nullptr;
	ID3D11Buffer*           indexBuffer = nullptr;
	
	int width=0, height=0, sphereIndexCount=0;
	float* data = nullptr;
	RenderLightHelper::Material mat;
};