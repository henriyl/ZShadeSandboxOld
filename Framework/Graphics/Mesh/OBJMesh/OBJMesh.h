//===============================================================================================================================
// OBJMesh.h
//
//===============================================================================================================================
// History
//
// -Created on 5/18/2015 by henriyl
// -Updated 5/21/2015 for the engine by Dustin Watson
//===============================================================================================================================
#ifndef __WAVEFRONTOBJ_H
#define __WAVEFRONTOBJ_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//

#include <vector>
#include <fstream>
#include <istream>
#include "Vertex.h"
#include "D3D.h"
#include "Camera.h"
#include "GameDirectory.h"
#include "OBJMeshShader.h"
#include "OBJMeshTessellationShader.h"
#include "OBJMeshSurfaceMaterial.h"
#include "OBJDeferredShader.h"

//===============================================================================================================================
//===============================================================================================================================

typedef unsigned int IndexType;

// SamplerStates etc in hlsl files do nothing if Effect framework is not used
// MaleLow.obj has quads, retriangulated in VS. Some of the triangles still have wrong winding order...

namespace ZShadeSandboxMesh
{
struct OBJMeshRenderParameters
{
	Camera* camera;
	LightCamera* lightCamera;
	ERenderType::Type renderType;
	bool tessellate;
	//float tessFactor;
	float minTessDist;
	float maxTessDist;
	float minTess;
	float maxTess;
	bool reflection;
	bool bRenderDeferred;
	XMFLOAT4 clipplane;

	OBJMeshRenderParameters()
	{
		camera = 0;
		renderType = ZShadeSandboxMesh::ERenderType::eTriangleList;
		tessellate = false;
		//tessFactor = 6.0f;
		minTessDist = 20.0f;
		maxTessDist = 500.0f;
		minTess = -1.0f;
		maxTess = 64.0f;
		reflection = false;
		bRenderDeferred = false;
		clipplane = XMFLOAT4(0, 0, 0, 0);
	}
};

class OBJMesh
{
	struct Group
	{
		OBJMeshSurfaceMaterial Material;
		string Name;
		int IndexStart;
		int IndexCount;
	};

	class Mesh
	{
	public:
		
		Mesh(D3D* d3d);
		~Mesh();
		
		bool Load(string filename, bool bRebuildNormals, bool isRHCoordSys);
		void Render(OBJMeshRenderParameters render_param);
		void SetWireframe(bool wire);
		void SetFarPlane(float farPlane);

		XMFLOAT3& Scale();
		XMFLOAT3& Rotate();
		XMFLOAT3& Position();

	private:

		void CalculateWorldMatrix();

		int							mVertexCount;
		int							mIndexCount;
		int							mTriangleCount;
		ID3D11Buffer*				mVB;
		ID3D11Buffer*				mIB;
		OBJMeshShader*      	   	mShader;
		OBJMeshTessellationShader*  mTessellationShader;
		OBJDeferredShader*			mOBJDeferredShader;
		D3D*						mD3DSystem;
		XMMATRIX					mWorld;
		XMFLOAT3					mPosition;
		XMFLOAT3					mScale;
		XMFLOAT3					mRotation;
		vector<Group>				mGroups;
	};

public:

	OBJMesh(D3D* d3d, GameDirectory3D* g3D);
	~OBJMesh();

	void Load(string filename, bool bRebuildNormals = false, bool isRHCoordSys = false);
	void Render(OBJMeshRenderParameters render_param);
	void SetWireframe(bool wire);
	void SetFarPlane(float farPlane);

	XMFLOAT3& Scale();
	XMFLOAT3& Rotate();
	XMFLOAT3& Position();

private:

	static GameDirectory3D* mGD3D;

	D3D*  mD3DSystem;
	Mesh* m_pMesh;

	static bool LoadMtl(string filename, D3D* d3d, map<string, OBJMeshSurfaceMaterial>& materials);
	static void InitMaterial(OBJMeshSurfaceMaterial* mat);
	static bool LoadObj(string filename, D3D* d3d, vector<VertexNormalTex>& vertices,
		vector<IndexType>& indices, vector<Group>& groups, bool& hadNormals, bool isRHCoordSys);
};
}
//===============================================================================================================================
//===============================================================================================================================
#endif//__WAVEFRONTOBJ_H