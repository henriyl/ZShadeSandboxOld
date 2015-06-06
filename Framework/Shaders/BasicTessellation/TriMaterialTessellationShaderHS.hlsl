//==============================================================================
// Triangle Material Tessellation Hull Shader
//
//==============================================================================
// History
//
// -Created on 5/12/2015 by Dustin Watson
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

struct HullInput
{
	float3 position		: POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD0;
};

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

PatchConstOutput ConstHSMaterialTri(InputPatch<HullInput, 3> inputPatch, uint patchID : SV_PrimitiveID)
{
	PatchConstOutput output;
	
	output.edges[0] = g_TessellationFactor;
	output.edges[1] = g_TessellationFactor;
	output.edges[2] = g_TessellationFactor;
	
	output.inside = g_TessellationFactor;
	
	return output;
}

[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("ConstHSMaterialTri")]
DomainInput TriMaterialTessellationShaderHS(InputPatch<HullInput, 3> inputPatch, uint pointID : SV_OutputControlPointID, uint patchID : SV_PrimitiveID)
{
	DomainInput output;
	
	output.position = inputPatch[pointID].position;
	
	output.uv = inputPatch[pointID].uv;
	
	output.normal = inputPatch[pointID].normal;
	
	return output;
}