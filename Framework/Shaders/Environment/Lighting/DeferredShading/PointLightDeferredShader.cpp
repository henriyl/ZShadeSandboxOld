#include "PointLightDeferredShader.h"
#include "ConstantBuffer.h"
#include "CGlobal.h"
//==============================================================================================================================
PointLightDeferredShader::PointLightDeferredShader(D3D* d3d)
:	ZShadeSandboxShader::Shader(d3d)
{
	Initialize();
}
//==============================================================================================================================
PointLightDeferredShader::PointLightDeferredShader(const PointLightDeferredShader& other)
:	ZShadeSandboxShader::Shader(other)
{
}
//==============================================================================================================================
PointLightDeferredShader::~PointLightDeferredShader()
{
}
//==============================================================================================================================
bool PointLightDeferredShader::Initialize()
{
	ConstantBuffer<cbDomainBuffer> domainCB(m_pD3DSystem);
	domainCB.Initialize(PAD16(sizeof(cbDomainBuffer)));
	m_pDomainCB = domainCB.Buffer();
	
	ConstantBuffer<cbPixelBuffer> pixelCB(m_pD3DSystem);
	pixelCB.Initialize(PAD16(sizeof(cbPixelBuffer)));
	m_pPixelCB = pixelCB.Buffer();
	
	LoadVertexShader("PointLightDeferredVS");
	LoadHullShader("PointLightDeferredHS");
	LoadDomainShader("PointLightDeferredDS");
	LoadPixelShader("PointLightDeferredPS");
	LoadPixelShader("PointLightDeferredWireframePS");
	
	return true;
}
//==============================================================================================================================
void PointLightDeferredShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
}
//==============================================================================================================================
bool PointLightDeferredShader::Render11
(	Camera* camera
,	ZShadeSandboxLighting::PointLight* light
,	ID3D11ShaderResourceView* color0Texture
,	ID3D11ShaderResourceView* color1Texture
,	ID3D11ShaderResourceView* normalTexture
,	ID3D11ShaderResourceView* depthTexture
)
{
	cbDomainBuffer cDB;
	cDB.g_LightProjection = light->Perspective()->LightViewProj4x4();
	// Map the domain buffer
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res2;
		m_pD3DSystem->GetDeviceContext()->Map(m_pDomainCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res2);
		{
			assert(mapped_res2.pData);
			*(cbDomainBuffer*)mapped_res2.pData = cDB;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pDomainCB, 0);
	}
	
	cbPixelBuffer cPB;
	cPB.g_PointLightPosition = light->Position();
	cPB.g_PointLightRange = light->Range();
	cPB.g_PointLightColor = XMFLOAT3(light->DiffuseColor().x, light->DiffuseColor().y, light->DiffuseColor().z);
	cPB.padding1 = 0;
	cPB.g_PointLightIntensity = light->Attenuation();
	cPB.padding2 = 0;
	cPB.g_EyePosition = camera->Position();
	cPB.padding3 = 0;
	// XY components should be filled with one over the projection matrix diagonal values.
	// 		g_PerspectiveValues.x = 1 / Pm[0][0]
	// Z component is the projection matrix Pm[3][2] value.
	// W component is the negative projection matrix Pm[2][2] value.
	ZShadeSandboxMath::XMMatrix proj(camera->Proj());
	cPB.g_PerspectiveValues.x = 1 / proj._11;
	cPB.g_PerspectiveValues.y = 1 / proj._11;
	cPB.g_PerspectiveValues.z = proj._32;
	cPB.g_PerspectiveValues.w = proj._22;
	cPB.g_ViewInv = camera->InvView4x4();
	// Map the pixel buffer
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res2;
		m_pD3DSystem->GetDeviceContext()->Map(m_pPixelCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res2);
		{
			assert(mapped_res2.pData);
			*(cbPixelBuffer*)mapped_res2.pData = cPB;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pPixelCB, 0);
	}
	
	ID3D11Buffer* ds_cbs[1] = { m_pDomainCB };
	m_pD3DSystem->GetDeviceContext()->DSSetConstantBuffers(0, 1, ds_cbs);
	
	ID3D11Buffer* ps_cbs[1] = { m_pPixelCB };
	m_pD3DSystem->GetDeviceContext()->PSSetConstantBuffers(1, 1, ps_cbs);
	
	// Assign Texture
	ID3D11ShaderResourceView* ps_srvs[4] = { color0Texture, color1Texture, normalTexture, depthTexture };
	ID3D11SamplerState* ps_samp[1] = { m_pD3DSystem->Point() };
	
	if (!m_Wireframe)
	{
		// Set new rasterizer and stencil states
		
		// Helps to avoid culling some or all of the point light volume faces when the camera is inside the volume
		m_pD3DSystem->SetDepthStencilState(m_pD3DSystem->ReverseDepthEnabled());
		m_pD3DSystem->SetRasterizerState(m_pD3DSystem->BackFaceCull());
		
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 4, ps_srvs);
		m_pD3DSystem->GetDeviceContext()->PSSetSamplers(0, 1, ps_samp);
		
		SwitchTo("PointLightDeferredPS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}
	else
	{
		SwitchTo("PointLightDeferredWireframePS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}
	
	// Use additive blending to accumulate the light values from all other lights
	m_pD3DSystem->TurnOnAdditiveBlending();
	m_pD3DSystem->TurnOnAlphaBlending();
	
	SetVertexShader();
	SetHullShader();
	SetDomainShader();
	SetPixelShader();
	
	// Enable the tessellation feature
	m_pD3DSystem->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_1_CONTROL_POINT_PATCHLIST);
	
	RenderDraw11(1);
	
	// Unbind
	if (!m_Wireframe)
	{
		ps_samp[0] = NULL;
		m_pD3DSystem->GetDeviceContext()->PSSetSamplers(0, 1, ps_samp);
		ps_srvs[0] = NULL;
		ps_srvs[1] = NULL;
		ps_srvs[2] = NULL;
		ps_srvs[3] = NULL;
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 4, ps_srvs);
	}
	
	m_pD3DSystem->TurnOffAdditiveBlending();
	
	return true;
}
//==============================================================================================================================