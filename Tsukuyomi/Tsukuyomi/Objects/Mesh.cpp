#include "Mesh.h"
#include "../common.h"
#include "../Effects/Effects.h"
#include "../Vertex.h"
#include "../D3DRenderer.h"
#include "../PbrMat/Matte.h"
#include "../Accelerate/Triangle.h"
#include "../ShadowTransform.h"
#include "../ShadowMap.h"
#include "../EnvMap.h"
#include "GlobalSys.h"
#include <iostream>
#include <fstream>

XMFLOAT2 UniformSampleTriangle(XMFLOAT2 u)
{
	float u0 = sqrtf(u.x);
	return XMFLOAT2(1.0 - u0, u.y * u0);
}

Mesh::Mesh(std::string name, std::string file_path, XMFLOAT3 t, XMFLOAT3 s, XMFLOAT3 r, Object* al, RenderMats*render_mats):Object(name, t, s, r), area_light(al)
{
	loadObjMesh(file_path);
	type = MESH;
	if (!isEmpty())
		generateBuffers(g_pGlobalSys->renderer->getDevice());
	if (!render_mats)
		pbrMat = new MatteMaterial(Spectrum(0.725000, 0.710000, 0.680000), 0.0);
	else
		pbrMat = render_mats->first;
	mat = render_mats? render_mats->second : RenderLightHelper::Material();
}

Mesh::~Mesh()
{
	SAFE_RELEASE(vertexBuffer);
	SAFE_RELEASE(indexBuffer);
	SAFE_DELETE(pbrMat);
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

	bool use_shadow_map = g_pGlobalSys->objectManager.getCurSelShadowLight();
	ShadowTransform sm_trans = renderer->getShadowMapTransform();

	bool use_ssao = g_pGlobalSys->render_paras.enableSSAO;

	ID3DX11EffectTechnique* activeTech = false;
	if (use_shadow_map && use_ssao)
		activeTech = basicEffect->CustomLightShadowSSAOTech;
	else if(use_shadow_map && !use_ssao)
		activeTech = basicEffect->CustomLightShadowTech;
	else if (!use_shadow_map && use_ssao)
		activeTech = basicEffect->CustomLightSSAOTech;
	else
		activeTech = basicEffect->CustomLightTech;

	basicEffect->SetIsLight(bool(area_light));
	D3DX11_TECHNIQUE_DESC techDesc;
	activeTech->GetDesc(&techDesc);

	XMVECTOR v;
	XMMATRIX worldMat = getGlobalWorldMatrix();
	XMMATRIX inv_world_mat = XMMatrixTranspose(XMMatrixInverse(&v, worldMat));	
	XMMATRIX WVP = worldMat * camera.getViewMatrix() * camera.getProjMatrix();
	XMMATRIX WVPT = WVP * renderer->getTexTransformMat();
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{	
		basicEffect->SetWorld(worldMat);
		basicEffect->SetWorldInvTranspose(inv_world_mat);
		basicEffect->SetTexTransform(inv_world_mat);
		basicEffect->SetWorldViewProj(WVP);
		basicEffect->SetMaterial(mat);
		basicEffect->SetWorldViewProjTex(WVPT);
		if (use_shadow_map)
			basicEffect->SetShadowTransform(worldMat * XMLoadFloat4x4(&sm_trans.shadowTransMat));
		else
			basicEffect->SetShadowTransform(XMMatrixIdentity());

		activeTech->GetPassByIndex(p)->Apply(0, context);
		context->DrawIndexed(shape.mesh.indices.size(), 0, 0);
	}
}

void Mesh::renderNormalDepthMap(ID3D11DeviceContext * context, D3DRenderer* renderer)
{
	BuildNormalDepthMapEffect* effect = Effects::BuildNormalDepthMapFX;

	UINT stride = sizeof(Basic32);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetInputLayout(InputLayouts::PosNorTex);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Camera& camera = renderer->getCamera();

	ID3DX11EffectTechnique* activeTech = effect->BuildNormalDepthMapTech;
	D3DX11_TECHNIQUE_DESC techDesc;
	activeTech->GetDesc(&techDesc);

	XMVECTOR v;
	XMMATRIX worldMat = getGlobalWorldMatrix();
	XMMATRIX inv_world_mat = XMMatrixTranspose(XMMatrixInverse(&v, worldMat));
	XMMATRIX view_mat = camera.getViewMatrix();
	XMMATRIX proj_mat = camera.getProjMatrix();
	XMMATRIX WVP = worldMat * view_mat * proj_mat;
	XMMATRIX world_view_mat = worldMat * view_mat;
	XMMATRIX world_inv_transpose_view_mat = inv_world_mat * view_mat;
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		effect->SetWorld(worldMat);
		effect->SetWorldInvTranspose(inv_world_mat);
		effect->SetWorldViewProj(WVP);
		effect->SetWorldView(world_view_mat);
		effect->SetWorldInvTransposeView(world_inv_transpose_view_mat);

		activeTech->GetPassByIndex(p)->Apply(0, context);
		context->DrawIndexed(shape.mesh.indices.size(), 0, 0);
	}
}

void Mesh::renderToShadowMap(ID3D11DeviceContext * context, D3DRenderer* renderer, ShadowTransform* sm_trans)
{
	BuildShadowMapEffect* buildShadowMapEffect = Effects::BuildShadowMapFX;

	UINT stride = sizeof(Basic32);
	UINT offset = 0;
	context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetInputLayout(InputLayouts::PosNorTex);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	Camera& camera = renderer->getCamera();

	ID3DX11EffectTechnique* activeTech = buildShadowMapEffect->BuildShadowMapTech;
	D3DX11_TECHNIQUE_DESC techDesc;
	activeTech->GetDesc(&techDesc);

	XMVECTOR v;
	XMMATRIX worldMat = getGlobalWorldMatrix();
	XMMATRIX inv_world_mat = XMMatrixInverse(&v, worldMat);
	XMMATRIX view_mat = XMLoadFloat4x4(&sm_trans->lightViewTransMat);
	XMMATRIX proj_mat = XMLoadFloat4x4(&sm_trans->lightProjTransMat);
	XMMATRIX WVP = worldMat * view_mat * proj_mat;
	for (UINT p = 0; p < techDesc.Passes; ++p)
	{
		buildShadowMapEffect->SetWorld(worldMat);
		buildShadowMapEffect->SetWorldInvTranspose(worldMat);
		buildShadowMapEffect->SetTexTransform(inv_world_mat);
		buildShadowMapEffect->SetWorldViewProj(WVP);

		activeTech->GetPassByIndex(p)->Apply(0, context);
		context->DrawIndexed(shape.mesh.indices.size(), 0, 0);
	}
}


IntersectInfo Mesh::sample(XMFLOAT2 u, float& area)const
{
	float r = generateRandomFloat();
	int index = accumula_areas.size() - 1;
	for (int i = 0; i < accumula_areas.size()/3; i++)
		if (r <= accumula_areas[i])
		{
			index = i;
			break;
		}

	u = UniformSampleTriangle(u);
	XMFLOAT3 p1, p2, p3;
	p1 = getTriangleVertex(int(shape.mesh.indices[index * 3]));
	p2 = getTriangleVertex(int(shape.mesh.indices[index * 3 + 1]));
	p3 = getTriangleVertex(int(shape.mesh.indices[index * 3 + 2]));
	area = MathHelper::TriangleArea(p1, p2, p3);
	IntersectInfo it;

	XMVECTOR pos = XMVectorSet(
		u.x * p1.x + u.y * p2.x + (1.0f - u.x - u.y) * p3.x,
		u.x * p1.y + u.y * p2.y + (1.0f - u.x - u.y) * p3.y,
		u.x * p1.z + u.y * p2.z + (1.0f - u.x - u.y) * p3.z,
		1.0f
	);
	XMStoreFloat3(&it.pos,  XMVector3TransformCoord(pos, world_mat));

	XMFLOAT3 n1, n2, n3;
	n1 = getTriangleNormal(int(shape.mesh.indices[index * 3]));
	n2 = getTriangleNormal(int(shape.mesh.indices[index * 3 + 1]));
	n3 = getTriangleNormal(int(shape.mesh.indices[index * 3 + 2]));

	XMVECTOR nor = XMVector3Normalize(XMVectorSet(
		u.x * n1.x + u.y * n2.x + (1.0f - u.x - u.y) * n3.x,
		u.x * n1.y + u.y * n2.y + (1.0f - u.x - u.y) * n3.y,
		u.x * n1.z + u.y * n2.z + (1.0f - u.x - u.y) * n3.z,
		0.0f));

	XMStoreFloat3(&it.normal, XMVector3TransformNormal(nor, rot_mat));
	return it;
}

IntersectInfo Mesh::sample(const IntersectInfo&ref, XMFLOAT2 u, float*pdf)const
{
	float area;
	IntersectInfo it = sample(u, area);
	XMFLOAT3 wi(it.pos.x - ref.pos.x, it.pos.y - ref.pos.y, it.pos.z - ref.pos.z);
	if (MathHelper::Float3Length(wi) == 0.0)
		*pdf = 0;
	else {
		*pdf = 1 / area;
		wi = MathHelper::NormalizeFloat3(wi);
		// Convert from area measure, as returned by the Sample() call
		// above, to solid angle measure.
		*pdf *= (MathHelper::DistanceSquared(ref.pos, it.pos) / MathHelper::DotFloat3(it.normal, MathHelper::NegativeFloat3(wi)));
		if (std::isinf(*pdf)) *pdf = 0.f;
	}
	return it;
}

float Mesh::Pdf(const IntersectInfo & ref, const XMFLOAT3& wi)
{
	Ray ray = ref.spawnRay(wi);
	IntersectInfo it;
	float t;
	if (is_intersect(ray, t, it))
	{
		float pdf = MathHelper::DistanceSquared(ref.pos, it.pos) / (fabs(MathHelper::DotFloat3(ref.normal, MathHelper::NegativeFloat3(wi))) * it.tri_area);
		if (std::isnan(pdf))
			return 0.0f;
		return pdf;
	}
	else
		return 0.0f;
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
	computeArea();
	std::cout << "load " << obj_path << " successfully!" << std::endl;
}

void Mesh::writeObj(const std::string& obj_path)
{
	std::ofstream fout(obj_path.c_str());
	int count = shape.mesh.positions.size() / 3;
	XMFLOAT3 center = boundingBox.getCenter();
	for (int i = 0; i < count; i++)
	{
		fout << "v " << shape.mesh.positions[i * 3] - center.x << " " << shape.mesh.positions[i * 3 + 1] - center.y << " " << shape.mesh.positions[i * 3 + 2] - center.z << std::endl;
		fout << "vn " << shape.mesh.normals[i * 3]<< " " << shape.mesh.normals[i * 3 + 1]<< " " << shape.mesh.normals[i * 3 + 2]<< std::endl;
	}

	fout << std::endl;
	for (int i = 0; i < shape.mesh.indices.size()/3; i++)
	{
		int id1 = shape.mesh.indices[i*3]+1;
		int id2 = shape.mesh.indices[i * 3+1] + 1;
		int id3 = shape.mesh.indices[i * 3+2] + 1;
		fout << "f " << id1 << "//" << id1 << " " << id2 << "//" << id2 << " " << id3 << "//" << id3 << std::endl;
	}
	fout.close();
}

void Mesh::computeBoundingBox()
{
	int num_vertices = shape.mesh.positions.size() / 3;
	boundingBox.top.x = boundingBox.bottom.x = shape.mesh.positions[0];
	boundingBox.top.y = boundingBox.bottom.y = shape.mesh.positions[1];
	boundingBox.top.z = boundingBox.bottom.z = shape.mesh.positions[2];
	for (int i = 0; i < num_vertices; i++)
	{
		float x = shape.mesh.positions[i * 3];
		float y = shape.mesh.positions[i * 3 + 1];
		float z = shape.mesh.positions[i * 3 + 2];

		boundingBox.top.x = std::max(boundingBox.top.x, x);
		boundingBox.top.y = std::max(boundingBox.top.y, y);
		boundingBox.top.z = std::max(boundingBox.top.z, z);

		boundingBox.bottom.x = std::min(boundingBox.bottom.x, x);
		boundingBox.bottom.y = std::min(boundingBox.bottom.y, y);
		boundingBox.bottom.z = std::min(boundingBox.bottom.z, z);
	}
}

float Mesh::computeArea()
{
	int num_faces = shape.mesh.indices.size() / 3;
	auto & indices = shape.mesh.indices;
	area = 0.0f;
	accumula_areas.resize(num_faces, 0.0);
	for (int i = 0; i < num_faces; i++)
	{
		XMFLOAT3 v1 = getTriangleVertex(indices[i * 3]);
		XMFLOAT3 v2 = getTriangleVertex(indices[i * 3 + 1]);
		XMFLOAT3 v3 = getTriangleVertex(indices[i * 3 + 2]);
		float cur_area = MathHelper::TriangleArea(v1, v2, v3);
		if (i == 0)
			accumula_areas[i] = cur_area;
		else
			accumula_areas[i] = accumula_areas[i - 1] + cur_area;
		area += cur_area;
	}

	for (int i = 0; i < accumula_areas.size(); i++)
		accumula_areas[i] /= area;
	accumula_areas[num_faces - 1] = 1.0f;
	return area;
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

std::vector<Primitive*> Mesh::getAllPrimitives()
{
	std::vector<Primitive*> primitives;
	int num_faces = shape.mesh.indices.size() / 3;
	auto & indices = shape.mesh.indices;
	for (int i = 0; i < num_faces; i++)
	{
		XMFLOAT3 v1 = getTriangleVertex(indices[i * 3]);
		XMFLOAT3 v2 = getTriangleVertex(indices[i * 3 + 1]);
		XMFLOAT3 v3 = getTriangleVertex(indices[i * 3 + 2]);

		XMFLOAT3 n1 = getTriangleNormal(indices[i * 3]);
		XMFLOAT3 n2 = getTriangleNormal(indices[i * 3 + 1]);
		XMFLOAT3 n3 = getTriangleNormal(indices[i * 3 + 2]);
		Triangle* triangle = new Triangle(this, v1, v2, v3, n1, n2, n3, i);
		primitives.push_back(triangle);
	}
	return primitives;
}

// return tirangle num
int Mesh::getComponentNum()
{
	return shape.mesh.indices.size() / 3;
}

bool Mesh::is_intersect(const Ray&ray, float& t, IntersectInfo& is_info)
{
	if (!is_intersect_bounding_box(ray))
		return false;
	int face_num = shape.mesh.indices.size() / 3;
	float min_t = -1.0;
	float beta, gama;
	int index[3];
	XMFLOAT3 vertices[3];
	XMVECTOR normals[3];
	XMMATRIX world_mat = getGlobalWorldMatrix();
	for (int i = 0; i < face_num; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			index[j] = shape.mesh.indices[i * 3 + j];
			XMStoreFloat3(&vertices[j],XMVector3TransformCoord(XMVectorSet(shape.mesh.positions[index[j] * 3],
				shape.mesh.positions[index[j] * 3 + 1],
				shape.mesh.positions[index[j] * 3 + 2], 1.0), world_mat));

			normals[j] = XMVector3Normalize(XMVector3TransformNormal(XMVectorSet(shape.mesh.normals[index[j] * 3],
				shape.mesh.normals[index[j] * 3 + 1],
				shape.mesh.normals[index[j] * 3 + 2], 0.0), world_mat));
		}
		if (ray.is_intersect_triangle(vertices[0], vertices[1], vertices[2], t, beta, gama))
		{
			if (min_t < 0.0 || t < min_t)
			{
				min_t = t;
				is_info.obj = this;
				is_info.pos = ray.getExtendPos(t);
				is_info.wo = XMFLOAT3(-ray.direction.x, -ray.direction.y, -ray.direction.z);
				is_info.tri_area = MathHelper::TriangleArea(vertices[0], vertices[1], vertices[2]);
				XMStoreFloat3(&is_info.normal, XMVector3Normalize(normals[0] + (normals[1] - normals[0])*beta + (normals[2] - normals[0])*gama));
			}
		}
	}
	if (min_t > 0.0)
	{
		t = min_t;
		return true;
	}
	return false;
}

XMFLOAT3 Mesh::getTriangleVertex(int index)const
{
	auto& verts = shape.mesh.positions;
	return XMFLOAT3(verts[index*3], verts[index * 3+1], verts[index * 3+2]);
}

XMFLOAT3 Mesh::getTriangleNormal(int index)const
{
	auto& norms = shape.mesh.normals;
	return XMFLOAT3(norms[index * 3], norms[index * 3 + 1], norms[index * 3 + 2]);
}

XMFLOAT2 Mesh::getTriangleTexCoord(int index)const
{
	auto& texs = shape.mesh.texcoords;
	return XMFLOAT2(texs[index * 3], texs[index * 3 + 1]);
}

XMFLOAT3 Mesh::getWorldCenter()
{
	XMMATRIX mat = getBoundingBox().getTransMatrix() * getGlobalWorldMatrix();
	XMVECTOR v = XMVector3TransformCoord(XMVectorSet(0.0, 0.0, 0.0, 1.0), mat);
	XMFLOAT3 world_pos;
	XMStoreFloat3(&world_pos, v);
	return world_pos;
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

Mesh* getMesh(Object* obj)
{
	Mesh* mesh = nullptr;
	if (obj->getType() == AREA_LIGHT)
		mesh = dynamic_cast<AreaLight*>(obj)->getMesh();
	else if (obj->getType() == MESH)
		mesh = dynamic_cast<Mesh*>(obj);
	return mesh;
}