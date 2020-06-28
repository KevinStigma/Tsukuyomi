#pragma once
#include "Object.h"
#include "LightHelper.h"
#include "../MathHelper/MathHelper.h"

class Camera : public Object
{
public:
	Camera(std::string name, XMFLOAT3 t = XMFLOAT3(0.0, 0.0, 0.0), XMFLOAT3 s = XMFLOAT3(1.0, 1.0, 1.0), XMFLOAT3 r = XMFLOAT3(0.0, 0.0, 0.0));
	void init();
	XMMATRIX getViewMatrix()const;
	XMMATRIX getProjMatrix()const;
	XMMATRIX getViewProjMatrix()const;
	void walkForward(float d);
	void walkRight(float d);
	void walkUp(float d);
	void lookAt(XMFLOAT3 pos, XMFLOAT3 target, XMFLOAT3 worldUp);
	void setPosition(float posX, float posY, float posZ);
	XMFLOAT3 getPosition();
	XMFLOAT3 getLook();
	XMFLOAT3 getUp();
	XMFLOAT3 getRight();
	void setLens(float fovY, float aspect, float zn, float zf);
	void updateViewMatrix();
	void rotate(XMFLOAT3 axis, float radian);
	void rotateY(float radian);
	void rotateRight(float radian);
	void updateAspectRatio(float ratio);
	Ray getRay(float width_ratio, float height_ratio)const;
	XMFLOAT2 projectCoord(XMFLOAT3 pos);
	XMFLOAT2 getProjPlaneSize(float depth);
	XMFLOAT3 unprojectCoord(XMFLOAT2 pos, float depth)const;
	XMMATRIX getFrustumMatrix();

	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;
	XMFLOAT4X4 mViewProj;
	float fov;
	float zNear;
	float zFar;
	float aspectRatio=800.0f/600.0f;
	static float frustumDepth;
};
