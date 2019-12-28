#pragma once
#include "Mesh.h"
class Sphere:public Mesh
{
public:
	Sphere(std::string name, std::string file_path, XMFLOAT3 t = XMFLOAT3(0.0, 0.0, 0.0), XMFLOAT3 s = XMFLOAT3(1.0, 1.0, 1.0), XMFLOAT3 r = XMFLOAT3(0.0, 0.0, 0.0));
	~Sphere();
	bool is_intersect(const Ray&ray, float& t, IntersectInfo& is_info);
protected:
	float radius;
};
