#pragma once
#include "../common.h"
#include "../MathHelper/MathHelper.h"
#include "tiny_obj_loader.h"
#include <string>
#include <windows.h>
#include <d3d11_1.h>
#include <DirectXMath.h>

using namespace DirectX;
class D3DRenderer;

enum ObjectType {EMPTY, MESH, POINT_LIGHT, CAM, DIR_LIGHT, AREA_LIGHT};

struct BoundingBox
{
	XMFLOAT3 top, bottom;
	BoundingBox();
	BoundingBox(XMFLOAT3 t,XMFLOAT3 b):top(t), bottom(b) {}
	XMFLOAT3 getCenter()const {
		return XMFLOAT3((top.x + bottom.x)*0.5f, (top.y + bottom.y)*0.5f, (top.z + bottom.z)*0.5f);
	}

	float getCenterValFromDim(int dim)const {
		return (getTopFromDim(dim) + getBottomFromDim(dim))*0.5f;
	}
	float surfaceArea();
	bool inBox(XMFLOAT3 v)
	{
		if (bottom.x <= v.x&&top.x >= v.x&&bottom.y <= v.y&&top.y >= v.y&&bottom.z <= v.z&&top.z >= v.z)
			return true;
		return false;
	}

	XMMATRIX getTransMatrix()const { XMFLOAT3 c = getCenter(); return XMMatrixTranslation(c.x, c.y, c.z); }
	bool isIntersect(const Ray&ray, XMMATRIX world_mat);
	int maximumExtent();
	float getTopFromDim(int dim)const;
	float getBottomFromDim(int dim)const;
	static BoundingBox Union(BoundingBox&b1, BoundingBox&b2)
	{
		BoundingBox b;
		b.bottom = XMFLOAT3(std::min<float>(b1.bottom.x, b2.bottom.x),
			std::min<float>(b1.bottom.y, b2.bottom.y),
			std::min<float>(b1.bottom.z, b2.bottom.z));
		b.top = XMFLOAT3(std::max<float>(b1.top.x, b2.top.x),
			std::max<float>(b1.top.y, b2.top.y),
			std::max<float>(b1.top.z, b2.top.z));
		return b;
	}

};

class Object
{
public:
	Object(std::string name, XMFLOAT3 t = XMFLOAT3(0.0, 0.0, 0.0), XMFLOAT3 s = XMFLOAT3(1.0, 1.0, 1.0), XMFLOAT3 r = XMFLOAT3(0.0, 0.0, 0.0));
	~Object();
	virtual bool isEmpty();
	virtual bool isLight() { return false; }
	void updateTransform(XMFLOAT3 t, XMFLOAT3 s, XMFLOAT3 r);
	virtual void render(ID3D11DeviceContext * context, D3DRenderer* renderer);
	std::string getName() { return name; }
	void setName(std::string obj_name) { name = obj_name; }
	ObjectType getType() { return type; }
	std::string getTranslationText();
	std::string getScaleText();
	std::string getRotationText();
	std::unordered_map<std::string, Object*> getChilds() { return childs; }
	Object* getParent() { return parent; }
	void setParent(Object* p);
	virtual const BoundingBox & getBoundingBox() { return boundingBox; }
	virtual void genereateWorldMatrix();
	XMMATRIX getWorldMatrix()const { return world_mat; }
	XMMATRIX getRotMatrix() { return rot_mat; }
	XMMATRIX getTransMatrix() { return trans_mat; }
	XMMATRIX getScaleMatrix() { return scale_mat; }
	XMMATRIX getGlobalWorldMatrix()const;
	XMFLOAT3 getTranslation()const { return translation; }
	XMFLOAT3 getRotation()const { return rotation; }
	XMFLOAT3 getScale()const { return scale; }
	virtual void setScale(XMFLOAT3 s);
	virtual void setRotation(XMFLOAT3 r);
	virtual void setTranslation(XMFLOAT3 t);
	bool is_intersect_bounding_box(const Ray&ray);
protected:
	Object* parent = nullptr;
	std::unordered_map<std::string, Object*> childs;
	std::string name;
	ObjectType type;
	BoundingBox boundingBox;
	XMFLOAT3 translation;
	XMFLOAT3 scale;
	XMFLOAT3 rotation;
	XMMATRIX scale_mat;
	XMMATRIX rot_mat;
	XMMATRIX trans_mat;
	XMMATRIX world_mat;
};