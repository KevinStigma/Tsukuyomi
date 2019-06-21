#pragma once

#define SAFE_DELETE(p){if(p){delete p;}p=NULL;}
#define SAFE_RELEASE(p){ if (p) { (p)->Release(); (p) = nullptr; } }

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <sstream>

extern void SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c);

template <class Type>
Type stringToNum(const std::string& str)
{
	std::istringstream iss(str);
	Type num;
	iss >> num;
	return num;
}