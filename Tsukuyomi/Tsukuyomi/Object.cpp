#define TINYOBJLOADER_IMPLEMENTATION
#include "Object.h"
#include <iostream>

Object::Object(std::string obj_name, std::string obj_path, XMFLOAT3 t, XMFLOAT3 s, XMFLOAT4 r):translation(t), scale(s), rotation(r)
{
	loadObjMesh(obj_path);
	name = obj_name;
}

Object::~Object()
{

}

bool Object::isEmpty()
{
	return (shape.mesh.positions.size() == 0) || (shape.mesh.indices.size() == 0);
}

void Object::setTranslation(XMFLOAT3 t)
{
	translation = t;
}

void Object::setScale(XMFLOAT3 s)
{
	scale = s;
}

void Object::setRotation(XMFLOAT4 r)
{
	rotation = r;
}

void Object::updateTransform(XMFLOAT3 t, XMFLOAT3 s, XMFLOAT4 r)
{
	setTranslation(t);
	setRotation(r);
	setScale(s);
}

void Object::constructNormals()
{
	if (isEmpty())
		return;
	int face_num = shape.mesh.indices.size() / 3;
	int vertex_num = shape.mesh.positions.size() / 3;
	std::vector<glm::vec3> face_normals(face_num);
	std::vector<std::vector<int>> faces_adj_verts(vertex_num);
	std::vector<float>& positions = shape.mesh.positions;
	int index[3];
	glm::vec3 tri_v[3];
	for (int i = 0; i < face_num; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			index[j] = shape.mesh.indices[i * 3 + j];
			tri_v[j] = glm::vec3(positions[index[j] * 3],
				positions[index[j] * 3 + 1],
				positions[index[j] * 3 + 2]);
			faces_adj_verts[index[j]].push_back(i);
		}
		face_normals[i] = glm::cross(glm::normalize(-(tri_v[2] - tri_v[1])), glm::normalize(tri_v[1] - tri_v[0]));
	}

	std::vector<float>& normals = shape.mesh.normals;
	normals.resize(positions.size());
	for (int i = 0; i < (int)faces_adj_verts.size(); i++)
	{
		glm::vec3 nor(0, 0, 0);
		for (int j = 0; j < (int)faces_adj_verts[i].size(); j++)
		{
			int index = faces_adj_verts[i][j];
			nor += face_normals[index];
		}
		nor /= (int)faces_adj_verts[i].size();
		normals[i * 3] = nor.x;
		normals[i * 3 + 1] = nor.y;
		normals[i * 3 + 2] = nor.z;
	}
}

void Object::loadObjMesh(const std::string& obj_path)
{
	if (obj_path.length() == 0)
		return;

	std::string err;
	std::vector<tinyobj::material_t> mat_list; //we ignore the information of materials now :)

	std::vector<tinyobj::shape_t> obj_list;
	bool ret = tinyobj::LoadObj(obj_list, mat_list, err, obj_path.c_str());
	if (!err.empty()) {
		std::cerr << err << std::endl;
	}
	if (!ret)
		return;
	shape = obj_list[0];

	std::cout << "load " << obj_path << " successfully!" << std::endl;
}
