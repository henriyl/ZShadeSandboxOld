#include "GeosphereMesh.h"
#include "ZMath.h"
#include <algorithm>
using ZShadeSandboxMesh::GeosphereMesh;
//===============================================================================================================================
GeosphereMesh::GeosphereMesh(float radius, UINT subdivisions, D3D* d3d, ZShadeSandboxMesh::MeshParameters mp)
:   ZShadeSandboxMesh::CustomMesh( d3d, mp )
,   fRadius(radius)
,   iSubdivisions(subdivisions)
{
	mMeshType = ZShadeSandboxMesh::EMeshType::GEOSPHERE;

	Initialize();
	
	CreateBuffers();
}
//===============================================================================================================================
GeosphereMesh::~GeosphereMesh()
{

}
//===============================================================================================================================
void GeosphereMesh::Initialize()
{
	int NUM_SPHERE_VERTS   = 0;
	int NUM_SPHERE_INDICES = 0;
	
	// Approximate a sphere by tessellating an icosahedron.
	
	// Make sure the number of subdivisions do not get out of control
	iSubdivisions = ZShadeSandboxMath::ZMath::Min(iSubdivisions, 5u);
	
	const float X = 0.525731f; 
	const float Z = 0.850651f;

	XMFLOAT3 pos[12] = 
	{
		XMFLOAT3(-X, 0.0f, Z),  XMFLOAT3(X, 0.0f, Z),  
		XMFLOAT3(-X, 0.0f, -Z), XMFLOAT3(X, 0.0f, -Z),    
		XMFLOAT3(0.0f, Z, X),   XMFLOAT3(0.0f, Z, -X), 
		XMFLOAT3(0.0f, -Z, X),  XMFLOAT3(0.0f, -Z, -X),    
		XMFLOAT3(Z, X, 0.0f),   XMFLOAT3(-Z, X, 0.0f), 
		XMFLOAT3(Z, -X, 0.0f),  XMFLOAT3(-Z, -X, 0.0f)
	};

	DWORD k[60] = 
	{
		1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,    
		1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,    
		3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0, 
		10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7 
	};
	
	//
	// Load Vertices
	//
	
	NUM_SPHERE_VERTS = 12;
	
	switch (mMeshParameters.vertexType)
	{
		case ZShadeSandboxMesh::EVertexType::VT_NormalTex:
		{
#pragma region "Vertex Normal Tex"
			mVerticesVNT.resize(NUM_SPHERE_VERTS);
			
			for(UINT i = 0; i < NUM_SPHERE_VERTS; ++i)
				mVerticesVNT[i].position = pos[i];
			
			mVertexByteWidth = sizeof( ZShadeSandboxMesh::VertexNormalTex );
			mVertexCount = NUM_SPHERE_VERTS;
			
			//
			// Load Indices
			//
			
			NUM_SPHERE_INDICES = 60;
			mIndexCount = NUM_SPHERE_INDICES;
			mIndices.resize( NUM_SPHERE_INDICES );
			
			for(UINT i = 0; i < NUM_SPHERE_INDICES; ++i)
				mIndices[i] = k[i];
			
			//
			// Subdivide and create the mesh
			//
			
			for (UINT i = 0; i < iSubdivisions; ++i)
				Subdivide_VertexNormalTex();
			
			ZShadeSandboxMesh::VertexNormalTex vertex;

			// Project vertices onto sphere and scale.
			for (UINT i = 0; i < mVertexCount; ++i)
			{
				// Project onto a unit sphere
				ZShadeSandboxMath::Vec3<XMFLOAT3> n = mVerticesVNT[i].position;
				
				n.Normalize();
				
				// Project onto a sphere
				ZShadeSandboxMath::Vec3<XMFLOAT3> p = n;
				
				p *= fRadius;

				vertex.position = p;
				vertex.normal = n;
				
				// Derive the texture coordinates from spherical coordinates
				float theta = ZShadeSandboxMath::ZMath::AngleFromXY(vertex.position.x, vertex.position.z);
				
				float phi = acosf(vertex.position.y / fRadius);
				
				vertex.texture.x = theta / TWO_PI;
				vertex.texture.y = phi / PI;

				mVerticesVNT[i] = vertex;
			}
#pragma endregion
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_NormalTexTan:
		{
#pragma region "Vertex Normal Tex Tan"
			mVerticesVNTT.resize(NUM_SPHERE_VERTS);
			
			for(UINT i = 0; i < NUM_SPHERE_VERTS; ++i)
				mVerticesVNTT[i].position = pos[i];
			
			mVertexByteWidth = sizeof( ZShadeSandboxMesh::VertexNormalTexTan );
			mVertexCount = NUM_SPHERE_VERTS;
			
			//
			// Load Indices
			//
			
			NUM_SPHERE_INDICES = 60;
			mIndexCount = NUM_SPHERE_INDICES;
			mIndices.resize( NUM_SPHERE_INDICES );
			
			for(UINT i = 0; i < NUM_SPHERE_INDICES; ++i)
				mIndices[i] = k[i];
			
			//
			// Subdivide and create the mesh
			//
			
			for (UINT i = 0; i < iSubdivisions; ++i)
				Subdivide_VertexNormalTexTan();
			
			ZShadeSandboxMesh::VertexNormalTexTan vertex;

			// Project vertices onto sphere and scale.
			for (UINT i = 0; i < mVertexCount; ++i)
			{
				// Project onto a unit sphere
				ZShadeSandboxMath::Vec3<XMFLOAT3> n = mVerticesVNTT[i].position;
				
				n.Normalize();
				
				// Project onto a sphere
				ZShadeSandboxMath::Vec3<XMFLOAT3> p = n;
				
				p *= fRadius;

				vertex.position = p;
				vertex.normal = n;
				
				// Derive the texture coordinates from spherical coordinates
				float theta = ZShadeSandboxMath::ZMath::AngleFromXY(vertex.position.x, vertex.position.z);
				
				float phi = acosf(vertex.position.y / fRadius);
				
				vertex.texture.x = theta / TWO_PI;
				vertex.texture.y = phi / PI;
				
				// Partial derivative of P with respect to theta
				vertex.tangentU.x = -fRadius * sinf(phi) * sinf(theta);
				vertex.tangentU.y = 0.0f;
				vertex.tangentU.z = fRadius * sinf(phi) * sinf(theta);
				
				ZShadeSandboxMath::Vec3<XMFLOAT3> T = vertex.tangentU;
				
				T.Normalize();
				
				vertex.tangentU = T.Vector();

				mVerticesVNTT[i] = vertex;
			}
#pragma endregion
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Pos:
		{
#pragma region "Vertex Pos"
			mVerticesPos.resize(NUM_SPHERE_VERTS);
			
			for(UINT i = 0; i < NUM_SPHERE_VERTS; ++i)
				mVerticesPos[i].position = pos[i];
			
			mVertexByteWidth = sizeof( ZShadeSandboxMesh::VertexPos );
			mVertexCount = NUM_SPHERE_VERTS;
			
			//
			// Load Indices
			//
			
			NUM_SPHERE_INDICES = 60;
			mIndexCount = NUM_SPHERE_INDICES;
			mIndices.resize( NUM_SPHERE_INDICES );
			
			for(UINT i = 0; i < NUM_SPHERE_INDICES; ++i)
				mIndices[i] = k[i];
			
			//
			// Subdivide and create the mesh
			//
			
			for (UINT i = 0; i < iSubdivisions; ++i)
				Subdivide_VertexPos();
			
			ZShadeSandboxMesh::VertexPos vertex;

			// Project vertices onto sphere and scale.
			for (UINT i = 0; i < mVertexCount; ++i)
			{
				// Project onto a unit sphere
				ZShadeSandboxMath::Vec3<XMFLOAT3> n = mVerticesPos[i].position;
				
				n.Normalize();
				
				// Project onto a sphere
				ZShadeSandboxMath::Vec3<XMFLOAT3> p = n;
				
				p *= fRadius;

				vertex.position = p;

				mVerticesPos[i] = vertex;
			}
#pragma endregion
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Tex:
		{
#pragma region "Vertex Tex"
			mVerticesTex.resize(NUM_SPHERE_VERTS);
			
			for(UINT i = 0; i < NUM_SPHERE_VERTS; ++i)
				mVerticesTex[i].position = pos[i];
			
			mVertexByteWidth = sizeof( ZShadeSandboxMesh::VertexTex );
			mVertexCount = NUM_SPHERE_VERTS;
			
			//
			// Load Indices
			//
			
			NUM_SPHERE_INDICES = 60;
			mIndexCount = NUM_SPHERE_INDICES;
			mIndices.resize( NUM_SPHERE_INDICES );
			
			for(UINT i = 0; i < NUM_SPHERE_INDICES; ++i)
				mIndices[i] = k[i];
			
			//
			// Subdivide and create the mesh
			//
			
			for (UINT i = 0; i < iSubdivisions; ++i)
				Subdivide_VertexTex();
			
			ZShadeSandboxMesh::VertexTex vertex;

			// Project vertices onto sphere and scale.
			for (UINT i = 0; i < mVertexCount; ++i)
			{
				// Project onto a unit sphere
				ZShadeSandboxMath::Vec3<XMFLOAT3> n = mVerticesTex[i].position;
				
				n.Normalize();
				
				// Project onto a sphere
				ZShadeSandboxMath::Vec3<XMFLOAT3> p = n;
				
				p *= fRadius;

				vertex.position = p;
				
				// Derive the texture coordinates from spherical coordinates
				float theta = ZShadeSandboxMath::ZMath::AngleFromXY(vertex.position.x, vertex.position.z);
				
				float phi = acosf(vertex.position.y / fRadius);
				
				vertex.texture.x = theta / TWO_PI;
				vertex.texture.y = phi / PI;

				mVerticesTex[i] = vertex;
			}
#pragma endregion
		}
		break;
		case ZShadeSandboxMesh::EVertexType::VT_Color:
		{
#pragma region "Vertex Color"
			XMFLOAT4 diffuseColor = mMaterial->DiffuseColor();

			mVerticesColor.resize(NUM_SPHERE_VERTS);
			
			for(UINT i = 0; i < NUM_SPHERE_VERTS; ++i)
				mVerticesColor[i].position = pos[i];
			
			mVertexByteWidth = sizeof( ZShadeSandboxMesh::VertexColor );
			mVertexCount = NUM_SPHERE_VERTS;
			
			//
			// Load Indices
			//
			
			NUM_SPHERE_INDICES = 60;
			mIndexCount = NUM_SPHERE_INDICES;
			mIndices.resize( NUM_SPHERE_INDICES );
			
			for(UINT i = 0; i < NUM_SPHERE_INDICES; ++i)
				mIndices[i] = k[i];
			
			//
			// Subdivide and create the mesh
			//
			
			for (UINT i = 0; i < iSubdivisions; ++i)
				Subdivide_VertexColor();
			
			ZShadeSandboxMesh::VertexColor vertex;

			// Project vertices onto sphere and scale.
			for (UINT i = 0; i < mVertexCount; ++i)
			{
				// Project onto a unit sphere
				ZShadeSandboxMath::Vec3<XMFLOAT3> n = mVerticesColor[i].position;
				
				n.Normalize();
				
				// Project onto a sphere
				ZShadeSandboxMath::Vec3<XMFLOAT3> p = n;
				
				p *= fRadius;

				vertex.position = p;
				vertex.color = diffuseColor;

				mVerticesColor[i] = vertex;
			}
#pragma endregion
		}
		break;
	}
	
	mTriangleCount = mVertexCount / 3;
}
//===============================================================================================================================
