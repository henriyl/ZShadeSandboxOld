//==============================================================================
// Material Tessellation Hull Shader
//
//==============================================================================
// History
//
// -Created on 4/8/2015 by Dustin Watson
//==============================================================================

//
// Constants
//

cbuffer cTessellationBuffer : register(b0)
{
	float g_TessellationFactor;
	float3 padding;
};

//======================================================================================================

//
// Hull Shader
//

struct HullInputTexture
{
	float3 position		: POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD0;
};

struct PatchConstOutput
{
	float edges[4]		: SV_TessFactor;
	float inside[2]		: SV_InsideTessFactor;
};

struct DomainInputTexture
{
	float3 position		: POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD0;
};

PatchConstOutput ConstHSMaterialQuad(InputPatch<HullInputTexture, 4> inputPatch, uint patchID : SV_PrimitiveID)
{
	PatchConstOutput output;
	
	output.edges[0] = g_TessellationFactor;
	output.edges[1] = g_TessellationFactor;
	output.edges[2] = g_TessellationFactor;
	output.edges[3] = g_TessellationFactor;
	
	output.inside[0] = g_TessellationFactor;
	output.inside[1] = g_TessellationFactor;
	
	return output;
}

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("ConstHSMaterialQuad")]
DomainInputTexture QuadMaterialTessellationShaderHS(InputPatch<HullInputTexture, 4> inputPatch, uint pointID : SV_OutputControlPointID, uint patchID : SV_PrimitiveID)
{
	DomainInputTexture output;
	
	output.position = inputPatch[pointID].position;
	
	output.uv = inputPatch[pointID].uv;
	
	output.normal = inputPatch[pointID].normal;
	
	return output;
}