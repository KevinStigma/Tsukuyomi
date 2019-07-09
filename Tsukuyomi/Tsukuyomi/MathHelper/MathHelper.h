//***************************************************************************************
// MathHelper.h by Frank Luna (C) 2011 All Rights Reserved.
//
// Helper math class.
//***************************************************************************************

#ifndef MATHHELPER_H
#define MATHHELPER_H

#include <utility>
#include <iostream>
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
};

struct Ray
{
	XMFLOAT3 origin;
	XMFLOAT3 direction;
	Ray(XMFLOAT3 o=XMFLOAT3(0.0, 0.0, 0.0), XMFLOAT3 d=XMFLOAT3(0.0, 0.0, 0.0)):origin(o), direction(d){}
	Ray(Ray& r) :origin(r.origin), direction(r.direction){}
	XMFLOAT3 getExtendPos(float t)const { return XMFLOAT3(origin.x + t * direction.x, origin.y + t * direction.y, origin.z + t * direction.z); }
	Ray transform(XMMATRIX trans_mat)const;
};

#endif // MATHHELPER_H