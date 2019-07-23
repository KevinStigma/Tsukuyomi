#include "Mesh.h"
#include "../common.h"
#include "../Effects/Effects.h"
#include "../Vertex.h"
#include "../D3DRenderer.h"
#include "GlobalSys.h"
#include <iostream>

Mesh::Mesh(std::string name, std::string file_path, XMFLOAT3 t, XMFLOAT3 s, XMFLOAT3 r):Object(name, t, s, r)
{
	loadObjMesh(file_path);
	type = MESH;
	if (!isEmpty())
		generateBuffers(g_pGlobalSys->renderer->getDevice());
}

Mesh::~Mesh()
{
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
}

bool Mesh::isEmpty()
{
	return (shape.mesh.positions.size() == 0) || (shape.mesh.indices.size() == 0);
}

void Mesh::render(ID3D11DeviceContext * context, D3DRenderer* renderer)
{
	BasicEffect*basicEffect = Effects::BasicFX;

	UINT stride = sizeof(Basic32);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetInputLayout(InputLayouts::PosNorTex);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Camera& camera = renderer->getCamera();
	auto & lights = renderer->getLights();
	auto & mats = renderer->getMaterials();

	XMFLOAT3 eyePosW = camera.getPosition();
	basicEffect->SetDirLights(&lights[0]);
	basicEffect->SetEyePosW(eyePosW);

	ID3DX11EffectTechnique* activeTech = basicEffect->Light1Tech;
	D3DX11_TECHNIQUE_DESC techDesc;
	activeTech->GetDesc(&techDesc);

	XMVECTOR v;
	XMMATRIX worldMat = getWorldMatrix();
	XMMATRIX inv_world_mat = XMMatrixInverse(&v, worldMat);
	XMMATRIX WVP = worldMat * camera.getViewMatrix() * camera.getProjMatrix();
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{	
		basicEffect->SetWorld(worldMat);
		basicEffect->SetWorldInvTranspose(worldMat);
		basicEffect->SetTexTransform(inv_world_mat);
		basicEffect->SetWorldViewProj(WVP);
		basicEffect->SetMaterial(mats[0]);

		activeTech->GetPassByIndex(p)->Apply(0, context);
		context->DrawIndexed(shape.mesh.indices.size(), 0, 0);
	}
}

void Mesh::loadObjMesh(const std::string& obj_path)
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
	mesh_path = obj_path;
	if (shape.mesh.normals.size() == 0)
		constructNormals();
	computeBoundingBox();
	std::cout << "load " << obj_path << " successfully!" << std::endl;
}

void Mesh::computeBoundingBox()
{
	int num_vertices = shape.mesh.positions.size() / 3;
	boundingBox.top.x = boundingBox.bottom.x = shape.mesh.positions[0];
	boundingBox.top.y = boundingBox.bottom.y = shape.mesh.positions[1];
	boundingBox.top.z = boundingBox.bottom.z = shape.mesh.positions[2];
	XMFLOAT3 center(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < num_vertices; i++)
	{
		float x = shape.mesh.positions[i * 3];
		float y = shape.mesh.positions[i * 3 + 1];
		float z = shape.mesh.positions[i * 3 + 2];

		center.x += x;
		center.y += y;
		center.z += z;

		boundingBox.top.x = max(boundingBox.top.x, x);
		boundingBox.top.y = max(boundingBox.top.y, y);
		boundingBox.top.z = max(boundingBox.top.z, z);

		boundingBox.bottom.x = min(boundingBox.bottom.x, x);
		boundingBox.bottom.y = min(boundingBox.bottom.y, y);
		boundingBox.bottom.z = min(boundingBox.bottom.z, z);
	}
	center.x /= num_vertices;
	center.y /= num_vertices;
	center.z /= num_vertices;
	boundingBox.center = center;
}

void Mesh::generateBuffers(ID3D11Device* device)
{
	if (isEmpty())
		return;

	tinyobj::mesh_t& mesh = shape.mesh;

	std::vector<Basic32> vertices;
	const std::vector<float>& positions = mesh.positions;
	const std::vector<float>& normals = mesh.normals;
	const std::vector<float>& texs = mesh.texcoords;
	bool m_hasTex = false;
	if (texs.size())
		m_hasTex = true;

	int num_vertex = positions.size() / 3;
	for (int i = 0; i < num_vertex; i++)
	{
		Basic32 v(positions[i * 3], positions[i * 3 + 1], positions[i * 3 + 2],
			normals[i * 3], normals[i * 3 + 1], normals[i * 3 + 2], 0, 0);
		if (m_hasTex)
		{
			v.tex.x = texs[i * 2];
			v.tex.y = texs[i * 2 + 1];
		}
		vertices.push_back(v);
	}

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(Basic32)* vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	device->CreateBuffer(&vbd, &vinitData, &vertexBuffer);

	const std::vector<unsigned int>& indices = mesh.indices;
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(unsigned int)* indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	device->CreateBuffer(&ibd, &iinitData, &indexBuffer);
}

void Mesh::constructNormals()
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
