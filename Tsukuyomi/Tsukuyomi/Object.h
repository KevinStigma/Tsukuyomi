#pragma once
#include "common.h"
#include "tiny_obj_loader.h"
#include <string>
#include <DirectXMath.h>

using namespace DirectX;

class Object
{
public:
	Object(std::string name, std::string obj_path = "", XMFLOAT3 t = XMFLOAT3(0.0, 0.0, 0.0), XMFLOAT3 s = XMFLOAT3(1.0, 1.0, 1.0), XMFLOAT4 r = XMFLOAT4(0.0, 0.0, 0.0, 1.0));
	~Object();
	bool isEmpty();
	void loadObjMesh(const std::string & obj_path);
	void setTranslation(XMFLOAT3 t);
	void setScale(XMFLOAT3 s);
	void setRotation(XMFLOAT4 r);
	void updateTransform(XMFLOAT3 t, XMFLOAT3 s, XMFLOAT4 r);
	void constructNormals();
	std::string getName() { return name; }
	void setName(std::string obj_name) { name = obj_name; }
	tinyobj::mesh_t * getMesh() { if (isEmpty()) return nullptr; else return &(shape.mesh);  }

protected:
	tinyobj::shape_t shape;
	XMFLOAT3 translation;
	XMFLOAT3 scale;
	XMFLOAT4 rotation;
	std::string name;
};