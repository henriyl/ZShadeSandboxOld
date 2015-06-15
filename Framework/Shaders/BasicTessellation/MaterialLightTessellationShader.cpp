#include "MaterialLightTessellationShader.h"
#include "ConstantBuffer.h"
#include "CGlobal.h"
//==============================================================================================================================
MaterialLightTessellationShader::MaterialLightTessellationShader(D3D* d3d, ZShadeSandboxLighting::EMaterialTessellationType::Type type)
:	ZShadeSandboxShader::Shader(d3d)
,	mType(type)
{
	Initialize();
}
//==============================================================================================================================
MaterialLightTessellationShader::MaterialLightTessellationShader(const MaterialLightTessellationShader& other)
:	ZShadeSandboxShader::Shader(other)
{
}
//==============================================================================================================================
MaterialLightTessellationShader::~MaterialLightTessellationShader()
{
}
//==============================================================================================================================
bool MaterialLightTessellationShader::Initialize()
{
	ConstantBuffer<ZShadeSandboxLighting::cbAmbientLightBuffer> ambientCB(m_pD3DSystem);
	ambientCB.Initialize(PAD16(sizeof(ZShadeSandboxLighting::cbAmbientLightBuffer)));
	m_pAmbientLightCB = ambientCB.Buffer();

	ConstantBuffer<ZShadeSandboxLighting::cbDirectionalLightBuffer> dirCB(m_pD3DSystem);
	dirCB.Initialize(PAD16(sizeof(ZShadeSandboxLighting::cbDirectionalLightBuffer)));
	m_pDirectionalLightCB = dirCB.Buffer();

	ConstantBuffer<ZShadeSandboxLighting::cbPointLightBuffer> pointCB(m_pD3DSystem);
	pointCB.Initialize(PAD16(sizeof(ZShadeSandboxLighting::cbPointLightBuffer)));
	m_pPointLightCB = pointCB.Buffer();

	ConstantBuffer<ZShadeSandboxLighting::cbSpotLightBuffer> spotCB(m_pD3DSystem);
	spotCB.Initialize(PAD16(sizeof(ZShadeSandboxLighting::cbSpotLightBuffer)));
	m_pSpotLightCB = spotCB.Buffer();

	ConstantBuffer<ZShadeSandboxLighting::cbCapsuleLightBuffer> capsuleCB(m_pD3DSystem);
	capsuleCB.Initialize(PAD16(sizeof(ZShadeSandboxLighting::cbCapsuleLightBuffer)));
	m_pCapsuleLightCB = capsuleCB.Buffer();

	ConstantBuffer<cbLightTypeBuffer> lightTypeCB(m_pD3DSystem);
	lightTypeCB.Initialize(PAD16(sizeof(cbLightTypeBuffer)));
	m_pLightTypeCB = lightTypeCB.Buffer();

	ConstantBuffer<ZShadeSandboxLighting::cbMaterialShadingBuffer> shadingCB(m_pD3DSystem);
	shadingCB.Initialize(PAD16(sizeof(ZShadeSandboxLighting::cbMaterialShadingBuffer)));
	m_pShadingCB = shadingCB.Buffer();

	ConstantBuffer<cbMatrixBuffer> matrixCB(m_pD3DSystem);
	matrixCB.Initialize(PAD16(sizeof(cbMatrixBuffer)));
	m_pMatrixCB = matrixCB.Buffer();
	
	ClearInputLayout();
	SetInputLayoutDesc("MaterialLightTessellationShader", ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_normal_tex, 3);
	
	switch (mType)
	{
		case ZShadeSandboxLighting::EMaterialTessellationType::Type::eQuad:
		{
			LoadVertexShader("QuadMaterialLightTessellationVS");
			LoadHullShader("QuadMaterialLightTessellationHS");
			LoadDomainShader("QuadMaterialLightTessellationDS");
			LoadPixelShader("QuadMaterialLightTessellationPS");
			LoadPixelShader("QuadMaterialLightTessellationWireframePS");
		}
		break;
		case ZShadeSandboxLighting::EMaterialTessellationType::Type::eTri:
		{
			LoadVertexShader("TriMaterialLightTessellationVS");
			LoadHullShader("TriMaterialLightTessellationHS");
			LoadDomainShader("TriMaterialLightTessellationDS");
			LoadPixelShader("TriMaterialLightTessellationPS");
			LoadPixelShader("TriMaterialLightTessellationWireframePS");
		}
		break;
	}

	AssignVertexShaderLayout("MaterialLightTessellationShader");
	
	return true;
}
//==============================================================================================================================
void MaterialLightTessellationShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
}
//==============================================================================================================================
bool MaterialLightTessellationShader::Render11
(	int indexCount
,	ZShadeSandboxMesh::MeshRenderParameters mrp
,	ZShadeSandboxLighting::ShaderMaterial* material
)
{
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
	ID3D11ShaderResourceView* displacementMapTexture = 0;

	material->GetTextures(
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
		ssaoTexture,
		displacementMapTexture
	);
	
	material->BuildMaterialTessellationBuffer(m_pTessellationCB, mrp.camera->Position());
	material->BuildMaterialDomainBuffer(m_pDomainCB, mrp.world, mrp.camera, mrp.clipplane, mrp.reflection);
	material->BuildMaterialConstantBuffer(m_pShadingCB, mrp.camera->Position(), mrp.clipplane);

	ZShadeSandboxLighting::LightManager::Instance()->BuildAmbientLightBuffer(m_pAmbientLightCB, mrp.light);
	ZShadeSandboxLighting::LightManager::Instance()->BuildDirectionalLightBuffer(m_pDirectionalLightCB, mrp.light);
	ZShadeSandboxLighting::LightManager::Instance()->BuildPointLightBuffer(m_pPointLightCB, mrp.light);
	ZShadeSandboxLighting::LightManager::Instance()->BuildSpotLightBuffer(m_pSpotLightCB, mrp.light);
	ZShadeSandboxLighting::LightManager::Instance()->BuildCapsuleLightBuffer(m_pCapsuleLightCB, mrp.light);

	cbLightTypeBuffer cLTB;
	cLTB.g_UsingAmbientLight = (mrp.light->LightType() == ZShadeSandboxLighting::ELightType::eAmbient) ? 1 : 0;
	cLTB.g_UsingDirectionalLight = (mrp.light->LightType() == ZShadeSandboxLighting::ELightType::eDirectional) ? 1 : 0;
	cLTB.g_UsingPointLight = (mrp.light->LightType() == ZShadeSandboxLighting::ELightType::ePoint) ? 1 : 0;
	cLTB.g_UsingSpotLight = (mrp.light->LightType() == ZShadeSandboxLighting::ELightType::eSpot) ? 1 : 0;
	cLTB.lighttypepadding = XMFLOAT3(0, 0, 0);
	cLTB.g_UsingCapsuleLight = (mrp.light->LightType() == ZShadeSandboxLighting::ELightType::eCapsule) ? 1 : 0;
	// Map the light type constants
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res2;
		m_pD3DSystem->GetDeviceContext()->Map(m_pLightTypeCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res2);
		{
			assert(mapped_res2.pData);
			*(cbLightTypeBuffer*)mapped_res2.pData = cLTB;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pLightTypeCB, 0);
	}

	mrp.camera->BuildCameraConstantBuffer(m_pD3DSystem, m_pMatrixCB, mrp.light, mrp.world, mrp.reflection);
	
	// Set the tessellation constant buffer into the Hull Shader
	ID3D11Buffer* hs_cbs[1] = { m_pTessellationCB };
	m_pD3DSystem->GetDeviceContext()->HSSetConstantBuffers(2, 1, hs_cbs);
	
	// Set the matrix constant buffer into the Domain Shader
	ID3D11Buffer* ds_cbs[1] = { m_pDomainCB };
	m_pD3DSystem->GetDeviceContext()->DSSetConstantBuffers(3, 1, ds_cbs);
	
	ID3D11Buffer* ps_cbs[7] = { m_pShadingCB, m_pLightTypeCB, m_pAmbientLightCB, m_pDirectionalLightCB, m_pPointLightCB, m_pSpotLightCB, m_pCapsuleLightCB };
	m_pD3DSystem->GetDeviceContext()->PSSetConstantBuffers(4, 7, ps_cbs);

	ID3D11ShaderResourceView* ps_srvs[11] = { diffuseArrayTexture, diffuseTexture, ambientTexture, specularTexture, emissiveTexture, normalMapTexture, blendMapTexture, detailMapTexture, alphaMapTexture, shadowMapTexture, ssaoTexture };
	ID3D11SamplerState* ps_samp[1] = { m_pD3DSystem->Linear() };

	if (!m_Wireframe)
	{
		// Assign Texture
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 11, ps_srvs);
		m_pD3DSystem->GetDeviceContext()->PSSetSamplers(0, 1, ps_samp);

		switch (mType)
		{
			case ZShadeSandboxLighting::EMaterialTessellationType::Type::eQuad: SwitchTo("QuadMaterialLightTessellationPS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL); break;
			case ZShadeSandboxLighting::EMaterialTessellationType::Type::eTri: SwitchTo("TriMaterialLightTessellationPS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL); break;
		}
	}
	else
	{
		switch (mType)
		{
			case ZShadeSandboxLighting::EMaterialTessellationType::Type::eQuad: SwitchTo("QuadMaterialLightTessellationWireframePS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL); break;
			case ZShadeSandboxLighting::EMaterialTessellationType::Type::eTri: SwitchTo("TriMaterialLightTessellationWireframePS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL); break;
		}
	}
	
	SetVertexShader();
	SetHullShader();
	SetDomainShader();
	SetPixelShader();

	SetInputLayout("MaterialLightTessellationShader");

	//Perform Drawing
	RenderIndex11(indexCount);

	// Unbind
	if (!m_Wireframe)
	{
		ps_samp[0] = NULL;
		m_pD3DSystem->GetDeviceContext()->PSSetSamplers(0, 1, ps_samp);

		for (int i = 0; i < 11; i++) ps_srvs[i] = NULL;
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 11, ps_srvs);
	}
	
	// Set Hull, Domain and Geometry Shaders to null in case they are not needed
	m_pD3DSystem->GetDeviceContext()->HSSetShader(NULL, NULL, 0);
	m_pD3DSystem->GetDeviceContext()->DSSetShader(NULL, NULL, 0);
	m_pD3DSystem->GetDeviceContext()->GSSetShader(NULL, NULL, 0);

	return true;
}
//==============================================================================================================================