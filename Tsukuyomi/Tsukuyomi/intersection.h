#pragma once
#include "MathHelper/MathHelper.h"

extern bool rayCuboidIntersection(const Ray& ray);

extern float rayCylinderIntersection(const Ray& ray, float radius, float height);

extern bool raySphereIntersection(const Ray& ray);