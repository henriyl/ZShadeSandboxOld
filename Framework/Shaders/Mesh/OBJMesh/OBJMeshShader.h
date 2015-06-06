//===============================================================================================================================
// OBJMeshShader
//
//===============================================================================================================================
// History
//
// -Created on 4/15/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __OBJMESHSHADER_H
#define __OBJMESHSHADER_H
//===============================================================================================================================
//===============================================================================================================================
//Includes
#include "D3D.h"
#include "Shader.h"
#include "Vertex.h"
using namespace std;
//===============================================================================================================================
//===============================================================================================================================
class OBJMeshShader : public ZShadeSandboxShader::Shader
{
	struct cbOBJShadingConst
	{
		XMFLOAT4 	g_ClipPlane;
		XMFLOAT4	g_DifColor;
		XMFLOAT2	padding;
		float		g_FarPlane;
		int			g_hasTexture;
	};
	
	struct cbMatrixBuffer
	{
		XMFLOAT4X4	g_WVPMatrix;
		XMFLOAT4X4	g_WorldMatrix;
	};
	
public:
	
	OBJMeshShader(D3D* d3d);
	OBJMeshShader(const OBJMeshShader& other);
	~OBJMeshShader();

	bool Initialize();
	void Shutdown();
	
	void SetFarPlane(float farPlane) { fFarPlane = farPlane; }

	bool Render(int startIndex, int indexCount, Camera* camera, XMMATRIX wvp, XMFLOAT4 clipplane, XMFLOAT4 difColor, bool hasTexture,
		ID3D11ShaderResourceView* texture);

private:
	
	float fFarPlane;

	ID3D11Buffer* m_pOBJShadingCB;
	ID3D11Buffer* m_pMatrixBufferCB;
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__OBJMESHSHADER_H