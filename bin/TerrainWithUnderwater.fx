//------- Constants --------
float4x4 View               : VIEW;
float4x4 Projection         : PROJECTION;
float4x4 World              : WORLD;
float4x4 WorldViewProjection: WORLDVIEWPROJECTION;
float4x4 LightViewProjection: LIGHTVIEWPROJECTION;
float4 LightDir             : LIGHT_DIRECTION;
float4 AmbientColor         : AMBIENT_COLOR;
float AmbientPower          : AMBIENT_POWER;
float4 SpecularColor        : SPECULAR_COLOR;
float SpecularPower         : SPECULAR_POWER;
float4 DiffuseColor         : DIFFUSE_COLOR;
float4 CameraForward        : VIEW_FORWARD;
float4 CameraPos            : VIEW_POS;
float FogNear               : FOG_NEAR;
float FogFar                : FOG_FAR;
float4 FogColor             : FOG_COLOR;
float FogAltitude           : FOG_ALTITUDE;
float FogThinning           : FOG_THINNING;

float TerrainScale          : SCALE_FACTOR;
float TerrainWidth          : TERRAIN_WIDTH;

float WaterElevation        : WATER_ELEVATION;
float4 WaterColor           : WATER_COLOR;

bool UsingClippingPlane     : USING_CLIPPING_PLANE;
float4 ClippingPlane        : CLIPPING_PLANE;

bool CameraUnderwater       : CAMERA_UNDERWATER;

float DepthBias = 0.0001f;

float4 darkWater = float4(0.0f, 0.126f, 0.367f, 1.0f);
float4 lightWater = float4(0.0f, 0.235f, 0.44f, 1.0f);

//------- Texture Samplers --------
Texture TextureMap          : TEXTURE_MAP;
sampler TextureMapSampler = sampler_state { texture = <TextureMap> ; magfilter = LINEAR; minfilter = LINEAR; 
                                                                         mipfilter = LINEAR; AddressU  = Wrap;
                                                                         AddressV  = Wrap; AddressW  = Wrap;};

Texture GrassTexture        : GRASS_TEXTURE;
sampler GrassTextureSampler = sampler_state { texture = <GrassTexture> ; magfilter = LINEAR; minfilter = LINEAR; 
                                                                         mipfilter=LINEAR; AddressU  = Wrap;
                                                                         AddressV  = Wrap; AddressW  = Wrap;};

Texture SandTexture         : SAND_TEXTURE;
sampler SandTextureSampler = sampler_state { texture = <SandTexture> ; magfilter = LINEAR; minfilter = LINEAR; 
                                                                       mipfilter =LINEAR; AddressU  = Wrap;
                                                                       AddressV  = Wrap; AddressW  = Wrap;};

Texture RockTexture         : ROCK_TEXTURE;
sampler RockTextureSampler = sampler_state { texture = <RockTexture> ; magfilter = LINEAR; minfilter = LINEAR; 
                                                                       mipfilter = LINEAR; AddressU  = Wrap;
                                                                       AddressV  = Wrap; AddressW  = Wrap;};

Texture GrassNormal         :GRASS_NORMAL;
sampler2D GrassNormalSampler : TEXUNIT1 = sampler_state
{ Texture   = (GrassNormal); magfilter = LINEAR; minfilter = LINEAR; 
                             mipfilter = LINEAR; AddressU  = Wrap;
                             AddressV  = Wrap; AddressW  = Wrap;};

Texture SandNormal          : SAND_NORMAL;
sampler2D SandNormalSampler : TEXUNIT1 = sampler_state
{ Texture   = (SandNormal); magfilter  = LINEAR; minfilter = LINEAR; 
                             mipfilter = LINEAR; AddressU  = Wrap;
                             AddressV  = Wrap; AddressW  = Wrap;};

Texture RockNormal          : ROCK_NORMAL;
sampler2D RockNormalSampler : TEXUNIT1 = sampler_state
{ Texture   = (RockNormal); magfilter = LINEAR; minfilter = LINEAR; 
                             mipfilter=LINEAR; AddressU  = Wrap;
                             AddressV  = Wrap; AddressW  = Wrap;};

Texture ShadowMap               : SHADOW_MAP;   
sampler ShadowMapSampler = sampler_state { texture = <ShadowMap>; MinFilter = POINT; MagFilter = POINT;
                                                                  MipFilter = NONE;  AddressU = Clamp;
                                                                  AddressV = Clamp;  AddressW  = Wrap; };



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
    float4 shadowInfo = tex2D(ShadowMapSampler, ShadowTexCoord);
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

//------- Technique: MultiTexturedNormaled --------

 struct VS_INPUT
 {
     float4 Position    : POSITION0;    
     float3 Normal      : NORMAL0;
 };

struct VS_OUTPUT
{
    float4 Position     : POSITION;
    float4 TexCoord     : TEXCOORD0;
    float3 Normal       : TEXCOORD1;
    float4 WorldPos     : TEXCOORD2;
    float4 ClipDistances    : TEXCOORD3;
};

 VS_OUTPUT MultiTexturedNormaledVS( VS_INPUT input)    
 {
     VS_OUTPUT Output;

     Output.Position = mul(input.Position, WorldViewProjection);
     Output.WorldPos = mul(input.Position, World);
     Output.Normal = normalize(mul(input.Normal, World));

     Output.TexCoord.x = input.Position.x * 0.03f / TerrainScale;
     Output.TexCoord.y = input.Position.z * 0.03f / TerrainScale;

     Output.TexCoord.z = input.Position.x / (TerrainWidth * TerrainScale);
     Output.TexCoord.w = input.Position.z / (TerrainWidth * TerrainScale);

     Output.ClipDistances = dot(Output.WorldPos, ClippingPlane);

     return Output;    
 }

 float4 MultiTexturedNormaledPS(VS_OUTPUT input) : COLOR0
 {
     if ( UsingClippingPlane )
     {
         clip(input.ClipDistances);
     }

     float3 TerrainColorWeight = tex2D(TextureMapSampler, input.TexCoord.zw);

     // @TODO: Determine if this quad-tree node has all three textures on it, so we don't have to multiply by
     //        unneeded values.
     float4 normalFromMap = (2.0f * tex2D(RockNormalSampler, input.TexCoord) - 1.0f)    * TerrainColorWeight.r;
     normalFromMap += (2.0f * tex2D(GrassNormalSampler, input.TexCoord) - 1.0f)         * TerrainColorWeight.g;
     normalFromMap += (2.0f * tex2D(SandNormalSampler, input.TexCoord) - 1.0f)          * TerrainColorWeight.b;

     float4 Color = tex2D(RockTextureSampler, input.TexCoord)   * TerrainColorWeight.r;
     Color += tex2D(GrassTextureSampler, input.TexCoord)        * TerrainColorWeight.g;
     Color += tex2D(SandTextureSampler, input.TexCoord)         * TerrainColorWeight.b;

     // Factor in normal mapping and terrain vertex normals as well in lighting of the pixel
     float lightingFactor = saturate(dot(normalFromMap + input.Normal, -LightDir));

     float4 WorldPosToCam = float4(input.WorldPos - CameraPos);
     float d = length(WorldPosToCam);

     Color = ComputeShadowColor(input.WorldPos, Color);

     // Handle fog differently if there is water, and also handle if the camera is underwater
     float waterTerrainDiff = WaterElevation - input.WorldPos.y;

     float fogValue = FogThinning;
     float fogAlt = FogAltitude;
     float fogNearDist = FogNear;
     float fogFarDist = FogFar;
     float4 fogColor = FogColor;

     // If this pixel on the terrain is underwater, and the water elevation is not the default value
     if ( waterTerrainDiff > 0 )
     {
        fogValue = 1;   // Setting FogThinning to 1 makes elevation-based fog become distance-based
        fogAlt = WaterElevation;
        fogNearDist = 0;
        fogFarDist = 30;
        d = waterTerrainDiff;

        waterTerrainDiff = min(waterTerrainDiff / 100.f, 1.f);
        fogColor = lerp(lightWater, darkWater, waterTerrainDiff);
     }

     float l = saturate((d - fogNearDist) / (fogFarDist - fogNearDist) / clamp(input.WorldPos.y / fogAlt + 1, 1, fogValue));

     Color.rgb *= (AmbientColor + (DiffuseColor * lightingFactor) + (SpecularColor * lightingFactor));
     Color.a = 1.0f;

     return lerp(Color, fogColor, l);
 }

 technique MultiTexturedNormaled
 {
     pass Pass0
     {
         VertexShader = compile vs_2_0 MultiTexturedNormaledVS();
         PixelShader = compile ps_2_0 MultiTexturedNormaledPS();
     }
 }

 float4 MultiTexturedNormaledCameraUnderwaterPS(VS_OUTPUT input) : COLOR0
 {
     if ( UsingClippingPlane )
     {
         clip(input.ClipDistances);
     }

     float3 TerrainColorWeight = tex2D(TextureMapSampler, input.TexCoord.zw);

     // @TODO: Determine if this quad-tree node has all three textures on it, so we don't have to multiply by
     //        unneeded values.
     float4 normalFromMap = (2.0f * tex2D(RockNormalSampler, input.TexCoord) - 1.0f)    * TerrainColorWeight.r;
     normalFromMap += (2.0f * tex2D(GrassNormalSampler, input.TexCoord) - 1.0f)         * TerrainColorWeight.g;
     normalFromMap += (2.0f * tex2D(SandNormalSampler, input.TexCoord) - 1.0f)          * TerrainColorWeight.b;

     float4 Color = tex2D(RockTextureSampler, input.TexCoord)   * TerrainColorWeight.r;
     Color += tex2D(GrassTextureSampler, input.TexCoord)        * TerrainColorWeight.g;
     Color += tex2D(SandTextureSampler, input.TexCoord)         * TerrainColorWeight.b;

     // Factor in normal mapping and terrain vertex normals as well in lighting of the pixel
     float lightingFactor = saturate(dot(normalFromMap + input.Normal, -LightDir));

     float4 WorldPosToCam = float4(input.WorldPos - CameraPos);
     float d = length(WorldPosToCam);

     Color = ComputeShadowColor(input.WorldPos, Color);

     // Handle fog differently if there is water, and also handle if the camera is underwater
     float waterTerrainDiff = WaterElevation - input.WorldPos.y;

     float fogValue = FogThinning;
     float fogAlt = FogAltitude;
     float fogNearDist = FogNear;
     float fogFarDist = FogFar;
     float4 fogColor = FogColor;

     // If this pixel on the terrain is underwater, and the water elevation is not the default value
     if ( waterTerrainDiff > 0 )
     {
        fogValue = 1;   // Setting FogThinning to 1 makes elevation-based fog become distance-based
        fogAlt = WaterElevation;
        fogNearDist = -200;
        fogFarDist = 250;
        fogColor = WaterColor;
     }

     float l = saturate((d - fogNearDist) / (fogFarDist - fogNearDist) / clamp(input.WorldPos.y / fogAlt + 1, 1, fogValue));

     Color.rgb *= (AmbientColor + (DiffuseColor * lightingFactor) + (SpecularColor * lightingFactor));
     Color.a = 1.0f;

     return lerp(Color, fogColor, l);
 }

 technique MultiTexturedNormaledCameraUnderwater
 {
     pass Pass0
     {
         VertexShader = compile vs_2_0 MultiTexturedNormaledVS();
         PixelShader = compile ps_2_0 MultiTexturedNormaledCameraUnderwaterPS();
     }
 }