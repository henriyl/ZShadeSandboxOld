//==============================================================================================================================
// LightShadingBuffers.h
//
// buffers needed for lighting needed in a shader
//==============================================================================================================================
// History
//
// -Created on 4/20/2015  by Dustin Watson
//==============================================================================================================================
#ifndef __LIGHTSHADERBUFFERS_H
#define __LIGHTSHADERBUFFERS_H
//==============================================================================================================================
//==============================================================================================================================
#include "D3D.h"
#include "LightManager.h"
//==============================================================================================================================
//==============================================================================================================================
namespace ZShadeSandboxLighting {
// These structs are for passing data to the packed constant buffers so the human does not need to worry about packing
struct AmbientLightBuffer
{
	XMFLOAT4	g_AmbientColor;
};
//==============================================================================================================================
//==============================================================================================================================
struct DirectionalLightBuffer
{
	XMFLOAT3	g_LightDirection;
	XMFLOAT4	g_AmbientColor;
	XMFLOAT4	g_DiffuseColor;
};
//==============================================================================================================================
//==============================================================================================================================
struct SpotLightBuffer
{
	XMFLOAT4	g_AmbientColor;
	XMFLOAT4	g_DiffuseColor;
	XMFLOAT3	g_LightPosition;
	float		g_LightRange;
	float		g_SpotCosOuterCone;
	float		g_SpotInnerConeReciprocal;
	float		g_CosineAngle;
};
//==============================================================================================================================
//==============================================================================================================================
struct PointLightBuffer
{
	XMFLOAT3	g_LightPosition;
	float		g_LightRange;
	XMFLOAT3	g_Attenuation;
	XMFLOAT4	g_AmbientColor;
	XMFLOAT4	g_DiffuseColor;
};
//==============================================================================================================================
//==============================================================================================================================
struct CapsuleLightBuffer
{
	XMFLOAT3 	g_LightPosition;
	float 		g_LightRange;
	XMFLOAT3 	g_LightDirection;
	float 		g_LightLength;
	XMFLOAT3 	g_CapsuleDirectionLength;
	float 		g_CapsuleIntensity;
	XMFLOAT4	g_AmbientColor;
	XMFLOAT4	g_DiffuseColor;
};
//==============================================================================================================================
//==============================================================================================================================
struct LightBuffer
{
	AmbientLightBuffer			g_AmbientLight[MAX_LIGHTS];
	DirectionalLightBuffer		g_DirectionalLight[MAX_LIGHTS];
	SpotLightBuffer				g_SpotLight[MAX_LIGHTS];
	PointLightBuffer			g_PointLight[MAX_LIGHTS];
	CapsuleLightBuffer			g_CapsuleLight[MAX_LIGHTS];
	
	int				 			g_DirectionalLightCount;
	int				 			g_SpotLightCount;
	int				 			g_PointLightCount;
	int              			g_CapsuleLightCount;
	
	XMFLOAT3					g_AmbientDown;
	int							g_AmbientLightCount;
	XMFLOAT3					g_AmbientUp;
};
//==============================================================================================================================
//==============================================================================================================================
struct SunLightBuffer
{
	XMFLOAT3	g_SunDir;
	int			g_EnableSun;
	XMFLOAT4	g_SunDiffuseColor;
	float		g_SunShineness;
};
//==============================================================================================================================
//==============================================================================================================================
// These structs are properly packed for the shading constant buffer
struct cbAmbientLightBuffer
{
	XMFLOAT4	g_AmbientColor;
};
//==============================================================================================================================
//==============================================================================================================================
struct cbDirectionalLightBuffer
{
	XMFLOAT3	g_Direction;
	float		padding;
	XMFLOAT4	g_Ambient;
	XMFLOAT4	g_Diffuse;
};
//==============================================================================================================================
//==============================================================================================================================
struct cbSpotLightBuffer
{
	XMFLOAT4	g_AmbientColor;
	XMFLOAT4	g_DiffuseColor;
	XMFLOAT3	g_LightPosition;
	float		padding;
	float		g_LightRange;
	float		g_SpotCosOuterCone;
	float		g_SpotInnerConeReciprocal;
	float		g_CosineAngle;
};
//==============================================================================================================================
//==============================================================================================================================
struct cbPointLightBuffer
{
	XMFLOAT3	g_LightPosition;
	float		g_LightRange;
	XMFLOAT3	g_Attenuation;
	float		padding;
	XMFLOAT4	g_AmbientColor;
	XMFLOAT4	g_DiffuseColor;
};
//==============================================================================================================================
//==============================================================================================================================
struct cbCapsuleLightBuffer
{
	XMFLOAT3 	g_LightPosition;
	float 		g_LightRange;
	XMFLOAT3 	g_LightDirection;
	float 		g_LightLength;
	XMFLOAT3 	g_CapsuleDirectionLength;
	float 		g_CapsuleIntensity;
	XMFLOAT4	g_AmbientColor;
	XMFLOAT4	g_DiffuseColor;
};
//==============================================================================================================================
//==============================================================================================================================
struct cbLightBuffer
{
	cbAmbientLightBuffer		g_AmbientLight[MAX_LIGHTS];
	cbDirectionalLightBuffer	g_DirectionalLight[MAX_LIGHTS];
	cbSpotLightBuffer			g_SpotLight[MAX_LIGHTS];
	cbPointLightBuffer			g_PointLight[MAX_LIGHTS];
	cbCapsuleLightBuffer		g_CapsuleLight[MAX_LIGHTS];
	
	int				 			g_DirectionalLightCount;
	int				 			g_SpotLightCount;
	int				 			g_PointLightCount;
	int              			g_CapsuleLightCount;
	
	XMFLOAT3					g_AmbientDown;
	int							g_AmbientLightCount;
	XMFLOAT3					g_AmbientUp;
	float						padding;
};
//==============================================================================================================================
//==============================================================================================================================
struct cbSunLightBuffer
{
	XMFLOAT3	g_SunDir;
	int			g_EnableSun;
	XMFLOAT4	g_SunDiffuseColor;
	XMFLOAT3	padding;
	float		g_SunShineness;
};
}
//==============================================================================================================================
//==============================================================================================================================
#endif//__LIGHTSHADERBUFFERS_H