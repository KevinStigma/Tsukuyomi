#pragma once
#include <vector>
#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxcolors.h>
#include "Objects/Camera.h"
#include "Axis/TransAxis.h"
#include "Axis/RotAxis.h"
#include "ShadowMap.h"
#include "ShadowTransform.h"

class Object;
enum  RenderSelObjMode { NONE, COORD_AXIS, ROT_AXIS};

class D3DRenderer
{
public:
	D3DRenderer();
	~D3DRenderer();
	void updateCamView(int width, int height);
	bool initD3D(HWND windowId, int width, int height);
	bool resizeD3D(int width, int height);
	void initScene();
	void renderScene();
	void cleanup();
	void renderToShadowMap();
	ShadowMap* getShadowMap() { return shadowMap; }
	Camera& getCamera() { return m_camera; }
	std::vector<RenderLightHelper::DirLight> & getLights() { return m_dirLights; }
	std::vector<RenderLightHelper::Material> & getMaterials() { return m_materials; }
	ID3D11Device* getDevice() { return m_pd3dDevice; }
	RenderSelObjMode getRenderSelObjMode() { return renderSelObjMode; }
	void setRenderSelObjMode(RenderSelObjMode mode) { renderSelObjMode = mode; }
	void rayAxisIntersectionDetect(float x_ratio, float y_ratio);
	void translateSelObj(XMFLOAT2 mouse_move_dir);
	void rotateSelObj(XMFLOAT2 np1, XMFLOAT2 np2);
	AXIS getCurSelAxis() { return curSelAxis; }
	void enableMSAA(bool enabled);
	void resetCameraTransform(Camera* cam);
	void buildShadowTransform();
	float computeFPS();
	ShadowTransform getShadowMapTransform() { return m_shadowTransform; }
	
protected:
	void initLights();
	void initMaterials();
	void updateLights();
	void renderRulerLlines();
	void renderSelObjFlag();
	void renderBVH();
	void renderFrustum(FXMMATRIX trans_mat);
	void renderWireFrameSphere(Object * obj);
	void renderDirectionalLight(Object* obj);
	void renderBoundingBox(Object* object);
	void renderBoundingBox(BoundingBox bb, XMFLOAT4 color);
	void renderCoordAxis(Object* obj);
	void renderRotAxis(Object* obj);
	void createRulerLlinesVertexBuffer();
	void createBoundingBoxBuffers();
	void createSelObjAxisBuffers();
	void createFrustumBuffers();
	void createCircleBuffers();
	void createDirectionalLightBuffers();

	Camera m_camera;
	D3D_DRIVER_TYPE         m_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL       m_featureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11Device*           m_pd3dDevice = nullptr;
	ID3D11Device1*          m_pd3dDevice1 = nullptr;
	ID3D11DeviceContext*    m_pImmediateContext = nullptr;
	ID3D11DeviceContext1*   m_pImmediateContext1 = nullptr;
	IDXGISwapChain*         m_pSwapChain = nullptr;
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
	ID3D11Buffer*			m_pFrumstumVertexBuffer = nullptr;
	ID3D11Buffer*           m_pFrumstumIndexBuffer = nullptr;
	ID3D11Buffer*			m_pCircleVertexBuffer = nullptr;
	ID3D11Buffer*           m_pCircleIndexBuffer = nullptr;
	ID3D11Buffer*			m_pDirLightVertexBuffer = nullptr;
	ID3D11Buffer*           m_pDirLightIndexBuffer = nullptr;
	RenderSelObjMode        renderSelObjMode = COORD_AXIS;
	D3D11_VIEWPORT          m_screenViewport;
	int						transAxisCylinderIndexCount;
	int						transAxisIndexCount;
	int                     rotAxisIndexBegin;
	int                     rotAxisIndexCount;
	int                     axisVertexCount;
	clock_t                 last_clock = 0;
	ShadowMap				*shadowMap;
	ShadowTransform		    m_shadowTransform;
	AXIS                    curSelAxis = AXIS::NO;
	XMFLOAT2                curSelAxisProjDir;
	std::vector<RenderLightHelper::DirLight> m_dirLights;
	std::vector<RenderLightHelper::Material> m_materials;
	TransAxis				transAxis;
	RotAxis					rotAxis;
};
