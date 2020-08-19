#pragma once
#define STB_IMAGE_IMPLEMENTATION

#include <string>
#include <iostream>
#include <stb_image.h>

class EnvironmentMap
{
public:
	EnvironmentMap(std::string path)
	{
		stbi_set_flip_vertically_on_load(true);
		int width, height, nrComponents;
		float *data = stbi_loadf(path.c_str(), &width, &height, &nrComponents, 0);
		std::cout << sizeof(data) << " " << width << " " << height << std::endl;
	}

	~EnvironmentMap()
	{
		stbi_image_free(data);
		data = nullptr;
	}

protected:
	int width=0, heigth=0;
	float* data=nullptr;
};