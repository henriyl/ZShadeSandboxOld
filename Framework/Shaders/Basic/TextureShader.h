//===============================================================================================================================
// TextureShader
//
//===============================================================================================================================
// History
//
// -Created on 6/18/2014 by Dustin Watson
//===============================================================================================================================
#ifndef __TEXTURESHADER_H
#define __TEXTURESHADER_H
//===============================================================================================================================
//===============================================================================================================================
//Includes
#include "D3D.h"
#include "Shader.h"
#include "Vertex.h"
using namespace std;
//===============================================================================================================================
//===============================================================================================================================
class TextureShader : public ZShadeSandboxShader::Shader
{
	struct Const_Per_Frame
	{
		XMFLOAT4 g_ClipPlane;
		XMFLOAT2 padding;
		int	     g_FlipTextureH;
		int	     g_FlipTextureV;
	};
	
	struct Const_Per_Object
	{
		XMFLOAT4X4	g_matWorld;
		XMFLOAT4X4	g_matView;
		XMFLOAT4X4	g_matProj;
	};

public:
	TextureShader(D3D* d3d);
	TextureShader(const TextureShader& other);
	~TextureShader();
	
	bool Initialize();
	void Shutdown();
	
	// true - flips the texture Horizontally
	// false - flips the texture Vertically
	void FlipTextureHorizontally(bool flip);
	
	bool Render11(int indexCount, ZShadeSandboxMath::XMMath4 clipplane, Camera* camera, ID3D11ShaderResourceView* texture);
	
private:
	
	bool bFlipHorizontally;
	bool bFlipVertically;
	
	ID3D11Buffer* m_pPerFrameCB;
	ID3D11Buffer* m_pPerObjectCB;
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__TEXTURESHADER_H