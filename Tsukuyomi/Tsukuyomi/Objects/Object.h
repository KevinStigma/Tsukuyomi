#pragma once
#include "../common.h"
#include "../Camera.h"
#include "tiny_obj_loader.h"
#include <string>
#include <windows.h>
#include <d3d11_1.h>
#include <DirectXMath.h>

using namespace DirectX;

enum ObjectType {EMPTY, MESH, LIGHT};
class D3DRenderer;

class Object
{
public:
	Object(std::string name, XMFLOAT3 t = XMFLOAT3(0.0, 0.0, 0.0), XMFLOAT3 s = XMFLOAT3(1.0, 1.0, 1.0), XMFLOAT3 r = XMFLOAT3(0.0, 0.0, 0.0));
	~Object();
	virtual bool isEmpty();
	void updateTransform(XMFLOAT3 t, XMFLOAT3 s, XMFLOAT3 r);
	virtual void render(ID3D11DeviceContext * context, D3DRenderer* renderer);
	std::string getName() { return name; }
	void setName(std::string obj_name) { name = obj_name; }
	ObjectType getType() { return type; }
	std::string getTranslationText();
	std::string getScaleText();
	std::string getRotationText();
	XMMATRIX genereateWorldMatrix();
	XMFLOAT3 translation;
	XMFLOAT3 scale;
	XMFLOAT3 rotation;

protected:
	std::string name;
	ObjectType type;
};