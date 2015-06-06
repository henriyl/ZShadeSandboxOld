//==============================================================================
// Material Tessellation Vertex Shader
//
//==============================================================================
// History
//
// -Created on 5/12/2015 by Dustin Watson
//==============================================================================

//
// Constants
//

//======================================================================================================

//
// Vertex Shader
//

struct VertexInput
{
	float3 position		: POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD0;
};

struct HullInput
{
	float3 position		: POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD0;
};

HullInput MaterialTessellationShaderVS(VertexInput input)
{
	HullInput output;
	
	// Pass vertex position into the hull shader
	output.position = input.position;
	
	// Pass texture uv and normal into the hull shader
	output.uv = input.uv;
	output.normal = input.normal;
	
	return output;
}