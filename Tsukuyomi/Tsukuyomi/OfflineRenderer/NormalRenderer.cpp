#include "NormalRenderer.h"
#include "../intersect_info.h"
#include "GlobalSys.h"
#include <QImage>

NormalRenderer::NormalRenderer()
{

}

NormalRenderer::~NormalRenderer()
{

}

void NormalRenderer::start_render(Camera* camera, int height)
{
	int width = int((height * camera->aspectRatio) + 0.5);
	QImage image(QSize(width, height), QImage::Format_ARGB32);
	std::cout << "start normals rendering!" << std::endl;
	clock_t start, finish;
	start = clock();
	#pragma omp parallel for num_threads(8)
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			Ray ray = camera->getRay((i + 0.5f) / width, (j + 0.5f) / height);
			IntersectInfo info;
			cast_ray_to_get_intersection(ray, info);
			if (!info.obj)
				image.setPixelColor(QPoint(i, height - 1 - j), QColor(0, 0, 0));
			else
			{
				auto normal = info.normal;
				image.setPixelColor(QPoint(i, height - 1 - j), QColor(int(max(normal.x, 0.0) * 255), int(max(normal.y, 0.0) * 255), int(max(normal.z, 0.0) * 255)));
			}
		}
	}
	finish = clock();
	double totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	std::cout << "total time:" << totaltime << "seconds" << std::endl;
	QString path = QString("./Data/RenderResults/") + QString(generateRandomId().c_str()) + ".png";
	image.save(path);
	std::cout << "Has output " << path.toStdString() << " successfully!" << std::endl;
}