#include "OfflineRenderUtility.h"

float PowerHeuristic(int nf, float fPdf, int ng, float gPdf) {
	float f = nf * fPdf, g = ng * gPdf;
	return (f * f) / (f * f + g * g);
}

Spectrum EstimateDirect(const IntersectInfo& it, float u_scattering, float v_scattering, Light* light, float u_light, float v_light, bool specular)
{
	Spectrum ld;
	// sample light source with multiple importance sampling
	XMFLOAT3 wi;
	float light_pdf = 0.0f, scattering_pdf = 0.0f;
	bool visibility = false;
	Spectrum li = light->sampleLi(it, u_light, v_light, &wi, &light_pdf, &visibility);
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
			if (!visibility)
				li = Spectrum();
			if (!li.isBlack())
			{
				if (light->isDelta())
					ld = ld + f * li / light_pdf;
				else
				{
					float weight = PowerHeuristic(1.0, scattering_pdf, 1.0, light_pdf);
					ld = ld + f * li * weight / light_pdf;
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
			f = it.bxdf->sample_f(it.wo, &wi, XMFLOAT2(u_scattering, v_scattering), &scattering_pdf, &sampled_type);
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
			Ray ray;
			Spectrum Tr(1.0f);

			g_pGlobalSys->cast_ray_to_get_intersection(ray, light_it);
			Spectrum li;
			if (light_it.isSurfaceInteraction())
			{

			}
			else
			{

			}
			if (!li.isBlack())
				ld = ld + f * li * Tr * weight / scattering_pdf;
		}
	}
	return ld;
}


Spectrum UniformSampleOneLight(const IntersectInfo& it)
{
	std::vector<Light*> lights = g_pGlobalSys->objectManager.getAllLights();
	Light* sel_light = lights[rand() % (lights.size())];

}