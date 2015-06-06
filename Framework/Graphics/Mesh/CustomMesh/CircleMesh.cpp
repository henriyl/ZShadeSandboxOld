#include "CircleMesh.h"
#include <algorithm>
using ZShadeSandboxMesh::CircleMesh;
//===============================================================================================================================
CircleMesh::CircleMesh(D3D* d3d, float radius, ZShadeSandboxMesh::MeshParameters mp)
:   ZShadeSandboxMesh::CustomMesh( d3d, mp )
,   fRadius(radius)
{
	mMeshType = ZShadeSandboxMesh::EMeshType::CIRCLE;

	Initialize();
	//LoadVertexAdjacency();
	CreateBuffers();
}
//===============================================================================================================================
CircleMesh::~CircleMesh()
{
}
//===============================================================================================================================
void CircleMesh::Initialize()
{
	//
	// Load Vertices
	//
	
	float iHeight = 1;
	float iStackCount = 1;
	float iSliceCount = 32;

	float stackHeight = iHeight / iStackCount;
	
	uint32 ringCount = iStackCount + 1;
	
	mVertexCount = ringCount;
	
	mTriangleCount = mVertexCount / 3;
	
	uint32 baseIndex = (uint32)0;

	float y = 0.5f * iHeight;
	float dTheta = 2.0f * PI / iSliceCount;
	
	switch (mMeshParameters.vertexType)
	{
		case ZShadeSandboxMesh::EVertexType::VT_NormalTex:
		{
#pragma region "Normal Tex"
			mVerticesVNT.resize(mVertexCount);
			mVertexByteWidth = sizeof( ZShadeSandboxMesh::VertexNormalTex );
			
			// Duplicate cap ring vertices because the texture coordinates and normals differ.
			for(uint32 i = 0; i <= iSliceCount; ++i)
			{
				float x = fRadius * cosf(i * dTheta);
				float z = fRadius * sinf(i * dTheta);

				// Scale down by the height to try and make top cap texture coord area
				// proportional to base.
				float u = x / iHeight + 0.5f;
				float v = z / iHeight + 0.5f;

				mVerticesVNT.push_back(ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(x, y, z, 0.0f, 1.0f, 0.0f, u, v));
			}

			// Cap center vertex.
			mVerticesVNT.push_back(ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(0.0f, y, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f));
#pragma endregion
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_NormalTexTan:
		{
#pragma region "Normal Tex Tan"
			mVerticesVNTT.resize(mVertexCount);
			mVertexByteWidth = sizeof( ZShadeSandboxMesh::VertexNormalTexTan );
			
			// Duplicate cap ring vertices because the texture coordinates and normals differ.
			for(uint32 i = 0; i <= iSliceCount; ++i)
			{
				float x = fRadius * cosf(i * dTheta);
				float z = fRadius * sinf(i * dTheta);

				// Scale down by the height to try and make top cap texture coord area
				// proportional to base.
				float u = x / iHeight + 0.5f;
				float v = z / iHeight + 0.5f;

				mVerticesVNTT.push_back(ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(x, y, z, 0.0f, 1.0f, 0.0f, u, v, 0.0f, 0.0f, 0.0f));
			}

			// Cap center vertex.
			mVerticesVNTT.push_back(ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(0.0f, y, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f));
#pragma endregion
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Pos:
		{
#pragma region "Normal Pos"
			mVerticesPos.resize(mVertexCount);
			mVertexByteWidth = sizeof( ZShadeSandboxMesh::VertexPos );
			
			// Duplicate cap ring vertices because the texture coordinates and normals differ.
			for(uint32 i = 0; i <= iSliceCount; ++i)
			{
				float x = fRadius * cosf(i * dTheta);
				float z = fRadius * sinf(i * dTheta);

				mVerticesPos.push_back(ZShadeSandboxMesh::VertexUtil::LoadVertexPos(x, y, z));
			}

			// Cap center vertex.
			mVerticesPos.push_back(ZShadeSandboxMesh::VertexUtil::LoadVertexPos(0.0f, y, 0.0f));
#pragma endregion
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Tex:
		{
#pragma region "Normal Tex"
			//mVerticesTex.resize(mVertexCount);
			mVertexByteWidth = sizeof( ZShadeSandboxMesh::VertexTex );
			
			// Duplicate cap ring vertices because the texture coordinates and normals differ.
			for(uint32 i = 0; i <= iSliceCount; ++i)
			{
				float x = fRadius * cosf(i * dTheta);
				float z = fRadius * sinf(i * dTheta);

				// Scale down by the height to try and make top cap texture coord area
				// proportional to base.
				float u = x / iHeight + 0.5f;
				float v = z / iHeight + 0.5f;

				mVerticesTex.push_back(ZShadeSandboxMesh::VertexUtil::LoadVertexTex(x, y, z, u, v));
			}

			// Cap center vertex.
			mVerticesTex.push_back(ZShadeSandboxMesh::VertexUtil::LoadVertexTex(0.0f, y, 0.0f, 0.5f, 0.5f));

			mVertexCount = mVerticesTex.size();
#pragma endregion
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Color:
		{
#pragma region "Normal Color"
			XMFLOAT4 diffuseColor = mMaterial->DiffuseColor();

			mVerticesColor.resize(mVertexCount);
			mVertexByteWidth = sizeof( ZShadeSandboxMesh::VertexColor );
			
			// Duplicate cap ring vertices because the texture coordinates and normals differ.
			for(uint32 i = 0; i <= iSliceCount; ++i)
			{
				float x = fRadius * cosf(i * dTheta);
				float z = fRadius * sinf(i * dTheta);

				mVerticesColor.push_back(ZShadeSandboxMesh::VertexUtil::LoadVertexColor(x, y, z, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w));
			}

			// Cap center vertex.
			mVerticesColor.push_back(ZShadeSandboxMesh::VertexUtil::LoadVertexColor(0.0f, y, 0.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w));
#pragma endregion
		}
		break;
	}
	
	//
	// Load Indices
	//
	
	//mIndexCount = (iStackCount * iSliceCount) * 3;
	
	//mIndices.resize( mIndexCount );
	
	// Index of center vertex.
	uint32 centerIndex = (uint32)mVerticesTex.size() - 1;

	uint32 index = 0;
	for(uint32 i = 0; i < iSliceCount; ++i)
	{
		mIndices.push_back(centerIndex);
		mIndices.push_back(baseIndex + i + 1);
		mIndices.push_back(baseIndex + i);
	}

	mIndexCount = mIndices.size();
}
//===============================================================================================================================