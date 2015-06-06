//===============================================================================================================================
// LightShader
//
//===============================================================================================================================
// History
//
// -Created on 4/23/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __LIGHTSHADER_H
#define __LIGHTSHADER_H
//===============================================================================================================================
//===============================================================================================================================
//Includes
#include "D3D.h"
#include "Shader.h"
#include "Vertex.h"
#include "ShaderMaterial.h"
#include "LightShadingBuffers.h"
using namespace std;
//===============================================================================================================================
//===============================================================================================================================
class LightShader : public ZShadeSandboxShader::Shader
{
	struct cbLightBuffer
	{
		XMFLOAT4 g_LightAmbient;
		XMFLOAT4 g_LightDiffuse;
		XMFLOAT3 g_LightDirection;
		float  g_ToggleShadowMap;
	};
	
	struct cbMatrixBuffer
	{
		XMFLOAT4X4	g_matWorld;
		XMFLOAT4X4	g_matView;
		XMFLOAT4X4	g_matProj;
		XMFLOAT4X4	g_ShadowMatrix;
		XMFLOAT4X4  g_LightView;
		XMFLOAT4X4  g_LightProj;
	};
	
public:
	
	LightShader(D3D* d3d);
	LightShader(const LightShader& other);
	~LightShader();
	
	bool Initialize();
	void Shutdown();
	
	bool Render11(int indexCount, ZShadeSandboxMath::XMMath4 clipplane, Camera* camera, ZShadeSandboxLighting::DirectionalLight* light, ZShadeSandboxLighting::ShaderMaterial* material);
	
private:
	
	bool bFlipHorizontally;
	bool bFlipVertically;
	
	ID3D11Buffer* m_pLightBufferCB;
	ID3D11Buffer* m_pMatrixBufferCB;
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__LIGHTSHADER_H