#include "D3DRenderer.h"
#include <directxcolors.h>
#include <vector>
#include "common.h"
#include "Vertex.h"
#include "Globalsys.h"
#include "Effects/Effects.h"
#include "tiny_obj_loader.h"
#include "Objects/Mesh.h"
#include "./GeometryGenerator/GeometryGenerator.h"

using namespace DirectX::Colors;

D3DRenderer::D3DRenderer()
{
	g_pGlobalSys->renderer = this;
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

	m_materials[2].Ambient = XMFLOAT4(0.0f, 0.5f, 0.0f, 1.0f);
	m_materials[2].Diffuse = XMFLOAT4(0.0f, 0.8f, 0.0f, 1.0f);
	m_materials[2].Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 32.0f);

	m_materials[3].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_materials[3].Diffuse = XMFLOAT4(0.8f, 0.0f, 0.0f, 1.0f);
	m_materials[3].Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 32.0f);

	m_materials[4].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_materials[4].Diffuse = XMFLOAT4(0.0f, 0.0f, 0.8f, 1.0f);
	m_materials[4].Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 32.0f);

	m_materials[5].Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_materials[5].Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.5f);
	m_materials[5].Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 16.0f);
}

void D3DRenderer::initScene()
{
	m_camera.init();
	initLights();
	initMaterials();
	createRulerLlinesVertexBuffer();
	createBoundingBoxBuffers();
	createSelObjAxisBuffers();
}

void D3DRenderer::renderScene()
{
	//Clear our backbuffer
	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, DirectX::Colors::Black);

	//Refresh the Depth/Stencil view
	m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	renderRulerLlines();

	g_pGlobalSys->objectManager.renderAllObjects(m_pImmediateContext, this);

	renderSelObjFlag();
	//Present the backbuffer to the screen
	m_pSwapChain->Present(0, 0);
}

void D3DRenderer::renderSelObjFlag()
{
	Object* sel_obj = g_pGlobalSys->objectManager.getCurSelObject();
	if (sel_obj)
	{
		renderBoundingBox(sel_obj);
		renderAxis(sel_obj);
	}
	renderRotAxis(nullptr);
}

void D3DRenderer::renderRotAxis(Object* obj)
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

	XMFLOAT3 eyePosW(m_camera.position.x, m_camera.position.y, m_camera.position.z);
	basicEffect->SetDirLights(&m_dirLights[0]);
	basicEffect->SetEyePosW(eyePosW);

	ID3DX11EffectTechnique* activeTech = basicEffect->Light1Tech;
	D3DX11_TECHNIQUE_DESC techDesc;
	activeTech->GetDesc(&techDesc);

	XMMATRIX worldMat = XMMatrixIdentity();
	XMMATRIX WVP = worldMat * m_camera.getViewMatrix() * m_camera.getProjMatrix();

	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		basicEffect->SetWorld(worldMat);
		basicEffect->SetWorldInvTranspose(worldMat);
		basicEffect->SetWorldViewProj(WVP);
		basicEffect->SetMaterial(m_materials[2]);
		activeTech->GetPassByIndex(p)->Apply(0, context);
		context->DrawIndexed(rotAxisIndexCount * 0.5, rotAxisIndexBegin, axisVertexCount);
	}
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
		context->Draw(214, 0);
	}
}

void D3DRenderer::renderAxis(Object* obj)
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

	XMFLOAT3 eyePosW(m_camera.position.x, m_camera.position.y, m_camera.position.z);
	basicEffect->SetDirLights(&m_dirLights[0]);
	basicEffect->SetEyePosW(eyePosW);

	ID3DX11EffectTechnique* activeTech = basicEffect->Light1Tech;
	D3DX11_TECHNIQUE_DESC techDesc;
	activeTech->GetDesc(&techDesc);

	XMVECTOR v;
	const BoundingBox &bb = obj->getBoundingBox();
	XMFLOAT3 center = bb.center;
	XMMATRIX trans_center_mat = XMMatrixTranslationFromVector(XMVector3TransformCoord(XMVectorSet(center.x, center.y, center.z, 1.0), obj->getWorldMatrix()));

	XMMATRIX axisTrans = XMMatrixIdentity();
	XMMATRIX scale_mat = XMMatrixScaling(1.5, 1.5, 1.5);
	
	XMMATRIX worldMat = scale_mat * axisTrans * trans_center_mat;
	XMMATRIX inv_world_mat = XMMatrixInverse(&v, worldMat);
	XMMATRIX WVP = worldMat * m_camera.getViewMatrix() * m_camera.getProjMatrix();

	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		basicEffect->SetWorld(worldMat);
		basicEffect->SetWorldInvTranspose(worldMat);
		basicEffect->SetWorldViewProj(WVP);
		basicEffect->SetMaterial(m_materials[2]);
		activeTech->GetPassByIndex(p)->Apply(0, context);
		context->DrawIndexed(axisIndexCount, 0, 0);

		axisTrans = XMMatrixRotationZ(-MathHelper::Pi * 0.5f);
		worldMat = scale_mat * axisTrans * trans_center_mat;
		inv_world_mat = XMMatrixInverse(&v, worldMat);
		WVP = worldMat * m_camera.getViewMatrix() * m_camera.getProjMatrix();
		basicEffect->SetWorld(worldMat);
		basicEffect->SetWorldInvTranspose(worldMat);
		basicEffect->SetWorldViewProj(WVP);
		basicEffect->SetMaterial(m_materials[3]);
		activeTech->GetPassByIndex(p)->Apply(0, context);
		context->DrawIndexed(axisIndexCount, 0, 0);

		axisTrans = XMMatrixRotationX(MathHelper::Pi * 0.5f);
		worldMat = scale_mat * axisTrans * trans_center_mat;
		inv_world_mat = XMMatrixInverse(&v, worldMat);
		WVP = worldMat * m_camera.getViewMatrix() * m_camera.getProjMatrix();
		basicEffect->SetWorld(worldMat);
		basicEffect->SetWorldInvTranspose(worldMat);
		basicEffect->SetWorldViewProj(WVP);
		basicEffect->SetMaterial(m_materials[4]);

		activeTech->GetPassByIndex(p)->Apply(0, context);
		context->DrawIndexed(axisIndexCount, 0, 0);
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
	geoGen.CreateCylinder(0.025, 0.025, 0.8, 4, 4, cylinder_data);

	GeometryGenerator::MeshData cone_data;
	geoGen.CreateCylinder(0.05, 0.0, 0.2, 4, 4, cone_data);

	GeometryGenerator::MeshData torus_data;
	geoGen.CreateTorus(1.0, 0.2, 8, 8, torus_data);

	for (int i = 0; i < cylinder_data.Vertices.size(); i++)
		cylinder_data.Vertices[i].Position.y += 0.4;
	
	for (int i = 0; i < cone_data.Vertices.size(); i++)
		cone_data.Vertices[i].Position.y += 0.8;

	std::vector<GeometryGenerator::Vertex> axis_verts(cylinder_data.Vertices);
	axis_verts.insert(axis_verts.end(), cone_data.Vertices.begin(), cone_data.Vertices.end());

	std::vector<UINT> axis_inds(cylinder_data.Indices);
	int count = cylinder_data.Vertices.size();
	for (int i = 0; i < cone_data.Indices.size(); i++)
		axis_inds.push_back(count + cone_data.Indices[i]);

	/*count = axis_verts.size();
	for (int i = 0; i < torus_data.Indices.size(); i++)
		torus_data.Indices[i] += count;*/
	axisVertexCount = axis_verts.size();
	rotAxisIndexBegin = axis_inds.size();
	rotAxisIndexCount = torus_data.Indices.size();

	D3D11_BUFFER_DESC buffDesc = {};
	D3D11_SUBRESOURCE_DATA initData = {};
	ZeroMemory(&buffDesc, sizeof(buffDesc));

	std::vector<Basic32> vertices;
	std::vector<unsigned int> indices(axis_inds);
	indices.insert(indices.end(), torus_data.Indices.begin(), torus_data.Indices.end());
	XMMATRIX rot_mat_x = XMMatrixRotationX(1.5707963);
	XMMATRIX rot_mat_z = XMMatrixRotationZ(-1.5707963);
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
	axisIndexCount = indices.size();
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
	XMFLOAT3 c = bb.center;
	XMMATRIX world_matrix = XMMatrixScaling(xl, yl, zl) * XMMatrixTranslation(c.x, c.y, c.z) * object->getWorldMatrix();
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
}