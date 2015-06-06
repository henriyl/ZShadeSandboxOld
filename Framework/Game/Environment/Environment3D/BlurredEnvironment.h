//===============================================================================================================================
// BlurredEnvironment
//
//===============================================================================================================================
// History
//
// -Created on 7/2/2014 by Dustin Watson
//===============================================================================================================================
#ifndef __BLURREDENVIRONMENT_H
#define __BLURREDENVIRONMENT_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//
#pragma region "Includes"
#include "Environment3D.h"
#include "CubeMesh.h"
#include "BlurShader.h"
#include "RenderTarget2D.h"
#include "QuadMesh.h"
#include "Shader.h"
#include "QuadMesh.h"
#include "TextureShader.h"
#include <vector>
using namespace std;
#pragma endregion

//===============================================================================================================================
//===============================================================================================================================

//
// Blurred Environment
//
class BlurredEnvironment : public Environment3D
{
public:
	BlurredEnvironment(LPCSTR base_window_name, LPCSTR render_window_name, EngineOptions* eo, bool init_lua);
	~BlurredEnvironment();

	bool Initialize();
	void Shutdown();

	void Update();
	void Render();
	void RenderDeferred();

	bool RenderSceneToTexture();
	bool DownSampleTexture();
	bool RenderHorizontalBlurToTexture();
	bool RenderVerticalBlurToTexture();
	bool UpSampleTexture();
	bool Render2DTextureScene();

	void RenderReflection(XMFLOAT4 clipplane);
	void RenderRefraction(XMFLOAT4 clipplane);

private:
	//Cube to Blur
	ZShadeSandboxMesh::CubeMesh* m_pCube;
	//Cube to Not Blur
	ZShadeSandboxMesh::CubeMesh* m_pNoBlurCube;

	//Cube to hold Blur
	ZShadeSandboxMesh::CubeMesh* m_pBlurredCube;

	TextureShader* m_pTextureShader;
	BlurShader* m_pBlurShader;

	RenderTarget2D* m_pRenderTexture;
	RenderTarget2D* m_pDownSampleTexture;
	RenderTarget2D* m_pHorizontalBlurTexture;
	RenderTarget2D* m_pVerticalBlurTexture;
	RenderTarget2D* m_pUpSampleTexture;

	ZShadeSandboxMesh::QuadMesh* m_pSmallWindow;
	ZShadeSandboxMesh::QuadMesh* m_pFullscreenWidow;
};

//===============================================================================================================================
//===============================================================================================================================
#endif//__BLURREDENVIRONMENT_H