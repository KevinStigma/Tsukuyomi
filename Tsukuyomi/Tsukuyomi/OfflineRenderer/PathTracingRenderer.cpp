#include "PathTracingRenderer.h"
#include "../intersect_info.h"
#include "OfflineRenderUtility.h"
#include "GlobalSys.h"
#include <QImage>

PathTracingRenderer::PathTracingRenderer()
{

}

PathTracingRenderer::~PathTracingRenderer()
{

}

void PathTracingRenderer::start_render(Camera* camera, int height)
{
	std::cout << "start path tracing!" << std::endl;
	clock_t start, finish;
	start = clock();
	int width = int((height * camera->aspectRatio) + 0.5);
	QImage image(QSize(width, height), QImage::Format_ARGB32);
#define DEBUG_PATHTRACING
#ifndef DEBUG_PATHTRACING
	#pragma omp parallel for num_threads(8)
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{	
#endif
#ifdef DEBUG_PATHTRACING
			int i = 507, j = 286;
#endif
			Spectrum color = sample_pixel(camera, i, j, width, height);
			color = Spectrum::Clamp(color);
			image.setPixelColor(QPoint(i, height - 1 - j), QColor(color.r * 255, color.g * 255, color.b * 255));
#ifndef DEBUG_PATHTRACING
		}
	}
#endif
	finish = clock();
	double totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	std::cout << "total time:" << totaltime << "seconds" << std::endl;
	QString path = QString("./Data/RenderResults/") + QString(generateRandomId().c_str()) + ".png";
	image.save(path);
	std::cout << "Has output " << path.toStdString() << " successfully!" << std::endl;
}

Spectrum PathTracingRenderer::sample_pixel(Camera* camera, int x, int y, int width, int height)
{
	srand(time(0));
	float sample_x = 0.0f;
	float sample_y = 0.0f;
	Spectrum color;
	for (int i = 0; i < sample_count; i++)
	{
		sample_x = x + ((rand() % 100) / 100.0f);
		sample_y = y + ((rand() % 100) / 100.0f);
		//sample_x = x;
		//sample_y = y;
		Ray ray = camera->getRay(sample_x / width, sample_y / height);
		color = color + Li(ray);
	}
	color = color / (float)sample_count;
	return color;
}

Spectrum PathTracingRenderer::Li(const Ray& r)
{
	bool specularBounce = false;
	Spectrum L, beta(1.0f, 1.0f, 1.0f);
	Ray ray(r);
	for (int bounce = 0; bounce < max_bounce; bounce++)
	{
		IntersectInfo it;
		g_pGlobalSys->cast_ray_to_get_intersection(ray, it);
		if (bounce == 0 || specularBounce)
		{
			if (!it.isSurfaceInteraction())
			{
				auto& lights = g_pGlobalSys->objectManager.getAllLights();
				for each (Light* light in lights)
					L += (beta* light->Le(ray));
			}
			else
				L += beta * it.Le(MathHelper::NegativeFloat3(ray.direction));
		}

		if (!it.isSurfaceInteraction() || bounce >= max_bounce)
			break;
		L += beta * UniformSampleOneLight(it);

		XMFLOAT3 wo(-ray.direction.x, -ray.direction.y, -ray.direction.z), wi;
		float pdf;
		BxDFType flags;
		Spectrum f = it.bxdf->sample_f(wo, &wi, XMFLOAT2(generateRandomFloat(), generateRandomFloat()), &pdf, &flags);
		wi = transVectorToWorldFromLocal(it.normal, wi);
		if (f.isBlack() || pdf == 0.0f)
			break;
		beta *= (f * abs(MathHelper::DotFloat3(wi, it.normal))/pdf);
		specularBounce = (flags&BxDFType::BSDF_SPECULAR) != 0;
		ray = it.spawnRay(wi);

		// Russian roulette
		if (bounce > 3)
		{
			float q = std::max<float>(0.05f, 1.0 - beta.y());
			if (generateRandomFloat() < q)
				break;
			beta /= (1.0 - q);
		}
	}
	return L;
}