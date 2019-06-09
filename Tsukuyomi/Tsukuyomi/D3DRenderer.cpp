#include "D3DRenderer.h"
#include <directxcolors.h>
#include <vector>
#include "Vertex.h"
#include "Globalsys.h"
using namespace DirectX::Colors;

D3DRenderer::D3DRenderer()
{

}

D3DRenderer::~D3DRenderer()
{

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

	return true;
}

void D3DRenderer::initScene()
{
	g_pGlobalSys->loadObjMesh("Data/Meshes/cow.obj");
	g_pGlobalSys->reconNormals(g_pGlobalSys->objects[0]);
	std::vector<Basic32> vertices;
	const std::vector<float>& positions = g_pGlobalSys->objects[0].mesh.positions;
	const std::vector<float>& normals = g_pGlobalSys->objects[0].mesh.normals;
	const std::vector<float>& texs = g_pGlobalSys->objects[0].mesh.texcoords;
	bool m_hasTex = false;
	if (texs.size())
		m_hasTex = true;

	int num_vertex = positions.size() / 3;
	for (int i = 0; i < num_vertex; i++)
	{
		Basic32 v(positions[i * 3], positions[i * 3 + 1], positions[i * 3 + 2],
			normals[i * 3], normals[i * 3 + 1], normals[i * 3 + 2], 0, 0);
		if (m_hasTex)
		{
			v.tex.x = texs[i * 2];
			v.tex.y = texs[i * 2 + 1];
		}
		vertices.push_back(v);
	}

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Basic32)* vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	m_pd3dDevice->CreateBuffer(&vbd, &vinitData, &m_pVertexBuffer);

	const std::vector<unsigned int>& indices = g_pGlobalSys->objects[0].mesh.indices;

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(unsigned int)* indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	m_pd3dDevice->CreateBuffer(&ibd, &iinitData, &m_pIndexBuffer);

	createRulerLlinesVertexBuffer();
}

void D3DRenderer::renderScene()
{
	//Clear our backbuffer
	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, DirectX::Colors::Black);

	//Refresh the Depth/Stencil view
	m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	renderRulerLlines();

	//Present the backbuffer to the screen
	m_pSwapChain->Present(0, 0);
}

void D3DRenderer::renderRulerLlines()
{
	m_pImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

}

void D3DRenderer::createRulerLlinesVertexBuffer()
{
	D3D11_BUFFER_DESC buffDesc = {};
	D3D11_SUBRESOURCE_DATA initData = {};

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

	if (FAILED(m_pd3dDevice->CreateBuffer(&buffDesc, &initData, &m_pVertexBuffer)))
		return;
}

void D3DRenderer::cleanup()
{

}