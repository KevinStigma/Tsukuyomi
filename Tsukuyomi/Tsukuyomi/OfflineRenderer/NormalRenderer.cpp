#include "NormalRenderer.h"

NormalRenderer::NormalRenderer()
{

}

NormalRenderer::~NormalRenderer()
{

}

void NormalRenderer::start_renderer(Camera* camera, int height)
{
	int width = int((height * camera->aspectRatio) + 0.5);
	for(int i=0;i<width;i++)
		for (int j = 0; j < height; j++)
		{
			Ray ray = camera->getRay((i+0.5f)/width, (j+0.5f)/height);
		}
}