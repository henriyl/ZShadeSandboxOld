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
public:
	
	QuadMaterialTessellationShader(D3D* d3d);
	QuadMaterialTessellationShader(const QuadMaterialTessellationShader& other);
	~QuadMaterialTessellationShader();
	
	bool Initialize();
	void Shutdown();
	
	bool Render11
	(	int indexCount
	,	ZShadeSandboxMesh::MeshRenderParameters mrp
	,	ZShadeSandboxLighting::ShaderMaterial* material
	);
	
private:
	
	ID3D11Buffer* m_pTessellationCB;
	ID3D11Buffer* m_pDomainCB;
	ID3D11Buffer* m_pShadingCB;
	ID3D11Buffer* m_pLightCB;
	ID3D11Buffer* m_pSunCB;
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__QUADMATERIALTESSELLATIONSHADER_H