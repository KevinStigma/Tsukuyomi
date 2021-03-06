#pragma once
#include "Mesh.h"
class Sphere:public Mesh
{
public:
	Sphere(std::string name, std::string file_path, XMFLOAT3 t = XMFLOAT3(0.0, 0.0, 0.0), XMFLOAT3 s = XMFLOAT3(1.0, 1.0, 1.0), XMFLOAT3 r = XMFLOAT3(0.0, 0.0, 0.0), Object* al=nullptr, RenderMats* render_mats=nullptr);
	~Sphere();
	void outputSphereMesh();
	bool is_intersect(const Ray&ray, float& t, IntersectInfo& is_info);
	XMFLOAT3 getWorldCenter()const;
	virtual IntersectInfo sample(const IntersectInfo&ref, XMFLOAT2 u, float*pdf)const;
	std::vector<Primitive*> getAllPrimitives();
	float Area() const { float r = Radius();  return 4.0 * MathHelper::Pi * r * r; }
	float Pdf() { return 1.0f / Area(); }
	float Pdf(const IntersectInfo & ref, const XMFLOAT3& wi);
	int getComponentNum() { return 1; }
	float Radius()const;
protected:
	float origin_radius;
};
