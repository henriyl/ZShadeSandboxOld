//==============================================================================
// OBJMesh Tessellation Vertex Shader
//
//==============================================================================
// History
//
// -Created on 4/15/2015 by Dustin Watson
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

HullInput OBJMeshTessellationVS(VertexInput input)
{
	HullInput output;
	
	// Pass vertex position into the hull shader
	output.position = input.position;
	
	// Pass normal into the hull shader
	output.normal = input.normal;
	
	// Pass texture uv into the hull shader
	output.uv = input.uv;
	
	return output;
}