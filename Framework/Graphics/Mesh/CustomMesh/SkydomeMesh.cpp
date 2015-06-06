#include "SkydomeMesh.h"
#include "TextureManager.h"
using ZShadeSandboxMesh::SkydomeMesh;
//===============================================================================================================================
SkydomeMesh::SkydomeMesh(D3D* d3d, ZShadeSandboxMesh::MeshParameters mp)
:   ZShadeSandboxMesh::CustomMesh(d3d, mp)
{
	mMeshType = ZShadeSandboxMesh::EMeshType::CUSTOM;
	Initialize();
	CreateBuffers();
}
//===============================================================================================================================
SkydomeMesh::~SkydomeMesh()
{
}
//===============================================================================================================================
void SkydomeMesh::Initialize()
{
	MeshData* md = MeshUtil::LoadMesh("Models\\skydome.txt", mMeshParameters.vertexType);
	
	mVertexCount = md->vertexCount;
	mIndexCount = md->indexCount;
	mIndices.resize(mIndexCount);
	
	mTriangleCount = mVertexCount / 3;
	
	switch (mMeshParameters.vertexType)
	{
		case ZShadeSandboxMesh::EVertexType::VT_NormalTex:
		{
			mVerticesVNT.resize(mVertexCount);
			mVertexByteWidth = sizeof( ZShadeSandboxMesh::VertexNormalTex );
			
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
			mVertexByteWidth = sizeof( ZShadeSandboxMesh::VertexNormalTexTan );
			
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
			mVertexByteWidth = sizeof( ZShadeSandboxMesh::VertexPos );
			
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
			mVertexByteWidth = sizeof( ZShadeSandboxMesh::VertexTex );
			
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
			mVertexByteWidth = sizeof( ZShadeSandboxMesh::VertexColor );
			
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
