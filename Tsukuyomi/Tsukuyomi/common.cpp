#include "common.h"
#include "time.h"
#include "string.h"

char table[] = { '1','2','3','4','5','6','7','8','9', '0', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o',
'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q',
'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '+', '-', '\0'};

void SplitString(const std::string& s, std::vector<std::string>& v, const std::string& c)
{
	std::string::size_type pos1, pos2;
	pos2 = s.find(c);
	pos1 = 0;
	v.clear();
	while (std::string::npos != pos2)
	{
		v.push_back(s.substr(pos1, pos2 - pos1));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != s.length())
		v.push_back(s.substr(pos1));
}

XMFLOAT4 transEulerAnglesToQuaternions(XMFLOAT3 angles)
{
	float psi = angles.x;
	float theta = angles.y;
	float phi = angles.z;
	float cos_half_phi = cos(phi * 0.5);
	float cos_half_psi = cos(psi * 0.5);
	float cos_half_theta = cos(theta * 0.5);
	float sin_half_phi = sin(phi * 0.5);
	float sin_half_psi = sin(psi * 0.5);
	float sin_half_theta = sin(theta * 0.5);

	return XMFLOAT4(cos_half_phi*sin_half_theta*cos_half_psi+sin_half_phi*cos_half_theta*sin_half_psi,
		cos_half_phi*cos_half_theta*sin_half_psi - sin_half_phi*sin_half_theta*cos_half_psi, 
		sin_half_phi*cos_half_theta*cos_half_psi - cos_half_phi*sin_half_theta*sin_half_psi, 
		cos_half_phi*cos_half_theta*cos_half_psi + sin_half_phi*sin_half_theta*sin_half_psi);
}

XMFLOAT3 transQuaternionsToEulerAngles(XMFLOAT4 quat)
{
	float x = quat.x;
	float y = quat.y;
	float z = quat.z;
	float w = quat.w;
	return XMFLOAT3(atan2(2.0*(w*z+x*y), 1.0-2.0*(z*z+x*x)), asin(2.0*(w*x-y*z)), atan2(2.0*(w*y+z*x),1.0-2.0*(x*x+y*y)));
}

std::string generateRandomId(int len)
{
	srand(time(0));
	int table_len = strlen(table);
	std::string str = "";
	for (int i = 0; i < len; i++)
	{
		str += table[rand()%table_len];
	}
	return str;
}