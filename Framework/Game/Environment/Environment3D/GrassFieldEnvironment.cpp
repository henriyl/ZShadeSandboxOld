#include "GrassFieldEnvironment.h"
#include "TextureManager.h"
#include "MaterialManager.h"
//===============================================================================================================================
//===============================================================================================================================
GrassFieldEnvironment::GrassFieldEnvironment(LPCSTR base_window_name, LPCSTR render_window_name, EngineOptions* eo, bool init_lua)
:   OutdoorEnvironment(base_window_name, render_window_name, eo, init_lua)
{
	bToggleTerrain = false;
	bToggleWater = false;
	
	m_CameraSystem->SetPosition(0.0f, 0.0f, 100.0f);
	fCamSpeedFast = 0.06f;
	fCamSpeedSlow = 0.02f;

	int size = 512;

	//Create some terrain for the grass to sit on
	m_pTerrain = new SimpleTerrain( m_D3DSystem, size, true );
	m_pMaterialShader = new MaterialShader(m_D3DSystem);

	m_pGrassField = new GrassField(m_D3DSystem);
	//Initialize the grass with a field size of 20480
	m_pGrassField->Initialize( 20480, size, m_pTerrain->GetHeightmap() );

	//D3DX11CreateShaderResourceViewFromFile(mD3DSystem->GetDevice11(), "Textures\\darkgrass.bmp", 0, 0, &tex, 0);
}
//===============================================================================================================================
GrassFieldEnvironment::~GrassFieldEnvironment()
{
	Shutdown();
}
//===============================================================================================================================
bool GrassFieldEnvironment::Initialize()
{
	return true;
}
//===============================================================================================================================
void GrassFieldEnvironment::Shutdown()
{
	OutdoorEnvironment::Shutdown();
}
//===============================================================================================================================
void GrassFieldEnvironment::Update()
{
	OutdoorEnvironment::Update();

	//Other update info
	m_pGrassField->Update(m_Timer.ConstantTime());

	//m_pTextureShader->SetWireframe( m_wire );
	//m_pGFM->SetWireframe( m_wire );
	//m_pTriangleMesh->Update( mFrameTime );
}
//===============================================================================================================================
void GrassFieldEnvironment::Render()
{
	OutdoorEnvironment::Render();

	//Quick wireframe mode
	if( Quickwire() )
	{
		//m_pTextureShader->SetWireframe( true );
		m_D3DSystem->TurnOnWireframe();
	}

	m_pGrassField->Render(m_CameraSystem.get());
	
	XMFLOAT3 Vertex = XMFLOAT3(0.0, 0.0f, 0.0f);
	XMFLOAT3 rot = XMFLOAT3(0.0, 0.0f, 0.0f);
	XMFLOAT3 scale = XMFLOAT3(1.0, 1.0f, 1.0f);

	XMMATRIX scaleM, rx, ry, rz, translate;

	scaleM = XMMatrixScaling( scale.x, scale.y, scale.z );
	rx = XMMatrixRotationX( rot.x );
	ry = XMMatrixRotationY( rot.y );
	rz = XMMatrixRotationZ( rot.z );
	translate = XMMatrixTranslation( Vertex.x, Vertex.y, Vertex.z );

	XMMATRIX world = XMLoadFloat4x4( &mWorld );
	
	world = XMMatrixMultiply( world, scaleM );
	world = XMMatrixMultiply( world, rx );
	world = XMMatrixMultiply( world, ry );
	world = XMMatrixMultiply( world, rz );
	world = XMMatrixMultiply( world, translate );


	m_pTerrain->RenderBuffers();
	//m_pTextureShader->SetWorld( world );
	//m_pTextureShader->SetView( mCamera->View() );
	//m_pTextureShader->SetProj( mCamera->Proj() );
	//m_pTextureShader->SetTexture( tex );
	//m_pTextureShader->SetClipPlane( XMFLOAT4(0,0,0,0) );
	//m_pTextureShader->Render11(m_pTerrain->GetIndexCount(), mCamera);
	m_pMaterialShader->Render11(m_pTerrain->GetIndexCount(), ZShadeSandboxMath::XMMath4(0, 0, 0, 0), m_CameraSystem.get(), 0, MaterialManager::Instance()->GetMaterial("Soil"));
}
//===============================================================================================================================
void GrassFieldEnvironment::RenderDeferred()
{

}
//===============================================================================================================================
void GrassFieldEnvironment::RenderRefraction(XMFLOAT4 clipplane)
{
	m_D3DSystem->TurnOffCulling();
	//
	// Render the triangle
	//
	//m_pTriangleMesh->RenderClip(mCamera, mCamera->View(), clipplane);
	m_D3DSystem->TurnOnCulling();

	/*m_pTerrain->RenderBuffers();
	m_pTextureShader->SetWorld( WorldXM() );
	m_pTextureShader->SetView( mCamera->View() );
	m_pTextureShader->SetProj( mCamera->Proj() );
	m_pTextureShader->SetTexture( TextureManager::GetInstance()->GetTexture(mD3DSystem, "Textures\\soil.jpg") );
	m_pTextureShader->SetClipPlane( clipplane );
	m_pTextureShader->Render11(mD3DSystem, m_pTerrain->GetIndexCount());*/
}
//===============================================================================================================================
void GrassFieldEnvironment::RenderReflection(XMFLOAT4 clipplane)
{
	m_D3DSystem->TurnOffCulling();
	//
	// Render the triangle
	//
	//m_pTriangleMesh->RenderClip(mCamera, mCamera->ReflectionView(), clipplane);
	m_D3DSystem->TurnOnCulling();

	/*m_pTerrain->RenderBuffers();
	m_pTextureShader->SetWorld( WorldXM() );
	m_pTextureShader->SetView( mCamera->ReflectionView() );
	m_pTextureShader->SetProj( mCamera->Proj() );
	m_pTextureShader->SetTexture( TextureManager::GetInstance()->GetTexture(mD3DSystem, "Textures\\soil.jpg") );
	m_pTextureShader->SetClipPlane( clipplane );
	m_pTextureShader->Render11(mD3DSystem, m_pTerrain->GetIndexCount());*/
}
//===============================================================================================================================