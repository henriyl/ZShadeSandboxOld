//==============================================================================
// Terrain Tessellation Pixel Shader
//
// http://stackoverflow.com/questions/17035891/how-do-constant-shaders-need-to-be-padded-in-order-to-avoid-a-e-invalidarg
// http://stackoverflow.com/questions/24276402/unexpected-sizes-of-arrays-in-a-hlsl-constant-buffer
//==============================================================================
// History
//
// -Created on 4/8/2015 by Dustin Watson
//==============================================================================

#include "..\Environment\Lighting\Lighting.hlsl"
//#include "..\Environment\Lighting\ShadowMap.hlsl"

// For Shadow Map:
// Need to get the shadow transformation matrix from the first directional light
// Build the texture resource from a render target by rendering the terrain shadow map to it
// Pass this texture resource into the terrain

//
// Constants
//

cbuffer cShadingConst : register(b0)
{
	// Position of the camera
	float3		g_EyePosW;
	float		g_DetailBrightness;
	
	// The parameters for fog
	float		g_fogThinning;
	float		g_fogAltitude;
	float		g_fogNear;
	float		g_fogFar;
	float4		g_fogColor;
	
	// The Terrain texture parameters
	float		g_TextureAmount;
	float		g_TextureWidth;
	float		g_TextureHeight;
	
	// The parameters for terrain/water updating
	float		g_seaLevel;
	float4		g_waterBodyColor;
	
	// Terrain Material
	float4		g_MaterialDiffuseColor;
	float4		g_MaterialAmbientColor;
	float 		g_TexelCellSpaceU;
	float 		g_TexelCellSpaceV;
	float		g_MaterialSpecularPower;
	float		g_MaterialSpecularIntensity;
	
	// Control variables
	int			g_useClippingPlane;
	int			g_useNormalMap;
	int			g_UseSobelFilter;
	int			g_useShadowMap;
	float3		tpadding;
	int			g_useSSAO;
	
	matrix g_ViewMatrix;
};

SamplerState ShadowMapSampler
{
	Filter = COMPARISON_MIN_MAG_MIP_POINT;
	AddressU = CLAMP;
	AddressV = CLAMP;
	AddressW  = CLAMP;
};

SamplerComparisonState ShadowMapSamplerComparison
{
	Filter = COMPARISON_MIN_MAG_MIP_POINT;
	AddressU = CLAMP;
	AddressV = CLAMP;
	AddressW  = CLAMP;
};

static const float SMAP_SIZE = 256.0f;
static const float SMAP_DX = 1.0f / SMAP_SIZE;

//======================================================================================================

//
// Globals
//

Texture2DArray g_LayerMapArray  : register(t0);
Texture2D      g_BlendMap 		: register(t1);
Texture2D      g_NormalMap 		: register(t2);
Texture2D      g_HeightMap 		: register(t3);
Texture2D	   g_DetailMap		: register(t4);
Texture2D	   g_ShadowMap		: register(t5);
Texture2D	   g_SSAOMap		: register(t6);

SamplerState SampleTypeLinear	: register(s0);

//======================================================================================================

//
// Pixel Shader
//

struct PixelInput
{
	float4 position		: SV_POSITION;
	float3 positionW	: POSITION;
	//float3 normal		: NORMAL;
	float pixelDepth	: DEPTH;
	float2 uv			: TEXCOORD0;
	float2 TiledTex 	: TEXCOORD1;
	float4 worldPos 	: TEXCOORD2;
	float clip      	: TEXCOORD3;
	float4 depth		: TEXCOORD4;
	float4 shadowPos	: TEXCOORD5;
	float4 ssaoPos		: TEXCOORD6;
};

float4 TerrainTessellationQuadPS(PixelInput input) : SV_Target
{
	float4 finalColor;
	
	//=======================================================
	// Estimate normal and tangent using central differences.
	float2 leftTex   = input.uv + float2(-g_TexelCellSpaceU, 0.0f);
	float2 rightTex  = input.uv + float2(g_TexelCellSpaceU, 0.0f);
	float2 bottomTex = input.uv + float2(0.0f, g_TexelCellSpaceV);
	float2 topTex    = input.uv + float2(0.0f, -g_TexelCellSpaceV);
	
	float leftY   = g_HeightMap.SampleLevel( SampleTypeLinear, leftTex, 0 ).r;
	float rightY  = g_HeightMap.SampleLevel( SampleTypeLinear, rightTex, 0 ).r;
	float bottomY = g_HeightMap.SampleLevel( SampleTypeLinear, bottomTex, 0 ).r;
	float topY    = g_HeightMap.SampleLevel( SampleTypeLinear, topTex, 0 ).r;
	
	float worldCellSpace 	= 1.0f;
	float3 tangent 		= normalize(float3(2.0f * worldCellSpace, rightY - leftY, 0.0f));
	float3 bitangent   	= normalize(float3(0.0f, bottomY - topY, -2.0f * worldCellSpace));
	float3 normalW 		= cross(tangent, bitangent);
	
	// Get the depth value of the pixel by dividing the Z pixel by the homogeneous W coordinate
	float depth = input.depth.z / input.depth.w;
	
	//============================================ Calculate position of camera eye for lighting
	
	// The toEye vector is used in lighting.
	float3 toEye = g_EyePosW - input.positionW;
	
	// Cache the distance to the eye from this surface point.
	float distToEye = length(toEye);
	
	// Normalize.
	toEye /= distToEye;
	
	float4 camWorld = float4(input.worldPos - float4(g_EyePosW, 1));
	float d = length(camWorld);
	
	//=======================================================
	// Create terrain normals
	
	//where pixelSize.x will be the width and pixelSize.y will be the height of pixels
	float2 pixelSize = float2(1 / g_TextureWidth, 1 / g_TextureHeight);
	
	//============================================ Normal Mapping
	
	float3 normal = float4(0, 1, 0, 1);
	
	if (g_useNormalMap == 1)
	{
		//if (depth < 500.0f)
		{
			float4 normalMap = g_NormalMap.Sample( SampleTypeLinear, input.uv );
			normal = (normalMap.rgb * 2.0f) - 1.0f;
			normal = normalW + normal.x * tangent + normal.y * bitangent;
			normal = normalize(normal);
		}
	}
	else
	{
		if (g_UseSobelFilter == 1)
		{
			/*
				Coordinates are laid out as follows:
				
					0,0 | 1,0 | 2,0
					----+-----+----
					0,1 | 1,1 | 2,1
					----+-----+----
					0,2 | 1,2 | 2,2
			*/
			
			// Compute the necessary offsets:
			float2 o00 = input.uv + float2( -pixelSize.x, -pixelSize.y );
			float2 o10 = input.uv + float2(         0.0f, -pixelSize.y );
			float2 o20 = input.uv + float2(  pixelSize.x, -pixelSize.y );

			float2 o01 = input.uv + float2( -pixelSize.x, 0.0f         );
			float2 o21 = input.uv + float2(  pixelSize.x, 0.0f         );

			float2 o02 = input.uv + float2( -pixelSize.x,  pixelSize.y );
			float2 o12 = input.uv + float2(         0.0f,  pixelSize.y );
			float2 o22 = input.uv + float2(  pixelSize.x,  pixelSize.y );

			// Use of the sobel filter requires the eight samples
			// surrounding the current pixel:
			float h00 = g_HeightMap.Sample( SampleTypeLinear, o00 ).r;
			float h10 = g_HeightMap.Sample( SampleTypeLinear, o10 ).r;
			float h20 = g_HeightMap.Sample( SampleTypeLinear, o20 ).r;

			float h01 = g_HeightMap.Sample( SampleTypeLinear, o01 ).r;
			float h21 = g_HeightMap.Sample( SampleTypeLinear, o21 ).r;

			float h02 = g_HeightMap.Sample( SampleTypeLinear, o02 ).r;
			float h12 = g_HeightMap.Sample( SampleTypeLinear, o12 ).r;
			float h22 = g_HeightMap.Sample( SampleTypeLinear, o22 ).r;
			
			// The Sobel X kernel is:
			//
			// [ 1.0  0.0  -1.0 ]
			// [ 2.0  0.0  -2.0 ]
			// [ 1.0  0.0  -1.0 ]
			
			float Gx = h00 - h20 + 2.0f * h01 - 2.0f * h21 + h02 - h22;
						
			// The Sobel Y kernel is:
			//
			// [  1.0    2.0    1.0 ]
			// [  0.0    0.0    0.0 ]
			// [ -1.0   -2.0   -1.0 ]
			
			float Gy = h00 + 2.0f * h10 + h20 - h02 - 2.0f * h12 - h22;
			
			// Generate the missing Z component - tangent
			// space normals are +Z which makes things easier
			// The 0.5f leading coefficient can be used to control
			// how pronounced the bumps are - less than 1.0 enhances
			// and greater than 1.0 smoothes.
			float Gz = 0.5f * sqrt( 1.0f - Gx * Gx - Gy * Gy );

			// Make sure the returned normal is of unit length
			normal = normalize( float3( 2.0f * Gx, 2.0f * Gy, Gz ) );
		}
		else
		{
			// Determine the offsets
			float2 o1 = float2( pixelSize.x, 0.0f        );
			float2 o2 = float2( 0.0f,        pixelSize.y );

			// Take three samples to determine two vectors that can be
			// use to generate the normal at this pixel
			float h0 = g_HeightMap.Sample( SampleTypeLinear, input.uv ).r;
			float h1 = g_HeightMap.Sample( SampleTypeLinear, input.uv + o1 ).r;
			float h2 = g_HeightMap.Sample( SampleTypeLinear, input.uv + o2 ).r;
						
			float3 v01 = float3( o1, h1 - h0 );
			float3 v02 = float3( o2, h2 - h0 );

			float3 n = cross( v01, v02 );

			// Can be useful to scale the Z component to tweak the
			// amount bumps show up, less than 1.0 will make them
			// more apparent, greater than 1.0 will smooth them out
			n.z *= 0.5f;

			normal =  normalize( n );
		}
	}
	
	//============================================ Texturing
	
	/*
		Texture2D g_TextureMask;
		Texture2D g_PaintingTexture;
		
		float4 paintingTexColor = float4(1, 1, 1, 1);
		
		if (g_Painting == 1)
		{
			float maskColor = g_TextureMask.Sample(SampleTypeLinear, input.uv).r;
			if (maskColor == 1.0f)
			{
				paintingTexColor = g_PaintingTexture.Sample(SampleTypeLinear, input.uv);
			}
		}
		
		// Regular layer map texturing
		...
		
		finalColor = paintingTexColor * texColor;
	*/
	
	// Sample layers in texture array.
	float4 c0 = g_LayerMapArray.Sample( SampleTypeLinear, float3(input.TiledTex, 0.0f) );
	float4 c1 = g_LayerMapArray.Sample( SampleTypeLinear, float3(input.TiledTex, 1.0f) );
	float4 c2 = g_LayerMapArray.Sample( SampleTypeLinear, float3(input.TiledTex, 2.0f) );
	float4 c3 = g_LayerMapArray.Sample( SampleTypeLinear, float3(input.TiledTex, 3.0f) );
	
	// Sample the blend map.
	float4 t = g_BlendMap.Sample( SampleTypeLinear, input.uv );
    
    // Blend the layers on top of each other.
    float4 texColor = c0;
    texColor = lerp(texColor, c1, t.r);
    texColor = lerp(texColor, c2, t.g);
    texColor = lerp(texColor, c3, t.b);
	
	finalColor = texColor;
	
	//============================================ Detail Mapping
	
	// Check if the depth value is close to the screen
	//if (depth < 500.0f)
	{
		float4 detailColor = g_DetailMap.Sample(SampleTypeLinear, input.uv);
		
		finalColor *= detailColor * g_DetailBrightness;
	}
	
	//============================================ Shadow Mapping
	
	//float3 shadowColor = float3(1.0f, 1.0f, 1.0f);
	float shadowColor = 1.0f;// = ShadowMapPCH(g_DirectionalLight[0].g_LightDirection, g_ShadowMatrix, g_ShadowMap);
	
	if (g_useShadowMap == 1)
	{
		//shadowColor = ComputeShadowColor(input.shadowPos, g_DirectionalLight[0].g_LightDirection, normal, g_ShadowMap);
		
		//float4 lightingPosition = input.shadowPos;
		//float3 lightPos = g_DirectionalLight[0].g_LightDirection;
		
		//float3 shadowColor = float3(1.0f, 1.0f, 1.0f);
		
		// Find the position in the shadow map for this pixel
		//float2 ShadowTexCoord = 0.5 * lightingPosition.xy / lightingPosition.w + float2( 0.5, 0.5 );
		
		//ShadowTexCoord.y = 1.0f - ShadowTexCoord.y;
		
		// Get the current depth stored in the shadow map
		//float4 shadowInfo = g_ShadowMap.Sample(ShadowMapSampler, ShadowTexCoord);
		//float shadowdepth = shadowInfo.r;
		//float shadowOpacity = 0.5f + 0.5f * (1 - shadowInfo.g);
		
		// Calculate the current pixel depth
		// The bias is used to prevent floating point errors that occur when
		// the pixel of the occluder is being drawn
		//float ourdepth = (lightingPosition.z / lightingPosition.w) - 0.0001f;
		
		// Check to see if this pixel is in front or behind the value in the shadow map
		//if (shadowdepth < ourdepth)
		//{
			// Calculate the amount of light on this pixel
		//	float intensity = saturate(dot(normal, lightPos));
			
		//	float3 lightDirection = -lightPos;
			
		//	if (intensity > 0)
		//	{
		//		float3 lightColor = finalColor.rgb * saturate(intensity);
		//		float3 toEye = g_EyePosW.xyz - input.positionW;
		//		toEye = normalize(toEye);
		//		float3 halfWay = normalize(toEye + lightDirection);
		//		float NDotH = saturate(dot(halfWay, normal));
		//		lightColor += finalColor.rgb * pow(NDotH, g_MaterialSpecularPower) * g_MaterialSpecularIntensity * shadowOpacity;
		//		finalColor.rgb *= lightColor * g_MaterialDiffuseColor.rgb;
				
				//finalColor += (g_MaterialDiffuseColor * intensity);
		//		finalColor = saturate(finalColor);
		//	}
		//}
		
		float4 shadowPos = input.shadowPos;
		
		shadowPos.xyz /= shadowPos.w;
		
		float depth = shadowPos.z;
		
		const float dx = SMAP_DX;
		
		float percentLit = 0.0f;
		const float2 offsets[9] =
		{
			float2(-dx,  -dx), float2(0.0f,  -dx), float2(dx,  -dx),
			float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
			float2(-dx,  +dx), float2(0.0f,  +dx), float2(dx,  +dx)
		};
		
		[unroll]
		for (int i = 0; i < 9; i++)
		{
			percentLit += g_ShadowMap.SampleCmpLevelZero(ShadowMapSamplerComparison, shadowPos.xy + offsets[i], depth).r;
		}
		
		shadowColor = percentLit / 9.0f;
	}
	
	//============================================ SSAO
	
	float4 ssaoPos = input.ssaoPos;
	ssaoPos /= ssaoPos.w;
	float ssaoColor = 1.0f;
	if (g_useSSAO == 1)
	{
		ssaoColor = g_SSAOMap.SampleLevel(SampleTypeLinear, ssaoPos.xy, 0.0f).r;
	}
	
	//============================================ Simple Fog Above Water

	float waterDiff 	= g_seaLevel - input.worldPos.y;
	float fogValue    	= g_fogThinning;
	float fogAlt      	= g_fogAltitude;
	float fogNearDist 	= g_fogNear;
	float fogFarDist  	= g_fogFar;
	float4 fogColor   	= g_fogColor;
	
	if (waterDiff > 0)
	{
		//Elevation based fog becomes distance-based
		fogValue = 1;
		fogAlt = g_seaLevel;
		fogNearDist = 0;
		fogFarDist = 30;
		d = waterDiff;
		
		waterDiff = min(waterDiff / 100.0f, 1.0f);
		
		fogColor = lerp(g_waterBodyColor, g_waterBodyColor, waterDiff);
	}
	
	float l = saturate((0 - fogNearDist) / (fogFarDist - fogNearDist) / clamp(input.worldPos.y / fogAlt + 1, 1, fogValue));
	
	//============================================ Lighting
	
	//finalColor *= CalculateLightColor
	//(	input.positionW
	//,	g_EyePosW
	//,	finalColor
	//,	normal
	//,	g_MaterialDiffuseColor
	//,	g_MaterialAmbientColor
	//,	g_MaterialSpecularPower
	//,	g_MaterialSpecularIntensity
	//,   shadowColor
	//,	ssaoColor
	//);
	
	//float3 lightPos = g_DirectionalLight[0].g_LightDirection;
	//float3 lightDirection = -lightPos;
	//float NDotL = dot(normal, lightDirection);
	//float3 lightColor = finalColor.rgb * saturate(NDotL);
	//float3 ltoEye = g_EyePosW.xyz - input.positionW;
	//ltoEye = normalize(ltoEye);
	//float3 halfWay = normalize(ltoEye + lightDirection);
	//float NDotH = saturate(dot(halfWay, normal));
	//lightColor += finalColor.rgb * pow(NDotH, g_MaterialSpecularPower) * g_MaterialSpecularIntensity * shadowColor;
	//finalColor.rgb *= lightColor * g_MaterialDiffuseColor.rgb;
	
	//============================================ Clip the terrain with the water/ocean
	
	if (g_useClippingPlane == 1)
	{
		clip((input.clip < 0.0) ? -1 : 1);
	}
	
	//============================================ Combine all Colors
	
	return lerp(finalColor, fogColor, l);
}

float4 TerrainTessellationWireframePS(PixelInput input) : SV_Target
{
	if (g_useClippingPlane == 1)
	{
		clip((input.clip < 0.0) ? -1 : 1);
	}
	
	return float4(0.9f, 0.9f, 0.9f, 1);
}

float4 TerrainTessellationShadowPS(PixelInput input) : SV_Target
{
	float depthValue = input.shadowPos.z / input.shadowPos.w;
	return float4(depthValue, depthValue, depthValue, 1.0f);
}

float4 TerrainTessellationNormalDepthPS(PixelInput input) : SV_Target
{
	//=======================================================
	// Estimate normal and tangent using central differences.
	float2 leftTex   = input.uv + float2(-g_TexelCellSpaceU, 0.0f);
	float2 rightTex  = input.uv + float2(g_TexelCellSpaceU, 0.0f);
	float2 bottomTex = input.uv + float2(0.0f, g_TexelCellSpaceV);
	float2 topTex    = input.uv + float2(0.0f, -g_TexelCellSpaceV);
	
	float leftY   = g_HeightMap.SampleLevel( SampleTypeLinear, leftTex, 0 ).r;
	float rightY  = g_HeightMap.SampleLevel( SampleTypeLinear, rightTex, 0 ).r;
	float bottomY = g_HeightMap.SampleLevel( SampleTypeLinear, bottomTex, 0 ).r;
	float topY    = g_HeightMap.SampleLevel( SampleTypeLinear, topTex, 0 ).r;
	
	float worldCellSpace 	= 0.5f;
	float3 tangent 		= normalize(float3(2.0f * worldCellSpace, rightY - leftY, 0.0f));
	float3 bitangent   	= normalize(float3(0.0f, bottomY - topY, -2.0f * worldCellSpace));
	float3 normalW 		= cross(tangent, bitangent);
	
	float3 posV = mul(float4(input.positionW, 1.0f), g_ViewMatrix).xyz;
	float3 normalV = mul(normalW, (float3x3)g_ViewMatrix);
	
	return float4(normalV, posV.z);
}