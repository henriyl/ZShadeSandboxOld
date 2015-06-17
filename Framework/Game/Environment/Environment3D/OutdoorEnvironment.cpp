#include "OutdoorEnvironment.h"
#include "LightManager.h"
#include "CapsuleLight.h"
#include "AmbientLight.h"
#include "LightShadingBuffers.h"
#include "MaterialManager.h"
//===============================================================================================================================
//===============================================================================================================================
OutdoorEnvironment::OutdoorEnvironment(LPCSTR base_window_name, LPCSTR render_window_name, EngineOptions* eo, bool init_lua)
:   Environment3D(base_window_name, render_window_name, eo, init_lua)
,	bWaterFog(false)
,	bToggleWater(true)
,	bRenderOcean(false)
,	bToggleTerrain(true)
,   bClipCameraToTerrain(false)
,   bRenderExtraRenderDist(false)
,   bRenderFullMap(false)
{
	Init();
}
//===============================================================================================================================
OutdoorEnvironment::~OutdoorEnvironment()
{
	Shutdown();
}
//===============================================================================================================================
bool OutdoorEnvironment::Init()
{
	fHeightScale = 1.0f;
	fTerrSize = 1.0f;
	
	bEnableReflections = true;
	bEnableRefractions = true;
	bToggleTerrainSSAO = true;
	bToggleTerrainShadow = true;
	bToggleTerrainNormalMap = true;
	bToggleTerrainDetailMap = true;

	m_CameraSystem->SetPosition(0.0f, 100.0f, 100.0f);
	
	fSeaLevel = 0.0f;// 1500.0f;
	m_CameraSystem->SetRenderReflectionView( bEnableReflections );

	//mCamSpeedFast = 2.0f;
	//mCamSpeedSlow = 0.5f;

	// Create a random infinite plane
	ZShadeSandboxMesh::MeshParameters mp;
	mp.useCustomShader = false;
	mp.vertexType = ZShadeSandboxMesh::EVertexType::VT_Tex;
	mp.rotationAxisX = false;
	mp.rotationAxisY = false;
	mp.rotationAxisZ = false;
	mp.pos = XMFLOAT3(0, 0, 0);
	mp.rot = XMFLOAT3(0, 0, 0);
	mp.scale = XMFLOAT3(1, 1, 1);
	mp.material = MaterialManager::Instance()->GetMaterial("Floor");
	mp.in2D = false;
	mp.textureWidth = -1;
	mp.textureHeight = -1;
	mp.shader = 0;
	m_pInfinitePlane = new InfinitePlane(100.0f, 0.0f, 50.0f, m_D3DSystem, mp);
	
	float size = 10;
	float r = size / 100;
	mp.scale = XMFLOAT3(size, size, size);
	mp.pos = XMFLOAT3(0, 5, 0);
	mSphere = new ZShadeSandboxMesh::SphereMesh(r, size, size, m_D3DSystem, mp);
	
	mp.pos = XMFLOAT3(0, 5, 10);
	mGeosphereMesh = new ZShadeSandboxMesh::GeosphereMesh(0.5f, 2, m_D3DSystem, mp);

	mp.scale = XMFLOAT3(1, 1, 1);
	mp.pos = XMFLOAT3(10, 5, 10);
	mCylindricalMesh = new ZShadeSandboxMesh::CylindricalMesh(0.5f, 0.5f, 3.0f, 10, 10, m_D3DSystem, mp);

	mp.pos = XMFLOAT3(0, 5, 20);
	mCircleMesh = new ZShadeSandboxMesh::CircleMesh(m_D3DSystem, 0.5f, mp);
	
	m_SpawnedMeshContainer.push_back(mSphere);
	m_SpawnedMeshContainer.push_back(mGeosphereMesh);
	m_SpawnedMeshContainer.push_back(mCylindricalMesh);
	
	//mSunQuad = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp, true);
	//mp.scale = XMFLOAT3(50, 50, 50);
	//mVolume = new Volume(m_D3DSystem, mp);
	
	// shapes
	// cube
	// spaceCompound
	// MaleLow
	mMesh = new ZShadeSandboxMesh::OBJMesh(m_D3DSystem, m_GameDirectory3D);
	mMesh->Load("Models//spaceCompound.obj", false, true);
	mMesh->Scale(XMFLOAT3(0.3f, 0.3f, 0.3f));
	mMesh->Position(XMFLOAT3(100, 15, 300));
	
	InitializeTerrain();
	InitializeWater();
	InitializeOcean();
	
	// Enable the lights for this scene
	ZShadeSandboxLighting::LightManager::Instance()->ToggleDirectionalLights(true);
	
	return true;
}
//===============================================================================================================================
bool OutdoorEnvironment::InitializeTerrain()
{
	//gTerrSize = 200;

	//gHeightScale = gTerrSize * 2;

	//gMinTessellationDist *= gTerrSize;
	//gMaxTessellationDist *= gTerrSize;

	fMapSize = 256;

	float    fMinTessellationDist = 20.0f;
	float    fMaxTessellationDist = 500.0f;
	float    fMinTessellation = 0.0f;
	float    fMaxTessellation = 64.0f;
	float    fFogStart = 5.0f;//15.0f;
	float    fFogRange = 1000.0f;//175.0f;
	XMVECTOR gFogColor = DXColors::Silver;
	int      iLeafSize = 32;
	int      iTerrSize = 1;
	float    fHeightScale = 200.0f;
	float    fCellSpacing = 0.5f;

	//fCamSpeedFast *= fMapSize;
	//fCamSpeedSlow *= fMapSize;
	
	bEnableTessellation = true;

	fMinDist = fMinTessellationDist;
	fMaxDist = fMapSize;// / 2;
	
	fTerrRenderSize = 1.0f;
	
	//m_pTerrainShader = new TerrainShader(m_D3DSystem, "Terrain\\Terrain.hlsl");

	//m_pQuadTree = new ZShadeSandboxTerrain::QuadTree(mD3DSystem, "Textures/Terrain/Island3Big.bmp", 0, gLeafSize, gCellSpacing, gHeightScale, BMP, gTessellate, gTerrSize);
	//m_pQuadTree = new ZShadeSandboxTerrain::QuadTree(m_D3DSystem, "Textures/Terrain/valleyBig.bmp", 0, gLeafSize, gCellSpacing, gHeightScale, BMP, gTessellate, gTerrSize);
	//m_pQuadTree = new ZShadeSandboxTerrain::QuadTree(m_D3DSystem, "Textures\\Terrain\\terrain.raw", gCellSize, gLeafSize, gCellSpacing, gHeightScale, RAW, gTessellate, gTerrSize);
	//m_pQuadTree = new ZShadeSandboxTerrain::QuadTree(m_D3DSystem, "Textures/Terrain/Chatham-terrain.raw", gCellSize, gLeafSize, gCellSpacing, gHeightScale, RAW, gTessellate, gTerrSize);

	ZShadeSandboxTerrain::TerrainParameters tp;
	//tp.g_heightmapName = "terrain3.bmp";
	//tp.g_heightmapName = "Island3Big.bmp";
	tp.g_heightmapName = "heightmap01.bmp";
	//tp.g_heightmapName = "valleyBig.bmp";
	//tp.g_heightmapName = "gcanyon.bmp";
	//tp.g_heightmapName = "terrain.raw";
	tp.g_TerrainSize = fMapSize;
	tp.g_leafWidth = iLeafSize;
	tp.g_cellSpacing = 0.5f;
	tp.g_heightScale = fHeightScale;
	tp.g_tessellate = bEnableTessellation;
	tp.g_terrScale = fTerrSize;

	m_pQuadTreeMesh = new ZShadeSandboxTerrain::QuadTreeMesh(m_D3DSystem, tp, m_GameDirectory3D);

	//vector<string> names;
	//names.push_back("Textures\\grass.dds");
	//names.push_back("Textures\\darkdirt.dds");
	//names.push_back("Textures\\lightdirt.dds");
	//names.push_back("Textures\\darkgrass.dds");
	//names.push_back("Textures\\soil.dds");
	
	m_pQuadTreeMesh->AddMaterialColors();
	m_pQuadTreeMesh->AddSpecularPower();
	m_pQuadTreeMesh->AddSpecularIntensity();
	m_pQuadTreeMesh->AddNormalMap(m_GameDirectory3D->m_textures_path, "lichen1_normal.dds");
	m_pQuadTreeMesh->AddDetailMap(m_GameDirectory3D->m_textures_path, "detail001.dds");
	//m_pQuadTreeMesh->AddMaterialTextures(names, "Textures", "blend.dds", "lichen1_normal.dds", "detail001.dds");

	//m_pQuadTreeMesh->BuildLayerMapArraySRV("Textures\\grass.dds");
	////m_pQuadTreeMesh->BuildLayerMapArraySRV("Textures\\darkdirt.dds");
	//m_pQuadTreeMesh->BuildLayerMapArraySRV("Textures\\darkdirt.dds");
	//m_pQuadTreeMesh->BuildLayerMapArraySRV("Textures\\lightdirt.dds");
	//m_pQuadTreeMesh->BuildLayerMapArraySRV("Textures\\darkgrass.dds", true);

	/*
	m_pQuadTreeMesh->BuildLayerMapArraySRV("Textures\\grass.dds");
	m_pQuadTreeMesh->BuildLayerMapArraySRV("Textures\\dirt01.dds");
	m_pQuadTreeMesh->BuildLayerMapArraySRV("Textures\\darkdirt.dds");
	m_pQuadTreeMesh->BuildLayerMapArraySRV("Textures\\lightdirt.dds");
	m_pQuadTreeMesh->BuildLayerMapArraySRV("Textures\\darkgrass.dds", true);
	m_pQuadTreeMesh->BuildBlendMapSRV("blend.dds");//blend.dds
	m_pQuadTreeMesh->BuildNormalMapSRV("stones_nmap.dds");// lichen1_normal.dds");
	m_pQuadTreeMesh->BuildDetailMapSRV("detail001.dds");
	*/
	
	//m_pQuadTreeMesh->BuildShadowMapSRV();

	m_pQuadTreeRenderer = new ZShadeSandboxTerrain::QuadTreeRenderer(m_D3DSystem, m_pQuadTreeMesh);

	return true;
}
//===============================================================================================================================
bool OutdoorEnvironment::InitializeWater()
{
	WaterParameters wp;
	wp.g_LightDirection = XMFLOAT3(0.936016f, -0.343206f, 0.0780013f);
	wp.g_RefractionTint = XMFLOAT4(0.07f, 0.15f, 0.2f, 1.0f);// 0.0f, 0.8f, 1.0f, 1.0f);
	wp.g_DullColor = XMFLOAT4(0.1f, 0.1f, 0.2f, 1.0f);
	//0.1f, 0.1f, 0.2f, 1.0f
	//0.3f, 0.3f, 0.5f, 1.0f
	wp.g_SpecularShininess = 20;
	wp.g_WaveHeight = 0.3f;
	wp.g_waterHeight = fSeaLevel;
	wp.g_waterRadius = 5000.0f;
	wp.g_TexScale = 2.5f;
	
	mWater = new Water();
	mWater->Initialize(m_D3DSystem, wp);
	//mWater->SetUseFlowmap(true);
	mWater->LoadFlowMapSRV(m_GameDirectory3D->m_textures_path + "\\flowmap.png");
	mWater->LoadNoiseMapSRV(m_GameDirectory3D->m_textures_path + "\\noise.png");
	mWater->LoadWaveMap0SRV(m_GameDirectory3D->m_textures_path + "\\wave0.dds");
	mWater->LoadWaveMap1SRV(m_GameDirectory3D->m_textures_path + "\\wave1.dds");
	
	return true;
}
//===============================================================================================================================
bool OutdoorEnvironment::InitializeOcean()
{
	/*OceanParameters ocean_param;

	// The size of displacement map. In this sample, it's fixed to 512.
	ocean_param.displacement_map_dim	= 512;
	// The side length (world space) of square patch
	ocean_param.patch_length		= 4000.0f;//2000.0f
	// Adjust this parameter to control the simulation speed
	ocean_param.time_scale			= 0.8f;
	// A scale to control the amplitude. Not the world space height
	ocean_param.wave_amplitude		= 0.35f;
	// 2D wind direction. No need to be normalized
	ocean_param.wind_dir			= XMMath2(0.8f, 0.6f);
	// The bigger the wind speed, the larger scale of wave crest.
	// But the wave scale can be no larger than patch_length
	ocean_param.wind_speed			= 3700.0f;//600
	// Damp out the components opVertexite to wind direction.
	// The smaller the value, the higher wind dependency
	ocean_param.wind_dependency		= 0.07f;
	// Control the scale of horizontal movement. Higher value creates
	// pointy crests.
	ocean_param.choppy_scale		= 1.3f;


	ocean_mesh_properties omp;

	//Mesh grid dimension, must be 2^n. 4x4 ~ 256x256
	omp.g_MeshDim              = 128;
	// Subdivision thredshold. Any quad covers more pixels than this value needs to be subdivided.
	omp.g_UpperGridCoverage    = 64.0f;
	// Draw distance = g_PatchLength * 2^g_FurthestCover
	omp.g_FurthestCover        = 8;
	//Two colors for waterbody and sky color
	omp.g_SkyColor             = XMMath3(0.38f, 0.45f, 0.56f);
	omp.g_WaterbodyColor = XMMath3(0.07f, 0.15f, 0.2f);
	//Blending term for sky cubemap
	omp.g_Skyblending          = 16.0f;
	// Perlin wave parameters
	omp.g_PerlinSize           = 1.0f;
	omp.g_PerlinSpeed          = 0.06f;
	omp.g_PerlinAmplitude	   = XMMath3(35, 42, 57);
	omp.g_PerlinGradient       = XMMath3(1.4f, 1.6f, 2.2f);
	omp.g_PerlinOctave         = XMMath3(1.12f, 0.59f, 0.23f);
	omp.g_BendParam            = XMMath3(0.1f, -0.4f, 0.2f);
	//Sunspot parameters
	omp.g_SunDir               = XMMath3(0.936016f, -0.343206f, 0.0780013f);
	omp.g_SunColor             = XMMath3(1.0f, 1.0f, 0.6f);
	omp.g_Shineness            = 400.0f;

	omp.g_PatchLength = ocean_param.patch_length;
	omp.g_DisplaceMapDim = ocean_param.displacement_map_dim;
	omp.g_WindDir = ocean_param.wind_dir;

	m_pOcean = new Ocean(m_D3DSystem, omp, ocean_param);

	//Update the ocean for the first time
	m_pOcean->Update( 0 );
	//m_D3DSystem->RunDummyTargetShader();
	*/
	return true;
}
//===============================================================================================================================
void OutdoorEnvironment::Shutdown()
{
	Environment3D::Shutdown();
}
//===============================================================================================================================
void OutdoorEnvironment::Update()
{
	// Need to get toggle update time based on the amount of triangles rendered
	float dt = fFrameTime;
	
	//Other update info
	if (GetAsyncKeyState( '1' ) & 0x8000 )
	{
		while(1) { Sleep(300); break; }
		bToggleWater = !bToggleWater;
	}
	if (GetAsyncKeyState( '2' ) & 0x8000 )
	{
		while(1) { Sleep(300); break; }
		bRenderOcean = !bRenderOcean;
	}
	if (GetAsyncKeyState( '3' ) & 0x8000 )
	{
		while(1) { Sleep(300); break; }
		bToggleTerrain = !bToggleTerrain;
	}
	if (GetAsyncKeyState( '6' ) & 0x8000 )
	{
		while(1) { Sleep(300); break; }
		bWaterFog = !bWaterFog;
	}

	if (keyboard->IsKeyDown(Keyboard::Key::P))
	{
		while(1) { Sleep(50); break; }
		fSeaLevel += fCamSpeedSlow + 1.0f;
	}
	if (keyboard->IsKeyDown(Keyboard::Key::O))
	{
		while(1) { Sleep(50); break; }
		fSeaLevel -= fCamSpeedSlow + 1.0f;
	}
	
	if (keyboard->IsKeyDown(Keyboard::Key::V))
	{
		while (1) { Sleep(30); break; }
		fHeightScale += 1.0f;
	}
	if (keyboard->IsKeyDown(Keyboard::Key::B))
	{
		while (1) { Sleep(30); break; }
		fHeightScale -= 1.0f;
		if (fHeightScale <= 0.0f)
			fHeightScale += 1.0f;
	}

	if (keyboard->RisingEdge(Keyboard::Key::G))
	{
		fTerrSize += 1.0f;
		fMaxDist = fMapSize * fTerrSize;
		m_pQuadTreeMesh->ReGenMesh(fTerrSize);
	}
	if (keyboard->RisingEdge(Keyboard::Key::H))
	{
		fTerrSize -= 1.0f;
		if (fTerrSize <= 0.0f)
			fTerrSize += 1.0f;
		else
		{
			fMaxDist = fMapSize * fTerrSize;
			m_pQuadTreeMesh->ReGenMesh(fTerrSize);
		}
	}
	
	if (keyboard->IsKeyDown(Keyboard::Key::T))
	{
		fMinDist -= 1.0f;
		if (fMinDist <= 0)
			fMinDist = 1.0f;
	}
	if (keyboard->IsKeyDown(Keyboard::Key::Y))
	{
		fMinDist += 1.0f;
	}

	if (keyboard->IsKeyDown(Keyboard::Key::U))
	{
		fMaxDist -= 1.0f;
		if (fMaxDist <= 0)
			fMaxDist = 1.0f;
	}
	if (keyboard->IsKeyDown(Keyboard::Key::I))
	{
		fMaxDist += 1.0f;
	}

	if (keyboard->RisingEdge(Keyboard::Key::C))
	{
		bClipCameraToTerrain = !bClipCameraToTerrain;
	}

	if (keyboard->RisingEdge(Keyboard::Key::K))
	{
		fTerrRenderSize -= 1.0f;
		if (fTerrRenderSize <= 0)
			fTerrRenderSize = 1.0f;
		
		m_pQuadTreeRenderer->RenderSize() = fTerrRenderSize;
	}

	if (keyboard->RisingEdge(Keyboard::Key::L))
	{
		fTerrRenderSize += 1.0f;
		if (fTerrRenderSize >= 6.0f)
			fTerrRenderSize = 6.0f;
		
		m_pQuadTreeRenderer->RenderSize() = fTerrRenderSize;
	}
	
	if (keyboard->RisingEdge(Keyboard::Key::N))
	{
		while(1) { Sleep(300); break; }
		bRenderFullMap = !bRenderFullMap;
	}
	
	if (keyboard->RisingEdge(Keyboard::Key::M))
	{
		while(1) { Sleep(300); break; }
		bRenderExtraRenderDist = !bRenderExtraRenderDist;
	}
	
	if (keyboard->RisingEdge(Keyboard::Key::Q))
	{
		while (1) { Sleep(30); break; }
		bToggleTerrainShadow = !bToggleTerrainShadow;
	}

	if (keyboard->RisingEdge(Keyboard::Key::Z))
	{
		while (1) { Sleep(30); break; }
		bToggleTerrainSSAO = !bToggleTerrainSSAO;
	}

	if (bToggleTerrain)
	{
		m_pQuadTreeRenderer->RenderFullMap() = bRenderFullMap;
		m_pQuadTreeRenderer->RenderExtraRenderDist() = bRenderExtraRenderDist;
		m_pQuadTreeRenderer->SetWireframe(bWireframeMode);
	}

	//m_pInfinitePlane->SetWireframe(m_wire);
	
	vector<ZShadeSandboxMesh::CustomMesh*>::iterator it = m_SpawnedMeshContainer.begin();
	for (; it != m_SpawnedMeshContainer.end(); it++)
	{
		(*it)->Update(dt);
		(*it)->SetWireframe(bWireframeMode);
	}
	
	mMesh->SetWireframe(bWireframeMode);
	
	//mSunRenderer->SetWireframe(bWireframeMode);

	if (bToggleTerrain)
	{
		m_pQuadTreeRenderer->SetWireframe(bWireframeMode);
	}
	
	/*if (mVolume->IntersectsAABB(m_CameraSystem->Position()))
	{
		m_ToggleSky = true;
	}
	else
	{
		m_ToggleSky = false;
	}*/

	XMFLOAT3 SpotLightPos = mSpotLight1->Position();
	XMFLOAT3 PointLightPos = mPointLight->Position();
	XMFLOAT3 CapsuleLightPos = mCapsuleLight->Position();
	XMFLOAT3 eye = m_CameraSystem->Position();
	XMFLOAT3 DirLightPos = mDirLight1->Position();
	//mCapsuleLight->UpdateMeshPosition(XMFLOAT3(eye.x, eye.y + 20.0f, eye.z));

	if (keyboard->IsKeyDown(Keyboard::Key::Up))
	{
		//mPointLight->UpdateMeshPosition(XMFLOAT3(PointLightPos.x + 1.0f, PointLightPos.y, PointLightPos.z));
		//mSpotLight1->UpdateMeshPosition(XMFLOAT3(SpotLightPos.x + 1.0f, SpotLightPos.y, SpotLightPos.z));
		//mCapsuleLight->UpdateMeshPosition(XMFLOAT3(CapsuleLightPos.x + 1.0f, CapsuleLightPos.y, CapsuleLightPos.z));
		//while (1) { Sleep(200); break; }
		mDirLight1->Direction().x += 0.01f;
		mDirLight1->UpdateMeshPosition(XMFLOAT3(DirLightPos.x + 0.01f, DirLightPos.y, DirLightPos.z));
	}
	if (keyboard->IsKeyDown(Keyboard::Key::Down))
	{
		//mPointLight->UpdateMeshPosition(XMFLOAT3(PointLightPos.x - 1.0f, PointLightPos.y, PointLightPos.z));
		//mSpotLight1->UpdateMeshPosition(XMFLOAT3(SpotLightPos.x - 1.0f, SpotLightPos.y, SpotLightPos.z));
		//mCapsuleLight->UpdateMeshPosition(XMFLOAT3(CapsuleLightPos.x - 1.0f, CapsuleLightPos.y, CapsuleLightPos.z));
		//while (1) { Sleep(200); break; }
		mDirLight1->Direction().x -= 0.01f;
		mDirLight1->UpdateMeshPosition(XMFLOAT3(DirLightPos.x - 0.01f, DirLightPos.y, DirLightPos.z));
	}
	if (keyboard->IsKeyDown(Keyboard::Key::Left))
	{
		//mPointLight->UpdateMeshPosition(XMFLOAT3(PointLightPos.x, PointLightPos.y, PointLightPos.z + 1.0f));
		//mSpotLight1->UpdateMeshPosition(XMFLOAT3(SpotLightPos.x, SpotLightPos.y, SpotLightPos.z + 1.0f));
		//mCapsuleLight->UpdateMeshPosition(XMFLOAT3(CapsuleLightPos.x, CapsuleLightPos.y, CapsuleLightPos.z + 1.0f));
		//while (1) { Sleep(200); break; }
		mDirLight1->Direction().z += 0.01f;
		mDirLight1->UpdateMeshPosition(XMFLOAT3(DirLightPos.x, DirLightPos.y, DirLightPos.z + 0.01f));
	}
	if (keyboard->IsKeyDown(Keyboard::Key::Right))
	{
		//mPointLight->UpdateMeshPosition(XMFLOAT3(PointLightPos.x, PointLightPos.y, PointLightPos.z - 1.0f));
		//mSpotLight1->UpdateMeshPosition(XMFLOAT3(SpotLightPos.x, SpotLightPos.y, SpotLightPos.z - 1.0f));
		//mCapsuleLight->UpdateMeshPosition(XMFLOAT3(CapsuleLightPos.x, CapsuleLightPos.y, CapsuleLightPos.z - 1.0f));
		//while (1) { Sleep(200); break; }
		mDirLight1->Direction().x -= 0.01f;
		mDirLight1->UpdateMeshPosition(XMFLOAT3(DirLightPos.x, DirLightPos.y, DirLightPos.z - 0.01f));
	}
	if (keyboard->IsKeyDown(Keyboard::Key::E))
	{
		//mPointLight->UpdateMeshPosition(XMFLOAT3(PointLightPos.x, PointLightPos.y + 1.0f, PointLightPos.z));
		//mSpotLight1->UpdateMeshPosition(XMFLOAT3(SpotLightPos.x, SpotLightPos.y + 1.0f, SpotLightPos.z));
		//mCapsuleLight->UpdateMeshPosition(XMFLOAT3(CapsuleLightPos.x, CapsuleLightPos.y + 1.0f, CapsuleLightPos.z));
		//while (1) { Sleep(200); break; }
		mDirLight1->Direction().y += 0.01f;
		mDirLight1->UpdateMeshPosition(XMFLOAT3(DirLightPos.x, DirLightPos.y + 0.01f, DirLightPos.z));
	}
	if (keyboard->IsKeyDown(Keyboard::Key::R))
	{
		//mPointLight->UpdateMeshPosition(XMFLOAT3(PointLightPos.x, PointLightPos.y - 1.0f, PointLightPos.z));
		//mSpotLight1->UpdateMeshPosition(XMFLOAT3(SpotLightPos.x, SpotLightPos.y - 1.0f, SpotLightPos.z));
		//mCapsuleLight->UpdateMeshPosition(XMFLOAT3(CapsuleLightPos.x, CapsuleLightPos.y - 1.0f, CapsuleLightPos.z));
		//while (1) { Sleep(200); break; }
		mDirLight1->Direction().y -= 0.01f;
		mDirLight1->UpdateMeshPosition(XMFLOAT3(DirLightPos.x, DirLightPos.y - 0.01f, DirLightPos.z));
	}
	
	// Move the light with mouse point in 3D
	/*if (mPickingRay != NULL)
	{
		XMMath3 pos(mPickingRay->position.x, mPickingRay->position.y, mPickingRay->position.z);
		XMMath3 dir(mPickingRay->direction.x, mPickingRay->direction.y, mPickingRay->direction.z);
		pos.x += 5.0f;
		pos.z += 5.0f;
		mCapsuleLight->UpdateMeshPosition(XMFLOAT3(pos.x, pos.y, pos.z));
	}*/

	// Updates the world matrix for the sun
	//mSun->Update(m_CameraSystem.get());

	if (bToggleWater)
	{
		if (!bRenderOcean)
		{
			mWater->SetWireframe( bWireframeMode );
			mWater->Update(fFrameTime, fSeaLevel);
		}
		else
		{
			//m_pOcean->SetWireframe( bWireframeMode );
			//m_pOcean->SetSeaLevel( fSeaLevel );
		}
	}
	
	// Count the amount of triangles in the scene
	iTriangleCount = 0;
	
	it = m_SpawnedMeshContainer.begin();
	for (; it != m_SpawnedMeshContainer.end(); it++)
	{
		iTriangleCount += (*it)->TriangleCount();
	}
	
	if (mPickingSphere != NULL)
	{
		iTriangleCount += mPickingSphere->TriangleCount();
	}
	
	// Add the triangle count of the light mesh that is rendered
	iTriangleCount += ZShadeSandboxLighting::LightManager::Instance()->TriangleCount();
	
	if (bToggleTerrain)
	{
		iTriangleCount += m_pQuadTreeMesh->TriangleCount();
	}
	
	if (bToggleWater)
	{
		if (!bRenderOcean)
			iTriangleCount += 2; // Water plane has 2 triangles
		else
			; // Not using Ocean yet
	}
	
	if (bToggleSky)
	{
		iTriangleCount += mSky->TriangleCount();
		
		if (bToggleSkyPlane)
		{
			iTriangleCount += mSkyPlane->TriangleCount();
		}
	}
	
	//iTriangleCount += mSpaceCompoundMesh->TriangleCount();
	
	// The scene is rendered 3 times with reflection and refraction enabled
	if (bToggleWater)
	{
		// The sky only has reflections
		if (bToggleSky)
		{
			iTriangleCount += mSky->TriangleCount();
			
			//if (m_ToggleSkyPlane)
			//{
			//	iTriangleCount += mSkyPlane->TriangleCount();
			//}
		}
		
		for (int i = 0; i < 2; i++)
		{
			if (bToggleTerrain)
			{
				iTriangleCount += m_pQuadTreeMesh->TriangleCount();
			}
			
			if (mPickingSphere != NULL)
			{
				iTriangleCount += mPickingSphere->TriangleCount();
			}
			
			it = m_SpawnedMeshContainer.begin();
			for (; it != m_SpawnedMeshContainer.end(); it++)
			{
				iTriangleCount += (*it)->TriangleCount();
			}
			
			//iTriangleCount += mSpaceCompoundMesh->TriangleCount();
			
			iTriangleCount += ZShadeSandboxLighting::LightManager::Instance()->TriangleCount();
		}
	}
	
	BetterString str = "Triangles: ";
	str += ZShadeSandboxGlobal::Convert::ConvertIntToString(iTriangleCount);
	mTriangleCountText.TextName() = str;
}
//===============================================================================================================================
void OutdoorEnvironment::Render()
{
	/*vector<ZShadeSandboxMesh::CustomMesh*>::iterator it = m_SpawnedMeshContainer.begin();
	for (; it != m_SpawnedMeshContainer.end(); it++)
	{
		(*it)->SetLightBuffer(ZShadeSandboxLighting::LightManager::Instance()->GetLightBuffer());
		(*it)->SetLightBuffer(ZShadeSandboxLighting::LightManager::Instance()->GetSunLightBuffer());
	}

	if (mPickingSphere != NULL)
	{
		mPickingSphere->SetLightBuffer(ZShadeSandboxLighting::LightManager::Instance()->GetLightBuffer());
		mPickingSphere->SetLightBuffer(ZShadeSandboxLighting::LightManager::Instance()->GetSunLightBuffer());
	}*/

	//if (m_ToggleWater)
	//{
	//	//Update the reflection and refraction textures for the scene, If there are any
	//	RenderRefractionToTexture();
	//	RenderReflectionToTexture();
	//}
	
	//RenderTerrainShadowSSAO();

	//We now get the position of the camera and then get the height of the triangle that would
	//be directly underneath it. Once we get the height back we set the height of the camera two
	//units directly above the triangle's height. This way when the camera moves it automatically
	//follows the exact height of the terrain.
	if (bClipCameraToTerrain)
	{
		float height = 0.0f;
		XMFLOAT3 eye = m_CameraSystem->Position();
		
		// Apply simple gravity
		m_CameraSystem->SetPositionY(eye.y - 1.0f);

		// Get the height of the triangle that is directly underneath the given camera position.
		if (m_pQuadTreeMesh->GetHeightAtPosition(eye, height))
		{
			// If there was a triangle under the camera then position the camera just above it by two units.
			m_CameraSystem->SetPositionY(height + 2.0f);
		}
	}
	
	// Mouse Picking Terrain Collision Test
	//POINT p;
	//GetCursorPos(&p);
	//ScreenToClient(m_BaseWindow->GetHwnd(), &p);
	//mPickingRay = m_CameraSystem->PickingRay(p.x, p.y, m_pQuadTreeRenderer->GetWorld());
	mPickingRay = m_CameraSystem->PickingRay(mLastMousePos.x, mLastMousePos.y, m_pQuadTreeRenderer->GetWorld());
	
	if (mPickingRay != NULL)
	{
		if (bLeftMouseDown)
		{
			bLeftMouseDown = false;
			bool hit = false;
			XMFLOAT3 hitPoint;
			ZShadeSandboxMath::Ray ray = *mPickingRay;
			m_pQuadTreeMesh->Intersects(ray, hit, hitPoint);
			if (hit)
			{
				//hitPoint.y += 2.0f;
				
				// Create a sphere at the point of intersection
				if (mPickingSphere == NULL)
				{
					ZShadeSandboxMesh::MeshParameters mp;
					mp.useCustomShader = false;
					mp.vertexType = ZShadeSandboxMesh::EVertexType::VT_Tex;
					mp.rotationAxisX = false;
					mp.rotationAxisY = true;
					mp.rotationAxisZ = false;
					mp.pos = hitPoint;
					mp.rot = XMFLOAT3(0, 0, 0);
					mp.scale = XMFLOAT3(1, 1, 1);
					mp.material = MaterialManager::Instance()->GetMaterial("Red");
					mp.in2D = false;
					mp.textureWidth = -1;
					mp.textureHeight = -1;
					mp.shader = 0;
					mPickingSphere = new ZShadeSandboxMesh::SphereMesh(2, 10, 10, m_D3DSystem, mp);

					//mPickingSphere->SetLightBuffer(ZShadeSandboxLighting::LightManager::Instance()->GetLightBuffer());
					//mPickingSphere->SetLightBuffer(ZShadeSandboxLighting::LightManager::Instance()->GetSunLightBuffer());
				}
				else
				{
					mPickingSphere->Position() = hitPoint;
				}
				
				//m_SpawnedCubes.push_back(new ZShadeSandboxMesh::CubeMesh(m_D3DSystem, mp));
			}
		}
	}

	if (!bWireframeMode)
		m_D3DSystem->TurnOnCulling();
	
	//m_D3DSystem->TurnOnZBuffer();

	//Quick wireframe mode
	if( Quickwire() )
	{
		if (bToggleWater)
		{
			if (!bRenderOcean)
				mWater->SetWireframe( true );
			else
				;//m_pOcean->SetWireframe( true );
		}

		if (bToggleTerrain)
		{
			m_pQuadTreeRenderer->SetWireframe(true);
		}

		vector<ZShadeSandboxMesh::CustomMesh*>::iterator it = m_SpawnedMeshContainer.begin();
		for (; it != m_SpawnedMeshContainer.end(); it++)
		{
			(*it)->SetWireframe(true);
		}

		mMesh->SetWireframe(true);
		
		mSunRenderer->SetWireframe(true);

		if (bToggleSky)
		{
			mSky->SetWireframe(true);
			
			if (bToggleSkyPlane)
				mSkyPlane->SetWireframe(true);
		}
		
		//m_pInfinitePlane->SetWireframe(true);
		
		m_D3DSystem->TurnOnWireframe();
	}

	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.camera = m_CameraSystem.get();
	mrp.light = mDirLight1;
	
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
	
	//
	//Render the terrain
	//

	if (bToggleTerrain)
		RenderTerrain( XMFLOAT4(0,0,0,0), false, false );
	
	// Render a random infinite plane
	//m_D3DSystem->TurnOnWireframe();
	//mVolume->SetWireframe(true);
	//mVolume->Render(mrp);
	//m_pInfinitePlane->SetWireframe(true);
	//mrp.fTessellationFactor = 30.0f;
	//mrp.bTessellate = true;
	//mrp.rt = RenderType::e3ControlPointPatchList;
	//m_pInfinitePlane->Render(mrp);
	//if (!m_wire || Quickwire())
	//	m_D3DSystem->TurnOnCulling();
	//mSphere->Render(mrp);
	//mGeosphereMesh->Render(mrp);
	//mCircleMesh->Render(mrp);
	
	if (mPickingSphere != NULL)
		mPickingSphere->Render(mrp);

	/*if (!m_wire && !Quickwire())
		m_D3DSystem->TurnOffCulling();
	mCylindricalMesh->Render(mrp);
	if (!m_wire && !Quickwire())
		m_D3DSystem->TurnOnCulling();*/

	//
	//Render some lake water or ocean water
	//

	if (bToggleWater)
	{
		if (!bRenderOcean)
			RenderWater();
		else
			RenderOcean();
	}

	//ZShadeSandboxMesh::OBJMeshRenderParameters omrp;
	//omrp.camera = m_CameraSystem.get();
	//omrp.renderType = ZShadeSandboxMesh::ERenderType::e3ControlPointPatchList;
	//omrp.tessellate = true;
	mMesh->SetFarPlane(m_EngineOptions->fFarPlane);
	mMesh->Render(mrp);

	//
	// Render the sphere mesh for the lights in the scene
	//
	
	ZShadeSandboxLighting::LightRenderParameters lrp;
	lrp.camera = m_CameraSystem.get();
	lrp.clipplane = XMFLOAT4(0, 0, 0, 0);
	lrp.reflect = false;
	lrp.renderDeferred = false;
	lrp.toggleMesh = true;
	lrp.toggleWireframe = bWireframeMode;
	ZShadeSandboxLighting::LightManager::Instance()->RenderLightMesh(lrp);
}
//===============================================================================================================================
void OutdoorEnvironment::RenderDeferred()
{

}
//===============================================================================================================================
void OutdoorEnvironment::RenderShadowMap()
{

}
//===============================================================================================================================
void OutdoorEnvironment::RenderReflection(XMFLOAT4 clipplane)
{
	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.camera = m_CameraSystem.get();
	mrp.light = mDirLight1;
	mrp.reflection = true;
	mrp.clipplane = clipplane;
	
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
	
	//
	// Render the terrain using the reflection view matrix and reflection clip plane
	//

	if (bToggleTerrain)
		RenderTerrain(clipplane, true, false);
	
	if (mPickingSphere != NULL)
		mPickingSphere->Render(mrp);
	
	ZShadeSandboxLighting::LightRenderParameters lrp;
	lrp.camera = m_CameraSystem.get();
	lrp.clipplane = clipplane;
	lrp.reflect = true;
	lrp.renderDeferred = false;
	lrp.toggleMesh = true;
	lrp.toggleWireframe = bWireframeMode;
	ZShadeSandboxLighting::LightManager::Instance()->RenderLightMesh(lrp);
	
	//ZShadeSandboxMesh::OBJMeshRenderParameters omrp;
	//omrp.camera = m_CameraSystem.get();
	//omrp.reflection = true;
	//omrp.clipplane = clipplane;
	//omrp.tessellate = true;
	//omrp.tessFactor = 64.0f;
	//omrp.renderType = ZShadeSandboxMesh::ERenderType::e3ControlPointPatchList;
	mMesh->SetFarPlane(m_EngineOptions->fFarPlane);
	mMesh->Render(mrp);
}
//===============================================================================================================================
void OutdoorEnvironment::RenderRefraction(XMFLOAT4 clipplane)
{
	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.camera = m_CameraSystem.get();
	mrp.light = mDirLight1;
	mrp.reflection = false;
	mrp.clipplane = clipplane;
	
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

	//
	// Render the terrain using the refraction clip plane to produce the refraction effect
	//

	if (bToggleTerrain)
		RenderTerrain(clipplane, false, true);
	
	if (mPickingSphere != NULL)
		mPickingSphere->Render(mrp);
	
	ZShadeSandboxLighting::LightRenderParameters lrp;
	lrp.camera = m_CameraSystem.get();
	lrp.clipplane = clipplane;
	lrp.reflect = false;
	lrp.renderDeferred = false;
	lrp.toggleMesh = true;
	lrp.toggleWireframe = bWireframeMode;
	ZShadeSandboxLighting::LightManager::Instance()->RenderLightMesh(lrp);
	
	//ZShadeSandboxMesh::OBJMeshRenderParameters omrp;
	//omrp.camera = m_CameraSystem.get();
	//omrp.reflection = false;
	//omrp.clipplane = clipplane;
	//omrp.tessellate = true;
	//omrp.tessFactor = 64.0f;
	//omrp.renderType = ZShadeSandboxMesh::ERenderType::e3ControlPointPatchList;
	mMesh->SetFarPlane(m_EngineOptions->fFarPlane);
	mMesh->Render(mrp);
}
//===============================================================================================================================
bool OutdoorEnvironment::RenderTerrainShadowSSAO()
{
	XMFLOAT4 fogColor;
	XMStoreFloat4(&fogColor, DXColors::Silver);

	ZShadeSandboxTerrain::TerrainShadingConst tsc;
	tsc.g_EyePosW = m_CameraSystem->Position();
	tsc.g_fogThinning = 1000;
	tsc.g_fogAltitude = 10;
	tsc.g_fogNear = 1.0f;
	tsc.g_fogFar = 1000.0f;
	tsc.g_fogColor = fogColor;
	tsc.g_TextureAmount = 1; // Amount of Layer textures (Not needed)
	tsc.g_seaLevel = fSeaLevel;
	tsc.g_waterBodyColor = XMFLOAT4(0.07f, 0.15f, 0.2f, 0.0f);

	float minTess = 0.0f;
	float maxTess = 64.0f;

	/*tsc.g_AmbientDown = mAmbientDown;
	tsc.g_AmbientLightCount = ZShadeSandboxLighting::LightManager::Instance()->AmbientLightCount();
	tsc.g_DirectionalLightCount = ZShadeSandboxLighting::LightManager::Instance()->DirectionalLightCount();
	tsc.g_SpotLightCount = ZShadeSandboxLighting::LightManager::Instance()->SpotLightCount();
	tsc.g_PointLightCount = ZShadeSandboxLighting::LightManager::Instance()->PointLightCount();
	tsc.g_CapsuleLightCount = ZShadeSandboxLighting::LightManager::Instance()->CapsuleLightCount();
	tsc.g_AmbientUp = mAmbientUp;*/
	tsc.g_useClippingPlane = 0;
	tsc.g_UseSobelFilter = 0;
	tsc.g_useNormalMap = (bToggleTerrainNormalMap == true) ? 1 : 0;
	tsc.g_useShadowMap = 0;// (bToggleTerrainShadow == true) ? 1 : 0;
	tsc.g_useSSAO = 0;// (bToggleTerrainSSAO == true) ? 1 : 0;
	tsc.g_MinDist = fMinDist;
	tsc.g_MaxDist = fMaxDist;
	tsc.g_MinTess = minTess;
	tsc.g_MaxTess = maxTess;
	tsc.g_FrustumCull = 0;
	tsc.g_HeightScale = fHeightScale;// gHeightScale;
	tsc.g_TerrSize = fTerrSize;// gTerrSize;
	tsc.g_TextureWidth = fMapSize;
	tsc.g_TextureHeight = fMapSize;
	tsc.g_TexelCellSpaceU = (1 / fMapSize);
	tsc.g_TexelCellSpaceV = (1 / fMapSize);
	tsc.g_ClipPlane = XMFLOAT4(0, 0, 0, 0);
	tsc.g_MapSize = fMapSize;
	tsc.g_DetailBrightness = 1.8f;

	m_pQuadTreeMesh->HeightScale() = fHeightScale;
	m_pQuadTreeMesh->TerrainZScale() = fTerrSize;

	// Render the shadow map for the terrain
	m_pQuadTreeRenderer->RenderShadowMap(m_CameraSystem.get(), mDirLight1->Perspective(), tsc);
	
	m_D3DSystem->SetBackBufferRenderTarget();
	// Pop RT and reset to the normal view
	m_D3DSystem->ResetViewport();
	
	m_pQuadTreeRenderer->RenderSSAO(m_CameraSystem.get(), mDirLight1->Perspective(), tsc);

	m_D3DSystem->SetBackBufferRenderTarget();
	// Pop RT and reset to the normal view
	m_D3DSystem->ResetViewport();

	return true;
}
//===============================================================================================================================
bool OutdoorEnvironment::RenderTerrain(XMFLOAT4 clipplane, bool bReflection, bool bRefraction)
{
	XMFLOAT4 fogColor;
	XMStoreFloat4(&fogColor, DXColors::Silver);

	ZShadeSandboxTerrain::TerrainShadingConst tsc;
	tsc.g_EyePosW = m_CameraSystem->Position();
	tsc.g_fogThinning = 1000;
	tsc.g_fogAltitude = 10;
	tsc.g_fogNear = 1.0f;
	tsc.g_fogFar = 1000.0f;
	tsc.g_fogColor = fogColor;
	tsc.g_TextureAmount = 1; // Amount of Layer textures (Not needed)
	tsc.g_seaLevel = fSeaLevel;
	tsc.g_waterBodyColor = XMFLOAT4(0.07f, 0.15f, 0.2f, 0.0f);
	
	float minTess = 0.0f;
	float maxTess = 64.0f;

	/*tsc.g_AmbientDown = mAmbientDown;
	tsc.g_AmbientLightCount = ZShadeSandboxLighting::LightManager::Instance()->AmbientLightCount();
	tsc.g_DirectionalLightCount = ZShadeSandboxLighting::LightManager::Instance()->DirectionalLightCount();
	tsc.g_SpotLightCount = ZShadeSandboxLighting::LightManager::Instance()->SpotLightCount();
	tsc.g_PointLightCount = ZShadeSandboxLighting::LightManager::Instance()->PointLightCount();
	tsc.g_CapsuleLightCount = ZShadeSandboxLighting::LightManager::Instance()->CapsuleLightCount();
	tsc.g_AmbientUp = mAmbientUp;*/
	tsc.g_useClippingPlane = ((bReflection || bRefraction) == true) ? 1 : 0;
	tsc.g_UseSobelFilter = 0;
	tsc.g_useNormalMap = (bToggleTerrainNormalMap == true) ? 1 : 0;
	tsc.g_useShadowMap = 0;// (bToggleTerrainShadow == true) ? 1 : 0;
	tsc.g_useSSAO = 0;// (bToggleTerrainSSAO == true) ? 1 : 0;
	tsc.g_MinDist = fMinDist;
	tsc.g_MaxDist = fMaxDist;
	tsc.g_MinTess = minTess;
	tsc.g_MaxTess = maxTess;
	tsc.g_FrustumCull = 0;
	tsc.g_HeightScale = fHeightScale;// gHeightScale;
	tsc.g_TerrSize = fTerrSize;// gTerrSize;
	tsc.g_TextureWidth = fMapSize;
	tsc.g_TextureHeight = fMapSize;
	tsc.g_TexelCellSpaceU = (1 / fMapSize);
	tsc.g_TexelCellSpaceV = (1 / fMapSize);
	tsc.g_ClipPlane = clipplane;
	tsc.g_MapSize = fMapSize;
	tsc.g_DetailBrightness = 1.8f;
	tsc.g_useReflection = bReflection;

	m_pQuadTreeMesh->HeightScale() = fHeightScale;
	m_pQuadTreeMesh->TerrainZScale() = fTerrSize;
	
	m_pQuadTreeRenderer->Render(m_CameraSystem.get(), mDirLight1, tsc);

	return true;
}
//===============================================================================================================================
bool OutdoorEnvironment::RenderWater()
{
	//
	//Render the Water
	//

	if (!bWireframeMode && !Quickwire()) m_D3DSystem->TurnOffCulling();
	if (Quickwire())
	{
		mWater->SetWireframe(true);
		m_D3DSystem->TurnOnWireframe();
	}
	mWater->SetFarPlane(m_EngineOptions->fFarPlane);
	mWater->Render(m_CameraSystem.get(), false, mReflectionTexture->SRView, mRefractionTexture->SRView);
	if (!bWireframeMode && !Quickwire()) m_D3DSystem->TurnOnCulling();

	return true;
}
//===============================================================================================================================
bool OutdoorEnvironment::RenderOcean()
{
	//
	//Render the Ocean
	//

	//m_pOcean->SetEnablePostProcessing(false);
	//m_pOcean->SetReflectionMap( mReflectionTexture->SRView );
	//m_pOcean->SetRefractionMap( mRefractionTexture->SRView );
	//m_pOcean->Update( m_Timer.ConstantTime() );
	//m_pOcean->Render(m_Timer.ConstantTime(), m_CameraSystem.get());

	return true;
}
//===============================================================================================================================