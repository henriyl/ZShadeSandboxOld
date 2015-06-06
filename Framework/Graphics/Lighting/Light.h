//==============================================================================================================================
// Light.h
//
//==============================================================================================================================
// History
//
// -Created on 4/17/2015  by Dustin Watson
//==============================================================================================================================
#ifndef __LIGHT_H
#define __LIGHT_H
//==============================================================================================================================
//==============================================================================================================================

//
// Includes
//

#include "D3D.h"
#include "Camera.h"
#include "ZMath.h"
#include "LightCamera.h"

namespace ZShadeSandboxMesh {
	class CustomMesh;
}

namespace ZShadeSandboxLighting {
	struct LightBuffer;
	struct SunLightBuffer;
}

//==============================================================================================================================
//==============================================================================================================================
namespace ZShadeSandboxLighting {
namespace ELightType
{
	enum
	{
		eAmbient,
		eDirectional,
		eSpot,
		ePoint,
		eCapsule
	};
}
//==============================================================================================================================
//==============================================================================================================================
class Light
{
public:
	
	Light();
	
	int& LightType() 						{ return mLightType; }
	XMFLOAT4& DiffuseColor() 				{ return mDiffuseColor; }
	XMFLOAT4& AmbientColor() 				{ return mAmbientColor; }
	XMFLOAT4& SpecularColor() 				{ return mSpecularColor; }
	XMFLOAT3& Position() 					{ return mPosition; }
	XMFLOAT3& Direction() 					{ return mDirection; }
	bool& ToggleSphereMesh()				{ return bToggleSphereMesh; }
	bool& ToggleSphereMeshWireframe()		{ return bToggleSphereMeshWireframe; }
	bool& ToggleRenderDeferredMesh()		{ return bToggleRenderDeferredMesh; }
	
	LightCamera* Perspective()				{ return mLightCamera; }
	int LightType() const 					{ return mLightType; }
	XMFLOAT4 DiffuseColor() const 			{ return mDiffuseColor; }
	XMFLOAT4 AmbientColor() const 			{ return mAmbientColor; }
	XMFLOAT4 SpecularColor() const			{ return mSpecularColor; }
	XMFLOAT3 Position() const 				{ return mPosition; }
	XMFLOAT3 Direction() const 				{ return mDirection; }
	bool ToggleSphereMesh() const			{ return bToggleSphereMesh; }
	bool ToggleSphereMeshWireframe() const	{ return bToggleSphereMeshWireframe; }
	bool ToggleRenderDeferredMesh() const	{ return bToggleRenderDeferredMesh; }

	int FetchLightTypeIndex();
	
	// Updates the light view projection matrix for this light
	void UpdateLVP();
	void Update();
	void SetLens(float fnear, float ffar);
	
	// When a light is created from the light manager it will build the mesh and render it
	void BuildSphereMesh(D3D* d3d);
	void RenderSphereMesh(Camera* camera, LightCamera* lightcamera, bool reflect, XMFLOAT4 clipplane);
	
	void UpdateMeshPosition(XMFLOAT3 pos);
	void ScaleMesh(XMFLOAT3 scale);
	void SetMesh(ZShadeSandboxMesh::CustomMesh* mesh);
	
	void AddMeshLightBuffer(ZShadeSandboxLighting::LightBuffer* lb);
	void AddMeshLightBuffer(ZShadeSandboxLighting::SunLightBuffer* sb);

	int TriangleCount() const;
	
	virtual Light* Clone() { return NULL; }
	
protected:
	
	int mLightType;
	XMFLOAT4 mDiffuseColor;
	XMFLOAT4 mAmbientColor;
	XMFLOAT4 mSpecularColor;
	XMFLOAT3 mPosition;
	XMFLOAT3 mDirection;
	
	// Camera for the perspective of this light
	LightCamera* mLightCamera;
	
	// Can toggle the position of this light source to see where it is
	bool bToggleSphereMesh;
	
	// Can the sphere mesh wireframe be shown ??
	bool bToggleSphereMeshWireframe;

	// Can the sphere mesh be rendered with deferred shading
	bool bToggleRenderDeferredMesh;
	
	// Mesh that represents the position of the light source in the shape of a sphere
	ZShadeSandboxMesh::CustomMesh* mMesh;
};
}
//==============================================================================================================================
//==============================================================================================================================
#endif//__LIGHT_H