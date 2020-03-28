//***************************************************************************************
// MathHelper.h by Frank Luna (C) 2011 All Rights Reserved.
//
// Helper math class.
//***************************************************************************************

#ifndef MATHHELPER_H
#define MATHHELPER_H

#include <utility>
#include <iostream>
#include <algorithm>
#include <Windows.h>
#include <DirectXMath.h>

using namespace DirectX;

class MathHelper
{
public:
	// Returns random float in [0, 1).
	static float RandF()
	{
		return (float)(rand()) / (float)RAND_MAX;
	}

	// Returns random float in [a, b).
	static float RandF(float a, float b)
	{
		return a + RandF()*(b-a);
	}

	template<typename T>
	static T Min(const T& a, const T& b)
	{
		return a < b ? a : b;
	}

	template<typename T>
	static T Max(const T& a, const T& b)
	{
		return a > b ? a : b;
	}
	 
	template<typename T>
	static T Lerp(const T& a, const T& b, float t)
	{
		return a + (b-a)*t;
	}

	template<typename T>
	static T Clamp(const T& x, const T& low, const T& high)
	{
		return x < low ? low : (x > high ? high : x); 
	}

	static XMFLOAT3 NormalizeFloat3(XMFLOAT3 n)
	{
		float l = sqrtf(n.x * n.x + n.y * n.y + n.z * n.z);
		return XMFLOAT3(n.x/l, n.y/l, n.z/l);
	}

	// Returns the polar angle of the point (x,y) in [0, 2*PI).
	static float AngleFromXY(float x, float y);

	static XMMATRIX InverseTranspose(CXMMATRIX M)
	{
		// Inverse-transpose is just applied to normals.  So zero out 
		// translation row so that it doesn't get into our inverse-transpose
		// calculation--we don't want the inverse-transpose of the translation.
		XMMATRIX A = M;
		A.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

		XMVECTOR det = XMMatrixDeterminant(A);
		return XMMatrixTranspose(XMMatrixInverse(&det, A));
	}

	static XMVECTOR RandUnitVec3();
	static XMVECTOR RandHemisphereUnitVec3(XMVECTOR n);

	static const float Infinity;
	static const float Pi;
	static const float InvPi;

	static bool solveForQuadraticEquation(float a, float b, float c, std::pair<float, float>& solution)
	{
		float val = b * b - 4.0*a*c;
		if (val < 0.0)
			return false;
		solution.first = (-b + sqrtf(val)) * 0.5f / a;
		solution.second = (-b - sqrtf(val)) * 0.5f / a;
		return true;
	}

	static float projectVector2D(XMFLOAT2 vector, XMFLOAT2 ref_vector)
	{
		float len = sqrtf(ref_vector.x * ref_vector.x + ref_vector.y * ref_vector.y);
		if (len < 1e-4)
			return 0.0f;
		return (vector.x * ref_vector.x + vector.y * ref_vector.y) / len;
	}

	static XMFLOAT3 transRotationMatrixToEulerAngles(const XMFLOAT4X4& rot_mat)
	{
		float pitch = asinf(-rot_mat(2, 1));
		float roll = 0.0f, yaw = 0.0f;
		if (abs(abs(pitch) - Pi * 0.5f) < 1e-6)
		{
			roll = 0.0f;
			yaw = atan2(-rot_mat(0, 2), rot_mat(0, 0));
		}
		else
		{
			roll = atan2(rot_mat(0, 1), rot_mat(1, 1));
			yaw = atan2(rot_mat(2, 0), rot_mat(2, 2));
		}
		return XMFLOAT3(pitch, yaw, roll);
	}

	static void CoordinateSystem(const XMFLOAT3 &v1, XMFLOAT3 *v2, XMFLOAT3 *v3) {
		if (std::abs(v1.x) > std::abs(v1.y))
			*v2 = MultFloat3(XMFLOAT3(-v1.z, 0, v1.x), 1.0 / std::sqrt(v1.x * v1.x + v1.z * v1.z));
		else
			*v2 = MultFloat3(XMFLOAT3(0, v1.z, -v1.y),  1.0 / std::sqrt(v1.y * v1.y + v1.z * v1.z));
		*v3 = Cross(v1, *v2);
	}

	static XMFLOAT2 ConcentricSampleDisk(const XMFLOAT2 &u) {
		// Map uniform random numbers to $[-1,1]^2$
		XMFLOAT2 uOffset(2.f * u.x - 1.0f, 2.0f * u.y - 1.0f);

		// Handle degeneracy at the origin
		if (uOffset.x == 0 && uOffset.y == 0) return XMFLOAT2(0, 0);

		float piOver4 = Pi * 0.25f;
		float piOver2 = Pi * 0.5f;
		// Apply concentric mapping to point
		float theta, r;
		if (std::abs(uOffset.x) > std::abs(uOffset.y)) {
			r = uOffset.x;
			theta = piOver4 * (uOffset.y / uOffset.x);
		}
		else {
			r = uOffset.y;
			theta = piOver2 - piOver4 * (uOffset.x / uOffset.y);
		}
		return XMFLOAT2(std::cos(theta) * r, std::sin(theta) * r);
	}

	static XMFLOAT3 CosineSampleHemisphere(const XMFLOAT2 &u) {
		XMFLOAT2 d = ConcentricSampleDisk(u);
		float z = std::sqrt(std::max<float>((float)0, 1 - d.x * d.x - d.y * d.y));
		return XMFLOAT3(d.x, d.y, z);
	}

	static float Degree2Radian(float degree)
	{
		return degree / 180.0f * Pi;
	}

	static float Radian2Degree(float radian)
	{
		return radian * InvPi * 180.0f;
	}

	static float DotFloat3(const XMFLOAT3& a, const XMFLOAT3& b)
	{
		return a.x*b.x + a.y*b.y + a.z*b.z;
	}

	static XMFLOAT3 Cross(const XMFLOAT3 &v1, const XMFLOAT3 &v2) {
		double v1x = v1.x, v1y = v1.y, v1z = v1.z;
		double v2x = v2.x, v2y = v2.y, v2z = v2.z;
		return XMFLOAT3((v1y * v2z) - (v1z * v2y), (v1z * v2x) - (v1x * v2z),
			(v1x * v2y) - (v1y * v2x));
	}

	static XMFLOAT3 SphericalDirection(float sinTheta, float cosTheta, float phi,
		const XMFLOAT3 &x, const XMFLOAT3 &y,
		const XMFLOAT3 &z) {
		float v1 = sinTheta * std::cos(phi);
		float v2 = sinTheta * std::sin(phi);
		return XMFLOAT3(v1*x.x + v2*y.x + cosTheta*z.x,
		v1*x.y + v2 * y.y + cosTheta * z.y,
		v1*x.z + v2 * y.z + cosTheta * z.z);
	}
	static XMFLOAT3 SphericalDirection(float sinTheta, float cosTheta, float phi) {
		return XMFLOAT3(sinTheta * std::cos(phi), sinTheta * std::sin(phi), cosTheta);
	}

	static XMFLOAT3 NegativeFloat3(const XMFLOAT3& a)
	{
		return XMFLOAT3(-a.x,-a.y,-a.z);
	}

	static XMFLOAT3 AddFloat3(const XMFLOAT3& a, const XMFLOAT3& b)
	{
		return XMFLOAT3(b.x + a.x, b.y + a.y, b.z + a.z);
	}

	static XMFLOAT3 MinusFloat3(const XMFLOAT3& a, const XMFLOAT3& b)
	{
		return XMFLOAT3(-b.x + a.x, -b.y + a.y, -b.z + a.z);
	}

	static XMFLOAT3 MultFloat3(const XMFLOAT3& a, float n)
	{
		return XMFLOAT3(n * a.x, n* a.y, a.z * n);
	}

	static float DistanceSquared(const XMFLOAT3& a, const XMFLOAT3& b)
	{
		XMFLOAT3 diff(a.x - b.x, a.y - b.y, a.z - b.z);
		return DotFloat3(diff, diff);
	}

	static float Distance(const XMFLOAT3& a, const XMFLOAT3& b)
	{
		return sqrtf(DistanceSquared(a,b));
	}

	static float TriangleFilterEval(float x_diff, float y_diff, float radius)
	{
		return std::max<float>(0.0, radius - fabs(x_diff)) * std::max<float>(0.0, radius - fabs(y_diff));
	}

	static float Float3Length(const XMFLOAT3 & l)
	{
		return sqrtf(DotFloat3(l,l));
	}

	static XMFLOAT3 Reflect(const XMFLOAT3 &wo, const XMFLOAT3 &n) {
		float dot = DotFloat3(wo, n);
		return XMFLOAT3(-wo.x + 2 * dot * n.x, -wo.y + 2 * dot * n.y, -wo.z + 2 * dot * n.z);
	}

	static float TriangleArea(const XMFLOAT3 & v1, const XMFLOAT3& v2, const XMFLOAT3& v3)
	{
		double a = Float3Length(XMFLOAT3(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z));
		double b = Float3Length(XMFLOAT3(v1.x - v3.x, v1.y - v3.y, v1.z - v3.z));
		double c = Float3Length(XMFLOAT3(v3.x - v2.x, v3.y - v2.y, v3.z - v2.z));
		double pp = (a + b + c) / 2.0;
		double s1 = sqrt(fabs(pp));
		double s2 = sqrt(fabs(pp - a));
		double s3 = sqrt(fabs(pp - b));
		double s4 = sqrt(fabs(pp - c));
		double s = s1 * s2 * s3 * s4;
		return s;
	}

};

struct Ray
{
	XMFLOAT3 origin;
	XMFLOAT3 direction;
	Ray(XMFLOAT3 o=XMFLOAT3(0.0, 0.0, 0.0), XMFLOAT3 d=XMFLOAT3(0.0, 0.0, 0.0)):origin(o), direction(d){}
	Ray(const Ray& r) :origin(r.origin), direction(r.direction){}
	bool dirIsNeg(int dim)const;
	XMFLOAT3 getExtendPos(float t)const { return XMFLOAT3(origin.x + t * direction.x, origin.y + t * direction.y, origin.z + t * direction.z); }
	Ray transform(XMMATRIX trans_mat)const;
	bool is_intersect_triangle(XMFLOAT3 a, XMFLOAT3 b, XMFLOAT3 c, float& t, float &beta, float& gama)const;
	bool is_intersect_sphere(XMFLOAT3 c, float r, float& t)const;
};

#endif // MATHHELPER_H