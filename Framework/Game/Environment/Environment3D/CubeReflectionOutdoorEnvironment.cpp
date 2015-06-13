#include "CubeReflectionOutdoorEnvironment.h"
#include "MaterialManager.h"
//===============================================================================================================================
//===============================================================================================================================
CubeReflectionOutdoorEnvironment::CubeReflectionOutdoorEnvironment(LPCSTR base_window_name, LPCSTR render_window_name, EngineOptions* eo, bool init_lua)
: OutdoorEnvironment(base_window_name, render_window_name, eo, init_lua)
{
	//Disable terrain
	bToggleTerrain = false;

	pDesaturationCS = new CSDesaturation(m_D3DSystem, "fiesta.bmp");
	
	ZShadeSandboxMesh::MeshParameters mp;
	mp.useCustomShader = false;
	mp.vertexType = ZShadeSandboxMesh::EVertexType::VT_Tex;
	mp.rotationAxisX = false;
	mp.rotationAxisY = false;
	mp.rotationAxisZ = false;
	mp.pos = XMFLOAT3(30, 50, 100);
	mp.rot = XMFLOAT3(0, 0, 0);
	mp.scale = XMFLOAT3(40, 40, 40);
	mp.material = MaterialManager::Instance()->GetMaterial("Wall");
	mp.in2D = false;
	mp.textureWidth = -1;
	mp.textureHeight = -1;
	mp.shader = 0;
	
	mCube = new ZShadeSandboxMesh::CubeMesh( m_D3DSystem, mp );
	
	//Do a Pre-Pass Render on the Compute Shader
	pDesaturationCS->RenderCS11( 32, 21, 1 );
	//Get the GPU Rendered Desaturated image
	mCube->RemoveTexture(0);
	mCube->SetMaterialDiffuseTexture(pDesaturationCS->GetGPUSRV());
}
//===============================================================================================================================
CubeReflectionOutdoorEnvironment::~CubeReflectionOutdoorEnvironment()
{
	Shutdown();
}
//===============================================================================================================================
bool CubeReflectionOutdoorEnvironment::Initialize()
{
	return true;
}
//===============================================================================================================================
void CubeReflectionOutdoorEnvironment::Shutdown()
{
	OutdoorEnvironment::Shutdown();
}
//===============================================================================================================================
void CubeReflectionOutdoorEnvironment::Update()
{
	mCube->SetWireframe( bWireframeMode );
	mCube->Update( m_Timer.DeltaSecondsF() );
}
//===============================================================================================================================
void CubeReflectionOutdoorEnvironment::Render()
{
	//Quick wireframe mode
	if( Quickwire() )
	{
		mCube->SetWireframe( true );
		m_D3DSystem->TurnOnWireframe();
	}

	//
	// Render the cube with defaults
	//
	
	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.camera = m_CameraSystem.get();
	mrp.light = mDirLight1;
	mCube->Render(mrp);
}
//===============================================================================================================================
void CubeReflectionOutdoorEnvironment::RenderDeferred()
{

}
//===============================================================================================================================
void CubeReflectionOutdoorEnvironment::RenderRefraction(XMFLOAT4 clipplane)
{
	//
	// Render the cube
	//
	
	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.camera = m_CameraSystem.get();
	mrp.light = mDirLight1;
	mrp.clipplane = clipplane; // Using normal view but with a clip plane for refraction
	mCube->Render(mrp);
}
//===============================================================================================================================
void CubeReflectionOutdoorEnvironment::RenderReflection(XMFLOAT4 clipplane)
{
	//
	// Render the cube
	//
	
	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.camera = m_CameraSystem.get();
	mrp.light = mDirLight1;
	mrp.reflection = true;
	mrp.clipplane = clipplane;
	mCube->Render(mrp);
}
//===============================================================================================================================