//===============================================================================================================================
// OceanSurfaceShader
//
//===============================================================================================================================
// History
//
// -Created on 5/30/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __OCEANSURFACESHADER_H
#define __OCEANSURFACESHADER_H
//===============================================================================================================================
//===============================================================================================================================

//
//Includes
//

#include "D3D.h"
#include "Shader.h"
#include "Vertex.h"
#include "OceanParameters.h"
using namespace std;

//===============================================================================================================================
//===============================================================================================================================
class OceanSurfaceShader : public ZShadeSandboxShader::Shader
{
	struct cbPerFrameBuffer
	{
		XMFLOAT2	g_padding;
		float		g_SeaLevel;
		float 		g_WaveHeight;
		XMFLOAT3 	g_LightDirection;
		float 		g_SpecularShininess;
		XMFLOAT3 	g_CamPos;
		float 		g_Time;
		XMFLOAT4 	g_RefractionTint;
		XMFLOAT4	g_DullColor;
	};
	
	struct cbTessellationBuffer
	{
		XMFLOAT3	g_EyePos;
		int			g_FrustumCull;
		float		g_MinDist;
		float		g_MaxDist;
		float		g_MinTess;
		float		g_MaxTess;
	};
	
	struct cbMatrixBuffer
	{
		XMFLOAT4X4 g_matWorld;
		XMFLOAT4X4 g_matView;
		XMFLOAT4X4 g_matProj;
		XMFLOAT4X4 g_ReflectionView;
	};
	
public:
	
	OceanSurfaceShader(D3D* d3d);
	OceanSurfaceShader(const OceanSurfaceShader& other);
	~OceanSurfaceShader();
	
	bool Initialize();
	void Shutdown();
	
	void SetTexDisplacement(ID3D11ShaderResourceView* srv)	{ mTexDisplacement = srv; }
	void SetTexPerlin(ID3D11ShaderResourceView* srv)		{ mTexPerlin = srv; }
	void SetTexGradient(ID3D11ShaderResourceView* srv)		{ mTexGradient = srv; }
	void SetWaveMap0(ID3D11ShaderResourceView* srv)			{ mWaveMap0 = srv; }
	void SetWaveMap1(ID3D11ShaderResourceView* srv)			{ mWaveMap1 = srv; }
	void SetReflectionMap(ID3D11ShaderResourceView* srv)	{ mReflectionMap = srv; }
	void SetRefractionMap(ID3D11ShaderResourceView* srv)	{ mRefractionMap = srv; }

	bool Render11(int indexCount, Camera* camera, int frustumCull, XMMATRIX world, OceanParameters ocean_params);
	
private:
	
	ID3D11Buffer* m_pTessellationCB;
	ID3D11Buffer* m_pPerFrameCB;
	ID3D11Buffer* m_pMatrixCB;

	ID3D11ShaderResourceView* mTexDisplacement;
	ID3D11ShaderResourceView* mTexPerlin;
	ID3D11ShaderResourceView* mTexGradient;
	ID3D11ShaderResourceView* mWaveMap0;
	ID3D11ShaderResourceView* mWaveMap1;
	ID3D11ShaderResourceView* mReflectionMap;
	ID3D11ShaderResourceView* mRefractionMap;
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__OCEANSURFACESHADER_H