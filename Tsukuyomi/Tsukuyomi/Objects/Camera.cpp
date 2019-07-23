#include "Camera.h"
#include <iostream>

Camera::Camera(std::string name, XMFLOAT3 t, XMFLOAT3 s, XMFLOAT3 r):Object(name, t, s, r)
{
	fov = 0.25f * XM_PI;
	zNear = 0.01f;
	zFar = 1000.0f;
	aspectRatio = 800.0f / 600.0f;
	ZeroMemory(&mProj, sizeof(XMFLOAT4X4));
	setLens(fov, aspectRatio, zNear, zFar);
	updateViewMatrix();
}

void Camera::init()
{
	translation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	genereateWorldMatrix();
	updateViewMatrix();
}

XMMATRIX Camera::getViewMatrix()const
{
	return XMLoadFloat4x4(&mView);
}
XMMATRIX Camera::getProjMatrix()const
{
	return XMLoadFloat4x4(&mProj);
}

XMMATRIX Camera::getViewProjMatrix()const
{
	return XMLoadFloat4x4(&mViewProj);
}

void Camera::setLens(float fovY, float aspect, float zn, float zf)
{
	// cache properties
	fov = fovY;
	aspectRatio = aspect;
	zNear = zn;
	zFar = zf;

	XMStoreFloat4x4(&mProj, XMMatrixPerspectiveFovLH(fov, aspectRatio, zNear, zFar));
}

void Camera::setPosition(float posX, float posY, float posZ)
{
	translation.x = posX;
	translation.y = posY;
	translation.z = posZ;
	updateViewMatrix();
}

XMFLOAT3 Camera::getPosition()
{
	return translation;
}

XMFLOAT3 Camera::getLook()
{
	XMFLOAT3 look;
	XMStoreFloat3(&look, XMVector3TransformNormal(XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), rot_mat));
	return look;
}

XMFLOAT3 Camera::getUp()
{
	XMFLOAT3 up;
	XMStoreFloat3(&up, XMVector3TransformNormal(XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), rot_mat));
	return up;
}

XMFLOAT3 Camera::getRight()
{
	XMFLOAT3 right;
	XMStoreFloat3(&right, XMVector3TransformNormal(XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), rot_mat));
	return right;
}

void Camera::lookAt(XMFLOAT3 pos, XMFLOAT3 target, XMFLOAT3 worldUp)
{
	world_mat  = XMMatrixLookAtLH(XMVectorSet(pos.x, pos.y, pos.z, 1.0), XMVectorSet(target.x, target.y, target.z, 1.0), XMVectorSet(worldUp.x, worldUp.y, worldUp.z, 1.0));
	trans_mat = XMMatrixTranslation(pos.x, pos.y, pos.z);
	rot_mat = world_mat * XMMatrixTranslation(-pos.x, -pos.y, -pos.z);
	updateViewMatrix();
}

void Camera::updateViewMatrix()
{
	ZeroMemory(&mView, sizeof(XMFLOAT4X4));
	ZeroMemory(&mViewProj, sizeof(XMFLOAT4X4));

	XMFLOAT3 right, look, up;
	XMFLOAT3 position = getPosition();
	right = getRight();
	look = getLook();
	up = getUp();

	mView(0, 0) = right.x;
	mView(1, 0) = right.y;
	mView(2, 0) = right.z;
	mView(3, 0) = 0.0f;

	mView(0, 1) = up.x;
	mView(1, 1) = up.y;
	mView(2, 1) = up.z;
	mView(3, 1) = 0.0f;

	mView(0, 2) = look.x;
	mView(1, 2) = look.y;
	mView(2, 2) = look.z;
	mView(3, 2) = 0.0f;

	XMVECTOR Q = XMLoadFloat3(&position);
	XMVECTOR u = XMLoadFloat3(&right);
	XMVECTOR v = XMLoadFloat3(&up);
	XMVECTOR w = XMLoadFloat3(&look);

	mView(3, 0) = -XMVectorGetX(XMVector3Dot(Q, u));
	mView(3, 1) = -XMVectorGetX(XMVector3Dot(Q, v));
	mView(3, 2) = -XMVectorGetX(XMVector3Dot(Q, w));
	mView(3, 3) = 1.0f;

	XMMATRIX camView = DirectX::XMLoadFloat4x4(&mView);
	XMMATRIX camViewProj = camView * XMLoadFloat4x4(&mProj);
	XMStoreFloat4x4(&mView, camView);
	XMStoreFloat4x4(&mViewProj, camViewProj);
}

void Camera::walkForward(float d)
{
	XMFLOAT3 look = getLook();
	translation.x += d * look.x;
	translation.y += d * look.y;
	translation.z += d * look.z;
	updateViewMatrix();
}

void Camera::walkRight(float d)
{
	XMFLOAT3 right = getRight();
	translation.x += d * right.x;
	translation.y += d * right.y;
	translation.z += d * right.z;

	updateViewMatrix();
}

void Camera::walkUp(float d)
{
	XMFLOAT3 up = getUp();
	translation.x += d * up.x;
	translation.y += d * up.y;
	translation.z += d * up.z;
	updateViewMatrix();
}

void Camera::rotate(XMFLOAT3 axis, float radian)
{
	XMMATRIX R = XMMatrixRotationAxis(XMVectorSet(axis.x, axis.y, axis.z, 0.0), radian);
	rot_mat = XMMatrixMultiply(rot_mat, R);
	updateViewMatrix();
}

void Camera::rotateY(float radian)
{
	rotate(XMFLOAT3(0.0, 1.0, 0.0), radian);
}

void Camera::rotateRight(float radian)
{
	rotate(getRight(), radian);
}

void Camera::updateAspectRatio(float ratio)
{
	aspectRatio = ratio;
	setLens(0.5f* XM_PI, aspectRatio, 0.01f, 1000.0f);
	XMMATRIX camView = DirectX::XMLoadFloat4x4(&mView);
	XMMATRIX camViewProj = camView * XMLoadFloat4x4(&mProj);
	XMStoreFloat4x4(&mViewProj, camViewProj);
}

Ray Camera::getRay(float width_ratio, float height_ratio)const
{
	float frustum_height = zNear * tanf(fov * 0.5f);
	float y = frustum_height * (height_ratio - 0.5f) * 2.0f;
	float x = frustum_height * aspectRatio * (width_ratio - 0.5f) * 2.0f;
	XMVECTOR v;
	XMMATRIX inv_view = XMMatrixInverse(&v, DirectX::XMLoadFloat4x4(&mView));
	XMFLOAT3 world_dir;
	XMStoreFloat3(&world_dir, XMVector3TransformNormal(XMVector3Normalize(XMVectorSet(x, y, zNear, 0.0)), inv_view));
	return Ray(translation, world_dir);
}

XMFLOAT2 Camera::projectCoord(XMFLOAT3 pos)
{
	XMVECTOR v = XMVectorSet(pos.x, pos.y, pos.z, 1.0);
	XMMATRIX camView = DirectX::XMLoadFloat4x4(&mView);
	XMMATRIX camViewProj = camView * XMLoadFloat4x4(&mProj);
	v = XMVector3TransformCoord(v, camViewProj);
	return XMFLOAT2(XMVectorGetX(v), XMVectorGetY(v));
}

XMFLOAT3 Camera::unprojectCoord(XMFLOAT2 pos, float depth)const
{
	float frustum_height = depth * tanf(fov * 0.5f);
	float y = frustum_height * pos.y;
	float x = frustum_height * aspectRatio * pos.x;
	XMVECTOR v;
	XMMATRIX inv_view = XMMatrixInverse(&v, DirectX::XMLoadFloat4x4(&mView));
	XMVECTOR view_pos = XMVectorSet(x, y, depth, 1.0f);
	XMFLOAT3 world_pos;
	XMStoreFloat3(&world_pos, XMVector3TransformCoord(view_pos, inv_view));
	return world_pos;
}
