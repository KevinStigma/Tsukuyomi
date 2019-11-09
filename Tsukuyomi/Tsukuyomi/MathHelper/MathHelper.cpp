//***************************************************************************************
// MathHelper.cpp by Frank Luna (C) 2011 All Rights Reserved.
//***************************************************************************************

#include "MathHelper.h"
#include <float.h>
#include <cmath>
const float MathHelper::Infinity = FLT_MAX;
const float MathHelper::Pi       = 3.1415926535f;
const float MathHelper::InvPi = 1 / 3.1415926535f;

float MathHelper::AngleFromXY(float x, float y)
{
	float theta = 0.0f;
 
	// Quadrant I or IV
	if(x >= 0.0f) 
	{
		// If x = 0, then atanf(y/x) = +pi/2 if y > 0
		//                atanf(y/x) = -pi/2 if y < 0
		theta = atanf(y / x); // in [-pi/2, +pi/2]

		if(theta < 0.0f)
			theta += 2.0f*Pi; // in [0, 2*pi).
	}

	// Quadrant II or III
	else      
		theta = atanf(y/x) + Pi; // in [0, 2*pi).

	return theta;
}

XMVECTOR MathHelper::RandUnitVec3()
{
	XMVECTOR One  = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	XMVECTOR Zero = XMVectorZero();

	// Keep trying until we get a point on/in the hemisphere.
	while(true)
	{
		// Generate random point in the cube [-1,1]^3.
		XMVECTOR v = XMVectorSet(MathHelper::RandF(-1.0f, 1.0f), MathHelper::RandF(-1.0f, 1.0f), MathHelper::RandF(-1.0f, 1.0f), 0.0f);

		// Ignore points outside the unit sphere in order to get an even distribution 
		// over the unit sphere.  Otherwise points will clump more on the sphere near 
		// the corners of the cube.

		if( XMVector3Greater( XMVector3LengthSq(v), One) )
			continue;

		return XMVector3Normalize(v);
	}
}

XMVECTOR MathHelper::RandHemisphereUnitVec3(XMVECTOR n)
{
	XMVECTOR One  = XMVectorSet(1.0f, 1.0f, 1.0f, 1.0f);
	XMVECTOR Zero = XMVectorZero();

	// Keep trying until we get a point on/in the hemisphere.
	while(true)
	{
		// Generate random point in the cube [-1,1]^3.
		XMVECTOR v = XMVectorSet(MathHelper::RandF(-1.0f, 1.0f), MathHelper::RandF(-1.0f, 1.0f), MathHelper::RandF(-1.0f, 1.0f), 0.0f);

		// Ignore points outside the unit sphere in order to get an even distribution 
		// over the unit sphere.  Otherwise points will clump more on the sphere near 
		// the corners of the cube.
		
		if( XMVector3Greater( XMVector3LengthSq(v), One) )
			continue;

		// Ignore points in the bottom hemisphere.
		if( XMVector3Less( XMVector3Dot(n, v), Zero ) )
			continue;

		return XMVector3Normalize(v);
	}
}

Ray Ray::transform(XMMATRIX trans_mat) const
{
	XMVECTOR v = XMLoadFloat3(&origin);
	XMFLOAT3 new_origin;
	XMStoreFloat3(&new_origin, XMVector3TransformCoord(v, trans_mat));

	v = XMLoadFloat3(&direction);
	XMFLOAT3 new_dir;
	XMStoreFloat3(&new_dir, XMVector3TransformNormal(v, trans_mat));
	return Ray(new_origin, new_dir);
}

// a , b,  c, we want to get intersect point o
// o = e + td
// o = a + (b-a) * beta + (c-a) * gama
// https://www.cnblogs.com/wickedpriest/p/9193314.html
bool Ray::is_intersect_triangle(XMFLOAT3 a, XMFLOAT3 b, XMFLOAT3 c, float& t, float &beta, float& gama)const
{
	XMMATRIX A(XMVectorSet(a.x - b.x, a.x - c.x, direction.x, 0.0f), 
		XMVectorSet(a.y - b.y, a.y - c.y, direction.y, 0.0f),
		XMVectorSet(a.z - b.z, a.z - c.z, direction.z, 0.0f),
		XMVectorSet(0.0f,0.0f,0.0f, 1.0f));
	float det = XMVectorGetX(XMMatrixDeterminant(A));
	if (abs(det) < 1e-6)
		return false;
	XMMATRIX B(XMVectorSet(a.x - origin.x, a.x - c.x, direction.x, 0.0f),
		XMVectorSet(a.y - origin.y, a.y - c.y, direction.y, 0.0f),
		XMVectorSet(a.z - origin.z, a.z - c.z, direction.z, 0.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f));

	XMMATRIX C(XMVectorSet(a.x - b.x, a.x - origin.x, direction.x, 0.0f),
		XMVectorSet(a.y - b.y, a.y - origin.y, direction.y, 0.0f),
		XMVectorSet(a.z - b.z, a.z - origin.z, direction.z, 0.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f));
	beta = XMVectorGetX(XMMatrixDeterminant(B)) / det;
	gama = XMVectorGetX(XMMatrixDeterminant(C)) / det;
	if (beta <0.0||beta>1.0||gama>1.0 || gama<0.0 || beta + gama > 1.0f||beta + gama < 0.0f)
		return false;
	XMMATRIX D(XMVectorSet(a.x - b.x, a.x - c.x, a.x - origin.x, 0.0f),
		XMVectorSet(a.y - b.y, a.y - c.y, a.y - origin.y, 0.0f),
		XMVectorSet(a.z - b.z, a.z - c.z, a.z - origin.z, 0.0f),
		XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f));
	t = XMVectorGetX(XMMatrixDeterminant(D)) / det;
	return true;
}