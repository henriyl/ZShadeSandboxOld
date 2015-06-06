//==============================================================================
// Directional Light Shader for Deferred Shading
//
//==============================================================================
// History
//
// -Created on 6/2/2015 by Dustin Watson
//==============================================================================
//#include "../Lighting.hlsl"

//
// Constants
//

cbuffer cbLightBuffer : register(b0)
{
	float4 g_DirectionLightColor;
	float3 g_LightDirection;
	float  padding1;
	float3 g_EyePosition;
	float  padding2;
	float3 g_AmbientDown;
	float  padding3;
	float3 g_AmbientUp;
	float  padding4;
};

//cbuffer cbGBufferUnpackConst : register(b1)
//{
//	// XY components should be filled with one over the projection matrix diagonal values.
//	// 		g_PerspectiveValues.x = 1 / Pm[0][0]
//	// Z component is the projection matrix Pm[3][2] value.
//	// W component is the negative projection matrix Pm[2][2] value.
//	float4 g_PerspectiveValues : packoffset(c0);
//	float4 g_x4ViewInv		   : packoffset(c1);
//};

cbuffer cbMatrixBuffer : register(b1)
{
	matrix		g_InvViewProj;
};

//======================================================================================================

//
// Samplers and textures
//

TextureCube g_Color0Texture	: register(t0);
Texture2D   g_Color1Texture	: register(t1);
Texture2D   g_NormalTexture	: register(t2);
Texture2D   g_DepthTexture	: register(t3);

// Use a point sampler to sample out exact per-pixel data from the two render textures
SamplerState pointSampler	: register(s0);
SamplerState linearSampler	: register(s1);

//======================================================================================================

//
// Global Functions
//

//float DepthToLinear(float depth)
//{
//	return g_PerspectiveValues.z / (depth + g_PerspectiveValues.w);
//}

//float3 CalculateWorldPosition(float2 csPos, float linearDepth)
//{
//	float4 position;
//	
//	position.xy = csPos.xy * g_PerspectiveValues.xy * linearDepth;
//	position.z = linearDepth;
//	position.w = 1.0;
//	
//	return mul(position, g_InvView).xyz;
//}

//======================================================================================================

//
// Structs
//

struct PixelInput
{
	float4 position	: SV_POSITION;
	float2 uv		: TEXCOORD0;
};

//======================================================================================================

//
// Vertex Shader
//

PixelInput DirectionalLightDeferredVS(uint vertexID : SV_VertexID)
{
	PixelInput output;
	
	output.uv = float2((vertexID << 1) & 2, vertexID & 2);
	output.position = float4(output.uv * float2(2, -2) + float2(-1, 1), 0, 1);
	
	return output;
}

//======================================================================================================

//
// Pixel Shader
//

float4 DirectionalLightDeferredPS(PixelInput input) : SV_Target
{
	float4 finalColor;
	
	//
	// Unpack the GBuffer
	//
	
	float4 depthTexture = g_DepthTexture.Sample(pointSampler, input.uv);
	
	float3 positionL = float3(depthTexture.y, depthTexture.z, depthTexture.w);
	float4 cubeColors = g_Color0Texture.Sample(linearSampler, positionL);
	
	float4 colors = g_Color1Texture.Sample(pointSampler, input.uv);
	
	//if (cubeColors.x == 0 && cubeColors.y == 0 && cubeColors.z == 0)
	//	colors.rgb *= cubeColors.rgb;
	
	// Make the color fully opaque
	colors.a = 1.0;
	
	float4 normals = g_NormalTexture.Sample(pointSampler, input.uv);
	float3 normal = normals.xyz;
	
	//float specularPowerNormal = normals.w;
	//float specularPowerU = specularPowerNormal + specularPowerNormal * g_SpecularPowerRange.y;
	//float specularPower = g_SpecularPowerRange.x + g_SpecularPowerRange.y * specularPowerU;
	
	float specularPower = normals.w;
	float specularIntensity = g_Color1Texture.Sample(pointSampler, input.uv).a;
	
	// Sample the depth
	float depth = g_DepthTexture.Sample(pointSampler, input.uv).r;
	
	// Compute to screen-space position
	float4 position;
	position.x = input.uv.x * 2.0f - 1.0f;
	position.y = -(input.uv.y * 2.0f - 1.0f);
	position.z = depth;
	position.w = 1.0;
	
	// Transform to world space
	position = mul(position, g_InvViewProj);
	position /= position.w;
	
	//float3 lightVector = -normalize(g_LightDirection);
	//float NdotL = max(0, dot(normal, lightVector));
	//float3 diffuseLight = NdotL * colors.rgb;
	//float3 reflectionVector = normalize(reflect(lightVector, normal));
	//float3 directionToCamera = normalize(g_EyePosition.xyz - position);
	//float specularLight = specularIntensity * pow(saturate(dot(reflectionVector, directionToCamera)), specularPower);
	//finalColor = float4(diffuseLight.rgb, specularLight);
	
	//
	// Perform Blinn Phong Directional Light + Ambient Lighting
	//
	
	float4 linearDiffuseColor = float4(colors.rgb * colors.rgb, colors.a);
	// Convert from [-1, 1] to [0, 1]
	float up = normal.y * 0.5 + 0.5;
	// Calculate the ambient value
	float3 ambient = g_AmbientDown + up * g_AmbientUp;
	// Apply the ambient color to the diffuse color
	finalColor.rgb = ambient * linearDiffuseColor.rgb;
	finalColor.a = 1.0f;
	
	//
	// Phong diffuse
	//
	
	float3 lightDir = g_LightDirection;
	
	float NDotL = dot(normal, lightDir);
	float3 lightColor = g_DirectionLightColor.rgb * saturate(NDotL);
	
	//
	// Blinn specular
	//
	
	float3 toEye = g_EyePosition.xyz - position;
	toEye = normalize(toEye);
	float3 halfWay = normalize(toEye + lightDir);
	float NDotH = saturate(dot(halfWay, normal));
	lightColor += g_DirectionLightColor.rgb * pow(NDotH, specularPower) * specularIntensity;
	
	finalColor.rgb = lightColor * colors.rgb;
	//finalColor.rgb = colors.rgb;
	finalColor.a = 1.0f;
	
	return finalColor;
}

float4 DirectionalLightDeferredWireframePS(PixelInput input) : SV_Target
{
    return float4(0.9f, 0.9f, 0.9f, 1);
}