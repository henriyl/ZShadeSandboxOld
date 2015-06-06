//===============================================================================================================================
// DirectionalLightDeferredShader
//
//===============================================================================================================================
// History
//
// -Created on 6/2/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __DIRECTIONALLIGHTDEFERREDSHADER_H
#define __DIRECTIONALLIGHTDEFERREDSHADER_H
//===============================================================================================================================
//===============================================================================================================================
//Includes
#include "D3D.h"
#include "Shader.h"
#include "Vertex.h"
#include "DirectionalLight.h"
using namespace std;
//===============================================================================================================================
//===============================================================================================================================
class DirectionalLightDeferredShader : public ZShadeSandboxShader::Shader
{
	struct cbLightBuffer
	{
		XMFLOAT4	g_DirectionLightColor;
		XMFLOAT3	g_LightDirection;
		float		padding1;
		XMFLOAT3	g_EyePosition;
		float		padding2;
		XMFLOAT3	g_AmbientDown;
		float		padding3;
		XMFLOAT3	g_AmbientUp;
		float		padding4;
	};
	
	struct cbMatrixBuffer
	{
		XMFLOAT4X4	g_InvViewProj;
	};
	
	//struct cbGBufferUnpackConst
	//{
	//	XMFLOAT4 g_PerspectiveValues;
	//	XMFLOAT4 g_x4ViewInv;// Not used
	//};
	
public:
	
	DirectionalLightDeferredShader(D3D* d3d);
	DirectionalLightDeferredShader(const DirectionalLightDeferredShader& other);
	~DirectionalLightDeferredShader();
	
	bool Initialize();
	void Shutdown();
	
	bool Render11
	(	Camera* camera
	,	ZShadeSandboxLighting::DirectionalLight* light
	,	XMFLOAT3 ambientUp
	,	XMFLOAT3 ambientDown
	,	ID3D11ShaderResourceView* color0Texture
	,	ID3D11ShaderResourceView* color1Texture
	,	ID3D11ShaderResourceView* normalTexture
	,	ID3D11ShaderResourceView* depthTexture
	);
	
private:
	
	ID3D11Buffer* m_pLightCB;
	ID3D11Buffer* m_pMatrixCB;
	//ID3D11Buffer* m_pUnpackCB;
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__DIRECTIONALLIGHTDEFERREDSHADER_H