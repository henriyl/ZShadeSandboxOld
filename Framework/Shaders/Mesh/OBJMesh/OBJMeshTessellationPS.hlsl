//==============================================================================
// OBJMesh Tessellation Pixel Shader
//
//==============================================================================
// History
//
// -Created on 4/15/2015 by Dustin Watson
//==============================================================================

//
// Constants
//

cbuffer cbOBJShadingConst : register(b0)
{
	float4	g_DifColor;
	float3	padding;
	int		g_hasTexture;
};

//======================================================================================================

//
// Globals
//

Texture2D shaderTexture : register(t0);

//------------------------------------------------------------------
//Sample States
//------------------------------------------------------------------
SamplerState SampleType : register(s0);
//{
//    Filter = MIN_MAG_MIP_LINEAR;
//    AddressU = WRAP;
//    AddressV = WRAP;
//};

//======================================================================================================

//
// Pixel Shader
//

struct PixelInput
{
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float pixelDepth	: DEPTH;
	float2 uv			: TEXCOORD0;
	float clip			: TEXCOORD1;
};

float4 OBJMeshTessellationPS(PixelInput input) : SV_Target
{
	float4 finalColor;
	
	//Set diffuse color of material
	float4 diffuse = g_DifColor;
	
	//If material has a diffuse texture map, set it now
	if(g_hasTexture == 1)
	{
		//Sample the pixel color from the texture using the sampler at this texture
		//coordinate location
		diffuse = shaderTexture.Sample(SampleType, input.uv);
    }
	
	clip( input.clip < 0.0 ? -1 : 1 );
	
	float3 toLight = normalize(float3(2,-5,3));
	finalColor = diffuse;
	float I = saturate(dot(normalize(input.normal), toLight));
	I += 0.2f; // Ambient
	return finalColor * I;
	
	return finalColor;
}

float4 OBJMeshTessellationWireframePS(PixelInput input) : SV_Target
{
	clip( input.clip < 0.0 ? -1 : 1 );
	
    return g_DifColor;
}