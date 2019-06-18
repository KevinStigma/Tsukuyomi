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
	Object(std::string name, XMFLOAT3 t = XMFLOAT3(0.0, 0.0, 0.0), XMFLOAT3 s = XMFLOAT3(1.0, 1.0, 1.0), XMFLOAT4 r = XMFLOAT4(0.0, 0.0, 0.0, 1.0));
	~Object();
	virtual bool isEmpty();
	void setTranslation(XMFLOAT3 t);
	void setScale(XMFLOAT3 s);
	void setRotation(XMFLOAT4 r);
	void updateTransform(XMFLOAT3 t, XMFLOAT3 s, XMFLOAT4 r);
	virtual void render(ID3D11DeviceContext * context, D3DRenderer* renderer);
	std::string getName() { return name; }
	void setName(std::string obj_name) { name = obj_name; }
	ObjectType getType() { return type; }

protected:
	XMFLOAT3 translation;
	XMFLOAT3 scale;
	XMFLOAT4 rotation;
	std::string name;
	ObjectType type;
};