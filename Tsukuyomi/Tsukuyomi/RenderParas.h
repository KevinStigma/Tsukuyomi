#pragma once

enum OfflineRenderType {
	NORMAL_DEBUG = 0,
	WHITTED = 1,
	PATH_TRACING = 2
};

struct RenderParas
{
	OfflineRenderType offline_render_type = PATH_TRACING;
	bool renderBVH = false;
	bool writeFile = false;
	bool enableHDR = false;
	bool enableSSAO = false;
	bool gammaCorrect = false;
	bool showDebugTex = false;
	bool showRulerLines = true;
	int depth = 1;
	int sampleCount = 16;
};