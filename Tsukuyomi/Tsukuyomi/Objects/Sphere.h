#pragma once
#include "Mesh.h"
class Sphere:public Mesh
{
public:
	Sphere(std::string name, std::string file_path, XMFLOAT3 t = XMFLOAT3(0.0, 0.0, 0.0), XMFLOAT3 s = XMFLOAT3(1.0, 1.0, 1.0), XMFLOAT3 r = XMFLOAT3(0.0, 0.0, 0.0));
	~Sphere();
	bool is_intersect(const Ray&ray, float& t, IntersectInfo& is_info);
	virtual IntersectInfo sample(XMFLOAT2 u)const;
	std::vector<Primitive*> getAllPrimitives();
	float Area() { float r = Radius();  return 4.0 * MathHelper::Pi * r * r; }
	float Pdf() { return 1.0f / Area(); }
	float Radius()const { return (scale.x * (boundingBox.top.x - boundingBox.bottom.x) * 0.5); }
protected:
	float origin_radius;
};
