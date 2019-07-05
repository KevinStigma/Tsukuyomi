#pragma once
#include "LightHelper.h"
#include "MathHelper/MathHelper.h"

class Camera
{
public:
	void init();
	XMMATRIX getViewMatrix()const;
	XMMATRIX getProjMatrix()const;
	XMMATRIX getViewProjMatrix()const;
	void walkForward(float d);
	void walkRight(float d);
	void walkUp(float d);
	void lookAt(FXMVECTOR pos, FXMVECTOR target, FXMVECTOR worldUp);
	void setPosition(float posX, float posY, float posZ);
	void setLens(float fovY, float aspect, float zn, float zf);
	void updateViewMatrix();
	void updateViewMatrix(XMFLOAT3 pos, XMFLOAT3 t, XMFLOAT3 u);
	void updateRight();
	void rotate(XMFLOAT3 axis, float radian);
	void rotateY(float radian);
	void rotateRight(float radian);
	void updateAspectRatio(float ratio);
	Ray getRay(float width_ratio, float height_ratio);
	XMFLOAT2 projectCoord(XMFLOAT3 pos);

	XMFLOAT3 position;
	XMFLOAT3 up;
	XMFLOAT3 right;
	XMFLOAT3 look;
	XMFLOAT4X4 mView;
	XMFLOAT4X4 mProj;
	XMFLOAT4X4 mViewProj;
	float fov;
	float zNear;
	float zFar;
	float aspectRatio=800.0f/600.0f;
};