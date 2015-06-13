//==============================================================================
// Static OBJ Mesh Tessellation Shader
//
//==============================================================================
// History
//
// -Created on 4/15/2015 by Dustin Watson
// -Updated on 6/7/2015 by Dustin Watson
//==============================================================================
#include "../../Environment/Lighting/Lighting.hlsl"

//
// Constants
//

cbuffer cTessellationBuffer : register(b0)
{
	float3 	g_EyePosW;
	float 	g_TessellationFactor;
	float 	g_MinDist;
	float 	g_MaxDist;
	float 	g_MinTess;
	float 	g_MaxTess;
};

cbuffer cbDomainBuffer : register(b1)
{
	float4  g_ClipPlane;
	float3	padding;
	float	g_FarPlane;
	matrix 	g_World;
	matrix 	g_View;
	matrix 	g_Proj;
};

cbuffer cbOBJShadingBuffer : register(b2)
{
	float4	g_DiffuseColor;
	float4	g_AmbientColor;
	float4	g_SpecularColor;
	float4	g_EmissiveColor;
	float3	g_TransmissionFilter;
	float	g_Alpha;
	float	g_SpecularPower;
	float	g_SpecularIntensity;
	float	g_RefractionIndex;
	float	g_DetailBrightness;
	int		g_IlluminationModel;
	int		g_HasDiffuseTexture;
	int		g_HasAmbientTexture;
	int		g_HasSpecularTexture;
	int		g_SpecularToggle;
	int		g_HasEmissiveTexture;
	int		g_HasNormalMap;
	int		g_HasDetailMap;
	float2	padding1;
	int		g_HasTransparency;
	int		g_HasAlphaTexture;
	float3	g_EyePosition;
	float	padding2;
};

//======================================================================================================

//
// Texturs and Samplers
//

Texture2D g_TexDiffuse		: register(t0);
Texture2D g_TexAmbient		: register(t1);
Texture2D g_TexSpecular		: register(t2);
Texture2D g_TexAlpha		: register(t3);
Texture2D g_TexNormalMap	: register(t4);
Texture2D g_TexEmissive		: register(t5);
Texture2D g_TexDetail		: register(t6);

SamplerState g_LinearSampler	: register(s0);

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

//======================================================================================================

//
// Hull Shader
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

float CalcTessFactor(float3 p)
{
	float d = distance(p, g_EyePosW);
	
	// max norm in xz plane (useful to see detail levels from a bird's eye).
	//float d = max( abs(p.x - g_EyePosW.x), abs(p.z - g_EyePosW.z) );
	
	float s = saturate( (d - g_MinDist) / (g_MaxDist - g_MinDist) );
	
	return pow(2, (lerp(g_MaxTess, g_MinTess, s)) );
}

void HCTessellate(out PatchConstOutput output, float factor)
{
	output.edges[0] = factor;
	output.edges[1] = factor;
	output.edges[2] = factor;
	
	output.inside = factor;
}

void HCTessellate(out PatchConstOutput output, float3 e0, float3 e1, float3 e2, float3 c)
{
	output.edges[0] = CalcTessFactor(e0);
	output.edges[1] = CalcTessFactor(e1);
	output.edges[2] = CalcTessFactor(e2);
	
	output.inside = CalcTessFactor(c);
}

PatchConstOutput ConstHS(InputPatch<HullInput, 3> inputPatch, uint patchID : SV_PrimitiveID)
{
	PatchConstOutput output;
	
	float3 WorldPosition0 = inputPatch[0].position;
	float3 WorldPosition1 = inputPatch[1].position;
	float3 WorldPosition2 = inputPatch[2].position;
	
	// Compute midpoint on edges, and patch center
	float3 e0 = 0.5f * (WorldPosition0 + WorldPosition1);
	float3 e1 = 0.5f * (WorldPosition1 + WorldPosition2);
	float3 e2 = 0.5f * (WorldPosition2 + WorldPosition0);
	
	float3  c = (WorldPosition0 + WorldPosition1 + WorldPosition2) / 3;
	
	HCTessellate(output, e0, e1, e2, c);
	
	return output;
}

[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("ConstHS")]
DomainInput OBJMeshTessellationHS(InputPatch<HullInput, 3> inputPatch, uint pointID : SV_OutputControlPointID, uint patchID : SV_PrimitiveID)
{
	DomainInput output;
	
	output.position = inputPatch[pointID].position;
	
	output.normal = inputPatch[pointID].normal;
	
	output.uv = inputPatch[pointID].uv;
	
	return output;
}

//======================================================================================================

//
// Domain Shader
//

struct PixelInput
{
	float4 position		: SV_POSITION;
	float3 positionW	: POSITION;
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
	
	output.positionW = vPos;
	
	// Calculate the new position of the new vertex with WVP matrices
	output.position = mul(float4(vPos, 1.0f), g_World);
	output.position = mul(output.position, g_View);
	output.position = mul(output.position, g_Proj);
	
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

//======================================================================================================

//
// Pixel Shader
//

float4 OBJMeshTessellationPS(PixelInput input) : SV_Target
{
	float4 finalColor;
	
	//============================================ Texture Color
	
	finalColor = (g_HasDiffuseTexture == 1) ? g_TexDiffuse.Sample(g_LinearSampler, input.uv) : g_DiffuseColor;
	finalColor += (g_HasAmbientTexture == 1) ? g_TexAmbient.Sample(g_LinearSampler, input.uv) : g_AmbientColor;
	finalColor += (g_HasSpecularTexture == 1) ? g_TexSpecular.Sample(g_LinearSampler, input.uv) : g_SpecularColor;
	finalColor += (g_HasEmissiveTexture == 1) ? g_TexEmissive.Sample(g_LinearSampler, input.uv) : g_EmissiveColor;
	
	//============================================ Normal Mapping
	
	float3 normal = input.normal;
	
	if (g_HasNormalMap == 1)
	{
		float4 normalMap = g_TexNormalMap.Sample(g_LinearSampler, input.uv);
		normal = (normalMap.rgb * 2.0f) - 1.0f;
		normal = normalize(normal);
	}
	
	//============================================ Detail Mapping
	
	if (g_HasDetailMap == 1)
	{
		float4 detailColor = g_TexDetail.Sample(g_LinearSampler, input.uv);
		finalColor *= detailColor * g_DetailBrightness;
	}
	
	//============================================ Alpha Map Blending (Alpha-to-Coverage)
	
	if (g_HasTransparency == 1)
	{
		if (g_HasAlphaTexture == 1)
		{
			float4 alphaMapColor = g_TexAlpha.Sample(g_LinearSampler, input.uv);
			if (alphaMapColor.x == g_Alpha && alphaMapColor.y == g_Alpha && alphaMapColor.z == g_Alpha)
			{
				clip(-1);
			}
		}
		else
		{
			clip(-1);
		}
	}
	
	clip( input.clip < 0.0 ? -1 : 1 );
	
	/*finalColor *= CalculateLightColor
	(	input.positionW
	,	g_EyePosition
	,	finalColor
	,	normal
	,	g_DiffuseColor
	,	g_AmbientColor
	,	g_SpecularPower
	,	g_SpecularIntensity
	,   1.0f
	,   1.0f
	);
	
	return finalColor;
	*/
	float3 toLight = normalize(float3(2, -5, 3));
	float I = saturate(dot(normalize(normal), toLight));
	I += 0.2f; // Ambient
	return finalColor * I;
}

float4 OBJMeshTessellationWireframePS(PixelInput input) : SV_Target
{
	clip( input.clip < 0.0 ? -1 : 1 );
	
    return float4(0.9f, 0.9f, 0.9f, 1);
}