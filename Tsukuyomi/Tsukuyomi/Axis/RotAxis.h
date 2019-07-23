#pragma once
#include "../common.h"
#include "../Objects/Object.h"
#include "../Objects/Camera.h"

class RotAxis
{
public:
	RotAxis();
	float getSmallRadius() { return small_radius; }
	float getLargetRadius() { return large_radius; }
	float getScale() { return scale; }
	XMMATRIX getAxisLocalTransform(AXIS axis_type);
	XMMATRIX computeWorldMatrix(Object * obj, AXIS axis_type);
	int rayIntersectDectect(const Ray& ray, Object* obj, float &t);
	void rotateSelObj(const Camera&cam, XMFLOAT2 cur_normalized_pos, XMFLOAT2 last_normalized_pos, AXIS curSelAxis, Object* obj);
	XMFLOAT2 getAxisDirectionProj(const Camera& cam, Object*sel_obj, AXIS axis_type);
	void setIntersectionPoint(XMFLOAT3 inter_pt) { intersection_pt = inter_pt; }
protected:
	XMFLOAT3 computeNormalSpherePoint(XMFLOAT2 normal_pos2d);
	float small_radius;
	float large_radius;
	float scale;
	XMFLOAT3 intersection_pt;
};