#include "DirectionalLightDeferredShader.h"
#include "ConstantBuffer.h"
#include "CGlobal.h"
//==============================================================================================================================
DirectionalLightDeferredShader::DirectionalLightDeferredShader(D3D* d3d)
:	ZShadeSandboxShader::Shader(d3d)
{
	Initialize();
}
//==============================================================================================================================
DirectionalLightDeferredShader::DirectionalLightDeferredShader(const DirectionalLightDeferredShader& other)
:	ZShadeSandboxShader::Shader(other)
{
}
//==============================================================================================================================
DirectionalLightDeferredShader::~DirectionalLightDeferredShader()
{
}
//==============================================================================================================================
bool DirectionalLightDeferredShader::Initialize()
{
	ConstantBuffer<cbLightBuffer> lightCB(m_pD3DSystem);
	lightCB.Initialize(PAD16(sizeof(cbLightBuffer)));
	m_pLightCB = lightCB.Buffer();
	
	ConstantBuffer<cbMatrixBuffer> matrixCB(m_pD3DSystem);
	matrixCB.Initialize(PAD16(sizeof(cbMatrixBuffer)));
	m_pMatrixCB = matrixCB.Buffer();
	
	//ConstantBuffer<cbGBufferUnpackConst> unpackCB(m_pD3DSystem);
	//unpackCB.Initialize(PAD16(sizeof(cbGBufferUnpackConst)));
	//m_pUnpackCB = unpackCB.Buffer();
	
	LoadVertexShader("DirectionalLightDeferredVS");
	LoadPixelShader("DirectionalLightDeferredPS");
	LoadPixelShader("DirectionalLightDeferredWireframePS");
	
	return true;
}
//==============================================================================================================================
void DirectionalLightDeferredShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
}
//==============================================================================================================================
bool DirectionalLightDeferredShader::Render11
(	Camera* camera
,	ZShadeSandboxLighting::DirectionalLight* light
,	XMFLOAT3 ambientUp
,	XMFLOAT3 ambientDown
,	ID3D11ShaderResourceView* color0Texture
,	ID3D11ShaderResourceView* color1Texture
,	ID3D11ShaderResourceView* normalTexture
,	ID3D11ShaderResourceView* depthTexture
)
{
	cbLightBuffer cLB;
	cLB.g_DirectionLightColor = light->DiffuseColor();
	cLB.g_LightDirection = light->Direction();
	cLB.padding1 = 0;
	cLB.g_EyePosition = camera->Position();
	cLB.padding2 = 0;
	cLB.g_AmbientDown = ambientDown;
	cLB.padding3 = 0;
	cLB.g_AmbientUp = ambientUp;
	cLB.padding4 = 0;
	// Map the light buffer
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res2;
		m_pD3DSystem->GetDeviceContext()->Map(m_pLightCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res2);
		{
			assert(mapped_res2.pData);
			*(cbLightBuffer*)mapped_res2.pData = cLB;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pLightCB, 0);
	}
	
	cbMatrixBuffer cMB;
	//cMB.g_matWorld = ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(world));
	//cMB.g_matView = camera->View4x4();
	//cMB.g_matProj = camera->Proj4x4();
	cMB.g_InvViewProj = camera->InvViewProj4x4();
	// Map the matrix buffer
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res2;
		m_pD3DSystem->GetDeviceContext()->Map(m_pMatrixCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res2);
		{
			assert(mapped_res2.pData);
			*(cbMatrixBuffer*)mapped_res2.pData = cMB;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pMatrixCB, 0);
	}
	
	/*
	cbGBufferUnpackConst cUB;
	// XY components should be filled with one over the projection matrix diagonal values.
	// 		g_PerspectiveValues.x = 1 / Pm[0][0]
	// Z component is the projection matrix Pm[3][2] value.
	// W component is the negative projection matrix Pm[2][2] value.
	XMMatrix proj(camera->Proj());
	cUB.g_PerspectiveValues.x = 1 / proj._11;
	cUB.g_PerspectiveValues.y = 1 / proj._44;
	cUB.g_PerspectiveValues.z = proj._32;
	cUB.g_PerspectiveValues.w = proj._22;
	cUB.g_x4ViewInv = XMFLOAT4(0, 0, 0, 0);
	// Map the gbuffer unpack buffer
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res2;
		m_pD3DSystem->GetDeviceContext()->Map(m_pUnpackCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res2);
		{
			assert(mapped_res2.pData);
			*(cbGBufferUnpackConst*)mapped_res2.pData = cUB;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pUnpackCB, 0);
	}
	*/
	
	//ID3D11Buffer* vs_cbs[1] = { m_pMatrixCB };
	//m_pD3DSystem->GetDeviceContext()->VSSetConstantBuffers(1, 1, vs_cbs);
	
	ID3D11Buffer* ps_cbs[2] = { m_pLightCB, m_pMatrixCB };
	m_pD3DSystem->GetDeviceContext()->PSSetConstantBuffers(0, 2, ps_cbs);
	
	// Assign Texture
	ID3D11ShaderResourceView* ps_srvs[4] = { color0Texture, color1Texture, normalTexture, depthTexture };
	ID3D11SamplerState* ps_samp[2] = { m_pD3DSystem->Point(), m_pD3DSystem->Linear() };
	
	if (!m_Wireframe)
	{
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 4, ps_srvs);
		m_pD3DSystem->GetDeviceContext()->PSSetSamplers(0, 2, ps_samp);
		
		SwitchTo("DirectionalLightDeferredPS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}
	else
	{
		SwitchTo("DirectionalLightDeferredWireframePS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}
	
	m_pD3DSystem->TurnOnAdditiveBlending();
	m_pD3DSystem->TurnOnAlphaBlending();

	SetVertexShader();
	SetPixelShader();
	
	//Perform Drawing onto a fullscreen quad with a NULL Input Layout
	RenderDraw11(4);
	
	// Unbind
	if (!m_Wireframe)
	{
		ps_samp[0] = NULL;
		ps_samp[1] = NULL;
		m_pD3DSystem->GetDeviceContext()->PSSetSamplers(0, 2, ps_samp);
		ps_srvs[0] = NULL;
		ps_srvs[1] = NULL;
		ps_srvs[2] = NULL;
		ps_srvs[3] = NULL;
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 4, ps_srvs);
	}
	
	m_pD3DSystem->TurnOffAlphaBlending();
	m_pD3DSystem->TurnOffAdditiveBlending();
	
	return true;
}
//==============================================================================================================================