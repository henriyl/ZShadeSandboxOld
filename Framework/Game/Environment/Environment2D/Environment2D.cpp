#include "Environment2D.h"
#include "World.h"
#include "Convert.h"
#include "SectionAdjacency.h"
#include "SectorBuilder.h"
#include "ToolWindow2D.h"
#include "ToolWindowMenu.h"
#include "ToolWindowHUD.h"
#include "MapEditorSystem2D.h"
#include "MenuEditorSystem.h"
#include "GDI.h"
#include "Scripting.h"
#include "AudioSystem.h"
#include "Environment2DMapHelper.h"
#include "ShaderCompiler.h"
#include "DefaultMaterials.h"
#include "TextureManager.h"
#include "Shaders.h"
//================================================================================================================
//================================================================================================================
Environment2D::Environment2D(LPCSTR base_window_name, LPCSTR render_window_name, EngineOptions* engineOptions, bool init_lua)
:   EnvironmentBase(engineOptions, base_window_name, render_window_name)
,   m_EnvBaseInit(true)
,   m_ToolWindow2D(0)
,   m_MapEditorSystem2D(0)
,   m_HUDEditorSystem(0)
{
	Init(base_window_name, render_window_name, init_lua);
}
//================================================================================================================
bool Environment2D::Init(LPCSTR base_window_name, LPCSTR render_window_name, bool init_lua)
{
	m_WorldLoaded = false;
	m_MapLoaded = false;
	bInEditorMap = true;
	bInEditorMenu = false;
	bInEditorHUD = false;
	//bMapToolPrevShown = true;
	//bMapToolPrevHiden = false;
	//bMenuToolPrevShown = false;
	//bMenuToolPrevHiden = false;
	//bHUDToolPrevShown = false;
	//bHUDToolPrevHiden = false;
	
	m_D3DSystem = new D3D();
	if (!m_D3DSystem) return false;
	
	if (!m_D3DSystem->Initialize11(m_EngineOptions))
	{
		MessageBox(NULL, "Direct3D Init Failed", "Error", MB_OK);
		return false;
	}
	
	TextureManager::NewInstance(m_D3DSystem);
	
	ZShadeSandboxShader::Shaders::Build(m_D3DSystem);

	//Initialize the Camera
	m_CameraSystem = new Camera(m_EngineOptions);
	if (!m_CameraSystem) return false;
	m_CameraSystem->SetPosition(0.0f, 0.0f, -15.0f);
	ZShadeSandboxMath::XMMatrix world(XMMatrixIdentity());
	m_CameraSystem->SetWorld(world);
	m_CameraSystem->CreateOrtho(m_EngineOptions->m_screenWidth, m_EngineOptions->m_screenHeight, 0.1f, 1000.0f);

	//Initialize the menu system
	m_MenuSystem = new MenuSystem(m_D3DSystem);

	//Initialize the HUD system
	m_HUDSystem = new HUDSystem(m_D3DSystem);

	//The game state will be set to GS_NONE by default
	GameState::NewInstance();
	
	GDI::Init();
	
	m_Timer.Initialize();
	
	// Load the Game Directory since the game is starting from here
	if (init_lua)
	{
		m_EngineOptions->m_inEditor = false;

		//Load the game directory
		m_GameDirectory2D = CGlobal::LoadGameDirectory2D(m_EngineOptions->m_GameFolderName);
		
		m_EngineOptions->m_GD2D = m_GameDirectory2D;
		
		// Read the Game.ini file
		ZShadeINIParser iniParser(m_GameDirectory2D->m_game_ini_path, false);
		string gameType = iniParser.GetString("GAME", "GameType");

		if (BetterString(gameType) == "Topdown")
		{
			m_EngineOptions->m_GameType2D = TOPDOWN;
			
			// Initializes adjacency and sector information for a top-down tile game
			TileAdjInfo3x3 tileAdj3x3;
			TileAdjInfo6x6 tileAdj6x6;
			TileAdjInfo8x8 tileAdj8x8;
			TileAdjInfo10x10 tileAdj10x10;
			TileAdjInfo20x20 tileAdj20x20;
			TileAdjInfo30x30 tileAdj30x30;
			SectorBuilder sb;
		}
		else if (BetterString(gameType) == "Platformer")
		{
			m_EngineOptions->m_GameType2D = PLATFORMER;
		}

		// Create the default materials and add them to the material manager
		// I think this will fail in 2D because the game folder does not contain the textures that this builds
		DefaultMaterials materials;
		materials.Build(m_D3DSystem, m_EngineOptions->m_GameFolderName);

		//Load the Fog of War data
		FogData::NewInstance(m_D3DSystem);
		FogData::Instance()->InitializeTable();
		FogData::Instance()->LoadTextures(m_GameDirectory2D->m_required_textures);
		
		// This needs to only be done with a platformer
		m_Background = new PlatformerBackground(m_D3DSystem);
		m_Background->AddBackground("frontBackground.png", 1600, 768, false, true, 3, 0);
		
		// Initialize Audio System
		AudioSystem::NewInstance(m_GameDirectory2D, m_RenderWindow->GetHwnd());
		
		// Initialize lua scripting system
		Scripting::NewInstance(m_D3DSystem, m_GameDirectory2D);
	}
	else
	{
		m_ToolWindow2D = 0;
		m_MapEditorSystem2D = 0;
		
		m_EngineOptions->m_GD2D = 0;

		m_EngineOptions->m_inEditor = true;
		bInEditorMap = true;
		
		if (m_EngineOptions->m_GameType2D == TOPDOWN)
		{
			// Initializes adjacency and sector information for a top-down tile game
			TileAdjInfo3x3 tileAdj3x3;
			TileAdjInfo6x6 tileAdj6x6;
			TileAdjInfo8x8 tileAdj8x8;
			TileAdjInfo10x10 tileAdj10x10;
			TileAdjInfo20x20 tileAdj20x20;
			TileAdjInfo30x30 tileAdj30x30;
			SectorBuilder sb;
		}
	}
	
	return true;
}
//================================================================================================================
Environment2D::~Environment2D()
{
//	Shutdown();
}
//================================================================================================================
void Environment2D::Shutdown()
{
	GDI::Shutdown();

	m_D3DSystem->Shutdown();
}
//================================================================================================================
void Environment2D::OnMouseDown(WPARAM btnState, int x, int y)
{
	if (GameState::GetInstance()->GetState() == GS_MAINMENU ||
		GameState::GetInstance()->GetState() == GS_INGAMEMENU)
	{
		if (m_MenuSystem != NULL)
		{
			m_MenuSystem->UpdateButtonMouseOn(btnState, x, y);
			m_MenuSystem->UpdateButtonMouseDown(btnState, x, y);
		}

		if (m_MenuEditorSystem != NULL)
		{
			m_MenuEditorSystem->OnMouseDown(btnState, x, y);
		}
	}
	else if (GameState::GetInstance()->GetState() == GS_INGAME)
	{
		if (m_MapEditorSystem2D != NULL)
			m_MapEditorSystem2D->OnMouseDown(btnState, x, y);
	}
}
//================================================================================================================
void Environment2D::OnMouseUp(WPARAM btnState, int x, int y)
{
	if (GameState::GetInstance()->GetState() == GS_MAINMENU ||
		GameState::GetInstance()->GetState() == GS_INGAMEMENU)
	{
		if (m_MenuSystem != NULL)
		{
			m_MenuSystem->UpdateButtonMouseOn(btnState, x, y);
			m_MenuSystem->UpdateButtonMouseUp(btnState, x, y);
		}

		if (m_MenuEditorSystem != NULL)
		{
			m_MenuEditorSystem->OnMouseUp(btnState, x, y);
		}
	}
	else if (GameState::GetInstance()->GetState() == GS_INGAME)
	{
		if (m_MapEditorSystem2D != NULL)
			m_MapEditorSystem2D->OnMouseUp(btnState, x, y);
	}
}
//================================================================================================================
void Environment2D::OnMouseMove(WPARAM btnState, int x, int y)
{
	if (GameState::GetInstance()->GetState() == GS_MAINMENU ||
		GameState::GetInstance()->GetState() == GS_INGAMEMENU)
	{
		if (m_MenuSystem != NULL)
		{
			m_MenuSystem->UpdateButtonMouseOn(btnState, x, y);
			m_MenuSystem->UpdateButtonMouseMove(btnState, x, y);
		}

		if (m_MenuEditorSystem != NULL)
		{
			m_MenuEditorSystem->OnMouseMove(btnState, x, y);
		}
	}
	else if (GameState::GetInstance()->GetState() == GS_INGAME)
	{
		if (m_MapEditorSystem2D != NULL)
			m_MapEditorSystem2D->OnMouseMove(btnState, x, y);
	}
}
//================================================================================================================
bool Environment2D::Update()
{
	/*if (m_EngineOptions->m_inEditor)
	{
		m_TabEditWindow2D->Update();

		if (m_TabEditWindow2D->InFocus())
		{
			return true;
		}
	}*/
	
	// Toggles the editor windows
	if (m_EngineOptions->m_inEditor)
	{
		m_ToolWindow2D->Update();
		
		// clicked on the To Map Button in ToolWindowMenu
		if (m_menuTool->DoHideWindow())
		{
			m_menuTool->HideWindow();
			m_menuTool->SetDoHideWindow(false);
			
			// Display m_ToolWindow2D window
			m_ToolWindow2D->ShowWindow();
		}
		
		// clicked on the To Map Button in ToolWindowHUD
		if (m_hudTool->DoHideWindow())
		{
			m_hudTool->HideWindow();
			m_hudTool->SetDoHideWindow(false);
			
			// Display m_ToolWindow2D window
			m_ToolWindow2D->ShowWindow();
		}
		
		// clicked on the To Menu Button in m_ToolWindow2D
		if (m_ToolWindow2D->IsEditorTypeMenu())
		{
			if (m_ToolWindow2D->DoHideWindow())
			{
				m_ToolWindow2D->HideWindow();
				m_ToolWindow2D->SetDoHideWindow(false);
				
				// Display m_menuTool window
				m_menuTool->ShowWindow();
			}
		}
		
		// clicked on the To HUD Button in m_ToolWindow2D
		if (m_ToolWindow2D->IsEditorTypeHUD())
		{
			if (m_ToolWindow2D->DoHideWindow())
			{
				m_ToolWindow2D->HideWindow();
				m_ToolWindow2D->SetDoHideWindow(false);
				
				// Display m_menuTool window
				m_hudTool->ShowWindow();
			}
		}
	}
	
	m_Timer.Frame();
	
	m_CameraSystem->Update();
	
	keyboard = Keyboard::GetKeyboard();
	mouse = Mouse::GetMouse();
	
	if (Scripting::Instance() != NULL)
		Scripting::Instance()->SetActiveWorldName(m_ActiveWorldName);
	if (Scripting::Instance() != NULL)
		Scripting::Instance()->SetActiveMapName(m_ActiveMapName);

	// Update all the GUI objects
	//for (UINT_PTR i = 0; i < m_GuiObjects.size(); ++i)
	//	m_GuiObjects[i]->Update(pt.x, pt.y, mouse.LButton.Pressed);
	
	//m_MapEditorSystem2D->UpdateStampSprites();

	bool update = true;
	
	if (GameState::GetInstance()->GetState() == GS_QUIT)
	{
		ExitProcess(0);
	}
	else if (GameState::GetInstance()->GetState() == GS_MAINMENU ||
			 GameState::GetInstance()->GetState() == GS_INGAMEMENU)
	{
		if (m_EngineOptions->m_inEditor)
		{
			//if (m_TabEditWindow2D->IsEditorTypeMenu())
			//	;// m_MenuEditorSystem->UpdateInput(keyboard);
		}
	}
	else if (GameState::GetInstance()->GetState() == GS_INGAME)
	{
		//if (m_EngineOptions->m_inEditor)
		//{
		//	if (m_TabEditWindow2D->IsEditorTypeMap())
		//		m_MapEditorSystem2D->UpdateInput(keyboard);
		//}
		//
		//// If we are in the editor in a map and a topdown map is in section picker mode
		//// then disable updating the map
		//if (m_EngineOptions->m_inEditor)
		//{
		//	if (m_TabEditWindow2D->IsEditorTypeMap())
		//	{
		//		if (m_MapEditorSystem2D->IsInSectionPicker())
		//		{
		//			update = false;
		//		}
		//	}
		//}
		
		// MAKING SURE THAT IF I'M IN MAP, MENU OR HUD MODE THAT EACH GET UPDATED AND RENDERED ACCORDINGLY
		
		// Update all the world and physics objects
		if (update)
		{
			if (m_WorldLoaded && m_MapLoaded)
			{
				if (m_EngineOptions->m_GameType2D == TOPDOWN)
				{
					World* world = GetTopdownWorld(m_ActiveWorldName);

					world->LoadSpritesMain();

					// Do we need to know when map is in section picker ?
					world->Update(m_Timer.DeltaSecondsF(), keyboard, m_RenderWindow->GetHwnd(), true);
				}
				else if (m_EngineOptions->m_GameType2D == PLATFORMER)
				{
					PlatformerWorld* world = GetPlatformerWorld(m_ActiveWorldName);

					world->LoadSpritesMain();

					if (m_EngineOptions->m_inEditor)
						world->UpdateScrollOffset(m_MapEditorSystem2D->GetScrollOffset());

					world->Update(m_Timer.DeltaSecondsF(), keyboard, m_RenderWindow->GetHwnd());
				}
			}
		}
		
		if (m_Background != NULL)
			m_Background->Update();
	}

	return true;
}
//================================================================================================================
bool Environment2D::RenderMaster()
{
	bool result = true;

	//If the game directory is not created yet make sure I cannot do anything until I create a game directory
	if (m_GameDirectory2D == NULL)
		return true;
	if (m_GameDirectory2D->GAME_FOLDER == "")
		return true;

	if (GameState::GetInstance()->GetState() == GS_MAINMENU ||
		GameState::GetInstance()->GetState() == GS_INGAMEMENU)
	{
		if (m_MenuSystem != NULL)
		{
			m_D3DSystem->TurnOffZBuffer();
			{
				m_MenuSystem->Render(m_CameraSystem);

				if (m_MenuEditorSystem != NULL)
				{
					m_MenuEditorSystem->Render(m_CameraSystem);
				}
			}
			m_D3DSystem->TurnOnZBuffer();
		}
	}
	else if (GameState::GetInstance()->GetState() == GS_INGAME)
	{
		//Turn z-buffer off for 2D Rendering
		m_D3DSystem->TurnOffZBuffer();
		{
			if (m_WorldLoaded && m_MapLoaded)
			{
				if (m_EngineOptions->m_GameType2D == TOPDOWN)
				{
					World* world = GetTopdownWorld(m_ActiveWorldName);

					// If we are in the editor
					if (m_EngineOptions->m_inEditor)
					{
						/*if (m_MapEditorSystem2D->IsInSectionPicker())
						{
							world->RenderSectionPicker(m_CameraSystem.get());

							m_MapEditorSystem2D->RenderStampSprites(m_CameraSystem.get());
						}
						else
						{
							world->Render(m_CameraSystem.get());

							m_MapEditorSystem2D->RenderStampSprites(m_CameraSystem.get());
						}*/
					}
					else// Just render the game
					{
						world->Render(m_CameraSystem);
					}
				}
				else if (m_EngineOptions->m_GameType2D == PLATFORMER)
				{
					PlatformerWorld* world = GetPlatformerWorld(m_ActiveWorldName);

					int mapWidth = 0;

					if (world->MapToRender() != "")
					{
						if (world->GetMap(world->MapToRender()) != NULL)
							mapWidth = world->GetMap(world->MapToRender())->Width();
					}

					if (m_Background != NULL)
						m_Background->Render(m_CameraSystem, mapWidth, 0);

					world->Render(m_CameraSystem);

					//if (m_EngineOptions->m_inEditor)
					//	m_MapEditorSystem2D->RenderStampSprites(m_CameraSystem.get());
				}
			}

			//// Render all the GUI objects
			//for (UINT_PTR i = 0; i < m_GuiObjects.size(); ++i)
			//	m_GuiObjects[i]->Render(m_SpriteGUI, FilterMode::Point);

			//POINT pt;
			//GetCursorPos(&pt);
			//ScreenToClient(m_Window->GetHwnd(), &pt);

			//text1Transform = XMMatrixTranslation(2, 2, 0);
			//text1Transform = XMMatrixMultiply(text1Transform, XMMatrixScaling(2, 2, 0));
			//text1.Render11(text1Transform, XMFLOAT4(1, 0, 0, 1));
		}
		m_D3DSystem->TurnOnZBuffer();
	}

	return result;
}
//================================================================================================================
int Environment2D::Run()
{
	if (m_EngineOptions->m_inEditor)
	{
		while (m_BaseEditWindow->IsAlive())
		{
			if (!m_BaseEditWindow->WinIsMinimized())
			{
				if (!Update()) break;

				if (m_D3DSystem->BeginScene(0.0f, 0.2f, 0.4f, 1.0f))
				{
					//Rendering Happens Here
					if (!RenderMaster()) break;

					m_D3DSystem->EndScene();
				}

				//Quick esc for the game
				if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
				{
					//Exit();
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
				if (!Update()) break;

				if (m_D3DSystem->BeginScene(0.0f, 0.2f, 0.4f, 1.0f))
				{
					//Rendering Happens Here
					if (!RenderMaster()) break;

					m_D3DSystem->EndScene();
				}

				//Quick esc for the game
				if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
				{
					//Exit();
					break;
				}
			}

			m_RenderWindow->Run();
		}
	}

	return 1;
}
//================================================================================================================
void Environment2D::RemoveMap(string activeMapName, string activeWorldName)
{
	if (m_EngineOptions->m_GameType2D == TOPDOWN)
	{
		map<string, SWorld*>::iterator iter = m_WorldsTopdown.begin();

		bool found = false;

		for (; iter != m_WorldsTopdown.end(); iter++)
		{
			if ((*iter).first == activeWorldName)
			{
				World* world = (*iter).second->m_World;

				world->RemoveMap(activeMapName);

				vector<string, string>::iterator MapNameIter = (*iter).second->m_mapNames.begin();
				vector<string, string>::iterator KillIter;

				for (; MapNameIter != (*iter).second->m_mapNames.end(); MapNameIter++)
				{
					string name = (*MapNameIter);

					if (BetterString(name) == BetterString(activeMapName))
					{
						KillIter = MapNameIter;
						found = true;
						break;
					}
				}

				if (found)
				{
					(*iter).second->m_mapNames.erase(KillIter);
				}

				break;
			}
		}
	}
	else if (m_EngineOptions->m_GameType2D == PLATFORMER)
	{
		map<string, SPlatformerWorld*>::iterator iter = m_WorldsPlatformer.begin();

		bool found = false;

		for (; iter != m_WorldsPlatformer.end(); iter++)
		{
			if ((*iter).first == activeWorldName)
			{
				PlatformerWorld* world = (*iter).second->m_World;

				world->RemoveMap(activeMapName);

				vector<string, string>::iterator MapNameIter = (*iter).second->m_mapNames.begin();
				vector<string, string>::iterator KillIter;

				for (; MapNameIter != (*iter).second->m_mapNames.end(); MapNameIter++)
				{
					string name = (*MapNameIter);

					if (BetterString(name) == BetterString(activeMapName))
					{
						KillIter = MapNameIter;
						found = true;
						break;
					}
				}

				if (found)
				{
					(*iter).second->m_mapNames.erase(KillIter);
				}

				break;
			}
		}
	}
}
//================================================================================================================
void Environment2D::GrabWorldNames(vector<string>& names)
{
	if (m_EngineOptions->m_GameType2D == TOPDOWN)
	{
		map<string, SWorld*>::iterator iter = m_WorldsTopdown.begin();

		for (; iter != m_WorldsTopdown.end(); iter++)
		{
			names.push_back((*iter).first);
		}
	}
	else if (m_EngineOptions->m_GameType2D == PLATFORMER)
	{
		map<string, SPlatformerWorld*>::iterator iter = m_WorldsPlatformer.begin();

		for (; iter != m_WorldsPlatformer.end(); iter++)
		{
			names.push_back((*iter).first);
		}
	}
}
//================================================================================================================
void Environment2D::GrabMapNames(string world_name, vector<string>& names)
{
	if (m_EngineOptions->m_GameType2D == TOPDOWN)
	{
		map<string, SWorld*>::iterator iter = m_WorldsTopdown.begin();

		for (; iter != m_WorldsTopdown.end(); iter++)
		{
			if ((*iter).first == world_name)
			{
				for (int i = 0; i < (*iter).second->m_mapNames.size(); i++)
					names.push_back((*iter).second->m_mapNames[i]);
			}
		}
	}
	else if (m_EngineOptions->m_GameType2D == PLATFORMER)
	{
		map<string, SPlatformerWorld*>::iterator iter = m_WorldsPlatformer.begin();

		for (; iter != m_WorldsPlatformer.end(); iter++)
		{
			if ((*iter).first == world_name)
			{
				for (int i = 0; i < (*iter).second->m_mapNames.size(); i++)
					names.push_back((*iter).second->m_mapNames[i]);
			}
		}
	}
}
//================================================================================================================
void Environment2D::LoadWorld(string world_name, SWorld* sw)
{
	m_WorldsTopdown[world_name] = sw;
}
//================================================================================================================
void Environment2D::LoadWorld(string world_name, SPlatformerWorld* sw)
{
	m_WorldsPlatformer[world_name] = sw;
}
//================================================================================================================
World* Environment2D::GetTopdownWorld(string world_name)
{
	SWorld* sw = NULL;

	//Get a copy of the world if it exists
	if (m_WorldsTopdown[world_name] != NULL)
	{
		sw = m_WorldsTopdown[world_name];
	}

	if (sw == NULL) return NULL;

	return sw->m_World;
}
//================================================================================================================
PlatformerWorld* Environment2D::GetPlatformerWorld(string world_name)
{
	SPlatformerWorld* sw = NULL;

	//Get a copy of the world if it exists
	if (m_WorldsPlatformer[world_name] != NULL)
	{
		sw = m_WorldsPlatformer[world_name];
	}

	if (sw == NULL) return NULL;

	return sw->m_World;
}
//================================================================================================================
SWorld* Environment2D::GetSWorld(string world_name)
{
	SWorld* sw = NULL;

	//Get a copy of the world if it exists
	if (m_WorldsTopdown[world_name] != NULL)
	{
		sw = m_WorldsTopdown[world_name];
	}

	if (sw == NULL) return NULL;

	return sw;
}
//================================================================================================================
SPlatformerWorld* Environment2D::GetSPlatformerWorld(string world_name)
{
	SPlatformerWorld* sw = NULL;

	//Get a copy of the world if it exists
	if (m_WorldsPlatformer[world_name] != NULL)
	{
		sw = m_WorldsPlatformer[world_name];
	}

	if (sw == NULL) return NULL;

	return sw;
}
//================================================================================================================