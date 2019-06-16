#pragma once

#define SAFE_DELETE(p){if(p){delete p;}p=NULL;}
#define SAFE_RELEASE(p){ if (p) { (p)->Release(); (p) = nullptr; } }

#include <glm/glm.hpp>
