//==============================================================================
// Quad Texture Hull Shader
//
//==============================================================================
// History
//
// -Created on 4/8/2015 by Dustin Watson
//==============================================================================

//
// Constants
//

cbuffer cTessellationBuffer : register(b0)
{
	float3 	g_EyePosW;
	int  	g_FrustumCull;
	
	float 	g_MinDist;
	float 	g_MaxDist;
	float 	g_MinTess;
	float 	g_MaxTess;
	
	float2  padding;
	float	g_TerrSize;
	float	g_TerrScale;
};

//======================================================================================================

//
// Globals
//

// returning height data from the gpu
//https://books.google.com/books?id=uIDSBQAAQBAJ&pg=PA49&lpg=PA49&dq=returning+height+data+from+the+gpu&source=bl&ots=XLKM46pkJk&sig=PRUJSfUCujpLLMAHlWs96LgkbI8&hl=en&sa=X&ei=
//CsBWVfn9JYmgNoSagYAI&ved=0CF0Q6AEwCQ#v=onepage&q=returnin//g%20height%20data%20from%20the%20gpu&f=false

float CalcTessFactor(float3 p)
{
	float d = distance(p, g_EyePosW);

	// max norm in xz plane (useful to see detail levels from a bird's eye).
	//float d = max( abs(p.x - gEyePosW.x), abs(p.z - gEyePosW.z) );
	
	float s = saturate( (d - g_MinDist) / (g_MaxDist - g_MinDist) );
	
	return pow(2, (lerp(g_MaxTess, g_MinTess, s)) );
}

float DistanceToPoint(float3 p, float3 vmin, float3 vmax)
{
	float dx = max(vmin.x - p.x, p.x - vmax.x);
	float dz = max(vmin.z - p.z, p.z - vmax.z);
	return sqrt(dx * dx + dz * dz);
}

//======================================================================================================

//
// Hull Shader
//

struct HullInput
{
	float3 position		: POSITION;
	//float3 normal		: NORMAL;
	float2 uv			: TEXCOORD0;
};

struct PatchConstOutput
{
	float edges[4]		: SV_TessFactor;
	float inside[2]		: SV_InsideTessFactor;
};

struct DomainInput
{
	float3 position		: POSITION;
	//float3 normal		: NORMAL;
	float2 uv			: TEXCOORD0;
};

void HCTessellate(out PatchConstOutput output, float factor)
{
	output.edges[0] = factor;
	output.edges[1] = factor;
	output.edges[2] = factor;
	output.edges[3] = factor;
	output.inside[0] = factor;
	output.inside[1] = factor;
}

void HCTessellate(out PatchConstOutput output, float3 e0, float3 e1, float3 e2, float3 e3, float3 c)
{
	output.edges[0] = CalcTessFactor(e0);
	output.edges[1] = CalcTessFactor(e1);
	output.edges[2] = CalcTessFactor(e2);
	output.edges[3] = CalcTessFactor(e3);
	output.inside[0] = CalcTessFactor(c);
	output.inside[1] = output.inside[0];
}

PatchConstOutput ConstHS(InputPatch<HullInput, 4> inputPatch, uint patchID : SV_PrimitiveID)
{
	PatchConstOutput output;
	
	if (g_FrustumCull == 0)
	{
		// Compute midpoint on edges, and patch center
		float3 e0 = 0.5f * (inputPatch[0].position + inputPatch[2].position);
		float3 e1 = 0.5f * (inputPatch[0].position + inputPatch[1].position);
		float3 e2 = 0.5f * (inputPatch[1].position + inputPatch[3].position);
		float3 e3 = 0.5f * (inputPatch[2].position + inputPatch[3].position);
		float3  c = 0.25f * (inputPatch[0].position + inputPatch[1].position + inputPatch[2].position + inputPatch[3].position);
		
		HCTessellate(output, e0, e1, e2, e3, c);
	}
	else
	{
		HCTessellate(output, 0.0f);
	}
	
	return output;
}

[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
[patchconstantfunc("ConstHS")]
DomainInput TerrainTessellationQuadHS(InputPatch<HullInput, 4> inputPatch, uint pointID : SV_OutputControlPointID, uint patchID : SV_PrimitiveID)
{
	DomainInput output;
	
	output.position = inputPatch[pointID].position;
	
	//output.normal = inputPatch[pointID].normal;
	
	output.uv = inputPatch[pointID].uv;
	
	return output;
}