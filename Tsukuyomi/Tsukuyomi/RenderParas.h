#pragma once

enum OfflineRenderType {
	NORMAL_DEBUG = 0,
	WHITTED = 1,
	PATH_TRACING = 2
};

struct RenderParas
{
	OfflineRenderType offline_render_type = NORMAL_DEBUG;
};