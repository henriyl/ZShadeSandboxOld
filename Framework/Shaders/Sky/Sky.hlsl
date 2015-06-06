//==============================================================================
// Sky Shader
//
//==============================================================================
// History
//
// -Created on 4/8/2015 by Dustin Watson
//==============================================================================

//
// Constants
//

cbuffer cbPerFrame : register(b0)
{
	matrix g_World;
	matrix g_View;
	matrix g_Proj;
};

//======================================================================================================

//
// Globals
//

TextureCube g_CubeMap 	 	 : register(t0);
SamplerState samTriLinearSam : register(s0);

//======================================================================================================

//
// Normal Sky
//

struct VertexInput
{
	float3 position : POSITION;
	float3 normal	: NORMAL;
	float2 uv		: TEXCOORD0;
};

struct VertexOutput
{
	float4 position  : SV_POSITION;
    float3 positionL : POSITION;
	float3 normal	 : NORMAL;
	float2 uv		 : TEXCOORD0;
};

VertexOutput SkyVS(VertexInput input)
{
	VertexOutput output;
	
	output.position = mul(float4(input.position, 1.0f), g_World);
    output.position = mul(output.position, g_View);
    output.position = mul(output.position, g_Proj);
	
	// Use local vertex position as cubemap lookup vector.
	output.positionL = input.position;
	output.uv = input.uv;
	
	output.normal = input.normal;
	
	return output;
}

float4 SkyPS(VertexOutput input) : SV_Target
{
	return g_CubeMap.Sample(samTriLinearSam, input.positionL);
}

float4 SkyWireframePS(VertexOutput input) : SV_Target
{
    return float4(0.9f, 0.9f, 0.9f, 1);
}

//======================================================================================================

//
// Deferred Sky
//

struct DeferredVertexInput
{
	float3 position : POSITION;
	float3 normal	: NORMAL;
	float2 uv 		: TEXCOORD0;
};

struct DeferredVertexOutput
{
	float4 position  : SV_POSITION;
    float3 positionL : POSITION;
	float3 normal	 : NORMAL;
	float2 uv 		 : TEXCOORD0;
	float2 depth 	 : TEXCOORD1;
};

DeferredVertexOutput DeferredSkyVS(DeferredVertexInput input)
{
	DeferredVertexOutput output;
	
	output.position = mul(float4(input.position, 1.0f), g_World);
    output.position = mul(output.position, g_View);
    output.position = mul(output.position, g_Proj);
	
	// Use local vertex position as cubemap lookup vector.
	output.positionL = input.position;
	output.uv = input.uv;
	
	output.normal = mul(input.normal, (float3x3)g_World);
	output.normal = normalize(output.normal);
	
	output.depth.x = output.position.z;
	output.depth.y = output.position.w;
	
	return output;
}

struct PixelOut
{
	float4 colors0	: SV_Target0;
	float4 colors1	: SV_Target1;
	float4 normals	: SV_Target2;
	float4 depth	: SV_Target3;
};

PixelOut DeferredSkyPS(DeferredVertexOutput input) : SV_Target
{
	PixelOut output;
	
	output.colors0 = g_CubeMap.Sample(samTriLinearSam, input.positionL);
	output.colors1 = float4(1, 1, 1, 1);
	output.colors1.a = 0.2f;
	output.normals = float4(input.normal, 100.0f);
	output.depth = float4(input.depth.x / input.depth.y, input.positionL.x, input.positionL.y, input.positionL.z);
	
	return output;
}

PixelOut DeferredSkyWireframePS(DeferredVertexOutput input) : SV_Target
{
	PixelOut output;
    
	output.colors0 = float4(0.9f, 0.9f, 0.9f, 0);
	output.colors1 = float4(0.9f, 0.9f, 0.9f, 0);
	output.normals = float4(0.5f, 0.5f, 0.5f, 0);
	output.depth = float4(1, 0, 0, 0);
	
	return output;
}
