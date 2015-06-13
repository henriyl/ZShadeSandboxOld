#include "OBJMeshTessellationShader.h"
#include "ConstantBuffer.h"
//==============================================================================================================================
OBJMeshTessellationShader::OBJMeshTessellationShader(D3D* d3d)
:	ZShadeSandboxShader::Shader(d3d)
{
	Initialize();
}
//==============================================================================================================================
OBJMeshTessellationShader::OBJMeshTessellationShader(const OBJMeshTessellationShader& other)
:	ZShadeSandboxShader::Shader(other)
{
}
//==============================================================================================================================
OBJMeshTessellationShader::~OBJMeshTessellationShader()
{
}
//==============================================================================================================================
bool OBJMeshTessellationShader::Initialize()
{
	ConstantBuffer<cbTessellationBuffer> tessellationCB(m_pD3DSystem);
	tessellationCB.Initialize(PAD16(sizeof(cbTessellationBuffer)));
	m_pTessellationCB = tessellationCB.Buffer();
	
	ConstantBuffer<cbDomainConst> domainCB(m_pD3DSystem);
	domainCB.Initialize(PAD16(sizeof(cbDomainConst)));
	m_pDomainCB = domainCB.Buffer();
	
	ConstantBuffer<cbOBJShadingConst> objShadingCB(m_pD3DSystem);
	objShadingCB.Initialize(PAD16(sizeof(cbOBJShadingConst)));
	m_pOBJShadingCB = objShadingCB.Buffer();
	
	ConstantBuffer<cbMatrixBuffer> matrixBufferCB(m_pD3DSystem);
	matrixBufferCB.Initialize(PAD16(sizeof(cbMatrixBuffer)));
	m_pMatrixBufferCB = matrixBufferCB.Buffer();
	
	ClearInputLayout();
	SetInputLayoutDesc("OBJMeshTessellationShader", ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_normal_tex, 3);
	
	LoadVertexShader("OBJMeshTessellationVS");
	LoadHullShader("OBJMeshTessellationHS");
	LoadDomainShader("OBJMeshTessellationDS");
	LoadPixelShader("OBJMeshTessellationPS");
	LoadPixelShader("OBJMeshTessellationWireframePS");
	
	AssignVertexShaderLayout("OBJMeshTessellationShader");
	
	return true;
}
//==============================================================================================================================
void OBJMeshTessellationShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
}
//==============================================================================================================================
bool OBJMeshTessellationShader::Render(int startIndex, int indexCount, float minTessDist, float maxTessDist, float minTess, float maxTess,
	Camera* camera, XMMATRIX wvp, XMFLOAT4 clipplane, XMFLOAT4 difColor, bool hasTexture,
	ID3D11ShaderResourceView* texture)
{
	cbTessellationBuffer cTB;
	cbDomainConst cDC;
	cbOBJShadingConst cSC;
	cbMatrixBuffer cMB;
	
	cTB.g_TessellationFactor = 64.0f;
	cTB.g_EyePosW = camera->Position();
	cTB.g_MinDist = minTessDist;
	cTB.g_MaxDist = maxTessDist;
	cTB.g_MinTess = minTess;
	cTB.g_MaxTess = maxTess;
	// Map tessellation constants
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res;
		m_pD3DSystem->GetDeviceContext()->Map(m_pTessellationCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
		{
			assert(mapped_res.pData);
			*(cbTessellationBuffer*)mapped_res.pData = cTB;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pTessellationCB, 0);
	}
	
	cDC.g_ClipPlane = clipplane;
	cDC.padding = XMFLOAT3(0, 0, 0);
	cDC.g_FarPlane = fFarPlane;
	// Map domain constants
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res;
		m_pD3DSystem->GetDeviceContext()->Map(m_pDomainCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
		{
			assert(mapped_res.pData);
			*(cbDomainConst*)mapped_res.pData = cDC;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pDomainCB, 0);
	}
	
	cSC.g_DifColor = difColor;
	cSC.padding = XMFLOAT3(0, 0, 0);
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
	
	cMB.g_WVPMatrix = ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(wvp));
	//if (m_UseCustomWorld)
	//	cMB.g_WorldMatrix = ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(mWorld.Get()));
	//else
	//	cMB.g_WorldMatrix = camera->World4x4();
	//cMB.g_World = ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(world));
	//cMB.g_View = camera->View4x4();
	//cMB.g_Proj = camera->Proj4x4();
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
	
	// Set the tessellation constant buffer into the Hull Shader
	ID3D11Buffer* hs_cbs[2] = { m_pTessellationCB, m_pMatrixBufferCB };
	m_pD3DSystem->GetDeviceContext()->HSSetConstantBuffers(0, 2, hs_cbs);
	
	// Set the domain constant buffer and matrix constant buffer into the Domain Shader
	ID3D11Buffer* ds_cbs[2] = { m_pDomainCB, m_pMatrixBufferCB };
	m_pD3DSystem->GetDeviceContext()->DSSetConstantBuffers(0, 2, ds_cbs);
	
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

		SwitchTo("OBJMeshTessellationPS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}
	else
	{
		SwitchTo("OBJMeshTessellationWireframePS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}
	
	// Tell the shader what input layout to use
	SetInputLayout("OBJMeshTessellationShader");
	
	// Assign the shaders to render the mesh
	SetVertexShader();
	SetHullShader();
	SetDomainShader();
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
	
	// Set Hull, Domain and Geometry Shaders to null in case they are not needed
	m_pD3DSystem->GetDeviceContext()->HSSetShader(NULL, NULL, 0);
	m_pD3DSystem->GetDeviceContext()->DSSetShader(NULL, NULL, 0);
	m_pD3DSystem->GetDeviceContext()->GSSetShader(NULL, NULL, 0);
	
	return true;
}
//==============================================================================================================================