//==============================================================================
// Tri Material Tessellation Shader
//
//==============================================================================
// History
//
// -Created on 5/12/2015 by Dustin Watson
//==============================================================================
#include "../Environment/Lighting/Lighting.hlsl"

//======================================================================================================

//
// Constants
//

cbuffer cbTessellationBuffer : register(b2)
{
	float3 	g_EyePosW;
	float 	g_NoDistTessFactor;
	float 	g_MinTessDist;
	float 	g_MaxTessDist;
	float 	g_MinTessFactor;
	float 	g_MaxTessFactor;
	float3	tesspadding;
	int		g_EnableDistTess;
};

cbuffer cbDomainBuffer : register(b3)
{
	float4  g_ClipPlane;
	float3	domainpadding;
	float	g_FarPlane;
	matrix 	g_World;
	matrix 	g_View;
	matrix 	g_Proj;
};

cbuffer cbShadingBuffer : register(b4)
{
	float3	g_EyePosition;
	float	g_Alpha;
	float4	g_ClipPlanes;
	float4	g_MaterialDiffuseColor;
	float4	g_MaterialAmbientColor;
	float4	g_MaterialSpecularColor;
	float4	g_MaterialEmissiveColor;
	float	g_Emissivity;
	float	g_Reflectivity;
	float	g_Transmissivity;
	float	g_RefractionIndex;
	float3	g_AlphaToCoverageValue;
	float	g_DetailBrightness;
	float3	g_TransmissionFilter;
	float	g_MaterialSpecularPower;
	float	g_MaterialSpecularIntensity;
	int		g_IlluminationModel;
	int	   	g_FlipTextureH;
	int	   	g_FlipTextureV;
	int		g_UsingDiffuseArrayTexture;
	int	   	g_UsingDiffuseTexture;
	int	   	g_UsingAmbientTexture;
	int	   	g_UsingSpecularTexture;
	int	   	g_UsingEmissiveTexture;
	int	   	g_UsingNormalMapTexture;
	int	   	g_UsingBlendMapTexture;
	int	   	g_UsingDetailMapTexture;
	int		g_UsingAlphaMapTexture;
	int		g_UsingTransparency;
	int		g_UsingShadowMap;
	int		g_UsingSSAOMap;
	float	materialpadding;
	float	g_FarPlanes;
	int		g_SpecularToggle;
	int		g_EnableLighting;
};

cbuffer cbLightTypeBuffer : register(b5)
{
	int		g_UsingAmbientLight;
	int		g_UsingDirectionalLight;
	int		g_UsingPointLight;
	int		g_UsingSpotLight;
	float3	lighttypepadding;
	int		g_UsingCapsuleLight;
};

cbuffer cbAmbientLightBuffer : register(b6)
{
	float4	g_AmbientColor;
	float3	ambientpadding;
	int		g_AmbientToggle;
};

cbuffer cbDirectionalLightBuffer : register(b7)
{
	float3	g_DirectionalDirection;
	float	g_DirectionalIntensity;
	float4	g_DirectionalAmbient;
	float4	g_DirectionalDiffuse;
	float3	directionalpadding;
	int		g_DirectionalToggle;
};

cbuffer cbSpotLightBuffer : register(b8)
{
	float4	g_SpotAmbientColor;
	float4	g_SpotDiffuseColor;
	float3	g_SpotLightPosition;
	float	g_SpotIntensity;
	float	g_SpotLightRange;
	float	g_SpotCosOuterCone;
	float	g_SpotInnerConeReciprocal;
	float	g_SpotCosineAngle;
	float3	spotpadding;
	int		g_SpotToggle;
};

cbuffer cbPointLightBuffer : register(b9)
{
	float3	g_PointLightPosition;
	float	g_PointLightRange;
	float3	g_PointAttenuation;
	float	g_PointIntensity;
	float4	g_PointAmbientColor;
	float4	g_PointDiffuseColor;
	float3	pointpadding;
	int		g_PointToggle;
};

cbuffer cbCapsuleLightBuffer : register(b10)
{
	float3 	g_CapsuleLightPosition;
	float 	g_CapsuleLightRange;
	float3 	g_CapsuleLightDirection;
	float 	g_CapsuleLightLength;
	float3 	g_CapsuleDirectionLength;
	float 	g_CapsuleIntensity;
	float4	g_CapsuleAmbientColor;
	float4	g_CapsuleDiffuseColor;
	float3	capsulepadding;
	int		g_CapsuleToggle;
};

//======================================================================================================

//
// Texturs and Samplers
//

Texture2DArray g_MaterialDiffuseArrayTexture 	: register(t0);
Texture2D g_MaterialDiffuseTexture 				: register(t1);
Texture2D g_MaterialAmbientTexture 				: register(t2);
Texture2D g_MaterialSpecularTexture 			: register(t3);
Texture2D g_MaterialEmissiveTexture 			: register(t4);
Texture2D g_MaterialNormalMapTexture 			: register(t5);
Texture2D g_MaterialBlendMapTexture 			: register(t6);
Texture2D g_MaterialDetailMapTexture 			: register(t7);
Texture2D g_MaterialAlphaMapTexture 			: register(t8);
Texture2D g_ShadowMap							: register(t9);
Texture2D g_SSAOMap								: register(t10);

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

HullInput TriMaterialLightTessellationVS(VertexInput input)
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
	float inside : SV_InsideTessFactor;
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

	float s = saturate((d - g_MinTessDist) / (g_MaxTessDist - g_MinTessDist));

	return pow(2, (lerp(g_MaxTessFactor, g_MinTessFactor, s)));
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

	if (g_EnableDistTess == 1)
	{
		float3 WorldPosition0 = inputPatch[0].position;
			float3 WorldPosition1 = inputPatch[1].position;
			float3 WorldPosition2 = inputPatch[2].position;

			// Compute midpoint on edges, and patch center
			float3 e0 = 0.5f * (WorldPosition0 + WorldPosition1);
			float3 e1 = 0.5f * (WorldPosition1 + WorldPosition2);
			float3 e2 = 0.5f * (WorldPosition2 + WorldPosition0);

			float3  c = (WorldPosition0 + WorldPosition1 + WorldPosition2) / 3;

			HCTessellate(output, e0, e1, e2, c);
	}
	else
	{
		HCTessellate(output, g_NoDistTessFactor);
	}

	return output;
}

[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("ConstHS")]
DomainInput TriMaterialLightTessellationHS(InputPatch<HullInput, 3> inputPatch, uint pointID : SV_OutputControlPointID, uint patchID : SV_PrimitiveID)
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
	float4 depth		: TEXCOORD2;
};

[domain("tri")]
PixelInput TriMaterialLightTessellationDS(PatchConstOutput input, float3 uvw : SV_DomainLocation, const OutputPatch<DomainInput, 3> inputPatch)
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
	output.depth = output.position;

	return output;
}

//======================================================================================================

//
// Pixel Shader
//

float4 TriMaterialLightTessellationPS(PixelInput input) : SV_Target
{
	float4 finalColor;

	// 1 - u: flips the texture horizontally
	if (g_FlipTextureH == 1)
	{
		input.uv.x = 1 - input.uv.x;
	}

	// 1 - v: flips the texture vertically
	if (g_FlipTextureV == 1)
	{
		input.uv.y = 1 - input.uv.y;
	}

	//============================================ Texturing

	float4 finalTextureColor = float4(0, 0, 0, 1);
	float4 diffuseLayers0 = float4(0, 0, 0, 1);
	float4 diffuseLayers1 = float4(0, 0, 0, 1);
	float4 diffuseLayers2 = float4(0, 0, 0, 1);
	float4 diffuseLayers3 = float4(0, 0, 0, 1);

	if (g_UsingDiffuseArrayTexture == 1)
	{
		// Sample layers in texture array.
		diffuseLayers0 = g_MaterialDiffuseArrayTexture.Sample(g_LinearSampler, float3(input.uv, 0.0f));
		diffuseLayers1 = g_MaterialDiffuseArrayTexture.Sample(g_LinearSampler, float3(input.uv, 1.0f));
		diffuseLayers2 = g_MaterialDiffuseArrayTexture.Sample(g_LinearSampler, float3(input.uv, 2.0f));
		diffuseLayers3 = g_MaterialDiffuseArrayTexture.Sample(g_LinearSampler, float3(input.uv, 3.0f));
	}
	else
	{
		if (g_UsingDiffuseTexture == 1)
		{
			finalTextureColor = (g_UsingDiffuseTexture == 1) ? g_MaterialDiffuseTexture.Sample(g_LinearSampler, input.uv) : g_MaterialDiffuseColor;
			//finalTextureColor += (g_UsingAmbientTexture == 1) ? g_MaterialAmbientTexture.Sample(g_LinearSampler, input.uv) : g_MaterialAmbientColor;
			//finalTextureColor += (g_UsingSpecularTexture == 1) ? g_MaterialSpecularTexture.Sample(g_LinearSampler, input.uv) : g_MaterialSpecularColor;
			//finalTextureColor += (g_UsingEmissiveTexture == 1) ? g_MaterialEmissiveTexture.Sample(g_LinearSampler, input.uv) : g_MaterialEmissiveColor;
		}
	}

	//============================================ Blend Mapping

	if (g_UsingBlendMapTexture == 1)
	{
		if (g_UsingDiffuseArrayTexture == 1)
		{
			// Sample the blend map.
			float4 t = g_MaterialBlendMapTexture.Sample(g_LinearSampler, input.uv);

			// Blend the layers on top of each other.
			finalTextureColor = diffuseLayers0;
			finalTextureColor = lerp(finalTextureColor, diffuseLayers1, t.r);
			finalTextureColor = lerp(finalTextureColor, diffuseLayers2, t.g);
			finalTextureColor = lerp(finalTextureColor, diffuseLayers3, t.b);
		}
	}
	else
	{
		if (g_UsingDiffuseArrayTexture == 1)
			finalTextureColor = saturate(diffuseLayers0 + diffuseLayers1 + diffuseLayers2 + diffuseLayers3);
	}

	// Now add the texture color
	finalColor = finalTextureColor;

	//============================================ Normal Mapping

	float depth = input.depth.z / input.depth.w;

	float3 normal = input.normal;

	if (g_UsingNormalMapTexture == 1)
	{
		float4 normalMap = g_MaterialNormalMapTexture.Sample(g_LinearSampler, input.uv);
		normal = (normalMap.rgb * 2.0f) - 1.0f;
		normal = normalize(normal);
	}

	//============================================ Shadow Mapping

	float shadowColor = 1.0f;

	//============================================ SSAO

	float ssaoColor = 1.0f;

	//============================================ Detail Mapping

	// Check if the depth value is close to the screen
	if (g_UsingDetailMapTexture == 1)
	{
		float4 detailColor = g_MaterialDetailMapTexture.Sample(g_LinearSampler, input.uv);
		finalColor *= detailColor * g_DetailBrightness;
	}

	//============================================ Lighting

	float3 lightColor = float3(1, 1, 1);

	if (g_EnableLighting == 1)
	{
		if (g_UsingAmbientLight == 1 && g_AmbientToggle == 1)
		{
			lightColor = g_AmbientColor;
		}

		if (g_UsingDirectionalLight == 1 && g_DirectionalToggle == 1)
		{
			lightColor = DirectionalLightColor
			(	input.positionW
			,	g_EyePosW
			,	g_DirectionalDirection
			,	g_DirectionalIntensity
			,	g_DirectionalDiffuse
			,	normal
			,	g_MaterialDiffuseColor
			,	g_MaterialSpecularPower
			,	g_MaterialSpecularIntensity
			,	shadowColor
			);
		}

		if (g_UsingPointLight == 1 && g_AmbientToggle == 1)
		{
			lightColor = PointLightColor
			(	input.positionW
			,	g_EyePosW
			,	g_PointDiffuseColor
			,	g_PointLightPosition
			,	g_PointIntensity
			,	1.0 / g_PointLightRange
			,	normal
			,	g_MaterialDiffuseColor
			,	g_MaterialSpecularPower
			,	g_MaterialSpecularIntensity
			);
		}

		if (g_UsingSpotLight == 1 && g_AmbientToggle == 1)
		{
			lightColor = SpotLightColor
			(	input.positionW
			,	g_EyePosW
			,	g_SpotDiffuseColor
			,	g_SpotLightPosition
			,	g_SpotIntensity
			,	1 / g_SpotLightRange
			,	float3(0, 0, 0)
			,	g_SpotCosOuterCone
			,	g_SpotInnerConeReciprocal
			,	g_SpotCosineAngle
			,	normal
			,	g_MaterialDiffuseColor
			,	g_MaterialSpecularPower
			,	g_MaterialSpecularIntensity
			);
		}

		if (g_UsingCapsuleLight == 1 && g_AmbientToggle == 1)
		{
			lightColor = CapsuleLightColor
			(	input.positionW
			,	g_EyePosW
			,	g_CapsuleLightPosition
			,	g_CapsuleLightRange
			,	1.0 / g_CapsuleLightRange
			,	g_CapsuleLightDirection
			,	g_CapsuleLightLength
			,	g_CapsuleDiffuseColor
			,	g_CapsuleDirectionLength
			,	g_CapsuleIntensity
			,	normal
			,	g_MaterialDiffuseColor
			,	g_MaterialSpecularPower
			,	g_MaterialSpecularIntensity
			);
		}
	}

	finalColor *= float4(lightColor, g_Alpha);

	//============================================ Alpha Map Blending (Alpha-to-Coverage)

	if (g_UsingAlphaMapTexture == 1)
	{
		float4 alphaMapColor = g_MaterialAlphaMapTexture.Sample(g_LinearSampler, input.uv);
		if (alphaMapColor.x == g_AlphaToCoverageValue.x && alphaMapColor.y == g_AlphaToCoverageValue.y && alphaMapColor.z == g_AlphaToCoverageValue.z)
		{
			clip(-1);
		}
	}

	//============================================ Clipping for water reflections and refractions

	clip(input.clip < 0.0 ? -1 : 1);

	// return the final color
	finalColor.a = 1.0f;

	return finalColor;
}

float4 TriMaterialLightTessellationWireframePS(PixelInput input) : SV_Target
{
	clip(input.clip < 0.0 ? -1 : 1);

	return float4(0.9f, 0.9f, 0.9f, 1);
}