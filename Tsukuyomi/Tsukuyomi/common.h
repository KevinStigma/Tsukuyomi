#pragma once

#define SAFE_DELETE(p){if(p){delete p;} p=nullptr;}
#define SAFE_RELEASE(p){ if (p) { (p)->Release(); (p) = nullptr; } }

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <fstream>
#include <limits>
#include <stack>
#include <DirectXMath.h>

using namespace DirectX;
typedef float Float;

enum AXIS { NO = -1, X = 0, Y = 1, Z = 2 };

extern void SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c);
extern XMFLOAT4 transEulerAnglesToQuaternions(XMFLOAT3 angles);
extern XMFLOAT3 transQuaternionsToEulerAngles(XMFLOAT4 quat);
extern std::string generateRandomId(int len=12);
extern float generateRandomFloat();

template <class Type>
Type stringToNum(const std::string& str)
{
	std::istringstream iss(str);
	Type num;
	iss >> num;
	return num;
}