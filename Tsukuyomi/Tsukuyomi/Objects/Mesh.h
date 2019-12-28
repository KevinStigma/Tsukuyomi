#pragma once
#include <string>
#include <LightHelper.h>
#include "Object.h"
#include "../intersect_info.h"

class BxDF;

class Mesh :public Object
{
public:
	Mesh(std::string name, std::string file_path, XMFLOAT3 t = XMFLOAT3(0.0, 0.0, 0.0), XMFLOAT3 s = XMFLOAT3(1.0, 1.0, 1.0), XMFLOAT3 r = XMFLOAT3(0.0, 0.0, 0.0));
	~Mesh();
	void writeObj(const std::string& obj_path);
	void loadObjMesh(const std::string & obj_path);
	void constructNormals();
	bool isEmpty();
	XMFLOAT3 getTriangleVertex(int index)const;
	XMFLOAT3 getTriangleNormal(int index)const;
	void render(ID3D11DeviceContext * context, D3DRenderer* renderer);
	tinyobj::mesh_t * getMesh() { if (isEmpty()) return nullptr; else return &(shape.mesh); }
	void generateBuffers(ID3D11Device* device);
	std::string getMeshPath() { return mesh_path; }
	bool is_intersect(const Ray&ray, float& t, IntersectInfo& is_info);
	void setMaterial(Material new_mat) { mat = new_mat; }
	BxDF* getPbrMat() { return bxdf; }
	IntersectInfo sample(XMFLOAT2 u)const;
	float Area() { return area; }
	float Pdf() { return 1.0f / area; }
protected:
	Material mat;
	void computeBoundingBox();
	float computeArea();
	tinyobj::shape_t shape;
	float area;
	std::string mesh_path = "";
	BxDF* bxdf=nullptr;
	std::vector<float> accumula_areas;
	ID3D11Buffer* vertexBuffer = nullptr;
	ID3D11Buffer* indexBuffer = nullptr;
};