#include "OfflineRenderer.h"
#include "../GlobalSys.h"
#include "../Objects/Mesh.h"
void OfflineRenderer::outputStr(std::string str)
{
	if (!write)
		return;
	writer << str << std::endl;
}

void OfflineRenderer::createOutputFile(std::string filepath)
{
	if (!write)
		return;
	writer.open(filepath);
}

void OfflineRenderer::endOutputFile()
{
	if (!write)
		return;
	writer.close();
}