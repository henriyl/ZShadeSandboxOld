//===============================================================================================================================
// Environment3D
//
//===============================================================================================================================
// History
//
// -Created on 6/14/2014 by Dustin Watson
//===============================================================================================================================
#ifndef __ENVIRONMENT3D_H
#define __ENVIRONMENT3D_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//
#pragma region "Includes"
#include <memory>
#include <Windows.h>
#include <string>
#include <windowsx.h>
#include <CommCtrl.h>
#include "Window.h"
#include "D3D.h"
#include "BetterString.h"
#include "Timer.h"
#include "FPSCounter.h"
#include "Camera.h"
#include "ZMath.h"
#include "RenderTarget2D.h"
#include "input.h"
#include "ShapeContact.h"
#include "EnvironmentBase.h"
#include "CubeMesh.h"
#include "SphereMesh.h"
#include "DirectionalLight.h"
#include "SpotLight.h"
#include "PointLight.h"
#include "LightManager.h"
#include "Ray.h"
#include "ShadowMapTarget.h"
#include "Text.h"
#include "PostProcessManager.h"
#include "DirectionalLightDeferredShader.h"
#include "PointLightDeferredShader.h"
#pragma endregion

//===============================================================================================================================
//===============================================================================================================================

//
// Globals
//
//static float    gMapSize = 1024.0f;
//static float    gMinTessellationDist = 20.0f;
//static float    gMaxTessellationDist = 500.0f;
//static float    gMinTessellation = 0.0f;
//static float    gMaxTessellation = 64.0f;
//static float    gFogStart = 5.0f;//15.0f;
//static float    gFogRange = 1000.0f;//175.0f;
//static XMVECTOR gFogColor = DXColors::Silver;
//static int      gCellSize = 1024;//2049;
//static int      gLeafSize = 64;
//static int      gTerrSize = 1;
//static float    gHeightScale = 200.0f;
//static float    gCellSpacing = 0.5f;
//static bool     gTessellate = true;
//static bool		gStarting = true;
//static bool g_DepthEnable = false;

//===============================================================================================================================
//===============================================================================================================================

//
// 3D Environment
//
class Environment3D : public EnvironmentBase
{
public:
	
	Environment3D(LPCSTR base_window_name, LPCSTR render_window_name, EngineOptions* eo, bool init_lua);
	~Environment3D();

public:
	
	bool Init(bool init_lua);
	
	XMMATRIX WorldXM() { return XMLoadFloat4x4(&mWorld); }
	
	bool Quickwire();
	void SetFullscreen(bool fullscreen) { this->bFullScreen = bFullScreen; }
	int Run();
	
	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);
	
	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void RenderDeferred() = 0;
	virtual void Shutdown();
	
	// Override this for any 2D rendering in the scene
	virtual void Render2D() {}
	
	//Override these for reflection and refraction functionality
	virtual void RenderReflection(XMFLOAT4 clipplane) {}
	virtual void RenderRefraction(XMFLOAT4 clipplane) {}
	
	// Override this and add all scene items to get a shadow map
	virtual void RenderShadowMap() {}
	
	// Override this and add all scene items for deferred shading
	//virtual void RenderDeferredShading() {}
	
private:
	
	// Base functions that render reflections and refractions
	void RenderRefractionToTexture();
	void RenderReflectionToTexture();
	
	// Base stuff for shadow map
	void RenderShadowMapToTexture();
	
	// Renders all scene elements to the GBuffer
	//void RenderDeferredShadingToGBuffer();
	
	void RenderMaster();
	void UpdateMaster();
	
protected:
	
	POINT mLastMousePos;
	
	XMFLOAT4X4 mWorld;
	//XMFLOAT4 mFrustumPlanes[6];
	
	ZShadeSandboxGraphics::Text mFPSText;
	XMMATRIX mFPSTextMatrix;
	ZShadeSandboxGraphics::Text mTriangleCountText;
	XMMATRIX mTriangleCountTextMatrix;
	
	RenderTarget2D* mRefractionTexture;
	RenderTarget2D* mReflectionTexture;
	//RenderTarget2D* mShadowTexture;
	RenderTarget2D* mSceneTarget;
	
	ShadowMapTarget* mShadowTexture;
	
	ZShadeSandboxLighting::DirectionalLight* mDirLight1;
	ZShadeSandboxLighting::DirectionalLight* mDirLight2;
	ZShadeSandboxLighting::DirectionalLight* mDirLight3;
	ZShadeSandboxLighting::SpotLight* mSpotLight1;
	ZShadeSandboxLighting::PointLight* mPointLight;
	ZShadeSandboxLighting::CapsuleLight* mCapsuleLight;
	
	XMFLOAT3 mAmbientUp;
	XMFLOAT3 mAmbientDown;
	ZShadeSandboxLighting::SunLightBuffer* mSunLightBuffer;
	
	// For mouse picking a 3D object
	ZShadeSandboxMesh::SphereMesh* mPickingSphere;
	ZShadeSandboxMath::Ray* mPickingRay;
	
	int iTriangleCount;
	
	float fFrameTime;
	float fCamSpeedFast;
	float fCamSpeedSlow;
	float fShadowMapWidth;
	float fShadowMapNear;
	float fShadowMapFar;
	float fSeaLevel;
	float fTessFactor;
	
	bool bUseEngineTextures;
	bool bLeftMouseDown;
	bool bWireframeMode;
	bool bEnableLighting;
	bool bCameraStill;
	bool bFullScreen;
	bool bEnableReflections;
	bool bEnableRefractions;
	bool bEnableShadows;
	bool bEnableTessellation;
	bool bEnableCulling;
	bool bSpawnCube;
	bool bEnablePostProcessing;
	bool bEnableDeferredShading;
	bool bUseDirectionalLightDeferred;
	bool bUsePointLightDeferred;
	bool bUseSpotLightDeferred;
	bool bUseCapsuleLightDeferred;
	
	DirectionalLightDeferredShader* mDirectionalLightDeferredShader;
	PointLightDeferredShader* mPointLightDeferredShader;
	
	PostProcessManager* mPostProcessManager;
	
	// Can spawn a new basic mesh at the press of a button
	vector<ZShadeSandboxMesh::CustomMesh*> m_SpawnedMeshContainer;
};

//===============================================================================================================================
//===============================================================================================================================
#endif//__ENVIRONMENT3D_H