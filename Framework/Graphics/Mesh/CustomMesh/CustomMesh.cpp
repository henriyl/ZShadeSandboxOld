#include "ZMath.h"
#include "CustomMesh.h"
#include "TextureManager.h"
#include "ZShadeMessageCenter.h"
#include "MeshUtil.h"
using ZShadeSandboxMesh::CustomMesh;
//===============================================================================================================================
MaterialShader*					CustomMesh::pMaterialShader = 0;
LightShader*					CustomMesh::pLightShader = 0;
QuadMaterialTessellationShader* CustomMesh::pQuadMaterialTessellationShader = 0;
TriMaterialTessellationShader* 	CustomMesh::pTriMaterialTessellationShader = 0;
ShadowMapBuildShader*			CustomMesh::pShadowMapBuildShader = 0;
DeferredShader*					CustomMesh::pDeferredShader = 0;
//===============================================================================================================================
CustomMesh::CustomMesh(D3D* d3d, ZShadeSandboxMesh::MeshParameters mp, char* filename)
:   pD3DSystem(d3d)
,   mPosition(mp.pos)
,   mRotation(mp.rot)
,   mScale(mp.scale)
,   m_Wireframe(false)
,   m_DisableIndexBuffer(false)
,   m_TranslateBillboard(false)
,   m_RenderShader(true)
,   mTriangleCount(0)
,   mMeshParameters(mp)
{
	pEngineOptions = d3d->GetEngineOptions();
	
	// Create the shaders only once if they do not exist
	if (CustomMesh::pMaterialShader == 0
	&&	CustomMesh::pLightShader == 0
	&&	CustomMesh::pQuadMaterialTessellationShader == 0
	&&	CustomMesh::pTriMaterialTessellationShader == 0
	&&	CustomMesh::pShadowMapBuildShader == 0
	&&	CustomMesh::pDeferredShader == 0)
	{
		CustomMesh::pMaterialShader = new MaterialShader(d3d);
		CustomMesh::pLightShader = new LightShader(d3d);
		CustomMesh::pQuadMaterialTessellationShader = new QuadMaterialTessellationShader(d3d);
		CustomMesh::pTriMaterialTessellationShader = new TriMaterialTessellationShader(d3d);
		CustomMesh::pShadowMapBuildShader = new ShadowMapBuildShader(d3d);
		CustomMesh::pDeferredShader = new DeferredShader(d3d);
	}

	LoadFromFile(filename);

	CreateBuffers();

	mMaterial = mp.material;

	//Load the world matrix
	XMStoreFloat4x4(&mWorld, XMMatrixIdentity());

	CreatePhysicsBody();
}
//===============================================================================================================================
CustomMesh::CustomMesh(D3D* d3d, ZShadeSandboxMesh::MeshParameters mp)
:	pD3DSystem(d3d)
,	mPosition(mp.pos)
,	mRotation(mp.rot)
,	mScale(mp.scale)
,	m_Wireframe(false)
,	m_DisableIndexBuffer(false)
,	m_TranslateBillboard(false)
,   m_RenderShader(true)
,   mTriangleCount(0)
,   mMeshParameters(mp)
{
	pEngineOptions = d3d->GetEngineOptions();
	
	// Create the shaders only once if they do not exist
	if (CustomMesh::pMaterialShader == 0
	&&	CustomMesh::pLightShader == 0
	&&	CustomMesh::pQuadMaterialTessellationShader == 0
	&&	CustomMesh::pTriMaterialTessellationShader == 0
	&&	CustomMesh::pShadowMapBuildShader == 0
	&&	CustomMesh::pDeferredShader == 0)
	{
		CustomMesh::pMaterialShader = new MaterialShader(d3d);
		CustomMesh::pLightShader = new LightShader(d3d);
		CustomMesh::pQuadMaterialTessellationShader = new QuadMaterialTessellationShader(d3d);
		CustomMesh::pTriMaterialTessellationShader = new TriMaterialTessellationShader(d3d);
		CustomMesh::pShadowMapBuildShader = new ShadowMapBuildShader(d3d);
		CustomMesh::pDeferredShader = new DeferredShader(d3d);
	}

	mMaterial = mp.material;
	
	//Load the world matrix
	XMStoreFloat4x4(&mWorld, XMMatrixIdentity());
	
	CreatePhysicsBody();
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

	mVertexCount = md->vertexCount;
	mIndexCount = md->indexCount;
	mIndices.resize(mIndexCount);

	mTriangleCount = mVertexCount / 3;

	switch (mMeshParameters.vertexType)
	{
		case ZShadeSandboxMesh::EVertexType::VT_NormalTex:
		{
			mVerticesVNT.resize(mVertexCount);
			mVertexByteWidth = sizeof(ZShadeSandboxMesh::VertexNormalTex);

			// Load the vertex array and index array with data.
			for (int i = 0; i < mVertexCount; i++)
			{
				mVerticesVNT[i].position = md->dataVNT[i].position;
				mVerticesVNT[i].normal = md->dataVNT[i].normal;
				mVerticesVNT[i].texture = md->dataVNT[i].texture;
				mIndices[i] = i;
			}
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_NormalTexTan:
		{
			mVerticesVNTT.resize(mVertexCount);
			mVertexByteWidth = sizeof(ZShadeSandboxMesh::VertexNormalTexTan);

			// Load the vertex array and index array with data.
			for (int i = 0; i < mVertexCount; i++)
			{
				mVerticesVNTT[i].position = md->dataVNTT[i].position;
				mVerticesVNTT[i].normal = md->dataVNTT[i].normal;
				mVerticesVNTT[i].texture = md->dataVNTT[i].texture;
				mVerticesVNTT[i].tangentU = md->dataVNTT[i].tangentU;
				mIndices[i] = i;
			}
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Pos:
		{
			mVerticesPos.resize(mVertexCount);
			mVertexByteWidth = sizeof(ZShadeSandboxMesh::VertexPos);

			// Load the vertex array and index array with data.
			for (int i = 0; i < mVertexCount; i++)
			{
				mVerticesPos[i].position = md->dataPos[i].position;
				mIndices[i] = i;
			}
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Tex:
		{
			mVerticesTex.resize(mVertexCount);
			mVertexByteWidth = sizeof(ZShadeSandboxMesh::VertexTex);

			// Load the vertex array and index array with data.
			for (int i = 0; i < mVertexCount; i++)
			{
				mVerticesTex[i].position = md->dataTex[i].position;
				mVerticesTex[i].texture = md->dataTex[i].texture;
				mIndices[i] = i;
			}
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Color:
		{
			mVerticesColor.resize(mVertexCount);
			mVertexByteWidth = sizeof(ZShadeSandboxMesh::VertexColor);

			// Load the vertex array and index array with data.
			for (int i = 0; i < mVertexCount; i++)
			{
				mVerticesColor[i].position = md->dataColor[i].position;
				mVerticesColor[i].color = md->dataColor[i].color;
				mIndices[i] = i;
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
void CustomMesh::ReplyMeshIndices(vector<UINT> ind)
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
}
//===============================================================================================================================
void CustomMesh::Render(ZShadeSandboxMesh::MeshRenderParameters rp)
{
	// Cannot do anything if there is no camera
	if (rp.pCamera == 0) return;
	
	//
	// Render the mesh buffers
	//

	SetBuffers(rp.renderType);
	
	//
	// Render the shader
	//
	
	if (rp.bReflection)
		rp.view = rp.pCamera->ReflectionView4x4();
	else
		rp.view = rp.pCamera->View4x4();
	
	if (!rp.bSpecifyWorld)
	{
		// Center the camera in this mesh
		if (rp.bCenterCam)
		{
			XMMATRIX scale, rx, ry, rz, translate;

			scale = XMMatrixScaling(mScale.x, mScale.y, mScale.z);
			rx = XMMatrixRotationX(mRotation.x);
			ry = XMMatrixRotationY(mRotation.y);
			rz = XMMatrixRotationZ(mRotation.z);

			//Get the position of the camera
			XMFLOAT3 eyeVertex = rp.pCamera->Position();

			if (rp.bReflection)
			{
				//Invert the Y coordinate of the camera around the water plane height for the reflected camera position
				eyeVertex.y = -eyeVertex.y + (rp.fplaneHeight * 2.0f);
			}
			
			translate = XMMatrixTranslation(eyeVertex.x, eyeVertex.y, eyeVertex.z);

			XMMATRIX world = XMLoadFloat4x4(&mWorld);

			world = XMMatrixMultiply(world, scale);
			world = XMMatrixMultiply(world, rx);
			world = XMMatrixMultiply(world, ry);
			world = XMMatrixMultiply(world, rz);
			world = XMMatrixMultiply(world, translate);
			
			rp.world = world;
		}
		else
		{
			rp.world = WorldXM();
		}
	}
	
	Shade( rp );
}
//===============================================================================================================================
void CustomMesh::Update(float dt)
{
	if (mMeshParameters.rotationAxisX)
		mRotation.x += dt;
	
	if (mMeshParameters.rotationAxisY)
		mRotation.y += dt;
	
	if (mMeshParameters.rotationAxisZ)
		mRotation.z += dt;
}
//===============================================================================================================================
void CustomMesh::Shade(ZShadeSandboxMesh::MeshRenderParameters mrp)
{
	// Need to shade the convex hull also (Not that important)
	ZShadeSandboxMath::XMMath4 cp = ZShadeSandboxMath::XMMath4(mrp.clipplane.x, mrp.clipplane.y, mrp.clipplane.z, mrp.clipplane.w);
	
	if (mrp.bRenderDeferred)
	{
		ID3D11ShaderResourceView* diffusemap = 0;
		
		for (int i = 0; i < mMaterial->TextureCount(); i++)
		{
			switch (mMaterial->GetMaterialTextureType(i))
			{
				case ZShadeSandboxLighting::EMaterialTextureType::eDiffuse:
					diffusemap = mMaterial->GetTexture(i)->getTexture11();
				break;
			}
		}
		
		CustomMesh::pDeferredShader->SetWireframe(m_Wireframe);
		CustomMesh::pDeferredShader->Render11(
			mIndexCount,
			mrp.pCamera,
			mrp.world,
			XMFLOAT2(10, 300), // Add this to the material
			mMaterial->SpecularIntensity(),
			mMaterial->SpecularPower(),
			diffusemap
		);
		
		return;
	}
	
	// Create a shadow map of the mesh
	if (mrp.bShadowMap)
	{
		CustomMesh::pShadowMapBuildShader->Render(mIndexCount, mrp.world, mrp.pLightCamera);
		return;
	}
	
	// Get the material information
	
	/*XMFLOAT3 materialDiffuseColor = mMaterial->DiffuseColor();
	XMFLOAT3 materialAmbientColor = mMaterial->AmbientColor();
	float materialSpecularPower = mMaterial->SpecularPower();
	float materialSpecularIntensity = mMaterial->SpecularIntensity();
	
	ID3D11ShaderResourceView* diffuseArray = 0;
	ID3D11ShaderResourceView* diffusemap = 0;
	ID3D11ShaderResourceView* normalmap = 0;
	ID3D11ShaderResourceView* blendmap = 0;
	ID3D11ShaderResourceView* detailmap = 0;
	ID3D11ShaderResourceView* alphamap = 0;
	
	for (int i = 0; i < mMaterial->TextureCount(); i++)
	{
		switch (mMaterial->GetMaterialTextureType(i))
		{
			case ZShadeSandboxLighting::EMaterialTextureType::eDiffuseArray:
				diffuseArray = mMaterial->GetTexture(i)->getTexture11();
			break;
			case ZShadeSandboxLighting::EMaterialTextureType::eDiffuse:
				diffusemap = mMaterial->GetTexture(i)->getTexture11();
			break;
			case ZShadeSandboxLighting::EMaterialTextureType::eNormal:
				normalmap = mMaterial->GetTexture(i)->getTexture11();
			break;
			case ZShadeSandboxLighting::EMaterialTextureType::eBlend:
				blendmap = mMaterial->GetTexture(i)->getTexture11();
			break;
			case ZShadeSandboxLighting::EMaterialTextureType::eDetail:
				detailmap = mMaterial->GetTexture(i)->getTexture11();
			break;
			case ZShadeSandboxLighting::EMaterialTextureType::eAlpha:
				alphamap = mMaterial->GetTexture(i)->getTexture11();
			break;
		}
	}
	
	// Same as texture shader but has a blend amount
	if (mrp.bTransparent && (mMeshParameters.vertexType == ZShadeSandboxMesh::EVertexType::VT_Tex))
	{
		//CustomMesh::pTransparentShader->SetWireframe(m_Wireframe);
		//CustomMesh::pTransparentShader->UseCustomWorld(true);
		//CustomMesh::pTransparentShader->SetCustomWorld(mrp.world);
		//CustomMesh::pTransparentShader->UseCustomView(true);
		//CustomMesh::pTransparentShader->SetCustomView(mrp.view);
		//CustomMesh::pTransparentShader->Render11(mIndexCount, cp, mrp.pCamera, diffuseTexture, mrp.blendAmount);
		
		CustomMesh::pMaterialShader->SetWireframe(m_Wireframe);
		CustomMesh::pMaterialShader->UseCustomWorld(true);
		CustomMesh::pMaterialShader->SetCustomWorld(mrp.world);
		CustomMesh::pMaterialShader->UseCustomView(true);
		CustomMesh::pMaterialShader->SetCustomView(mrp.view);
		CustomMesh::pMaterialShader->Render11(mIndexCount, cp, mrp.pCamera, mMaterial);
		return;
	}*/
	
	if (mMeshParameters.useCustomShader)
	{
		//
		// Adds custom functionality for shading the mesh
		//
		
		if (mMeshParameters.shader != 0)
		{
			mMeshParameters.shader->RenderFunc(mIndexCount, mrp, mMaterial);
			//if (mMeshParameters.useTexture)
			//{
			//	mMeshParameters.shader->RenderFunc(mIndexCount, mrp, mMaterial);
			//}
			//else
			//{
			//	mMeshParameters.shader->RenderFunc(mIndexCount, mrp);
			//}
		}
		else
		{
			//The shader was not defined
			ZShadeMessageCenter::MsgBoxError(NULL, "CustomMesh: Shader was not defined !!!");
		}

		return;
	}
	
	if (mrp.bTessellate)
	{
		switch (mrp.renderType)
		{
			case ZShadeSandboxMesh::ERenderType::e3ControlPointPatchList:
			{
				CustomMesh::pTriMaterialTessellationShader->SetWireframe(m_Wireframe);
				CustomMesh::pTriMaterialTessellationShader->UseCustomWorld(true);
				CustomMesh::pTriMaterialTessellationShader->SetCustomWorld(mrp.world);
				CustomMesh::pTriMaterialTessellationShader->UseCustomView(true);
				CustomMesh::pTriMaterialTessellationShader->SetCustomView(mrp.view);
				CustomMesh::pTriMaterialTessellationShader->Render11(mIndexCount, mrp.clipplane, mrp.pCamera, mrp.fTessellationFactor, mMaterial);
			}
			break;
			case ZShadeSandboxMesh::ERenderType::e4ControlPointPatchList:
			{
				CustomMesh::pQuadMaterialTessellationShader->SetWireframe(m_Wireframe);
				CustomMesh::pQuadMaterialTessellationShader->UseCustomWorld(true);
				CustomMesh::pQuadMaterialTessellationShader->SetCustomWorld(mrp.world);
				CustomMesh::pQuadMaterialTessellationShader->UseCustomView(true);
				CustomMesh::pQuadMaterialTessellationShader->SetCustomView(mrp.view);
				CustomMesh::pQuadMaterialTessellationShader->Render11(mIndexCount, mrp.clipplane, mrp.pCamera, mrp.fTessellationFactor, mMaterial);
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
		CustomMesh::pMaterialShader->SetWireframe(m_Wireframe);
		CustomMesh::pMaterialShader->UseCustomWorld(true);
		CustomMesh::pMaterialShader->SetCustomWorld(mrp.world);
		CustomMesh::pMaterialShader->UseCustomView(true);
		CustomMesh::pMaterialShader->SetCustomView(mrp.view);
		CustomMesh::pMaterialShader->Render11(mIndexCount, cp, mrp.pCamera, mrp.pLightCamera, mMaterial);

		/*CustomMesh::pLightShader->SetWireframe(m_Wireframe);
		CustomMesh::pLightShader->UseCustomWorld(true);
		CustomMesh::pLightShader->SetCustomWorld(mrp.world);
		CustomMesh::pLightShader->UseCustomView(true);
		CustomMesh::pLightShader->SetCustomView(mrp.view);
		CustomMesh::pLightShader->Render11(mIndexCount, cp, mrp.pCamera, mrp.dirLight, mMaterial);*/
	}
	
	/*switch (mMeshParameters.vertexType)
	{
		case ZShadeSandboxMesh::EVertexType::VT_Color:
		{
			if (mrp.bTessellate)
			{
				switch (mrp.renderType)
				{
					case ZShadeSandboxMesh::ERenderType::e3ControlPointPatchList:
					{
						CustomMesh::pTriColorTessellationShader->SetWireframe(m_Wireframe);
						CustomMesh::pTriColorTessellationShader->UseCustomWorld(true);
						CustomMesh::pTriColorTessellationShader->SetCustomWorld(mrp.world);
						CustomMesh::pTriColorTessellationShader->UseCustomView(true);
						CustomMesh::pTriColorTessellationShader->SetCustomView(mrp.view);
						CustomMesh::pTriColorTessellationShader->Render(mIndexCount, mrp.pCamera, mrp.fTessellationFactor);
					}
					break;
					case ZShadeSandboxMesh::ERenderType::e4ControlPointPatchList:
					{
						CustomMesh::pQuadColorTessellationShader->SetWireframe(m_Wireframe);
						CustomMesh::pQuadColorTessellationShader->UseCustomWorld(true);
						CustomMesh::pQuadColorTessellationShader->SetCustomWorld(mrp.world);
						CustomMesh::pQuadColorTessellationShader->UseCustomView(true);
						CustomMesh::pQuadColorTessellationShader->SetCustomView(mrp.view);
						CustomMesh::pQuadColorTessellationShader->Render(mIndexCount, mrp.pCamera, mrp.fTessellationFactor);
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
				//CustomMesh::pColorShader->SetWireframe(m_Wireframe);
				//CustomMesh::pColorShader->UseCustomWorld(true);
				//CustomMesh::pColorShader->SetCustomWorld(mrp.world);
				//CustomMesh::pColorShader->UseCustomView(true);
				//CustomMesh::pColorShader->SetCustomView(mrp.view);
				//CustomMesh::pColorShader->Render11(mIndexCount, cp, mrp.pCamera);
			}
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Tex:
		{
			if (mrp.bTessellate)
			{
				switch (mrp.renderType)
				{
					case ZShadeSandboxMesh::ERenderType::e3ControlPointPatchList:
					{
						CustomMesh::pTriTextureTessellationShader->SetWireframe(m_Wireframe);
						CustomMesh::pTriTextureTessellationShader->UseCustomWorld(true);
						CustomMesh::pTriTextureTessellationShader->SetCustomWorld(mrp.world);
						CustomMesh::pTriTextureTessellationShader->UseCustomView(true);
						CustomMesh::pTriTextureTessellationShader->SetCustomView(mrp.view);
						CustomMesh::pTriTextureTessellationShader->Render(mIndexCount, mrp.pCamera, mrp.fTessellationFactor,
							diffusemap);
					}
					break;
					case ZShadeSandboxMesh::ERenderType::e4ControlPointPatchList:
					{
						CustomMesh::pQuadTextureTessellationShader->SetWireframe(m_Wireframe);
						CustomMesh::pQuadTextureTessellationShader->UseCustomWorld(true);
						CustomMesh::pQuadTextureTessellationShader->SetCustomWorld(mrp.world);
						CustomMesh::pQuadTextureTessellationShader->UseCustomView(true);
						CustomMesh::pQuadTextureTessellationShader->SetCustomView(mrp.view);
						CustomMesh::pQuadTextureTessellationShader->Render(mIndexCount, mrp.pCamera, mrp.fTessellationFactor,
							diffusemap);
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
				//CustomMesh::pTextureShader->SetWireframe(m_Wireframe);
				//CustomMesh::pTextureShader->UseCustomWorld(true);
				//CustomMesh::pTextureShader->SetCustomWorld(mrp.world);
				//CustomMesh::pTextureShader->UseCustomView(true);
				//CustomMesh::pTextureShader->SetCustomView(mrp.view);
				//CustomMesh::pTextureShader->Render11(mIndexCount, cp, mrp.pCamera, diffuseTexture);
			}
		}
		break;
		default:
		{
			//The shader was not defined
			ZShadeMessageCenter::MsgBoxError(NULL, "CustomMesh: Shader was not defined !!!");
		}
		break;
	}*/
}
//===============================================================================================================================
void CustomMesh::SetBuffers(ZShadeSandboxMesh::ERenderType::Type renderType)
{
	mRenderType = renderType;

	unsigned int stride;
	unsigned int offset;

	//Set the vertex buffer stride and offset
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
void CustomMesh::SetLightBuffer(ZShadeSandboxLighting::LightBuffer* lb)
{
	mMaterial->SetLightBuffer(lb);
}
//===============================================================================================================================
void CustomMesh::SetLightBuffer(ZShadeSandboxLighting::SunLightBuffer* slb)
{
	mMaterial->SetLightBuffer(slb);
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
}
//===============================================================================================================================
void CustomMesh::CreateBuffers()
{
	//
	// Create Vertex Buffer
	//

	HRESULT result;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

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

	//
	// Create Index Buffer
	//

	if (!m_DisableIndexBuffer)
	{
		mIndexCount = mIndices.size();

		//Setup the description of the static index buffer
		indexBufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
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
		if (FAILED(result)) return;
	}
}
//===============================================================================================================================
void CustomMesh::RegenVertexBuffer()
{
	if (mVerticesPos.size() > 0)
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
	}
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
	mBody = new PhysicsBody(pD3DSystem);
	mBody->InitAABB(mMeshParameters.pos, mMeshParameters.scale);
	mBody->AddPoints(mVerticesTex);
	mBody->ColorTheHull(ZShadeSandboxMath::XMMath4(1, 0, 0, 1));
	mBody->CreateConvexHull();
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