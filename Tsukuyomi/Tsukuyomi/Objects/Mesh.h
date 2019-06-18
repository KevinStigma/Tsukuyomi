#pragma once
#include <string>
#include "Object.h"

class Mesh :public Object
{
public:
	Mesh(std::string name, std::string file_path, XMFLOAT3 t = XMFLOAT3(0.0, 0.0, 0.0), XMFLOAT3 s = XMFLOAT3(1.0, 1.0, 1.0), XMFLOAT4 r = XMFLOAT4(0.0, 0.0, 0.0, 1.0));
	~Mesh();
	void loadObjMesh(const std::string & obj_path);
	void constructNormals();
	bool isEmpty();
	void render(ID3D11DeviceContext * context, D3DRenderer* renderer);
	tinyobj::mesh_t * getMesh() { if (isEmpty()) return nullptr; else return &(shape.mesh); }
	void generateBuffers(ID3D11Device* device);

protected:
	tinyobj::shape_t shape;
	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;
};