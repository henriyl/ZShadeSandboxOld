#include "TriangleMesh.h"
#include <algorithm>
using ZShadeSandboxMesh::TriangleMesh;
//===============================================================================================================================
TriangleMesh::TriangleMesh(D3D* d3d, ZShadeSandboxMesh::MeshParameters mp)
:   ZShadeSandboxMesh::CustomMesh( d3d, mp )
{
	mMeshType = ZShadeSandboxMesh::EMeshType::TRI;

	Initialize();
	CreateBuffers();
}
//===============================================================================================================================
TriangleMesh::~TriangleMesh()
{
}
//===============================================================================================================================
void TriangleMesh::Initialize()
{
	const static int numVerts = 3;
	const static int numInd = 3;
	
	// Sample Adjacency information for the mesh
	//verts[0].next = verts[1];
	//verts[0].prev = verts[2];
	
	//verts[1].next = verts[0];
	//verts[1].prev = verts[2];
	
	//verts[2].next = verts[1];
	//verts[2].prev = verts[0];
	
	//
	// Load Vertices
	//
	
	switch (mMeshParameters.vertexType)
	{
		case ZShadeSandboxMesh::EVertexType::VT_NormalTex:
		{
			ZShadeSandboxMesh::VertexNormalTex t1 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTex t2 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTex t3 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
			
			ZShadeSandboxMesh::VertexNormalTex verts[numVerts] =
			{
				t1, t2, t3
			};
			
			mVerticesVNT.resize( numVerts );
			
			for (int i = 0; i < numVerts; i++)
			{
				mVerticesVNT[i] = verts[i];
			}
			
			mVertexByteWidth = sizeof(ZShadeSandboxMesh::VertexNormalTex);
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_NormalTexTan:
		{
			ZShadeSandboxMesh::VertexNormalTexTan t1 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTexTan t2 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexNormalTexTan t3 = ZShadeSandboxMesh::VertexUtil::LoadVertexNormalTexTan(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
			
			ZShadeSandboxMesh::VertexNormalTexTan verts[numVerts] =
			{
				t1, t2, t3
			};
			
			mVerticesVNTT.resize( numVerts );
			
			for (int i = 0; i < numVerts; i++)
			{
				mVerticesVNTT[i] = verts[i];
			}
			
			mVertexByteWidth = sizeof(ZShadeSandboxMesh::VertexNormalTexTan);
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Pos:
		{
			ZShadeSandboxMesh::VertexPos t1 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(-1.0f, 1.0f, -1.0f);
			ZShadeSandboxMesh::VertexPos t2 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(1.0f, 1.0f, -1.0f);
			ZShadeSandboxMesh::VertexPos t3 = ZShadeSandboxMesh::VertexUtil::LoadVertexPos(-1.0f, -1.0f, -1.0f);
			
			ZShadeSandboxMesh::VertexPos verts[numVerts] =
			{
				t1, t2, t3
			};
			
			mVerticesPos.resize( numVerts );
			
			for (int i = 0; i < numVerts; i++)
			{
				mVerticesPos[i] = verts[i];
			}
			
			mVertexByteWidth = sizeof(ZShadeSandboxMesh::VertexPos);
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Tex:
		{
			ZShadeSandboxMesh::VertexTex t1 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(-1.0f, 1.0f, -1.0f, 0.0f, 0.0f);
			ZShadeSandboxMesh::VertexTex t2 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(1.0f, 1.0f, -1.0f, 1.0f, 0.0f);
			ZShadeSandboxMesh::VertexTex t3 = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f);
			
			ZShadeSandboxMesh::VertexTex verts[numVerts] =
			{
				t1, t2, t3
			};
			
			mVerticesTex.resize( numVerts );
			
			for (int i = 0; i < numVerts; i++)
			{
				mVerticesTex[i] = verts[i];
			}
			
			mVertexByteWidth = sizeof(ZShadeSandboxMesh::VertexTex);
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Color:
		{
			XMFLOAT4 diffuseColor = mMaterial->DiffuseColor();

			ZShadeSandboxMesh::VertexColor t1 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(-1.0f, 1.0f, -1.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			ZShadeSandboxMesh::VertexColor t2 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(1.0f, 1.0f, -1.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			ZShadeSandboxMesh::VertexColor t3 = ZShadeSandboxMesh::VertexUtil::LoadVertexColor(-1.0f, -1.0f, -1.0f, diffuseColor.x, diffuseColor.y, diffuseColor.z, diffuseColor.w);
			
			ZShadeSandboxMesh::VertexColor verts[numVerts] =
			{
				t1, t2, t3
			};
			
			mVerticesColor.resize( numVerts );
			
			for (int i = 0; i < numVerts; i++)
			{
				mVerticesColor[i] = verts[i];
			}
			
			mVertexByteWidth = sizeof(ZShadeSandboxMesh::VertexColor);
		}
		break;
	}
	
	mVertexCount = numVerts;
	mTriangleCount = mVertexCount / 3;
	
	//
	// Load Indices
	//

	UINT indices[numInd] =
	{
		0, 1, 2,
	};

	mIndices.resize( numInd );

	for (int i = 0; i < numInd; i++)
	{
		mIndices[i] = indices[i];
	}
}
//===============================================================================================================================