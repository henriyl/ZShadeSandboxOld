//==============================================================================
// Quad Texture Domain Shader
//
//==============================================================================
// History
//
// -Created on 4/8/2015 by Dustin Watson
//==============================================================================

//
// Constants
//

cbuffer cDomainConst : register(b0)
{
	float2  g_TexScale;
	float 	g_HeightScale;
	float 	g_TerrainZScale;
	float4 	g_ClipPlane;
	float3  g_tpadding;
	float   g_FarPlane;
};

cbuffer cMatrixBuffer : register(b1)
{
	matrix g_WorldMatrix;
	matrix g_ViewMatrix;
	matrix g_ProjMatrix;
	matrix g_TexSpaceMatrix;
	matrix g_ShadowMatrix;
};

//======================================================================================================

//
// Globals
//

Texture2D g_HeightMap : register(t0);

//------------------------------------------------------------------
//Sample States
//------------------------------------------------------------------
SamplerState samHeightmap
{
	Filter = MIN_MAG_LINEAR_MIP_POINT;
	AddressU = CLAMP;
	AddressV = CLAMP;
};

//======================================================================================================

//
// Domain Shader
//

struct PatchConstOutput
{
	float edges[4]		: SV_TessFactor;
	float inside[2]		: SV_InsideTessFactor;
};

struct DomainInput
{
	float3 position		: POSITION;
	//float3 normal		: NORMAL;
	float2 uv			: TEXCOORD0;
};

struct PixelInput
{
	float4 position		: SV_POSITION;
	float3 positionW	: POSITION;
	//float3 normal		: NORMAL;
	float pixelDepth	: DEPTH;
	float2 uv			: TEXCOORD0;
	float2 TiledTex 	: TEXCOORD1;
	float4 worldPos 	: TEXCOORD2;
	float clip      	: TEXCOORD3;
	float4 depth		: TEXCOORD4;
	float4 shadowPos	: TEXCOORD5;
	float4 ssaoPos		: TEXCOORD6;
};

[domain("quad")]
PixelInput TerrainTessellationQuadDS(PatchConstOutput input, float2 uv : SV_DomainLocation, const OutputPatch<DomainInput, 4> inputPatch)
{
	PixelInput output;
	
	// Bilinear interpolation.
	
	// Find the position of the new vertex
	float3 vPos = lerp(lerp(inputPatch[0].position, inputPatch[1].position, uv.x), lerp(inputPatch[2].position, inputPatch[3].position, uv.x), uv.y);
	
	// Find the normal of the new vertex
	//float3 vNormal = lerp(lerp(inputPatch[0].normal, inputPatch[1].normal, uv.x), lerp(inputPatch[2].normal, inputPatch[3].normal, uv.x), uv.y);
	
	// Find the texture coordinate of the new vertex
	float2 vTex = lerp(lerp(inputPatch[0].uv, inputPatch[1].uv, uv.x), lerp(inputPatch[2].uv, inputPatch[3].uv, uv.x), uv.y);
	
	// Send the normal to the pixel shader
	//output.normal = vNormal;
	
	// Send the texture uv to the pixel shader
	output.uv = vTex;
	
	// Tile layer textures over terrain.
	output.TiledTex = output.uv * g_TexScale;
	
	// Sample the height for this vertex
	float4 SampleValue = g_HeightMap.SampleLevel( samHeightmap, output.uv, 0 );
	vPos.y = ((SampleValue.r * g_HeightScale * 100.0) / 255.0) / ((g_TerrainZScale * 2) + 1);
	
	// Calculate independent world position
	output.worldPos = mul(float4(vPos, 1.0f), g_WorldMatrix);
	
	// Get the non-homogenous (untransformed) position of the final vertex
	output.positionW = vPos;
	
	// Calculate the new position of the new vertex with WVP matrices
	output.position = mul(float4(vPos, 1.0f), g_WorldMatrix);
	output.position = mul(output.position, g_ViewMatrix);
	output.position = mul(output.position, g_ProjMatrix);
	
	// Logarithmic z buffer
	// DirectX Depth range 0..1
	int C = 1;
	output.pixelDepth = log2(max(1e-6, C * output.position.z + 1)) * 2 / log2(C * g_FarPlane + 1) * output.position.z;
	
	// Determine where this vertex lies on the clipping plane
	output.clip = dot(output.position, g_ClipPlane);
	
	// Store the positions for depth calculations
	output.depth = output.position;
	
	// Calculate the position for shadow mapping
	output.shadowPos = mul(float4(vPos, 1.0f), g_ShadowMatrix);
	
	// Calculate the new SSAO position
	output.ssaoPos = mul(float4(vPos, 1.0f), g_ViewMatrix);
	output.ssaoPos = mul(output.ssaoPos, g_ProjMatrix);
	output.ssaoPos = mul(output.ssaoPos, g_TexSpaceMatrix);
	
	return output;
}