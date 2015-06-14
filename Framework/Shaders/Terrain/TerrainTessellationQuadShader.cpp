#include "TerrainTessellationQuadShader.h"
#include "ConstantBuffer.h"
#include "TerrainParameters.h"
//#include "LightShadingBuffers.h"
//==============================================================================================================================
TerrainTessellationQuadShader::TerrainTessellationQuadShader(D3D* d3d)
:	ZShadeSandboxShader::Shader(d3d)
{
	Initialize();
}
//==============================================================================================================================
TerrainTessellationQuadShader::TerrainTessellationQuadShader(const TerrainTessellationQuadShader& other)
:	ZShadeSandboxShader::Shader(other)
{
}
//==============================================================================================================================
TerrainTessellationQuadShader::~TerrainTessellationQuadShader()
{
}
//==============================================================================================================================
bool TerrainTessellationQuadShader::Initialize()
{
	ConstantBuffer<ZShadeSandboxTerrain::cbTessellationBuffer> tessellationCB(m_pD3DSystem);
	tessellationCB.Initialize(PAD16(sizeof(ZShadeSandboxTerrain::cbTessellationBuffer)));
	m_pTessellationCB = tessellationCB.Buffer();
	
	ConstantBuffer<ZShadeSandboxTerrain::cbDomainConstBuffer> domainCB(m_pD3DSystem);
	domainCB.Initialize(PAD16(sizeof(ZShadeSandboxTerrain::cbDomainConstBuffer)));
	m_pDomainCB = domainCB.Buffer();

	ConstantBuffer<ZShadeSandboxTerrain::cbShadingConstBuffer> shadingCB(m_pD3DSystem);
	shadingCB.Initialize(PAD16(sizeof(ZShadeSandboxTerrain::cbShadingConstBuffer)));
	m_pShadingCB = shadingCB.Buffer();

	ConstantBuffer<ZShadeSandboxLighting::cbLightBuffer> lightCB(m_pD3DSystem);
	lightCB.Initialize(PAD16(sizeof(ZShadeSandboxLighting::cbLightBuffer)));
	m_pLightCB = lightCB.Buffer();
	
	ConstantBuffer<ZShadeSandboxLighting::cbSunLightBuffer> sunCB(m_pD3DSystem);
	sunCB.Initialize(PAD16(sizeof(ZShadeSandboxLighting::cbSunLightBuffer)));
	m_pSunCB = sunCB.Buffer();
	
	ClearInputLayout();
	SetInputLayoutDesc("TerrainTessellationQuadShader", ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_tex, 2);
	
	LoadVertexShader("TerrainTessellationQuadVS");
	LoadHullShader("TerrainTessellationQuadHS");
	LoadDomainShader("TerrainTessellationQuadDS");
	LoadPixelShader("TerrainTessellationQuadPS");
	LoadPixelShader("TerrainTessellationWireframePS");

	AssignVertexShaderLayout("TerrainTessellationQuadShader");

	return true;
}
//==============================================================================================================================
void TerrainTessellationQuadShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
}
//==============================================================================================================================
bool TerrainTessellationQuadShader::Render
(	int indexCount
,	Camera* camera
,	ZShadeSandboxLighting::Light* light
,	ZShadeSandboxTerrain::TerrainShadingConst terrainShadingConst
,	ID3D11ShaderResourceView* heightMapSRV
,	ZShadeSandboxLighting::ShaderMaterial* terrainMaterial
)
{
	/*cTessellationBuffer cTB;
	cDomainConstBuffer cDCB;
	cShadingConstBuffer cSCB;
	
	cTB.g_EyePosW = terrainShadingConst.g_EyePosW;
	cTB.g_MinDist = terrainShadingConst.g_MinDist;
	cTB.g_MaxDist = terrainShadingConst.g_MaxDist;
	cTB.g_MinTess = terrainShadingConst.g_MinTess;
	cTB.g_MaxTess = terrainShadingConst.g_MaxTess;
	cTB.g_FrustumCull = terrainShadingConst.g_FrustumCull;
	cTB.padding = XMFLOAT2(0, 0);
	cTB.g_TerrSize = terrainShadingConst.g_MapSize;
	cTB.g_TerrScale = terrainShadingConst.g_TerrSize;

	cDCB.g_TexScale = XMFLOAT2(66.0f, 66.0f);
	cDCB.g_HeightScale = terrainShadingConst.g_HeightScale;
	cDCB.g_TerrainZScale = terrainShadingConst.g_TerrSize;
	cDCB.g_ClipPlane = terrainShadingConst.g_ClipPlane;
	cDCB.g_tpadding = XMFLOAT3(0, 0, 0);
	cDCB.g_FarPlane = terrainShadingConst.g_FarPlane;
	if (m_UseCustomWorld)
		cDCB.g_WorldMatrix = ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(mWorld.Get()));
	else
		cDCB.g_WorldMatrix = camera->World4x4();

	if (bReflect)
		cDCB.g_ViewMatrix = camera->ReflectionView4x4();
	else
		cDCB.g_ViewMatrix = camera->View4x4();

	cDCB.g_ProjMatrix = camera->Proj4x4();

	XMMATRIX toTexSpace = XMMatrixScaling(0.5f, -0.5f, 1.0f) * XMMatrixTranslation(0.5f, 0.5f, 0);
	cDCB.g_TexSpaceMatrix = ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(toTexSpace));

	// Need to create the shadow matrix
	cDCB.g_ShadowMatrix = lightcamera->ShadowTransform4x4();
	
	cSCB.g_EyePosW = terrainShadingConst.g_EyePosW;
	cSCB.g_DetailBrightness = terrainShadingConst.g_DetailBrightness;
	cSCB.g_fogThinning = terrainShadingConst.g_fogThinning;
	cSCB.g_fogAltitude = terrainShadingConst.g_fogAltitude;
	cSCB.g_fogNear = terrainShadingConst.g_fogNear;
	cSCB.g_fogFar = terrainShadingConst.g_fogFar;
	cSCB.g_fogColor = terrainShadingConst.g_fogColor;
	cSCB.g_TextureAmount = terrainShadingConst.g_TextureAmount;
	cSCB.g_TextureWidth = terrainShadingConst.g_TextureWidth;
	cSCB.g_TextureHeight = terrainShadingConst.g_TextureHeight;
	cSCB.g_seaLevel = terrainShadingConst.g_seaLevel;
	cSCB.g_waterBodyColor = terrainShadingConst.g_waterBodyColor;
	cSCB.g_MaterialDiffuseColor = terrainShadingConst.g_MaterialDiffuseColor;
	cSCB.g_MaterialAmbientColor = terrainShadingConst.g_MaterialAmbientColor;
	cSCB.g_TexelCellSpaceU = terrainShadingConst.g_TexelCellSpaceU;
	cSCB.g_TexelCellSpaceV = terrainShadingConst.g_TexelCellSpaceV;
	cSCB.g_MaterialSpecularPower = terrainShadingConst.g_MaterialSpecularPower;
	cSCB.g_MaterialSpecularIntensity = terrainShadingConst.g_MaterialSpecularIntensity;
	cSCB.g_useClippingPlane = terrainShadingConst.g_useClippingPlane;
	cSCB.g_useNormalMap = terrainShadingConst.g_useNormalMap;
	cSCB.g_UseSobelFilter = terrainShadingConst.g_UseSobelFilter;
	cSCB.g_useShadowMap = terrainShadingConst.g_useShadowMap;
	cSCB.tpadding = XMFLOAT3(0, 0, 0);
	cSCB.g_useSSAO = terrainShadingConst.g_useSSAO;
	cSCB.g_ViewMatrix = cDCB.g_ViewMatrix;
	
	// Map tessellation constants
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res;
		m_pD3DSystem->GetDeviceContext()->Map(m_pTessellationCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
		{
			assert(mapped_res.pData);
			*(cTessellationBuffer*)mapped_res.pData = cTB;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pTessellationCB, 0);
	}
	
	// Map domain shader constants
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res;
		m_pD3DSystem->GetDeviceContext()->Map(m_pDomainCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
		{
			assert(mapped_res.pData);
			*(cDomainConstBuffer*)mapped_res.pData = cDCB;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pDomainCB, 0);
	}
	
	// Map pixel shading constants
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res;
		m_pD3DSystem->GetDeviceContext()->Map(m_pShadingCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
		{
			assert(mapped_res.pData);
			*(cShadingConstBuffer*)mapped_res.pData = cSCB;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pShadingCB, 0);
	}*/

	ID3D11ShaderResourceView* diffuseArrayTexture = 0;
	ID3D11ShaderResourceView* diffuseTexture = 0;
	ID3D11ShaderResourceView* ambientTexture = 0;
	ID3D11ShaderResourceView* specularTexture = 0;
	ID3D11ShaderResourceView* emissiveTexture = 0;
	ID3D11ShaderResourceView* normalMapTexture = 0;
	ID3D11ShaderResourceView* blendMapTexture = 0;
	ID3D11ShaderResourceView* detailMapTexture = 0;
	ID3D11ShaderResourceView* alphaMapTexture = 0;
	ID3D11ShaderResourceView* shadowMapTexture = 0;
	ID3D11ShaderResourceView* ssaoTexture = 0;

	terrainMaterial->GetTextures(
		diffuseArrayTexture,
		diffuseTexture,
		ambientTexture,
		specularTexture,
		emissiveTexture,
		normalMapTexture,
		blendMapTexture,
		detailMapTexture,
		alphaMapTexture,
		shadowMapTexture,
		ssaoTexture
	);

	ZShadeSandboxTerrain::BuildTerrainTessellationConstantBuffer(m_pD3DSystem, m_pTessellationCB, terrainShadingConst);
	ZShadeSandboxTerrain::BuildTerrainDomainConstantBuffer(m_pD3DSystem, m_pDomainCB, camera->World(), camera, light, terrainShadingConst);

	XMFLOAT4X4 view;
	view = (terrainShadingConst.g_useReflection) ? camera->ReflectionView4x4() : camera->View4x4();

	ZShadeSandboxTerrain::BuildTerrainShadingConstantBuffer(m_pD3DSystem, m_pShadingCB, view, terrainShadingConst);

	ZShadeSandboxLighting::LightManager::Instance()->BuildFinalLightBuffers(m_pLightCB, m_pSunCB);

	// Set the tessellation constant buffer into the Hull Shader
	ID3D11Buffer* hs_cbs[1] = { m_pTessellationCB };
	m_pD3DSystem->GetDeviceContext()->HSSetConstantBuffers(2, 1, hs_cbs);
	
	// Set the domain constant buffer into the Domain Shader
	// Set the matrix constant buffer into the Domain Shader
	ID3D11Buffer* ds_cbs[1] = { m_pDomainCB };
	m_pD3DSystem->GetDeviceContext()->DSSetConstantBuffers(3, 1, ds_cbs);

	// Set the shading constant buffer into the Pixel Shader
	ID3D11Buffer* ps_cbs[5] = { m_pLightCB, m_pSunCB, m_pTessellationCB, m_pDomainCB, m_pShadingCB };
	m_pD3DSystem->GetDeviceContext()->PSSetConstantBuffers(0, 5, ps_cbs);

	// Set the heightmap texture into the vertex shader and domain shader
	ID3D11ShaderResourceView* vsds_srvs[1] = { heightMapSRV };
	
	// Set the textures into the Pixel Shader
	ID3D11ShaderResourceView* ps_srvs[7] = {
		diffuseArrayTexture, blendMapTexture, normalMapTexture, heightMapSRV, detailMapTexture, shadowMapTexture, ssaoTexture
	};
	ID3D11SamplerState* ps_samp[3] = { m_pD3DSystem->Point(), m_pD3DSystem->Linear(), m_pD3DSystem->ShadowMapPCF() };
	
	// Tell the shader what input layout to use
	SetInputLayout("TerrainTessellationQuadShader");

	if (!m_Wireframe)
	{
		m_pD3DSystem->GetDeviceContext()->VSSetSamplers(0, 3, ps_samp);
		m_pD3DSystem->GetDeviceContext()->DSSetSamplers(0, 3, ps_samp);
		m_pD3DSystem->GetDeviceContext()->PSSetSamplers(0, 3, ps_samp);
		
		m_pD3DSystem->GetDeviceContext()->VSSetShaderResources(3, 1, vsds_srvs);
		m_pD3DSystem->GetDeviceContext()->DSSetShaderResources(3, 1, vsds_srvs);
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 7, ps_srvs);

		SwitchTo("TerrainTessellationQuadPS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}
	else
	{
		SwitchTo("TerrainTessellationWireframePS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
		m_pD3DSystem->TurnOnWireframe();
	}

	// Assign the shaders to render the mesh
	SetVertexShader();
	SetHullShader();
	SetDomainShader();
	SetPixelShader();
	
	//Perform Drawing
	RenderIndex11(indexCount);
	
	// Unbind
	if (!m_Wireframe)
	{
		ps_samp[0] = NULL;
		ps_samp[1] = NULL;
		ps_samp[2] = NULL;
		m_pD3DSystem->GetDeviceContext()->VSSetSamplers(0, 3, ps_samp);
		m_pD3DSystem->GetDeviceContext()->DSSetSamplers(0, 3, ps_samp);
		m_pD3DSystem->GetDeviceContext()->PSSetSamplers(0, 3, ps_samp);

		vsds_srvs[0] = NULL;
		m_pD3DSystem->GetDeviceContext()->VSSetShaderResources(0, 1, vsds_srvs);
		m_pD3DSystem->GetDeviceContext()->DSSetShaderResources(0, 1, vsds_srvs);
		
		for (int i = 0; i < 7; i++) ps_srvs[i] = NULL;
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 7, ps_srvs);
	}
	
	return true;
}
//==============================================================================================================================