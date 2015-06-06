//==============================================================================
// Material Tessellation Pixel Shader
//
//==============================================================================
// History
//
// -Created on 5/12/2015 by Dustin Watson
//==============================================================================
#include "../Environment/Lighting/Lighting.hlsl"

//
// Constants
//

cbuffer cbPerFrame : register(b0)
{
	float3	g_EyePosW;
	int		g_UsingDiffuseArrayTexture;
	float4 	g_ClipPlane;
	float4	g_MaterialDiffuseColor;
	float4	g_MaterialAmbientColor;
	float	g_MaterialSpecularPower;
	float	g_MaterialSpecularIntensity;
	int	   	g_UsingDiffuseTexture;
	int	   	g_UsingNormalMapTexture;
	int	   	g_UsingBlendMapTexture;
	int	   	g_UsingDetailMapTexture;
	int		g_UseAlphaMapTexture;
	int		g_EnableTransparency;
	int		g_EnableLighting;
	float	g_DetailBrightness;
	int	   	g_FlipTextureH;
	int	   	g_FlipTextureV;
	float3	g_AlphaToCoverageValue; // Value that clips pixel during alpha blending
	float	mspadding;
};

cbuffer cbMatrixBuffer : register(b1)
{
    matrix g_World;
    matrix g_View;
    matrix g_Proj;
};

//======================================================================================================

//
// Globals
//

Texture2DArray g_MaterialDiffuseArrayTexture 	: register(t0);
Texture2D g_MaterialDiffuseTexture 				: register(t1);
Texture2D g_MaterialNormalMapTexture 			: register(t2);
Texture2D g_MaterialBlendMapTexture 			: register(t3);
Texture2D g_MaterialDetailMapTexture 			: register(t4);
Texture2D g_MaterialAlphaMapTexture 			: register(t5);

SamplerState LinearSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

//======================================================================================================

//
// Pixel Shader
//

struct PixelInput
{
	float4 position		: SV_POSITION;
	float3 positionW	: POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD0;
	float  clip			: TEXCOORD1;
	float4 depth		: TEXCOORD2;
};

float4 MaterialTessellationShaderPS(PixelInput input) : SV_TARGET
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
	
	float4 finalTextureColor = float4(0, 0, 0, 0);
	float4 diffuseLayers0 = float4(0, 0, 0, 0);
	float4 diffuseLayers1 = float4(0, 0, 0, 0);
	float4 diffuseLayers2 = float4(0, 0, 0, 0);
	float4 diffuseLayers3 = float4(0, 0, 0, 0);
	
	//if (g_UsingDiffuseArrayTexture == 1)
	//{
		// Sample layers in texture array.
		//diffuseLayers0 = g_MaterialDiffuseArrayTexture.Sample( LinearSampler, float3(input.uv, 0.0f) );
		//diffuseLayers1 = g_MaterialDiffuseArrayTexture.Sample( LinearSampler, float3(input.uv, 1.0f) );
		//diffuseLayers2 = g_MaterialDiffuseArrayTexture.Sample( LinearSampler, float3(input.uv, 2.0f) );
		//diffuseLayers3 = g_MaterialDiffuseArrayTexture.Sample( LinearSampler, float3(input.uv, 3.0f) );
	//}
	//else
	{
		//if (g_UsingDiffuseTexture == 1)
		{
			finalTextureColor = g_MaterialDiffuseTexture.Sample( LinearSampler, input.uv );
		}
	}
    
	//============================================ Blend Mapping
	
	if (g_UsingBlendMapTexture == 1)
	{
		if (g_UsingDiffuseArrayTexture == 1)
		{
			// Sample the blend map.
			float4 t = g_MaterialBlendMapTexture.Sample( LinearSampler, input.uv );
			
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
		if (depth < 0.9f)
		{
			float4 normalMap = g_MaterialNormalMapTexture.Sample( LinearSampler, input.uv );
			normal = (normalMap.rgb * 2.0f) - 1.0f;
			normal = normalize(normal);
		}
	}
	
	//============================================ Shadow Mapping
	
	float shadowColor = 1.0f;
	
	//============================================ SSAO
	
	float ssaoColor = 1.0f;
	
	//============================================ Detail Mapping
	
	// Check if the depth value is close to the screen
	//if (depth < 0.9f)
	{
		float4 detailColor = g_MaterialDetailMapTexture.Sample( LinearSampler, input.uv );
		finalColor *= detailColor * g_DetailBrightness;
	}
	
	//============================================ Lighting
	
	if (g_EnableLighting == 1)
	{
		finalColor *= CalculateLightColor
		(	input.positionW
		,	g_EyePosW
		,	finalColor
		,	normal
		,	g_MaterialDiffuseColor
		,	g_MaterialAmbientColor
		,	g_MaterialSpecularPower
		,	g_MaterialSpecularIntensity
		,   shadowColor
		,   ssaoColor
		);
	}
	
	//============================================ Alpha Map Blending (Alpha-to-Coverage)
	
	if (g_UseAlphaMapTexture == 1)
	{
		float4 alphaMapColor = g_MaterialAlphaMapTexture.Sample( LinearSampler, input.uv );
		if (alphaMapColor.x == g_AlphaToCoverageValue.x && alphaMapColor.y == g_AlphaToCoverageValue.y && alphaMapColor.z == g_AlphaToCoverageValue.z)
			clip(-1);
	}
	
	//============================================ Clipping for water reflections and refractions
	
	clip( input.clip < 0.0 ? -1 : 1 );
	
	// return the final color
	return finalColor;
}

float4 MaterialTessellationShaderWireframePS(PixelInput input) : SV_Target
{
	clip( input.clip < 0.0 ? -1 : 1 );
    return float4(0.9f, 0.9f, 0.9f, 1);
}