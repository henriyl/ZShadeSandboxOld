//==============================================================================================================================
// TerrainParameters.h
//==============================================================================================================================
#ifndef TERRAINPARAMETERS_H
#define TERRAINPARAMETERS_H
//==============================================================================================================================
//==============================================================================================================================

//
// Includes
//

#include "ZMath.h"
#include "Heightmap.h"
#include "LightManager.h"
#include "LightShadingBuffers.h"

//==============================================================================================================================
//==============================================================================================================================
namespace ZShadeSandboxTerrain {
struct TerrainParameters
{
	string 		g_heightmapName;
	int 		g_TextureRepeat;
	int 		g_TerrainSize;
	int 		g_leafWidth;
	float 		g_cellSpacing;
	float 		g_heightScale;
	int 		g_extension;
	bool 		g_tessellate;
	bool 		g_procedural;
	int 		g_terrScale;
	float 		g_seaLevel;
	int			g_RenderPrimitive;
	
	TerrainParameters()
	{
		g_heightmapName = "";
		g_TextureRepeat = 8;
		g_TerrainSize = 16;
		g_leafWidth = 5;
		g_cellSpacing = 0.5f;
		g_heightScale = 2;
		g_terrScale = 2;
		g_extension = EHeightExtension::HENONE;
		g_tessellate = false;
		g_procedural = false;
		g_seaLevel = 0;
		g_RenderPrimitive = ZShadeSandboxMesh::ERenderType::e4ControlPointPatchList;
	}
};
//==============================================================================================================================
//==============================================================================================================================
struct TerrainShadingConst
{
	// Position of the camera
	XMFLOAT3        g_EyePosW;
	float			g_DetailBrightness;
	
	// The parameters for fog
	float       	g_fogAltitude;
	float       	g_fogNear;
	float       	g_fogFar;
	float       	g_fogThinning;
	XMFLOAT4      	g_fogColor;
	
	// The parameters to scale the terrain
	//float       	g_WorldCellSpace;
	
	// The Terrain texture parameters
	float       	g_TextureAmount;
	float			g_TextureWidth;
	float			g_TextureHeight;
	
	// The parameters for terrain/water updating
	float       	g_seaLevel;
	XMFLOAT4      	g_waterBodyColor;
	
	//// The direction and color of the sun
	//XMFLOAT3	g_SunDir;
	//int			g_EnableSun;
	//XMFLOAT4    g_SunDiffuseColor;
	//float		g_SunShineness;
	
	//// Forward lighting
	//ZShadeSandboxLighting::AmbientLightBuffer		g_AmbientLight[MAX_LIGHTS];
	//ZShadeSandboxLighting::DirectionalLightBuffer	g_DirectionalLight[MAX_LIGHTS];
	//ZShadeSandboxLighting::SpotLightBuffer			g_SpotLight[MAX_LIGHTS];
	//ZShadeSandboxLighting::PointLightBuffer		g_PointLight[MAX_LIGHTS];
	//ZShadeSandboxLighting::CapsuleLightBuffer		g_CapsuleLight[MAX_LIGHTS];
	//
	//int				 g_AmbientLightCount;
	//int				 g_DirectionalLightCount;
	//int				 g_SpotLightCount;
	//int				 g_PointLightCount;
	//int              g_CapsuleLightCount;
	//
	//XMFLOAT3	g_AmbientDown;
	//XMFLOAT3	g_AmbientUp;
	
	// Terrain Material
	XMFLOAT4	g_MaterialDiffuseColor;
	XMFLOAT4	g_MaterialAmbientColor;
	float		g_MaterialSpecularPower;
	float		g_MaterialSpecularIntensity;

	// Control variables
	int        		g_useClippingPlane;
	//int        		g_cameraUnderwater;
	int        		g_useNormalMap;
	int				g_UseSobelFilter;
	int        		g_useShadowMap;
	int				g_useSSAO;
	
	// Are we performing frustum culling
	int  			g_FrustumCull;
	
	// Amount of tessellation to perform
	float			g_MinDist;
	float			g_MaxDist;
	float			g_MinTess;
	float			g_MaxTess;
	
	float			g_MapSize;
	float   		g_FarPlane;
	
	// The parameters to scale the terrain
	float       	g_TerrSize;
	float       	g_HeightScale;
	float       	g_TexelCellSpaceU;
	float       	g_TexelCellSpaceV;

	// The parameters for terrain/water updating
	XMFLOAT4      	g_ClipPlane;
};
}
//==============================================================================================================================
//==============================================================================================================================
#endif//TERRAINPARAMETERS_H