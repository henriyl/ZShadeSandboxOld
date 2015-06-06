#include "ModelEnvironment.h"
#include "LightManager.h"
#include "CapsuleLight.h"
#include "AmbientLight.h"
#include "LightShadingBuffers.h"
#include "MaterialManager.h"
//===============================================================================================================================
//===============================================================================================================================
ModelEnvironment::ModelEnvironment(LPCSTR base_window_name, LPCSTR render_window_name, EngineOptions* eo, bool init_lua)
:   Environment3D(base_window_name, render_window_name, eo, init_lua)
{
	m_CameraSystem->SetPosition(0.0f, 40.0f, 10.0f);
	
	bEnableShadows = true;

	bEnableDeferredShading = true;
	bUseDirectionalLightDeferred = true;
	
	// Not using any reflections
	m_CameraSystem->SetRenderReflectionView( false );
	
	// Create a random plane
	ZShadeSandboxMesh::MeshParameters mp;
	mp.useCustomShader = false;
	mp.vertexType = ZShadeSandboxMesh::EVertexType::VT_NormalTex;
	mp.rotationAxisX = false;
	mp.rotationAxisY = false;
	mp.rotationAxisZ = false;
	mp.pos = XMFLOAT3(0, 0, 0);
	mp.rot = XMFLOAT3(0, 0, 0);
	mp.scale = XMFLOAT3(1, 1, 1);
	mp.shader = 0;
	
	//mp.pos = XMFLOAT3(0, 1, 0);
	mp.scale = XMFLOAT3(20, 1, 20);
	mp.material = MaterialManager::Instance()->GetMaterial("Metal");
	//mPlane = new ZShadeSandboxMesh::PlaneMesh(200.0f, 0.0f, m_D3DSystem, mp, true);
	mPlane = new ZShadeSandboxMesh::PlaneMesh(m_D3DSystem, mp, "Models\\plane01.txt");
	
	//mp.pos = XMFLOAT3(2, 2, 0);
	mp.material = MaterialManager::Instance()->GetMaterial("White");//Red
	mp.scale = XMFLOAT3(1, 1, 1);
	mp.pos = XMFLOAT3(0, 1, 0);
	mPickingSphere = new ZShadeSandboxMesh::SphereMesh(0.2f, 10, 10, m_D3DSystem, mp);

	float size = 10;
	float r = size / 100;
	//mp.pos = XMFLOAT3(4, 2, 0);
	mp.scale = XMFLOAT3(size, size, size);
	mp.pos = XMFLOAT3(50, 5, -45);
	mp.material = MaterialManager::Instance()->GetMaterial("Floor");//Floor
	mSphere = new ZShadeSandboxMesh::SphereMesh(r, size, size, m_D3DSystem, mp);
	
	//mp.pos = XMFLOAT3(0, 2, 2);
	mp.pos = XMFLOAT3(-45, 5, -30);
	mp.material = MaterialManager::Instance()->GetMaterial("Ice");//Ice
	mGeosphereMesh = new ZShadeSandboxMesh::GeosphereMesh(0.5f, 2, m_D3DSystem, mp);

	//mp.pos = XMFLOAT3(4, 2, 2);
	mp.pos = XMFLOAT3(30, 2, 60);
	mp.scale = XMFLOAT3(1, 1, 1);
	mp.material = MaterialManager::Instance()->GetMaterial("White");
	mCylindricalMesh = new ZShadeSandboxMesh::CylindricalMesh(0.5f, 0.5f, 3.0f, 10, 10, m_D3DSystem, mp);

	//mp.pos = XMFLOAT3(-2, 2, 0);
	mp.pos = XMFLOAT3(-50, 5, 40);
	mp.scale = XMFLOAT3(5, 5, 5);
	mp.material = MaterialManager::Instance()->GetMaterial("Wall");//Wall
	ZShadeSandboxMesh::CubeMesh* cubeMesh = new ZShadeSandboxMesh::CubeMesh(m_D3DSystem, mp, "Models\\cube.txt");

	//mp.pos = XMFLOAT3(1, 1, 0);
	//mp.material = MaterialManager::Instance()->GetMaterial("Floor");
	//mCircleMesh = new ZShadeSandboxMesh::CircleMesh(m_D3DSystem, 0.5f, mp);

	//m_SpawnedMeshContainer.push_back(mSphere);
	m_SpawnedMeshContainer.push_back(mGeosphereMesh);
	//m_SpawnedMeshContainer.push_back(mCylindricalMesh);
	//m_SpawnedMeshContainer.push_back(mCircleMesh);
	m_SpawnedMeshContainer.push_back(cubeMesh);
	
	// Enable the capsule lights for this scene
	//ZShadeSandboxLighting::LightManager::Instance()->ToggleCapsuleLights(true);
	
	mSpaceCompound = new ZShadeSandboxMesh::OBJMesh(m_D3DSystem, m_GameDirectory3D);
	mSpaceCompound->Load("Models//spaceCompound.obj", false, true);
	mSpaceCompound->Scale() = XMFLOAT3(0.3f, 0.3f, 0.3f);
	mSpaceCompound->Position() = XMFLOAT3(100, 15, 100);
	
	mHuman = new ZShadeSandboxMesh::OBJMesh(m_D3DSystem, m_GameDirectory3D);
	mHuman->Load("Models//MaleLow.obj", false, true);
	mHuman->Scale() = XMFLOAT3(0.3f, 0.3f, 0.3f);
	mHuman->Position() = XMFLOAT3(50, 15, 50);

	mSky = new Sky(m_D3DSystem, m_GameDirectory3D->m_textures_path, "sky_cube.dds", 1000.0f);
}
//===============================================================================================================================
ModelEnvironment::~ModelEnvironment()
{
	Shutdown();
}
//===============================================================================================================================
void ModelEnvironment::RenderShadowMap()
{
	mDirLight1->Update();
	
	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.pCamera = m_CameraSystem.get();
	mrp.pLightCamera = mDirLight1->Perspective();
	mrp.bShadowMap = true;

	mPlane->Render(mrp);

	vector<ZShadeSandboxMesh::CustomMesh*>::iterator it = m_SpawnedMeshContainer.begin();
	for (; it != m_SpawnedMeshContainer.end(); it++)
	{
		if ((*it)->MeshType() == ZShadeSandboxMesh::EMeshType::CYLINDER)
		{
			if (!bWireframeMode && !Quickwire())
				m_D3DSystem->TurnOffCulling();
		}

		(*it)->Render(mrp);

		if ((*it)->MeshType() == ZShadeSandboxMesh::EMeshType::CYLINDER)
		{
			if (!bWireframeMode && !Quickwire())
				m_D3DSystem->TurnOnCulling();
		}
	}

	mPickingSphere->Render(mrp);
}
//===============================================================================================================================
void ModelEnvironment::RenderDeferred()
{
	if (!bWireframeMode)
	{
		m_D3DSystem->TurnOnCulling();
	}

	//Quick wire mode to view the objects underlying triangles quickly
	if (Quickwire())
	{
		mPlane->SetWireframe(true);

		vector<ZShadeSandboxMesh::CustomMesh*>::iterator it = m_SpawnedMeshContainer.begin();
		for (; it != m_SpawnedMeshContainer.end(); it++)
		{
			(*it)->SetWireframe(true);
		}

		// Uncomment if using the picking sphere
		mPickingSphere->SetWireframe(true);

		mSpaceCompound->SetWireframe(true);
		mHuman->SetWireframe(true);

		m_D3DSystem->TurnOnWireframe();
	}

	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.pCamera = m_CameraSystem.get();
	mrp.bRenderDeferred = true;
	
	mPlane->Render(mrp);

	vector<ZShadeSandboxMesh::CustomMesh*>::iterator it = m_SpawnedMeshContainer.begin();
	for (; it != m_SpawnedMeshContainer.end(); it++)
	{
		if ((*it)->MeshType() == ZShadeSandboxMesh::EMeshType::CYLINDER)
		{
			if (!bWireframeMode && !Quickwire())
				m_D3DSystem->TurnOffCulling();
		}

		(*it)->Render(mrp);

		if ((*it)->MeshType() == ZShadeSandboxMesh::EMeshType::CYLINDER)
		{
			if (!bWireframeMode && !Quickwire())
				m_D3DSystem->TurnOnCulling();
		}
	}

	mPickingSphere->Render(mrp);
	
	// Render deferred shading on obj mesh
	ZShadeSandboxMesh::OBJMeshRenderParameters omrp;
	omrp.camera = m_CameraSystem.get();
	omrp.renderType = ZShadeSandboxMesh::ERenderType::eTriangleList;
	omrp.bRenderDeferred = true;
	mSpaceCompound->Render(omrp);
	mHuman->Render(omrp);

	bool toggleMesh = true;
	bool reflect = false;
	bool renderDeferred = true;
	ZShadeSandboxLighting::LightManager::Instance()->RenderLightMesh(m_CameraSystem.get(), mDirLight1->Perspective(), toggleMesh, reflect, bWireframeMode, renderDeferred);

	//
	//Render the sky
	//

	if (!Quickwire() && !bWireframeMode)
	{
		m_D3DSystem->TurnOffCulling();
		mSky->Render(m_D3DSystem, m_CameraSystem.get(), true);
		m_D3DSystem->TurnOnZBuffer();
	}

	m_D3DSystem->TurnOffCulling();
}
//===============================================================================================================================
bool ModelEnvironment::Initialize()
{
	return true;
}
//===============================================================================================================================
void ModelEnvironment::Shutdown()
{
	Environment3D::Shutdown();
}
//===============================================================================================================================
void ModelEnvironment::Update()
{
	// Need to get toggle update time based on the amount of triangles rendered (This needs to be fixed)
	float dt = fFrameTime;
	
	mPlane->SetWireframe(bWireframeMode);
	
	// Uncomment if using the picking sphere
	mPickingSphere->SetWireframe(bWireframeMode);
	
	vector<ZShadeSandboxMesh::CustomMesh*>::iterator it = m_SpawnedMeshContainer.begin();
	for (; it != m_SpawnedMeshContainer.end(); it++)
	{
		(*it)->Update(dt);
		(*it)->SetWireframe(bWireframeMode);
	}
	
	//XMFLOAT3 eye = m_CameraSystem->Position();
	// Set the capsule light to the camera's position so things can be seen
	//mCapsuleLight->UpdateMeshPosition(XMFLOAT3(eye.x, eye.y, eye.z));
	
	//static float lightAngle = 270.0f;
	//float radians;
	//static float lightPosX = 9.0f;

	//// Update the position of the light each frame.
	//lightPosX -= 0.003f;// *mFrameTime;

	//// Update the angle of the light each frame.
	//lightAngle -= 0.03f;// *mFrameTime;
	//if (lightAngle < 90.0f)
	//{
	//	lightAngle = 270.0f;

	//	// Reset the light position also.
	//	lightPosX = 9.0f;
	//}
	//radians = lightAngle * 0.0174532925f;

	//// Update the direction of the light.
	//mDirLight1->Direction() = XMFLOAT3(sinf(radians), cosf(radians), 0.0f);

	//// Set the position and lookat for the light.
	//mDirLight1->Position() = XMFLOAT3(lightPosX, 8.0f, -0.1f);
	//mDirLight1->Perspective()->LookAt() = XMFLOAT3(-lightPosX, 0.0f, 0.0f);

	/*XMFLOAT3 dir = mDirLight1->Direction();
	static float lightDirX = dir.x;
	lightDirX += 0.0005f;
	if (lightDirX > (0.57735f + 5.0f))
	{
		lightDirX = 0.57735f;
	}
	mDirLight1->Direction() = XMFLOAT3(lightDirX, dir.y, dir.z);*/

	XMFLOAT3 eye = m_CameraSystem->Position();
	XMFLOAT3 DirLightPos = mDirLight1->Position();
	
	if (keyboard->IsKeyDown(Keyboard::Key::Up))
	{
		mDirLight1->Direction().x += 0.01f;
		mDirLight1->UpdateMeshPosition(XMFLOAT3(DirLightPos.x + 0.01f, DirLightPos.y, DirLightPos.z));
	}
	if (keyboard->IsKeyDown(Keyboard::Key::Down))
	{
		mDirLight1->Direction().x -= 0.01f;
		mDirLight1->UpdateMeshPosition(XMFLOAT3(DirLightPos.x - 0.01f, DirLightPos.y, DirLightPos.z));
	}
	if (keyboard->IsKeyDown(Keyboard::Key::Left))
	{
		mDirLight1->Direction().z += 0.01f;
		mDirLight1->UpdateMeshPosition(XMFLOAT3(DirLightPos.x, DirLightPos.y, DirLightPos.z + 0.01f));
	}
	if (keyboard->IsKeyDown(Keyboard::Key::Right))
	{
		mDirLight1->Direction().x -= 0.01f;
		mDirLight1->UpdateMeshPosition(XMFLOAT3(DirLightPos.x, DirLightPos.y, DirLightPos.z - 0.01f));
	}
	if (keyboard->IsKeyDown(Keyboard::Key::E))
	{
		mDirLight1->Direction().y += 0.01f;
		mDirLight1->UpdateMeshPosition(XMFLOAT3(DirLightPos.x, DirLightPos.y + 0.01f, DirLightPos.z));
	}
	if (keyboard->IsKeyDown(Keyboard::Key::R))
	{
		mDirLight1->Direction().y -= 0.01f;
		mDirLight1->UpdateMeshPosition(XMFLOAT3(DirLightPos.x, DirLightPos.y - 0.01f, DirLightPos.z));
	}

	mSpaceCompound->SetWireframe(bWireframeMode);
	mHuman->SetWireframe(bWireframeMode);
}
//===============================================================================================================================
void ModelEnvironment::Render()
{
	// If there are any lights in the scene capture them
	ZShadeSandboxLighting::LightManager::Instance()->RebuildLightBuffer(mAmbientUp, mAmbientDown);
	ZShadeSandboxLighting::LightManager::Instance()->RebuildSunBuffer(*mSunLightBuffer);

	// Add the light buffers to the mesh system
	mPlane->SetLightBuffer(ZShadeSandboxLighting::LightManager::Instance()->GetLightBuffer());
	mPlane->SetLightBuffer(ZShadeSandboxLighting::LightManager::Instance()->GetSunLightBuffer());

	vector<ZShadeSandboxMesh::CustomMesh*>::iterator it = m_SpawnedMeshContainer.begin();
	for (; it != m_SpawnedMeshContainer.end(); it++)
	{
		(*it)->SetLightBuffer(ZShadeSandboxLighting::LightManager::Instance()->GetLightBuffer());
		(*it)->SetLightBuffer(ZShadeSandboxLighting::LightManager::Instance()->GetSunLightBuffer());
	}

	mPickingSphere->SetLightBuffer(ZShadeSandboxLighting::LightManager::Instance()->GetLightBuffer());
	mPickingSphere->SetLightBuffer(ZShadeSandboxLighting::LightManager::Instance()->GetSunLightBuffer());

	// Mouse Picking
	XMMATRIX world = XMMatrixIdentity();
	mPickingRay = m_CameraSystem->PickingRay(mLastMousePos.x, mLastMousePos.y, world);
	if (mPickingRay != NULL)
	{
		if (bLeftMouseDown)
		{
			bLeftMouseDown = false;
			bool hit = false;
			XMFLOAT3 hitPoint;
			ZShadeSandboxMath::Ray ray = *mPickingRay;
			// Picking Test goes here
			if (hit)
			{
				// Place the sphere at the point of intersection
				mPickingSphere->Position() = hitPoint;
			}
		}
	}
	
	if (!bWireframeMode)
	{
		m_D3DSystem->TurnOnCulling();
	}
	
	//Quick wire mode to view the objects underlying triangles quickly
	if( Quickwire() )
	{
		mPlane->SetWireframe(true);
		
		vector<ZShadeSandboxMesh::CustomMesh*>::iterator it = m_SpawnedMeshContainer.begin();
		for (; it != m_SpawnedMeshContainer.end(); it++)
		{
			(*it)->SetWireframe(true);
		}
		
		// Uncomment if using the picking sphere
		mPickingSphere->SetWireframe(true);
		
		mSpaceCompound->SetWireframe(true);
		mHuman->SetWireframe(true);
		
		m_D3DSystem->TurnOnWireframe();
	}
	
	RenderScene();
	
	m_D3DSystem->TurnOffCulling();
}
//===============================================================================================================================
bool ModelEnvironment::RenderScene()
{
	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.pCamera = m_CameraSystem.get();
	mrp.pLightCamera = mDirLight1->Perspective();
	mrp.dirLight = mDirLight1;
	
	vector<ZShadeSandboxMesh::CustomMesh*>::iterator it = m_SpawnedMeshContainer.begin();
	for (; it != m_SpawnedMeshContainer.end(); it++)
	{
		if ((*it)->MeshType() == ZShadeSandboxMesh::EMeshType::CYLINDER)
		{
			if (!bWireframeMode && !Quickwire())
				m_D3DSystem->TurnOffCulling();
		}

		(*it)->EnableShadowMap(bEnableShadows);
		(*it)->SetShadowMapSRV(mShadowTexture->SRView);
		(*it)->SetSSAOMapSRV(0);
		(*it)->Render(mrp);

		if ((*it)->MeshType() == ZShadeSandboxMesh::EMeshType::CYLINDER)
		{
			if (!bWireframeMode && !Quickwire())
				m_D3DSystem->TurnOnCulling();
		}
	}
	
	// Render the plane in tessellation mode
	//m_D3DSystem->TurnOnWireframe();
	//mPlane->SetWireframe(true);
	//mrp.fTessellationFactor = m_TessFactor;
	//mrp.bTessellate = true;
	//mrp.renderType = ZShadeSandboxMesh::ERenderType::e3ControlPointPatchList;
	mPlane->EnableShadowMap(bEnableShadows);
	mPlane->SetShadowMapSRV(mShadowTexture->SRView);
	mPlane->SetSSAOMapSRV(0);
	mPlane->Render(mrp);
	
	// Uncomment if using the picking sphere
	//mrp.bTessellate = false;
	//mrp.rt = RenderType::eTriangleList;
	mPickingSphere->EnableShadowMap(bEnableShadows);
	mPickingSphere->SetShadowMapSRV(mShadowTexture->SRView);
	mPickingSphere->SetSSAOMapSRV(0);
	mPickingSphere->Render(mrp);
	
	ZShadeSandboxMesh::OBJMeshRenderParameters omrp;
	omrp.camera = m_CameraSystem.get();
	omrp.renderType = ZShadeSandboxMesh::ERenderType::eTriangleList;
	omrp.tessellate = false;
	mSpaceCompound->Render(omrp);
	mHuman->Render(omrp);
	
	bool toggleMesh = true;
	bool reflect = false;
	bool renderDeferred = false;
	ZShadeSandboxLighting::LightManager::Instance()->RenderLightMesh(m_CameraSystem.get(), mDirLight1->Perspective(), toggleMesh, reflect, bWireframeMode, renderDeferred);

	//
	//Render the sky
	//

	if (!Quickwire() && !bWireframeMode)
	{
		m_D3DSystem->TurnOffCulling();

		mSky->Render(m_D3DSystem, m_CameraSystem.get(), false);

		m_D3DSystem->TurnOnZBuffer();
	}

	return true;
}
//===============================================================================================================================