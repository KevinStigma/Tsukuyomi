#include "OfflineRenderUtility.h"
#include <algorithm>

float PowerHeuristic(int nf, float fPdf, int ng, float gPdf) {
	float f = nf * fPdf, g = ng * gPdf;
	return (f * f) / (f * f + g * g);
}

Spectrum EstimateDirect(const IntersectInfo& it, XMFLOAT2 uScattering, Light* light, XMFLOAT2 uLight, bool specular)
{
	Spectrum ld;
	// sample light source with multiple importance sampling
	XMFLOAT3 wi;
	float light_pdf = 0.0f, scattering_pdf = 0.0f;
	VisibilityTester vt;
	Spectrum li = light->sample_li(it, uLight, &wi, &light_pdf, vt);
	if (light_pdf > 0.0f && !li.isBlack())
	{
		Spectrum f;
		if (it.isSurfaceInteraction())
		{
			//TODO: the wi and wo may be transformed to local coordinates
			f = it.bxdf->f(it.wo, wi) * MathHelper::DotFloat3(wi, it.normal);
			scattering_pdf = it.bxdf->Pdf(it.wo, wi);
		}
		if (!f.isBlack())
		{
			if (!vt.unoccluded())
				li = Spectrum();
			if (!li.isBlack())
			{
				if (light->isDelta())
					ld += f * li / light_pdf;
				else
				{
					float weight = PowerHeuristic(1.0, scattering_pdf, 1.0, light_pdf);
					ld += f * li * weight / light_pdf;
				}
			}
		}
	}

	// sample BSDF with multiple imortance sampling
	if (!light->isDelta())
	{
		Spectrum f;
		bool sampledSpecular = false;
		BxDFType sampled_type;
		if (it.isSurfaceInteraction())
		{
			f = it.bxdf->sample_f(it.wo, &wi, uScattering, &scattering_pdf, &sampled_type);
			f = f * MathHelper::DotFloat3(wi, it.normal);
			sampledSpecular = sampled_type & BSDF_SPECULAR;
		}

		if (!f.isBlack() && scattering_pdf > 0.0f)
		{
			float weight = 1.0;
			if (!sampledSpecular)
			{
				light_pdf = 1.0;
				if (light_pdf == 0.0)
					return ld;
				weight = PowerHeuristic(1.0, scattering_pdf, 1.0, light_pdf);
			}
			IntersectInfo light_it;
			Ray ray = it.spawnRay(wi);
			g_pGlobalSys->cast_ray_to_get_intersection(ray, light_it);
			Spectrum li;
			if (light_it.isSurfaceInteraction())
			{
				if (light_it.obj->getType() == AREA_LIGHT)
				{

				}
			}
			else
				li = light->Le(ray);
			if (!li.isBlack())
				ld = ld + f * li * weight / scattering_pdf;
		}
	}
	return ld;
}

XMFLOAT3 transVectorToLocalFromWorld(XMFLOAT3 n, XMFLOAT3 v)
{
	XMVECTOR Nt, Nb, N;
	N = XMVectorSet(n.x, n.y, n.z, 0.0);
	if (std::fabs(n.x) > std::fabs(n.y))
		Nt = XMVector3Normalize(XMVectorSet(n.z, 0, -n.x, 0.0));
	else
		Nt = XMVector3Normalize(XMVectorSet(0.0, -n.z, n.x, 0.0));
	Nb = XMVector3Cross(N, Nt);
	XMMATRIX mat;
	mat.r[0] = Nt;
	mat.r[1] = Nb;
	mat.r[2] = N;
	mat.r[3] = XMVectorSet(0.0, 0.0, 0.0, 1.f);
	mat = XMMatrixTranspose(mat);
	XMVECTOR local_V = XMVector3TransformNormal(XMVector3Normalize(XMVectorSet(v.x, v.y, v.z, 0.0)), mat);
	XMFLOAT3 local_v;
	XMStoreFloat3(&local_v, local_V);
	return local_v;
}

XMFLOAT3 transVectorToWorldFromLocal(XMFLOAT3 n, XMFLOAT3 v)
{
	XMVECTOR Nt, Nb, N;
	N = XMVectorSet(n.x, n.y, n.z, 0.0);
	if (std::fabs(n.x) > std::fabs(n.y))
		Nt = XMVector3Normalize(XMVectorSet(n.z, 0, -n.x, 0.0));
	else
		Nt = XMVector3Normalize(XMVectorSet(0.0, -n.z, n.x, 0.0));
	Nb = XMVector3Cross(N, Nt);
	XMMATRIX mat;
	mat.r[0] = Nt;
	mat.r[1] = Nb;
	mat.r[2] = N;
	mat.r[3] = XMVectorSet(0.0, 0.0, 0.0, 1.f);
	XMVECTOR world_V = XMVector3TransformNormal(XMVector3Normalize(XMVectorSet(v.x, v.y, v.z, 0.0)), mat);
	XMFLOAT3 world_v;
	XMStoreFloat3(&world_v, world_V);
	return world_v;
}

Spectrum UniformSampleOneLight(const IntersectInfo& it)
{
	std::vector<Light*> lights = g_pGlobalSys->objectManager.getAllLights();
	Light* sel_light = lights[rand() % (lights.size())];
	XMFLOAT2 uScattering(generateRandomFloat(), generateRandomFloat());
	XMFLOAT2 uLight(generateRandomFloat(), generateRandomFloat());
	return (float)lights.size() * EstimateDirect(it, uScattering, sel_light, uLight);
}