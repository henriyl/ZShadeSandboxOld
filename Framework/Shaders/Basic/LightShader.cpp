#include "LightShader.h"
#include "ConstantBuffer.h"
#include "CGlobal.h"
#include "DirectionalLight.h"
//==============================================================================================================================
LightShader::LightShader(D3D* d3d)
:	ZShadeSandboxShader::Shader(d3d)
{
	Initialize();
}
//==============================================================================================================================
LightShader::LightShader(const LightShader& other)
:	ZShadeSandboxShader::Shader(other)
{
}
//==============================================================================================================================
LightShader::~LightShader()
{
}
//==============================================================================================================================
bool LightShader::Initialize()
{
	ConstantBuffer<cbLightBuffer> perFrameCB(m_pD3DSystem);
	perFrameCB.Initialize(PAD16(sizeof(cbLightBuffer)));
	m_pLightBufferCB = perFrameCB.Buffer();
	
	ConstantBuffer<cbMatrixBuffer> perObjectCB(m_pD3DSystem);
	perObjectCB.Initialize(PAD16(sizeof(cbMatrixBuffer)));
	m_pMatrixBufferCB = perObjectCB.Buffer();
	
	ClearInputLayout();
	
	SetInputLayoutDesc("LightShader", ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_normal_tex, 3);
	//SetInputLayoutDesc("MaterialShader", ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_tex, 2);
	
	LoadVertexShader("LightShaderVS");
	LoadPixelShader("LightShaderPS");
	LoadPixelShader("LightShaderWireframePS");
	
	AssignVertexShaderLayout("LightShader");
	
	return true;
}
//==============================================================================================================================
void LightShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
}
//==============================================================================================================================
bool LightShader::Render11(int indexCount, ZShadeSandboxMath::XMMath4 clipplane, Camera* camera, ZShadeSandboxLighting::DirectionalLight* light, ZShadeSandboxLighting::ShaderMaterial* material)
{
	cbLightBuffer cLB;
	cbMatrixBuffer cMB;

	int useDiffuseTexture = 0;
	
	ID3D11ShaderResourceView* diffuseTexture = 0;
	ID3D11ShaderResourceView* shadowMapTexture = material->ShadowMapTexture();
	ID3D11ShaderResourceView* ssaoTexture = material->SSAOTexture();
	
	for (int i = 0; i < material->TextureCount(); i++)
	{
		switch (material->GetMaterialTextureType(i))
		{
			case ZShadeSandboxLighting::EMaterialTextureType::eDiffuse:
				useDiffuseTexture = 1;
				diffuseTexture = material->GetTexture(i)->getTexture11();
			break;
		}
	}

	cLB.g_LightAmbient = light->AmbientColor();
	cLB.g_LightDiffuse = light->DiffuseColor();
	cLB.g_LightDirection = light->Direction();
	cLB.g_ToggleShadowMap = (material->EnableShadowMap() == true) ? 1 : 0;

	if (m_UseCustomWorld)
	{
		XMFLOAT4X4 world = ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(mWorld.Get()));
		cMB.g_matWorld = world;
	}
	else
		cMB.g_matWorld = camera->World4x4();

	if (m_UseCustomView)
	{
		cMB.g_matView = mView;
	}
	else
		cMB.g_matView = camera->View4x4();

	if (m_pD3DSystem->GetEngineOptions()->m_DimType == DimType::ZSHADE_2D)
		cMB.g_matProj = camera->Ortho4x4();
	else if (m_pD3DSystem->GetEngineOptions()->m_DimType == DimType::ZSHADE_3D)
		cMB.g_matProj = camera->Proj4x4();

	if (m_UseOrtho)
	{
		cMB.g_matProj = camera->Ortho4x4();
	}
	
	if (light == NULL)
	{
		cMB.g_LightView = ZShadeSandboxMath::ZMath::GMathMF(XMMatrixIdentity());
		cMB.g_LightProj = ZShadeSandboxMath::ZMath::GMathMF(XMMatrixIdentity());
		cMB.g_ShadowMatrix = ZShadeSandboxMath::ZMath::GMathMF(XMMatrixIdentity());
	}
	else
	{
		cMB.g_LightView = light->Perspective()->LightView4x4();
		cMB.g_LightProj = light->Perspective()->LightProj4x4();
		XMMATRIX shadowTransform = mWorld.Get() * light->Perspective()->ShadowTransform();
		cMB.g_ShadowMatrix = ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(shadowTransform));//lightcamera->ShadowTransform4x4();
	}

	// Map the per frame constants
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res;
		m_pD3DSystem->GetDeviceContext()->Map(m_pLightBufferCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
		{
			assert(mapped_res.pData);
			*(cbLightBuffer*)mapped_res.pData = cLB;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pLightBufferCB, 0);
	}
	
	// Map the matrix constants
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res2;
		m_pD3DSystem->GetDeviceContext()->Map(m_pMatrixBufferCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res2);
		{
			assert(mapped_res2.pData);
			*(cbMatrixBuffer*)mapped_res2.pData = cMB;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pMatrixBufferCB, 0);
	}
	
	ID3D11Buffer* vs_cbs[1] = { m_pMatrixBufferCB };
	m_pD3DSystem->GetDeviceContext()->VSSetConstantBuffers(1, 1, vs_cbs);
	
	ID3D11Buffer* ps_cbs[1] = { m_pLightBufferCB };
	m_pD3DSystem->GetDeviceContext()->PSSetConstantBuffers(0, 1, ps_cbs);

	ID3D11ShaderResourceView* ps_srvs[2] = { diffuseTexture, shadowMapTexture };// , ssaoTexture
	
	if (!m_Wireframe)
	{
		// Assign Texture
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 2, ps_srvs);

		SwitchTo("LightShaderPS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}
	else
	{
		SwitchTo("LightShaderWireframePS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}
	
	SetVertexShader();
	SetPixelShader();

	SetInputLayout("LightShader");

	//Perform Drawing
	RenderIndex11(indexCount);

	// Unbind
	if (!m_Wireframe)
	{
		for (int i = 0; i < 2; i++) ps_srvs[i] = NULL;
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 2, ps_srvs);
	}

	return true;
}
//==============================================================================================================================