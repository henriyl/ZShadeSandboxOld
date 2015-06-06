//===============================================================================================================================
// CustomMeshParameters.h
//
//===============================================================================================================================
// History
//
// -Created on 4/14/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __CUSTOMMESHPARAMETERS_H
#define __CUSTOMMESHPARAMETERS_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//

#include "D3D.h"
#include "Camera.h"
#include "Vertex.h"
#include "MeshRenderType.h"
#include "ShaderMaterial.h"

namespace ZShadeSandboxShader {
	class Shader;
}

//===============================================================================================================================
//===============================================================================================================================
namespace ZShadeSandboxMesh {
struct MeshParameters
{
	int vertexType;
	bool rotationAxisX;
	bool rotationAxisY;
	bool rotationAxisZ;
	XMFLOAT3 pos;
	XMFLOAT3 rot;
	XMFLOAT3 scale;
	ZShadeSandboxLighting::ShaderMaterial* material;
	//XMFLOAT4 color;
	//string texturePath;   // Full path to the texture
	//string textureName;   // Name of the texture without full path
	//TextureType tt;
	int textureWidth;
	int textureHeight;
	//bool useTexture;
	ZShadeSandboxShader::Shader* shader;
	bool in2D;
	bool useCustomShader;
	//bool useCustomShaderTextureNoCubemap; // Wanting a custom shader with a texture but not a cubemap
	bool bUseTessellation; // Decide if we are loading tessellation
	
	ZShadeSandboxMesh::MeshParameters()
	{
		useCustomShader = false;
		vertexType = EVertexType::VT_Color;
		rotationAxisX = false;
		rotationAxisY = false;
		rotationAxisZ = false;
		pos = XMFLOAT3(0,0,0);
		rot = XMFLOAT3(0,0,0);
		scale = XMFLOAT3(1,1,1);
		//color = XMFLOAT4(0,0,0,1);
		//textureName = "";
		//tt = PNG;
		in2D = true;
		textureWidth = -1;
		textureHeight = -1;
		shader = 0;
		//useCubemap = false;
		//useTexture = false;
		bUseTessellation = false;
		material = 0;
	}
};

struct MeshRenderParameters
{
	Camera* pCamera;
	LightCamera* pLightCamera;
	ZShadeSandboxLighting::DirectionalLight* dirLight;
	XMMATRIX world; // This will be added by the mesh, unless bSpecifyWorld=true
	XMFLOAT4X4 view;  // This will be added by the mesh
	ERenderType::Type renderType;
	bool bReflection;
	//bool bTransparent;
	bool bCenterCam;   // Centers the camera in the mesh
	bool bTessellate;
	bool bSpecifyWorld; // Can override what the mesh adds for the world matrix with a custom world
	bool bShadowMap; // Renders the mesh with a shadow map
	bool bRenderDeferred; // Enables deferred shading
	float fTessellationFactor;
	//float blendAmount;
	float fplaneHeight;
	XMFLOAT4 clipplane;
	
	ZShadeSandboxMesh::MeshRenderParameters()
	{
		pCamera = 0;
		pLightCamera = 0;
		dirLight = 0;
		renderType = ERenderType::eTriangleList;
		bReflection = false;
		//bTransparent = false;
		bCenterCam = false;
		bTessellate = false;
		bSpecifyWorld = false;
		bShadowMap = false;
		bRenderDeferred = false;
		fTessellationFactor = 6.0f;
		//blendAmount = 0;
		fplaneHeight = 0;
		clipplane = XMFLOAT4(0, 0, 0, 0);
	}
};
}
//===============================================================================================================================
//===============================================================================================================================
#endif//__CUSTOMMESHPARAMETERS_H