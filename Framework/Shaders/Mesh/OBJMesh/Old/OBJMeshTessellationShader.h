//===============================================================================================================================
// OBJMeshTessellationShader
//
//===============================================================================================================================
// History
//
// -Created on 4/15/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __OBJMESHTESSELLATIONSHADER_H
#define __OBJMESHTESSELLATIONSHADER_H
//===============================================================================================================================
//===============================================================================================================================
//Includes
#include "D3D.h"
#include "Shader.h"
#include "Vertex.h"
using namespace std;
//===============================================================================================================================
//===============================================================================================================================
class OBJMeshTessellationShader : public ZShadeSandboxShader::Shader
{
	struct cbOBJShadingConst
	{
		XMFLOAT4	g_DifColor;
		XMFLOAT3	padding;
		int			g_hasTexture;
	};
	
	struct cbTessellationBuffer
	{
		XMFLOAT3 	g_EyePosW;
		float		g_TessellationFactor;
		
		float 		g_MinDist;
		float 		g_MaxDist;
		float 		g_MinTess;
		float 		g_MaxTess;
	};
	
	struct cbDomainConst
	{
		XMFLOAT4 	g_ClipPlane;
		XMFLOAT3	padding;
		float		g_FarPlane;
	};
	
	struct cbMatrixBuffer
	{
		//XMFLOAT4X4	g_World;
		//XMFLOAT4X4	g_View;
		//XMFLOAT4X4	g_Proj;
		XMFLOAT4X4	g_WVPMatrix;
	};
	
public:
	
	OBJMeshTessellationShader(D3D* d3d);
	OBJMeshTessellationShader(const OBJMeshTessellationShader& other);
	~OBJMeshTessellationShader();

	bool Initialize();
	void Shutdown();
	
	void SetFarPlane(float farPlane) { fFarPlane = farPlane; }

	bool Render(int startIndex, int indexCount, float minTessDist, float maxTessDist, float minTess, float maxTess,
		Camera* camera, XMMATRIX wvp, XMFLOAT4 clipplane, XMFLOAT4 difColor, bool hasTexture,
		ID3D11ShaderResourceView* texture);

private:
	
	float fFarPlane;

	ID3D11Buffer* m_pTessellationCB;
	ID3D11Buffer* m_pDomainCB;
	ID3D11Buffer* m_pOBJShadingCB;
	ID3D11Buffer* m_pMatrixBufferCB;
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__OBJMESHTESSELLATIONSHADER_H