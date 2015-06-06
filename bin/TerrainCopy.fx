 
#include "LightHelper.fx"
 
cbuffer cbPerFrame
{
	DirectionalLight gDirLights[3];
	float3 gEyePosW;

	float  g_fogThinning;
	float  g_fogAltitude;
	float  g_fogNear;
	float  g_fogFar;
	float4 g_fogColor;
	
	float gToggle;
	
	// When distance is minimum, the tessellation is maximum.
	// When distance is maximum, the tessellation is minimum.
	float gMinDist;
	float gMaxDist;

	float gTerrSize;

	// Exponents for power of 2 tessellation.  The tessellation
	// range is [2^(gMinTess), 2^(gMaxTess)].  Since the maximum
	// tessellation is 64, this means gMaxTess can be at most 6
	// since 2^6 = 64.
	float gMinTess;
	float gMaxTess;
	
	float gHeightScale;
	float gTexelCellSpaceU;
	float gTexelCellSpaceV;
	float gWorldCellSpace;
	float2 gTexScale = 50.0f;
	
	float4 gWorldFrustumPlanes[6];

	bool g_useClippingPlane;
	float4 g_ClipPlane;

	float g_seaLevel;
	float4 g_waterColor;

	bool g_cameraUnderwater;
};

float g_depthBias = 0.0001f;


cbuffer cbPerObject
{
	// Terrain coordinate specified directly 
	// at center of world space.
	
	float4x4 gViewProj;
	float4x4 gWorld;
	float4x4 gView;
	float4x4 gProj;
	float4x4 LightViewProjection;

	Material gMaterial;
};

// Nonnumeric values cannot be added to a cbuffer.
Texture2DArray gLayerMapArray;
Texture2D gBlendMap;
Texture2D gHeightMap;

SamplerState samLinear
{
	Filter = MIN_MAG_MIP_LINEAR;

	AddressU = WRAP;
	AddressV = WRAP;
};

SamplerState samHeightmap
{
	Filter = MIN_MAG_LINEAR_MIP_POINT;

	AddressU = CLAMP;
	AddressV = CLAMP;
};

Texture ShadowMap;
SamplerState ShadowMapSampler
{
	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = NONE;
	AddressU = CLAMP;
	AddressV = CLAMP;
	AddressW  = WRAP;
};


struct VertexIn
{
	float3 PosL     : POSITION;
	float2 Tex      : TEXCOORD0;
	//float3 Col      : COLOR;
	float2 BoundsY  : TEXCOORD1;
};

struct VertexOut
{
	float3 PosW     : POSITION;
	float2 Tex      : TEXCOORD0;
	//float3 Col      : COLOR;
	float2 BoundsY  : TEXCOORD1;
};

// For any shader that can support shadow mapping, simply add 
// this function and call it from your pixel shader. Of course,
// you'll need to make sure this shader and material are setup to
// receive the ShadowMap sampler, and any needed parameters (e.g. LightViewProjection)                                                        
float4 ComputeShadowColor(float4 worldPos, float4 Color)
{
     // Find the position of this pixel in light space
    float4 lightingPosition = mul(worldPos, LightViewProjection);

    // Find the position in the shadow map for this pixel
    float2 ShadowTexCoord = 0.5 * lightingPosition.xy / 
                            lightingPosition.w + float2( 0.5, 0.5 );
    ShadowTexCoord.y = 1.0f - ShadowTexCoord.y;

    // Get the current depth stored in the shadow map
    float4 shadowInfo = ShadowMap.Sample(ShadowMapSampler, ShadowTexCoord);
    float shadowdepth = shadowInfo.r;
    float shadowOpacity = 0.5f + 0.5f * (1 - shadowInfo.g);

    // Calculate the current pixel depth
    // The bias is used to prevent folating point errors that occur when
    // the pixel of the occluder is being drawn
    float ourdepth = (lightingPosition.z / lightingPosition.w) - DepthBias;

    // Check to see if this pixel is in front or behind the value in the shadow map
    if ( shadowdepth < ourdepth)
    {
        // Shadow the pixel by lowering the intensity
        Color *= float4(shadowOpacity, shadowOpacity, shadowOpacity, 1);
    };

    return Color;
}

VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	
	// Terrain specified directly in world space.
	vout.PosW = vin.PosL;

	// Displace the patch corners to world space.  This is to make 
	// the eye to patch distance calculation more accurate.
	vout.PosW.y = gHeightMap.SampleLevel( samHeightmap, vin.Tex, 0 ).r;

	// Output vertex attributes to next stage.
	vout.Tex      = vin.Tex;
	//vout.Col      = vin.Col;
	vout.BoundsY  = vin.BoundsY;
	
	return vout;
}
 
float CalcTessFactor(float3 p)
{
	float d = distance(p, gEyePosW);

	// max norm in xz plane (useful to see detail levels from a bird's eye).
	//float d = max( abs(p.x-gEyePosW.x), abs(p.z-gEyePosW.z) );
	
	float s = saturate( (d - gMinDist) / (gMaxDist - gMinDist) );
	
	return pow(2, (lerp(gMaxTess, gMinTess, s)) );
}

// Returns true if the box is completely behind (in negative half space) of plane.
bool AabbBehindPlaneTest(float3 center, float3 extents, float4 plane)
{
	float3 n = abs(plane.xyz);
	
	// This is always positive.
	float r = dot(extents, n);
	
	// signed distance from center point to plane.
	float s = dot( float4(center, 1.0f), plane );
	
	// If the center point of the box is a distance of e or more behind the
	// plane (in which case s is negative since it is behind the plane),
	// then the box is completely in the negative half space of the plane.
	return (s + r) < (-700.0f * gTerrSize);
}

// Returns true if the box is completely outside the frustum.
bool AabbOutsideFrustumTest(float3 center, float3 extents, float4 frustumPlanes[6])
{
	for(int i = 0; i < 6; ++i)
	{
		// If the box is completely behind any of the frustum planes
		// then it is outside the frustum.
		if( AabbBehindPlaneTest(center, extents, frustumPlanes[i]) )
		{
			return true;
		}
	}
	
	return false;
}

struct PatchTess
{
	float EdgeTess[4]   : SV_TessFactor;
	float InsideTess[2] : SV_InsideTessFactor;
};

PatchTess ConstantHS(InputPatch<VertexOut, 4> patch, uint patchID : SV_PrimitiveID)
{
	PatchTess pt;
	
	//
	// Frustum cull
	//
	
	// We store the patch BoundsY in the first control point.
	float minY = patch[0].BoundsY.x;
	float maxY = patch[0].BoundsY.y;
	
	// Build axis-aligned bounding box.  patch[2] is lower-left corner
	// and patch[1] is upper-right corner.
	float3 vMin = float3(patch[2].PosW.x, minY, patch[2].PosW.z);
	float3 vMax = float3(patch[1].PosW.x, maxY, patch[1].PosW.z);
	
	float3 boxCenter  = 0.5f*(vMin + vMax);
	float3 boxExtents = 0.5f*(vMax - vMin);
	if( AabbOutsideFrustumTest(boxCenter, boxExtents, gWorldFrustumPlanes) )
	{
		pt.EdgeTess[0] = 0.0f;
		pt.EdgeTess[1] = 0.0f;
		pt.EdgeTess[2] = 0.0f;
		pt.EdgeTess[3] = 0.0f;
		
		pt.InsideTess[0] = 0.0f;
		pt.InsideTess[1] = 0.0f;
		
		return pt;
	}
	//
	// Do normal tessellation based on distance.
	//
	else 
	{
		// It is important to do the tess factor calculation based on the
		// edge properties so that edges shared by more than one patch will
		// have the same tessellation factor.  Otherwise, gaps can appear.
		
		// Compute midpoint on edges, and patch center
		float3 e0 = 0.5f*(patch[0].PosW + patch[2].PosW);
		float3 e1 = 0.5f*(patch[0].PosW + patch[1].PosW);
		float3 e2 = 0.5f*(patch[1].PosW + patch[3].PosW);
		float3 e3 = 0.5f*(patch[2].PosW + patch[3].PosW);
		float3  c = 0.25f*(patch[0].PosW + patch[1].PosW + patch[2].PosW + patch[3].PosW);
		
		pt.EdgeTess[0] = CalcTessFactor(e0);
		pt.EdgeTess[1] = CalcTessFactor(e1);
		pt.EdgeTess[2] = CalcTessFactor(e2);
		pt.EdgeTess[3] = CalcTessFactor(e3);
		
		pt.InsideTess[0] = CalcTessFactor(c);
		pt.InsideTess[1] = pt.InsideTess[0];
	
		return pt;
	}
}

struct HullOut
{
	float3 PosW     : POSITION;
	float2 Tex      : TEXCOORD0;
	//float3 Col      : COLOR;
};

[domain("quad")]
[partitioning("fractional_even")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("ConstantHS")]
[maxtessfactor(64.0f)]
HullOut HS(InputPatch<VertexOut, 4> p, 
           uint i : SV_OutputControlPointID,
           uint patchId : SV_PrimitiveID)
{
	HullOut hout;
	
	// Pass through shader.
	hout.PosW     = p[i].PosW;
	hout.Tex      = p[i].Tex;
	//hout.Col      = p[i].Col;
	
	return hout;
}

struct DomainOut
{
	float4 PosH     : SV_POSITION;
    float3 PosW     : POSITION;
	float2 Tex      : TEXCOORD0;
	//float3 Col      : COLOR;
	float2 TiledTex : TEXCOORD1;
	float clip : TEXCOORD2;
};

// The domain shader is called for every vertex created by the tessellator.  
// It is like the vertex shader after tessellation.
[domain("quad")]
DomainOut DS(PatchTess patchTess, 
             float2 uv : SV_DomainLocation, 
             const OutputPatch<HullOut, 4> quad)
{
	DomainOut dout;
	
	// Bilinear interpolation.
	dout.PosW = lerp(
		lerp(quad[0].PosW, quad[1].PosW, uv.x),
		lerp(quad[2].PosW, quad[3].PosW, uv.x),
		uv.y); 
	
	dout.Tex = lerp(
		lerp(quad[0].Tex, quad[1].Tex, uv.x),
		lerp(quad[2].Tex, quad[3].Tex, uv.x),
		uv.y); 
		
	//dout.Col = lerp(
	//	lerp(quad[0].Col, quad[1].Col, uv.x),
	//	lerp(quad[2].Col, quad[3].Col, uv.x),
	//	uv.y); 
		
	// Tile layer textures over terrain.
	dout.TiledTex = dout.Tex*gTexScale; 
	
	// Displacement mapping

	// Choose the mipmap level based on distance to the eye; specifically, choose
	// the next miplevel every MipInterval units, and clamp the miplevel in [0,6].
	//const float MipInterval = 20.0f;
	//float mipLevel = clamp( (distance(dout.PosW, gEyePosW) - MipInterval) / MipInterval, 0.0f, 6.0f);
	
	// Sample height map (stored in alpha channel).
	//float h = gBlendMap.SampleLevel(samLinear, dout.Tex, mipLevel).a;
	
	// Offset vertex along normal.
	//dout.PosW += (gHeightScale * (h - 1.0)) * dout.NormalW;
	
	dout.PosW.y = gHeightMap.SampleLevel( samHeightmap, dout.Tex, 0 ).r;
	
	// NOTE: We tried computing the normal in the shader using finite difference, 
	// but the vertices move continuously with fractional_even which creates
	// noticable light shimmering artifacts as the normal changes.  Therefore,
	// we moved the calculation to the pixel shader.  
	
	// Project to homogeneous clip space.
	dout.PosH    = mul(float4(dout.PosW, 1.0f), gViewProj);
	
	//dout.PosH = mul(float4(dout.PosW, 1.0f), gWorld);
	//dout.PosH = mul(dout.PosH, gView);
        //dout.PosH = mul(dout.PosH, gProj);
	
	//if (gToggle == 1.0f)
	//dout.clip = dot(mul(dout.PosW, gWorld), gClipPlane);
	dout.clip = dot(dout.PosH, g_ClipPlane);

	return dout;
}

float4 PS(DomainOut pin, 
          uniform int gLightCount, 
		  uniform bool gFogEnabled) : SV_Target
{
	//
	// Estimate normal and tangent using central differences.
	//
	float2 leftTex   = pin.Tex + float2(-gTexelCellSpaceU, 0.0f);
	float2 rightTex  = pin.Tex + float2(gTexelCellSpaceU, 0.0f);
	float2 bottomTex = pin.Tex + float2(0.0f, gTexelCellSpaceV);
	float2 topTex    = pin.Tex + float2(0.0f, -gTexelCellSpaceV);
	
	float leftY   = gHeightMap.SampleLevel( samHeightmap, leftTex, 0 ).r;
	float rightY  = gHeightMap.SampleLevel( samHeightmap, rightTex, 0 ).r;
	float bottomY = gHeightMap.SampleLevel( samHeightmap, bottomTex, 0 ).r;
	float topY    = gHeightMap.SampleLevel( samHeightmap, topTex, 0 ).r;
	
	float3 tangent = normalize(float3(2.0f*gWorldCellSpace, rightY - leftY, 0.0f));
	float3 bitan   = normalize(float3(0.0f, bottomY - topY, -2.0f*gWorldCellSpace)); 
	float3 normalW = cross(tangent, bitan);

	// The toEye vector is used in lighting.
	float3 toEye = gEyePosW - pin.PosW;

	// Cache the distance to the eye from this surface point.
	float distToEye = length(toEye);

	// Normalize.
	toEye /= distToEye;
	
	//
	// Texturing
	//
	
	// Sample layers in texture array.
	float4 c0 = gLayerMapArray.Sample( samLinear, float3(pin.TiledTex, 0.0f) );
	float4 c1 = gLayerMapArray.Sample( samLinear, float3(pin.TiledTex, 1.0f) );
	float4 c2 = gLayerMapArray.Sample( samLinear, float3(pin.TiledTex, 2.0f) );
	float4 c3 = gLayerMapArray.Sample( samLinear, float3(pin.TiledTex, 3.0f) );
	float4 c4 = gLayerMapArray.Sample( samLinear, float3(pin.TiledTex, 4.0f) ); 
	
	// Sample the blend map.
	float4 t  = gBlendMap.Sample( samLinear, pin.Tex );
    
    // Blend the layers on top of each other.
    float4 texColor = c0;
    texColor = lerp(texColor, c1, t.r);
    texColor = lerp(texColor, c2, t.g);
    texColor = lerp(texColor, c3, t.b);
    texColor = lerp(texColor, c4, t.a);
 
	//
	// Lighting.
	//

	float4 litColor = texColor;
	if( gLightCount > 0  )
	{  
		// Start with a sum of zero. 
		float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 spec    = float4(0.0f, 0.0f, 0.0f, 0.0f);

		// Sum the light contribution from each light source.  
		[unroll]
		for(int i = 0; i < gLightCount; ++i)
		{
			float4 A, D, S;
			ComputeDirectionalLight(gMaterial, gDirLights[i], normalW, toEye, 
				A, D, S);

			ambient += A;
			diffuse += D;
			spec    += S;
		}

		litColor = texColor*(ambient + diffuse) + spec;
	}
 
	//
	// Fogging
	//

	if( gFogEnabled )
	{
		float fogLerp = saturate( (distToEye - gFogStart) / gFogRange ); 

		// Blend the fog color and the lit color.
		litColor = lerp(litColor, gFogColor, fogLerp);
	}
//float4(1.0f, 1.0f, 1.0f, 1.0f);
    if (pin.clip < 0.0)
    {
        clip(-1);
		//return float4(255,255,255,255);
		//litColor.a = 0.0f;
    }
    return litColor;
}

float4 WireframePS(DomainOut input) : SV_Target
{
    return float4(0.9f, 0.9f, 0.9f, 1);
}

technique11 NoLight
{
    pass P0
    {
		
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetHullShader( CompileShader( hs_5_0, HS() ) );
        SetDomainShader( CompileShader( ds_5_0, DS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(0, false) ) );
    }
}

technique11 Light1
{
    pass P0
    {
		
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetHullShader( CompileShader( hs_5_0, HS() ) );
        SetDomainShader( CompileShader( ds_5_0, DS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(1, false) ) );
    }
}

technique11 Light2
{
    pass P0
    {
		
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetHullShader( CompileShader( hs_5_0, HS() ) );
        SetDomainShader( CompileShader( ds_5_0, DS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(2, false) ) );
    }
}

technique11 Light3
{
    pass P0
    {
		
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetHullShader( CompileShader( hs_5_0, HS() ) );
        SetDomainShader( CompileShader( ds_5_0, DS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(3, false) ) );
    }
}

technique11 Light1Fog
{
    pass P0
    {
		
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetHullShader( CompileShader( hs_5_0, HS() ) );
        SetDomainShader( CompileShader( ds_5_0, DS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(1, true) ) );
    }
}

technique11 Light2Fog
{
    pass P0
    {
		
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetHullShader( CompileShader( hs_5_0, HS() ) );
        SetDomainShader( CompileShader( ds_5_0, DS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(2, true) ) );
    }
}

technique11 Light3Fog
{
    pass P0
    {
		
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetHullShader( CompileShader( hs_5_0, HS() ) );
        SetDomainShader( CompileShader( ds_5_0, DS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS(3, true) ) );
    }
}

technique11 WireframeTech
{
    pass P0
    {
		
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
        SetHullShader( CompileShader( hs_5_0, HS() ) );
        SetDomainShader( CompileShader( ds_5_0, DS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, WireframePS() ) );
    }
}