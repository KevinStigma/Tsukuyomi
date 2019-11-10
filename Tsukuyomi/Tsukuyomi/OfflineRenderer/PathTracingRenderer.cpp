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
	int width = int((height * camera->aspectRatio) + 0.5);
	QImage image(QSize(width, height), QImage::Format_ARGB32);
#pragma omp parallel for num_threads(8)
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			image.setPixelColor(QPoint(i, height - 1 - j), QColor(0, 0, 0));
		}
	}
	QString path = QString("./Data/RenderResults/") + QString(generateRandomId().c_str()) + ".png";
	image.save(path);
	std::cout << "Has output " << path.toStdString() << " successfully!" << std::endl;
}