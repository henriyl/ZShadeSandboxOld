#include "ZMath.h"
#include "CustomMesh.h"
#include "TextureManager.h"
#include "ZShadeMessageCenter.h"
#include "MeshUtil.h"
#include "Shaders.h"
#include "MaterialShader.h"
#include "ShadowMapBuildShader.h"
#include "MaterialGBufferShader.h"
#include "MaterialLightShader.h"
#include "MaterialTessellationShader.h"
#include "MaterialLightTessellationShader.h"
using ZShadeSandboxMesh::CustomMesh;
//===============================================================================================================================
//MaterialShader*						CustomMesh::pMaterialShader = 0;
//MaterialLightShader*				CustomMesh::pLightShader = 0;
//MaterialTessellationShader*			CustomMesh::pQuadMaterialTessellationShader = 0;
//MaterialTessellationShader* 		CustomMesh::pTriMaterialTessellationShader = 0;
//MaterialLightTessellationShader*	CustomMesh::pQuadMaterialLightTessellationShader = 0;
//MaterialLightTessellationShader*	CustomMesh::pTriMaterialLightTessellationShader = 0;
//ShadowMapBuildShader*				CustomMesh::pShadowMapBuildShader = 0;
//MaterialGBufferShader*				CustomMesh::pMaterialGBufferShader = 0;
//===============================================================================================================================
CustomMesh::CustomMesh(D3D* d3d, ZShadeSandboxMesh::MeshParameters mp, char* filename)
:   mD3DSystem(d3d)
//,   mPosition(mp.pos)
//,   mRotation(mp.rot)
//,   mScale(mp.scale)
,   m_Wireframe(false)
,   m_DisableIndexBuffer(false)
,   m_TranslateBillboard(false)
,   m_RenderShader(true)
//,   mTriangleCount(0)
,   mMeshParameters(mp)
{
	Init();
	
	mEngineOptions = d3d->GetEngineOptions();
	
	LoadFromFile(filename);
	
	CreateBuffers();
	
	mMaterial = mp.material;
}
//===============================================================================================================================
CustomMesh::CustomMesh(D3D* d3d, ZShadeSandboxMesh::MeshParameters mp)
:	mD3DSystem(d3d)
//,	mPosition(mp.pos)
//,	mRotation(mp.rot)
//,	mScale(mp.scale)
,	m_Wireframe(false)
,	m_DisableIndexBuffer(false)
,	m_TranslateBillboard(false)
,   m_RenderShader(true)
//,   mTriangleCount(0)
,   mMeshParameters(mp)
{
	Init();

	mEngineOptions = d3d->GetEngineOptions();
	
	mMaterial = mp.material;
}
//===============================================================================================================================
CustomMesh::~CustomMesh()
{
	//SAFE_RELEASE( mTexture );
}
//===============================================================================================================================
void CustomMesh::LoadFromFile(char* filename)
{
	MeshData* md = MeshUtil::LoadMesh(filename, mMeshParameters.vertexType);

	mAttributes->mVertexCount = md->vertexCount;
	mAttributes->mIndexCount = md->indexCount;
	mAttributes->mIndices.resize(mAttributes->mIndexCount);

	mAttributes->mTriangleCount = mAttributes->mVertexCount / 3;

	switch (mMeshParameters.vertexType)
	{
		case ZShadeSandboxMesh::EVertexType::VT_NormalTex:
		{
			mAttributes->mVerticesNormalTex.resize(mAttributes->mVertexCount);
			
			// Load the vertex array and index array with data.
			for (int i = 0; i < mAttributes->mVertexCount; i++)
			{
				mAttributes->mVerticesNormalTex[i].position = md->dataVNT[i].position;
				mAttributes->mVerticesNormalTex[i].normal = md->dataVNT[i].normal;
				mAttributes->mVerticesNormalTex[i].texture = md->dataVNT[i].texture;
				mAttributes->mIndices[i] = i;
			}
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_NormalTexTan:
		{
			mAttributes->mVerticesNormalTexTan.resize(mAttributes->mVertexCount);
			
			// Load the vertex array and index array with data.
			for (int i = 0; i < mAttributes->mVertexCount; i++)
			{
				mAttributes->mVerticesNormalTexTan[i].position = md->dataVNTT[i].position;
				mAttributes->mVerticesNormalTexTan[i].normal = md->dataVNTT[i].normal;
				mAttributes->mVerticesNormalTexTan[i].texture = md->dataVNTT[i].texture;
				mAttributes->mVerticesNormalTexTan[i].tangentU = md->dataVNTT[i].tangentU;
				mAttributes->mIndices[i] = i;
			}
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Pos:
		{
			mAttributes->mVerticesPos.resize(mAttributes->mVertexCount);

			// Load the vertex array and index array with data.
			for (int i = 0; i < mAttributes->mVertexCount; i++)
			{
				mAttributes->mVerticesPos[i].position = md->dataPos[i].position;
				mAttributes->mIndices[i] = i;
			}
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Tex:
		{
			mAttributes->mVerticesTex.resize(mAttributes->mVertexCount);
			
			// Load the vertex array and index array with data.
			for (int i = 0; i < mAttributes->mVertexCount; i++)
			{
				mAttributes->mVerticesTex[i].position = md->dataTex[i].position;
				mAttributes->mVerticesTex[i].texture = md->dataTex[i].texture;
				mAttributes->mIndices[i] = i;
			}
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Color:
		{
			mAttributes->mVerticesColor.resize(mAttributes->mVertexCount);
			
			// Load the vertex array and index array with data.
			for (int i = 0; i < mAttributes->mVertexCount; i++)
			{
				mAttributes->mVerticesColor[i].position = md->dataColor[i].position;
				mAttributes->mVerticesColor[i].color = md->dataColor[i].color;
				mAttributes->mIndices[i] = i;
			}
		}
		break;
	}
}
//===============================================================================================================================
void CustomMesh::InitializeMesh()
{
	if (mMeshParameters.bUseTessellation)
		InitTessellation();
	else
		Initialize();
}
//===============================================================================================================================
void CustomMesh::Init()
{
	//// Create the shaders only once if they do not exist
	//if (CustomMesh::pMaterialShader == 0
	//	&& CustomMesh::pLightShader == 0
	//	&& CustomMesh::pQuadMaterialTessellationShader == 0
	//	&& CustomMesh::pTriMaterialTessellationShader == 0
	//	&& CustomMesh::pQuadMaterialLightTessellationShader == 0
	//	&& CustomMesh::pTriMaterialLightTessellationShader == 0
	//	&& CustomMesh::pShadowMapBuildShader == 0
	//	&& CustomMesh::pMaterialGBufferShader == 0)
	//{
	//	CustomMesh::pMaterialShader = new MaterialShader(mD3DSystem);
	//	CustomMesh::pLightShader = new MaterialLightShader(mD3DSystem);
	//	CustomMesh::pQuadMaterialTessellationShader = new MaterialTessellationShader(mD3DSystem, ZShadeSandboxLighting::EMaterialTessellationType::eQuad);
	//	CustomMesh::pTriMaterialTessellationShader = new MaterialTessellationShader(mD3DSystem, ZShadeSandboxLighting::EMaterialTessellationType::eTri);
	//	CustomMesh::pQuadMaterialLightTessellationShader = new MaterialLightTessellationShader(mD3DSystem, ZShadeSandboxLighting::EMaterialTessellationType::eQuad);
	//	CustomMesh::pTriMaterialLightTessellationShader = new MaterialLightTessellationShader(mD3DSystem, ZShadeSandboxLighting::EMaterialTessellationType::eTri);
	//	CustomMesh::pShadowMapBuildShader = new ShadowMapBuildShader(mD3DSystem);
	//	CustomMesh::pMaterialGBufferShader = new MaterialGBufferShader(mD3DSystem);
	//}
	
	//pVB = 0;
	//pInstanceBuffer = 0;
	//mInstanceCount = 0;
	
	//Load the world matrix
	//XMStoreFloat4x4(&mWorld, XMMatrixIdentity());
	
	mAttributes = new MeshAttributes();
	mAttributes->mD3D = mD3DSystem;

	mAttributes->mPosition = mMeshParameters.pos;
	mAttributes->mScale = mMeshParameters.scale;
	mAttributes->mRotation = mMeshParameters.rot;

	CreatePhysicsBody();
}
//===============================================================================================================================
//void CustomMesh::SetTexture(ID3D11ShaderResourceView* texSRV)
//{
//	mTexture->setTexture11() = texSRV;
//}
//===============================================================================================================================
//void CustomMesh::SetTextureName(string textureName)
//{
//	//return mTexture->GetTextureName();
//	if (mMaterial->GetTexture(0, ZShadeSandboxLighting::EMaterialTextureType::eDiffuse))
//	{
//		//return mMaterial->GetTexture(0)->GetTextureName();
//	}
//}
//===============================================================================================================================
void CustomMesh::RemoveTexture(int i)
{
	mMaterial->RemoveTexture(i);
}
//===============================================================================================================================
void CustomMesh::SetMaterialDiffuseTexture(ID3D11ShaderResourceView* texSRV)
{
	mMaterial->SetMaterialDiffuseTexture(texSRV);
}
//===============================================================================================================================
string CustomMesh::TextureName() const
{
	if (mMaterial->GetTexture(0, ZShadeSandboxLighting::EMaterialTextureType::eDiffuse))
		return mMaterial->GetTexture(0)->GetTextureName();
	return "";
}
//===============================================================================================================================
/*void CustomMesh::ReplyMeshIndices(vector<UINT> ind)
{
	mIndexCount = ind.size();
	
	for (UINT i = 0; i < mIndexCount; i++)
	{
		mIndices.push_back(ind[i]);
	}
}
//===============================================================================================================================
void CustomMesh::Subdivide_VertexPos()
{
	//       v1
	//       *
	//      / \
	//     /   \
	//  m0*-----*m1
	//   / \   / \
	//  /   \ /   \
	// *-----*-----*
	// v0    m2     v2

	// As long as the mesh is dynamic it can subdivide in real time

	vector<UINT> ind;
	vector<ZShadeSandboxMesh::VertexPos> v;
	GrabSubdividedGeometry_VertexPos(mVerticesPos, mIndices, v, ind);

	mVertexCount = v.size();

	mIndices.resize(0);
	mVerticesPos.resize(0);

	// Reply the mesh with the new subdivision

	for (UINT i = 0; i < mVertexCount; i++)
	{
		mVerticesPos.push_back(v[i]);
	}

	ReplyMeshIndices(ind);
}
//===============================================================================================================================
void CustomMesh::GrabSubdividedGeometry_VertexPos(vector<ZShadeSandboxMesh::VertexPos> verts, vector<UINT> inds, vector<ZShadeSandboxMesh::VertexPos>& OutVerts, vector<UINT>& OutInd)
{
	UINT numTris = mIndexCount / 3;

	// Loop through the amount of triangles and subdivide them
	for (UINT i = 0; i < numTris; ++i)
	{
		ZShadeSandboxMesh::VertexPos v0 = verts[inds[i * 3 + 0]];
		ZShadeSandboxMesh::VertexPos v1 = verts[inds[i * 3 + 1]];
		ZShadeSandboxMesh::VertexPos v2 = verts[inds[i * 3 + 2]];

		// Create the midpoints of each vertex
		ZShadeSandboxMesh::VertexPos m0, m1, m2;

		m0.position.x = 0.5f * (v0.position.x + v1.position.x);
		m0.position.y = 0.5f * (v0.position.y + v1.position.y);
		m0.position.z = 0.5f * (v0.position.z + v1.position.z);

		m1.position.x = 0.5f * (v1.position.x + v2.position.x);
		m1.position.y = 0.5f * (v1.position.y + v2.position.y);
		m1.position.z = 0.5f * (v1.position.z + v2.position.z);

		m2.position.x = 0.5f * (v0.position.x + v2.position.x);
		m2.position.y = 0.5f * (v0.position.y + v2.position.y);
		m2.position.z = 0.5f * (v0.position.z + v2.position.z);

		// Add the new geometry
		OutVerts.push_back(v0);
		OutVerts.push_back(v1);
		OutVerts.push_back(v2);
		OutVerts.push_back(m0);
		OutVerts.push_back(m1);
		OutVerts.push_back(m2);

		// Link the indices for the new geometry
		OutInd.push_back(i * 6 + 0);
		OutInd.push_back(i * 6 + 3);
		OutInd.push_back(i * 6 + 5);

		OutInd.push_back(i * 6 + 3);
		OutInd.push_back(i * 6 + 4);
		OutInd.push_back(i * 6 + 5);

		OutInd.push_back(i * 6 + 5);
		OutInd.push_back(i * 6 + 4);
		OutInd.push_back(i * 6 + 2);

		OutInd.push_back(i * 6 + 3);
		OutInd.push_back(i * 6 + 1);
		OutInd.push_back(i * 6 + 4);
	}
}
//===============================================================================================================================
void CustomMesh::Subdivide_VertexTex()
{
	//       v1
	//       *
	//      / \
	//     /   \
	//  m0*-----*m1
	//   / \   / \
	//  /   \ /   \
	// *-----*-----*
	// v0    m2     v2
	
	// As long as the mesh is dynamic it can subdivide in real time
	
	vector<UINT> ind;
	vector<ZShadeSandboxMesh::VertexTex> v;
	GrabSubdividedGeometry_VertexTex(mVerticesTex, mIndices, v, ind);

	mVertexCount = v.size();
	
	mIndices.resize(0);
	mVerticesTex.resize(0);

	// Reply the mesh with the new subdivision

	for (UINT i = 0; i < mVertexCount; i++)
	{
		mVerticesTex.push_back(v[i]);
	}

	ReplyMeshIndices(ind);
}
//===============================================================================================================================
void CustomMesh::GrabSubdividedGeometry_VertexTex(vector<ZShadeSandboxMesh::VertexTex> verts, vector<UINT> inds, vector<ZShadeSandboxMesh::VertexTex>& OutVerts, vector<UINT>& OutInd)
{
	UINT numTris = mIndexCount / 3;

	// Loop through the amount of triangles and subdivide them
	for (UINT i = 0; i < numTris; ++i)
	{
		ZShadeSandboxMesh::VertexTex v0 = verts[inds[i * 3 + 0]];
		ZShadeSandboxMesh::VertexTex v1 = verts[inds[i * 3 + 1]];
		ZShadeSandboxMesh::VertexTex v2 = verts[inds[i * 3 + 2]];

		// Create the midpoints of each vertex
		ZShadeSandboxMesh::VertexTex m0, m1, m2;

		m0.position.x = 0.5f * (v0.position.x + v1.position.x);
		m0.position.y = 0.5f * (v0.position.y + v1.position.y);
		m0.position.z = 0.5f * (v0.position.z + v1.position.z);

		m1.position.x = 0.5f * (v1.position.x + v2.position.x);
		m1.position.y = 0.5f * (v1.position.y + v2.position.y);
		m1.position.z = 0.5f * (v1.position.z + v2.position.z);

		m2.position.x = 0.5f * (v0.position.x + v2.position.x);
		m2.position.y = 0.5f * (v0.position.y + v2.position.y);
		m2.position.z = 0.5f * (v0.position.z + v2.position.z);

		// Add the new geometry
		OutVerts.push_back(v0);
		OutVerts.push_back(v1);
		OutVerts.push_back(v2);
		OutVerts.push_back(m0);
		OutVerts.push_back(m1);
		OutVerts.push_back(m2);

		// Link the indices for the new geometry
		OutInd.push_back(i * 6 + 0);
		OutInd.push_back(i * 6 + 3);
		OutInd.push_back(i * 6 + 5);

		OutInd.push_back(i * 6 + 3);
		OutInd.push_back(i * 6 + 4);
		OutInd.push_back(i * 6 + 5);

		OutInd.push_back(i * 6 + 5);
		OutInd.push_back(i * 6 + 4);
		OutInd.push_back(i * 6 + 2);

		OutInd.push_back(i * 6 + 3);
		OutInd.push_back(i * 6 + 1);
		OutInd.push_back(i * 6 + 4);
	}
}
//===============================================================================================================================
void CustomMesh::Subdivide_VertexColor()
{
	//       v1
	//       *
	//      / \
	//     /   \
	//  m0*-----*m1
	//   / \   / \
	//  /   \ /   \
	// *-----*-----*
	// v0    m2     v2

	// As long as the mesh is dynamic it can subdivide in real time

	vector<UINT> ind;
	vector<ZShadeSandboxMesh::VertexColor> v;
	GrabSubdividedGeometry_VertexColor(mVerticesColor, mIndices, v, ind);

	mVertexCount = v.size();

	mIndices.resize(0);
	mVerticesColor.resize(0);

	// Reply the mesh with the new subdivision

	for (UINT i = 0; i < mVertexCount; i++)
	{
		mVerticesColor[i] = v[i];
	}

	ReplyMeshIndices(ind);
}
//===============================================================================================================================
void CustomMesh::GrabSubdividedGeometry_VertexColor(vector<ZShadeSandboxMesh::VertexColor> verts, vector<UINT> inds, vector<ZShadeSandboxMesh::VertexColor>& OutVerts, vector<UINT>& OutInd)
{
	UINT numTris = mIndexCount / 3;

	// Loop through the amount of triangles and subdivide them
	for (UINT i = 0; i < numTris; ++i)
	{
		ZShadeSandboxMesh::VertexColor v0 = verts[inds[i * 3 + 0]];
		ZShadeSandboxMesh::VertexColor v1 = verts[inds[i * 3 + 1]];
		ZShadeSandboxMesh::VertexColor v2 = verts[inds[i * 3 + 2]];

		// Create the midpoints of each vertex
		ZShadeSandboxMesh::VertexColor m0, m1, m2;

		m0.position.x = 0.5f * (v0.position.x + v1.position.x);
		m0.position.y = 0.5f * (v0.position.y + v1.position.y);
		m0.position.z = 0.5f * (v0.position.z + v1.position.z);

		m1.position.x = 0.5f * (v1.position.x + v2.position.x);
		m1.position.y = 0.5f * (v1.position.y + v2.position.y);
		m1.position.z = 0.5f * (v1.position.z + v2.position.z);

		m2.position.x = 0.5f * (v0.position.x + v2.position.x);
		m2.position.y = 0.5f * (v0.position.y + v2.position.y);
		m2.position.z = 0.5f * (v0.position.z + v2.position.z);

		// Add the new geometry
		OutVerts.push_back(v0);
		OutVerts.push_back(v1);
		OutVerts.push_back(v2);
		OutVerts.push_back(m0);
		OutVerts.push_back(m1);
		OutVerts.push_back(m2);

		// Link the indices for the new geometry
		OutInd.push_back(i * 6 + 0);
		OutInd.push_back(i * 6 + 3);
		OutInd.push_back(i * 6 + 5);

		OutInd.push_back(i * 6 + 3);
		OutInd.push_back(i * 6 + 4);
		OutInd.push_back(i * 6 + 5);

		OutInd.push_back(i * 6 + 5);
		OutInd.push_back(i * 6 + 4);
		OutInd.push_back(i * 6 + 2);

		OutInd.push_back(i * 6 + 3);
		OutInd.push_back(i * 6 + 1);
		OutInd.push_back(i * 6 + 4);
	}
}
//===============================================================================================================================
void CustomMesh::Subdivide_VertexNormalTexTan()
{
	//       v1
	//       *
	//      / \
	//     /   \
	//  m0*-----*m1
	//   / \   / \
	//  /   \ /   \
	// *-----*-----*
	// v0    m2     v2

	// As long as the mesh is dynamic it can subdivide in real time

	vector<UINT> ind;
	vector<ZShadeSandboxMesh::VertexNormalTexTan> v;
	GrabSubdividedGeometry_VertexNormalTexTan(mVerticesVNTT, mIndices, v, ind);

	mVertexCount = v.size();

	mIndices.resize(0);
	mVerticesVNTT.resize(0);

	// Reply the mesh with the new subdivision

	for (UINT i = 0; i < mVertexCount; i++)
	{
		mVerticesVNTT.push_back(v[i]);
	}

	ReplyMeshIndices(ind);
}
//===============================================================================================================================
void CustomMesh::GrabSubdividedGeometry_VertexNormalTexTan(vector<ZShadeSandboxMesh::VertexNormalTexTan> verts, vector<UINT> inds, vector<ZShadeSandboxMesh::VertexNormalTexTan>& OutVerts, vector<UINT>& OutInd)
{
	UINT numTris = mIndexCount / 3;

	// Loop through the amount of triangles and subdivide them
	for (UINT i = 0; i < numTris; ++i)
	{
		ZShadeSandboxMesh::VertexNormalTexTan v0 = verts[inds[i * 3 + 0]];
		ZShadeSandboxMesh::VertexNormalTexTan v1 = verts[inds[i * 3 + 1]];
		ZShadeSandboxMesh::VertexNormalTexTan v2 = verts[inds[i * 3 + 2]];

		// Create the midpoints of each vertex
		ZShadeSandboxMesh::VertexNormalTexTan m0, m1, m2;

		m0.position.x = 0.5f * (v0.position.x + v1.position.x);
		m0.position.y = 0.5f * (v0.position.y + v1.position.y);
		m0.position.z = 0.5f * (v0.position.z + v1.position.z);

		m1.position.x = 0.5f * (v1.position.x + v2.position.x);
		m1.position.y = 0.5f * (v1.position.y + v2.position.y);
		m1.position.z = 0.5f * (v1.position.z + v2.position.z);

		m2.position.x = 0.5f * (v0.position.x + v2.position.x);
		m2.position.y = 0.5f * (v0.position.y + v2.position.y);
		m2.position.z = 0.5f * (v0.position.z + v2.position.z);

		// Add the new geometry
		OutVerts.push_back(v0);
		OutVerts.push_back(v1);
		OutVerts.push_back(v2);
		OutVerts.push_back(m0);
		OutVerts.push_back(m1);
		OutVerts.push_back(m2);

		// Link the indices for the new geometry
		OutInd.push_back(i * 6 + 0);
		OutInd.push_back(i * 6 + 3);
		OutInd.push_back(i * 6 + 5);

		OutInd.push_back(i * 6 + 3);
		OutInd.push_back(i * 6 + 4);
		OutInd.push_back(i * 6 + 5);

		OutInd.push_back(i * 6 + 5);
		OutInd.push_back(i * 6 + 4);
		OutInd.push_back(i * 6 + 2);

		OutInd.push_back(i * 6 + 3);
		OutInd.push_back(i * 6 + 1);
		OutInd.push_back(i * 6 + 4);
	}
}
//===============================================================================================================================
void CustomMesh::Subdivide_VertexNormalTex()
{
	//       v1
	//       *
	//      / \
	//     /   \
	//  m0*-----*m1
	//   / \   / \
	//  /   \ /   \
	// *-----*-----*
	// v0    m2     v2

	// As long as the mesh is dynamic it can subdivide in real time

	vector<UINT> ind;
	vector<ZShadeSandboxMesh::VertexNormalTex> v;
	GrabSubdividedGeometry_VertexNormalTex(mVerticesVNT, mIndices, v, ind);

	mVertexCount = v.size();

	mIndices.resize(0);
	mVerticesVNT.resize(0);

	// Reply the mesh with the new subdivision

	for (UINT i = 0; i < mVertexCount; i++)
	{
		mVerticesVNT.push_back(v[i]);
	}

	ReplyMeshIndices(ind);
}
//===============================================================================================================================
void CustomMesh::GrabSubdividedGeometry_VertexNormalTex(vector<ZShadeSandboxMesh::VertexNormalTex> verts, vector<UINT> inds, vector<ZShadeSandboxMesh::VertexNormalTex>& OutVerts, vector<UINT>& OutInd)
{
	UINT numTris = mIndexCount / 3;

	// Loop through the amount of triangles and subdivide them
	for (UINT i = 0; i < numTris; ++i)
	{
		ZShadeSandboxMesh::VertexNormalTex v0 = verts[inds[i * 3 + 0]];
		ZShadeSandboxMesh::VertexNormalTex v1 = verts[inds[i * 3 + 1]];
		ZShadeSandboxMesh::VertexNormalTex v2 = verts[inds[i * 3 + 2]];

		// Create the midpoints of each vertex
		ZShadeSandboxMesh::VertexNormalTex m0, m1, m2;

		m0.position.x = 0.5f * (v0.position.x + v1.position.x);
		m0.position.y = 0.5f * (v0.position.y + v1.position.y);
		m0.position.z = 0.5f * (v0.position.z + v1.position.z);

		m1.position.x = 0.5f * (v1.position.x + v2.position.x);
		m1.position.y = 0.5f * (v1.position.y + v2.position.y);
		m1.position.z = 0.5f * (v1.position.z + v2.position.z);

		m2.position.x = 0.5f * (v0.position.x + v2.position.x);
		m2.position.y = 0.5f * (v0.position.y + v2.position.y);
		m2.position.z = 0.5f * (v0.position.z + v2.position.z);

		// Add the new geometry
		OutVerts.push_back(v0);
		OutVerts.push_back(v1);
		OutVerts.push_back(v2);
		OutVerts.push_back(m0);
		OutVerts.push_back(m1);
		OutVerts.push_back(m2);

		// Link the indices for the new geometry
		OutInd.push_back(i * 6 + 0);
		OutInd.push_back(i * 6 + 3);
		OutInd.push_back(i * 6 + 5);

		OutInd.push_back(i * 6 + 3);
		OutInd.push_back(i * 6 + 4);
		OutInd.push_back(i * 6 + 5);

		OutInd.push_back(i * 6 + 5);
		OutInd.push_back(i * 6 + 4);
		OutInd.push_back(i * 6 + 2);

		OutInd.push_back(i * 6 + 3);
		OutInd.push_back(i * 6 + 1);
		OutInd.push_back(i * 6 + 4);
	}
}*/
//===============================================================================================================================
void CustomMesh::Update(float dt)
{
	if (mMeshParameters.rotationAxisX)
		mAttributes->mRotation.x += dt;
	
	if (mMeshParameters.rotationAxisY)
		mAttributes->mRotation.y += dt;
	
	if (mMeshParameters.rotationAxisZ)
		mAttributes->mRotation.z += dt;
}
//===============================================================================================================================
void CustomMesh::Render(ZShadeSandboxMesh::MeshRenderParameters rp)
{
	// Cannot do anything if there is no camera
	if (rp.camera == 0) return;
	
	//
	// Render the mesh buffers
	//
	
	if (rp.useInstancing)
		mAttributes->SetBuffersInstanced(rp.renderType);
	else
		mAttributes->SetBuffers(rp.renderType);
	
	//
	// Render the shader
	//
	
	if (rp.reflection)
		rp.view = rp.camera->ReflectionView4x4();
	else
		rp.view = rp.camera->View4x4();
	
	if (!rp.specifyWorld)
	{
		// Center the camera in this mesh
		if (rp.centerCam)
		{
			XMMATRIX scale, rx, ry, rz, translate;

			scale = XMMatrixScaling(mAttributes->mScale.x, mAttributes->mScale.y, mAttributes->mScale.z);
			rx = XMMatrixRotationX(mAttributes->mRotation.x);
			ry = XMMatrixRotationY(mAttributes->mRotation.y);
			rz = XMMatrixRotationZ(mAttributes->mRotation.z);

			//Get the position of the camera
			XMFLOAT3 eyeVertex = rp.camera->Position();

			if (rp.reflection)
			{
				//Invert the Y coordinate of the camera around the water plane height for the reflected camera position
				eyeVertex.y = -eyeVertex.y + (rp.seaLevel * 2.0f);
			}
			
			translate = XMMatrixTranslation(eyeVertex.x, eyeVertex.y, eyeVertex.z);

			XMMATRIX world = XMMatrixIdentity();

			world = XMMatrixMultiply(world, scale);
			world = XMMatrixMultiply(world, rx);
			world = XMMatrixMultiply(world, ry);
			world = XMMatrixMultiply(world, rz);
			world = XMMatrixMultiply(world, translate);
			
			rp.world = world;
		}
		else
		{
			if (rp.translateBillboard)
				rp.world = mAttributes->BillboardWorldXM(rp.camera->Position());
			else
				rp.world = mAttributes->WorldXM();
		}
	}
	
	Shade( rp );
}
//===============================================================================================================================
void CustomMesh::Shade(ZShadeSandboxMesh::MeshRenderParameters mrp)
{
	// Need to shade the convex hull also (Not that important)
	ZShadeSandboxMath::XMMath4 cp = ZShadeSandboxMath::XMMath4(mrp.clipplane.x, mrp.clipplane.y, mrp.clipplane.z, mrp.clipplane.w);
	
	if (mrp.renderDeferred)
	{
		ZShadeSandboxShader::Shaders::pMaterialGBufferShader->Wireframe() = m_Wireframe;
		ZShadeSandboxShader::Shaders::pMaterialGBufferShader->Render11(
			mAttributes->mIndexCount,
			mAttributes->mInstanceCount,
			mrp,
			mMaterial
		);
		
		return;
	}
	
	// Create a shadow map of the mesh
	if (mrp.shadowMap)
	{
		ZShadeSandboxShader::Shaders::pShadowMapBuildShader->Render(mAttributes->mIndexCount, mrp.world, mrp.light->Perspective());
		return;
	}
	
	if (mMeshParameters.useCustomShader)
	{
		//
		// Adds custom functionality for shading the mesh
		//
		
		if (mMeshParameters.shader != 0)
		{
			mMeshParameters.shader->RenderFunc(mAttributes->mIndexCount, mrp, mMaterial);
		}
		else
		{
			//The shader was not defined
			ZShadeMessageCenter::MsgBoxError(NULL, "CustomMesh: Shader was not defined !!!");
		}

		return;
	}
	
	if (mrp.tessellate)
	{
		// Assign the tessellation attributes to the material
		mMaterial->bEnableDistTess = mrp.enableDistTess;
		mMaterial->fMinTessDist = mrp.minTessDist;
		mMaterial->fMaxTessDist = mrp.maxTessDist;
		mMaterial->fMinTessFactor = mrp.minTess;
		mMaterial->fMaxTessFactor = mrp.maxTess;
		mMaterial->fNoDistTessFactor = mrp.noDistTessFactor;

		switch (mrp.renderType)
		{
			case ZShadeSandboxMesh::ERenderType::e3ControlPointPatchList:
			{
				if (mrp.renderLight)
				{
					ZShadeSandboxShader::Shaders::pTriMaterialLightTessellationShader->Wireframe() = m_Wireframe;
					ZShadeSandboxShader::Shaders::pTriMaterialLightTessellationShader->Render11(
						mAttributes->mIndexCount,
						mAttributes->mInstanceCount,
						mrp,
						mMaterial
					);
				}
				else
				{
					ZShadeSandboxShader::Shaders::pTriMaterialTessellationShader->Wireframe() = m_Wireframe;
					ZShadeSandboxShader::Shaders::pTriMaterialTessellationShader->Render11(
						mAttributes->mIndexCount,
						mAttributes->mInstanceCount,
						mrp,
						mMaterial
					);
				}
			}
			break;
			case ZShadeSandboxMesh::ERenderType::e4ControlPointPatchList:
			{
				if (mrp.renderLight)
				{
					ZShadeSandboxShader::Shaders::pQuadMaterialLightTessellationShader->Wireframe() = m_Wireframe;
					ZShadeSandboxShader::Shaders::pQuadMaterialLightTessellationShader->Render11(
						mAttributes->mIndexCount,
						mAttributes->mInstanceCount,
						mrp,
						mMaterial
					);
				}
				else
				{
					ZShadeSandboxShader::Shaders::pQuadMaterialTessellationShader->Wireframe() = m_Wireframe;
					ZShadeSandboxShader::Shaders::pQuadMaterialTessellationShader->Render11(
						mAttributes->mIndexCount,
						mAttributes->mInstanceCount,
						mrp,
						mMaterial
					);
				}
			}
			break;
			default:
			{
				//The shader was not defined
				ZShadeMessageCenter::MsgBoxError(NULL, "CustomMesh: Tessellation Shader was not defined !!!");
			}
			break;
		}
	}
	else
	{
		if (mrp.renderLight)
		{
			ZShadeSandboxShader::Shaders::pMaterialLightShader->Wireframe() = m_Wireframe;
			ZShadeSandboxShader::Shaders::pMaterialLightShader->Render11(
				mAttributes->mIndexCount,
				mAttributes->mInstanceCount,
				mrp,
				mMaterial
			);
		}
		else
		{
			ZShadeSandboxShader::Shaders::pMaterialShader->Wireframe() = m_Wireframe;
			ZShadeSandboxShader::Shaders::pMaterialShader->Render11(
				mAttributes->mIndexCount,
				mAttributes->mInstanceCount,
				mrp,
				mMaterial
			);
		}
		
		//CustomMesh::pLightShader->SetWireframe(m_Wireframe);
		//CustomMesh::pLightShader->UseCustomWorld(true);
		//CustomMesh::pLightShader->SetCustomWorld(mrp.world);
		//CustomMesh::pLightShader->UseCustomView(true);
		//CustomMesh::pLightShader->SetCustomView(mrp.view);
		//CustomMesh::pLightShader->Render11(mIndexCount, cp, mrp.pCamera, mrp.dirLight, mMaterial);
	}
}
//===============================================================================================================================
/*void CustomMesh::SetBuffers(ERenderType::Type renderType)
{
	mRenderType = renderType;

	unsigned int stride;
	unsigned int offset;
	
	//Set the vertex buffer stride and offset for the vertex buffer and instance buffer
	stride = mVertexByteWidth;
	offset = 0;
	
	pD3DSystem->GetDeviceContext()->IASetVertexBuffers(0, 1, &pVB, &stride, &offset);

	if (!m_DisableIndexBuffer)
	{
		//Set the index buffer to active in the input assembler so it can be rendered
		pD3DSystem->GetDeviceContext()->IASetIndexBuffer(pIB, DXGI_FORMAT_R32_UINT, 0);
	}
	
	//Set the type of primitive that should be rendered from this vertex buffer
	switch (mRenderType)
	{
		case ZShadeSandboxMesh::ERenderType::eTriangleList: pD3DSystem->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); break;
		case ZShadeSandboxMesh::ERenderType::eTriangleStrip: pD3DSystem->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); break;
		case ZShadeSandboxMesh::ERenderType::ePointList: pD3DSystem->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST); break;
		case ZShadeSandboxMesh::ERenderType::e3ControlPointPatchList:
		{
			pD3DSystem->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
		}
		break;
		case ZShadeSandboxMesh::ERenderType::e4ControlPointPatchList:
		{
			pD3DSystem->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
		}
		break;
		default:
		{
			//The shader was not defined
			ZShadeMessageCenter::MsgBoxError(NULL, "CustomMesh: Mesh Render Type was not defined !!!");
		}
		break;
	}
}
//===============================================================================================================================
void CustomMesh::SetBuffersInstanced(ZShadeSandboxMesh::ERenderType::Type renderType)
{
	mRenderType = renderType;

	unsigned int strides[2];
	unsigned int offsets[2];
	
	//Set the vertex buffer stride and offset for the vertex buffer and instance buffer
	strides[0] = mVertexByteWidth;
	offsets[0] = 0;
	strides[1] = mInstanceByteWidth;
	offsets[1] = 0;
	
	ID3D11Buffer* buf_arr[2] = { pVB, pInstanceBuffer };
	
	pD3DSystem->GetDeviceContext()->IASetVertexBuffers(0, 2, buf_arr, strides, offsets);

	if (!m_DisableIndexBuffer)
	{
		//Set the index buffer to active in the input assembler so it can be rendered
		pD3DSystem->GetDeviceContext()->IASetIndexBuffer(pIB, DXGI_FORMAT_R32_UINT, 0);
	}
	
	//Set the type of primitive that should be rendered from this vertex buffer
	switch (mRenderType)
	{
		case ZShadeSandboxMesh::ERenderType::eTriangleList: pD3DSystem->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); break;
		case ZShadeSandboxMesh::ERenderType::eTriangleStrip: pD3DSystem->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP); break;
		case ZShadeSandboxMesh::ERenderType::ePointList: pD3DSystem->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST); break;
		case ZShadeSandboxMesh::ERenderType::e3ControlPointPatchList:
		{
			pD3DSystem->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);
		}
		break;
		case ZShadeSandboxMesh::ERenderType::e4ControlPointPatchList:
		{
			pD3DSystem->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
		}
		break;
		default:
		{
			//The shader was not defined
			ZShadeMessageCenter::MsgBoxError(NULL, "CustomMesh: Mesh Render Type was not defined !!!");
		}
		break;
	}
}
//===============================================================================================================================
XMMATRIX CustomMesh::WorldXM()
{
	if (m_TranslateBillboard)
	{
		return mBillboardWorld;
	}
	else
	{
		XMMATRIX scale, rx, ry, rz, translate;

		scale = XMMatrixScaling( mScale.x, mScale.y, mScale.z );
		rx = XMMatrixRotationX( mRotation.x );
		ry = XMMatrixRotationY( mRotation.y );
		rz = XMMatrixRotationZ( mRotation.z );
		translate = XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);

		XMMATRIX world = XMMatrixIdentity();// XMLoadFloat4x4(&mWorld);
		
		world = XMMatrixMultiply( world, scale );
		world = XMMatrixMultiply( world, rx );
		world = XMMatrixMultiply( world, ry );
		world = XMMatrixMultiply( world, rz );
		world = XMMatrixMultiply( world, translate );

		return world;
	}
}
//===============================================================================================================================
void CustomMesh::AddInstancePositions(vector<XMFLOAT3> v)
{
	bool used = false;
	
	if (mInstancePos.size() > 0)
	{
		used = true;
		mInstancePos.clear();
	}
	
	mInstanceCount = v.size();
	mInstancePos.resize(mInstanceCount);
	mInstanceByteWidth = sizeof(ZShadeSandboxMesh::InstancePos);
	
	for (int i = 0; i < mInstanceCount; i++)
	{
		mInstancePos[i].position = v[i];
	}
	
	(used) ? RegenInstanceBuffer() : CreateInstanceBuffer();
}
//===============================================================================================================================
void CustomMesh::BillboardWorldXM(Camera* camera)
{
	// Calculate the rotation that needs to be applied to the billboard model to face the current camera position using the arc tangent function.
	float angle = atan2(mPosition.x - camera->Position().x, mPosition.z - camera->Position().z) * (180.0 / PI);

	// ZShadeSandboxGlobal::Convert rotation into radians.
	float rotation = (float)angle * 0.0174532925f;

	//
	//Use the rotation to first rotate the world matrix accordingly, and then translate to the position of the billboard in the world.
	//

	// Setup the rotation the billboard at the origin using the world matrix.
	//D3DXMatrixRotationY(&worldMatrix, rotation);
	mBillboardWorld = XMMatrixRotationY( rotation );

	// Setup the translation matrix from the billboard model.
	//D3DXMatrixTranslation(&translateMatrix, modelposition.x, modelposition.y, modelposition.z);
	XMMATRIX mBillboardTrans = XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);

	//Perform a scale for the billboard matrix
	XMMATRIX scale = XMMatrixScaling( mScale.x, mScale.y, mScale.z );

	// Finally combine the rotation and translation matrices to create the final world matrix for the billboard model.
	//D3DXMatrixMultiply(&worldMatrix, &worldMatrix, &translateMatrix);
	mBillboardWorld = scale * mBillboardWorld * mBillboardTrans;
}*/
//===============================================================================================================================
void CustomMesh::AddInstancePositions(vector<XMFLOAT3> v)
{
	mAttributes->AddInstancePositions(v);
}
//===============================================================================================================================
void CustomMesh::CreateBuffers()
{
	//
	// Create Vertex Buffer
	//
	
	mAttributes->BuildVertexBuffer(mMeshParameters.vertexType);
	
	/*HRESULT result;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc, instanceBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData, instanceData;

	//Setup the description of the static vertex buffer
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = mVertexByteWidth * mVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Setup a vertex buffer for position and texture
	if (mVerticesTex.size() > 0)
	{
		// Give the subresource a pointer to the vertex data
		vertexData.pSysMem = &mVerticesTex[0];
	}

	// Setup a vertex buffer for position and color
	if (mVerticesColor.size() > 0)
	{
		// Give the subresource a pointer to the vertex data
		vertexData.pSysMem = &mVerticesColor[0];
	}

	// Setup a vertex buffer for position
	if (mVerticesPos.size() > 0)
	{
		// Give the subresource a pointer to the vertex data
		vertexData.pSysMem = &mVerticesPos[0];
	}

	//Setup a vertex buffer for position, normal, texture and tangentU
	if (mVerticesVNTT.size() > 0)
	{
		// Give the subresource a pointer to the vertex data
		vertexData.pSysMem = &mVerticesVNTT[0];
	}

	//Setup a vertex buffer for position, normal, texture and tangentU
	if (mVerticesVNT.size() > 0)
	{
		// Give the subresource a pointer to the vertex data
		vertexData.pSysMem = &mVerticesVNT[0];
	}

	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//Create the vertex buffer
	result = pD3DSystem->GetDevice11()->CreateBuffer(&vertexBufferDesc, &vertexData, &pVB);
	if (FAILED(result)) return;
	*/
	
	//
	// Create Index Buffer
	//

	if (!m_DisableIndexBuffer)
	{
		mAttributes->BuildIndexBuffer();
		
		//Setup the description of the static index buffer
		/*indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
		indexBufferDesc.ByteWidth = sizeof(UINT) * mIndexCount;
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;
		indexBufferDesc.StructureByteStride = 0;

		//Give the subresource a pointer to the index data
		indexData.pSysMem = &mIndices[0];
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;

		//Create the index buffer
		result = pD3DSystem->GetDevice11()->CreateBuffer(&indexBufferDesc, &indexData, &pIB);
		if (FAILED(result)) return;*/
	}
}
//===============================================================================================================================
void CustomMesh::CreateInstanceBuffer()
{
	mAttributes->BuildInstanceBuffer();
	
	/*if (mInstancePos.size() > 0)
	{
		//Setup the description of the instance buffer
		instanceBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		instanceBufferDesc.ByteWidth = mInstanceByteWidth * mInstanceCount;
		instanceBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		instanceBufferDesc.CPUAccessFlags = 0;
		instanceBufferDesc.MiscFlags = 0;
		instanceBufferDesc.StructureByteStride = 0;

		//Give the subresource a pointer to the instance data
		instanceData.pSysMem = &mInstancePos[0];
		instanceData.SysMemPitch = 0;
		instanceData.SysMemSlicePitch = 0;

		//Create the instance buffer
		result = pD3DSystem->GetDevice11()->CreateBuffer(&instanceBufferDesc, &instanceData, &pInstanceBuffer);
		if (FAILED(result)) return;
	}*/
}
//===============================================================================================================================
void CustomMesh::RegenVertexBuffer()
{
	mAttributes->ReBuildVertexBuffer(mMeshParameters.vertexType);
	
	/*if (mVerticesPos.size() > 0)
	{
		D3D11_MAPPED_SUBRESOURCE resource;
		pD3DSystem->GetDeviceContext()->Map(pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		memcpy(resource.pData, &mVerticesPos[0], mVertexByteWidth * mVertexCount);
		pD3DSystem->GetDeviceContext()->Unmap(pVB, 0);
	}

	if (mVerticesTex.size() > 0)
	{
		D3D11_MAPPED_SUBRESOURCE resource;
		pD3DSystem->GetDeviceContext()->Map(pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		memcpy(resource.pData, &mVerticesTex[0], mVertexByteWidth * mVertexCount);
		pD3DSystem->GetDeviceContext()->Unmap(pVB, 0);
	}

	if (mVerticesColor.size() > 0)
	{
		D3D11_MAPPED_SUBRESOURCE resource;
		pD3DSystem->GetDeviceContext()->Map(pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		memcpy(resource.pData, &mVerticesColor[0], mVertexByteWidth * mVertexCount);
		pD3DSystem->GetDeviceContext()->Unmap(pVB, 0);
	}

	if (mVerticesVNTT.size() > 0)
	{
		D3D11_MAPPED_SUBRESOURCE resource;
		pD3DSystem->GetDeviceContext()->Map(pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		memcpy(resource.pData, &mVerticesVNTT[0], mVertexByteWidth * mVertexCount);
		pD3DSystem->GetDeviceContext()->Unmap(pVB, 0);
	}

	if (mVerticesVNT.size() > 0)
	{
		D3D11_MAPPED_SUBRESOURCE resource;
		pD3DSystem->GetDeviceContext()->Map(pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		memcpy(resource.pData, &mVerticesVNT[0], mVertexByteWidth * mVertexCount);
		pD3DSystem->GetDeviceContext()->Unmap(pVB, 0);
	}*/
}
//===============================================================================================================================
void CustomMesh::RegenInstanceBuffer()
{
	mAttributes->ReBuildInstanceBuffer();
	
	/*if (mInstancePos.size() > 0)
	{
		D3D11_MAPPED_SUBRESOURCE resource;
		pD3DSystem->GetDeviceContext()->Map(pInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
		memcpy(resource.pData, &mInstancePos[0], mInstanceByteWidth * mInstanceCount);
		pD3DSystem->GetDeviceContext()->Unmap(pInstanceBuffer, 0);
	}*/
}
//===============================================================================================================================
void CustomMesh::UpdateAABB(XMFLOAT3 center, XMFLOAT3 scale)
{
	mBody->InitAABB(center, scale);
}
//===============================================================================================================================
ZShadeSandboxMath::AABB* CustomMesh::GetAABB() { return mBody->GetAABB(); }
//===============================================================================================================================
PhysicsBody* CustomMesh::GetPhysicsBody() { return mBody; }
//===============================================================================================================================
bool CustomMesh::Collides(CustomMesh* mesh)
{
	if (!mBody) return false;
	if (!mesh) return false;
	if (!mesh->GetPhysicsBody()) return false;
	return mBody->CollidesGJK(mesh->GetPhysicsBody());
}
//===============================================================================================================================
bool CustomMesh::IntersectsAABB(XMFLOAT3 point)
{
	return mBody->IntersectsAABB(point);
}
//===============================================================================================================================
float CustomMesh::DistanceAABBToPoint(XMFLOAT3 point)
{
	return mBody->DistanceAABBToPoint(point);
}
//===============================================================================================================================
void CustomMesh::CreatePhysicsBody()
{
	mBody = new PhysicsBody(mD3DSystem);
	mBody->InitAABB(mMeshParameters.pos, mMeshParameters.scale);
	
	//switch (mMeshParameters.vertexType)
	//{
	//	//case ZShadeSandboxMesh::EVertexType::Type::VT_Pos: mBody->AddPoints(mAttributes->mVerticesPos); break;
	//	//case ZShadeSandboxMesh::EVertexType::Type::VT_Color: mBody->AddPoints(mAttributes->mVerticesColor); break;
	//	case ZShadeSandboxMesh::EVertexType::Type::VT_Tex: mBody->AddPoints(mAttributes->mVerticesTex); break;
	//	//case ZShadeSandboxMesh::EVertexType::Type::VT_NormalTex: mBody->AddPoints(mAttributes->mVerticesNormalTex); break;
	//	//case ZShadeSandboxMesh::EVertexType::Type::VT_NormalTexTan: mBody->AddPoints(mAttributes->mVerticesNormalTexTan); break;
	//	//case ZShadeSandboxMesh::EVertexType::Type::VT_NormalTexTanBi: mBody->AddPoints(mAttributes->mVerticesNormalTexTanBi); break;
	//	default:
	//	{
	//		//The shader was not defined
	//		ZShadeMessageCenter::MsgBoxError(NULL, "Mesh Vertex Type was not defined !!!");
	//	}
	//	break;
	//}

	//mBody->ColorTheHull(ZShadeSandboxMath::XMMath4(1, 0, 0, 1));
	//mBody->CreateConvexHull();
}
//===============================================================================================================================
void CustomMesh::UpdatePhysicsBody()
{
	if (!mBody) return;
	if (mBody->EPhysicsType() == PhysicsType::DYNAMIC)
	{
		// Update the position of the mesh
	}
}
//===============================================================================================================================