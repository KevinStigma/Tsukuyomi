#pragma once
#include "Mesh.h"
class Sphere:public Mesh
{
public:
	Sphere(std::string name, std::string file_path, XMFLOAT3 t = XMFLOAT3(0.0, 0.0, 0.0), XMFLOAT3 s = XMFLOAT3(1.0, 1.0, 1.0), XMFLOAT3 r = XMFLOAT3(0.0, 0.0, 0.0), Object* al=nullptr);
	~Sphere();
	bool is_intersect(const Ray&ray, float& t, IntersectInfo& is_info);
	virtual IntersectInfo sample(XMFLOAT2 u, float& area)const;
	std::vector<Primitive*> getAllPrimitives();
	float Area() const { float r = Radius();  return 4.0 * MathHelper::Pi * r * r; }
	float Pdf() { return 1.0f / Area(); }
	float Pdf(const IntersectInfo & ref, const XMFLOAT3& wi) { return Pdf(); }
	int getComponentNum() { return 1; }
	float Radius()const { return (scale.x * (boundingBox.top.x - boundingBox.bottom.x) * 0.5); }
protected:
	float origin_radius;
};
