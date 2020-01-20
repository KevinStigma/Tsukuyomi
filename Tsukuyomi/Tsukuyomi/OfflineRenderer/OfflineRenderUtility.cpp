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
			std::vector<XMFLOAT3> local_vs = transVectorsToLocalFromWorld(it.normal, std::vector<XMFLOAT3>({ it.wo, wi }));
			XMFLOAT3 local_wo = local_vs[0];
			XMFLOAT3 local_wi = local_vs[1];
			f = it.bxdf->f(local_wo, local_wi) * MathHelper::DotFloat3(wi, it.normal);
			scattering_pdf = it.bxdf->Pdf(local_wo, local_wi);
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

	// sample BSDF with multiple importance sampling
	if (!light->isDelta())
	{
		Spectrum f;
		bool sampledSpecular = false;
		BxDFType sampled_type;
		if (it.isSurfaceInteraction())
		{ 
			f = it.bxdf->sample_f(transVectorToLocalFromWorld(it.normal, it.wo), &wi, uScattering, &scattering_pdf, &sampled_type);
			wi = transVectorToWorldFromLocal(it.normal, wi);
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
					li = light_it.Le(MathHelper::NegativeFloat3(wi));
			}
			else
				li = light->Le(ray);
			if (!li.isBlack())
				ld = ld + f * li * weight / scattering_pdf;
		}
	}
	return ld;
}

XMMATRIX computeMatrixToLocalFromWrold(XMFLOAT3 n)
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
	return XMMatrixTranspose(mat);
}

XMMATRIX computeMatrixToWorldFromLocal(XMFLOAT3 n)
{
	XMVECTOR Nt, Nb, N;
	N = XMVectorSet(n.x, n.y, n.z, 0.0);
	if (std::fabs(n.x) > std::fabs(n.y))
		Nt = XMVector3Normalize(XMVectorSet(n.z, 0, -n.x, 0.0));
	else
		Nt = XMVector3Normalize(XMVectorSet(0.0, -n.z, n.y, 0.0));
	Nb = XMVector3Cross(N, Nt);
	XMMATRIX mat;
	mat.r[0] = Nt;
	mat.r[1] = Nb;
	mat.r[2] = N;
	mat.r[3] = XMVectorSet(0.0, 0.0, 0.0, 1.f);
	return mat;
}

XMFLOAT3 transVectorToLocalFromWorld(XMFLOAT3 n, XMFLOAT3 v)
{
	XMMATRIX mat = computeMatrixToLocalFromWrold(n);
	XMVECTOR local_V = XMVector3TransformNormal(XMVector3Normalize(XMVectorSet(v.x, v.y, v.z, 0.0)), mat);
	XMFLOAT3 local_v;
	XMStoreFloat3(&local_v, local_V);
	return local_v;
}

XMFLOAT3 transVectorToWorldFromLocal(XMFLOAT3 n, XMFLOAT3 v)
{
	XMMATRIX mat = computeMatrixToWorldFromLocal(n);
	XMVECTOR world_V = XMVector3TransformNormal(XMVector3Normalize(XMVectorSet(v.x, v.y, v.z, 0.0)), mat);
	XMFLOAT3 world_v;
	XMStoreFloat3(&world_v, world_V);
	return world_v;
}

std::vector<XMFLOAT3> transVectorsToLocalFromWorld(XMFLOAT3 n, std::vector<XMFLOAT3> vs)
{
	XMMATRIX mat = computeMatrixToLocalFromWrold(n);
	std::vector<XMFLOAT3> local_vs;
	for (int i = 0; i < vs.size(); i++)
	{
		XMFLOAT3 v = vs[i];
		XMVECTOR local_V = XMVector3TransformNormal(XMVector3Normalize(XMVectorSet(v.x, v.y, v.z, 0.0)), mat);
		XMFLOAT3 local_v;
		XMStoreFloat3(&local_v, local_V);
		local_vs.push_back(local_v);
	}
	return local_vs;
}

std::vector<XMFLOAT3> transVectorsToWorldFromLocal(XMFLOAT3 n, std::vector<XMFLOAT3> vs)
{
	XMMATRIX mat = computeMatrixToWorldFromLocal(n);
	std::vector<XMFLOAT3> world_vs;
	for (int i = 0; i < vs.size(); i++)
	{
		XMFLOAT3 v = vs[i];
		XMVECTOR world_V = XMVector3TransformNormal(XMVector3Normalize(XMVectorSet(v.x, v.y, v.z, 0.0)), mat);
		XMFLOAT3 world_v;
		XMStoreFloat3(&world_v, world_V);
		world_vs.push_back(world_v);
	}
	return world_vs;
}

Spectrum UniformSampleOneLight(const IntersectInfo& it)
{
	std::vector<Light*> lights = g_pGlobalSys->objectManager.getAllLights();
	Light* sel_light = lights[rand() % (lights.size())];
	XMFLOAT2 uScattering(generateRandomFloat(), generateRandomFloat());
	XMFLOAT2 uLight(generateRandomFloat(), generateRandomFloat());
	return (float)lights.size() * EstimateDirect(it, uScattering, sel_light, uLight);
}

Spectrum UniformSampleAllLights(const IntersectInfo& it)
{
	std::vector<Light*> lights = g_pGlobalSys->objectManager.getAllLights();
	Spectrum l;
	for (int i = 0; i < lights.size(); i++)
	{
		XMFLOAT2 uScattering(generateRandomFloat(), generateRandomFloat());
		XMFLOAT2 uLight(generateRandomFloat(), generateRandomFloat());
		l += EstimateDirect(it, uScattering, lights[i], uLight);
	}
	return l;
}