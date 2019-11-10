#include "PathTracingRenderer.h"
#include "../intersect_info.h"
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
	#pragma omp parallel for num_threads(8)
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{	
			Spectrum color = sample_pixel(camera, i, j, width, height);
			image.setPixelColor(QPoint(i, height - 1 - j), QColor(color.r*255, color.g*255,  color.b*255));
		}
	}
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
		Ray ray = camera->getRay(sample_x / width, sample_y / height);
		color = color + Li();
	}
	color = color / (float)sample_count;
	return color;
}

Spectrum PathTracingRenderer::Li()
{
	for (int bounce = 0; bounce < max_bounce; bounce++)
	{

	}
	return Spectrum();
}