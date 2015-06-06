#include "TerrainTessellationQuadSSAOShader.h"
#include "ConstantBuffer.h"
//#include "LightShadingBuffers.h"
//==============================================================================================================================
TerrainTessellationQuadSSAOShader::TerrainTessellationQuadSSAOShader(D3D* d3d)
	: ZShadeSandboxShader::Shader(d3d)
{
	Initialize();
}
//==============================================================================================================================
TerrainTessellationQuadSSAOShader::TerrainTessellationQuadSSAOShader(const TerrainTessellationQuadSSAOShader& other)
	: ZShadeSandboxShader::Shader(other)
{
}
//==============================================================================================================================
TerrainTessellationQuadSSAOShader::~TerrainTessellationQuadSSAOShader()
{
}
//==============================================================================================================================
bool TerrainTessellationQuadSSAOShader::Initialize()
{
	ConstantBuffer<cTessellationBuffer> tessellationCB(m_pD3DSystem);
	tessellationCB.Initialize(PAD16(sizeof(cTessellationBuffer)));
	m_pTessellationCB = tessellationCB.Buffer();

	ConstantBuffer<cDomainConstBuffer> domainCB(m_pD3DSystem);
	domainCB.Initialize(PAD16(sizeof(cDomainConstBuffer)));
	m_pDomainCB = domainCB.Buffer();

	ConstantBuffer<cMatrixBuffer> matrixBufferCB(m_pD3DSystem);
	matrixBufferCB.Initialize(PAD16(sizeof(cMatrixBuffer)));
	m_pMatrixBufferCB = matrixBufferCB.Buffer();

	ConstantBuffer<cShadingConstBuffer> shadingCB(m_pD3DSystem);
	shadingCB.Initialize(PAD16(sizeof(cShadingConstBuffer)));
	m_pShadingCB = shadingCB.Buffer();

	ConstantBuffer<ZShadeSandboxLighting::cbLightBuffer> lightCB(m_pD3DSystem);
	lightCB.Initialize(PAD16(sizeof(ZShadeSandboxLighting::cbLightBuffer)));
	m_pLightCB = lightCB.Buffer();

	ConstantBuffer<ZShadeSandboxLighting::cbSunLightBuffer> sunCB(m_pD3DSystem);
	sunCB.Initialize(PAD16(sizeof(ZShadeSandboxLighting::cbSunLightBuffer)));
	m_pSunCB = sunCB.Buffer();

	ClearInputLayout();
	SetInputLayoutDesc("TerrainTessellationQuadSSAOShader", ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_tex, 2);

	LoadVertexShader("TerrainTessellationQuadVS");
	LoadHullShader("TerrainTessellationQuadShadowHS");
	LoadDomainShader("TerrainTessellationQuadDS");
	LoadPixelShader("TerrainTessellationNormalDepthPS");

	AssignVertexShaderLayout("TerrainTessellationQuadSSAOShader");

	return true;
}
//==============================================================================================================================
void TerrainTessellationQuadSSAOShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
}
//==============================================================================================================================
bool TerrainTessellationQuadSSAOShader::Render(int indexCount, Camera* camera, LightCamera* lightcamera, ZShadeSandboxTerrain::TerrainShadingConst terrainShadingConst, ID3D11ShaderResourceView* heightMapSRV)
{
	cTessellationBuffer cTB;
	cMatrixBuffer cMB;
	cDomainConstBuffer cDCB;
	cShadingConstBuffer cSCB;
	ZShadeSandboxLighting::cbLightBuffer cLB;
	ZShadeSandboxLighting::cbSunLightBuffer cSLB;

	if (m_UseCustomWorld)
		cMB.g_WorldMatrix = ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(mWorld.Get()));
	else
		cMB.g_WorldMatrix = camera->World4x4();

	cMB.g_ViewMatrix = camera->View4x4();

	cMB.g_ProjMatrix = camera->Proj4x4();

	XMMATRIX toTexSpace = XMMatrixScaling(0.5f, -0.5f, 1.0f) * XMMatrixTranslation(0.5f, 0.5f, 0);
	cMB.g_TexSpaceMatrix = ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(toTexSpace));

	// Need to create the shadow matrix
	cMB.g_ShadowMatrix = lightcamera->ShadowTransform4x4();

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
	//cSCB.g_SunDir = terrainShadingConst.g_SunDir;
	//cSCB.g_EnableSun = terrainShadingConst.g_EnableSun;
	//cSCB.g_SunDiffuseColor = terrainShadingConst.g_SunDiffuseColor;
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
	cSCB.g_ViewMatrix = cMB.g_ViewMatrix;

	ZShadeSandboxLighting::cbAmbientLightBuffer alb;
	ZShadeSandboxLighting::cbDirectionalLightBuffer dlb;
	ZShadeSandboxLighting::cbSpotLightBuffer slb;
	ZShadeSandboxLighting::cbPointLightBuffer plb;
	ZShadeSandboxLighting::cbCapsuleLightBuffer clb;
	for (int i = 0; i < terrainShadingConst.g_AmbientLightCount; i++)
	{
		alb.g_AmbientColor = terrainShadingConst.g_AmbientLight[i].g_AmbientColor;
		cLB.g_AmbientLight[i] = alb;
	}
	for (int i = 0; i < terrainShadingConst.g_DirectionalLightCount; i++)
	{
		dlb.g_Direction = terrainShadingConst.g_DirectionalLight[i].g_LightDirection;
		dlb.padding = 0;
		dlb.g_Ambient = terrainShadingConst.g_DirectionalLight[i].g_AmbientColor;
		dlb.g_Diffuse = terrainShadingConst.g_DirectionalLight[i].g_DiffuseColor;
		cLB.g_DirectionalLight[i] = dlb;
	}
	for (int i = 0; i < terrainShadingConst.g_SpotLightCount; i++)
	{
		slb.g_AmbientColor = terrainShadingConst.g_SpotLight[i].g_AmbientColor;
		slb.g_DiffuseColor = terrainShadingConst.g_SpotLight[i].g_DiffuseColor;
		slb.g_LightPosition = terrainShadingConst.g_SpotLight[i].g_LightPosition;
		slb.padding = 0;
		slb.g_LightRange = terrainShadingConst.g_SpotLight[i].g_LightRange;
		slb.g_SpotCosOuterCone = terrainShadingConst.g_SpotLight[i].g_SpotCosOuterCone;
		slb.g_SpotInnerConeReciprocal = terrainShadingConst.g_SpotLight[i].g_SpotInnerConeReciprocal;
		slb.g_CosineAngle = terrainShadingConst.g_SpotLight[i].g_CosineAngle;
		cLB.g_SpotLight[i] = slb;
	}
	for (int i = 0; i < terrainShadingConst.g_PointLightCount; i++)
	{
		plb.g_LightPosition = terrainShadingConst.g_PointLight[i].g_LightPosition;
		plb.g_LightRange = terrainShadingConst.g_PointLight[i].g_LightRange;
		plb.g_Attenuation = terrainShadingConst.g_PointLight[i].g_Attenuation;
		plb.padding = 0;
		plb.g_AmbientColor = terrainShadingConst.g_PointLight[i].g_AmbientColor;
		plb.g_DiffuseColor = terrainShadingConst.g_PointLight[i].g_DiffuseColor;
		cLB.g_PointLight[i] = plb;
	}
	for (int i = 0; i < terrainShadingConst.g_CapsuleLightCount; i++)
	{
		clb.g_LightPosition = terrainShadingConst.g_CapsuleLight[i].g_LightPosition;
		clb.g_LightRange = terrainShadingConst.g_CapsuleLight[i].g_LightRange;
		clb.g_LightDirection = terrainShadingConst.g_CapsuleLight[i].g_LightDirection;
		//clb.g_LightRangeReciprocal = terrainShadingConst.g_CapsuleLight[i].g_LightRangeReciprocal;
		//clb.g_LightColor = terrainShadingConst.g_CapsuleLight[i].g_LightColor;
		clb.g_LightLength = terrainShadingConst.g_CapsuleLight[i].g_LightLength;
		clb.g_CapsuleDirectionLength = terrainShadingConst.g_CapsuleLight[i].g_CapsuleDirectionLength;
		clb.g_CapsuleIntensity = terrainShadingConst.g_CapsuleLight[i].g_CapsuleIntensity;
		clb.g_AmbientColor = terrainShadingConst.g_CapsuleLight[i].g_AmbientColor;
		clb.g_DiffuseColor = terrainShadingConst.g_CapsuleLight[i].g_DiffuseColor;
		cLB.g_CapsuleLight[i] = clb;
	}
	cLB.g_AmbientLightCount = terrainShadingConst.g_AmbientLightCount;
	cLB.g_DirectionalLightCount = terrainShadingConst.g_DirectionalLightCount;
	cLB.g_SpotLightCount = terrainShadingConst.g_SpotLightCount;
	cLB.g_PointLightCount = terrainShadingConst.g_PointLightCount;
	cLB.g_CapsuleLightCount = terrainShadingConst.g_CapsuleLightCount;
	cLB.g_AmbientDown = terrainShadingConst.g_AmbientDown;
	cLB.g_AmbientUp = terrainShadingConst.g_AmbientUp;
	cLB.padding = 0;

	cSLB.g_SunDir = terrainShadingConst.g_SunDir;
	cSLB.g_EnableSun = terrainShadingConst.g_EnableSun;
	cSLB.g_SunDiffuseColor = terrainShadingConst.g_SunDiffuseColor;
	cSLB.padding = XMFLOAT3(0, 0, 0);
	cSLB.g_SunShineness = terrainShadingConst.g_SunShineness;

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

	// Map matrix constants
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res;
		m_pD3DSystem->GetDeviceContext()->Map(m_pMatrixBufferCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
		{
			assert(mapped_res.pData);
			*(cMatrixBuffer*)mapped_res.pData = cMB;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pMatrixBufferCB, 0);
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
	}

	// Map light shading constants
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res;
		m_pD3DSystem->GetDeviceContext()->Map(m_pLightCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
		{
			assert(mapped_res.pData);
			*(ZShadeSandboxLighting::cbLightBuffer*)mapped_res.pData = cLB;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pLightCB, 0);
	}

	// Map sun light shading constants
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res;
		m_pD3DSystem->GetDeviceContext()->Map(m_pSunCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
		{
			assert(mapped_res.pData);
			*(ZShadeSandboxLighting::cbSunLightBuffer*)mapped_res.pData = cSLB;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pSunCB, 0);
	}

	// Set the tessellation constant buffer into the Hull ZShadeSandboxShader::Shader
	ID3D11Buffer* hs_cbs[1] = { m_pTessellationCB };
	m_pD3DSystem->GetDeviceContext()->HSSetConstantBuffers(0, 1, hs_cbs);

	// Set the domain constant buffer into the Domain ZShadeSandboxShader::Shader
	// Set the matrix constant buffer into the Domain ZShadeSandboxShader::Shader
	ID3D11Buffer* ds_cbs[2] = { m_pDomainCB, m_pMatrixBufferCB };
	m_pD3DSystem->GetDeviceContext()->DSSetConstantBuffers(0, 2, ds_cbs);

	// Set the shading constant buffer into the Pixel ZShadeSandboxShader::Shader
	ID3D11Buffer* ps_cbs[3] = { m_pShadingCB, m_pLightCB, m_pSunCB };
	m_pD3DSystem->GetDeviceContext()->PSSetConstantBuffers(0, 3, ps_cbs);

	// Set the heightmap texture into the vertex shader
	ID3D11ShaderResourceView* vs_srvs[1] = { heightMapSRV };
	m_pD3DSystem->GetDeviceContext()->VSSetShaderResources(0, 1, vs_srvs);

	// Set the heightmap texture into the domain shader
	ID3D11ShaderResourceView* ds_srvs[1] = { heightMapSRV };
	m_pD3DSystem->GetDeviceContext()->DSSetShaderResources(0, 1, ds_srvs);

	// Set the textures into the Pixel Shader
	ID3D11ShaderResourceView* ps_srvs[1] = { heightMapSRV };
	m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 1, ps_srvs);

	// Tell the shader what input layout to use
	SetInputLayout("TerrainTessellationQuadSSAOShader");

	// Assign the shaders to render the mesh
	SetVertexShader();
	SetHullShader();
	SetDomainShader();
	SetPixelShader();

	//Perform Drawing
	RenderIndex11(indexCount);

	return true;
}
//==============================================================================================================================