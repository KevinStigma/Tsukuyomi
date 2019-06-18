#include "D3DRenderer.h"
#include <directxcolors.h>
#include <vector>
#include "common.h"
#include "Vertex.h"
#include "Globalsys.h"
#include "Effects/Effects.h"
#include "tiny_obj_loader.h"
#include "Objects/Mesh.h"

using namespace DirectX::Colors;

D3DRenderer::D3DRenderer()
{

}

D3DRenderer::~D3DRenderer()
{
	cleanup();
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

	Effects::InitAll(m_pd3dDevice);
	InputLayouts::initAll(m_pd3dDevice);

	return true;
}

void D3DRenderer::initLights()
{
	m_dirLights.resize(3);
	m_dirLights[0].Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	m_dirLights[0].Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_dirLights[0].Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_dirLights[0].Direction = XMFLOAT3(0.57735f, -0.57735f, 0.57735f);

	m_dirLights[1].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_dirLights[1].Diffuse = XMFLOAT4(0.20f, 0.20f, 0.20f, 1.0f);
	m_dirLights[1].Specular = XMFLOAT4(0.25f, 0.25f, 0.25f, 1.0f);
	m_dirLights[1].Direction = XMFLOAT3(-0.57735f, -0.57735f, 0.57735f);

	m_dirLights[2].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_dirLights[2].Diffuse = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	m_dirLights[2].Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_dirLights[2].Direction = XMFLOAT3(0.0f, -0.707f, -0.707f);
}

void D3DRenderer::initMaterials()
{
	float r = 0.73725f, g = 0.741176f, b = 0.74902f;
	m_materials.resize(6);
	m_materials[0].Ambient = XMFLOAT4(r*0.2f, g*0.2f, b*0.2f, 1.0f);
	m_materials[0].Diffuse = XMFLOAT4(r*0.6f, g*0.6f, b*0.6f, 1.0f);
	m_materials[0].Specular = XMFLOAT4(r*0.5f, g*0.5f, b*0.5f, 16.0f);

	m_materials[1].Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_materials[1].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_materials[1].Specular = XMFLOAT4(0.2f, 0.2f, 0.2f, 16.0f);

	m_materials[2].Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_materials[2].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.6f);
	m_materials[2].Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 32.0f);

	m_materials[3].Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_materials[3].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_materials[3].Specular = XMFLOAT4(0.4f, 0.4f, 0.4f, 16.0f);

	m_materials[4].Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_materials[4].Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 0.5f);
	m_materials[4].Specular = XMFLOAT4(0.4f, 0.4f, 0.4f, 16.0f);

	m_materials[5].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_materials[5].Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.5f);
	m_materials[5].Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 16.0f);
}

void D3DRenderer::initScene()
{
	m_camera.init();
	initLights();
	initMaterials();
	g_pGlobalSys->createNewObjectOfMesh("cow", "./Data/Meshes/cow.obj", true, XMFLOAT3(0.0, 0.0, 0.0), XMFLOAT3(0.01, 0.01, 0.01));
	Mesh * mesh = (Mesh*)g_pGlobalSys->getObjectFromName("cow");
	mesh->generateBuffers(m_pd3dDevice);

	createRulerLlinesVertexBuffer();
}

void D3DRenderer::renderScene()
{
	//Clear our backbuffer
	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, DirectX::Colors::Black);

	//Refresh the Depth/Stencil view
	m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	renderRulerLlines();

	renderObjects();
	//Present the backbuffer to the screen
	m_pSwapChain->Present(0, 0);
}

void D3DRenderer::renderRulerLlines()
{
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
		context->Draw(204, 0);
	}
}

void D3DRenderer::renderObjects()
{
	for (int i = 0; i < g_pGlobalSys->objects.size(); i++)
	{
		Object * obj = g_pGlobalSys->objects[i];
		obj->render(m_pImmediateContext, this);
	}
}

void D3DRenderer::createRulerLlinesVertexBuffer()
{
	D3D11_BUFFER_DESC buffDesc = {};
	D3D11_SUBRESOURCE_DATA initData = {};

	ZeroMemory(&buffDesc, sizeof(buffDesc));
	std::vector<SimpleVertex> line_pts;

	for(int i=-50;i<=50;i+=2)
	{
		SimpleVertex vertex = SimpleVertex();
		vertex.Pos = XMFLOAT3(i,0.0,-50.0);
		vertex.Color = XMFLOAT4(0.75, 0.75, 0.75, 1.0);
		line_pts.push_back(vertex);

		vertex = SimpleVertex();
		vertex.Pos = XMFLOAT3(i, 0.0, 50.0);
		vertex.Color = XMFLOAT4(0.75, 0.75, 0.75, 1.0);
		line_pts.push_back(vertex);

		vertex = SimpleVertex();
		vertex.Pos = XMFLOAT3(-50.0, 0.0, i);
		vertex.Color = XMFLOAT4(0.75, 0.75, 0.75, 1.0);
		line_pts.push_back(vertex);

		vertex = SimpleVertex();
		vertex.Pos = XMFLOAT3(50.0, 0.0, i);
		vertex.Color = XMFLOAT4(0.75, 0.75, 0.75, 1.0);
		line_pts.push_back(vertex);
	}

	buffDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	buffDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
	buffDesc.CPUAccessFlags = 0;
	buffDesc.ByteWidth = sizeof(SimpleVertex) * line_pts.size();

	initData.pSysMem = line_pts.data();

	if (FAILED(m_pd3dDevice->CreateBuffer(&buffDesc, &initData, &m_pRulerLineVertexBuffer)))
		return;
}

void D3DRenderer::cleanup()
{
	SAFE_RELEASE(m_pIndexBuffer);
	SAFE_RELEASE(m_pVertexBuffer);
	SAFE_RELEASE(m_pRulerLineVertexBuffer);
}