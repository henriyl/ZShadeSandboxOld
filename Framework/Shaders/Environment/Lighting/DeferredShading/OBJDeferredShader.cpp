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
	
	ConstantBuffer<cbMatrixBuffer2> matrix2CB(m_pD3DSystem);
	matrix2CB.Initialize(PAD16(sizeof(cbMatrixBuffer2)));
	m_pMatrix2CB = matrix2CB.Buffer();
	
	ClearInputLayout();
	
	SetInputLayoutDesc("DeferredShader", ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_normal_tex, 3);
	
	LoadVertexShader("DeferredShaderVS2");
	LoadPixelShader("DeferredShaderPS");
	LoadPixelShader("DeferredShaderWireframePS");
	
	AssignVertexShaderLayout("DeferredShader");
	
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
,	XMMATRIX wvp
,	XMFLOAT2 specularPowerRange
,	float specularIntensity
,	float specularPower
,	ID3D11ShaderResourceView* texture
)
{
	cbPackBuffer cPB;
	cPB.g_SpecularPowerRange = specularPowerRange;
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
	
	cbMatrixBuffer2 cMB;
	cMB.g_matWVP = ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(wvp));
	// Map the matrix buffer
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res2;
		m_pD3DSystem->GetDeviceContext()->Map(m_pMatrix2CB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res2);
		{
			assert(mapped_res2.pData);
			*(cbMatrixBuffer2*)mapped_res2.pData = cMB;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pMatrix2CB, 0);
	}
	
	ID3D11Buffer* vs_cbs[1] = { m_pMatrix2CB };
	m_pD3DSystem->GetDeviceContext()->VSSetConstantBuffers(2, 1, vs_cbs);
	
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

		SwitchTo("DeferredShaderPS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}
	else
	{
		SwitchTo("DeferredShaderWireframePS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}

	SetInputLayout("DeferredShader");
	
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