#include "TextureShader.h"
#include "ConstantBuffer.h"
#include "CGlobal.h"
//==============================================================================================================================
TextureShader::TextureShader(D3D* d3d)
:	ZShadeSandboxShader::Shader(d3d)
{
	Initialize();
}
//==============================================================================================================================
TextureShader::TextureShader(const TextureShader& other)
:	ZShadeSandboxShader::Shader(other)
{
}
//==============================================================================================================================
TextureShader::~TextureShader()
{
}
//==============================================================================================================================
bool TextureShader::Initialize()
{
	ConstantBuffer<Const_Per_Frame> perFrameCB(m_pD3DSystem);
	perFrameCB.Initialize(PAD16(sizeof(Const_Per_Frame)));
	m_pPerFrameCB = perFrameCB.Buffer();
	
	ConstantBuffer<Const_Per_Object> perObjectCB(m_pD3DSystem);
	perObjectCB.Initialize(PAD16(sizeof(Const_Per_Object)));
	m_pPerObjectCB = perObjectCB.Buffer();
	
	ClearInputLayout();
	
	SetInputLayoutDesc("textureVertexShader", ZShadeSandboxMesh::VertexLayout::mesh_layout_pos_tex, 2);
	
	LoadVertexShader("textureVertexShader");
	LoadPixelShader("texturePixelShader");
	LoadPixelShader("TextureWireframePS");
	
	AssignVertexShaderLayout("textureVertexShader");
	
	return true;
}
//==============================================================================================================================
void TextureShader::Shutdown()
{
	// Shutdown the vertex and pixel shaders as well as the related objects.
}
//==============================================================================================================================
void TextureShader::FlipTextureHorizontally(bool flip)
{
	bFlipHorizontally = flip;
	bFlipVertically = !flip;
}
//==============================================================================================================================
bool TextureShader::Render11(int indexCount, ZShadeSandboxMath::XMMath4 clipplane, Camera* camera, ID3D11ShaderResourceView* texture)
{
	Const_Per_Frame per_frame;
	per_frame.g_ClipPlane = XMFLOAT4(clipplane.x, clipplane.y, clipplane.z, clipplane.w);
	per_frame.padding = XMFLOAT2(0, 0);
	per_frame.g_FlipTextureH = (bFlipHorizontally == true) ? 1 : 0;
	per_frame.g_FlipTextureV = (bFlipVertically == true) ? 1 : 0;
	
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
		//XMFLOAT4X4 view = GMathMF(XMMatrixTranspose(mView.Get()));
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

	// Consts
	D3D11_MAPPED_SUBRESOURCE mapped_res;
	m_pD3DSystem->GetDeviceContext()->Map(m_pPerFrameCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res);
	{
		assert(mapped_res.pData);
		*(Const_Per_Frame*)mapped_res.pData = per_frame;
	}
	m_pD3DSystem->GetDeviceContext()->Unmap(m_pPerFrameCB, 0);

	D3D11_MAPPED_SUBRESOURCE mapped_res2;
	m_pD3DSystem->GetDeviceContext()->Map(m_pPerObjectCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_res2);
	{
		assert(mapped_res2.pData);
		*(Const_Per_Object*)mapped_res2.pData = per_object;
	}
	m_pD3DSystem->GetDeviceContext()->Unmap(m_pPerObjectCB, 0);

	ID3D11Buffer* vs_cbs[2] = { m_pPerFrameCB, m_pPerObjectCB };
	m_pD3DSystem->GetDeviceContext()->VSSetConstantBuffers(0, 2, vs_cbs);

	ID3D11ShaderResourceView* ps_srvs[1] = { texture };

	if (!m_Wireframe)
	{
		// Assign Texture
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 1, ps_srvs);

		SwitchTo("texturePixelShader", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}
	else
	{
		SwitchTo("TextureWireframePS", ZShadeSandboxShader::EShaderTypes::ST_PIXEL);
	}

	SwitchTo("textureVertexShader", ZShadeSandboxShader::EShaderTypes::ST_VERTEX);

	SetVertexShader();
	SetPixelShader();

	SetInputLayout("textureVertexShader");

	//Perform Drawing
	RenderIndex11(indexCount);

	// Unbind
	if (!m_Wireframe)
	{
		ps_srvs[0] = NULL;
		m_pD3DSystem->GetDeviceContext()->PSSetShaderResources(0, 1, ps_srvs);
	}

	return true;
}
//==============================================================================================================================