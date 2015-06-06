#include "OBJMesh.h"
#include "TextureManager.h"
#include <memory>
using namespace ZShadeSandboxMesh;
//===============================================================================================================================
//===============================================================================================================================
GameDirectory3D* OBJMesh::mGD3D = 0;
//===============================================================================================================================
OBJMesh::OBJMesh(D3D* d3d, GameDirectory3D* g3D)
:   mD3DSystem(d3d)
{
	OBJMesh::mGD3D = g3D;
}
//===============================================================================================================================
OBJMesh::~OBJMesh()
{
}
//===============================================================================================================================
XMFLOAT3& OBJMesh::Scale()
{
	return m_pMesh->Scale();
}
//===============================================================================================================================
XMFLOAT3& OBJMesh::Rotate()
{
	return m_pMesh->Rotate();
}
//===============================================================================================================================
XMFLOAT3& OBJMesh::Position()
{
	return m_pMesh->Position();
}
//===============================================================================================================================
void OBJMesh::Load(string filename, bool bRebuildNormals, bool isRHCoordSys)
{
	m_pMesh = new Mesh(mD3DSystem);
	m_pMesh->Load(filename, bRebuildNormals, isRHCoordSys);
}
//===============================================================================================================================
void OBJMesh::Render(OBJMeshRenderParameters render_param)
{
	m_pMesh->Render(render_param);
}
//===============================================================================================================================
void OBJMesh::SetWireframe(bool wire)
{
	m_pMesh->SetWireframe(wire);
}
//===============================================================================================================================
void OBJMesh::SetFarPlane(float farPlane)
{
	m_pMesh->SetFarPlane(farPlane);
}
//===============================================================================================================================
XMFLOAT3& OBJMesh::Mesh::Scale()
{
	return mScale;
}
//===============================================================================================================================
XMFLOAT3& OBJMesh::Mesh::Rotate()
{
	return mRotation;
}
//===============================================================================================================================
XMFLOAT3& OBJMesh::Mesh::Position()
{
	return mPosition;
}
//===============================================================================================================================
void OBJMesh::Mesh::SetWireframe(bool wire)
{
	mShader->SetWireframe(wire);
	mTessellationShader->SetWireframe(wire);
	mOBJDeferredShader->SetWireframe(wire);
}
//===============================================================================================================================
void OBJMesh::Mesh::SetFarPlane(float farPlane)
{
	mShader->SetFarPlane(farPlane);
	mTessellationShader->SetFarPlane(farPlane);
}
//===============================================================================================================================
bool OBJMesh::LoadObj(string filename, D3D* d3d, vector<VertexNormalTex>& vertices,
	vector<IndexType>& indices, vector<Group>& groups, bool& hadNormals, bool isRHCoordSys)
{
	// coordinate system handedness is handled similarly as in MeshFromOBJ10 sample (do nothing)

	vector<XMFLOAT3> positions;
	vector<XMFLOAT3> normals;
	vector<XMFLOAT2> texCoords;
	map<string, OBJMeshSurfaceMaterial> materials;

	OBJMeshSurfaceMaterial* currentMaterial = new OBJMeshSurfaceMaterial();
	InitMaterial(currentMaterial); // kind of default material

	// File Input
	ifstream file(filename.c_str());
	if (!file) return false;

	// Read the file line by line.
	for (string line; getline(file, line);)
	{
		string key;
		istringstream ss(line);
		ss >> key;

		if (key == "v") // vertex position
		{
			float x, y, z;
			ss >> x >> y >> z;
			if (isRHCoordSys)
			{
				z = z * -1.0f;
			}
			positions.push_back(XMFLOAT3(x*10, y*10, z*10));
		}
		else if (key == "vt") // texture coordinate
		{
			float u, v;
			ss >> u >> v;
			if (isRHCoordSys)
			{
				v = 1.0f - v;
			}
			texCoords.push_back(XMFLOAT2(u, v));
		}
		else if (key == "vn") // vertex normal
		{
			float  x, y, z;
			ss >> x >> y >> z;
			if (isRHCoordSys)
			{
				z = z * -1.0f;
			}
			normals.push_back(XMFLOAT3(x, y, z));
		}
		else if (key == "f") // face
		{
			// Other than triangular faces are not supported.
			for (size_t i = 0; i < 3; i++)
			{
				string vertexString;
				ss >> vertexString;
				istringstream vertexss(vertexString);

				string indexString;
				int index;
				VertexNormalTex vertex;

				// Each vertex must have position. Texture coordinates and normal are optional.
				getline(vertexss, indexString, '/');
				istringstream(indexString) >> index;
				vertex.position = positions[index - 1];

				if (getline(vertexss, indexString, '/') && !indexString.empty())
				{
					istringstream(indexString) >> index;
					vertex.texture = texCoords[index - 1];
				}
				else
					vertex.texture = XMFLOAT2(0, 0);

				if (getline(vertexss, indexString, '/') && !indexString.empty())
				{
					istringstream(indexString) >> index;
					vertex.normal = normals[index - 1];
				}
				else
					vertex.normal = XMFLOAT3(0, 1, 0);

				// If a similar vertex was added before, there is no need to add a new one.
				bool found = false;
				for (int i = 0; i < vertices.size(); i++)
				{
					if (memcmp(&vertices[i], &vertex, sizeof(VertexNormalTex)) == 0)
					{
						// The vertices area similar, use previously stored vertex.
						indices.push_back(i);
						found = true;
						break;
						// goto next_line;
					}					
				}
				if (!found)
				{
					indices.push_back(vertices.size());
					vertices.push_back(vertex);
				}
			}
		}
		else if (key == "g") // group
		{
			if (!groups.empty())
			{
				// finalize current group
				groups.back().Material = *currentMaterial;
				groups.back().IndexCount = indices.size() - groups.back().IndexStart;
			}

			// Start new group
			Group group;
			ss >> group.Name;
			group.IndexStart = indices.size();
			groups.push_back(group);
		}
		else if (key == "mtllib")
		{
			string mtllibFileName;
			ss >> mtllibFileName;
			if (!LoadMtl("Models//" + mtllibFileName, d3d, materials))
				return false;
		}
		else if (key == "usemtl") // material
		{
			string materialName;
			ss >> materialName;
			currentMaterial = &materials[materialName];
		}
	}
	hadNormals = !normals.empty();

	// Last group doesn't get finalized elsewhere
	if (!groups.empty())
	{
		groups.back().Material = *currentMaterial;
		groups.back().IndexCount = indices.size() - groups.back().IndexStart;
	}

	file.close();
	materials.clear();
	return true;
}
//===============================================================================================================================
bool OBJMesh::LoadMtl(string filename, D3D* d3d, map<string, OBJMeshSurfaceMaterial>& materials)
{
	// File Input
	ifstream file(filename.c_str());
	if (!file) return false;

	OBJMeshSurfaceMaterial* mat = NULL;
	for (string line; getline(file, line);)
	{
		string key;
		istringstream ss(line);
		ss >> key;

		if (key == "newmtl")
		{
			// Switching active materials
			mat = new OBJMeshSurfaceMaterial();
			InitMaterial(mat);
			ss >> mat->sMaterialName;
			materials[mat->sMaterialName] = *mat;
			mat = &materials[mat->sMaterialName];
		}

		// The rest of the commands need an active material
		if (mat == NULL) continue;

		if (key == "Ka")
		{
			// Ambient color
			float r, g, b;
			ss >> r >> g >> b;
			mat->vAmbientColor = XMFLOAT3(r, g, b);
		}
		else if (key == "Kd")
		{
			// Diffuse color
			float r, g, b;
			ss >> r >> g >> b;
			mat->vDiffuseColor = XMFLOAT3(r, g, b);
		}
		else if (key == "Ks")
		{
			// Specular color
			float r, g, b;
			ss >> r >> g >> b;
			mat->vSpecularColor = XMFLOAT3(r, g, b);
		}
		else if (key == "Ke")
		{
			// Emissive color
			float r, g, b;
			ss >> r >> g >> b;
			mat->vEmissiveColor = XMFLOAT3(r, g, b);
		}
		else if (key == "Tf")
		{
			// Transmission Filter
			float r, g, b;
			ss >> r >> g >> b;
			mat->vTransmissionFilter = XMFLOAT3(r, g, b);
		}
		else if (key == "d" || key == "Tr")
		{
			// Material Dissolve (Alpha)
			ss >> mat->fAlpha;
			if (mat->fAlpha > 0.0f)
				mat->bHasTransparency = true;
		}
		else if (key == "Ns")
		{
			// Specular Shininess
			int nShininess;
			ss >> nShininess;
			mat->fSpecularPower = nShininess;
		}
		else if (key == "Ni")
		{
			ss >> mat->fRefractionIndex;
		}
		else if (key == "illum")
		{
			// Specular on/off
			int illumination;
			ss >> illumination;
			mat->iIlluminationModel = illumination;
			mat->bSpecularToggle = (illumination == 2);
		}
		else if (key == "map_Kd")
		{
			// Diffuse Texture
			ss >> mat->sDiffuseTextureName;
			mat->bHasDiffuseTexture = true;
		}
		else if (key == "map_Ka")
		{
			// Ambient Texture
			ss >> mat->sAmbientTextureName;
			mat->bHasAmbientTexture = true;
		}
		else if (key == "map_Ks")
		{
			// Specular Texture
			ss >> mat->sSpecularTextureName;
			mat->bHasSpecularTexture = true;
		}
		else if (key == "map_Ke")
		{
			// Specular Texture
			ss >> mat->sEmissiveTextureName;
			mat->bHasEmissiveTexture = true;
		}
		else if (key == "map_d")
		{
			// Alpha Texture
			ss >> mat->sAlphaTextureName;
			mat->bHasTransparency = true;
		}
		else if (key == "map_bump")
		{
			// Normal Map (Bump Map) Texture
			ss >> mat->sNormalMapTextureName;
			mat->bHasNormalMap = true;
		}
	}

	// Load material textures
	for (auto& iter : materials)
	{
		auto& mat = iter.second;
		if (mat.bHasDiffuseTexture)
			mat.diffuseSRV = TextureManager::Instance()->GetTexture(BetterString(mGD3D->m_textures_path + "\\" + mat.sDiffuseTextureName));
		if (mat.bHasAmbientTexture)
			mat.ambientSRV = TextureManager::Instance()->GetTexture(BetterString(mGD3D->m_textures_path + "\\" + mat.sAmbientTextureName));
		if (mat.bHasSpecularTexture)
			mat.specularSRV = TextureManager::Instance()->GetTexture(BetterString(mGD3D->m_textures_path + "\\" + mat.sSpecularTextureName));
		if (mat.bHasEmissiveTexture)
			mat.emissiveSRV = TextureManager::Instance()->GetTexture(BetterString(mGD3D->m_textures_path + "\\" + mat.sEmissiveTextureName));
		if (mat.bHasNormalMap)
			mat.normalMapSRV = TextureManager::Instance()->GetTexture(BetterString(mGD3D->m_textures_path + "\\" + mat.sNormalMapTextureName));
		if (mat.bHasTransparency)
			mat.alphaSRV = TextureManager::Instance()->GetTexture(BetterString(mGD3D->m_textures_path + "\\" + mat.sAlphaTextureName));
	}

	file.close();
	return true;
}
//===============================================================================================================================
void OBJMesh::InitMaterial(OBJMeshSurfaceMaterial* mat)
{
	ZeroMemory(mat, sizeof(ZShadeSandboxMesh::OBJMeshSurfaceMaterial));
	mat->diffuseSRV = NULL;
	mat->ambientSRV = NULL;
	mat->specularSRV = NULL;
	mat->alphaSRV = NULL;
	mat->normalMapSRV = NULL;
	mat->emissiveSRV = NULL;
	mat->vDiffuseColor = XMFLOAT3(0.8f, 0.8f, 0.8f);
	mat->vAmbientColor = XMFLOAT3(0.2f, 0.2f, 0.2f);
	mat->vSpecularColor = XMFLOAT3(1.0f, 1.0f, 1.0f);
	mat->vEmissiveColor = XMFLOAT3(1.0f, 1.0f, 1.0f);
	mat->vTransmissionFilter = XMFLOAT3(1.0f, 1.0f, 1.0f);
	mat->fRefractionIndex = 0.0f;
	mat->iIlluminationModel = 1.0f;
	mat->fAlpha = 1.0f;
	mat->fSpecularPower = 0.3f;
	mat->fSpecularIntensity = 0.8f;
	mat->bSpecularToggle = false;
	mat->bHasDiffuseTexture = false;
	mat->bHasAmbientTexture = false;
	mat->bHasSpecularTexture = false;
	mat->bHasNormalMap = false;
	mat->bHasTransparency = false;
}
//===============================================================================================================================
OBJMesh::Mesh::Mesh(D3D* d3d)
	: mD3DSystem(d3d)
{
	mPosition = XMFLOAT3(0, 0, 0);
	mScale = XMFLOAT3(1, 1, 1);
	mRotation = XMFLOAT3(0, 0, 0);

	// Set the world matrix to the identity matrix
	mWorld = XMMatrixIdentity();

	mShader = new OBJMeshShader(mD3DSystem);
	mTessellationShader = new OBJMeshTessellationShader(mD3DSystem);
	mOBJDeferredShader = new OBJDeferredShader(mD3DSystem);
}
//===============================================================================================================================
OBJMesh::Mesh::~Mesh()
{
	// Need to release all subsets
	SAFE_RELEASE(mVB);
	SAFE_RELEASE(mIB);
	mGroups.clear();
	delete mShader;
}
//===============================================================================================================================
bool OBJMesh::Mesh::Load(string filename, bool bRebuildNormals, bool isRHCoordSys)
{
	vector<VertexNormalTex> vertices;
	vector<IndexType> indices;
	bool objHadNormals;

	if (!LoadObj(filename, mD3DSystem, vertices, indices, mGroups, objHadNormals, isRHCoordSys))
		return false;

	mVertexCount = vertices.size();
	mIndexCount = indices.size();
	mTriangleCount = mIndexCount / 3;

	if (!objHadNormals || bRebuildNormals)
	{
		for (auto& vertex : vertices)
		{
			vertex.normal = XMFLOAT3(0, 0, 0);
		}
		for (int i = 0; i < indices.size(); i += 3)
		{
			// Calculate triangle face normal

			int i0 = indices[i];
			int i1 = indices[i + 1];
			int i2 = indices[i + 2];

			XMFLOAT3 p0 = vertices[i0].position;
			XMFLOAT3 p1 = vertices[i1].position;
			XMFLOAT3 p2 = vertices[i2].position;

			float edge1x = p1.x - p0.x;
			float edge1y = p1.y - p0.y;
			float edge1z = p1.z - p0.z;

			float edge2x = p2.x - p0.x;
			float edge2y = p2.y - p0.y;
			float edge2z = p2.z - p0.z;

			float normalx = edge1y * edge2z - edge1z * edge2y;
			float normaly = edge1z * edge2x - edge1x * edge2z;
			float normalz = edge1x * edge2y - edge1y * edge2x;

			vertices[i0].normal.x += normalx;
			vertices[i0].normal.y += normaly;
			vertices[i0].normal.z += normalz;
			vertices[i1].normal.x += normalx;
			vertices[i1].normal.y += normaly;
			vertices[i1].normal.z += normalz;
			vertices[i2].normal.x += normalx;
			vertices[i2].normal.y += normaly;
			vertices[i2].normal.z += normalz;
		}
		for (auto& vertex : vertices)
		{
			float f = 1.0f / (sqrtf(vertex.normal.x * vertex.normal.x + vertex.normal.y * vertex.normal.y + vertex.normal.z * vertex.normal.z));

			vertex.normal.x *= f;
			vertex.normal.y *= f;
			vertex.normal.z *= f;
		}
	}

	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(ZShadeSandboxMesh::VertexNormalTex) * mVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = &vertices[0];
	mD3DSystem->GetDevice11()->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &mVB);

	D3D11_BUFFER_DESC indexBufferDesc;
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(IndexType) * mIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	mD3DSystem->GetDevice11()->CreateBuffer(&indexBufferDesc, &iinitData, &mIB);

	return true;
}
//===============================================================================================================================
void OBJMesh::Mesh::CalculateWorldMatrix()
{
	XMMATRIX scale, rx, ry, rz, translation;

	scale = XMMatrixScaling(mScale.x, mScale.y, mScale.z);
	rx = XMMatrixRotationX(mRotation.x);
	ry = XMMatrixRotationY(mRotation.y);
	rz = XMMatrixRotationZ(mRotation.z);
	translation = XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);

	XMMATRIX world = XMMatrixIdentity();

	mWorld = XMMatrixMultiply(world, scale);
	mWorld = XMMatrixMultiply(mWorld, rx);
	mWorld = XMMatrixMultiply(mWorld, ry);
	mWorld = XMMatrixMultiply(mWorld, rz);
	mWorld = XMMatrixMultiply(mWorld, translation);
}
//===============================================================================================================================
void OBJMesh::Mesh::Render(OBJMeshRenderParameters render_param)
{
	if (render_param.camera == NULL) return;

	// Get the world matrix transformation for the mesh
	CalculateWorldMatrix();
	
	XMMATRIX view = render_param.camera->View();
	if (render_param.reflection)
	{
		view = render_param.camera->ReflectionView();
	}
	
	XMMATRIX wvp = mWorld * view * render_param.camera->Proj();

	//Set the vertex buffer stride and offset
	unsigned int stride = sizeof(VertexNormalTex);
	unsigned int offset = 0;

	//Set the vertex buffer to active in the input assembler so it can be rendered
	mD3DSystem->GetDeviceContext()->IASetVertexBuffers(0, 1, &mVB, &stride, &offset);

	//Set the index buffer to active in the input assembler so it can be rendered
	if (sizeof(IndexType) == 2)
		mD3DSystem->GetDeviceContext()->IASetIndexBuffer(mIB, DXGI_FORMAT_R16_UINT, 0);
	else
		mD3DSystem->GetDeviceContext()->IASetIndexBuffer(mIB, DXGI_FORMAT_R32_UINT, 0);

	//Set the type of primitive that should be rendered from this vertex buffer
	switch (render_param.renderType)
	{
		case ERenderType::eTriangleList: mD3DSystem->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); break;
		case ERenderType::e3ControlPointPatchList: mD3DSystem->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST); break;
	}

	for (const auto& group : mGroups)
	{
		// Currently only uses a diffuse texture, need to add in all the other material textures to the shader
		// Also need to define what happens when there is no material -> uses material defined in WavefrontObj::InitMaterial

		auto* material = &group.Material;

		XMFLOAT4 materialDiffuse(material->vDiffuseColor.x, material->vDiffuseColor.y, material->vDiffuseColor.z, 0.0f);
		bool hasDiffuseTexture = material->bHasDiffuseTexture;
		
		if (render_param.bRenderDeferred)
		{
			mOBJDeferredShader->Render11(
				group.IndexStart,
				group.IndexCount,
				render_param.camera,
				wvp,
				XMFLOAT2(10, 300), // Add this to the material
				material->fSpecularIntensity,
				material->fSpecularPower,
				material->diffuseSRV
			);
		}
		else
		{
			if (render_param.tessellate)
			{
				mTessellationShader->Render(group.IndexStart, group.IndexCount, render_param.minTessDist, render_param.maxTessDist, render_param.minTess, render_param.maxTess,
					render_param.camera, wvp, render_param.clipplane, materialDiffuse, hasDiffuseTexture, material->diffuseSRV);
			}
			else
			{
				mShader->Render(group.IndexStart, group.IndexCount, render_param.camera, wvp, render_param.clipplane, materialDiffuse, hasDiffuseTexture, material->diffuseSRV);
			}
		}
	}
}
//===============================================================================================================================