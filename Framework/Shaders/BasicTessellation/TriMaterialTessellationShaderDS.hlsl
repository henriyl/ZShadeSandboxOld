//==============================================================================
// Triangle Material Tessellation Domain Shader
//
//==============================================================================
// History
//
// -Created on 5/12/2015 by Dustin Watson
//==============================================================================

//
// Constants
//

cbuffer cbPerFrame : register(b0)
{
	float3	g_EyePosW;
	int		g_UsingDiffuseArrayTexture;
	float4 	g_ClipPlane;
	float4	g_MaterialDiffuseColor;
	float4	g_MaterialAmbientColor;
	float	g_MaterialSpecularPower;
	float	g_MaterialSpecularIntensity;
	int	   	g_UsingDiffuseTexture;
	int	   	g_UsingNormalMapTexture;
	int	   	g_UsingBlendMapTexture;
	int	   	g_UsingDetailMapTexture;
	int		g_UseAlphaMapTexture;
	int		g_EnableTransparency;
	int		g_EnableLighting;
	float	g_DetailBrightness;
	int	   	g_FlipTextureH;
	int	   	g_FlipTextureV;
	float3	g_AlphaToCoverageValue; // Value that clips pixel during alpha blending
	float	mspadding;
};

cbuffer cMatrixBuffer : register(b1)
{
	matrix g_WorldMatrix;
	matrix g_ViewMatrix;
	matrix g_ProjMatrix;
};

//======================================================================================================

//
// Domain Shader
//

struct PatchConstOutput
{
	float edges[3]		: SV_TessFactor;
	float inside		: SV_InsideTessFactor;
};

struct DomainInput
{
	float3 position		: POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD0;
};

struct PixelInput
{
	float4 position		: SV_POSITION;
	float3 positionW	: POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD0;
	float  clip			: TEXCOORD1;
	float4 depth		: TEXCOORD2;
};

[domain("tri")]
PixelInput TriMaterialTessellationShaderDS(PatchConstOutput input, float3 uvw : SV_DomainLocation, const OutputPatch<DomainInput, 3> inputPatch)
{
	PixelInput output;
	
	// Find the position of the new vertex
	float3 vPos = uvw.x * inputPatch[0].position + uvw.y * inputPatch[1].position + uvw.z * inputPatch[2].position;
	
	// Find the normal of the new vertex
	float3 vNormal = uvw.x * inputPatch[0].normal + uvw.y * inputPatch[1].normal + uvw.z * inputPatch[2].normal;
	
	// Find the texture coordinate of the new vertex
	float2 vTex = uvw.x * inputPatch[0].uv + uvw.y * inputPatch[1].uv + uvw.z * inputPatch[2].uv;
	
	// Calculate the new position of the new vertex with WVP matrices
	output.position = mul(float4(vPos, 1.0f), g_WorldMatrix);
	output.position = mul(output.position, g_ViewMatrix);
	output.position = mul(output.position, g_ProjMatrix);
	
	// Send the texture uv to the pixel shader
	output.uv = vTex;
	
	// Send the normal to the pixel shader
	output.normal = vNormal;
	
	output.clip = dot(output.position, g_ClipPlane);
	output.depth = output.position;
	
	return output;
}