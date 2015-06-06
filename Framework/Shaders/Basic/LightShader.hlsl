//==============================================================================
// Light Shader
//
//==============================================================================
// History
//
// -Created on 5/17/2015 by Dustin Watson
//==============================================================================

//
// Constants
//

//======================================================================================================

cbuffer cbLightBuffer : register(b0)
{
	float4 g_LightAmbient;
	float4 g_LightDiffuse;
	float3 g_LightDirection;
	float  g_ToggleShadowMap;
};

cbuffer cbMatrixBuffer : register(b1)
{
    matrix g_World;
    matrix g_View;
    matrix g_Proj;
	matrix g_ShadowMatrix;
	matrix g_LightViewMatrix;
    matrix g_LightProjectionMatrix;
};

//
// Textures and samplers
//

//======================================================================================================

Texture2D g_DiffuseTexture	: register(t0);
Texture2D g_ShadowMap		: register(t1);
//Texture2D g_SSAOMap			: register(t2);

SamplerState LinearSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
	AddressW = WRAP;
	MaxLOD = D3D11_FLOAT32_MAX;
};

SamplerState ShadowMapSampler
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = CLAMP;
	AddressV = CLAMP;
	AddressW  = CLAMP;
	MaxLOD = D3D11_FLOAT32_MAX;
};

//
// Structs
//

//======================================================================================================

struct VertexInput
{
	float3 position		: POSITION;
	float2 uv			: TEXCOORD0;
	float3 normal		: NORMAL;
};

struct PixelInput
{
	float4 position		: SV_POSITION;
	float3 positionW	: POSITION;
	float2 uv			: TEXCOORD0;
	float3 normal		: NORMAL;
	float4 depth		: TEXCOORD1;
	float4 shadowPos	: TEXCOORD2;
	float4 lightViewPosition : TEXCOORD3;
	float4 worldPos : TEXCOORD4;
};

//
// Vertex Shader
//

//======================================================================================================

PixelInput LightShaderVS(VertexInput input)
{
	PixelInput output;
    
    output.position = mul(float4(input.position, 1.0), g_World);
    output.position = mul(output.position, g_View);
    output.position = mul(output.position, g_Proj);
	
	output.positionW = input.position;
    
	output.uv = input.uv;
    
	output.normal = mul(input.normal, (float3x3)g_World);
	output.normal = normalize(output.normal);
	
	output.depth = output.position;
	
	// Calculate the position of the vertice from the light source
	output.lightViewPosition = mul(float4(input.position, 1.0), g_World);
	output.lightViewPosition = mul(output.lightViewPosition, g_LightViewMatrix);
	output.lightViewPosition = mul(output.lightViewPosition, g_LightProjectionMatrix);
	
	output.worldPos = mul(float4(input.position, 1.0), g_World);
	
	output.shadowPos = mul(float4(input.position, 1.0), g_ShadowMatrix);
	
    return output;
}

//
// Pixel Shader
//

//======================================================================================================

float4 LightShaderPS(PixelInput input) : SV_Target
{
	float3 lightDir = g_LightDirection;
	lightDir = -lightDir;
	float bias = 0.001f;
	float4 finalColor = g_LightAmbient;
	float lightIntensity = 1.0f;
	
	// Calculate the projected texture coordinates
	float2 projectTexCoord;
	projectTexCoord.x = input.lightViewPosition.x / input.lightViewPosition.w / 2.0 + 0.5f;
	projectTexCoord.y = -input.lightViewPosition.y / input.lightViewPosition.w / 2.0 + 0.5f;
	
	// Determine if the projected coordinates are in the 0 to 1 range. If so then this pixel is in the view of the light.
	if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
	{
		// Sample the shadow map depth value from the shadow map using the sampler at the projected texture coordinate location
		float depthValue = g_ShadowMap.Sample(ShadowMapSampler, projectTexCoord).r;
		
		// Calculate the depth of the light
		float lightDepthValue = input.lightViewPosition.z / input.lightViewPosition.w;
		
		// Subtract the bias from the lightDepthValue
		lightDepthValue = lightDepthValue - bias;
		
		// Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
        // If the light is in front of the object then light the pixel, if not then shadow this pixel since an object (occluder) is casting a shadow on it.
		if (lightDepthValue < depthValue)
		{
			// Calculate the amount of light on this pixel
			lightIntensity = saturate(dot(input.normal, lightDir));
			
			if (lightIntensity > 0.0f)
			{
				// Determine the final diffuse color based on the diffuse color and the light intensity
				finalColor += (g_LightDiffuse * lightIntensity);
				finalColor = saturate(finalColor);
			}
		}
	}
	else
	{
		// If this is outside the area of shadow map range then draw things normally with regular lighting.
		lightIntensity = saturate(dot(input.normal, lightDir));
		
		if (lightIntensity > 0.0f)
		{
			// Determine the final diffuse color based on the diffuse color and the light intensity
			finalColor += (g_LightDiffuse * lightIntensity);
			finalColor = saturate(finalColor);
		}
	}
	
	// Sample the pixel color from the texture
	float4 textureColor = g_DiffuseTexture.Sample(LinearSampler, input.uv);
	textureColor.a = 1.0f;
	
	if (g_ToggleShadowMap == 1)
		finalColor = finalColor * textureColor;
	else if (g_ToggleShadowMap == 0)
		finalColor = textureColor;
	
	return finalColor;
}

float4 LightShaderWireframePS(PixelInput input) : SV_Target
{
	return float4(0.9f, 0.9f, 0.9f, 1);
}