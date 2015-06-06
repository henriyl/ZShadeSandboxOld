//==============================================================================
// Point Light Shader for Deferred Shading
//
//==============================================================================
// History
//
// -Created on 6/5/2015 by Dustin Watson
//==============================================================================

//
// Constants
//

cbuffer cbDomainBuffer : register(b0)
{
	matrix g_LightProjection;
};

cbuffer cbPixelBuffer : register(b1)
{
	float3 	g_PointLightPosition;
	float	g_PointLightRange;
	float3	g_PointLightColor;
	float	padding1;
	float3	g_PointLightIntensity;
	float	padding2;
	float3	g_EyePosition;
	float	padding3;
	float4	g_PerspectiveValues;
	matrix	g_ViewInv;
};

//======================================================================================================

//
// Samplers and textures
//

TextureCube g_Color0Texture	: register(t0);
Texture2D g_Color1Texture	: register(t1);
Texture2D g_NormalTexture	: register(t2);
Texture2D g_DepthTexture	: register(t3);

// Use a point sampler to sample out exact per-pixel data from the two render textures
SamplerState pointSampler	: register(s0);

//======================================================================================================

//
// Global functions
//

float3 CalcWorldPos(float2 clipSpacePos, float depth)
{
	float linearDepth = g_PerspectiveValues.z / (depth + g_PerspectiveValues.w);
	
	float4 position;
	
	position.xy = clipSpacePos.xy * g_PerspectiveValues.xy * linearDepth;
	position.z = linearDepth;
	position.w = 1.0;
	
	return mul(position, g_ViewInv).xyz;
}

//======================================================================================================

//
// Vertex Shader
//

// Call Draw with 2
// Primitive topology -> D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST
float4 PointLightDeferredVS() : SV_Position
{
	return float4(0, 0, 0, 1);
}

//======================================================================================================

//
// Hull Shader
//

struct HullConstOutput
{
	float edges[4]	: SV_TessFactor;
	float inside[2]	: SV_InsideTessFactor;
};

HullConstOutput PointLightDeferredConstHS()
{
	HullConstOutput output;
	
	float tessFactor = 18.0;
	
	output.edges[0] = output.edges[1] = output.edges[2] = output.edges[3] = tessFactor;
	
	output.inside[0] = output.inside[1] = tessFactor;
	
	return output;
}

static const float3 HemiDir[2] =
{
	float3( 1.0, 1.0,  1.0),
	float3(-1.0, 1.0, -1.0)
};

struct HullOutput
{
	float3 HemiDir : POSITION;
};

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_ccw")]
[outputcontrolpoints(4)]
[patchconstantfunc("PointLightDeferredConstHS")]
HullOutput PointLightDeferredHS(uint PatchID : SV_PrimitiveID)
{
	HullOutput output;
	
	output.HemiDir = HemiDir[PatchID];
	
	return output;
}

//======================================================================================================

//
// Domain Shader
//

struct DomainOutput
{
	float4 position 	: SV_Position;
	float2 clipSpacePos	: TEXCOORD0;
};

[domain("quad")]
DomainOutput PointLightDeferredDS(HullConstOutput input, float2 uv : SV_DomainLocation, const OutputPatch<HullOutput, 4> quad)
{
	DomainOutput output;
	
	// Transform the uv's to clip-space
	float2 clipSpace = uv.xy * 2.0 - 1.0;
	
	// Find the max distance from the center
	float2 clipSpaceAbs = abs(clipSpace.xy);
	float maxLen = max(clipSpaceAbs.x, clipSpaceAbs.y);
	
	// Generate final position in clip-space
	float3 normDir = normalize(float3(clipSpace.xy, maxLen - 1.0) * quad[0].HemiDir);
	
	float4 posLS = float4(normDir.xyz, 1.0);
	
	// Transform all the way to projected space and generate the uv's
	output.position = mul(posLS, g_LightProjection);
	
	// Store the clip-space position
	output.clipSpacePos = output.position.xy / output.position.w;
	
	return output;
}

//======================================================================================================

//
// Pixel Shader
//

float4 PointLightDeferredPS(DomainOutput input) : SV_Target
{
	//
	// Unpack the GBuffer
	//
	
	int3 location3 = int3(input.position.x, input.position.y, input.position.z);
	
	float posLX = g_DepthTexture.Load(location3).g;
	float posLY = g_DepthTexture.Load(location3).b;
	float posLZ = g_DepthTexture.Load(location3).a;
	float3 positionL = float3(posLX, posLY, posLZ);
	float4 cubeColors = g_Color0Texture.Sample(pointSampler, positionL);
	
	//float4 colors = g_Color1Texture.Sample(pointSampler, input.uv);
	float4 colors = g_Color1Texture.Load(location3);
	
	// Make the color fully opaque
	colors.a = 1.0;
	
	float4 normals = g_NormalTexture.Load(location3);
	float3 normal = normals.xyz;
	
	float specularPower = normals.w;
	float specularIntensity = g_Color1Texture.Load(location3).a;
	
	// Sample the depth
	float depth = g_DepthTexture.Load(location3).r;
	
	// Compute to screen-space position
	//float4 position;
	//position.x = input.uv.x * 2.0f - 1.0f;
	//position.y = -(input.uv.y * 2.0f - 1.0f);
	//position.z = depth;
	//position.w = 1.0;
	
	// Transform to world space
	//position = mul(position, g_InvViewProj);
	//position /= position.w;
	
	float3 position = CalcWorldPos(input.clipSpacePos, depth);
	
	//
	// Perform Blinn Phong Point Light
	//
	
	float3 toLight = g_PointLightPosition.xyz - position;
	float3 toEye = g_EyePosition.xyz - position;
	float distToLight = length(toLight);
	
	//
	// Phong Diffuse
	//
	
	toLight /= distToLight;
	float NDotL = saturate(dot(toLight, normal));
	float3 lightColor = g_PointLightColor * NDotL;
	
	//
	// Blinn specular
	//
	
	toEye = normalize(toEye);
	float3 halfWay = normalize(toEye + toLight);
	float NDotH = saturate(dot(halfWay, normal));
	lightColor += g_PointLightColor * pow(NDotH, specularPower) * specularIntensity;
	
	//
	// Attenuation
	//
	
	float distToLightNormal = 1.0 - saturate(distToLight * (1 / g_PointLightRange));
	float attenuation = distToLightNormal * distToLightNormal;
	lightColor *= colors.rgb * attenuation;
	
	return float4(lightColor, 1.0);
}

float4 PointLightDeferredWireframePS(DomainOutput input) : SV_Target
{
    return float4(0.9f, 0.9f, 0.9f, 1);
}