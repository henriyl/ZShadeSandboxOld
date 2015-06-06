//==============================================================================
// OBJMesh Tessellation Domain Shader
//
//==============================================================================
// History
//
// -Created on 4/15/2015 by Dustin Watson
//==============================================================================

//
// Constants
//

cbuffer cbDomainConst : register(b0)
{
	float4  g_ClipPlane;
	float3	padding;
	float	g_FarPlane;
};

cbuffer cbMatrixBuffer : register(b1)
{
	matrix g_WVPMatrix;
	//matrix g_WorldMatrix;
	//matrix g_World;
	//matrix g_View;
	//matrix g_Proj;
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
	float3 normal		: NORMAL;
	float pixelDepth	: DEPTH;
	float2 uv			: TEXCOORD0;
	float clip			: TEXCOORD1;
};

[domain("tri")]
PixelInput OBJMeshTessellationDS(PatchConstOutput input, float3 uvw : SV_DomainLocation, const OutputPatch<DomainInput, 3> inputPatch)
{
	PixelInput output;
	
	// Find the position of the new vertex
	float3 vPos = uvw.x * inputPatch[0].position + uvw.y * inputPatch[1].position + uvw.z * inputPatch[2].position;
	
	// Find the normal of the new vertex
	float3 vNormal = uvw.x * inputPatch[0].normal + uvw.y * inputPatch[1].normal + uvw.z * inputPatch[2].normal;
	
	// Find the texture coordinate of the new vertex
	float2 vTex = uvw.x * inputPatch[0].uv + uvw.y * inputPatch[1].uv + uvw.z * inputPatch[2].uv;
	
	// Calculate the new position of the new vertex with WVP matrices
	output.position = mul(float4(vPos, 1.0f), g_WVPMatrix);
	//output.position = mul(float4(vPos, 1.0f), g_World);
	//output.position = mul(output.position, g_View);
	//output.position = mul(output.position, g_Proj);
	
	// Logarithmic z buffer
	// DirectX Depth range 0..1
	int C = 1;
	output.pixelDepth = log2(max(1e-6, C * output.position.z + 1)) * 2 / log2(C * g_FarPlane + 1) * output.position.z;
	
	// Send the normal to the pixel shader
	output.normal = vNormal;
	
	// Send the texture uv to the pixel shader
	output.uv = vTex;
	
	output.clip = dot(output.position, g_ClipPlane);
	
	return output;
}