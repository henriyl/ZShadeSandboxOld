#include "GridMesh.h"
#include <algorithm>
using ZShadeSandboxMesh::GridMesh;
//===============================================================================================================================
GridMesh::GridMesh(float width, float height, float depth, D3D* d3d, ZShadeSandboxMesh::MeshParameters mp)
:   ZShadeSandboxMesh::CustomMesh( d3d, mp )
,   fWidth(width)
,   fHeight(height)
,   fDepth(depth)
{
	fScreenWidth = pEngineOptions->m_screenWidth;
	fScreenHeight = pEngineOptions->m_screenHeight;
	
	mMeshType = ZShadeSandboxMesh::EMeshType::GRID;

	Initialize();
	
	CreateBuffers();
}
//===============================================================================================================================
GridMesh::~GridMesh()
{
}
//===============================================================================================================================
void GridMesh::Initialize()
{
	UINT vertexCount = fWidth * fHeight;
	UINT faceCount = (fWidth - 1) * (fHeight - 1) * 2;
	
	float aspectRatio = fScreenWidth / fScreenHeight * 1.55f;
	
	float halfWidth = 0.5f * fWidth;
	float halfDepth = 0.5f * fDepth;
	
	float dx = fWidth / (fWidth - 1);
	float dz = fDepth / (fHeight - 1);
	
	float du = 1.0f / (fWidth - 1);
	float dv = 1.0f / (fHeight - 1);
	
	//
	// Load Vertices
	//
	
	mVertexCount = vertexCount;
	mTriangleCount = mVertexCount / 3;
	
	switch (mMeshParameters.vertexType)
	{
		case ZShadeSandboxMesh::EVertexType::VT_NormalTex:
		{
			mVerticesVNT.resize(mVertexCount);
			
			for (UINT i = 0; i < fHeight; ++i)
			{
				float z = -halfDepth - i * dz;
				
				for (UINT j = 0; j < fWidth; ++j)
				{
					float x = -halfWidth + j * dx;
					
					mVerticesVNT[i * fWidth + j].position = XMFLOAT3(x, 0, z);
					mVerticesVNT[i * fWidth + j].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
					
					// Stretch the texture over the grid
					mVerticesVNT[i * fWidth + j].texture.x = j * du;
					mVerticesVNT[i * fWidth + j].texture.y = i * du;
				}
			}

			mVertexByteWidth = sizeof(ZShadeSandboxMesh::VertexNormalTex);
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_NormalTexTan:
		{
			mVerticesVNTT.resize(mVertexCount);
			
			for (UINT i = 0; i < fHeight; ++i)
			{
				float z = -halfDepth - i * dz;
				
				for (UINT j = 0; j < fWidth; ++j)
				{
					float x = -halfWidth + j * dx;
					
					mVerticesVNTT[i * fWidth + j].position = XMFLOAT3(x, 0, z);
					mVerticesVNTT[i * fWidth + j].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
					mVerticesVNTT[i * fWidth + j].tangentU = XMFLOAT3(1.0f, 0.0f, 0.0f);
					
					// Stretch the texture over the grid
					mVerticesVNTT[i * fWidth + j].texture.x = j * du;
					mVerticesVNTT[i * fWidth + j].texture.y = i * du;
				}
			}

			mVertexByteWidth = sizeof(ZShadeSandboxMesh::VertexNormalTexTan);
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Pos:
		{
			mVerticesPos.resize(mVertexCount);
			
			for (UINT i = 0; i < fHeight; ++i)
			{
				float z = -halfDepth - i * dz;
				
				for (UINT j = 0; j < fWidth; ++j)
				{
					float x = -halfWidth + j * dx;
					
					mVerticesPos[i * fWidth + j].position = XMFLOAT3(x, 0, z);
				}
			}

			mVertexByteWidth = sizeof(ZShadeSandboxMesh::VertexPos);
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Tex:
		{
			mVerticesTex.resize(mVertexCount);
			
			for (UINT i = 0; i < fHeight; ++i)
			{
				float z = -halfDepth - i * dz;
				
				for (UINT j = 0; j < fWidth; ++j)
				{
					float x = -halfWidth + j * dx;
					
					mVerticesTex[i * fWidth + j].position = XMFLOAT3(x, 0, z);
					
					// Stretch the texture over the grid
					mVerticesTex[i * fWidth + j].texture.x = j * du;
					mVerticesTex[i * fWidth + j].texture.y = i * du;
				}
			}

			mVertexByteWidth = sizeof(ZShadeSandboxMesh::VertexTex);
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Color:
		{
			XMFLOAT4 diffuseColor = mMaterial->DiffuseColor();

			mVerticesColor.resize(mVertexCount);
			
			for (UINT i = 0; i < fHeight; ++i)
			{
				float z = -halfDepth - i * dz;
				
				for (UINT j = 0; j < fWidth; ++j)
				{
					float x = -halfWidth + j * dx;
					
					mVerticesColor[i * fWidth + j].position = XMFLOAT3(x, 0, z);
					mVerticesColor[i * fWidth + j].color = diffuseColor;
				}
			}

			mVertexByteWidth = sizeof(ZShadeSandboxMesh::VertexColor);
		}
		break;
	}
	
	//
	// Load Indices
	//
	
	mIndexCount = faceCount * 3;
	mIndices.resize( mIndexCount );
	
	// Iterate over each quad and compute indices.
	UINT k = 0;
	
	for (UINT i = 0; i < fHeight - 1; ++i)
	{
		for (UINT j = 0; j < fWidth - 1; ++j)
		{
			mIndices[ k ]     = i * fWidth + j;
			mIndices[ k + 1 ] = i * fWidth + j + 1;
			mIndices[ k + 2 ] = (i + 1) * fWidth + j;
			
			mIndices[ k + 3 ] = (i + 1) * fWidth + j;
			mIndices[ k + 4 ] = i * fWidth + j + 1;
			mIndices[ k + 5 ] = (i + 1) * fWidth + j + 1;

			k += 6; // next quad
		}
	}
}
//===============================================================================================================================
