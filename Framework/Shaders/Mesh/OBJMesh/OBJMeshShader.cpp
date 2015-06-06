#include "OBJMeshShader.h"
#include "ConstantBuffer.h"
//==============================================================================================================================
OBJMeshShader::OBJMeshShader(D3D* d3d)
:	ZShadeSandboxShader::Shader(d3d)
{
	Initialize();
}
//==============================================================================================================================
OBJMeshShader::OBJMeshShader(const OBJMeshShader& other)
:	ZShadeSandboxShader::Shader(other)
{
}
//==============================================================================================================================
OBJMeshShader::~OBJMeshShader()
{
	m_pOBJShadingCB->Release();
	m_pMatrixBufferCB->Release();
}
//==============================================================================================================================
bool OBJMeshShader::Initialize()
{
	ConstantBuffer<cbOBJShadingConst> objShadingCB(m_pD3DSystem);
	objShadingCB.Initialize(PAD16(sizeof(cbOBJShadingConst)));
	m_pOBJShadingCB = objShadingCB.Buffer();
	
	ConstantBuffer<cbMatrixBuffer> matrixBufferCB(m_pD3DSystem);
	matrixBufferCB.Initialize(PAD16(sizeof(cbMatrixBuffer)));
	m_pMatrixBufferCB = matrixBufferCB.Buffer();
	
	ClearInputLayout();
	SetInputLayoutDesc("OBJMeshShader", ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_normal_tex, 3);
	
	LoadVertexShader("OBJMeshVS");
	LoadPixelShader("OBJMeshPS");
	LoadPixelShader("OBJMeshWireframePS");
	
	AssignVertexShaderLayout("OBJMeshShader");
	
	return true;
}
//==============================================================================================================================
void OBJMeshShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
}
//==============================================================================================================================
bool OBJMeshShader::Render(int startIndex, int indexCount, Camera* camera, XMMATRIX wvp, XMFLOAT4 clipplane, XMFLOAT4 difColor, bool hasTexture,
	ID3D11ShaderResourceView* texture)
{
	cbOBJShadingConst cSC;
	cbMatrixBuffer cMB;
	
	cMB.g_WVPMatrix = ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(wvp));
	
	//if (m_UseCustomWorld)
	//	cMB.g_WorldMatrix = ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(mWorld.Get()));
	//else
	//	cMB.g_WorldMatrix = camera->World4x4();
	
	cSC.g_ClipPlane = clipplane;
	cSC.g_DifColor = difColor;
	cSC.padding = XMFLOAT2(0, 0);
	cSC.g_FarPlane = fFarPlane;
	cSC.g_hasTexture = (hasTexture == true) ? 1 : 0;
	
	// Map shading constants
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res;
		m_pD3DSystem->GetDeviceContext()->Map(m_pOBJShadingCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
		{
			assert(mapped_res.pData);
			*(cbOBJShadingConst*)mapped_res.pData = cSC;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pOBJShadingCB, 0);
	}
	
	// Map matrix constants
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res;
		m_pD3DSystem->GetDeviceContext()->Map(m_pMatrixBufferCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
		{
			assert(mapped_res.pData);
			*(cbMatrixBuffer*)mapped_res.pData = cMB;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pMatrixBufferCB, 0);
	}
	
	// Set the shading constant buffer and matrix constant buffer into the Vertex ZShadeSandboxShader::Shader
	ID3D11Buffer* vs_cbs[2] = { m_pOBJShadingCB, m_pMatrixBufferCB };
	m_pD3DSystem->GetDeviceContext()->VSSetConstantBuffers(0, 2, vs_cbs);
	
	// Set the shading constant buffer into the Pixel Shader
	ID3D11Buffer* ps_cbs[1] = { m_pOBJShadingCB };
	m_pD3DSystem->GetDeviceContext()->PSSetConstantBuffers(0, 1, ps_cbs);
	
	ID3D11ShaderResourceView* ps_srvs[1] = { texture };
	ID3D11SamplerState* ps_samp[1] = { m_pD3DSystem->Linear() };
	
	if (!m_Wireframe)
	{
		m_pD3DSystem->TurnOffCulling();

		// Set the texture into the Pixel Shader
		if (hasTexture)
		{
			m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 1, ps_srvs);
			m_pD3DSystem->GetDeviceContext()->PSSetSamplers(0, 1, ps_samp);
		}

		SwitchTo("OBJMeshPS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}
	else
	{
		SwitchTo("OBJMeshWireframePS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}

	// Tell the shader what input layout to use
	SetInputLayout("OBJMeshShader");
	
	// Assign the shaders to render the mesh
	SetVertexShader();
	SetPixelShader();
	
	//Perform Drawing
	RenderIndex11(startIndex, indexCount);

	if (!m_Wireframe)
	{
		m_pD3DSystem->TurnOnCulling();
	}

	// Unbind
	if (!m_Wireframe && hasTexture)
	{
		ps_samp[0] = NULL;
		m_pD3DSystem->GetDeviceContext()->PSSetSamplers(0, 1, ps_samp);

		ps_srvs[0] = NULL;
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 1, ps_srvs);
	}
	
	return true;
}
//==============================================================================================================================