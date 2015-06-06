#include "MaterialShader.h"
#include "ConstantBuffer.h"
#include "CGlobal.h"
//==============================================================================================================================
MaterialShader::MaterialShader(D3D* d3d)
:	ZShadeSandboxShader::Shader(d3d)
{
	Initialize();
}
//==============================================================================================================================
MaterialShader::MaterialShader(const MaterialShader& other)
:	ZShadeSandboxShader::Shader(other)
{
}
//==============================================================================================================================
MaterialShader::~MaterialShader()
{
}
//==============================================================================================================================
bool MaterialShader::Initialize()
{
	ConstantBuffer<Const_Per_Frame> perFrameCB(m_pD3DSystem);
	perFrameCB.Initialize(PAD16(sizeof(Const_Per_Frame)));
	m_pPerFrameCB = perFrameCB.Buffer();
	
	ConstantBuffer<Const_Per_Object> perObjectCB(m_pD3DSystem);
	perObjectCB.Initialize(PAD16(sizeof(Const_Per_Object)));
	m_pPerObjectCB = perObjectCB.Buffer();
	
	ConstantBuffer<ZShadeSandboxLighting::cbLightBuffer> lightCB(m_pD3DSystem);
	lightCB.Initialize(PAD16(sizeof(ZShadeSandboxLighting::cbLightBuffer)));
	m_pLightCB = lightCB.Buffer();
	
	ConstantBuffer<ZShadeSandboxLighting::cbSunLightBuffer> sunCB(m_pD3DSystem);
	sunCB.Initialize(PAD16(sizeof(ZShadeSandboxLighting::cbSunLightBuffer)));
	m_pSunCB = sunCB.Buffer();
	
	ClearInputLayout();
	
	SetInputLayoutDesc("MaterialShader", ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_normal_tex, 3);
	//SetInputLayoutDesc("MaterialShader", ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_tex, 2);
	
	LoadVertexShader("MaterialShaderVS");
	LoadPixelShader("MaterialShaderPS");
	LoadPixelShader("MaterialShaderWireframePS");
	
	AssignVertexShaderLayout("MaterialShader");
	
	return true;
}
//==============================================================================================================================
void MaterialShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
}
//==============================================================================================================================
void MaterialShader::FlipTextureHorizontally(bool flip)
{
	bFlipHorizontally = flip;
	bFlipVertically = !flip;
}
//==============================================================================================================================
bool MaterialShader::Render11(int indexCount, ZShadeSandboxMath::XMMath4 clipplane, Camera* camera, LightCamera* lightcamera, ZShadeSandboxLighting::ShaderMaterial* material)
{
	int useDiffuseArrayTexture = 0;
	int useDiffuseTexture = 0;
	int useNormalMapTexture = 0;
	int useBlendMapTexture = 0;
	int useDetailMapTexture = 0;
	int useAlphaMapTexture = 0;
	
	ID3D11ShaderResourceView* diffuseArrayTexture = 0;
	ID3D11ShaderResourceView* diffuseTexture = 0;
	ID3D11ShaderResourceView* normalMapTexture = 0;
	ID3D11ShaderResourceView* blendMapTexture = 0;
	ID3D11ShaderResourceView* detailMapTexture = 0;
	ID3D11ShaderResourceView* alphaMapTexture = 0;

	ID3D11ShaderResourceView* shadowMapTexture = material->ShadowMapTexture();
	ID3D11ShaderResourceView* ssaoTexture = material->SSAOTexture();
	
	for (int i = 0; i < material->TextureCount(); i++)
	{
		switch (material->GetMaterialTextureType(i))
		{
			case ZShadeSandboxLighting::EMaterialTextureType::eDiffuseArray:
				useDiffuseArrayTexture = 1;
				diffuseArrayTexture = material->GetTexture(i)->getTexture11();
			break;
			case ZShadeSandboxLighting::EMaterialTextureType::eDiffuse:
				useDiffuseTexture = 1;
				diffuseTexture = material->GetTexture(i)->getTexture11();
			break;
			case ZShadeSandboxLighting::EMaterialTextureType::eNormal:
				useNormalMapTexture = 1;
				normalMapTexture = material->GetTexture(i)->getTexture11();
			break;
			case ZShadeSandboxLighting::EMaterialTextureType::eBlend:
				useBlendMapTexture = 1;
				blendMapTexture = material->GetTexture(i)->getTexture11();
			break;
			case ZShadeSandboxLighting::EMaterialTextureType::eDetail:
				useDetailMapTexture = 1;
				detailMapTexture = material->GetTexture(i)->getTexture11();
			break;
			case ZShadeSandboxLighting::EMaterialTextureType::eAlpha:
				useAlphaMapTexture = 1;
				alphaMapTexture = material->GetTexture(i)->getTexture11();
			break;
		}
	}

	//if (material->Name() == "Metal")
	//{
	//	if (shadowMapTexture != 0)
	//		diffuseTexture = shadowMapTexture;
	//}

	Const_Per_Frame per_frame;
	per_frame.g_EyePosW = camera->Position();
	per_frame.g_ClipPlane = XMFLOAT4(clipplane.x, clipplane.y, clipplane.z, clipplane.w);
	per_frame.g_MaterialDiffuseColor = material->DiffuseColor();
	per_frame.g_MaterialAmbientColor = material->AmbientColor();
	per_frame.g_MaterialSpecularPower = material->SpecularPower();
	per_frame.g_MaterialSpecularIntensity = material->SpecularIntensity();
	per_frame.g_UsingDiffuseArrayTexture = useDiffuseArrayTexture;
	per_frame.g_UsingDiffuseTexture = useDiffuseTexture;
	per_frame.g_UsingNormalMapTexture = useNormalMapTexture;
	per_frame.g_UsingBlendMapTexture = useBlendMapTexture;
	per_frame.g_UsingDetailMapTexture = useDetailMapTexture;
	per_frame.g_UseAlphaMapTexture = useAlphaMapTexture;
	per_frame.g_EnableTransparency = material->EnableTransparency();
	per_frame.g_EnableLighting = material->EnableLighting();
	per_frame.g_DetailBrightness = material->DetailBrightness();
	per_frame.g_AlphaToCoverageValue = material->AlphaToCoverageValue(); // Value that clips pixel during alpha blending
	per_frame.g_FlipTextureH = (bFlipHorizontally == true) ? 1 : 0;
	per_frame.g_FlipTextureV = (bFlipVertically == true) ? 1 : 0;
	per_frame.g_padding1 = 0;
	per_frame.g_padding2 = XMFLOAT2(0, 0);
	per_frame.g_UseShadowMap = (material->EnableShadowMap() == true) ? 1 : 0;
	per_frame.g_UseSSAOMap = (material->EnableSSAOMap() == true) ? 1 : 0;
	
	Const_Per_Object per_object;
	
	if (m_UseCustomWorld)
	{
		XMFLOAT4X4 world = ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(mWorld.Get()));
		per_object.g_matWorld = world;
	}
	else
		per_object.g_matWorld = camera->World4x4();

	if (m_UseCustomView)
	{
		per_object.g_matView = mView;
	}
	else
		per_object.g_matView = camera->View4x4();

	if (m_pD3DSystem->GetEngineOptions()->m_DimType == DimType::ZSHADE_2D)
		per_object.g_matProj = camera->Ortho4x4();
	else if (m_pD3DSystem->GetEngineOptions()->m_DimType == DimType::ZSHADE_3D)
		per_object.g_matProj = camera->Proj4x4();

	if (m_UseOrtho)
	{
		per_object.g_matProj = camera->Ortho4x4();
	}
	
	if (lightcamera == NULL)
	{
		per_object.g_LightView = ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(XMMatrixIdentity()));
		per_object.g_LightProj = ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(XMMatrixIdentity()));
		per_object.g_ShadowMatrix = ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(XMMatrixIdentity()));
	}
	else
	{
		per_object.g_LightView = lightcamera->LightView4x4();
		per_object.g_LightProj = lightcamera->LightProj4x4();
		XMMATRIX shadowTransform = mWorld.Get() * lightcamera->ShadowTransform();
		per_object.g_ShadowMatrix = ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(shadowTransform));//lightcamera->ShadowTransform4x4();
	}
	
	ZShadeSandboxLighting::cbLightBuffer cLB;
	ZShadeSandboxLighting::cbAmbientLightBuffer alb;
	ZShadeSandboxLighting::cbDirectionalLightBuffer dlb;
	ZShadeSandboxLighting::cbSpotLightBuffer slb;
	ZShadeSandboxLighting::cbPointLightBuffer plb;
	ZShadeSandboxLighting::cbCapsuleLightBuffer clb;
	ZShadeSandboxLighting::cbSunLightBuffer cSLB;

	if (material->GetLightBuffer() != 0)
	{
		for (int i = 0; i < material->GetLightBuffer()->g_AmbientLightCount; i++)
		{
			alb.g_AmbientColor = material->GetLightBuffer()->g_AmbientLight[i].g_AmbientColor;
			cLB.g_AmbientLight[i] = alb;
		}
		for (int i = 0; i < material->GetLightBuffer()->g_DirectionalLightCount; i++)
		{
			dlb.g_Direction = material->GetLightBuffer()->g_DirectionalLight[i].g_LightDirection;
			dlb.padding = 0;
			dlb.g_Ambient = material->GetLightBuffer()->g_DirectionalLight[i].g_AmbientColor;
			dlb.g_Diffuse = material->GetLightBuffer()->g_DirectionalLight[i].g_DiffuseColor;
			cLB.g_DirectionalLight[i] = dlb;
		}
		for (int i = 0; i < material->GetLightBuffer()->g_SpotLightCount; i++)
		{
			slb.g_AmbientColor = material->GetLightBuffer()->g_SpotLight[i].g_AmbientColor;
			slb.g_DiffuseColor = material->GetLightBuffer()->g_SpotLight[i].g_DiffuseColor;
			slb.g_LightPosition = material->GetLightBuffer()->g_SpotLight[i].g_LightPosition;
			slb.padding = 0;
			slb.g_LightRange = material->GetLightBuffer()->g_SpotLight[i].g_LightRange;
			slb.g_SpotCosOuterCone = material->GetLightBuffer()->g_SpotLight[i].g_SpotCosOuterCone;
			slb.g_SpotInnerConeReciprocal = material->GetLightBuffer()->g_SpotLight[i].g_SpotInnerConeReciprocal;
			slb.g_CosineAngle = material->GetLightBuffer()->g_SpotLight[i].g_CosineAngle;
			cLB.g_SpotLight[i] = slb;
		}
		for (int i = 0; i < material->GetLightBuffer()->g_PointLightCount; i++)
		{
			plb.g_LightPosition = material->GetLightBuffer()->g_PointLight[i].g_LightPosition;
			plb.g_LightRange = material->GetLightBuffer()->g_PointLight[i].g_LightRange;
			plb.g_Attenuation = material->GetLightBuffer()->g_PointLight[i].g_Attenuation;
			plb.padding = 0;
			plb.g_AmbientColor = material->GetLightBuffer()->g_PointLight[i].g_AmbientColor;
			plb.g_DiffuseColor = material->GetLightBuffer()->g_PointLight[i].g_DiffuseColor;
			cLB.g_PointLight[i] = plb;
		}
		for (int i = 0; i < material->GetLightBuffer()->g_CapsuleLightCount; i++)
		{
			clb.g_LightPosition = material->GetLightBuffer()->g_CapsuleLight[i].g_LightPosition;
			clb.g_LightRange = material->GetLightBuffer()->g_CapsuleLight[i].g_LightRange;
			clb.g_LightDirection = material->GetLightBuffer()->g_CapsuleLight[i].g_LightDirection;
			clb.g_DiffuseColor = material->GetLightBuffer()->g_CapsuleLight[i].g_DiffuseColor;
			clb.g_LightLength = material->GetLightBuffer()->g_CapsuleLight[i].g_LightLength;
			clb.g_CapsuleDirectionLength = material->GetLightBuffer()->g_CapsuleLight[i].g_CapsuleDirectionLength;
			clb.g_CapsuleIntensity = material->GetLightBuffer()->g_CapsuleLight[i].g_CapsuleIntensity;
			cLB.g_CapsuleLight[i] = clb;
		}
		cLB.g_AmbientLightCount = material->GetLightBuffer()->g_AmbientLightCount;
		cLB.g_DirectionalLightCount = material->GetLightBuffer()->g_DirectionalLightCount;
		cLB.g_SpotLightCount = material->GetLightBuffer()->g_SpotLightCount;
		cLB.g_PointLightCount = material->GetLightBuffer()->g_PointLightCount;
		cLB.g_CapsuleLightCount = material->GetLightBuffer()->g_CapsuleLightCount;
		cLB.g_AmbientDown = material->GetLightBuffer()->g_AmbientDown;
		cLB.g_AmbientUp = material->GetLightBuffer()->g_AmbientUp;
		cLB.padding = 0;

		cSLB.g_SunDir = material->GetSunLightBuffer()->g_SunDir;
		cSLB.g_EnableSun = material->GetSunLightBuffer()->g_EnableSun;
		cSLB.g_SunDiffuseColor = material->GetSunLightBuffer()->g_SunDiffuseColor;
		cSLB.padding = XMFLOAT3(0, 0, 0);
		cSLB.g_SunShineness = material->GetSunLightBuffer()->g_SunShineness;
	}

	// Map the per frame constants
	{
	D3D11_MAPPED_SUBRESOURCE mapped_res;
		m_pD3DSystem->GetDeviceContext()->Map(m_pPerFrameCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
		{
			assert(mapped_res.pData);
			*(Const_Per_Frame*)mapped_res.pData = per_frame;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pPerFrameCB, 0);
	}
	
	// Map the matrix constants
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res2;
		m_pD3DSystem->GetDeviceContext()->Map(m_pPerObjectCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res2);
		{
			assert(mapped_res2.pData);
			*(Const_Per_Object*)mapped_res2.pData = per_object;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pPerObjectCB, 0);
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
	
	ID3D11Buffer* vs_cbs[2] = { m_pPerFrameCB, m_pPerObjectCB };
	m_pD3DSystem->GetDeviceContext()->VSSetConstantBuffers(0, 2, vs_cbs);
	
	ID3D11Buffer* ps_cbs[3] = { m_pPerFrameCB, m_pLightCB, m_pSunCB };
	m_pD3DSystem->GetDeviceContext()->PSSetConstantBuffers(0, 3, ps_cbs);

	ID3D11ShaderResourceView* ps_srvs[8] = { diffuseArrayTexture, diffuseTexture, normalMapTexture, blendMapTexture, detailMapTexture, alphaMapTexture, shadowMapTexture, ssaoTexture };
	
	if (!m_Wireframe)
	{
		// Assign Texture
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 8, ps_srvs);

		SwitchTo("MaterialShaderPS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}
	else
	{
		SwitchTo("MaterialShaderWireframePS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}
	
	SetVertexShader();
	SetPixelShader();

	SetInputLayout("MaterialShader");

	//Perform Drawing
	RenderIndex11(indexCount);

	// Unbind
	if (!m_Wireframe)
	{
		for (int i = 0; i < 8; i++) ps_srvs[i] = NULL;
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 8, ps_srvs);
	}

	return true;
}
//==============================================================================================================================