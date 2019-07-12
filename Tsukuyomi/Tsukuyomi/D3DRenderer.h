#pragma once
#include <vector>
#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxcolors.h>
#include "Camera.h"
#include "Axis/TransAxis.h"
#include "Axis/RotAxis.h"

class Object;
enum  RenderSelObjMode { NONE, COORD_AXIS, ROT_AXIS};

class D3DRenderer
{
public:
	D3DRenderer();
	~D3DRenderer();
	bool initD3D(HWND windowId, int width, int height);
	void initScene();
	void renderScene();
	void cleanup();
	Camera& getCamera() { return m_camera; }
	std::vector<DirectionalLight> & getLights() { return m_dirLights; }
	std::vector<Material> & getMaterials() { return m_materials; }
	ID3D11Device* getDevice() { return m_pd3dDevice; }
	RenderSelObjMode getRenderSelObjMode() { return renderSelObjMode; }
	void setRenderSelObjMode(RenderSelObjMode mode) { renderSelObjMode = mode; }
	void rayAxisIntersectionDetect(float x_ratio, float y_ratio);
	void translateSelObj(XMFLOAT2 mouse_move_dir);
	void rotateSelObj(XMFLOAT2 np1, XMFLOAT2 np2);
	AXIS getCurSelAxis() { return curSelAxis; }

protected:
	void initLights();
	void initMaterials();
	void renderRulerLlines();
	void renderSelObjFlag();
	void renderBoundingBox(Object* object);
	void renderCoordAxis(Object* obj);
	void renderRotAxis(Object* obj);
	void createRulerLlinesVertexBuffer();
	void createBoundingBoxBuffers();
	void createSelObjAxisBuffers();

	Camera m_camera;
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
	ID3D11Buffer*           m_pConstantBuffer = nullptr;
	ID3D11VertexShader*		m_pVertexShader = nullptr;
	ID3D11PixelShader*      m_pPixelShader = nullptr;
	ID3D11Buffer*			m_pRulerLineVertexBuffer = nullptr;
	ID3D11Buffer*			m_pBoundingBoxVertexBuffer = nullptr;
	ID3D11Buffer*           m_pBoundingBoxIndexBuffer = nullptr;
	ID3D11Buffer*			m_pAxisVertexBuffer = nullptr;
	ID3D11Buffer*           m_pAxisIndexBuffer = nullptr;
	RenderSelObjMode        renderSelObjMode = COORD_AXIS;
	int						transAxisCylinderIndexCount;
	int						transAxisIndexCount;
	int                     rotAxisIndexBegin;
	int                     rotAxisIndexCount;
	int                     axisVertexCount;
	AXIS                    curSelAxis = AXIS::NO;
	XMFLOAT2                curSelAxisProjDir;
	std::vector<DirectionalLight> m_dirLights;
	std::vector<Material> m_materials;
	TransAxis				transAxis;
	RotAxis					rotAxis;
};
