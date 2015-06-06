//==============================================================================
// Ocean Surface Domain Shader
//
//==============================================================================
// History
//
// -Created on 5/30/2015 by Dustin Watson
//==============================================================================

//
// Constants
//

cbuffer cbPerFrame : register(b0)
{
	float2	g_padding;
	float	g_SeaLevel;
	float 	g_WaveHeight;
	float3 	g_LightDirection;
	float 	g_SpecularShininess;
	float3 	g_CamPos;
	float 	g_Time;
	float4 	g_RefractionTint;
	float4  g_DullColor;
};

cbuffer cMatrixBuffer : register(b1)
{
	matrix g_WorldMatrix;
	matrix g_ViewMatrix;
	matrix g_ProjMatrix;
	matrix g_ReflectionView;
};

//======================================================================================================

//
// Textures and samplers
//

Texture2D g_texDisplacement	: register(t0); // FFT wave displacement map in VS (wave displacement from ocean heightfield)

SamplerState pointSampler	: register(s0);

SamplerState samHeightmap
{
	Filter = MIN_MAG_LINEAR_MIP_POINT;
	AddressU = WRAP;
	AddressV = WRAP;
};

SamplerState linearSampler
{
	Filter = MIN_MAG_LINEAR_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

//======================================================================================================

//
// Structs
//

struct PatchConstOutput
{
	float edges[4]		: SV_TessFactor;
	float inside[2]		: SV_InsideTessFactor;
};

struct DomainInput
{
	float3 position		: POSITION;
	float2 uv			: TEXCOORD0;
};

struct PixelInput
{
	float4 position					: SV_POSITION;
	float3 positionW				: POSITION;
	float2 uv						: TEXCOORD0;
	float2 bumpMapSamplingPos1      : TEXCOORD1;
	float2 bumpMapSamplingPos2      : TEXCOORD2;
	float4 reflectionMapSamplingPos : TEXCOORD3;
	float4 refractionMapSamplingPos : TEXCOORD4;
	float4 position3D               : TEXCOORD5;
	float3 viewDirection            : TEXCOORD6;
};

//

//
// Domain Shader
//

[domain("quad")]
PixelInput OceanSurfaceDS(PatchConstOutput input, float2 uv : SV_DomainLocation, const OutputPatch<DomainInput, 4> inputPatch)
{
	PixelInput output;
	
	// Bilinear interpolation.
	
	// Find the position of the new vertex
	float3 vPos = lerp(lerp(inputPatch[0].position, inputPatch[1].position, uv.x), lerp(inputPatch[2].position, inputPatch[3].position, uv.x), uv.y);
	
	// Find the texture coordinate of the new vertex
	float2 vTex = lerp(lerp(inputPatch[0].uv, inputPatch[1].uv, uv.x), lerp(inputPatch[2].uv, inputPatch[3].uv, uv.x), uv.y);
	
	//float4x4 matLocal = 
	// Local position
	//float4 pos_local = mul(float4(vPos, 1), g_WorldMatrix);
	// UV
	//float2 uv_local = pos_local.xy * (1 / 512) + (0.5 / 512);
	
	// Displacement map
	//float4 displacement = g_texDisplacement.SampleLevel(samHeightmap, vTex, 0);
	//float4 displacement = g_texDisplacement.Sample(linearSampler, vTex);
	
	//vPos.x += displacement.x;
	//vPos.y += displacement.y;
	//vPos.z += displacement.z;
	
	// Calculate the new position of the new vertex with WVP matrices
	//output.position = mul(float4(vPos, 1.0f), g_WorldMatrix);
	//output.position = mul(output.position, g_ViewMatrix);
	//output.position = mul(output.position, g_ProjMatrix);
	
	// Send the texture uv to the pixel shader
	output.uv = vTex;
	
	float4x4 viewProj = mul(g_ViewMatrix, g_ProjMatrix);
	float4x4 worldViewProj = mul(g_WorldMatrix, viewProj);
	float4x4 reflectionViewProjection = mul(g_ReflectionView, g_ProjMatrix);
	float4x4 worldReflectionViewProjection = mul(g_WorldMatrix, reflectionViewProjection);

	//
	//Calculate the refraction and reflection view matrices the same as before also. 
	//
	
	float4 pos = float4(vPos, 1);
	
	// Calculate the input position against the viewProjectWorld matrix.
	output.position = mul(float4(vPos, 1.0f), worldViewProj);
	output.reflectionMapSamplingPos = mul(float4(vPos, 1.0f), worldReflectionViewProjection);
	output.refractionMapSamplingPos = mul(float4(vPos, 1.0f), worldViewProj);
	output.position3D = mul(float4(vPos, 1.0f), g_WorldMatrix);
	
	//
	//Calculate the camera's view direction for fresnel and specular calculations.
	//
	
    // Calculate the position of the vertex in the world.
    float3 worldPosition = mul(float4(vPos, 1.0f), g_WorldMatrix);
	
    // Determine the viewing direction based on the position of the camera and the position of the vertex in the world.
    output.viewDirection = g_CamPos.xyz - worldPosition.xyz;

    // Normalize the viewing direction vector.
    output.viewDirection = normalize(output.viewDirection);
	
	output.bumpMapSamplingPos1 = vTex / 0.01f;
	output.bumpMapSamplingPos2 = vTex / 0.02f;
	
	return output;
}