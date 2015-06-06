//==============================================================================
// Ocean Surface Pixel Shader
//
// Add Perlin Noise and Tessendorf's FFT later
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

//======================================================================================================

//
// Globals
//

Texture2D g_texDisplacement	: register(t0); // FFT wave displacement map in VS (wave displacement from ocean heightfield)
Texture2D g_texPerlin		: register(t1); // FFT wave gradient map in PS
Texture2D g_texGradient		: register(t2); // Perlin wave displacement & gradient map in both VS & PS
Texture2D g_WaveMap0 	  	: register(t3); // First Normal map
Texture2D g_WaveMap1 	  	: register(t4); // Second Normal map
Texture2D g_ReflectionMap 	: register(t5);
Texture2D g_RefractionMap 	: register(t6);

SamplerState mirrorSampler	: register(s0);
SamplerState linearSampler	: register(s1);
SamplerState pointSampler	: register(s2);

//======================================================================================================

//
// Pixel Shader
//

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

float4 OceanSurfacePS(PixelInput input) : SV_TARGET
{
	float4 color = float4(1, 1, 1, 1);
	
	float2 windForce = float2(1, 1);
	float2 moveVector = float2(0, 1);
	input.bumpMapSamplingPos1 += g_Time * windForce * moveVector;
	input.bumpMapSamplingPos2 += g_Time * windForce * moveVector;
	
	//
	//Sample the water normal map two times using the two different texture sampling coordinates.
	//

    // Sample the normal from the normal map texture using the two different tiled and translated coordinates.
    float4 bumpColor1 = g_WaveMap0.Sample( mirrorSampler, input.bumpMapSamplingPos1 );
	float4 bumpColor2 = g_WaveMap1.Sample( mirrorSampler, input.bumpMapSamplingPos2 );
	
    // Expand the range of the normal from (0,1) to (-1,+1).
    float3 normal1 = (bumpColor1.rgb * 2.0f) - 1.0f;
    float3 normal2 = (bumpColor2.rgb * 2.0f) - 1.0f;
	
	//
	//Now combine the two normal map results to get an animated water ripple effect instead of just a single rotated normal map ripple.
	//

    // Combine the normals to add the normal maps together.
    float3 normal = normalize(normal1 + normal2);

	float2 perturbation = g_WaveHeight * (bumpColor1.rg - 0.5f) * 2.0f;

	//
	// Set the reflection color
	//

	float2 projectedCoords;
	projectedCoords.x = input.reflectionMapSamplingPos.x / input.reflectionMapSamplingPos.w / 2.0f + 0.5f;
	projectedCoords.y = -input.reflectionMapSamplingPos.y / input.reflectionMapSamplingPos.w / 2.0f + 0.5f;

	// Re-position the texture coordinate sampling position by the scaled normal map value to simulate the rippling wave effect.
	float2 reflectTexCoord = projectedCoords + (normal.xy * 0.03f);
	float2 perturbatedTexCoords = projectedCoords + perturbation;

	float4 reflectiveColor = g_ReflectionMap.Sample( mirrorSampler, reflectTexCoord );
	
	//
	// Set the refraction color
	//

	float2 projectedRefrCoords;
	projectedRefrCoords.x = input.refractionMapSamplingPos.x / input.refractionMapSamplingPos.w / 2.0f + 0.5f;
	projectedRefrCoords.y = -input.refractionMapSamplingPos.y / input.refractionMapSamplingPos.w / 2.0f + 0.5f;

	// Re-position the texture coordinate sampling position by the scaled normal map value to simulate the rippling wave effect.
	float2 refractTexCoord = projectedRefrCoords + (normal.xy * 0.03f);
	float2 perturbatedRefrTexCoords = projectedRefrCoords + perturbation;
	
	float4 refractiveColor = g_RefractionMap.Sample( mirrorSampler, refractTexCoord );
	
	//
	//Add a water color tint to the refraction.
	//

    // Combine the tint with the refraction color.
	//float4(0.0f, 0.8f, 1.0f, 1.0f) is refraction tint
    refractiveColor = saturate(refractiveColor * g_RefractionTint);

	//
	// Calculate the fresnel term
	//
	float3 eyeVector = normalize( g_CamPos - input.position3D );
	float3 normalVectorUp = float3(0, 1, 0);
	float fangle = 1 + dot(eyeVector, normalVectorUp);
	fangle = pow(fangle, 5);
	float fresnelTerm = 1 / fangle;
	fresnelTerm = fresnelTerm * 3;
	fresnelTerm = fresnelTerm < 0 ? 0 : fresnelTerm;
	fresnelTerm = fresnelTerm > 1 ? 1 : fresnelTerm;
	float4 combinedColor = refractiveColor * (1 - fresnelTerm) + reflectiveColor * fresnelTerm;

	//
	// Add some dull color
	//

	//float4 dullColor = float4(0.1f, 0.1f, 0.2f, 1.0f);
	//float4 dullColor = float4(0.3f, 0.3f, 0.5f, 1.0f);
	color = lerp( combinedColor, g_DullColor, 0.2f );
	
	//
	//Finally do a specular light calculation using the water normals and add it to the final color result to get the specular effect on just the water ripples.
	//

    // Calculate the reflection vector using the normal and the direction of the light.
    float3 reflection = -reflect(normalize(g_LightDirection), normal);
	
    // Calculate the specular light based on the reflection and the camera position.
    float specular = dot(normalize(reflection), normalize(input.viewDirection));
	
    // Check to make sure the specular was positive so we aren't adding black spots to the water.
    if(specular > 0.0f)
    {
        // Increase the specular light by the shininess value.
        specular = pow(specular, g_SpecularShininess);

        // Add the specular to the final color.
        color = saturate(color + specular);
    }
	
	// Displacement map
	//float4 displacement = g_texDisplacement.Sample(linearSampler, input.uv);
	//color *= displacement;
	
	return color;
}

float4 OceanSurfaceWireframePS(PixelInput input) : SV_Target
{
    return float4(0.9f, 0.9f, 0.9f, 1);
}