#include "D3DRenderer.h"
#include <directxcolors.h>
#include <vector>
#include <iostream>
#include <dxgi.h>
#include <ScreenGrab.h>
#include "common.h"
#include "Vertex.h"
#include "Globalsys.h"
#include "Effects/Effects.h"
#include "tiny_obj_loader.h"
#include "Objects/Mesh.h"
#include "EnvMap.h"
#include "./GeometryGenerator/GeometryGenerator.h"

using namespace DirectX::Colors;


D3DRenderer::D3DRenderer():m_camera("main_cam")
{
	g_pGlobalSys->renderer = this;
}

D3DRenderer::~D3DRenderer()
{
	cleanup();
}

void D3DRenderer::updateCamView(int width, int height)
{
	m_camera.setLens(m_camera.fov, (float)width / (float)height, m_camera.zNear, m_camera.zFar);
}

bool D3DRenderer::initD3D(HWND windowId, int viewport_width, int viewport_height)
{
	HRESULT hr = S_OK;

	UINT width = viewport_width;
	UINT height = viewport_height;

	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	
	DXGI_MODE_DESC bufferDesc;

	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));

	bufferDesc.Width = width;
	bufferDesc.Height = height;
	bufferDesc.RefreshRate.Numerator = 60;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	
	//Describe our SwapChain
	DXGI_SWAP_CHAIN_DESC swapChainDesc;

	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = windowId;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, 0, featureLevels, numFeatureLevels, 
		D3D11_SDK_VERSION, &swapChainDesc, &m_pSwapChain, &m_pd3dDevice, NULL, &m_pImmediateContext);
	
	if (FAILED(hr))
		return false;

	// Create a render target view
	ID3D11Texture2D* pBackBuffer = nullptr;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&pBackBuffer));
	if (FAILED(hr))
		return false;

	hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &m_pRenderTargetView);
	pBackBuffer->Release();
	if (FAILED(hr))
		return false;

	//Describe our Depth/Stencil Buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	m_pd3dDevice->CreateTexture2D(&depthStencilDesc, NULL, &m_pDepthStencilBuffer);
	hr = m_pd3dDevice->CreateDepthStencilView(m_pDepthStencilBuffer, NULL, &m_pDepthStencilView);

	if (FAILED(hr))
		return false;

	m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_pImmediateContext->RSSetViewports(1, &vp);
	m_screenViewport = vp;

	Effects::InitAll(m_pd3dDevice);
	InputLayouts::initAll(m_pd3dDevice);

	int shadowMapSize = 2048;
	shadowMap = new ShadowMap(m_pd3dDevice, shadowMapSize, shadowMapSize);
	ssaoMap = new SSAOMap(m_pd3dDevice, viewport_width, viewport_height);
	return true;
}

bool D3DRenderer::resizeD3D(int width, int height)
{
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pDepthStencilBuffer);
	HRESULT hr = S_OK;
	m_pSwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	
	//Create our BackBuffer
	ID3D11Texture2D* backBuffer;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);

	//Create our Render Target
	hr = m_pd3dDevice->CreateRenderTargetView(backBuffer, NULL, &m_pRenderTargetView);
	SAFE_RELEASE(backBuffer);

	//Describe our Depth/Stencil Buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	//Create the Depth/Stencil View
	m_pd3dDevice->CreateTexture2D(&depthStencilDesc, NULL, &m_pDepthStencilBuffer);
	m_pd3dDevice->CreateDepthStencilView(m_pDepthStencilBuffer, NULL, &m_pDepthStencilView);

	//Set our Render Target
	m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	m_screenViewport.Width = width;
	m_screenViewport.Height = height;

	//Set the Viewport
	m_pImmediateContext->RSSetViewports(1, &m_screenViewport);

	SAFE_DELETE(ssaoMap);
	ssaoMap = new SSAOMap(m_pd3dDevice, width, height);
	return true;
}

void D3DRenderer::initLights()
{
	m_dirLights.resize(3);
	m_dirLights[0].Color = XMFLOAT3(0.5f, 0.5f, 0.5f);
	m_dirLights[0].Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);

	m_dirLights[1].Color = XMFLOAT3(0.20f, 0.20f, 0.20f);
	m_dirLights[1].Direction = XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);

	m_dirLights[2].Color = XMFLOAT3(0.2f, 0.2f, 0.2f);
	m_dirLights[2].Direction = XMFLOAT3(0.0f, -0.707f, -0.707f);
}

void D3DRenderer::initMaterials()
{
	m_materials.resize(9);
	m_materials[0].albedo = XMFLOAT3(0.73725f, 0.741176f, 0.74902f);
	m_materials[1].albedo = XMFLOAT3(1.0f, 1.0f, 1.0f);
	m_materials[2].albedo = XMFLOAT3(0.0f, 0.8f, 0.0f);
	m_materials[3].albedo = XMFLOAT3(0.8f, 0.0f, 0.0f);
	m_materials[4].albedo = XMFLOAT3(0.0f, 0.0f, 0.8f);
	m_materials[5].albedo = XMFLOAT3(0.0f, 1.0f, 0.0f);
	m_materials[6].albedo = XMFLOAT3(1.0f, 0.0f, 0.0f);
	m_materials[7].albedo = XMFLOAT3(0.0f, 0.0f, 1.0f);
	m_materials[8].albedo = XMFLOAT3(0.0f, 0.0f, 0.0f);
}

void D3DRenderer::initScene()
{
	initLights();
	initMaterials();
	createRulerLlinesVertexBuffer();
	createBoundingBoxBuffers();
	createSelObjAxisBuffers();
	createFrustumBuffers();
	createCircleBuffers();
	createFullScreenQuadBuffers();
	createDirectionalLightBuffers();

	// Transform NDC space [-1,+1]^2 to texture space [0,1]^2
	XMStoreFloat4x4(&TexTransformMat, XMMATRIX(
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f));

	XMStoreFloat4x4(&DebugTexTransMat, XMMATRIX(
		0.25f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.25f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.75f, -0.75f, 0.0001f, 1.0f));	
}

void D3DRenderer::updateLightsForRenderingMeshes()
{
	std::vector<Light*> lights = g_pGlobalSys->objectManager.getAllLights();
	std::vector<RenderLightHelper::DirLight> dirLights;
	std::vector<RenderLightHelper::PointLight> pointLights;
	for each (Light* light in lights)
	{
		XMFLOAT3 color = light->getColor();
		if (light->getType() == DIR_LIGHT)
		{
			RenderLightHelper::DirLight dirLight;
			dirLight.Direction = ((DirectionalLight*)light)->getWorldDir();
			dirLight.Color = XMFLOAT3(color.x, color.y, color.z);
			dirLights.push_back(dirLight);
		}
		else if(light->getType() == POINT_LIGHT)
		{
			RenderLightHelper::PointLight pLight;
			pLight.Color = XMFLOAT3(color.x, color.y, color.z);
			pLight.Position = ((PointLight*)light)->getTranslation();
			pointLights.push_back(pLight);
		}
		else if (light->getType() == AREA_LIGHT)
		{
			RenderLightHelper::PointLight pLight;
			pLight.Color = XMFLOAT3(color.x, color.y, color.z);
			pLight.Position = ((AreaLight*)light)->getMesh()->getWorldCenter();
			pointLights.push_back(pLight);
		}
	}
	BasicEffect*basicEffect = Effects::BasicFX;
	basicEffect->SetDirLightCount(dirLights.size());
	basicEffect->SetPointLightCount(pointLights.size());
	if(pointLights.size())
		basicEffect->SetPointLights(&pointLights[0], pointLights.size());
	if(dirLights.size())
		basicEffect->SetDirLights(&dirLights[0], dirLights.size());

	XMFLOAT3 eyePosW = m_camera.getPosition();
	basicEffect->SetEyePosW(eyePosW);
	basicEffect->SetGammaCorrect(g_pGlobalSys->render_paras.gammaCorrect);
	basicEffect->SetEnableHDR(g_pGlobalSys->render_paras.enableHDR);
	basicEffect->SetHDRExposure(g_pGlobalSys->getHDRExposure());
}

void D3DRenderer::updateLightsForRenderingSelFlags()
{
	std::vector<RenderLightHelper::DirLight> dirLights;
	std::vector<RenderLightHelper::PointLight> pointLights;

	RenderLightHelper::DirLight dirLight;
	dirLight.Direction = XMFLOAT3(1.0, 1.0, 1.0);
	dirLight.Color = XMFLOAT3(2.0, 2.0, 2.0);
	dirLights.push_back(dirLight);

	dirLight.Direction = XMFLOAT3(-1.0, -1.0, -1.0);
	dirLights.push_back(dirLight);

	BasicEffect*basicEffect = Effects::BasicFX;
	basicEffect->SetDirLightCount(dirLights.size());
	basicEffect->SetPointLightCount(0);
	basicEffect->SetDirLights(&dirLights[0], dirLights.size());
}

void D3DRenderer::renderToShadowMap()
{
	if (!g_pGlobalSys->objectManager.getCurSelShadowLight())
		return;
	shadowMap->BindDsvAndSetNullRenderTarget(m_pImmediateContext);
	std::vector<Object*> objects = g_pGlobalSys->objectManager.getAllObjects();
	for each (auto obj in objects)
	{
		if(obj->getType() == MESH)
		   dynamic_cast<Mesh*>(obj)->renderToShadowMap(m_pImmediateContext, this, &m_shadowTransform);
	}
	m_pImmediateContext->RSSetState(0);
}

void D3DRenderer::renderNormalDepthMap()
{
	ssaoMap->SetNormalDepthRenderTarget(m_pImmediateContext, m_pDepthStencilView);
	m_pImmediateContext->RSSetViewports(1, &m_screenViewport);
	std::vector<Object*> objects = g_pGlobalSys->objectManager.getAllObjects();
	for each (auto obj in objects)
	{
		if (obj->getType() == MESH)
			dynamic_cast<Mesh*>(obj)->renderNormalDepthMap(m_pImmediateContext, this);
	}
}

void D3DRenderer::renderInitialSSAOMap()
{
	ssaoMap->SetRenderSSAORenderTarget(m_pImmediateContext);
	BuildSSAOMapEffect* buildSSAOMapEffect = Effects::BuildSSAOMapFX;

	buildSSAOMapEffect->SetFarPlaneDepth(m_camera.zFar);
	buildSSAOMapEffect->SetFarPlaneSize(XMLoadFloat2(&m_camera.getProjPlaneSize(m_camera.zFar)));

	UINT stride = sizeof(Basic32);
	UINT offset = 0;
	m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pQuadVertexBuffer, &stride, &offset);
	m_pImmediateContext->IASetIndexBuffer(m_pQuadIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_pImmediateContext->IASetInputLayout(InputLayouts::PosNorTex);
	m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ID3DX11EffectTechnique* activeTech = buildSSAOMapEffect->BuildSSAOMapTech;
	D3DX11_TECHNIQUE_DESC techDesc;
	activeTech->GetDesc(&techDesc);

	XMVECTOR v;
	XMMATRIX T = XMLoadFloat4x4(&TexTransformMat);
	XMMATRIX worldMat = XMMatrixIdentity();
	XMMATRIX inv_world_mat = XMMatrixInverse(&v, worldMat);
	XMMATRIX view_mat = m_camera.getViewMatrix();
	XMMATRIX proj_mat = m_camera.getProjMatrix();
	XMMATRIX WVP = worldMat * view_mat * proj_mat;
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		buildSSAOMapEffect->SetWorld(worldMat);
		buildSSAOMapEffect->SetWorldInvTranspose(inv_world_mat);
		buildSSAOMapEffect->SetWorldViewProj(WVP);
		buildSSAOMapEffect->SetViewToTexSpace(proj_mat * T);
		buildSSAOMapEffect->SetNormalDepthMap(ssaoMap->getNormalDepthMapSRV());
		buildSSAOMapEffect->SetOffsetVectors(ssaoMap->getOffsets().data());
		buildSSAOMapEffect->SetRandomVecMap(ssaoMap->getRandomVectorSRV());

		activeTech->GetPassByIndex(p)->Apply(0, m_pImmediateContext);
		m_pImmediateContext->DrawIndexed(6, 0, 0);
	}
}

void D3DRenderer::renderSSAOMap()
{
	if (!g_pGlobalSys->render_paras.enableSSAO)
		return;
	renderNormalDepthMap();
	renderInitialSSAOMap();
	ssaoMap->blurSSAOMap(m_pImmediateContext, m_pQuadVertexBuffer, m_pQuadIndexBuffer, 4);
}

void D3DRenderer::renderScene()
{
	updateLightsForRenderingMeshes();
	renderToShadowMap();

	renderSSAOMap();

	m_pImmediateContext->RSSetViewports(1, &m_screenViewport);
	ID3D11RenderTargetView* renderTargets[1] = { m_pRenderTargetView };
	m_pImmediateContext->OMSetRenderTargets(1, renderTargets, m_pDepthStencilView);
	//Clear our backbuffer
	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, DirectX::Colors::Black);
	m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	renderRulerLlines();
	renderBVH();

	if(g_pGlobalSys->objectManager.getCurSelShadowLight())
		Effects::BasicFX->SetShadowMap(shadowMap->DepthMapSRV());
	if (g_pGlobalSys->render_paras.enableSSAO)
		Effects::BasicFX->SetSSAOMap(ssaoMap->getSSAOMapSRV());

	EnvironmentMap* env_map = g_pGlobalSys->objectManager.getEnvironmentMap();

	Effects::BasicFX->SetIrradianceMap(env_map&&env_map->irradianceSRV ? env_map->irradianceSRV : nullptr);
	Effects::BasicFX->SetBrdfLutMap(env_map&&env_map->brdfLUTSRV ? env_map->brdfLUTSRV : nullptr);
	Effects::BasicFX->SetPreFilterEnvMap(env_map&&env_map->preFileterSRV ? env_map->preFileterSRV : nullptr);

	g_pGlobalSys->objectManager.renderAllObjects(m_pImmediateContext, this);


	EnvironmentMap* environmentMap = g_pGlobalSys->objectManager.getEnvironmentMap();
	if (environmentMap)
		environmentMap->renderEnvironmentMap(&m_camera);

	renderSelObjFlag();

	renderDebugTex();

	//Present the backbuffer to the screen
	m_pSwapChain->Present(0, 0);
}

void D3DRenderer::GragScreenShot()
{	
	std::string file_name = "./Data/ScreenShots/"+generateRandomId()+".jpg";

	std::wstring w_file_name = string2wstring(file_name);

	ID3D11Texture2D *pBackBuffer;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void **>(&pBackBuffer));
	SaveWICTextureToFile(m_pImmediateContext, pBackBuffer, { 0x19e4a5aa, 0x5662, 0x4fc5, 0xa0, 0xc0, 0x17, 0x58, 0x02, 0x8e, 0x10, 0x57 }, w_file_name.c_str());
	std::cout << "screenshot on " << file_name << std::endl;
}

void D3DRenderer::renderSelObjFlag()
{
	if (!g_pGlobalSys->render_paras.showRulerLines)
		return;
	Object* sel_obj = g_pGlobalSys->objectManager.getCurSelObject();
	if (sel_obj)
	{
		if (sel_obj->getType() == MESH)
			renderBoundingBox(sel_obj);
		else if (sel_obj->getType() == CAM)
			renderFrustum(((Camera*)sel_obj)->getFrustumMatrix());
		else if (sel_obj->getType() == POINT_LIGHT)
			renderWireFrameSphere(sel_obj);
		else if (sel_obj->getType() == DIR_LIGHT)
			renderDirectionalLight(sel_obj);

		updateLightsForRenderingSelFlags();
		if(renderSelObjMode == COORD_AXIS)
			renderCoordAxis(sel_obj);
		else if(renderSelObjMode == ROT_AXIS)
			renderRotAxis(sel_obj);
	}
}

void D3DRenderer::renderDebugTex()
{
	if (!g_pGlobalSys->render_paras.showDebugTex)
		return;
	DebugTexEffect* debugTexEffect = Effects::DebugTexFX;

	UINT stride = sizeof(Basic32);
	UINT offset = 0;
	m_pImmediateContext->IASetVertexBuffers(0, 1, &m_pQuadVertexBuffer, &stride, &offset);
	m_pImmediateContext->IASetIndexBuffer(m_pQuadIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	m_pImmediateContext->IASetInputLayout(InputLayouts::PosNorTex);
	m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	if (g_pGlobalSys->render_paras.enableSSAO)
		debugTexEffect->SetDebugTex(ssaoMap->getSSAOMapSRV());
	else if (g_pGlobalSys->objectManager.getCurSelShadowLight())
		debugTexEffect->SetDebugTex(shadowMap->DepthMapSRV());
	else if (g_pGlobalSys->objectManager.getEnvironmentMap())
		debugTexEffect->SetDebugTex(g_pGlobalSys->objectManager.getEnvironmentMap()->environmentSRV);

	ID3DX11EffectTechnique* activeTech = debugTexEffect->DebugTexTech;
	D3DX11_TECHNIQUE_DESC techDesc;
	activeTech->GetDesc(&techDesc);

	XMVECTOR v;
	XMMATRIX worldMat = XMLoadFloat4x4(&DebugTexTransMat);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		debugTexEffect->SetWorldViewProj(worldMat);

		activeTech->GetPassByIndex(p)->Apply(0, m_pImmediateContext);
		m_pImmediateContext->DrawIndexed(6, 0, 0);
	}
}

void D3DRenderer::renderBVH()
{
	if (!g_pGlobalSys->render_paras.renderBVH)
		return;
	auto bvh_nodes = g_pGlobalSys->objectManager.getBVHManager()->getBvhNodes();
	XMFLOAT4 color;
	for each (auto node in bvh_nodes)
	{
		renderBoundingBox(node.boundingbox, color);
	}
}


void D3DRenderer::rayAxisIntersectionDetect(float x_ratio, float y_ratio)
{
	Object* obj = g_pGlobalSys->objectManager.getCurSelObject();
	if (obj)
	{
		Ray ray = m_camera.getRay(x_ratio, y_ratio);
		if (renderSelObjMode == RenderSelObjMode::COORD_AXIS)
		{
			AXIS lastSelAxis = curSelAxis;
			curSelAxis = AXIS(transAxis.rayIntersectDectect(ray, obj));
			if (lastSelAxis == AXIS::NO && curSelAxis != AXIS::NO)
				transAxis.computeAxisDirectionProj(obj, m_camera, curSelAxis);
		}
		else if (renderSelObjMode == RenderSelObjMode::ROT_AXIS)
		{
			AXIS lastSelAxis = curSelAxis;
			float t;
			curSelAxis = AXIS(rotAxis.rayIntersectDectect(ray, obj, t));
			if (curSelAxis != AXIS::NO)
			{
				XMFLOAT3 inter_pt(ray.origin.x + t * ray.direction.x, ray.origin.y + t * ray.direction.y, ray.origin.z + t * ray.direction.z);
				rotAxis.setIntersectionPoint(inter_pt);
			}
		}
		else
			curSelAxis = AXIS::NO;
	}
	else
		curSelAxis = AXIS::NO;
}

void D3DRenderer::translateSelObj(XMFLOAT2 mouse_move_dir)
{
	transAxis.translateSelObj(mouse_move_dir, curSelAxis);
}

void D3DRenderer::rotateSelObj(XMFLOAT2 np1, XMFLOAT2 np2)
{
	rotAxis.rotateSelObj(m_camera, np1, np2, curSelAxis, g_pGlobalSys->objectManager.getCurSelObject());
}

void D3DRenderer::enableMSAA(bool enabled)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	m_pSwapChain->GetDesc(&swapChainDesc);
	int width = swapChainDesc.BufferDesc.Width;
	int height = swapChainDesc.BufferDesc.Height;

	UINT m4xMsaaQuality = 0;
	if (enabled)
	{
		m_pd3dDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m4xMsaaQuality);
		swapChainDesc.SampleDesc.Count = 4;
		swapChainDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
	}
	else
	{
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.SampleDesc.Quality = 0;
	}

	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pRenderTargetView);
	SAFE_RELEASE(m_pDepthStencilView);
	SAFE_RELEASE(m_pDepthStencilBuffer);

	IDXGIDevice* dxgiDevice = 0; 
	m_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice);
	IDXGIAdapter* dxgiAdapter = 0; 
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
	IDXGIFactory* dxgiFactory = 0; 
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory),(void**)&dxgiFactory);
	dxgiFactory->CreateSwapChain(m_pd3dDevice, &swapChainDesc, &m_pSwapChain);

	HRESULT hr = S_OK;
	m_pSwapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

	//Create our BackBuffer
	ID3D11Texture2D* backBuffer;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);

	//Create our Render Target
	hr = m_pd3dDevice->CreateRenderTargetView(backBuffer, NULL, &m_pRenderTargetView);
	SAFE_RELEASE(backBuffer);

	//Describe our Depth/Stencil Buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	if(enabled)
	{
		depthStencilDesc.SampleDesc.Count = 4;
		depthStencilDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
	}
	else
	{
		depthStencilDesc.SampleDesc.Count = 1;
		depthStencilDesc.SampleDesc.Quality = 0;
	}
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	//Create the Depth/Stencil View
	m_pd3dDevice->CreateTexture2D(&depthStencilDesc, NULL, &m_pDepthStencilBuffer);
	m_pd3dDevice->CreateDepthStencilView(m_pDepthStencilBuffer, NULL, &m_pDepthStencilView);

	//Set our Render Target
	m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView);

	m_screenViewport.Width = width;
	m_screenViewport.Height = height;

	//Set the Viewport
	m_pImmediateContext->RSSetViewports(1, &m_screenViewport);
}

void D3DRenderer::resetCameraTransform(Camera* cam)
{
	if (!cam)
		return;
	m_camera.setTranslation(cam->getTranslation());
	m_camera.setRotation(cam->getRotation());
	m_camera.updateViewMatrix();
}

float D3DRenderer::computeFPS()
{
	clock_t cur_clock = clock();
	float passed_secs = std::max<float>((float)(cur_clock - last_clock) / CLOCKS_PER_SEC, 0.001);
	last_clock = cur_clock;
	return 1.0 / passed_secs;
}

void D3DRenderer::bakeIrradiance()
{
	EnvironmentMap* env_map = g_pGlobalSys->objectManager.getEnvironmentMap();
	if (env_map)
	{
		env_map->bakeIrradiance(m_pQuadVertexBuffer, m_pQuadIndexBuffer);
		env_map->exportIrradianceMap();
	}
}

void D3DRenderer::bakePreFilterMaps()
{
	EnvironmentMap* env_map = g_pGlobalSys->objectManager.getEnvironmentMap();
	if (env_map)
		env_map->bakePreFilterMap(m_pQuadVertexBuffer, m_pQuadIndexBuffer);
}

void D3DRenderer::buildShadowTransform()
{
	// Only the first "main" light casts a shadow.
	DirectionalLight* light = dynamic_cast<DirectionalLight*>(g_pGlobalSys->objectManager.getCurSelShadowLight());
	if (!light)
		return;

	BoundingBox boundingbox = BoundingBox::Union(g_pGlobalSys->objectManager.getBVHManager()->getBvhNodes()[0].boundingbox,
		BoundingBox(light->getTranslation(), light->getTranslation()));

	XMFLOAT3 diff((boundingbox.bottom.x - boundingbox.top.x) * 0.5f, 
		(boundingbox.bottom.y - boundingbox.top.y) * 0.5f,
		(boundingbox.bottom.z - boundingbox.top.z) * 0.5f);

	float scene_radius = MathHelper::Float3Length(diff);
	std::cout << "update scene radius:" << scene_radius << std::endl;
	XMVECTOR lightDir = XMLoadFloat3(&light->getWorldDir());
	XMVECTOR lightPos = XMLoadFloat3(&light->getTranslation());
	XMVECTOR targetPos = lightPos + lightDir * scene_radius;
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX V = XMMatrixLookAtLH(lightPos, targetPos, up);

	// Transform bounding sphere to light space.
	XMFLOAT3 sphereCenterLS;
	XMStoreFloat3(&sphereCenterLS, XMVector3TransformCoord(targetPos, V));

	// Ortho frustum in light space encloses scene.
	float l = sphereCenterLS.x - scene_radius;
	float b = sphereCenterLS.y - scene_radius;
	float n = sphereCenterLS.z - scene_radius;
	float r = sphereCenterLS.x + scene_radius;
	float t = sphereCenterLS.y + scene_radius;
	float f = sphereCenterLS.z + scene_radius;
	XMMATRIX P = XMMatrixOrthographicOffCenterLH(l, r, b, t, n, f);

	// Transform NDC space [-1,+1]^2 to texture space [0,1]^2
	XMMATRIX T = XMLoadFloat4x4(&TexTransformMat);
	XMMATRIX S = V * P * T;
	XMStoreFloat4x4(&m_shadowTransform.shadowTransMat, S);
	XMStoreFloat4x4(&m_shadowTransform.lightViewTransMat, V);
	XMStoreFloat4x4(&m_shadowTransform.lightProjTransMat, P);
}

void D3DRenderer::renderRulerLlines()
{
	if (!g_pGlobalSys->render_paras.showRulerLines)
		return;
	BasicEffect*basicEffect = Effects::BasicFX;

	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	ID3D11DeviceContext * context = m_pImmediateContext;
	context->IASetVertexBuffers(0, 1, &m_pRulerLineVertexBuffer, &stride, &offset);
	context->IASetInputLayout(InputLayouts::PosColor);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	
	ID3DX11EffectTechnique* activeTech = basicEffect->SimpleColorTech;
	D3DX11_TECHNIQUE_DESC techDesc;
	activeTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		XMMATRIX WVP;
		WVP = XMMatrixIdentity() * m_camera.getViewMatrix() * m_camera.getProjMatrix();
		basicEffect->SetWorld(XMMatrixIdentity());
		basicEffect->SetTexTransform(XMMatrixIdentity());
		basicEffect->SetWorldViewProj(WVP);

		activeTech->GetPassByIndex(p)->Apply(0, context);
		context->Draw(214, 0);
	}
}

void D3DRenderer::renderFrustum(FXMMATRIX trans_mat)
{
	BasicEffect*basicEffect = Effects::BasicFX;

	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	ID3D11DeviceContext * context = m_pImmediateContext;
	context->IASetVertexBuffers(0, 1, &m_pFrumstumVertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(m_pFrumstumIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetInputLayout(InputLayouts::PosColor);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	ID3DX11EffectTechnique* activeTech = basicEffect->SimpleColorTech;
	D3DX11_TECHNIQUE_DESC techDesc;
	activeTech->GetDesc(&techDesc);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		XMMATRIX WVP;
		WVP = trans_mat * m_camera.getViewMatrix() * m_camera.getProjMatrix();
		basicEffect->SetWorld(trans_mat);
		basicEffect->SetTexTransform(XMMatrixIdentity());
		basicEffect->SetWorldViewProj(WVP);

		activeTech->GetPassByIndex(p)->Apply(0, context);
		context->DrawIndexed(16, 0, 0);
	}
}

void D3DRenderer::renderWireFrameSphere(Object* obj)
{
	BasicEffect*basicEffect = Effects::BasicFX;

	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	ID3D11DeviceContext * context = m_pImmediateContext;
	context->IASetVertexBuffers(0, 1, &m_pCircleVertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(m_pCircleIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetInputLayout(InputLayouts::PosColor);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	ID3DX11EffectTechnique* activeTech = basicEffect->SimpleColorTech;
	D3DX11_TECHNIQUE_DESC techDesc;
	activeTech->GetDesc(&techDesc);
	
	XMMATRIX global_world_mat = obj->getGlobalWorldMatrix();
	XMMATRIX trans_mat = XMMatrixTranslationFromVector(XMVector3TransformCoord(XMVectorSet(0, 0, 0, 1), global_world_mat));
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		XMMATRIX WVP;
		XMMATRIX world_matrix = trans_mat;
		WVP = world_matrix * m_camera.getViewMatrix() * m_camera.getProjMatrix();
		basicEffect->SetWorld(world_matrix);
		basicEffect->SetTexTransform(XMMatrixIdentity());
		basicEffect->SetWorldViewProj(WVP);
		activeTech->GetPassByIndex(p)->Apply(0, context);
		context->DrawIndexed(60, 0, 0);

		world_matrix = XMMatrixRotationY(MathHelper::Pi * 0.5) * trans_mat;
		WVP = world_matrix * m_camera.getViewMatrix() * m_camera.getProjMatrix();
		basicEffect->SetWorld(world_matrix);
		basicEffect->SetTexTransform(XMMatrixIdentity());
		basicEffect->SetWorldViewProj(WVP);
		activeTech->GetPassByIndex(p)->Apply(0, context);
		context->DrawIndexed(60, 0, 0);

		world_matrix = XMMatrixRotationX(MathHelper::Pi * 0.5) * trans_mat;
		WVP = world_matrix * m_camera.getViewMatrix() * m_camera.getProjMatrix();
		basicEffect->SetWorld(world_matrix);
		basicEffect->SetTexTransform(XMMatrixIdentity());
		basicEffect->SetWorldViewProj(WVP);
		activeTech->GetPassByIndex(p)->Apply(0, context);
		context->DrawIndexed(60, 0, 0);
	}
}

void D3DRenderer::renderDirectionalLight(Object* obj)
{
	BasicEffect*basicEffect = Effects::BasicFX;

	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	ID3D11DeviceContext * context = m_pImmediateContext;
	context->IASetVertexBuffers(0, 1, &m_pDirLightVertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(m_pDirLightIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetInputLayout(InputLayouts::PosColor);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	ID3DX11EffectTechnique* activeTech = basicEffect->SimpleColorTech;
	D3DX11_TECHNIQUE_DESC techDesc;
	activeTech->GetDesc(&techDesc);

	XMMATRIX global_world_mat = obj->getGlobalWorldMatrix();
	XMMATRIX trans_mat = XMMatrixTranslationFromVector(XMVector3TransformCoord(XMVectorSet(0, 0, 0, 1), global_world_mat));

	XMVECTOR default_vec = XMVectorSet(1, 0, 0, 0.0);
	XMVECTOR world_dir = XMVector3Normalize(XMVector3TransformNormal(XMVectorSet(1, 0, 0, 0.0), global_world_mat));
	XMVECTOR axis = XMVector3Normalize(XMVector3Cross(default_vec, world_dir));
	float radian = acosf(XMVectorGetX(XMVector3Dot(default_vec, world_dir)));
	XMMATRIX rot_mat = radian == 0.0 ? XMMatrixIdentity() : XMMatrixRotationAxis(axis, radian);

	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		XMMATRIX WVP;
		XMMATRIX world_matrix = rot_mat * trans_mat;
		WVP = world_matrix * m_camera.getViewMatrix() * m_camera.getProjMatrix();
		basicEffect->SetWorld(world_matrix);
		basicEffect->SetTexTransform(XMMatrixIdentity());
		basicEffect->SetWorldViewProj(WVP);
		activeTech->GetPassByIndex(p)->Apply(0, context);
		context->DrawIndexed(72, 0, 0);
	}
}

void D3DRenderer::renderCoordAxis(Object* obj)
{
	BasicEffect*basicEffect = Effects::BasicFX;
	UINT stride = sizeof(Basic32);
	UINT offset = 0;
	ID3D11DeviceContext*  context = m_pImmediateContext;
	context->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->IASetVertexBuffers(0, 1, &m_pAxisVertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(m_pAxisIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetInputLayout(InputLayouts::PosNorTex);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ID3DX11EffectTechnique* activeTech = basicEffect->CustomLightTech;
	D3DX11_TECHNIQUE_DESC techDesc;
	activeTech->GetDesc(&techDesc);

	XMVECTOR v = XMVectorZero();
	XMMATRIX worldMat = transAxis.computeWorldMatrix(obj, AXIS::X);
	XMMATRIX inv_world_mat = XMMatrixTranspose(XMMatrixInverse(&v, worldMat));
	XMMATRIX WVP = worldMat * m_camera.getViewMatrix() * m_camera.getProjMatrix();
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		basicEffect->SetWorld(worldMat);
		basicEffect->SetWorldInvTranspose(inv_world_mat);
		basicEffect->SetWorldViewProj(WVP);
		basicEffect->SetMaterial(m_materials[curSelAxis == AXIS::X ? 6 : 3]);
		activeTech->GetPassByIndex(p)->Apply(0, context);
		context->DrawIndexed(transAxisIndexCount, 0, 0);

		worldMat = transAxis.computeWorldMatrix(obj, AXIS::Y);
		inv_world_mat = XMMatrixTranspose(XMMatrixInverse(&v, worldMat));
		WVP = worldMat * m_camera.getViewMatrix() * m_camera.getProjMatrix();
		basicEffect->SetWorld(worldMat);
		basicEffect->SetWorldInvTranspose(inv_world_mat);
		basicEffect->SetWorldViewProj(WVP);
		basicEffect->SetMaterial(m_materials[curSelAxis == AXIS::Y ? 5 : 2]);
		activeTech->GetPassByIndex(p)->Apply(0, context);
		context->DrawIndexed(transAxisIndexCount, 0, 0);

		worldMat = transAxis.computeWorldMatrix(obj, AXIS::Z);
		inv_world_mat = XMMatrixTranspose(XMMatrixInverse(&v, worldMat));
		WVP = worldMat * m_camera.getViewMatrix() * m_camera.getProjMatrix();
		basicEffect->SetWorld(worldMat);
		basicEffect->SetWorldInvTranspose(inv_world_mat);
		basicEffect->SetWorldViewProj(WVP);
		basicEffect->SetMaterial(m_materials[curSelAxis == AXIS::Z ? 7 : 4]);

		activeTech->GetPassByIndex(p)->Apply(0, context);
		context->DrawIndexed(transAxisIndexCount, 0, 0);
	}
}

void D3DRenderer::renderRotAxis(Object* obj)
{
	BasicEffect*basicEffect = Effects::BasicFX;
	UINT stride = sizeof(Basic32);
	UINT offset = 0;
	ID3D11DeviceContext* context = m_pImmediateContext;
	context->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	context->IASetVertexBuffers(0, 1, &m_pAxisVertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(m_pAxisIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetInputLayout(InputLayouts::PosNorTex);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	ID3DX11EffectTechnique* activeTech = basicEffect->CustomLightTech;
	D3DX11_TECHNIQUE_DESC techDesc;
	activeTech->GetDesc(&techDesc);

	XMVECTOR v;
	XMMATRIX worldMat = rotAxis.computeWorldMatrix(obj, AXIS::X);
	XMMATRIX inv_world_mat = XMMatrixTranspose(XMMatrixInverse(&v, worldMat));
	XMMATRIX WVP = worldMat * m_camera.getViewMatrix() * m_camera.getProjMatrix();

	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		basicEffect->SetWorld(worldMat);
		basicEffect->SetWorldInvTranspose(inv_world_mat);
		basicEffect->SetWorldViewProj(WVP);
		basicEffect->SetMaterial(m_materials[curSelAxis == AXIS::X ? 6 : 3]);
		activeTech->GetPassByIndex(p)->Apply(0, context);
		context->DrawIndexed(rotAxisIndexCount, rotAxisIndexBegin, axisVertexCount);

		worldMat = rotAxis.computeWorldMatrix(obj, AXIS::Y);
		inv_world_mat = XMMatrixTranspose(XMMatrixInverse(&v, worldMat));
		WVP = worldMat * m_camera.getViewMatrix() * m_camera.getProjMatrix();
		basicEffect->SetWorld(worldMat);
		basicEffect->SetWorldInvTranspose(inv_world_mat);
		basicEffect->SetWorldViewProj(WVP);
		basicEffect->SetMaterial(m_materials[curSelAxis == AXIS::Y ? 5 : 2]);
		activeTech->GetPassByIndex(p)->Apply(0, context);
		context->DrawIndexed(rotAxisIndexCount, rotAxisIndexBegin, axisVertexCount);

		worldMat = rotAxis.computeWorldMatrix(obj, AXIS::Z);
		inv_world_mat = XMMatrixTranspose(XMMatrixInverse(&v, worldMat));
		WVP = worldMat * m_camera.getViewMatrix() * m_camera.getProjMatrix();
		basicEffect->SetWorld(worldMat);
		basicEffect->SetWorldInvTranspose(inv_world_mat);
		basicEffect->SetWorldViewProj(WVP);
		basicEffect->SetMaterial(m_materials[curSelAxis == AXIS::Z ? 7 : 4]);
		activeTech->GetPassByIndex(p)->Apply(0, context);
		context->DrawIndexed(rotAxisIndexCount, rotAxisIndexBegin, axisVertexCount);
	}
}

void D3DRenderer::createRulerLlinesVertexBuffer()
{
	D3D11_BUFFER_DESC buffDesc = {};
	D3D11_SUBRESOURCE_DATA initData = {};

	ZeroMemory(&buffDesc, sizeof(buffDesc));
	std::vector<SimpleVertex> line_pts;

	for(int i=-51;i<=51;i+=2)
	{
		SimpleVertex vertex = SimpleVertex();
		vertex.Pos = XMFLOAT3(i,0.0,-50.0);
		vertex.Color = XMFLOAT4(0.4, 0.4, 0.4, 1.0);
		line_pts.push_back(vertex);

		vertex = SimpleVertex();
		vertex.Pos = XMFLOAT3(i, 0.0, 50.0);
		vertex.Color = XMFLOAT4(0.4, 0.4, 0.4, 1.0);
		line_pts.push_back(vertex);

		vertex = SimpleVertex();
		vertex.Pos = XMFLOAT3(-50.0, 0.0, i);
		vertex.Color = XMFLOAT4(0.4, 0.4, 0.4, 1.0);
		line_pts.push_back(vertex);

		vertex = SimpleVertex();
		vertex.Pos = XMFLOAT3(50.0, 0.0, i);
		vertex.Color = XMFLOAT4(0.4, 0.4, 0.4, 1.0);
		line_pts.push_back(vertex);
	}

	SimpleVertex vertex = SimpleVertex();
	vertex.Pos = XMFLOAT3(0.0, 0.0, 0.0);
	vertex.Color = XMFLOAT4(1.0, 0.0, 0.0, 1.0);
	line_pts.push_back(vertex);

	vertex = SimpleVertex();
	vertex.Pos = XMFLOAT3(1000.0, 0.0, 0.0);
	vertex.Color = XMFLOAT4(1.0, 0.0, 0.0, 1.0);
	line_pts.push_back(vertex);

	vertex = SimpleVertex();
	vertex.Pos = XMFLOAT3(0.0, 0.0, 0.0);
	vertex.Color = XMFLOAT4(0.0, 1.0, 0.0, 1.0);
	line_pts.push_back(vertex);

	vertex = SimpleVertex();
	vertex.Pos = XMFLOAT3(0.0, 1000.0, 0.0);
	vertex.Color = XMFLOAT4(0.0, 1.0, 0.0, 1.0);
	line_pts.push_back(vertex);

	vertex = SimpleVertex();
	vertex.Pos = XMFLOAT3(0.0, 0.0, 0.0);
	vertex.Color = XMFLOAT4(0.0, 0.0, 1.0, 1.0);
	line_pts.push_back(vertex);

	vertex = SimpleVertex();
	vertex.Pos = XMFLOAT3(0.0, 0.0, 1000.0);
	vertex.Color = XMFLOAT4(0.0, 0.0, 1.0, 1.0);
	line_pts.push_back(vertex);

	buffDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	buffDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	buffDesc.CPUAccessFlags = 0;
	buffDesc.ByteWidth = sizeof(SimpleVertex) * line_pts.size();

	initData.pSysMem = line_pts.data();

	if (FAILED(m_pd3dDevice->CreateBuffer(&buffDesc, &initData, &m_pRulerLineVertexBuffer)))
		return;
}

void D3DRenderer::createBoundingBoxBuffers()
{
	D3D11_BUFFER_DESC buffDesc = {};
	D3D11_SUBRESOURCE_DATA initData = {};

	ZeroMemory(&buffDesc, sizeof(buffDesc));
	std::vector<SimpleVertex> line_pts;

	SimpleVertex vertex = SimpleVertex();
	vertex.Pos = XMFLOAT3(0.5, 0.5, -0.5);
	vertex.Color = XMFLOAT4(0.96, 0.906, 0.62, 1.0);
	line_pts.push_back(vertex);

	vertex = SimpleVertex();
	vertex.Pos = XMFLOAT3(0.5, -0.5, -0.5);
	vertex.Color = XMFLOAT4(0.96, 0.906, 0.62, 1.0);
	line_pts.push_back(vertex);

	vertex = SimpleVertex();
	vertex.Pos = XMFLOAT3(-0.5, -0.5, -0.5);
	vertex.Color = XMFLOAT4(0.96, 0.906, 0.62, 1.0);
	line_pts.push_back(vertex);

	vertex = SimpleVertex();
	vertex.Pos = XMFLOAT3(-0.5, 0.5, -0.5);
	vertex.Color = XMFLOAT4(0.96, 0.906, 0.62, 1.0);
	line_pts.push_back(vertex);

	vertex = SimpleVertex();
	vertex.Pos = XMFLOAT3(0.5, 0.5, 0.5);
	vertex.Color = XMFLOAT4(0.96, 0.906, 0.62, 1.0);
	line_pts.push_back(vertex);

	vertex = SimpleVertex();
	vertex.Pos = XMFLOAT3(0.5, -0.5, 0.5);
	vertex.Color = XMFLOAT4(0.96, 0.906, 0.62, 1.0);
	line_pts.push_back(vertex);

	vertex = SimpleVertex();
	vertex.Pos = XMFLOAT3(-0.5, -0.5, 0.5);
	vertex.Color = XMFLOAT4(0.96, 0.906, 0.62, 1.0);
	line_pts.push_back(vertex);

	vertex = SimpleVertex();
	vertex.Pos = XMFLOAT3(-0.5, 0.5, 0.5);
	vertex.Color = XMFLOAT4(0.96, 0.906, 0.62, 1.0);
	line_pts.push_back(vertex);

	buffDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	buffDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	buffDesc.CPUAccessFlags = 0;
	buffDesc.ByteWidth = sizeof(SimpleVertex) * line_pts.size();

	initData.pSysMem = line_pts.data();

	if (FAILED(m_pd3dDevice->CreateBuffer(&buffDesc, &initData, &m_pBoundingBoxVertexBuffer)))
		return;

	std::vector<unsigned int> indices(24);
	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 1;
	indices[3] = 2;
	indices[4] = 2;
	indices[5] = 3;
	indices[6] = 3;
	indices[7] = 0;

	indices[8] = 0;
	indices[9] = 4;
	indices[10] = 1;
	indices[11] = 5;
	indices[12] = 2;
	indices[13] = 6;
	indices[14] = 3;
	indices[15] = 7;

	indices[16] = 4;
	indices[17] = 5;
	indices[18] = 5;
	indices[19] = 6;
	indices[20] = 6;
	indices[21] = 7;
	indices[22] = 7;
	indices[23] = 4;

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(unsigned int)* indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	if (FAILED(m_pd3dDevice->CreateBuffer(&ibd, &iinitData, &m_pBoundingBoxIndexBuffer)))
		return;
}

void D3DRenderer::createSelObjAxisBuffers()
{
	GeometryGenerator geoGen;
	GeometryGenerator::MeshData cylinder_data;
	geoGen.CreateCylinder(transAxis.get_cylinder_radius(), transAxis.get_cylinder_radius(), transAxis.get_cylinder_length(), 4, 4, cylinder_data);

	GeometryGenerator::MeshData cone_data;
	geoGen.CreateCylinder(transAxis.get_cone_radius(), 0.0, transAxis.get_cone_length(), 4, 4, cone_data);

	GeometryGenerator::MeshData torus_data;
	geoGen.CreateTorus(rotAxis.getLargetRadius(), rotAxis.getSmallRadius(), 30, 30, torus_data);

	for (int i = 0; i < cylinder_data.Vertices.size(); i++)
		cylinder_data.Vertices[i].Position.y += transAxis.get_cylinder_length() * 0.5;
	
	for (int i = 0; i < cone_data.Vertices.size(); i++)
		cone_data.Vertices[i].Position.y += transAxis.get_cylinder_length();

	std::vector<GeometryGenerator::Vertex> axis_verts(cylinder_data.Vertices);
	axis_verts.insert(axis_verts.end(), cone_data.Vertices.begin(), cone_data.Vertices.end());

	std::vector<UINT> axis_inds(cylinder_data.Indices);
	int count = cylinder_data.Vertices.size();
	transAxisCylinderIndexCount = cylinder_data.Indices.size();
	for (int i = 0; i < cone_data.Indices.size(); i++)
		axis_inds.push_back(count + cone_data.Indices[i]);

	axisVertexCount = axis_verts.size();
	rotAxisIndexBegin = axis_inds.size();
	rotAxisIndexCount = torus_data.Indices.size();

	D3D11_BUFFER_DESC buffDesc = {};
	D3D11_SUBRESOURCE_DATA initData = {};
	ZeroMemory(&buffDesc, sizeof(buffDesc));

	std::vector<Basic32> vertices;
	std::vector<unsigned int> indices(axis_inds);
	transAxisIndexCount = indices.size();
	indices.insert(indices.end(), torus_data.Indices.begin(), torus_data.Indices.end());
	for (int i = 0; i < axis_verts.size(); i++)
	{
		auto vertex = axis_verts[i];
		Basic32 vert(vertex.Position.x, vertex.Position.y, vertex.Position.z, vertex.Normal.x, vertex.Normal.y, vertex.Normal.z,
			vertex.TexC.x, vertex.TexC.y);
		vertices.push_back(vert);
	}

	for (int i = 0; i < torus_data.Vertices.size(); i++)
	{
		auto vertex = torus_data.Vertices[i];
		Basic32 vert(vertex.Position.x, vertex.Position.y, vertex.Position.z, vertex.Normal.x, vertex.Normal.y, vertex.Normal.z,
			vertex.TexC.x, vertex.TexC.y);
		vertices.push_back(vert);
	}
	
	buffDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	buffDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	buffDesc.CPUAccessFlags = 0;
	buffDesc.ByteWidth = sizeof(Basic32) * vertices.size();

	initData.pSysMem = vertices.data();

	if (FAILED(m_pd3dDevice->CreateBuffer(&buffDesc, &initData, &m_pAxisVertexBuffer)))
		return;

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(unsigned int)* indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	if (FAILED(m_pd3dDevice->CreateBuffer(&ibd, &iinitData, &m_pAxisIndexBuffer)))
		return;
}

void D3DRenderer::createFullScreenQuadBuffers()
{
	D3D11_BUFFER_DESC buffDesc = {};
	D3D11_SUBRESOURCE_DATA initData = {};
	ZeroMemory(&buffDesc, sizeof(buffDesc));

	GeometryGenerator gen;
	GeometryGenerator::MeshData mesh_data;
	gen.CreateFullscreenQuad(mesh_data);

	std::vector<Basic32> vertices;
	for (int i = 0; i < mesh_data.Vertices.size(); i++)
	{
		GeometryGenerator::Vertex v = mesh_data.Vertices[i];
		vertices.push_back(Basic32(v.Position.x, v.Position.y, v.Position.z, v.Normal.x, v.Normal.y, v.Normal.z, v.TexC.x, v.TexC.y));
	}

	buffDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	buffDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	buffDesc.CPUAccessFlags = 0;
	buffDesc.ByteWidth = sizeof(Basic32) * vertices.size();

	initData.pSysMem = vertices.data();
	if (FAILED(m_pd3dDevice->CreateBuffer(&buffDesc, &initData, &m_pQuadVertexBuffer)))
		return;

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(unsigned int)* mesh_data.Indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &mesh_data.Indices[0];
	if (FAILED(m_pd3dDevice->CreateBuffer(&ibd, &iinitData, &m_pQuadIndexBuffer)))
		return;
}

void D3DRenderer::createFrustumBuffers()
{
	D3D11_BUFFER_DESC buffDesc = {};
	D3D11_SUBRESOURCE_DATA initData = {};
	ZeroMemory(&buffDesc, sizeof(buffDesc));

	std::vector<SimpleVertex> vertices;
	float depth = m_camera.frustumDepth;
	vertices.push_back(SimpleVertex(0.0f, 0.0f, 0.0f, 0.9686, 0.59215686, 0.2));
	vertices.push_back(SimpleVertex(-1.0f, -1.0f, depth, 0.9686, 0.59215686, 0.2));
	vertices.push_back(SimpleVertex(1.0f, -1.0f, depth, 0.9686, 0.59215686, 0.2));
	vertices.push_back(SimpleVertex(1.0f, 1.0f, depth, 0.9686, 0.59215686, 0.2));
	vertices.push_back(SimpleVertex(-1.0f, 1.0f, depth, 0.9686, 0.59215686, 0.2));

	buffDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	buffDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	buffDesc.CPUAccessFlags = 0;
	buffDesc.ByteWidth = sizeof(Basic32) * vertices.size();

	initData.pSysMem = vertices.data();
	if (FAILED(m_pd3dDevice->CreateBuffer(&buffDesc, &initData, &m_pFrumstumVertexBuffer)))
		return;

	std::vector<unsigned int> indices(16);
	indices[0] = 0;
	indices[1] = 1;

	indices[2] = 0;
	indices[3] = 2;

	indices[4] = 0;
	indices[5] = 3;

	indices[6] = 0;
	indices[7] = 4;

	indices[8] = 1;
	indices[9] = 2;

	indices[10] = 2;
	indices[11] = 3;

	indices[12] = 3;
	indices[13] = 4;

	indices[14] = 4;
	indices[15] = 1;

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(unsigned int)* indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	if (FAILED(m_pd3dDevice->CreateBuffer(&ibd, &iinitData, &m_pFrumstumIndexBuffer)))
		return;
}

void D3DRenderer::createCircleBuffers()
{
	D3D11_BUFFER_DESC buffDesc = {};
	D3D11_SUBRESOURCE_DATA initData = {};
	ZeroMemory(&buffDesc, sizeof(buffDesc));

	std::vector<SimpleVertex> vertices;
	GeometryGenerator geoGen;
	GeometryGenerator::MeshData circle_data;
	geoGen.CreateCircle(1.0, 30, circle_data);

	for (int i = 0; i <circle_data.Vertices.size();i++)
	{
		auto& pos = circle_data.Vertices[i].Position;
		vertices.push_back(SimpleVertex(pos.x, pos.y, pos.z, 0.96, 0.906, 0.62));
	}

	buffDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	buffDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	buffDesc.CPUAccessFlags = 0;
	buffDesc.ByteWidth = sizeof(Basic32) * vertices.size();

	initData.pSysMem = vertices.data();
	if (FAILED(m_pd3dDevice->CreateBuffer(&buffDesc, &initData, &m_pCircleVertexBuffer)))
		return;
	
	std::vector<unsigned int> indices;
	for (int i = 0; i < circle_data.Indices.size(); i++)
		indices.push_back(circle_data.Indices[i]);

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(unsigned int)* indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	if (FAILED(m_pd3dDevice->CreateBuffer(&ibd, &iinitData, &m_pCircleIndexBuffer)))
		return;
}

void D3DRenderer::createDirectionalLightBuffers()
{
	D3D11_BUFFER_DESC buffDesc = {};
	D3D11_SUBRESOURCE_DATA initData = {};
	ZeroMemory(&buffDesc, sizeof(buffDesc));

	std::vector<SimpleVertex> vertices;
	GeometryGenerator geoGen;
	GeometryGenerator::MeshData circle_data;
	geoGen.CreateCircle(0.5, 30, circle_data);

	for (int i = 0; i < circle_data.Vertices.size(); i++)
	{
		auto& pos = circle_data.Vertices[i].Position;
		vertices.push_back(SimpleVertex(pos.x, pos.y, pos.z, 0.96, 0.906, 0.62));
	}

	int inds[] = { 0, 5, 10, 15, 20, 25 };
	for (int i = 0; i < 6; i++)
	{
		auto&vert = vertices[inds[i]];
		vertices.push_back(SimpleVertex(vert.Pos.x, vert.Pos.y, vert.Pos.z - 3.0, 0.96, 0.906, 0.62));
	}

	for (int i = 0; i < vertices.size(); i++)
	{
		auto&vert = vertices[i];
		XMVECTOR v = XMVector3TransformCoord(XMVectorSet(vert.Pos.x, vert.Pos.y, vert.Pos.z, 1.0), XMMatrixRotationY(-MathHelper::Pi * 0.5f));
		XMStoreFloat3(&vert.Pos, v);
	}

	buffDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	buffDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	buffDesc.CPUAccessFlags = 0;
	buffDesc.ByteWidth = sizeof(Basic32) * vertices.size();

	initData.pSysMem = vertices.data();
	if (FAILED(m_pd3dDevice->CreateBuffer(&buffDesc, &initData, &m_pDirLightVertexBuffer)))
		return;

	std::vector<unsigned int> indices;
	for (int i = 0; i < circle_data.Indices.size(); i++)
		indices.push_back(circle_data.Indices[i]);
	
	for (int i = 0; i < 6; i++)
	{
		indices.push_back(inds[i]);
		indices.push_back(i+circle_data.Vertices.size());
	}

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(unsigned int)* indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	if (FAILED(m_pd3dDevice->CreateBuffer(&ibd, &iinitData, &m_pDirLightIndexBuffer)))
		return;
}


void D3DRenderer::renderBoundingBox(Object* object)
{
	BasicEffect*basicEffect = Effects::BasicFX;

	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	ID3D11DeviceContext * context = m_pImmediateContext;
	context->IASetVertexBuffers(0, 1, &m_pBoundingBoxVertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(m_pBoundingBoxIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetInputLayout(InputLayouts::PosColor);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	ID3DX11EffectTechnique* activeTech = basicEffect->SimpleColorTech;
	D3DX11_TECHNIQUE_DESC techDesc;
	activeTech->GetDesc(&techDesc);
	const BoundingBox &bb = object->getBoundingBox();
	float xl = bb.top.x - bb.bottom.x;
	float yl = bb.top.y - bb.bottom.y;
	float zl = bb.top.z - bb.bottom.z;
	XMFLOAT3 c = bb.getCenter();
	XMMATRIX world_matrix = XMMatrixScaling(xl, yl, zl) * XMMatrixTranslation(c.x, c.y, c.z) * object->getGlobalWorldMatrix();
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		XMMATRIX WVP;
		WVP = world_matrix * m_camera.getViewMatrix() * m_camera.getProjMatrix();
		basicEffect->SetWorld(world_matrix);
		basicEffect->SetTexTransform(XMMatrixIdentity());
		basicEffect->SetWorldViewProj(WVP);

		activeTech->GetPassByIndex(p)->Apply(0, context);
		context->DrawIndexed(24, 0, 0);
	}
}

void D3DRenderer::renderBoundingBox(BoundingBox bb, XMFLOAT4 color)
{
	BasicEffect*basicEffect = Effects::BasicFX;

	UINT stride = sizeof(SimpleVertex);
	UINT offset = 0;
	ID3D11DeviceContext * context = m_pImmediateContext;
	context->IASetVertexBuffers(0, 1, &m_pBoundingBoxVertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(m_pBoundingBoxIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetInputLayout(InputLayouts::PosColor);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	ID3DX11EffectTechnique* activeTech = basicEffect->SimpleColorTech;
	D3DX11_TECHNIQUE_DESC techDesc;
	activeTech->GetDesc(&techDesc);
	float xl = bb.top.x - bb.bottom.x;
	float yl = bb.top.y - bb.bottom.y;
	float zl = bb.top.z - bb.bottom.z;
	XMFLOAT3 c = bb.getCenter();
	XMMATRIX world_matrix = XMMatrixScaling(xl, yl, zl) * XMMatrixTranslation(c.x, c.y, c.z);
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		XMMATRIX WVP;
		WVP = world_matrix * m_camera.getViewMatrix() * m_camera.getProjMatrix();
		basicEffect->SetWorld(world_matrix);
		basicEffect->SetTexTransform(XMMatrixIdentity());
		basicEffect->SetWorldViewProj(WVP);

		activeTech->GetPassByIndex(p)->Apply(0, context);
		context->DrawIndexed(24, 0, 0);
	}
}

void D3DRenderer::cleanup()
{
	SAFE_RELEASE(m_pRulerLineVertexBuffer);
	SAFE_RELEASE(m_pBoundingBoxVertexBuffer);
	SAFE_RELEASE(m_pBoundingBoxIndexBuffer);
	SAFE_RELEASE(m_pAxisVertexBuffer);
	SAFE_RELEASE(m_pAxisIndexBuffer);
	SAFE_RELEASE(m_pFrumstumIndexBuffer);
	SAFE_RELEASE(m_pFrumstumVertexBuffer);
	SAFE_RELEASE(m_pQuadVertexBuffer);
	SAFE_RELEASE(m_pQuadIndexBuffer);
	SAFE_DELETE(shadowMap);
	SAFE_DELETE(ssaoMap);
}