#pragma once
#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxcolors.h>

class D3DRenderer
{
public:
	D3DRenderer();
	~D3DRenderer();
	bool initD3D(HWND windowId, int width, int height);
	void initScene();
	void renderScene();
	void cleanup();

protected:
	void renderRulerLlines();
	void createRulerLlinesVertexBuffer();

	D3D_DRIVER_TYPE         m_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL       m_featureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11Device*           m_pd3dDevice = nullptr;
	ID3D11Device1*          m_pd3dDevice1 = nullptr;
	ID3D11DeviceContext*    m_pImmediateContext = nullptr;
	ID3D11DeviceContext1*   m_pImmediateContext1 = nullptr;
	IDXGISwapChain*         m_pSwapChain = nullptr;
	IDXGISwapChain1*        m_pSwapChain1 = nullptr;
	ID3D11Texture2D*        m_pDepthStencilBuffer = nullptr;
	ID3D11RenderTargetView* m_pRenderTargetView = nullptr;
	ID3D11DepthStencilView* m_pDepthStencilView = nullptr;
	ID3D11InputLayout*      m_pVertexLayout = nullptr;
	ID3D11Buffer*           m_pVertexBuffer = nullptr;
	ID3D11Buffer*           m_pIndexBuffer = nullptr;
	ID3D11Buffer*           m_pConstantBuffer = nullptr;
	ID3D11VertexShader*		m_pVertexShader = nullptr;
	ID3D11PixelShader*      m_pPixelShader = nullptr;
	ID3D11Buffer*			m_pRulerLineVertexBuffer = nullptr;

};
