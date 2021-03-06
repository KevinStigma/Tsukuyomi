#pragma once
#include <omp.h>
#include "../Objects/Camera.h"
#include "../intersect_info.h"
class OfflineRenderer
{
public:
	OfflineRenderer() {};
	virtual ~OfflineRenderer() {};
	virtual void start_render(Camera* camera, int height) = 0;
protected:
	void outputStr(std::string str);
	void createOutputFile(std::string filepath);
	void endOutputFile();
	bool write = false;
	std::ofstream writer;
};