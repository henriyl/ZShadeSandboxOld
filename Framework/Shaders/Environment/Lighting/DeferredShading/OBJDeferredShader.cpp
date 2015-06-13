#include "OBJDeferredShader.h"
#include "ConstantBuffer.h"
#include "CGlobal.h"
//==============================================================================================================================
OBJDeferredShader::OBJDeferredShader(D3D* d3d)
:	ZShadeSandboxShader::Shader(d3d)
{
	Initialize();
}
//==============================================================================================================================
OBJDeferredShader::OBJDeferredShader(const OBJDeferredShader& other)
:	ZShadeSandboxShader::Shader(other)
{
}
//==============================================================================================================================
OBJDeferredShader::~OBJDeferredShader()
{
}
//==============================================================================================================================
bool OBJDeferredShader::Initialize()
{
	ConstantBuffer<cbPackBuffer> packCB(m_pD3DSystem);
	packCB.Initialize(PAD16(sizeof(cbPackBuffer)));
	m_pPackCB = packCB.Buffer();
	
	ConstantBuffer<cbMatrixBuffer> matrixCB(m_pD3DSystem);
	matrixCB.Initialize(PAD16(sizeof(cbMatrixBuffer)));
	m_pMatrixCB = matrixCB.Buffer();
	
	ClearInputLayout();
	SetInputLayoutDesc("WVPGBufferShader", ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_normal_tex, 3);
	LoadVertexShader("WVPGBufferVS");
	LoadPixelShader("WVPGBufferPS");
	LoadPixelShader("WVPGBufferWireframePS");
	AssignVertexShaderLayout("WVPGBufferShader");
	
	return true;
}
//==============================================================================================================================
void OBJDeferredShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
}
//==============================================================================================================================
bool OBJDeferredShader::Render11
(	int startIndex
,	int indexCount
,	Camera* camera
,	XMMATRIX world
,	XMMATRIX wvp
,	float specularIntensity
,	float specularPower
,	ID3D11ShaderResourceView* texture
)
{
	cbPackBuffer cPB;
	cPB.padding = XMFLOAT2(0, 0);
	cPB.g_SpecularIntensity = specularIntensity;
	cPB.g_SpecularPower = specularPower;
	// Map the pack buffer
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res2;
		m_pD3DSystem->GetDeviceContext()->Map(m_pPackCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res2);
		{
			assert(mapped_res2.pData);
			*(cbPackBuffer*)mapped_res2.pData = cPB;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pPackCB, 0);
	}
	
	cbMatrixBuffer cMB;
	cMB.g_World = ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(world));
	cMB.g_matWVP = ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(wvp));
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
	
	ID3D11Buffer* vs_cbs[1] = { m_pMatrixCB };
	m_pD3DSystem->GetDeviceContext()->VSSetConstantBuffers(1, 1, vs_cbs);
	
	ID3D11Buffer* ps_cbs[1] = { m_pPackCB };
	m_pD3DSystem->GetDeviceContext()->PSSetConstantBuffers(0, 1, ps_cbs);
	
	// Assign Texture
	ID3D11ShaderResourceView* ps_srvs[1] = { texture };
	ID3D11SamplerState* ps_samp[1] = { m_pD3DSystem->Linear() };
	
	if (!m_Wireframe)
	{
		m_pD3DSystem->TurnOffCulling();

		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 1, ps_srvs);
		m_pD3DSystem->GetDeviceContext()->PSSetSamplers(0, 1, ps_samp);

		SwitchTo("WVPGBufferPS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}
	else
	{
		SwitchTo("WVPGBufferWireframePS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}

	SetInputLayout("WVPGBufferShader");
	
	SetVertexShader();
	SetPixelShader();
	
	//Perform Drawing
	RenderIndex11(startIndex, indexCount);

	// Unbind
	if (!m_Wireframe)
	{
		m_pD3DSystem->TurnOnCulling();

		ps_samp[0] = NULL;
		m_pD3DSystem->GetDeviceContext()->PSSetSamplers(0, 1, ps_samp);
		ps_srvs[0] = NULL;
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 1, ps_srvs);
	}

	return true;
}
//==============================================================================================================================