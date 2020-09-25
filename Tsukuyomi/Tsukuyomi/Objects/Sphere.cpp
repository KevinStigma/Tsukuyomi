#include "Sphere.h"
#include "../Accelerate/PrimSphere.h"
#include <algorithm>

XMFLOAT3 UniformSampleSphere(const XMFLOAT2 &u) {
	float z = 1 - 2 * u.x;
	float r = sqrtf(std::max<float>(0.0f, 1.0f - z * z));
	float phi = 2 * MathHelper::Pi * u.y;
	return XMFLOAT3(r * std::cos(phi), r * std::sin(phi), z);
}

Sphere::Sphere(std::string name, std::string file_path, XMFLOAT3 t, XMFLOAT3 s, XMFLOAT3 r, Object* al, RenderMats* rendermats):Mesh(name,file_path,t,s,r, al, rendermats)
{
	origin_radius = (boundingBox.top.x - boundingBox.bottom.x) * 0.5;
}

Sphere::~Sphere()
{
	Mesh::~Mesh();
}

void CoordinateSystem(const XMFLOAT3 &v1, XMFLOAT3 *v2,
	XMFLOAT3 *v3) {
	if (std::abs(v1.x) > std::abs(v1.y))
	{
		float v = 1.0f / std::sqrtf(v1.x * v1.x + v1.z * v1.z);
		*v2 = XMFLOAT3(-v1.z*v, 0, v1.x*v);
	}
	else
	{
		float v = 1.0f / std::sqrt(v1.y * v1.y + v1.z * v1.z);
		*v2 = XMFLOAT3(0, v1.z * v, -v1.y * v);
	}
	*v3 = MathHelper::Cross(v1, *v2);
}

IntersectInfo Sphere::sample(const IntersectInfo&ref, XMFLOAT2 u, float*pdf)const
{
	XMFLOAT3 center = getWorldCenter();

	// Sample uniformly on sphere if $\pt{}$ is inside it
	float radius = Radius();
	float v = MathHelper::DistanceSquared(ref.pos, center);
	if (v <= radius * radius) {
		XMFLOAT3 uni_sample_coord = UniformSampleSphere(u);
		float r = Radius();
		XMVECTOR offset = XMVectorSet(r * uni_sample_coord.x, r * uni_sample_coord.y, r * uni_sample_coord.z, 0.0f);
		IntersectInfo it;
		XMVECTOR world_normal = XMVector3Normalize(offset);
		XMStoreFloat3(&it.normal, world_normal);
		float ratio = r / XMVectorGetX(XMVector3Length(offset));
		offset = XMVectorMultiply(offset, XMVectorSet(ratio, ratio, ratio, ratio));
		XMFLOAT3 center = boundingBox.getCenter();
		XMVECTOR world_pos = XMVectorAdd(XMVectorSet(center.x, center.y, center.z, 1.0), offset);
		XMStoreFloat3(&it.pos, world_pos);
		XMFLOAT3 wi(it.pos.x - ref.pos.x, it.pos.y - ref.pos.y , it.pos.z - ref.pos.z);
		if (MathHelper::Float3Length(wi) == 0.0)
			*pdf = 0;
		else {
			// Convert from area measure returned by Sample() call above to
			// solid angle measure.
			*pdf = 1.0 / Area();
			wi = MathHelper::NormalizeFloat3(wi);
			*pdf *= MathHelper::DistanceSquared(ref.pos, it.pos) / MathHelper::DotFloat3(it.normal, MathHelper::NegativeFloat3(wi));
		}
		if (std::isinf(*pdf)) *pdf = 0.f;
		return it;
	}

	// Sample sphere uniformly inside subtended cone

	// Compute coordinate system for sphere sampling
	float dc = MathHelper::Distance(ref.pos, center);
	float invDc = 1 / dc;
	XMFLOAT3 wc = XMFLOAT3((center.x - ref.pos.x)*invDc, (center.y - ref.pos.y)*invDc, (center.z - ref.pos.z)*invDc);
	XMFLOAT3 wcX, wcY;
	CoordinateSystem(wc, &wcX, &wcY);

	// Compute $\theta$ and $\phi$ values for sample in cone
	float sinThetaMax = radius * invDc;
	float sinThetaMax2 = sinThetaMax * sinThetaMax;
	float invSinThetaMax = 1 / sinThetaMax;
	float cosThetaMax = std::sqrtf(std::max<float>(0.f, 1 - sinThetaMax2));

	float cosTheta = (cosThetaMax - 1) * u.x + 1.0f;
	float sinTheta2 = 1 - cosTheta * cosTheta;

	if (sinThetaMax2 < 0.00068523f /* sin^2(1.5 deg) */) {
		/* Fall back to a Taylor series expansion for small angles, where
		   the standard approach suffers from severe cancellation errors */
		sinTheta2 = sinThetaMax2 * u.x;
		cosTheta = std::sqrtf(1 - sinTheta2);
	}

	// Compute angle $\alpha$ from center of sphere to sampled point on surface
	float cosAlpha = sinTheta2 * invSinThetaMax +
		cosTheta * std::sqrtf(std::max(0.f, 1.f - sinTheta2 * invSinThetaMax * invSinThetaMax));
	float sinAlpha = std::sqrtf(std::max(0.f, 1.f - cosAlpha * cosAlpha));
	float phi = u.y * 2.f * MathHelper::Pi;

	// Compute surface normal and sampled point on sphere
	XMFLOAT3 nWorld = MathHelper::SphericalDirection(sinAlpha, cosAlpha, phi,
		MathHelper::NegativeFloat3(wcX), 
		MathHelper::NegativeFloat3 (wcY),
		MathHelper::NegativeFloat3(wc));
	XMFLOAT3 pWorld = XMFLOAT3(center.x + nWorld.x * radius,center.y + nWorld.y * radius,center.z + nWorld.z * radius);

	// Return _Interaction_ for sampled point on sphere
	IntersectInfo it;
	it.pos = pWorld;
	it.normal = MathHelper::NormalizeFloat3(nWorld);

	// Uniform cone PDF.
	*pdf = 1 / (2 * MathHelper::Pi * (1 - cosThetaMax));

	return it;
}

bool Sphere::is_intersect(const Ray&ray, float& t, IntersectInfo& is_info)
{
	XMFLOAT3 center = getWorldCenter();
	if (ray.is_intersect_sphere(center, Radius(), t))
	{
		float phiMax = MathHelper::Pi * 2.0;
		is_info.wo = XMFLOAT3(-ray.direction.x, -ray.direction.y, -ray.direction.z);
		is_info.obj = this;
		is_info.pos = XMFLOAT3(ray.origin.x + t * ray.direction.x, ray.origin.y + t * ray.direction.y, ray.origin.z + t * ray.direction.z);
		is_info.normal = MathHelper::NormalizeFloat3(XMFLOAT3(is_info.pos.x - center.x, is_info.pos.y - center.y, is_info.pos.z - center.z));
		is_info.dpdu = XMFLOAT3(-phiMax * is_info.pos.y, phiMax * is_info.pos.x, 0.0);
		return true;
	}
	else
		return false;
}

std::vector<Primitive*> Sphere::getAllPrimitives()
{
	std::vector<Primitive*> prims;
	PrimSphere* s = new PrimSphere(this, origin_radius, XMFLOAT3(0.0f, 0.0f, 0.0f));
	prims.push_back(s);
	return prims;
}

float UniformConePdf(float cosThetaMax) {
	return 1.0f / (2 * MathHelper::Pi * (1.0f - cosThetaMax));
}

float Sphere::Pdf(const IntersectInfo & ref, const XMFLOAT3& wi)
{
	XMFLOAT3 pCenter = getWorldCenter();;
	// Return uniform PDF if point is inside sphere
	float radius = Radius();
	float distance_val = MathHelper::DistanceSquared(ref.pos, pCenter);
	if (distance_val <= radius * radius)
		return Pdf();

	// Compute general sphere PDF
	float sinThetaMax2 = radius * radius / distance_val;
	float cosThetaMax = std::sqrt(std::max(0.0f, 1 - sinThetaMax2));
	return UniformConePdf(cosThetaMax);
}

XMFLOAT3 Sphere::getWorldCenter()const
{
	XMFLOAT3 center;
	XMStoreFloat3(&center, XMVector3TransformCoord(XMVectorSet(0, 0, 0, 1), getGlobalWorldMatrix()));
	return center;
}

float Sphere::Radius()const
{
	XMMATRIX world_mat = getGlobalWorldMatrix();
	return origin_radius * XMVectorGetX(XMVector3Length(XMVector3TransformNormal(XMVectorSet(1, 0, 0, 0), world_mat)));
}

void Sphere::outputSphereMesh()
{
	std::ofstream out("sphere.obj");
	auto mesh = getMesh();
	float r = origin_radius;
	for (int i = 0; i < mesh->positions.size() / 3; i++)
	{
		float x = mesh->positions[i * 3];
		float y = mesh->positions[i * 3+1];
		float z = mesh->positions[i * 3+2];
		out << "v " << x / origin_radius << " " << y/origin_radius << " " << z/origin_radius << std::endl;
		x = mesh->normals[i * 3];
		y = mesh->normals[i * 3 + 1];
		z = mesh->normals[i * 3 + 2];
		out << "vn " << x << " " << y << " " << z << std::endl;
	}
	for (int i = 0; i < mesh->indices.size() / 3; i++)
	{
		int a = mesh->indices[i * 3]+1;
		int b = mesh->indices[i * 3+1]+1;
		int c = mesh->indices[i * 3+2]+1;
		out << "f " << a << "//" << a << " " << b << "//" << b << " " << c << "//" << c << std::endl;
	}
	out.close();
}

