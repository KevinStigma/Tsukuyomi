#pragma once
#pragma once
#include "../common.h"
#include "../GlobalSys.h"
#include "../RGBSpectrum.h"
#include "../intersect_info.h"

Spectrum EstimateDirect(const IntersectInfo& it, XMFLOAT2 uScattering, Light* light, XMFLOAT2 uLight, bool specular=false);

extern Spectrum UniformSampleOneLight(const IntersectInfo& it);
