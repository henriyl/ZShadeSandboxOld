//===============================================================================================================================
// QuadMaterialTessellationShader
//
//===============================================================================================================================
// History
//
// -Created on 5/12/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __QUADMATERIALTESSELLATIONSHADER_H
#define __QUADMATERIALTESSELLATIONSHADER_H
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
class QuadMaterialTessellationShader : public ZShadeSandboxShader::Shader
{
	struct Const_Per_Frame
	{
		XMFLOAT3	g_EyePosW;
		int			g_UsingDiffuseArrayTexture;
		XMFLOAT4 	g_ClipPlane;
		XMFLOAT4	g_MaterialDiffuseColor;
		XMFLOAT4	g_MaterialAmbientColor;
		float		g_MaterialSpecularPower;
		float		g_MaterialSpecularIntensity;
		int	   		g_UsingDiffuseTexture;
		int	   		g_UsingNormalMapTexture;
		int	   		g_UsingBlendMapTexture;
		int	   		g_UsingDetailMapTexture;
		int			g_UseAlphaMapTexture;
		int			g_EnableTransparency;
		int			g_EnableLighting;
		float		g_DetailBrightness;
		int	   		g_FlipTextureH;
		int	   		g_FlipTextureV;
		XMFLOAT3	g_AlphaToCoverageValue; // Value that clips pixel during alpha blending
		float		padding;
	};
	
	struct cTessellationBuffer
	{
		float g_TessellationFactor;
		XMFLOAT3 padding;
	};
	
	struct Const_Per_Object
	{
		XMFLOAT4X4	g_matWorld;
		XMFLOAT4X4	g_matView;
		XMFLOAT4X4	g_matProj;
	};
	
public:
	
	QuadMaterialTessellationShader(D3D* d3d);
	QuadMaterialTessellationShader(const QuadMaterialTessellationShader& other);
	~QuadMaterialTessellationShader();
	
	bool Initialize();
	void Shutdown();
	
	// true - flips the texture Horizontally
	// false - flips the texture Vertically
	void FlipTextureHorizontally(bool flip);
	
	bool Render11(int indexCount, ZShadeSandboxMath::XMMath4 clipplane, Camera* camera, float tessFactor, ZShadeSandboxLighting::ShaderMaterial* material);
	
private:
	
	bool bFlipHorizontally;
	bool bFlipVertically;
	
	ID3D11Buffer* m_pTessellationCB;
	ID3D11Buffer* m_pPerFrameCB;
	ID3D11Buffer* m_pPerObjectCB;
	ID3D11Buffer* m_pLightCB;
	ID3D11Buffer* m_pSunCB;
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__QUADMATERIALTESSELLATIONSHADER_H