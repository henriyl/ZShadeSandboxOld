#include "BlurredEnvironment.h"
#include "ZMath.h"
#include "MaterialManager.h"
//===============================================================================================================================
//===============================================================================================================================
BlurredEnvironment::BlurredEnvironment(LPCSTR base_window_name, LPCSTR render_window_name, EngineOptions* eo, bool init_lua)
:   Environment3D(base_window_name, render_window_name, eo, init_lua)
,	m_pBlurShader(0)
{
	//m_PostProcessEnable = false;

	m_CameraSystem->CreateOrtho(eo->m_screenWidth, eo->m_screenHeight, 0.1f, 1000.0f);

	//This is blurring using ortho
	bCameraStill = true;

	m_CameraSystem->SetPosition(0.0f, 0.0f, -10.0f);

	//Set the size to sample down to
	int downSampleWidth = eo->m_screenWidth / 2;
	int downSampleHeight = eo->m_screenHeight / 2;

	m_CameraSystem->SetRenderReflectionView(false);
	
	// Initialize the parameters needed for any mesh
	ZShadeSandboxMesh::MeshParameters mp;
	mp.vertexType = ZShadeSandboxMesh::EVertexType::VT_Tex;
	mp.rotationAxisX = false;
	mp.rotationAxisY = false;
	mp.rotationAxisZ = false;
	mp.pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	mp.rot = XMFLOAT3(0.0f, 0.0f, 0.0f);
	mp.scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	mp.useCustomShader = false;
	mp.material = MaterialManager::Instance()->GetMaterial("Wall");
	mp.textureWidth = -1;  // Not used in Texture class
	mp.textureHeight = -1; // Not used in Texture class
	mp.in2D = false;
	mp.shader = 0;
	
	//Create the texture shader
	m_pTextureShader = new TextureShader(m_D3DSystem);

	//Create the blur shader
	m_pBlurShader = new BlurShader(m_D3DSystem);

	//m_pSimplePP = new SimplePP(mD3DSystem);

	//Create the render to texture
	m_pRenderTexture = new RenderTarget2D(m_D3DSystem);
	m_pRenderTexture->Initialize(eo->m_screenWidth, eo->m_screenHeight, DXGI_FORMAT_R32G32B32A32_FLOAT);

	//Create the down sample render to texture
	m_pDownSampleTexture = new RenderTarget2D(m_D3DSystem);
	m_pDownSampleTexture->Initialize(downSampleWidth, downSampleHeight, DXGI_FORMAT_R32G32B32A32_FLOAT);

	//Create the horizontal blur render to texture
	m_pHorizontalBlurTexture = new RenderTarget2D(m_D3DSystem);
	m_pHorizontalBlurTexture->Initialize(downSampleWidth, downSampleHeight, DXGI_FORMAT_R32G32B32A32_FLOAT);

	//Create the vertical blur render to texture
	m_pVerticalBlurTexture = new RenderTarget2D(m_D3DSystem);
	m_pVerticalBlurTexture->Initialize(downSampleWidth, downSampleHeight, DXGI_FORMAT_R32G32B32A32_FLOAT);

	//Create the up sample render to texture
	m_pUpSampleTexture = new RenderTarget2D(m_D3DSystem);
	m_pUpSampleTexture->Initialize(eo->m_screenWidth, eo->m_screenHeight, DXGI_FORMAT_R32G32B32A32_FLOAT);

	//Create a small window mesh
	mp.textureWidth = downSampleWidth;
	mp.textureHeight = downSampleHeight;
	m_pSmallWindow = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp, false);
	//m_pSmallWindow->Initialize(downSampleWidth, downSampleHeight);

	//Create a large window mesh
	mp.textureWidth = eo->m_screenWidth;
	mp.textureHeight = eo->m_screenHeight;
	m_pFullscreenWidow = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp, false);
	//m_pFullscreenWidow->Initialize(eo->m_screenWidth, eo->m_screenHeight);

	//Create the cube mesh

	mp.textureWidth = -1;  // Not used in Texture class
	mp.textureHeight = -1; // Not used in Texture class
	
	//mp.textureName = "bricks.dds";
	mp.pos = XMFLOAT3(60.0f, 10.0f, 200.0f);
	mp.scale = XMFLOAT3(40.0f, 40.0f, 40.0f);
	m_pCube = new ZShadeSandboxMesh::CubeMesh(m_D3DSystem, mp);
	
	mp.pos = XMFLOAT3(-100.0f, 10.0f, 800.0f);
	m_pNoBlurCube = new ZShadeSandboxMesh::CubeMesh(m_D3DSystem, mp);
	
	mp.pos = XMFLOAT3(60.0f, 0.0f, 200.0f);
	m_pBlurredCube = new ZShadeSandboxMesh::CubeMesh(m_D3DSystem, mp);
}
//===============================================================================================================================
BlurredEnvironment::~BlurredEnvironment()
{
	Shutdown();
}
//===============================================================================================================================
bool BlurredEnvironment::Initialize()
{
	return true;
}
//===============================================================================================================================
void BlurredEnvironment::Shutdown()
{
	Environment3D::Shutdown();
}
//===============================================================================================================================
void BlurredEnvironment::Update()
{
	m_pCube->Update( m_Timer.DeltaSecondsF() );
	m_pCube->SetWireframe( bWireframeMode );
	//m_pNoBlurCube->Update( m_timer->DeltaSecondsF() );
	m_pNoBlurCube->SetWireframe( bWireframeMode );
}
//===============================================================================================================================
void BlurredEnvironment::Render()
{
	if( Quickwire() )
	{
		m_pCube->SetWireframe( true );
		m_pNoBlurCube->SetWireframe( true );
		m_D3DSystem->TurnOnWireframe();
	}

	m_D3DSystem->TurnOnZBuffer();
	
	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.camera = m_CameraSystem.get();
	m_pCube->Render(mrp);
	//m_pNoBlurCube->Render(mCamera, TriangleList, false, true);

	/*if (m_VertextProcessEnable)
	{
		ID3D11ShaderResourceView* pInputSRV = mD3DSystem->GetColorRenderTarget()->GetSRV();
		DepthStencilBuffer* pDepth = mD3DSystem->GetDepthStencilBuffer();
		ID3D11RenderTargetView* pOutputRTV = mD3DSystem->GetBackBufferRTV();

		m_pSimplePP->Render(pInputSRV, pOutputRTV);

		Reset the backbuffer and viewports after Vertext processing and turn the z buffer back on
		mD3DSystem->SetBackBufferRenderTarget(m_VertextProcessEnable);
		mD3DSystem->ResetViewport();
		mD3DSystem->TurnOnZBuffer();
	}*/

	RenderSceneToTexture();
	DownSampleTexture();
	RenderHorizontalBlurToTexture();
	RenderVerticalBlurToTexture();
	UpSampleTexture();
	Render2DTextureScene();
}
//===============================================================================================================================
void BlurredEnvironment::RenderDeferred()
{

}
//===============================================================================================================================
bool BlurredEnvironment::RenderSceneToTexture()
{
	// Set the render target to be the render to texture.
	m_pRenderTexture->SetRenderTarget();

	// Clear the render to texture.
	m_pRenderTexture->ClearRenderTarget(0.1f,0.2f,0.4f,0.0f);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	//m_pCube->Render(mCamera);
	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.camera = m_CameraSystem.get();
	m_pCube->Render(mrp);

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_D3DSystem->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_D3DSystem->ResetViewport();

	return true;
}
//===============================================================================================================================
bool BlurredEnvironment::DownSampleTexture()
{
	// Set the render target to be the render to texture.
	m_pDownSampleTexture->SetRenderTarget();

	// Clear the render to texture.
	m_pDownSampleTexture->ClearRenderTarget(0.1f,0.2f,0.4f,0.0f);

	XMMATRIX ortho;
	//Get the ortho matrix
	m_pDownSampleTexture->GetOrthoMatrix(ortho);

	//Turn off the z buffer for 2D rendering
	m_D3DSystem->TurnOffZBuffer();

	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.camera = m_CameraSystem.get();
	m_pSmallWindow->Render(mrp);

	//Render the shader
	m_pTextureShader->SetWireframe( false );
	m_pTextureShader->UseOrtho(true);
	m_pTextureShader->SetOrtho(ortho);
	//m_pTextureShader->Render11(m_pSmallWindow->GetIndexCount(), ZShadeSandboxMath::XMMath4(0, 0, 0, 0),
	//	m_CameraSystem.get(), m_pRenderTexture->SRView);

	//m_pTextureShader->SetWorld( XMMatrixIdentity() );
	//m_pTextureShader->SetView( mCamera->View() );
	//m_pTextureShader->SetProj( MathHelper::ConvertToXMMatrix(ortho) );
	//m_pTextureShader->SetTexture( m_pRenderTexture->GetSRV() );
	//XMFLOAT4 plane = XMFLOAT4(0,0,0,0);
	//m_pTextureShader->SetClipPlane(plane);
	//m_pTextureShader->Render11(mD3DSystem, m_pSmallWindow->GetIndexCount());

	//2D render is complete
	m_D3DSystem->TurnOnZBuffer();
	
	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_D3DSystem->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_D3DSystem->ResetViewport();

	return true;
}
//===============================================================================================================================
bool BlurredEnvironment::RenderHorizontalBlurToTexture()
{
	// Set the render target to be the render to texture.
	m_pHorizontalBlurTexture->SetRenderTarget();

	// Clear the render to texture.
	m_pHorizontalBlurTexture->ClearRenderTarget(0.1f,0.2f,0.4f,0.0f);

	XMMATRIX ortho;
	//Get the ortho matrix
	m_pHorizontalBlurTexture->GetOrthoMatrix(ortho);

	//Turn off the z buffer for 2D rendering
	m_D3DSystem->TurnOffZBuffer();

	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.camera = m_CameraSystem.get();
	m_pSmallWindow->Render(mrp);

	//Render the horizontal blur
	m_pBlurShader->SetHorizontalBlur( true );
	m_pBlurShader->UseOrtho(true);
	m_pBlurShader->SetOrtho(ortho);
	m_pBlurShader->Render11(m_pSmallWindow->GetIndexCount(), m_CameraSystem.get(),
		m_D3DSystem->GetEngineOptions()->m_screenWidth,
		m_D3DSystem->GetEngineOptions()->m_screenHeight,
		m_pDownSampleTexture->SRView, 0);

	/*m_pBlurShader->SetScreenWidth( m_pHorizontalBlurTexture->GetTextureWidth() );
	m_pBlurShader->SetWorld( XMMatrixIdentity() );
	m_pBlurShader->SetView( mCamera->View() );
	m_pBlurShader->SetProj( MathHelper::ConvertToXMMatrix(ortho) );
	m_pBlurShader->SetHorizontalBlurTexture( m_pDownSampleTexture->GetSRV() );*/
	//m_pBlurShader->Render11(mD3DSystem, m_pSmallWindow->GetIndexCount());

	//2D render is complete
	m_D3DSystem->TurnOnZBuffer();
	
	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_D3DSystem->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_D3DSystem->ResetViewport();

	return true;
}
//===============================================================================================================================
bool BlurredEnvironment::RenderVerticalBlurToTexture()
{
	// Set the render target to be the render to texture.
	m_pVerticalBlurTexture->SetRenderTarget();

	// Clear the render to texture.
	m_pVerticalBlurTexture->ClearRenderTarget(0.1f,0.2f,0.4f,0.0f);

	XMMATRIX ortho;
	//Get the ortho matrix
	m_pVerticalBlurTexture->GetOrthoMatrix(ortho);

	//Turn off the z buffer for 2D rendering
	m_D3DSystem->TurnOffZBuffer();

	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.camera = m_CameraSystem.get();
	m_pSmallWindow->Render(mrp);

	//Render the vertical blur
	m_pBlurShader->SetHorizontalBlur( false );
	m_pBlurShader->UseOrtho(true);
	m_pBlurShader->SetOrtho(ortho);
	m_pBlurShader->Render11(m_pSmallWindow->GetIndexCount(), m_CameraSystem.get(),
		m_D3DSystem->GetEngineOptions()->m_screenWidth,
		m_D3DSystem->GetEngineOptions()->m_screenHeight,
		0, m_pHorizontalBlurTexture->SRView);

	/*m_pBlurShader->SetScreenHeight( m_pVerticalBlurTexture->GetTextureHeight() );
	m_pBlurShader->SetWorld( XMMatrixIdentity() );
	m_pBlurShader->SetView( mCamera->View() );
	m_pBlurShader->SetProj( MathHelper::ConvertToXMMatrix(ortho) );
	m_pBlurShader->SetVerticalBlurTexture( m_pHorizontalBlurTexture->GetSRV() );*/
	//m_pBlurShader->Render11(mD3DSystem, m_pSmallWindow->GetIndexCount());

	//2D render is complete
	m_D3DSystem->TurnOnZBuffer();
	
	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_D3DSystem->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_D3DSystem->ResetViewport();

	return true;
}
//===============================================================================================================================
bool BlurredEnvironment::UpSampleTexture()
{
	// Set the render target to be the render to texture.
	m_pUpSampleTexture->SetRenderTarget();

	// Clear the render to texture.
	m_pUpSampleTexture->ClearRenderTarget(0.1f,0.2f,0.4f,0.0f);

	XMMATRIX ortho;
	//Get the ortho matrix
	m_pUpSampleTexture->GetOrthoMatrix(ortho);

	//Turn off the z buffer for 2D rendering
	m_D3DSystem->TurnOffZBuffer();

	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.camera = m_CameraSystem.get();
	m_pFullscreenWidow->Render(mrp);

	//Render the shader
	m_pTextureShader->SetWireframe( false );
	m_pTextureShader->UseOrtho(true);
	m_pTextureShader->SetOrtho(ortho);
	//m_pTextureShader->Render11(m_pFullscreenWidow->GetIndexCount(), ZShadeSandboxMath::XMMath4(0, 0, 0, 0),
	//	m_CameraSystem.get(), m_pVerticalBlurTexture->SRView);

	/*m_pTextureShader->SetWorld( XMMatrixIdentity() );
	m_pTextureShader->SetView( mCamera->View() );
	m_pTextureShader->SetProj( MathHelper::ConvertToXMMatrix(ortho) );
	m_pTextureShader->SetTexture( m_pVerticalBlurTexture->GetSRV() );
	XMFLOAT4 plane = XMFLOAT4(0,0,0,0);
	m_pTextureShader->SetClipPlane(plane);*/
	//m_pTextureShader->Render11(mD3DSystem, m_pFullscreenWidow->GetIndexCount());

	//2D render is complete
	m_D3DSystem->TurnOnZBuffer();
	
	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_D3DSystem->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_D3DSystem->ResetViewport();

	return true;
}
//===============================================================================================================================
bool BlurredEnvironment::Render2DTextureScene()
{
	//Turn off the z buffer for 2D rendering
	m_D3DSystem->TurnOffZBuffer();

	//XMMATRIX ortho;
	//Get the ortho matrix
	//mD3DSystem->GetOrthoMatrix(ortho);

	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.camera = m_CameraSystem.get();

	m_pFullscreenWidow->Render(mrp);
	
	//Render the shader
	m_pTextureShader->SetWireframe( false );
	m_pTextureShader->UseOrtho(true);
	m_pTextureShader->SetOrtho(m_CameraSystem.get()->Ortho());
	//m_pTextureShader->Render11(m_pFullscreenWidow->GetIndexCount(), ZShadeSandboxMath::XMMath4(0, 0, 0, 0),
	//	m_CameraSystem.get(), m_pUpSampleTexture->SRView);

	/*m_pTextureShader->SetWorld( XMMatrixIdentity() );
	m_pTextureShader->SetView( mCamera->View() );
	m_pTextureShader->SetProj( ortho );
	m_pTextureShader->SetTexture( m_pUpSampleTexture->GetSRV() );
	XMFLOAT4 plane = XMFLOAT4(0,0,0,0);
	m_pTextureShader->SetClipPlane(plane);*/
	//m_pTextureShader->Render11(mD3DSystem, m_pFullscreenWidow->GetIndexCount());

	//2D render is complete
	m_D3DSystem->TurnOnZBuffer();

	//Render a cube that is not blurred
	m_pNoBlurCube->Render(mrp);

	return true;
}
//===============================================================================================================================
void BlurredEnvironment::RenderRefraction(XMFLOAT4 clipplane)
{
	
}
//===============================================================================================================================
void BlurredEnvironment::RenderReflection(XMFLOAT4 clipplane)
{
	
}
//===============================================================================================================================