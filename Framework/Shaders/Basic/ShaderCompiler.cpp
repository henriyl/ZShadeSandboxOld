#include "ShaderCompiler.h"
#include "ZShadeMessageCenter.h"
//#include "PostProcessor.h"
using ZShadeSandboxShader::ShaderCompiler;
//=================================================================================================================
//=================================================================================================================
map<string, ShaderCompiler::ShaderMacros*> ShaderCompiler::m_ShaderMacros;
vector<ZShadeSandboxShader::ShaderBase*> ShaderCompiler::m_Shaders;
//=================================================================================================================
// Converts a number to an ansi string
template<typename T> inline std::string ToAnsiString(const T& val)
{
	std::ostringstream stream;
	if (!(stream << val))
	{
		exit(0);
	}
	//throw Exception(L"Error converting value to string");
	return stream.str();
}
//=================================================================================================================
ShaderCompiler::ShaderMacros::ShaderMacros()
:   bufferIdx(0)
,   numDefines(0)
{
	ClearMacroDefines();
}
//=================================================================================================================
void ShaderCompiler::ShaderMacros::ClearMacroDefines()
{
	for (UINT32 i = 0; i < MaxDefines; ++i)
	{
		defines[i].Name = NULL;
		defines[i].Definition = NULL;
	}

	ZeroMemory(buffer, BufferSize);
}
//=================================================================================================================
void ShaderCompiler::ShaderMacros::AddMacro(const std::string& name, UINT32 value)
{
	assert(numDefines < MaxDefines);

	defines[numDefines].Name = buffer + bufferIdx;
	for (UINT32 i = 0; i < name.length(); ++i)
		buffer[bufferIdx++] = name[i];
	++bufferIdx;

	std::string stringVal = ToAnsiString(value);
	defines[numDefines].Definition = buffer + bufferIdx;
	for (UINT32 i = 0; i < stringVal.length(); ++i)
		buffer[bufferIdx++] = stringVal[i];
	++bufferIdx;

	++numDefines;
}
//=================================================================================================================
void ShaderCompiler::CompileAll(D3D* d3d)
{
	// ==================================== Compile the Shadowmap Builder Shader
	Compile(d3d, "Environment\\Lighting\\ShadowMap.hlsl", "ShadowMapBuilderVS", EShaderTypes::ST_VERTEX);
	Compile(d3d, "Environment\\Lighting\\ShadowMap.hlsl", "ShadowMapBuilderPS", EShaderTypes::ST_PIXEL);
	
	// ==================================== Compile the Color Shader
	//Compile(d3d, "Basic\\Color.hlsl", "colorVertexShader", EShaderTypes::ST_VERTEX);
	//Compile(d3d, "Basic\\Color.hlsl", "colorPixelShader", EShaderTypes::ST_PIXEL);
	
	// ==================================== Compile the Light Shader
	Compile(d3d, "Basic\\LightShader.hlsl", "LightShaderVS", EShaderTypes::ST_VERTEX);
	Compile(d3d, "Basic\\LightShader.hlsl", "LightShaderPS", EShaderTypes::ST_PIXEL);
	Compile(d3d, "Basic\\LightShader.hlsl", "LightShaderWireframePS", EShaderTypes::ST_PIXEL);

	// ==================================== Compile the Texture Shader
	Compile(d3d, "Basic\\Texture.hlsl", "textureVertexShader", EShaderTypes::ST_VERTEX);
	Compile(d3d, "Basic\\Texture.hlsl", "texturePixelShader", EShaderTypes::ST_PIXEL);
	Compile(d3d, "Basic\\Texture.hlsl", "TextureWireframePS", EShaderTypes::ST_PIXEL);
	
	// ==================================== Compile the Material Shader
	Compile(d3d, "Basic\\MaterialShader.hlsl", "MaterialShaderVS", EShaderTypes::ST_VERTEX);
	Compile(d3d, "Basic\\MaterialShader.hlsl", "MaterialShaderPS", EShaderTypes::ST_PIXEL);
	Compile(d3d, "Basic\\MaterialShader.hlsl", "MaterialShaderWireframePS", EShaderTypes::ST_PIXEL);
	
	// ==================================== Compile the Transparent Shader
	//Compile(d3d, "Basic\\Transparent.hlsl", "transparentVertexShader", EShaderTypes::ST_VERTEX);
	//Compile(d3d, "Basic\\Transparent.hlsl", "transparentPixelShader", EShaderTypes::ST_PIXEL);
	//Compile(d3d, "Basic\\Transparent.hlsl", "transparentWireframePS", EShaderTypes::ST_PIXEL);

	// ==================================== Compile the sprite gui shader
	Compile(d3d, "Basic\\Sprite.hlsl", "SpriteVS", EShaderTypes::ST_VERTEX);
	Compile(d3d, "Basic\\Sprite.hlsl", "SpriteInstancedVS", EShaderTypes::ST_VERTEX);
	Compile(d3d, "Basic\\Sprite.hlsl", "SpritePS", EShaderTypes::ST_PIXEL);

	// ==================================== Compile the Position Normal Texture Tangent Shader
	//Compile(d3d, "Basic\\PosNormalTexTan.hlsl", "PNTT_VS", EShaderTypes::ST_VERTEX);
	//Compile(d3d, "Basic\\PosNormalTexTan.hlsl", "PNTT_PS", EShaderTypes::ST_PIXEL);
	//Compile(d3d, "Basic\\PosNormalTexTan.hlsl", "PNTTWireframePS", EShaderTypes::ST_PIXEL);

	// ==================================== Compile the Desaturation Compute Shader
	Compile(d3d, "Basic\\CSDesaturation.hlsl", "main", EShaderTypes::ST_COMPUTE);
	
	// ==================================== Compile the Tessellation Color Shader
	/*Compile(d3d, "BasicTessellation\\TessellationColorVS.hlsl", "TessellationColorVS", EShaderTypes::ST_VERTEX);
	Compile(d3d, "BasicTessellation\\TessellationColorPS.hlsl", "TessellationColorPS", EShaderTypes::ST_PIXEL);
	
	// ==================================== Compile the Tessellation Texture Shader
	Compile(d3d, "BasicTessellation\\TessellationTextureVS.hlsl", "TessellationTextureVS", EShaderTypes::ST_VERTEX);
	Compile(d3d, "BasicTessellation\\TessellationTexturePS.hlsl", "TessellationTexturePS", EShaderTypes::ST_PIXEL);
	
	// ==================================== Compile the Tessellation Quad Color Shader
	Compile(d3d, "BasicTessellation\\TessellationQuadColorHS.hlsl", "TessellationQuadColorHS", EShaderTypes::ST_HULL);
	Compile(d3d, "BasicTessellation\\TessellationQuadColorDS.hlsl", "TessellationQuadColorDS", EShaderTypes::ST_DOMAIN);
	
	// ==================================== Compile the Tessellation Quad Texture Shader
	Compile(d3d, "BasicTessellation\\TessellationQuadTextureHS.hlsl", "TessellationQuadTextureHS", EShaderTypes::ST_HULL);
	Compile(d3d, "BasicTessellation\\TessellationQuadTextureDS.hlsl", "TessellationQuadTextureDS", EShaderTypes::ST_DOMAIN);
	
	// ==================================== Compile the Tessellation Tri Color Shader
	Compile(d3d, "BasicTessellation\\TessellationTriColorHS.hlsl", "TessellationTriColorHS", EShaderTypes::ST_HULL);
	Compile(d3d, "BasicTessellation\\TessellationTriColorDS.hlsl", "TessellationTriColorDS", EShaderTypes::ST_DOMAIN);
	
	// ==================================== Compile the Tessellation Tri Texture Shader
	Compile(d3d, "BasicTessellation\\TessellationTriTextureHS.hlsl", "TessellationTriTextureHS", EShaderTypes::ST_HULL);
	Compile(d3d, "BasicTessellation\\TessellationTriTextureDS.hlsl", "TessellationTriTextureDS", EShaderTypes::ST_DOMAIN);
	*/

	// ==================================== Compile the Material Tessellation Shader
	Compile(d3d, "BasicTessellation\\MaterialTessellationShaderVS.hlsl", "MaterialTessellationShaderVS", EShaderTypes::ST_VERTEX);
	Compile(d3d, "BasicTessellation\\MaterialTessellationShaderPS.hlsl", "MaterialTessellationShaderPS", EShaderTypes::ST_PIXEL);
	Compile(d3d, "BasicTessellation\\MaterialTessellationShaderPS.hlsl", "MaterialTessellationShaderWireframePS", EShaderTypes::ST_PIXEL);
	Compile(d3d, "BasicTessellation\\QuadMaterialTessellationShaderHS.hlsl", "QuadMaterialTessellationShaderHS", EShaderTypes::ST_HULL);
	Compile(d3d, "BasicTessellation\\QuadMaterialTessellationShaderDS.hlsl", "QuadMaterialTessellationShaderDS", EShaderTypes::ST_DOMAIN);
	Compile(d3d, "BasicTessellation\\TriMaterialTessellationShaderHS.hlsl", "TriMaterialTessellationShaderHS", EShaderTypes::ST_HULL);
	Compile(d3d, "BasicTessellation\\TriMaterialTessellationShaderDS.hlsl", "TriMaterialTessellationShaderDS", EShaderTypes::ST_DOMAIN);

	// ==================================== Compile the Ocean Shaders
	//Compile(d3d, "Ocean\\OceanFFTCS.hlsl", "Radix008A_CS", EShaderTypes::ST_COMPUTE);
	//Compile(d3d, "Ocean\\OceanFFTCS.hlsl", "Radix008A_CS2", EShaderTypes::ST_COMPUTE);
	//Compile(d3d, "Ocean\\OceanFFTShader.hlsl", "QuadVS", EShaderTypes::ST_VERTEX);
	//Compile(d3d, "Ocean\\OceanFFTShader.hlsl", "UpdateDisplacementPS", EShaderTypes::ST_PIXEL);
	//Compile(d3d, "Ocean\\OceanFFTShader.hlsl", "GenGradientFoldingPS", EShaderTypes::ST_PIXEL);
	//Compile(d3d, "Ocean\\OceanHeightfieldCS.hlsl", "UpdateSpectrumCS", EShaderTypes::ST_COMPUTE);
	//Compile(d3d, "Ocean\\OceanShading.hlsl", "OceanSurfVS", EShaderTypes::ST_VERTEX);
	//Compile(d3d, "Ocean\\OceanShading.hlsl", "OceanSurfPS", EShaderTypes::ST_PIXEL);
	//Compile(d3d, "Ocean\\OceanShading.hlsl", "WireframePS", EShaderTypes::ST_PIXEL);
	
	/*Compile(d3d, "Ocean\\OceanFFTCS.hlsl", "Radix008A_CS", EShaderTypes::ST_COMPUTE);
	Compile(d3d, "Ocean\\OceanFFTCS.hlsl", "Radix008A_CS2", EShaderTypes::ST_COMPUTE);
	Compile(d3d, "Ocean\\OceanFFTShader.hlsl", "QuadVS", EShaderTypes::ST_VERTEX);
	Compile(d3d, "Ocean\\OceanFFTShader.hlsl", "UpdateDisplacementPS", EShaderTypes::ST_PIXEL);
	Compile(d3d, "Ocean\\OceanFFTShader.hlsl", "GenGradientFoldingPS", EShaderTypes::ST_PIXEL);
	Compile(d3d, "Ocean\\OceanHeightfieldCS.hlsl", "UpdateSpectrumCS", EShaderTypes::ST_COMPUTE);
	Compile(d3d, "Ocean\\OceanSurfaceVS.hlsl", "OceanSurfaceVS", EShaderTypes::ST_VERTEX);
	Compile(d3d, "Ocean\\OceanSurfaceHS.hlsl", "OceanSurfaceHS", EShaderTypes::ST_HULL);
	Compile(d3d, "Ocean\\OceanSurfaceDS.hlsl", "OceanSurfaceDS", EShaderTypes::ST_DOMAIN);
	Compile(d3d, "Ocean\\OceanSurfacePS.hlsl", "OceanSurfacePS", EShaderTypes::ST_PIXEL);
	Compile(d3d, "Ocean\\OceanSurfacePS.hlsl", "OceanSurfaceWireframePS", EShaderTypes::ST_PIXEL);*/

	//Compile(d3d, "Ocean\\OceanPatch.hlsl", "OceanPatchVS", EShaderTypes::ST_VERTEX);
	//Compile(d3d, "Ocean\\OceanPatch.hlsl", "OceanPatchPS", EShaderTypes::ST_PIXEL);
	//Compile(d3d, "Ocean\\OceanPatch.hlsl", "OceanPatchWireframePS", EShaderTypes::ST_PIXEL);

	// ==================================== Compile the Water Shader
	Compile(d3d, "Water\\Water.hlsl", "WaterNormalVS", EShaderTypes::ST_VERTEX);
	Compile(d3d, "Water\\Water.hlsl", "WaterFlowVS", EShaderTypes::ST_VERTEX);
	Compile(d3d, "Water\\Water.hlsl", "WaterNormalPS", EShaderTypes::ST_PIXEL);
	Compile(d3d, "Water\\Water.hlsl", "WaterFlowPS", EShaderTypes::ST_PIXEL);
	Compile(d3d, "Water\\Water.hlsl", "WaterNormalWireframePS", EShaderTypes::ST_PIXEL);
	Compile(d3d, "Water\\Water.hlsl", "WaterFlowWireframePS", EShaderTypes::ST_PIXEL);
	Compile(d3d, "Water\\LakeBed.hlsl", "LakeBedVS", EShaderTypes::ST_VERTEX);
	Compile(d3d, "Water\\LakeBed.hlsl", "LakeBedPS", EShaderTypes::ST_PIXEL);
	Compile(d3d, "Water\\LakeBed.hlsl", "LakeBedWireframePS", EShaderTypes::ST_PIXEL);
	
	// ==================================== Compile the Sky Shader
	Compile(d3d, "Sky\\Sky.hlsl", "SkyVS", EShaderTypes::ST_VERTEX);
	Compile(d3d, "Sky\\Sky.hlsl", "SkyPS", EShaderTypes::ST_PIXEL);
	Compile(d3d, "Sky\\Sky.hlsl", "SkyWireframePS", EShaderTypes::ST_PIXEL);
	Compile(d3d, "Sky\\Sky.hlsl", "DeferredSkyVS", EShaderTypes::ST_VERTEX);
	Compile(d3d, "Sky\\Sky.hlsl", "DeferredSkyPS", EShaderTypes::ST_PIXEL);
	Compile(d3d, "Sky\\Sky.hlsl", "DeferredSkyWireframePS", EShaderTypes::ST_PIXEL);
	
	// ==================================== Compile the SkyPlane Shader
	Compile(d3d, "Sky\\SkyPlane.hlsl", "skyPlaneVS", EShaderTypes::ST_VERTEX);
	Compile(d3d, "Sky\\SkyPlane.hlsl", "skyPlanePS", EShaderTypes::ST_PIXEL);
	Compile(d3d, "Sky\\SkyPlane.hlsl", "skyPlaneWireframePS", EShaderTypes::ST_PIXEL);
	
	// ==================================== Compile the Sun Shader
	Compile(d3d, "Sun\\Sun.hlsl", "SunVS", EShaderTypes::ST_VERTEX);
	Compile(d3d, "Sun\\Sun.hlsl", "SunPS", EShaderTypes::ST_PIXEL);
	Compile(d3d, "Sun\\Sun.hlsl", "SunWireframePS", EShaderTypes::ST_PIXEL);
	
	// ==================================== Compile the Terrain Shader
	Compile(d3d, "Terrain\\TerrainTessellationQuadVS.hlsl", "TerrainTessellationQuadVS", EShaderTypes::ST_VERTEX);
	Compile(d3d, "Terrain\\TerrainTessellationQuadHS.hlsl", "TerrainTessellationQuadHS", EShaderTypes::ST_HULL);
	Compile(d3d, "Terrain\\TerrainTessellationQuadShadowHS.hlsl", "TerrainTessellationQuadShadowHS", EShaderTypes::ST_HULL);
	Compile(d3d, "Terrain\\TerrainTessellationQuadDS.hlsl", "TerrainTessellationQuadDS", EShaderTypes::ST_DOMAIN);
	Compile(d3d, "Terrain\\TerrainTessellationQuadPS.hlsl", "TerrainTessellationQuadPS", EShaderTypes::ST_PIXEL);
	Compile(d3d, "Terrain\\TerrainTessellationQuadPS.hlsl", "TerrainTessellationNormalDepthPS", EShaderTypes::ST_PIXEL);
	Compile(d3d, "Terrain\\TerrainTessellationQuadPS.hlsl", "TerrainTessellationWireframePS", EShaderTypes::ST_PIXEL);
	Compile(d3d, "Terrain\\TerrainTessellationQuadPS.hlsl", "TerrainTessellationShadowPS", EShaderTypes::ST_PIXEL);
	Compile(d3d, "Terrain\\TerrainTessellationTriVS.hlsl", "TerrainTessellationTriVS", EShaderTypes::ST_VERTEX);
	Compile(d3d, "Terrain\\TerrainTessellationTriHS.hlsl", "TerrainTessellationTriHS", EShaderTypes::ST_HULL);
	Compile(d3d, "Terrain\\TerrainTessellationTriDS.hlsl", "TerrainTessellationTriDS", EShaderTypes::ST_DOMAIN);
	Compile(d3d, "Terrain\\TerrainTessellationTriPS.hlsl", "TerrainTessellationTriPS", EShaderTypes::ST_PIXEL);
	
	// ==================================== Compile the OBJ Mesh Shader
	Compile(d3d, "Mesh\\OBJMesh\\OBJMesh.hlsl", "OBJMeshVS", EShaderTypes::ST_VERTEX);
	Compile(d3d, "Mesh\\OBJMesh\\OBJMesh.hlsl", "OBJMeshPS", EShaderTypes::ST_PIXEL);
	Compile(d3d, "Mesh\\OBJMesh\\OBJMesh.hlsl", "OBJMeshWireframePS", EShaderTypes::ST_PIXEL);
	
	// ==================================== Compile the OBJ Mesh Tessellation Shader
	Compile(d3d, "Mesh\\OBJMesh\\OBJMeshTessellationVS.hlsl", "OBJMeshTessellationVS", EShaderTypes::ST_VERTEX);
	Compile(d3d, "Mesh\\OBJMesh\\OBJMeshTessellationHS.hlsl", "OBJMeshTessellationHS", EShaderTypes::ST_HULL);
	Compile(d3d, "Mesh\\OBJMesh\\OBJMeshTessellationDS.hlsl", "OBJMeshTessellationDS", EShaderTypes::ST_DOMAIN);
	Compile(d3d, "Mesh\\OBJMesh\\OBJMeshTessellationPS.hlsl", "OBJMeshTessellationPS", EShaderTypes::ST_PIXEL);
	Compile(d3d, "Mesh\\OBJMesh\\OBJMeshTessellationPS.hlsl", "OBJMeshTessellationWireframePS", EShaderTypes::ST_PIXEL);
	
	// ==================================== Compile the Grass Shader
	
	
	// ==================================== Compile the Deferred Shader
	Compile(d3d, "Environment\\Lighting\\DeferredShading\\ClearGBuffer.hlsl", "ClearGBufferVS", EShaderTypes::ST_VERTEX);
	Compile(d3d, "Environment\\Lighting\\DeferredShading\\ClearGBuffer.hlsl", "ClearGBufferPS", EShaderTypes::ST_PIXEL);
	Compile(d3d, "Environment\\Lighting\\DeferredShading\\DeferredShader.hlsl", "DeferredShaderVS", EShaderTypes::ST_VERTEX);
	Compile(d3d, "Environment\\Lighting\\DeferredShading\\DeferredShader.hlsl", "DeferredShaderVS2", EShaderTypes::ST_VERTEX);
	Compile(d3d, "Environment\\Lighting\\DeferredShading\\DeferredShader.hlsl", "DeferredShaderPS", EShaderTypes::ST_PIXEL);
	Compile(d3d, "Environment\\Lighting\\DeferredShading\\DeferredShader.hlsl", "DeferredShaderWireframePS", EShaderTypes::ST_PIXEL);
	Compile(d3d, "Environment\\Lighting\\DeferredShading\\DirectionalLightDeferred.hlsl", "DirectionalLightDeferredVS", EShaderTypes::ST_VERTEX);
	Compile(d3d, "Environment\\Lighting\\DeferredShading\\DirectionalLightDeferred.hlsl", "DirectionalLightDeferredPS", EShaderTypes::ST_PIXEL);
	Compile(d3d, "Environment\\Lighting\\DeferredShading\\DirectionalLightDeferred.hlsl", "DirectionalLightDeferredWireframePS", EShaderTypes::ST_PIXEL);
	Compile(d3d, "Environment\\Lighting\\DeferredShading\\PointLightDeferred.hlsl", "PointLightDeferredVS", EShaderTypes::ST_VERTEX);
	Compile(d3d, "Environment\\Lighting\\DeferredShading\\PointLightDeferred.hlsl", "PointLightDeferredHS", EShaderTypes::ST_HULL);
	Compile(d3d, "Environment\\Lighting\\DeferredShading\\PointLightDeferred.hlsl", "PointLightDeferredDS", EShaderTypes::ST_DOMAIN);
	Compile(d3d, "Environment\\Lighting\\DeferredShading\\PointLightDeferred.hlsl", "PointLightDeferredPS", EShaderTypes::ST_PIXEL);
	Compile(d3d, "Environment\\Lighting\\DeferredShading\\PointLightDeferred.hlsl", "PointLightDeferredWireframePS", EShaderTypes::ST_PIXEL);
	
	// ==================================== Compile the Environment Blur Shader 
	Compile(d3d, "Environment\\Blur.hlsl", "HorizontalBlurVS", EShaderTypes::ST_VERTEX);
	Compile(d3d, "Environment\\Blur.hlsl", "VerticalBlurVS", EShaderTypes::ST_VERTEX);
	Compile(d3d, "Environment\\Blur.hlsl", "HorizontalBlurPS", EShaderTypes::ST_PIXEL);
	Compile(d3d, "Environment\\Blur.hlsl", "VerticalBlurPS", EShaderTypes::ST_PIXEL);

	// ==================================== Compile the Post Processing Shaders
	Compile(d3d, "PostProcess\\PostProcessVS.hlsl", "PostProcessQuadVS", EShaderTypes::ST_VERTEX);
	Compile(d3d, "PostProcess\\PostProcessColorPS.hlsl", "PostProcessColorPS", EShaderTypes::ST_PIXEL);
	Compile(d3d, "PostProcess\\PostProcessHDR.hlsl", "DownScaleFirstPass", EShaderTypes::ST_COMPUTE);
	Compile(d3d, "PostProcess\\PostProcessHDR.hlsl", "DownScaleSecondPass", EShaderTypes::ST_COMPUTE);
	Compile(d3d, "PostProcess\\PostProcessHDR.hlsl", "PostProcessHDR", EShaderTypes::ST_PIXEL);
	
	/*Compile(d3d, "PostProcess\\PostProcessLens.hlsl", "PostProcessLensVS", EShaderTypes::ST_VERTEX);
	Compile(d3d, "PostProcess\\PostProcessing.hlsl", "Threshold", EShaderTypes::ST_PIXEL);
	Compile(d3d, "PostProcess\\PostProcessing.hlsl", "BloomBlurH", EShaderTypes::ST_PIXEL);
	Compile(d3d, "PostProcess\\PostProcessing.hlsl", "BloomBlurV", EShaderTypes::ST_PIXEL);
	Compile(d3d, "PostProcess\\PostProcessing.hlsl", "LuminanceMap", EShaderTypes::ST_PIXEL);
	Compile(d3d, "PostProcess\\PostProcessing.hlsl", "Composite", EShaderTypes::ST_PIXEL);
	Compile(d3d, "PostProcess\\PostProcessing.hlsl", "Scale", EShaderTypes::ST_PIXEL);
	Compile(d3d, "PostProcess\\PostProcessing.hlsl", "AdaptLuminance", EShaderTypes::ST_PIXEL);
	Compile(d3d, "PostProcess\\PostProcessing.hlsl", "DepthBlurGeneration", EShaderTypes::ST_PIXEL);
	Compile(d3d, "PostProcess\\PostProcessing.hlsl", "DOFBlurH", EShaderTypes::ST_PIXEL);
	Compile(d3d, "PostProcess\\PostProcessing.hlsl", "DOFBlurV", EShaderTypes::ST_PIXEL);
	Compile(d3d, "PostProcess\\PostProcessing.hlsl", "BokehComposite", EShaderTypes::ST_PIXEL);
	Compile(d3d, "PostProcess\\PostProcessing.hlsl", "GenerateBokeh", EShaderTypes::ST_PIXEL);
	Compile(d3d, "PostProcess\\PostProcessing.hlsl", "DepthOfFieldGather", EShaderTypes::ST_PIXEL);
	Compile(d3d, "PostProcess\\PostProcessing.hlsl", "DepthOfFieldBlurComposite", EShaderTypes::ST_PIXEL);
	// Depth of Field
	AddMacro("DOF", "MaxSampleRadius_", PostProcessor::MaxSampleRadius);
	AddMacro("DOF", "GridSize_", PostProcessor::GridSize);
	AddMacro("DOF", "ApronSize_", PostProcessor::ApronSize);
	AddMacro("DOF", "TGSize_", PostProcessor::TGSize);
	Compile(d3d, "PostProcess\\DOF.hlsl", "DepthOfFieldH", EShaderTypes::ST_COMPUTE, "DOF");
	Compile(d3d, "PostProcess\\DOF.hlsl", "DepthOfFieldV", EShaderTypes::ST_COMPUTE, "DOF");
	// Circle of Confusion
	AddMacro("CoCSpread", "MaxSampleRadius_", PostProcessor::MaxSampleRadius);
	AddMacro("CoCSpread", "GridSize_", PostProcessor::GridSize);
	AddMacro("CoCSpread", "ApronSize_", PostProcessor::ApronSize);
	AddMacro("CoCSpread", "TGSize_", PostProcessor::TGSize);
	Compile(d3d, "PostProcess\\CoCSpread.hlsl", "CoCSpreadH", EShaderTypes::ST_COMPUTE, "CoCSpread");
	Compile(d3d, "PostProcess\\CoCSpread.hlsl", "CoCSpreadV", EShaderTypes::ST_COMPUTE, "CoCSpread");
	// Bokeh
	Compile(d3d, "PostProcess\\Bokeh.hlsl", "BokehVS", EShaderTypes::ST_VERTEX);
	Compile(d3d, "PostProcess\\Bokeh.hlsl", "BokehGS", EShaderTypes::ST_GEOMETRY);
	Compile(d3d, "PostProcess\\Bokeh.hlsl", "BokehPS", EShaderTypes::ST_PIXEL);*/
}
//=================================================================================================================
void ShaderCompiler::AddMacro(BetterString macroName, const std::string& name, UINT32 value)
{
	ShaderCompiler::ShaderMacros* sm;
	
	if (m_ShaderMacros[macroName] == NULL)
		sm = new ShaderCompiler::ShaderMacros();
	else
		sm = m_ShaderMacros[macroName];

	sm->AddMacro(name, value);

	m_ShaderMacros[macroName] = sm;
}
//=================================================================================================================
HRESULT ShaderCompiler::Compile(D3D* d3d, BetterString filename, BetterString shaderFuncName, int type, BetterString macroName, bool loadPrecompiledShader)
{
	// Compiling Shaders with D3DCompileFromFile
	//Google: visual studio 2013 hlsl shader compiling
	//http://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
	//http://stackoverflow.com/questions/10759300/how-do-i-use-shader-model-5-0-compiled-shaders-in-visual-studio-11
	/*
		Here "PixelShader.cso" is the precompiled hlsl shader generated by Visual Studio 11 from a .hlsl file in the project.
		The compiled .cso file is usually moved to the Projects/ProjectName/Debug folder by default.
		As a result it must be cut and paste into the same directory as your source code before using.
		Mind you this setting can be changed by right-clicking the HLSL file while inside Visual Studio 11 and
		editing the Output Settings. By default its: $(OutDir)%(Filename).cso, change it to: $(Directory)%(Filename).cso
	*/
	
	//Google: creating a cso file in DirectX11.1
	//http://stackoverflow.com/questions/5020204/loading-a-precompiled-hlsl-shader-into-memory-for-use-with-createpixelshader
	
	BetterString base = "..\\Framework\\Shaders\\";
	filename = base + filename;

	unique_ptr<wchar_t> name = filename.ToWideStr();

	HRESULT hr;
	ID3DBlob* blob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	
    UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
    flags |= D3DCOMPILE_DEBUG;
#endif

	ShaderMacros* macros = 0;
	map<string, ShaderMacros*>::iterator it = m_ShaderMacros.find(macroName);
	if (it != m_ShaderMacros.end())
		macros = (*it).second;

	switch (type)
	{
		case EShaderTypes::ST_VERTEX:
		{
			VertexShader* vs = new VertexShader();
			LPCSTR profile = "vs_5_0";
			
			blob = Compile(name.get(), shaderFuncName, profile, flags, errorBlob, macros, loadPrecompiledShader);
			if (blob == nullptr)
			{
				OutputShaderErrorMessage(errorBlob, NULL, shaderFuncName, filename);
				exit(0);
			}
			
			hr = d3d->GetDevice11()->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &vs->mShader);
			if( FAILED(hr) )
			{
				OutputShaderErrorMessage(errorBlob, NULL, shaderFuncName, filename);
				exit(0);
			}
			
			vs->mBlobData = blob;
			vs->mName = shaderFuncName;
			
#if defined(DEBUG) || defined(PROFILE)
			if ( SUCCEEDED(hr) )
			{
				vs->mShader->SetPrivateData( WKPDID_D3DDebugObjectName, lstrlenA(shaderFuncName), shaderFuncName );
			}
#endif
			//Add the vertex shader to the list of shaders
			m_Shaders.push_back(vs);
		}
		break;
		case EShaderTypes::ST_HULL:
		{
			HullShader* hs = new HullShader();
			LPCSTR profile = "hs_5_0";
			
			blob = Compile(name.get(), shaderFuncName, profile, flags, errorBlob, macros, loadPrecompiledShader);
			if (blob == nullptr)
			{
				OutputShaderErrorMessage(errorBlob, NULL, shaderFuncName, filename);
				exit(0);
			}
			
			hr = d3d->GetDevice11()->CreateHullShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &hs->mShader);
			if (FAILED(hr))
			{
				OutputShaderErrorMessage(errorBlob, NULL, shaderFuncName, filename);
				exit(0);
			}
			
			hs->mBlobData = blob;
			hs->mName = shaderFuncName;
			
#if defined(DEBUG) || defined(PROFILE)
			if (SUCCEEDED(hr))
			{
				hs->mShader->SetPrivateData(WKPDID_D3DDebugObjectName, lstrlenA(shaderFuncName), shaderFuncName);
			}
#endif
			
			//Add the hull shader to the list of shaders
			m_Shaders.push_back(hs);
		}
		break;
		case EShaderTypes::ST_DOMAIN:
		{
			DomainShader* ds = new DomainShader();
			LPCSTR profile = "ds_5_0";
			
			blob = Compile(name.get(), shaderFuncName, profile, flags, errorBlob, macros, loadPrecompiledShader);
			if (blob == nullptr)
			{
				OutputShaderErrorMessage(errorBlob, NULL, shaderFuncName, filename);
				exit(0);
			}
			
			hr = d3d->GetDevice11()->CreateDomainShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &ds->mShader);
			if (FAILED(hr))
			{
				OutputShaderErrorMessage(errorBlob, NULL, shaderFuncName, filename);
				exit(0);
			}
			
			ds->mBlobData = blob;
			ds->mName = shaderFuncName;
			
#if defined(DEBUG) || defined(PROFILE)
			if (SUCCEEDED(hr))
			{
				ds->mShader->SetPrivateData(WKPDID_D3DDebugObjectName, lstrlenA(shaderFuncName), shaderFuncName);
			}
#endif
			
			//Add the domain shader to the list of shaders
			m_Shaders.push_back(ds);
		}
		break;
		case EShaderTypes::ST_PIXEL:
		{
			PixelShader* ps = new PixelShader();
			LPCSTR profile = "ps_5_0";
			
			blob = Compile(name.get(), shaderFuncName, profile, flags, errorBlob, macros, loadPrecompiledShader);
			if (blob == nullptr)
			{
				OutputShaderErrorMessage(errorBlob, NULL, shaderFuncName, filename);
				exit(0);
			}

			hr = d3d->GetDevice11()->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &ps->mShader);
			if (FAILED(hr))
			{
				OutputShaderErrorMessage(errorBlob, NULL, shaderFuncName, filename);
				exit(0);
			}

			ps->mBlobData = blob;
			ps->mName = shaderFuncName;

#if defined(DEBUG) || defined(PROFILE)
			if (SUCCEEDED(hr))
			{
				ps->mShader->SetPrivateData(WKPDID_D3DDebugObjectName, lstrlenA(shaderFuncName), shaderFuncName);
			}
#endif
			
			//Add the pixel shader to the list of shaders
			m_Shaders.push_back(ps);
		}
		break;
		case EShaderTypes::ST_GEOMETRY:
		{
			GeometryShader* gs = new GeometryShader();
			LPCSTR profile = "gs_5_0";
			
			blob = Compile(name.get(), shaderFuncName, profile, flags, errorBlob, macros, loadPrecompiledShader);
			if (blob == nullptr)
			{
				OutputShaderErrorMessage(errorBlob, NULL, shaderFuncName, filename);
				exit(0);
			}

			hr = d3d->GetDevice11()->CreateGeometryShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &gs->mShader);
			if (FAILED(hr))
			{
				OutputShaderErrorMessage(errorBlob, NULL, shaderFuncName, filename);
				exit(0);
			}

			gs->mBlobData = blob;
			gs->mName = shaderFuncName;

#if defined(DEBUG) || defined(PROFILE)
			if (SUCCEEDED(hr))
			{
				gs->mShader->SetPrivateData(WKPDID_D3DDebugObjectName, lstrlenA(shaderFuncName), shaderFuncName);
			}
#endif
			
			//Add the geometry shader to the list of shaders
			m_Shaders.push_back(gs);
		}
		break;
		case EShaderTypes::ST_COMPUTE:
		{
			ComputeShader* cs = new ComputeShader();
			LPCSTR profile = "cs_5_0";
			
			blob = Compile(name.get(), shaderFuncName, profile, flags, errorBlob, macros, loadPrecompiledShader);
			if (blob == nullptr)
			{
				OutputShaderErrorMessage(errorBlob, NULL, shaderFuncName, filename);
				exit(0);
			}

			hr = d3d->GetDevice11()->CreateComputeShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, &cs->mShader);
			if (FAILED(hr))
			{
				OutputShaderErrorMessage(errorBlob, NULL, shaderFuncName, filename);
				exit(0);
			}

			cs->mBlobData = blob;
			cs->mName = shaderFuncName;

#if defined(DEBUG) || defined(PROFILE)
			if (SUCCEEDED(hr))
			{
				cs->mShader->SetPrivateData(WKPDID_D3DDebugObjectName, lstrlenA(shaderFuncName), shaderFuncName);
			}
#endif
			
			//Add the compute shader to the list of shaders
			m_Shaders.push_back(cs);
		}
		break;
		default:
		{
			ZShadeMessageCenter::MsgBoxError(NULL, "ShaderCompiler: ZShadeSandboxShader::Shader type not defined !!!");
			exit(0);
		}
		break;
	}
	
	return S_OK;
}
//=================================================================================================================
ID3DBlob* ShaderCompiler::Compile(LPCWSTR name, BetterString shaderFuncName, LPCSTR profile, UINT flags, ID3DBlob*& errorBlob, ShaderMacros* macros, bool loadPrecompiledShader)
{
	HRESULT hr;
	
	ID3DBlob* blob = nullptr;
	
	if (loadPrecompiledShader == false)
	{
		if (macros != 0)
		{
			if (macros->NumDefines() > 0)
			{
				hr = D3DCompileFromFile(name, macros->Defines(), D3D_COMPILE_STANDARD_FILE_INCLUDE,
					shaderFuncName.toString().c_str(), profile,
					flags, 0, &blob, &errorBlob);
			}
			else
			{
				ZShadeMessageCenter::MsgBoxError(NULL, "ShaderCompiler: macro obj defined but no defines available for shader !!!");
			}
		}
		else
		{
			hr = D3DCompileFromFile(name, 0, D3D_COMPILE_STANDARD_FILE_INCLUDE,
				shaderFuncName.toString().c_str(), profile,
				flags, 0, &blob, &errorBlob);
		}

		if (FAILED(hr))
		{
			if (errorBlob)
			{
				OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			}

			if (blob)
				blob->Release();
		}
	}
	else
	{
		D3DReadFileToBlob(name, &blob);
	}
	
	return blob;
}
//=================================================================================================================
void ShaderCompiler::OutputShaderErrorMessage(ID3D10Blob* blob, HWND hwnd, BetterString shaderFuncName, string shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;

	compileErrors = (char*)(blob->GetBufferPointer());

	bufferSize = blob->GetBufferSize();
	
	//BetterString filename = shaderFuncName;
	//filename += "Error.txt";
	
	fout.open("shader-error.txt");

	for (i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	fout.close();

	blob->Release();
	blob = 0;

	char path[MAX_PATH];
	char buffer[MAX_PATH];
	sprintf_s(buffer, "%s", (char*)shaderFilename.c_str());
	MessageBox(hwnd, "Compile Shader Error", buffer, MB_OK);
}
//=================================================================================================================
ZShadeSandboxShader::ShaderBase* ShaderCompiler::GetShaderByName(BetterString name)
{
	vector<ShaderBase*>::iterator it = m_Shaders.begin();
	for (; it != m_Shaders.end(); it++)
	{
		if ((*it)->mName == name)
		{
			return (*it);
		}
	}
	return NULL;
}
//=================================================================================================================