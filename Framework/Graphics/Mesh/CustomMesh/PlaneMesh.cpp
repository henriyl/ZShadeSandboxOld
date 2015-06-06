#include "PlaneMesh.h"
using ZShadeSandboxMesh::PlaneMesh;
//===============================================================================================================================
PlaneMesh::PlaneMesh(D3D* d3d, ZShadeSandboxMesh::MeshParameters mp, char* filename)
:   ZShadeSandboxMesh::CustomMesh(d3d, mp, filename)
,   iPlaneSize(0)
,   fPlaneHeight(0)
,   bCenterPlane(false)
{
	mMeshType = ZShadeSandboxMesh::EMeshType::PLANE;
}
//===============================================================================================================================
PlaneMesh::PlaneMesh(int planeSize, float planeHeight, D3D* d3d, ZShadeSandboxMesh::MeshParameters mp, bool centerPlane)
:   ZShadeSandboxMesh::CustomMesh(d3d, mp)
,   iPlaneSize(planeSize)
,   fPlaneHeight(planeHeight)
,   bCenterPlane(centerPlane)
{
	mMeshType = ZShadeSandboxMesh::EMeshType::PLANE;
	Initialize();
	CreateBuffers();
}
//===============================================================================================================================
PlaneMesh::~PlaneMesh()
{
}
//===============================================================================================================================
void PlaneMesh::Initialize()
{
	int numVertices = 6;
	
	mTriangleCount = mVertexCount / 3;
	
	float halfWidth = 0;
	
	if (bCenterPlane)
	{
		halfWidth = ((float)iPlaneSize - 1.0f) / 2.0f;
		iPlaneSize = iPlaneSize - halfWidth;
	}
	
	//
	// Load Vertices and Indices
	//

	switch (mMeshParameters.vertexType)
	{
		case ZShadeSandboxMesh::EVertexType::VT_NormalTex:
		{
#pragma region "Vertex Normal Tex"
			mVertexByteWidth = sizeof(ZShadeSandboxMesh::VertexNormalTex);
			mVertexCount = numVertices;
			mVerticesVNT.resize(mVertexCount);

			// Load the vertex array with data.
			mVerticesVNT[0].position = XMFLOAT3(-iPlaneSize, fPlaneHeight, iPlaneSize);  // Top left.
			mVerticesVNT[0].normal = XMFLOAT3(0, 1, 0);
			mVerticesVNT[0].texture = XMFLOAT2(0.0f, 0.0f);//0, 0

			mVerticesVNT[1].position = XMFLOAT3(iPlaneSize, fPlaneHeight, iPlaneSize);  // Top right.
			mVerticesVNT[1].normal = XMFLOAT3(0, 1, 0);
			mVerticesVNT[1].texture = XMFLOAT2(1.0f, 0.0f);//1, 0

			mVerticesVNT[2].position = XMFLOAT3(-iPlaneSize, fPlaneHeight, -iPlaneSize);  // Bottom left.
			mVerticesVNT[2].normal = XMFLOAT3(0, 1, 0);
			mVerticesVNT[2].texture = XMFLOAT2(0.0f, 1.0f);//0, 1

			mVerticesVNT[3].position = XMFLOAT3(-iPlaneSize, fPlaneHeight, -iPlaneSize);  // Bottom left.
			mVerticesVNT[3].normal = XMFLOAT3(0, 1, 0);
			mVerticesVNT[3].texture = XMFLOAT2(0.0f, 1.0f);//0, 1

			mVerticesVNT[4].position = XMFLOAT3(iPlaneSize, fPlaneHeight, iPlaneSize);  // Top right.
			mVerticesVNT[4].normal = XMFLOAT3(0, 1, 0);
			mVerticesVNT[4].texture = XMFLOAT2(1.0f, 0.0f);//1, 0

			mVerticesVNT[5].position = XMFLOAT3(iPlaneSize, fPlaneHeight, -iPlaneSize);  // Bottom right.
			mVerticesVNT[5].normal = XMFLOAT3(0, 1, 0);
			mVerticesVNT[5].texture = XMFLOAT2(1.0f, 1.0f);//1, 1
#pragma endregion
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Tex:
		{
#pragma region "Vertex Tex"
			mVertexByteWidth = sizeof(ZShadeSandboxMesh::VertexTex);
			mVertexCount = numVertices;
			mVerticesTex.resize(mVertexCount);

			// Load the vertex array with data.
			mVerticesTex[0].position = XMFLOAT3(-iPlaneSize, fPlaneHeight, iPlaneSize);  // Top left.
			mVerticesTex[0].texture = XMFLOAT2(0.0f, 0.0f);

			mVerticesTex[1].position = XMFLOAT3(iPlaneSize, fPlaneHeight, iPlaneSize);  // Top right.
			mVerticesTex[1].texture = XMFLOAT2(1.0f, 0.0f);

			mVerticesTex[2].position = XMFLOAT3(-iPlaneSize, fPlaneHeight, -iPlaneSize);  // Bottom left.
			mVerticesTex[2].texture = XMFLOAT2(0.0f, 1.0f);

			mVerticesTex[3].position = XMFLOAT3(-iPlaneSize, fPlaneHeight, -iPlaneSize);  // Bottom left.
			mVerticesTex[3].texture = XMFLOAT2(0.0f, 1.0f);

			mVerticesTex[4].position = XMFLOAT3(iPlaneSize, fPlaneHeight, iPlaneSize);  // Top right.
			mVerticesTex[4].texture = XMFLOAT2(1.0f, 0.0f);

			mVerticesTex[5].position = XMFLOAT3(iPlaneSize, fPlaneHeight, -iPlaneSize);  // Bottom right.
			mVerticesTex[5].texture = XMFLOAT2(1.0f, 1.0f);
#pragma endregion
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Color:
		{
#pragma region "Vertex Color"
			mVertexByteWidth = sizeof(ZShadeSandboxMesh::VertexColor);
			mVertexCount = numVertices;
			mVerticesColor.resize(mVertexCount);

			XMFLOAT4 diffuseColor = mMeshParameters.material->DiffuseColor();

			// Load the vertex array with data.
			mVerticesColor[0].position = XMFLOAT3(-iPlaneSize, fPlaneHeight, iPlaneSize);  // Top left.
			mVerticesColor[0].color = diffuseColor;

			mVerticesColor[1].position = XMFLOAT3(iPlaneSize, fPlaneHeight, iPlaneSize);  // Top right.
			mVerticesColor[1].color = diffuseColor;

			mVerticesColor[2].position = XMFLOAT3(-iPlaneSize, fPlaneHeight, -iPlaneSize);  // Bottom left.
			mVerticesColor[2].color = diffuseColor;

			mVerticesColor[3].position = XMFLOAT3(-iPlaneSize, fPlaneHeight, -iPlaneSize);  // Bottom left.
			mVerticesColor[3].color = diffuseColor;

			mVerticesColor[4].position = XMFLOAT3(iPlaneSize, fPlaneHeight, iPlaneSize);  // Top right.
			mVerticesColor[4].color = diffuseColor;

			mVerticesColor[5].position = XMFLOAT3(iPlaneSize, fPlaneHeight, -iPlaneSize);  // Bottom right.
			mVerticesColor[5].color = diffuseColor;
#pragma endregion
		}
		break;
	}

	mIndexCount = 6;
	mIndices.resize(mIndexCount);

	// Load the index array with data.
	mIndices[0] = 0;
	mIndices[1] = 1;
	mIndices[2] = 2;
	mIndices[3] = 3;
	mIndices[4] = 4;
	mIndices[5] = 5;
}
//===============================================================================================================================
void PlaneMesh::UpdateBuffers(float planeHeight)
{
	fPlaneHeight = planeHeight;
	
	float halfWidth = 0;
	
	if (bCenterPlane)
	{
		halfWidth = ((float)iPlaneSize - 1.0f) / 2.0f;
		iPlaneSize = iPlaneSize - halfWidth;
	}

	// Update the AABB
	UpdateAABB(mPosition, XMFLOAT3(iPlaneSize, fPlaneHeight, iPlaneSize));

	switch (mMeshParameters.vertexType)
	{
		case ZShadeSandboxMesh::EVertexType::VT_NormalTex:
		{
			mVerticesVNT[0].position = XMFLOAT3(-iPlaneSize, fPlaneHeight, iPlaneSize);  // Top left.
			mVerticesVNT[0].normal = XMFLOAT3(0, 1, 0);
			mVerticesVNT[0].texture = XMFLOAT2(0.0f, 0.0f);

			mVerticesVNT[1].position = XMFLOAT3(iPlaneSize, fPlaneHeight, iPlaneSize);  // Top right.
			mVerticesVNT[1].normal = XMFLOAT3(0, 1, 0);
			mVerticesVNT[1].texture = XMFLOAT2(1.0f, 0.0f);

			mVerticesVNT[2].position = XMFLOAT3(-iPlaneSize, fPlaneHeight, -iPlaneSize);  // Bottom left.
			mVerticesVNT[2].normal = XMFLOAT3(0, 1, 0);
			mVerticesVNT[2].texture = XMFLOAT2(0.0f, 1.0f);

			mVerticesVNT[3].position = XMFLOAT3(-iPlaneSize, fPlaneHeight, -iPlaneSize);  // Bottom left.
			mVerticesVNT[3].normal = XMFLOAT3(0, 1, 0);
			mVerticesVNT[3].texture = XMFLOAT2(0.0f, 1.0f);

			mVerticesVNT[4].position = XMFLOAT3(iPlaneSize, fPlaneHeight, iPlaneSize);  // Top right.
			mVerticesVNT[4].normal = XMFLOAT3(0, 1, 0);
			mVerticesVNT[4].texture = XMFLOAT2(1.0f, 0.0f);

			mVerticesVNT[5].position = XMFLOAT3(iPlaneSize, fPlaneHeight, -iPlaneSize);  // Bottom right.
			mVerticesVNT[5].normal = XMFLOAT3(0, 1, 0);
			mVerticesVNT[5].texture = XMFLOAT2(1.0f, 1.0f);
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Tex:
		{
			// Load the vertex array with data.
			mVerticesTex[0].position = XMFLOAT3(-iPlaneSize, fPlaneHeight, iPlaneSize);  // Top left.
			mVerticesTex[0].texture = XMFLOAT2(0.0f, 0.0f);

			mVerticesTex[1].position = XMFLOAT3(iPlaneSize, fPlaneHeight, iPlaneSize);  // Top right.
			mVerticesTex[1].texture = XMFLOAT2(1.0f, 0.0f);

			mVerticesTex[2].position = XMFLOAT3(-iPlaneSize, fPlaneHeight, -iPlaneSize);  // Bottom left.
			mVerticesTex[2].texture = XMFLOAT2(0.0f, 1.0f);

			mVerticesTex[3].position = XMFLOAT3(-iPlaneSize, fPlaneHeight, -iPlaneSize);  // Bottom left.
			mVerticesTex[3].texture = XMFLOAT2(0.0f, 1.0f);

			mVerticesTex[4].position = XMFLOAT3(iPlaneSize, fPlaneHeight, iPlaneSize);  // Top right.
			mVerticesTex[4].texture = XMFLOAT2(1.0f, 0.0f);

			mVerticesTex[5].position = XMFLOAT3(iPlaneSize, fPlaneHeight, -iPlaneSize);  // Bottom right.
			mVerticesTex[5].texture = XMFLOAT2(1.0f, 1.0f);
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Color:
		{
			XMFLOAT4 diffuseColor = mMeshParameters.material->DiffuseColor();

			// Load the vertex array with data.
			mVerticesColor[0].position = XMFLOAT3(-iPlaneSize, fPlaneHeight, iPlaneSize);  // Top left.
			mVerticesColor[0].color = diffuseColor;

			mVerticesColor[1].position = XMFLOAT3(iPlaneSize, fPlaneHeight, iPlaneSize);  // Top right.
			mVerticesColor[1].color = diffuseColor;

			mVerticesColor[2].position = XMFLOAT3(-iPlaneSize, fPlaneHeight, -iPlaneSize);  // Bottom left.
			mVerticesColor[2].color = diffuseColor;

			mVerticesColor[3].position = XMFLOAT3(-iPlaneSize, fPlaneHeight, -iPlaneSize);  // Bottom left.
			mVerticesColor[3].color = diffuseColor;

			mVerticesColor[4].position = XMFLOAT3(iPlaneSize, fPlaneHeight, iPlaneSize);  // Top right.
			mVerticesColor[4].color = diffuseColor;

			mVerticesColor[5].position = XMFLOAT3(iPlaneSize, fPlaneHeight, -iPlaneSize);  // Bottom right.
			mVerticesColor[5].color = diffuseColor;
		}
		break;
	}

	// Rebuild the vertex buffer
	RegenVertexBuffer();
}
//===============================================================================================================================