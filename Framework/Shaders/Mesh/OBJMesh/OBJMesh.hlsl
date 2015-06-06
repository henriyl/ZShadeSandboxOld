//==============================================================================
// Static OBJ Mesh Shader
//
//==============================================================================
// History
//
// -Created on 4/14/2015 by Dustin Watson
//==============================================================================

//
// Constants
//

cbuffer cbOBJShadingConst : register(b0)
{
	float4  g_ClipPlane;
	float4	g_DifColor;
	float2	padding;
	float	g_FarPlane;
	int		g_hasTexture;
};

cbuffer cbMatrixBuffer : register(b1)
{
	matrix g_WVP;
	matrix g_World;
};

//======================================================================================================

//
// Globals
//

Texture2D shaderTexture : register(t0);

//------------------------------------------------------------------
//Sample States
//------------------------------------------------------------------
// this affects nothing when Effects framework isn't used
SamplerState SampleType : register(s0);
//{
//    Filter = MIN_MAG_MIP_LINEAR;
//    AddressU = Wrap;
//    AddressV = Wrap;
//};

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

struct PixelInput
{
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float pixelDepth	: DEPTH;
	float2 uv			: TEXCOORD0;
	float clip 			: TEXCOORD1;
};

PixelInput OBJMeshVS(VertexInput input)
{
	PixelInput output;
	
	output.position = mul(float4(input.position, 1.0f), g_WVP);
	
	output.normal = input.normal;
    output.uv = input.uv;
    
	output.clip = dot(output.position, g_ClipPlane);
	
	// Logarithmic z buffer
	// DirectX Depth range 0..1
	int C = 1;
	output.pixelDepth = log2(max(1e-6, C * output.position.z + 1)) * 2 / log2(C * g_FarPlane + 1) * output.position.z;
	
	return output;
}

//
// Pixel Shader
//

float4 OBJMeshPS(PixelInput input) : SV_Target
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
}

float4 OBJMeshWireframePS(PixelInput input) : SV_Target
{
	clip( input.clip < 0.0 ? -1 : 1 );
	
    return g_DifColor;
}