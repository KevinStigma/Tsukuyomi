#include "PathTracingRenderer.h"
#include "../intersect_info.h"
#include "OfflineRenderUtility.h"
#include "../BxDFs/BxDFType.h"
#include "../PbrMat/BSDF.h"
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
	srand(time(0));
	int width = int((height * camera->aspectRatio) + 0.5);
	QImage image(QSize(width, height), QImage::Format_ARGB32);
	std::string filename = generateRandomId();
	write = g_pGlobalSys->render_paras.writeFile;
	createOutputFile("./Data/RenderResults/" + filename + ".txt");
	sample_count = g_pGlobalSys->render_paras.sampleCount;
	max_bounce = g_pGlobalSys->render_paras.depth;
//#define DEBUG_PATHTRACING
#ifndef DEBUG_PATHTRACING
	int thread_count = write ? 1 : 8;
	#pragma omp parallel for num_threads(thread_count)
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{	
#endif
#ifdef DEBUG_PATHTRACING
			int i = 480, j = height - 1 - 360;
#endif
			outputStr("begin pixel:" + std::to_string(i) + " " + std::to_string(j));
			Pixel p = sample_pixel(camera, i, j, width, height);
			XMFLOAT3 rgb = Spectrum::XYZToRGB(p.xyz);
			if (p.filterWeightSum > 0.0)
			{
				rgb.x /= p.filterWeightSum;
				rgb.y /= p.filterWeightSum;
				rgb.z /= p.filterWeightSum;
			}
			int r = int(MathHelper::Clamp<float>(Spectrum::GammaCorrect(rgb.x) *255.0f + 0.5f, 0.0, 255.0));
			int g = int(MathHelper::Clamp<float>(Spectrum::GammaCorrect(rgb.y) *255.0f + 0.5f, 0.0, 255.0));
			int b = int(MathHelper::Clamp<float>(Spectrum::GammaCorrect(rgb.z) *255.0f + 0.5f, 0.0, 255.0));
			outputStr("RGB: "+std::to_string(r) + " " + std::to_string(g) + " " + std::to_string(b));
			image.setPixelColor(QPoint(i, height - 1 - j), QColor(r, g, b));
#ifndef DEBUG_PATHTRACING
		}
	}
#endif
	finish = clock();
	double totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	std::cout << "total time:" << totaltime << "seconds" << std::endl;
	QString path = QString("./Data/RenderResults/") + QString(filename.c_str()) + ".png";
	image.save(path);
	endOutputFile();
	std::cout << "Has output " << path.toStdString() << " successfully!" << std::endl;
}

Pixel PathTracingRenderer::sample_pixel(Camera* camera, int x, int y, int width, int height)
{
	float sample_x = 0.0f;
	float sample_y = 0.0f;
	Spectrum r;
	Pixel p;
	for (int i = 0; i < sample_count; i++)
	{
		sample_x = x + ((rand() % 100) / 100.0f);
		sample_y = y + ((rand() % 100) / 100.0f);
		outputStr("  sample:" + std::to_string(sample_x) + " " + std::to_string(sample_y));
		float w = MathHelper::TriangleFilterEval(sample_x - x - 0.5f, sample_y - y - 0.5f, 1.0f);
		Ray ray = camera->getRay(sample_x / width, sample_y / height);
		Spectrum radiance = Li(ray);
		p.xyz = MathHelper::AddFloat3(p.xyz, Spectrum::RGBToXYZ((radiance * w).getFloat3()));
		p.filterWeightSum += w;
	}
	return p;
}

Spectrum PathTracingRenderer::UniformSampleOneLight(const IntersectInfo& it)
{
	std::vector<Light*> lights = g_pGlobalSys->objectManager.getAllLights();
	Light* sel_light = lights[rand() % (lights.size())];
	XMFLOAT2 uScattering(generateRandomFloat(), generateRandomFloat());
	XMFLOAT2 uLight(generateRandomFloat(), generateRandomFloat());
	outputStr("      sample one light, ulight:" + std::to_string(uLight.x) + " "+ std::to_string(uLight.y)+
		" scattering:"+ std::to_string(uScattering.x)+" "+ std::to_string(uScattering.y));
	float light_count = g_pGlobalSys->objectManager.getLightsCountParameter();
	return light_count * EstimateDirect(it, uScattering, sel_light, uLight);
}

Spectrum PathTracingRenderer::Li(const Ray& r)
{
	bool specularBounce = false;
	Spectrum L, beta(1.0f, 1.0f, 1.0f);
	Ray ray(r);
	for (int bounce = 0; bounce < max_bounce; bounce++)
	{
		outputStr("    bounce:" + std::to_string(bounce)+":");
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
			{
				L += beta * it.Le(MathHelper::NegativeFloat3(ray.direction));
			}
		}

		if (!it.isSurfaceInteraction() || bounce >= max_bounce)
			break;
		it.ComputeScatteringFunctions();
		L += beta * UniformSampleOneLight(it);
		outputStr("      L:" + std::to_string(L.r) + " " + std::to_string(L.g) + " " + std::to_string(L.b));
		XMFLOAT3 wo(-ray.direction.x, -ray.direction.y, -ray.direction.z), wi;
		float pdf;
		BxDFType flags;
		float f1 = generateRandomFloat(), f2 = generateRandomFloat();
		Spectrum f = it.bsdf->Sample_f(wo, &wi, XMFLOAT2(f1, f2), &pdf, BSDF_ALL, &flags);
		outputStr("      sample f:" + std::to_string(f.r) + " " + std::to_string(f.g) + " " + std::to_string(f.b)
		+" "+std::to_string(f1)+" "+std::to_string(f2));
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