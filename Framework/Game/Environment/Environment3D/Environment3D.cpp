#include "Exceptions.h"
#include "Environment3D.h"
#include "Convert.h"
#include "ZException.h"
#include "ShaderCompiler.h"
#include "LightManager.h"
#include "CapsuleLight.h"
#include "AmbientLight.h"
#include "MaterialManager.h"
#include "DefaultMaterials.h"
#include "TextureManager.h"
#include "ZShadeINIParser.h"
#include "HDR.h"
#include "PostProcessColor.h"
//===============================================================================================================================
//===============================================================================================================================
Environment3D::Environment3D(LPCSTR base_window_name, LPCSTR render_window_name, EngineOptions* eo, bool init_lua)
:   EnvironmentBase(eo, base_window_name, render_window_name)
{
	Init(init_lua);
}
//===============================================================================================================================
Environment3D::~Environment3D()
{
	Shutdown();
}
//===============================================================================================================================
bool Environment3D::Init(bool init_lua)
{
	mPickingSphere = NULL;
	iTriangleCount = 0;
	fTessFactor = 6.0f;
	fSeaLevel = 0.0f;
	fCamSpeedFast = 0.09f;
	fCamSpeedSlow = 0.05f;
	bWireframeMode = false;
	bEnableLighting = true;
	bCameraStill = false;
	bFullScreen = false;
	bEnableTessellation = true;
	bEnableReflections = false;
	bEnableRefractions = false;
	bEnableShadows = false;
	bSpawnCube = false;
	bEnableCulling = true;
	bLeftMouseDown = false;
	bEnablePostProcessing = false;
	bEnableDeferredShading = false;
	bUseDirectionalLightDeferred = false;
	bUsePointLightDeferred = false;
	bUseSpotLightDeferred = false;
	bUseCapsuleLightDeferred = false;
	
	m_D3DSystem = new D3D();
	if (!m_D3DSystem) return false;
	
	if (!m_D3DSystem->Initialize11(m_EngineOptions))
	{
		MessageBox(NULL, "Direct3D Init Failed", "Error", MB_OK);
		return false;
	}
	
	TextureManager::NewInstance(m_D3DSystem);
	
	// This should be false when a 3D game is being loaded
	bUseEngineTextures = true;

	// Load the Game Directory since the game is starting from here
	if (init_lua)
	{
		//Load the game directory
		m_GameDirectory3D = CGlobal::LoadGameDirectory3D(m_EngineOptions->m_GameFolderName);

		// Use the textures folder from development
		if (bUseEngineTextures)
		{
			m_GameDirectory3D->m_textures_path = "Textures";
		}

		m_EngineOptions->m_GD3D = m_GameDirectory3D;

		// Read the Game.ini file
		ZShadeINIParser iniParser(m_GameDirectory3D->m_game_ini_path, false);
		string gameType = iniParser.GetString("GAME", "GameType");

		if (BetterString(gameType) == "3D")
		{
			// This is what we want
		}

		// Create the default materials and add them to the material manager
		DefaultMaterials materials;
		materials.Build(m_D3DSystem, m_GameDirectory3D->m_textures_path);

		// NOTE: The audio and scripting system currently does not load from a 3D Game Directory

		// Initialize Audio System
		//AudioSystem::NewInstance(m_GameDirectory3D, m_RenderWindow->GetHwnd());

		// Initialize lua scripting system
		//Scripting::NewInstance(m_D3DSystem, m_GameDirectory3D);
	}
	else
	{
		// Probably loading from the editor
	}

	//Load the world matrix
	XMStoreFloat4x4(&mWorld, XMMatrixIdentity());
	
	m_Timer.Initialize();
	
	m_CameraSystem.reset(new Camera(m_EngineOptions));
	m_CameraSystem->SetLens(PI / 4, (float)m_EngineOptions->m_screenWidth/(float)m_EngineOptions->m_screenHeight,
		m_EngineOptions->fNearPlane, m_EngineOptions->fFarPlane);
	m_CameraSystem->SetPosition(0.0f, 0.0f, -10.0f);
	m_CameraSystem->SetWorld(XMMatrixIdentity());

	float xPos = 0.0f;
	float yPos = 10.0f;
	
	fShadowMapWidth = 2048.0f;
	fShadowMapNear = 0.1f;
	fShadowMapFar = 50.0f;

	mDirLight1 = new ZShadeSandboxLighting::DirectionalLight();
	mDirLight1->LightType() = ZShadeSandboxLighting::ELightType::eDirectional;
	mDirLight1->DiffuseColor() = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mDirLight1->AmbientColor() = XMFLOAT4(0.15f, 0.15f, 0.15f, 1.0f);
	mDirLight1->SpecularColor() = XMFLOAT4(0.8f, 0.8f, 0.7f, 1.0f);
	mDirLight1->Position() = XMFLOAT3(xPos, yPos, 0);
	//mDirLight1->Direction() = XMFLOAT3(0.0f, 1.0f, 0.0f);
	//mDirLight1->Direction() = XMFLOAT3(xPos, yPos, 0);
	//mDirLight1->Direction() = XMFLOAT3(0.57735f, -0.57735f, -0.57735f);
	mDirLight1->Direction() = XMFLOAT3(0.936016f, -0.343206f, 0.0780013f);
	//mDirLight1->Direction() = XMFLOAT3(0.37735f, -0.47735f, -0.57735f);
	//mDirLight1->Direction() = XMFLOAT3(0.707f, -0.707f, 0.0f);
	//mDirLight1->Direction() = XMFLOAT3(0.0f, 0.0f, 0.707f);
	mDirLight1->Perspective()->SceneWidth() = fShadowMapWidth;
	mDirLight1->Perspective()->SceneHeight() = fShadowMapWidth;
	mDirLight1->SetLens(fShadowMapNear, fShadowMapFar);

	xPos += 10;
	mDirLight2 = new ZShadeSandboxLighting::DirectionalLight();
	mDirLight2->LightType() = ZShadeSandboxLighting::ELightType::eDirectional;
	mDirLight2->DiffuseColor() = XMFLOAT4(0.2f, 0.2f, 1.2f, 1.0f);
	mDirLight2->AmbientColor() = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mDirLight2->SpecularColor() = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLight2->Position() = XMFLOAT3(xPos, yPos, 0);
	mDirLight2->Direction() = XMFLOAT3(0.575f, -0.735f, 0.5735f);
	mDirLight2->SetLens(m_EngineOptions->fNearPlane, m_EngineOptions->fFarPlane);

	xPos += 10;
	mDirLight3 = new ZShadeSandboxLighting::DirectionalLight();
	mDirLight3->LightType() = ZShadeSandboxLighting::ELightType::eDirectional;
	mDirLight3->DiffuseColor() = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLight3->AmbientColor() = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mDirLight3->SpecularColor() = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);
	mDirLight3->Position() = XMFLOAT3(xPos, yPos, 0);
	mDirLight3->Direction() = XMFLOAT3(-0.57735f, -0.57735f, -0.57735f);
	mDirLight3->SetLens(m_EngineOptions->fNearPlane, m_EngineOptions->fFarPlane);

	xPos += 50;
	mSpotLight1 = new ZShadeSandboxLighting::SpotLight();
	mSpotLight1->LightType() = ZShadeSandboxLighting::ELightType::eSpot;
	mSpotLight1->DiffuseColor() = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mSpotLight1->AmbientColor() = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mSpotLight1->SpecularColor() = XMFLOAT4(0.8f, 0.8f, 0.7f, 1.0f);
	mSpotLight1->Position() = XMFLOAT3(xPos, yPos, 0);
	mSpotLight1->Direction() = XMFLOAT3(0.707f, -0.707f, 0.0f);
	mSpotLight1->Range() = 1000.0f;
	mSpotLight1->SpotAngle() = 45.0f;
	mSpotLight1->SpotCosOuterCone() = 10.0f;
	mSpotLight1->SpotInnerConeReciprocal() = 1.0f / 10.0f;
	mSpotLight1->Attenuation() = XMFLOAT3(0.5f, 3.0f, 0.1f);

	//xPos += 50;
	mPointLight = new ZShadeSandboxLighting::PointLight();
	mPointLight->LightType() = ZShadeSandboxLighting::ELightType::ePoint;
	mPointLight->DiffuseColor() = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mPointLight->AmbientColor() = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mPointLight->SpecularColor() = XMFLOAT4(0.8f, 0.8f, 0.7f, 1.0f);
	mPointLight->Position() = XMFLOAT3(xPos, yPos, 500);
	mPointLight->Direction() = XMFLOAT3(0.707f, -0.707f, 0.0f);
	mPointLight->Range() = 1000.0f;
	mPointLight->Attenuation() = XMFLOAT3(0.5f, 3.0f, 0.1f);

	xPos += 30;
	mCapsuleLight = new ZShadeSandboxLighting::CapsuleLight();
	mCapsuleLight->LightType() = ZShadeSandboxLighting::ELightType::eCapsule;
	mCapsuleLight->DiffuseColor() = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mCapsuleLight->AmbientColor() = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
	mCapsuleLight->SpecularColor() = XMFLOAT4(0.8f, 0.8f, 0.7f, 1.0f);
	mCapsuleLight->Position() = XMFLOAT3(xPos, yPos, 100);
	mCapsuleLight->Range() = 300.0f;
	mCapsuleLight->Direction() = XMFLOAT3(0.707f, -0.707f, 0.0f);
	mCapsuleLight->LightLength() = 50.0f;
	mCapsuleLight->CapsuleDirectionLength() = XMFLOAT3(0.707f, -0.707f, 0.0f);
	mCapsuleLight->CapsuleIntensity() = 100.0f;

	ZShadeSandboxLighting::AmbientLight* ambientLight = new ZShadeSandboxLighting::AmbientLight();
	ambientLight->Position() = XMFLOAT3(xPos, yPos, 100);
	ambientLight->AmbientColor() = XMFLOAT4(0.3f, 1.5f, 0.3f, 1.0f);

	// Initialize the light manager that handles all the basic lights in the scene
	ZShadeSandboxLighting::LightManager::NewInstance(m_D3DSystem);

	ZShadeSandboxLighting::LightManager::Instance()->AddLight(mDirLight1);
	//ZShadeSandboxLighting::LightManager::Instance()->AddLight(mDirLight2);
	//ZShadeSandboxLighting::LightManager::Instance()->AddLight(mDirLight3);
	//ZShadeSandboxLighting::LightManager::Instance()->AddLight(mCapsuleLight);
	//ZShadeSandboxLighting::LightManager::Instance()->AddLight(ambientLight);
	//ZShadeSandboxLighting::LightManager::Instance()->AddLight(mSpotLight1);
	ZShadeSandboxLighting::LightManager::Instance()->AddLight(mPointLight);
	
	// Disable all the lights
	//ZShadeSandboxLighting::LightManager::Instance()->ToggleAmbientLights(false);
	//ZShadeSandboxLighting::LightManager::Instance()->ToggleDirectionalLights(false);
	//ZShadeSandboxLighting::LightManager::Instance()->ToggleSpotLights(false);
	//ZShadeSandboxLighting::LightManager::Instance()->TogglePointLights(false);
	//ZShadeSandboxLighting::LightManager::Instance()->ToggleCapsuleLights(false);
	
	// Build the initial forward lighting buffers
	mSunLightBuffer = new ZShadeSandboxLighting::SunLightBuffer();
	mSunLightBuffer->g_SunDir = XMFLOAT3(0.936016f, -0.343206f, 0.0780013f);
	mSunLightBuffer->g_SunDiffuseColor = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	mSunLightBuffer->g_SunShineness = 500.0f;
	mSunLightBuffer->g_EnableSun = 1;
	mAmbientUp = XMFLOAT3(1.3f, 1.3f, 1.3f);
	mAmbientDown = XMFLOAT3(0.3f, 0.3f, 0.3f);
	ZShadeSandboxLighting::SunLightBuffer sb = *mSunLightBuffer;
	ZShadeSandboxLighting::LightManager::Instance()->BuildLightingBuffers(mAmbientUp, mAmbientDown, sb);
	
	//Create the refraction render to texture object
	mRefractionTexture = new RenderTarget2D(m_D3DSystem);
	mRefractionTexture->Initialize(m_EngineOptions->m_screenWidth, m_EngineOptions->m_screenHeight, DXGI_FORMAT_R32G32B32A32_FLOAT);

	//Create the reflection render to texture object
	mReflectionTexture = new RenderTarget2D(m_D3DSystem);
	mReflectionTexture->Initialize(m_EngineOptions->m_screenWidth, m_EngineOptions->m_screenHeight, DXGI_FORMAT_R32G32B32A32_FLOAT);
	
	//Create the shadow map render to texture object
	//mShadowTexture = new RenderTarget2D(m_D3DSystem);
	//mShadowTexture->Initialize(fShadowMapWidth, fShadowMapWidth, DXGI_FORMAT_R32G32B32A32_FLOAT);//DXGI_FORMAT_R24G8_TYPELESS
	
	mShadowTexture = new ShadowMapTarget(m_D3DSystem);
	mShadowTexture->Initialize(fShadowMapWidth, fShadowMapWidth);

	mSceneTarget = new RenderTarget2D(m_D3DSystem);
	mSceneTarget->Initialize(m_EngineOptions->m_screenWidth, m_EngineOptions->m_screenHeight, DXGI_FORMAT_R32G32B32A32_FLOAT);

	GDI::Init();
	
	int text_size = 16;
	mFPSText.SetD3D(m_D3DSystem);
	mFPSText.Initialize();
	mFPSText.TextName() = BetterString("");
	//mFPSText.CreateShadow();
	mFPSText.ChangeFont(L"Times New Roman", text_size, FontStyle::BoldItalic, true);
	mFPSTextMatrix = XMMatrixTranslation(2, 2, 0);
	mFPSTextMatrix = XMMatrixMultiply(mFPSTextMatrix, XMMatrixScaling(1, 1, 1));
	mFPSText.TextTransform() = mFPSTextMatrix;
	mFPSText.TextColor() = XMFLOAT4(1, 0, 0, 1);
	
	mTriangleCountText.SetD3D(m_D3DSystem);
	mTriangleCountText.Initialize();
	mTriangleCountText.TextName() = BetterString("");
	//mTriangleCountText.CreateShadow();
	mTriangleCountText.ChangeFont(L"Times New Roman", text_size, FontStyle::BoldItalic, true);
	mTriangleCountTextMatrix = XMMatrixTranslation(2, text_size + 2, 0);
	mTriangleCountTextMatrix = XMMatrixMultiply(mTriangleCountTextMatrix, XMMatrixScaling(1, 1, 1));
	mTriangleCountText.TextTransform() = mTriangleCountTextMatrix;
	mTriangleCountText.TextColor() = XMFLOAT4(1, 0, 0, 1);
	
	mPostProcessManager = new PostProcessManager(m_D3DSystem);
	mPostProcessManager->AddPostProcess(new HDR(m_D3DSystem));
	
	mDirectionalLightDeferredShader = new DirectionalLightDeferredShader(m_D3DSystem);
	mPointLightDeferredShader = new PointLightDeferredShader(m_D3DSystem);
	
	return true;
}
//===============================================================================================================================
void Environment3D::Shutdown()
{
	m_D3DSystem->Shutdown();
}
//===============================================================================================================================
bool Environment3D::Quickwire()
{
	// LeftAlt or Space
	if (keyboard->IsKeyDown(Keyboard::Key::Space))
		return true;
	else
		return false;
}
//==============================================================================================================================
void Environment3D::UpdateMaster()
{
	keyboard = Keyboard::GetKeyboard();
	mouse = Mouse::GetMouse();
	
	HWND wnd = GetForegroundWindow();
	//if (wnd != m_BaseWindow->GetHwnd() && wnd != m_RenderWindow->GetHwnd()) return;
	
	FPSCounter::GetInstance()->Update();
	BetterString str = "FPS: ";
	str += ZShadeSandboxGlobal::Convert::ConvertIntToString(FPSCounter::GetInstance()->FPS());
	mFPSText.TextName() = str;
	
	m_Timer.Frame();
	
	// What is the difference between these
	fFrameTime = m_Timer.DeltaSecondsF();
	float time = m_Timer.GetTime();
	
	// Generate the view and reflection view matrix based on the camera's position.
	XMFLOAT4 frustumPlanes[6];
	m_CameraSystem->ExtractFrustumPlanes(frustumPlanes);
	m_CameraSystem->Update();
	
	// Update the view projection matrix from the perspective of the light
	//mDirLight1->Update();
	//mDirLight2->Update();
	//mDirLight1->UpdateLVP();

	mDirLight1->Perspective()->SetSceneBounds(XMFLOAT3(0, 0, 0), 2048);
	mDirLight2->Perspective()->SetSceneBounds(XMFLOAT3(0, 0, 0), 2048);

	// Update the view projection matrix from the perspective of the light
	//mDirLight1->UpdateLVP();
	//mDirLight2->UpdateLVP();
	mDirLight1->Update();
	
	if (!bCameraStill)
	{
		if( keyboard->IsKeyDown(Keyboard::Key::W) )
		{
			//if( GetAsyncKeyState(VK_LSHIFT) & 0x8000 )
			if (keyboard->IsKeyDown(Keyboard::Key::LeftShift) )
				m_CameraSystem->Walk(fCamSpeedFast * time);
			else
				m_CameraSystem->Walk(fCamSpeedSlow * time);
		}

		if( keyboard->IsKeyDown(Keyboard::Key::S) )
		{
			if (keyboard->IsKeyDown(Keyboard::Key::LeftShift) )
				m_CameraSystem->Walk(-fCamSpeedFast * time);
			else
				m_CameraSystem->Walk(-fCamSpeedSlow * time);
		}

		if( keyboard->IsKeyDown(Keyboard::Key::A) )
		{
			if (keyboard->IsKeyDown(Keyboard::Key::LeftShift) )
				m_CameraSystem->Strafe(-fCamSpeedFast * time);
			else
				m_CameraSystem->Strafe(-fCamSpeedSlow * time);
		}

		if( keyboard->IsKeyDown(Keyboard::Key::D) )
		{
			if (keyboard->IsKeyDown(Keyboard::Key::LeftShift) )
				m_CameraSystem->Strafe(fCamSpeedFast * time);
			else
				m_CameraSystem->Strafe(fCamSpeedSlow * time);
		}
	}

	// This is not used to toggle forward lighting system
	if (keyboard->RisingEdge(Keyboard::Key::F1) )
	{
		//while(1) { Sleep(300); break; }
		bEnableLighting = !bEnableLighting;
	}

	//if( GetAsyncKeyState(VK_F2) & 0x8000 )
	if (keyboard->RisingEdge(Keyboard::Key::F2) )
	{
		//while(1) { Sleep(300); break; }
		bWireframeMode = !bWireframeMode;
	}

	if (keyboard->RisingEdge(Keyboard::Key::F3))
	{
		bEnablePostProcessing = !bEnablePostProcessing;
	}
	
	if (keyboard->RisingEdge(Keyboard::Key::F4))
	{
		bEnableDeferredShading = !bEnableDeferredShading;
	}

	if (keyboard->RisingEdge(Keyboard::Key::F5))
	{
		while(1) { Sleep(30); break; }
		bEnableTessellation = !bEnableTessellation;
	}

	if (keyboard->RisingEdge(Keyboard::Key::F))
	{
		// TODO: Randomly spawn a mesh object
		
		ZShadeSandboxMesh::MeshParameters mp;
		mp.useCustomShader = false;
		mp.vertexType = ZShadeSandboxMesh::EVertexType::VT_Tex;
		mp.rotationAxisX = false;
		mp.rotationAxisY = true;
		mp.rotationAxisZ = false;
		mp.pos = m_CameraSystem->Position();
		mp.rot = XMFLOAT3(0, 1, 0);
		mp.scale = XMFLOAT3(40, 40, 40);
		mp.material = MaterialManager::Instance()->GetMaterial("Wall");
		mp.in2D = false;
		mp.textureWidth = -1;
		mp.textureHeight = -1;
		mp.shader = 0;
		m_SpawnedMeshContainer.push_back(new ZShadeSandboxMesh::CubeMesh(m_D3DSystem, mp));
	}

	//Fullscreen mode
	/*if ( keyboard->RisingEdge(Keyboard::Key::F4) )
	{
		//while(1) { Sleep(5); break; }
		fullScreen = !fullScreen;
		if(fullScreen != m_D3DSystem->GetEngineOptions()->fullscreen)
		{
			BeforeReset();

			//The concrete class that uses this will set the device to fullscreen
			m_D3DSystem->SetFullscreen(fullScreen);

			AfterReset();
		}
	}*/

	// Update camera speed
	if (keyboard->RisingEdge(Keyboard::Key::F6))
	{
		while (1) { Sleep(30); break; }
		fCamSpeedFast -= 0.01f;
		fCamSpeedSlow -= 0.01f;
		if (fCamSpeedFast <= 0)
			fCamSpeedFast += 0.01f;
		if (fCamSpeedSlow <= 0)
			fCamSpeedSlow += 0.01f;
	}
	if (keyboard->RisingEdge(Keyboard::Key::F7))
	{
		while (1) { Sleep(30); break; }
		fCamSpeedFast += 0.01f;
		fCamSpeedSlow += 0.01f;
	}
	
	if (keyboard->IsKeyDown(Keyboard::Key::F8))
	{
		while (1) { Sleep(30); break; }
		fTessFactor -= 1.0f;
		if (fTessFactor <= 0)
			fTessFactor += 1.0f;
	}

	if (keyboard->IsKeyDown(Keyboard::Key::F9))
	{
		while (1) { Sleep(30); break; }
		fTessFactor += 1.0f;
		if (fTessFactor >= 64.0f)
			fTessFactor -= 1.0f;
	}
	
	// If there are any lights in the scene capture them
	ZShadeSandboxLighting::LightManager::Instance()->RebuildLightBuffer(mAmbientUp, mAmbientDown);
	ZShadeSandboxLighting::LightManager::Instance()->RebuildSunBuffer(*mSunLightBuffer);
	
	// Update other scene components
	Update();
}
//===============================================================================================================================
void Environment3D::RenderMaster()
{
	// Set cull mode or wireframe mode for the scene
	if (bWireframeMode)
	{
		m_D3DSystem->TurnOnWireframe();
	}
	else
	{
		m_D3DSystem->TurnOnCulling();
	}
	
	if (bEnableReflections)
	{
		RenderReflectionToTexture();
	}
	
	if (bEnableRefractions)
	{
		RenderRefractionToTexture();
	}
	
	if (bEnableShadows)
	{
		RenderShadowMapToTexture();
	}
	
	if (bEnableDeferredShading)
	{
		m_D3DSystem->GBufferBegin();
		{
			RenderDeferred();
		}
		m_D3DSystem->GBufferEnd();
		
		if (bUseDirectionalLightDeferred)
		{
			mDirectionalLightDeferredShader->SetWireframe(bWireframeMode);
			mDirectionalLightDeferredShader->Render11(
				m_CameraSystem.get(),
				mDirLight1,
				mAmbientUp,
				mAmbientDown,
				m_D3DSystem->GBufferCubeColorTarget()->SRView,
				m_D3DSystem->GBufferColorTarget()->SRView,
				m_D3DSystem->GBufferNormalTarget()->SRView,
				m_D3DSystem->GBufferDepthTarget()->SRView
			);
		}
		
		if (bUsePointLightDeferred)
		{
			mPointLightDeferredShader->SetWireframe(bWireframeMode);
			mPointLightDeferredShader->Render11(
				m_CameraSystem.get(),
				mPointLight,
				m_D3DSystem->GBufferCubeColorTarget()->SRView,
				m_D3DSystem->GBufferColorTarget()->SRView,
				m_D3DSystem->GBufferNormalTarget()->SRView,
				m_D3DSystem->GBufferDepthTarget()->SRView
			);
		}
		
		if (bUseSpotLightDeferred)
		{
			
		}
		
		if (bUseCapsuleLightDeferred)
		{
			
		}
		
		if (bEnablePostProcessing)
		{
			mPostProcessManager->Render(m_D3DSystem->GBufferColorTarget()->SRView, m_D3DSystem->GetBackbufferRenderTarget()->RTView);
		}
	}
	else
	{
		// Normal 3D Rendering for the scene Happens Here
		Render();

		if (bEnablePostProcessing)
		{
			mPostProcessManager->Render(m_D3DSystem->GetBackbufferRenderTarget()->SRView, m_D3DSystem->GetBackbufferRenderTarget()->RTView);
		}
	}
	
	//
	// 2D Rendering
	//
	
	m_D3DSystem->TurnOffZBuffer();
	{
		mFPSTextMatrix = XMMatrixTranslation(2, 2, 0);
		mFPSTextMatrix = XMMatrixMultiply(mFPSTextMatrix, XMMatrixScaling(1, 1, 1));
		mFPSText.Render11();
		
		mTriangleCountTextMatrix = XMMatrixTranslation(2, 4, 0);
		mTriangleCountTextMatrix = XMMatrixMultiply(mTriangleCountTextMatrix, XMMatrixScaling(1, 1, 1));
		mTriangleCountText.Render11();
		
		Render2D();
	}
	m_D3DSystem->TurnOnZBuffer();
}
//===============================================================================================================================
void Environment3D::OnMouseDown(WPARAM btnState, int x, int y)
{
	mLastMousePos.x = x;
	mLastMousePos.y = y;

	if ((btnState & MK_LBUTTON) != 0)
	{
		bLeftMouseDown = true;
	}
	
	if (m_EngineOptions->m_inEditor)
		;
	else
		SetCapture(m_RenderWindow->GetHwnd());
}
//===============================================================================================================================
void Environment3D::OnMouseUp(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_LBUTTON) != 0)
	{
		bLeftMouseDown = false;
	}

	ReleaseCapture();
}
//===============================================================================================================================
void Environment3D::OnMouseMove(WPARAM btnState, int x, int y)
{
	if ((btnState & MK_RBUTTON) != 0)
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f*static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f*static_cast<float>(y - mLastMousePos.y));
		
		if (!bCameraStill)
			m_CameraSystem->UpdateRotation(dx, dy);
	}

	mLastMousePos.x = x;
	mLastMousePos.y = y;
}
//===============================================================================================================================
void Environment3D::RenderRefractionToTexture()
{
	//Use the camera to render the reflection and craete a reflection view matrix
	//Get the camera reflection view matrix instead of the normal view matrix
	m_CameraSystem->UpdateReflection(fSeaLevel);

	XMVECTOR planeNormalDirection = XMVectorSet(0.0f, -1.0f, 0.0f, fSeaLevel);
	planeNormalDirection = XMVector3Normalize(planeNormalDirection);

	XMFLOAT4 planeNormalDirectionF;
	XMStoreFloat4(&planeNormalDirectionF, planeNormalDirection);
	XMFLOAT4 planeCoefficients = XMFLOAT4(planeNormalDirectionF.x, planeNormalDirectionF.y, planeNormalDirectionF.z, planeNormalDirectionF.w);
	XMMATRIX camMatrix = XMMatrixMultiply(m_CameraSystem->View(), m_CameraSystem->Proj());
	XMVECTOR dummy;
	XMMATRIX invCamMatrix = XMMatrixInverse(&dummy, camMatrix);
	invCamMatrix = XMMatrixTranspose(invCamMatrix);
	XMVECTOR planeCoefficientsV = XMLoadFloat4(&planeCoefficients);
	XMVECTOR planeCoefficientsTrans = XMVector4Transform(planeCoefficientsV, invCamMatrix);
	XMFLOAT4 refractionClipPlane;
	XMStoreFloat4(&refractionClipPlane, planeCoefficientsTrans);

	//Use the camera to render the reflection and craete a reflection view matrix
	//Get the camera reflection view matrix instead of the normal view matrix
	//m_CameraSystem->UpdateReflection(m_SeaLevel);

	//Set the render target to be the refraction render to texture
	mRefractionTexture->SetRenderTarget();

	//Clear the refraction render to texture
	mRefractionTexture->ClearRenderTarget(0.0f, 0.0f, 0.0f, 1.0f);

	//Rendering happens here
	{
		RenderRefraction(refractionClipPlane);
	}

	//Reset the render target back to the original back buffer and not the render to texture anymore
	//if (m_PostProcessEnable)
	//{
	//	m_D3DSystem->SetColorRenderTarget();
	//}
	//else
	m_D3DSystem->SetBackBufferRenderTarget();

	//Reset the viewport back to the original
	m_D3DSystem->ResetViewport();
}
//===============================================================================================================================
void Environment3D::RenderReflectionToTexture()
{
	//Use the camera to render the reflection and craete a reflection view matrix
	//Get the camera reflection view matrix instead of the normal view matrix
	m_CameraSystem->UpdateReflection(fSeaLevel);

	XMVECTOR planeNormalDirection = XMVectorSet(0.0f, 1.0f, 0.0f, -fSeaLevel);
	planeNormalDirection = XMVector3Normalize(planeNormalDirection);

	XMFLOAT4 planeNormalDirectionF;
	XMStoreFloat4(&planeNormalDirectionF, planeNormalDirection);
	XMFLOAT4 planeCoefficients = XMFLOAT4(planeNormalDirectionF.x, planeNormalDirectionF.y, planeNormalDirectionF.z, planeNormalDirectionF.w + 1.0);
	XMMATRIX camMatrix = XMMatrixMultiply(m_CameraSystem->ReflectionView(), m_CameraSystem->Proj());
	XMVECTOR dummy;
	XMMATRIX invCamMatrix = XMMatrixInverse(&dummy, camMatrix);
	invCamMatrix = XMMatrixTranspose(invCamMatrix);
	XMVECTOR planeCoefficientsV = XMLoadFloat4(&planeCoefficients);
	XMVECTOR planeCoefficientsTrans = XMVector4Transform(planeCoefficientsV, invCamMatrix);
	XMFLOAT4 reflectionClipPlane;
	XMStoreFloat4(&reflectionClipPlane, planeCoefficientsTrans);

	//Set the render target to be the reflection render to texture
	mReflectionTexture->SetRenderTarget();

	//Clear the reflection render to texture
	mReflectionTexture->ClearRenderTarget(0.0f, 0.0f, 0.0f, 1.0f);

	//Rendering happens here
	{
		RenderReflection(reflectionClipPlane);
	}

	//Reset the render target to the original back buffer and not the render to texture anymore
	//if (m_PostProcessEnable)
	//{
	//	m_D3DSystem->SetColorRenderTarget();
	//}
	//else
	m_D3DSystem->SetBackBufferRenderTarget();

	//Reset the viewport back to the original
	m_D3DSystem->ResetViewport();
}
//===============================================================================================================================
void Environment3D::RenderShadowMapToTexture()
{
	mShadowTexture->SetRenderTarget();
	//Rendering happens here
	{
		RenderShadowMap();
	}
	mShadowTexture->ResetView();
}
//===============================================================================================================================
int Environment3D::Run()
{
	try
    {
		if (m_EngineOptions->m_inEditor)
		{
			while (m_BaseEditWindow->IsAlive())
			{
				if (!m_BaseEditWindow->WinIsMinimized())
				{
					UpdateMaster();

					if (m_D3DSystem->BeginScene(0.1f, 0.2f, 0.4f, 0.0f))
					{
						//Rendering Happens Here
						
						RenderMaster();

						m_D3DSystem->EndScene();
					}

					//Quick esc for the game
					if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
					{
						m_BaseEditWindow->Destroy();
						break;
					}
				}

				m_BaseEditWindow->Run();
			}
		}
		else
		{
			while (m_RenderWindow->IsAlive())
			{
				if (!m_RenderWindow->WinIsMinimized())
				{
					UpdateMaster();

					if (m_D3DSystem->BeginScene(0.1f, 0.2f, 0.4f, 0.0f))
					{
						//Rendering Happens Here
						
						RenderMaster();

						m_D3DSystem->EndScene();
					}

					//Quick esc for the game
					if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
					{
						m_RenderWindow->Destroy();
						break;
					}
				}

				m_RenderWindow->Run();
			}
		}
    }
	catch (Exception exception)
    {
        exception.ShowErrorMessage();
    }
	
	return 1;
}
//===============================================================================================================================