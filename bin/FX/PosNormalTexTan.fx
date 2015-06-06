cbuffer cbPerFrame
{
	// Light related
	float4 AmbientColor;
	float  AmbientIntensity;

	float3 LightDirection;
	float4 DiffuseColor;
	float  DiffuseIntensity;

	float4 SpecularColor;
	float SpecularShininess;
	float3 EyePosition;
	
	float4 gClipPlane;
};

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gViewProj;
};

Texture2D gColorMap;
Texture2D gNormalMap;

SamplerState samLinear
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};

struct VSInput
{
	float4 position : POSITION;
	float2 tex      : TEXCOORD0;
	float3 normal   : NORMAL;
	float3 tangent  : TANGENT;
	//float3 binormal : BINORMAL;
};

struct VSOutput
{
	float4 position : SV_POSITION;
	float2 tex      : TEXCOORD0;
	float3 view     : TEXCOORD1;
	float clip      : TEXCOORD2;
	float3x3 WorldToTangentSpace : TEXCOORD3;
};

VSOutput VS(VSInput input)
{
	VSOutput output;
	
	float4 worldPosition = mul(input.position, gWorld);

	//input.position.z += sin((TotalTime * 16) + (input.position.y / 1)) / 16;

	output.position = mul(input.position, gViewProj);

	output.tex = input.tex;

	output.WorldToTangentSpace[0] = mul(normalize(input.tangent), gWorld);
	output.WorldToTangentSpace[1] = mul(normalize(input.normal), gWorld);
	output.WorldToTangentSpace[2] = mul(normalize(input.normal), gWorld);

	output.view = normalize(float4(EyePosition, 1.0) - worldPosition);
	
	output.clip = dot(mul(output.position, gWorld), gClipPlane);
	//dout.clip = dot(dout.PosH, gClipPlane);

	return output;
}

float4 PS(VSOutput input, uniform bool gEnableLighting) : SV_Target
{
	float4 finalColor;

	float4 color = gColorMap.Sample( samLinear, input.tex );

	finalColor = color;

	float3 normalMap = 2.0 * (gNormalMap.Sample( samLinear, input.tex )) - 1.0;

	normalMap = normalize(mul(normalMap, input.WorldToTangentSpace));

	float4 normal = float4(normalMap, 1.0);
	if (gEnableLighting)
	{
	    float4 diffuse = saturate(dot(-LightDirection, normal));
		float4 reflect = normalize(2 * diffuse * normal - float4(LightDirection, 1.0));
		float4 specular = pow(saturate(dot(reflect, input.view)), SpecularShininess);

		finalColor =  color * AmbientIntensity + color * DiffuseIntensity * diffuse + specular * 250;
	}

	//finalColor.a = 0.3f;

	if (input.clip < 0.0)
		clip(-1);

	return finalColor;
}

technique11 PosNormalTexTanNoLight
{
	pass P0
	{
		SetVertexShader( CompileShader(vs_5_0, VS()) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader(ps_5_0, PS(false)) );
	}
}

technique11 PosNormalTexTanLight
{
	pass P0
	{
		SetVertexShader( CompileShader(vs_5_0, VS()) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader(ps_5_0, PS(true)) );
	}
}