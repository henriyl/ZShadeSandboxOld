#include "OceanSurfaceShader.h"
#include "ConstantBuffer.h"
#include "CGlobal.h"
//==============================================================================================================================
OceanSurfaceShader::OceanSurfaceShader(D3D* d3d)
:	ZShadeSandboxShader::Shader(d3d)
{
	Initialize();
}
//==============================================================================================================================
OceanSurfaceShader::OceanSurfaceShader(const OceanSurfaceShader& other)
:	ZShadeSandboxShader::Shader(other)
{
}
//==============================================================================================================================
OceanSurfaceShader::~OceanSurfaceShader()
{
}
//==============================================================================================================================
bool OceanSurfaceShader::Initialize()
{
	ConstantBuffer<cbPerFrameBuffer> perFrameCB(m_pD3DSystem);
	perFrameCB.Initialize(PAD16(sizeof(cbPerFrameBuffer)));
	m_pPerFrameCB = perFrameCB.Buffer();
	
	ConstantBuffer<cbTessellationBuffer> tessellationCB(m_pD3DSystem);
	tessellationCB.Initialize(PAD16(sizeof(cbTessellationBuffer)));
	m_pTessellationCB = tessellationCB.Buffer();
	
	ConstantBuffer<cbMatrixBuffer> matrixCB(m_pD3DSystem);
	matrixCB.Initialize(PAD16(sizeof(cbMatrixBuffer)));
	m_pMatrixCB = matrixCB.Buffer();
	
	ClearInputLayout();
	
	SetInputLayoutDesc("OceanSurfaceShader", ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_tex, 2);
	LoadVertexShader("OceanSurfaceVS");
	LoadHullShader("OceanSurfaceHS");
	LoadDomainShader("OceanSurfaceDS");
	LoadPixelShader("OceanSurfacePS");
	LoadPixelShader("OceanSurfaceWireframePS");
	AssignVertexShaderLayout("OceanSurfaceShader");
	
	mTexDisplacement = 0;
	mTexPerlin = 0;
	mTexGradient = 0;
	mWaveMap0 = 0;
	mWaveMap1 = 0;
	mReflectionMap = 0;
	mRefractionMap = 0;

	return true;
}
//==============================================================================================================================
void OceanSurfaceShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
}
//==============================================================================================================================
bool OceanSurfaceShader::Render11(int indexCount, Camera* camera, int frustumCull, XMMATRIX world, OceanParameters ocean_params)
{
	cbPerFrameBuffer cFB;
	cbTessellationBuffer cTB;
	cbMatrixBuffer cMB;
	
	cTB.g_EyePos = camera->Position();
	cTB.g_FrustumCull = frustumCull;
	cTB.g_MinDist = ocean_params.g_MinDist;
	cTB.g_MaxDist = ocean_params.g_MaxDist;
	cTB.g_MinTess = ocean_params.g_MinTess;
	cTB.g_MaxTess = ocean_params.g_MaxTess;
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
	
	cFB.g_padding = XMFLOAT2(0, 0);
	cFB.g_SeaLevel = ocean_params.g_SeaLevel;
	cFB.g_WaveHeight = ocean_params.g_WaveHeight;
	cFB.g_LightDirection = ocean_params.g_LightDirection;
	cFB.g_SpecularShininess = ocean_params.g_SpecularShininess;
	cFB.g_CamPos = camera->Position();
	cFB.g_Time = ocean_params.g_Time;
	cFB.g_RefractionTint = ocean_params.g_RefractionTint;
	cFB.g_DullColor = ocean_params.g_DullColor;
	// Map the per frame constants
	{
	D3D11_MAPPED_SUBRESOURCE mapped_res;
		m_pD3DSystem->GetDeviceContext()->Map(m_pPerFrameCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
		{
			assert(mapped_res.pData);
			*(cbPerFrameBuffer*)mapped_res.pData = cFB;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pPerFrameCB, 0);
	}
	
	cMB.g_matWorld = ZShadeSandboxMath::ZMath::GMathMF(XMMatrixTranspose(world));
	cMB.g_matView = camera->View4x4();
	cMB.g_matProj = camera->Proj4x4();
	cMB.g_ReflectionView = camera->ReflectionView4x4();
	// Map the matrix constants
	{
		D3D11_MAPPED_SUBRESOURCE mapped_res2;
		m_pD3DSystem->GetDeviceContext()->Map(m_pMatrixCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res2);
		{
			assert(mapped_res2.pData);
			*(cbMatrixBuffer*)mapped_res2.pData = cMB;
		}
		m_pD3DSystem->GetDeviceContext()->Unmap(m_pMatrixCB, 0);
	}
	
	// Set the tessellation constant buffer into the Hull Shader
	ID3D11Buffer* hs_cbs[1] = { m_pTessellationCB };
	m_pD3DSystem->GetDeviceContext()->HSSetConstantBuffers(0, 1, hs_cbs);
	
	// Set the matrix constant buffer into the Domain Shader
	ID3D11Buffer* ds_cbs[2] = { m_pPerFrameCB, m_pMatrixCB };
	m_pD3DSystem->GetDeviceContext()->DSSetConstantBuffers(0, 2, ds_cbs);
	
	// Set the matrix constant buffer into the Vertex Shader
	//ID3D11Buffer* vs_cbs[2] = { m_pPerFrameCB, m_pMatrixCB };
	//m_pD3DSystem->GetDeviceContext()->VSSetConstantBuffers(0, 2, vs_cbs);

	ID3D11Buffer* ps_cbs[1] = { m_pPerFrameCB };
	m_pD3DSystem->GetDeviceContext()->PSSetConstantBuffers(0, 1, ps_cbs);

	ID3D11ShaderResourceView* ds_srvs[1] = { mTexDisplacement };
	ID3D11SamplerState* ds_samp[1] = { m_pD3DSystem->Point() };

	ID3D11ShaderResourceView* ps_srvs[7] = { mTexDisplacement, mTexPerlin, mTexGradient, mWaveMap0, mWaveMap1, mReflectionMap, mRefractionMap };
	ID3D11SamplerState* ps_samp[3] = { m_pD3DSystem->Mirror(), m_pD3DSystem->Linear(), m_pD3DSystem->Point() };

	if (!m_Wireframe)
	{
		// Assign Texture
		m_pD3DSystem->GetDeviceContext()->DSSetShaderResources(0, 1, ds_srvs);
		m_pD3DSystem->GetDeviceContext()->DSSetSamplers(0, 1, ds_samp);

		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 7, ps_srvs);
		m_pD3DSystem->GetDeviceContext()->PSSetSamplers(0, 3, ps_samp);

		SwitchTo("OceanSurfacePS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}
	else
	{
		SwitchTo("OceanSurfaceWireframePS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}
	
	SetInputLayout("OceanSurfaceShader");

	SetVertexShader();
	SetHullShader();
	SetDomainShader();
	SetPixelShader();

	//Perform Drawing
	RenderIndex11(indexCount);

	// Unbind
	if (!m_Wireframe)
	{
		for (int i = 0; i < 1; i++) ds_srvs[i] = NULL;
		m_pD3DSystem->GetDeviceContext()->DSSetShaderResources(0, 1, ds_srvs);
		for (int i = 0; i < 1; i++) ds_samp[i] = NULL;
		m_pD3DSystem->GetDeviceContext()->DSSetSamplers(0, 1, ds_samp);

		for (int i = 0; i < 7; i++) ps_srvs[i] = NULL;
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 7, ps_srvs);
		for (int i = 0; i < 3; i++) ps_samp[i] = NULL;
		m_pD3DSystem->GetDeviceContext()->PSSetSamplers(0, 3, ps_samp);
	}
	
	// Set Hull, Domain and Geometry Shaders to null in case they are not needed
	m_pD3DSystem->GetDeviceContext()->HSSetShader(NULL, NULL, 0);
	m_pD3DSystem->GetDeviceContext()->DSSetShader(NULL, NULL, 0);
	m_pD3DSystem->GetDeviceContext()->GSSetShader(NULL, NULL, 0);

	return true;
}
//==============================================================================================================================