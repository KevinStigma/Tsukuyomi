#include "intersection.h"

bool rayCuboidIntersection(const Ray& ray)
{
	return true;
}

bool inCylinderHeight(float y, float height)
{
	if (y > height || y < 0.0)
		return false;
	return true;
}

// x^2 + z^2 = r^2
float rayCylinderIntersection(const Ray& ray, float radius, float height)
{
	std::pair<float, float> solution;
	float a = ray.direction.x * ray.direction.x + ray.direction.z * ray.direction.z;
	float b = 2.0 * (ray.direction.x * ray.origin.x + ray.direction.z * ray.origin.z);
	float c = ray.origin.x * ray.origin.x + ray.origin.z * ray.origin.z - radius * radius;
	if(!MathHelper::solveForQuadraticEquation(a, b, c, solution))
		return -1.0;
	if (solution.first < 0.0 && solution.second < 0.0)
		return -1.0;
	else if (solution.first < 0.0 && solution.second >= 0.0)
	{
		float t = solution.second;
		XMFLOAT3 intersect_pos = ray.getExtendPos(t);
		if (!inCylinderHeight(intersect_pos.y, height))
			return -1.0;
		return t;
	}
	else if (solution.first >= 0.0 && solution.second < 0.0)
	{
		float t = solution.first;
		XMFLOAT3 intersect_pos = ray.getExtendPos(t);
		if (!inCylinderHeight(intersect_pos.y, height))
			return -1.0;
		return t;
	}
	else
	{
		float t1 = solution.first;
		float t2 = solution.second;
		XMFLOAT3 intersect_pos1 = ray.getExtendPos(t1);
		XMFLOAT3 intersect_pos2 = ray.getExtendPos(t2);
		bool inCylinderHeight1 = inCylinderHeight(intersect_pos1.y, height);
		bool inCylinderHeight2 = inCylinderHeight(intersect_pos2.y, height);
		if (!inCylinderHeight1 && !inCylinderHeight2)
			return -1.0;
		else if (inCylinderHeight1 && !inCylinderHeight2)
			return t1;
		else if (!inCylinderHeight1 && inCylinderHeight2)
			return t2;
		else
			return min(t1, t2);
	}
}

float raySphereIntersection(const Ray& ray, float radius)
{	
	return -1.0;
}

// x^2 + z^2 = r^2
float rayCircleIntersection(const Ray& ray, float radius)
{
	if (ray.direction.y == 0.0f)
		return -1.0;
	float t = -ray.origin.y / (ray.direction.y);
	float x = ray.origin.x + t * ray.direction.x;
	float z = ray.origin.z + t * ray.direction.z;
	if (x*x + z * z < radius * radius)
		return t;
	else
		return -1.0;
}