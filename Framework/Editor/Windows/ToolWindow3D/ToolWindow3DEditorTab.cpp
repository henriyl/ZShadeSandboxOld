#include "ToolWindow3DEditorTab.h"
//================================================================================================================
//================================================================================================================
ToolWindow3DEditorTab::ToolWindow3DEditorTab(HWND hwnd, EngineOptions* eo, D3D* d3d)
:   ToolWindow3DTab(hwnd, eo, d3d)
,   m_GameDirectoryCreated(false)
{
	EnableTab();
}
//================================================================================================================
ToolWindow3DEditorTab::~ToolWindow3DEditorTab()
{

}
//================================================================================================================
void ToolWindow3DEditorTab::BuildTab()
{
	int x = 10;
	int y = 40;

	HWND hwnd = m_hwnd;

	m_GameNameLabel.Init(hwnd, "Game Path", x, y, 90, 16, EEditorTypeID3D::eGameDirectoryNameLabel);
	m_GameNameTextbox.Init(hwnd, x + 100, y, 200, 20, EEditorTypeID3D::eGameDirectoryNameTextbox);

	y += 40;
	m_CreateGameDirectory.Init(hwnd, "Create Game Directory", x, y, 180, 30, EEditorTypeID3D::eCreateGameDirectory);
	y += 40;
	m_OpenGameDirectory.Init(hwnd, "Open Game Directory", x, y, 180, 30, EEditorTypeID3D::eOpenGameDirectory);

	//y += 70;
	//m_Undo.Init(hwnd, "Undo", x, y, 100, 30, UNDO);
	//y += 40;
	//m_Redo.Init(hwnd, "Redo", x, y, 100, 30, REDO);

	y += 70;
	m_GameTypeLabel.Init(hwnd, "Game Type", x, y, 100, 30, EEditorTypeID3D::eGameTypeLabel);
	y += 20;
	m_GameTypeRC.Init(hwnd, x, y, 0, 0, EEditorTypeID3D::eGameTypeRC);// 120, 80
	m_GameTypeRC.AddRadioButton(hwnd, true, "Top Down", x + 10, y + 10, 100, 30, EEditorTypeID3D::eGameTypeTopdown);
	m_GameTypeRC.AddRadioButton(hwnd, false, "Platformer", x + 10, y + 40, 100, 30, EEditorTypeID3D::eGameTypePlatformer);
	SendMessage(m_GameTypeRC.GetButtonHandle(0), BM_SETCHECK, BST_CHECKED, 0);

	x = 300;
	y = 100;
	m_EditModeLabel.Init(hwnd, "Edit Mode", x, y, 100, 30, EEditorTypeID3D::eEditModeLabel);
	y += 20;
	m_EditModes.Init(hwnd, x, y, 0, 0, EEditorTypeID3D::eEditModeRG);
	//y += 10;
	m_EditModes.AddRadioButton(hwnd, true, "Tile", x + 10, y, 110, 30, EEditorTypeID3D::eEditModeTile);
	y += 23;
	m_EditModes.AddRadioButton(hwnd, false, "Sprite", x + 10, y, 110, 30, EEditorTypeID3D::eEditModeSprite);
	y += 23;
	m_EditModes.AddRadioButton(hwnd, false, "Music", x + 10, y, 110, 30, EEditorTypeID3D::eEditModeMusic);
	y += 23;
	m_EditModes.AddRadioButton(hwnd, false, "Hardness", x + 10, y, 110, 30, EEditorTypeID3D::eEditModeHardness);
	y += 23;
	m_EditModes.AddRadioButton(hwnd, false, "Menu", x + 10, y, 110, 30, EEditorTypeID3D::eEditModeMenu);
	y += 23;
	m_EditModes.AddRadioButton(hwnd, false, "HUD", x + 10, y, 110, 30, EEditorTypeID3D::eEditModeHUD);
	SendMessage(m_EditModes.GetButtonHandle(0), BM_SETCHECK, BST_CHECKED, 0);
	y += 40;
	m_EditSubModeLabel.Init(hwnd, "Sub Mode", x, y, 100, 30, EEditorTypeID3D::eEditSubModeLabel);
	y += 30;
	m_EditModeSubPicker.Init(hwnd, x, y, 120, 100, EEditorTypeID3D::eEditModeCB);

	y += 70;
	m_EditorTypeLabel.Init(hwnd, "Editor Type", x, y, 100, 30, EEditorTypeID3D::eEditTypeLabel);
	y += 20;
	m_EditorTypeRC.Init(hwnd, x, y, 0, 0, EEditorTypeID3D::eEditTypeRC);//120, 110
	m_EditorTypeRC.AddRadioButton(hwnd, true, "Map", x + 10, y + 10, 100, 30, EEditorTypeID3D::eEditTypeMap);
	m_EditorTypeRC.AddRadioButton(hwnd, false, "Menu", x + 10, y + 40, 100, 30, EEditorTypeID3D::eEditTypeMenu);
	m_EditorTypeRC.AddRadioButton(hwnd, false, "HUD", x + 10, y + 70, 100, 30, EEditorTypeID3D::eEditTypeHUD);
	SendMessage(m_EditorTypeRC.GetButtonHandle(0), BM_SETCHECK, BST_CHECKED, 0);

	m_EditorTypeRC.SetVisible(true);
	m_EditorTypeRC.Enable(true);

	m_Play.Init(hwnd, "Play", 30, 500, 100, 30, EEditorTypeID3D::ePlay);

	bEditorTypeMap = true;
	bEditorTypeMenu = false;
	bEditorTypeHUD = false;
	bTileMode = true;
	bSpriteMode = false;
	bMusicMode = false;
	bHardnessMode = false;
}
//================================================================================================================
void ToolWindow3DEditorTab::DisableTab()
{
	m_enabled = false;


}
//================================================================================================================
void ToolWindow3DEditorTab::EnableTab()
{
	m_enabled = true;


}
//================================================================================================================
void ToolWindow3DEditorTab::SetGameTypeRC(string val)
{
	if (BetterString(val) == "Topdown")
	{
		m_platformer = false;
		m_EngineOptions->m_GameType2D = TOPDOWN;
		SendMessage(m_GameTypeRC.GetButtonHandle(0), BM_SETCHECK, BST_CHECKED, 0);
		SendMessage(m_GameTypeRC.GetButtonHandle(1), BM_SETCHECK, BST_UNCHECKED, 0);
		m_GameTypeRC.Enable(false);
	}
	else if (BetterString(val) == "Platformer")
	{
		m_platformer = true;
		m_EngineOptions->m_GameType2D = PLATFORMER;
		SendMessage(m_GameTypeRC.GetButtonHandle(0), BM_SETCHECK, BST_UNCHECKED, 0);
		SendMessage(m_GameTypeRC.GetButtonHandle(1), BM_SETCHECK, BST_CHECKED, 0);
		m_GameTypeRC.Enable(false);
	}
}
//================================================================================================================
void ToolWindow3DEditorTab::FillEditModeSubPicker()
{
	if (bEditorTypeMap)
	{
		if (bTileMode)
		{
			m_EditModeSubPicker.Reset();
			m_EditModeSubPicker.AddItem("Regular");
			m_EditModeSubPicker.AddItem("Multi Stamp");

			if (m_EngineOptions->m_GameType2D == PLATFORMER)
			{
				m_EditModeSubPicker.AddItem("Add Tile");
				m_EditModeSubPicker.AddItem("Remove Tile");
				m_EditModeSubPicker.AddItem("Remove Texture");
				m_EditModeSubPicker.AddItem("Rotate");
			}

			m_EditModeSubPicker.SetDisplayItem("Regular", 0);
		}

		if (bSpriteMode)
		{
			m_EditModeSubPicker.Reset();
			m_EditModeSubPicker.AddItem("None");
			m_EditModeSubPicker.AddItem("Move");
			m_EditModeSubPicker.AddItem("Delete");
			m_EditModeSubPicker.AddItem("Place");
			m_EditModeSubPicker.AddItem("Size");
			m_EditModeSubPicker.SetDisplayItem("None", 0);
		}

		if (bMusicMode || bHardnessMode)
		{
			m_EditModeSubPicker.Reset();
		}

		if (m_EngineOptions->m_GameType2D == PLATFORMER)
		{
			if (bHardnessMode)
			{
				m_EditModeSubPicker.Reset();
				m_EditModeSubPicker.AddItem("Tiles");
				m_EditModeSubPicker.AddItem("Sprites");
			}
		}
	}

	if (bMenuMode || bHUDMode)
	{
		//if (m_menuTab->MenuItemNone() || m_hudTab->HUDItemNone())
		//	m_EditModeSubPicker.Reset();

		//if (m_menuTab->MenuItemButton() || m_menuTab->MenuItemText() ||
		//	m_hudTab->HUDItemImage() || m_hudTab->HUDItemText())
		{
			m_EditModeSubPicker.Reset();
			m_EditModeSubPicker.AddItem("None");
			m_EditModeSubPicker.AddItem("Move");
			m_EditModeSubPicker.AddItem("Delete");
			m_EditModeSubPicker.AddItem("Place");
			m_EditModeSubPicker.AddItem("Size");
			m_EditModeSubPicker.SetDisplayItem("None", 0);
		}
	}
}
//================================================================================================================
void ToolWindow3DEditorTab::Update()
{
	/*ToolWindow2DTab::Update();
	
#pragma region "Editor Type Menu"
	if (bEditorTypeMenu)
	{
		EnableWindow(m_Play.GetHwnd(), false);

		// Update action attributes of a menu item for editing
		if (m_menuTab->MenuItemButton() || m_menuTab->MenuItemText())
		{
			string str = m_EditModeSubPicker.GetSelectedItem();
			if (str != "")
			{
				BetterString s(str);

				if (s == "None") m_menuEditorSystem->SwitchToActionNone();
				else if (s == "Move") m_menuEditorSystem->SwitchToActionMove();
				else if (s == "Delete") m_menuEditorSystem->SwitchToActionDelete();
				else if (s == "Place") m_menuEditorSystem->SwitchToActionPlace();
				else if (s == "Size") m_menuEditorSystem->SwitchToActionEditSize();
			}
		}
	}
#pragma endregion
#pragma region "Editor Type Map"
	if (bEditorTypeMap)
	{
		if (m_worldTab->WorldCreated())
		{
			if (m_mapEditorSystem2D->HasMaps(m_worldTab->ActiveWorldName()))
				EnableWindow(m_Play.GetHwnd(), true);
		}
		else
			EnableWindow(m_Play.GetHwnd(), false);

		if (bTileMode)
		{
			string str = m_EditModeSubPicker.GetSelectedItem();
			if (str != "")
			{
				BetterString s(str);

				if (s == "Regular") m_mapEditorSystem2D->SwitchToTileModeRegular();
				else if (s == "Multi Stamp") m_mapEditorSystem2D->SwitchToTileModeMultiStamp();
				else if (s == "Add Tile") m_mapEditorSystem2D->SwitchToTileModeAddTile();
				else if (s == "Remove Tile") m_mapEditorSystem2D->SwitchToTileModeRemoveTile();
				else if (s == "Remove Texture") m_mapEditorSystem2D->SwitchToTileModeRemoveTexture();
				else if (s == "Rotate") m_mapEditorSystem2D->SwitchToTileModeRotateTile();
			}
		}

		if (bSpriteMode)
		{
			string str = m_EditModeSubPicker.GetSelectedItem();
			if (str != "")
			{
				BetterString s(str);

				if (s == "None") m_mapEditorSystem2D->SwitchToSpriteModeNone();
				else if (s == "Move") m_mapEditorSystem2D->SwitchToSpriteModeMove();
				else if (s == "Delete") m_mapEditorSystem2D->SwitchToSpriteModeDelete();
				else if (s == "Place") m_mapEditorSystem2D->SwitchToSpriteModePlace();
				else if (s == "Size") m_mapEditorSystem2D->SwitchToSpriteModeSize();
			}
		}

		if (bHardnessMode)
		{
			if (m_EngineOptions->m_GameType2D == PLATFORMER)
			{
				string str = m_EditModeSubPicker.GetSelectedItem();
				if (str != "")
				{
					BetterString s(str);

					if (s == "Tiles") m_mapEditorSystem2D->SwitchToHardnessModeTiles();
					else if (s == "Sprites") m_mapEditorSystem2D->SwitchToHardnessModeSprites();
				}
			}
		}
	}
#pragma endregion
	*/
}
//================================================================================================================
LRESULT ToolWindow3DEditorTab::Command(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	/*
	if (!IsEnabled()) return true;

	//m_DriveLetters.UpdateCommand(wParam, lParam);
	m_EditModeSubPicker.UpdateCommand(wParam, lParam);

	bool gd_created = false;

	switch (LOWORD(wParam))
	{
	case CREATE_GD_BUTTON:
	{
		if (m_GameNameTextbox.GetText() == "")
		{
			ZShadeMessageCenter::MsgBoxOK(hwnd, "Must have a game name !!!", "Create Game Directory");
		}
		else
		{
			//string drive = m_DriveLetters.GetSelectedItem();

			//Full path of game directory
			string username = CGlobal::GrabUserName();
			BetterString gameFolder = "C:\\Users\\";
			gameFolder += username;
			gameFolder += "\\AppData\\Roaming\\";
			gameFolder += m_GameNameTextbox.GetText();

			m_gameDirectory = CGlobal::CreateGD2D(gameFolder, m_EngineOptions->m_GameType2D);

			// Everything will be loaded below if this flag is true
			gd_created = true;
		}
	}
	break;
	case OPEN_GD_BUTTON:
	{
		BROWSEINFO bi = { 0 };

		bi.lpszTitle = "Browse for Folder";
		bi.ulFlags = BIF_NEWDIALOGSTYLE | BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS;

		LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

		bool pathFound = false;

		if (pidl != NULL)
		{
			//TCHAR tszPath[MAX_PATH] = _T("\0");
			TCHAR tszPath[MAX_PATH];

			memset(tszPath, 0, MAX_PATH);

			if (SHGetPathFromIDList(pidl, tszPath) == TRUE)
			{
				m_GameNameTextbox.SetText(tszPath);

				pathFound = true;
			}

			// — Free pidl
			CoTaskMemFree(pidl);
		}

		if (pathFound)
		{
			m_gameDirectory = CGlobal::CreateGD2D(m_GameNameTextbox.GetText(), m_EngineOptions->m_GameType2D);

			// Everything will be loaded below if this flag is true
			gd_created = true;
		}
	}
	break;
	case EDIT_MODE_TILE:
	{
		bTileMode = true;
		bSpriteMode = false;
		bMusicMode = false;
		bHardnessMode = false;
		bMenuMode = false;
		bHUDMode = false;

		FillEditModeSubPicker();

		EnableWindow(m_EditModeSubPicker.GetHwnd(), true);
	}
	break;
	case EDIT_MODE_SPRITE:
	{
		bTileMode = false;
		bSpriteMode = true;
		bMusicMode = false;
		bHardnessMode = false;
		bMenuMode = false;
		bHUDMode = false;

		FillEditModeSubPicker();

		EnableWindow(m_EditModeSubPicker.GetHwnd(), true);
	}
	break;
	case EDIT_MODE_MUSIC:
	{
		bTileMode = false;
		bSpriteMode = false;
		bMusicMode = true;
		bHardnessMode = false;
		bMenuMode = false;
		bHUDMode = false;

		FillEditModeSubPicker();

		EnableWindow(m_EditModeSubPicker.GetHwnd(), false);
	}
	break;
	case EDIT_MODE_HARDNESS:
	{
		bTileMode = false;
		bSpriteMode = false;
		bMusicMode = false;
		bHardnessMode = true;
		bMenuMode = false;
		bHUDMode = false;

		FillEditModeSubPicker();

		EnableWindow(m_EditModeSubPicker.GetHwnd(), true);
	}
	break;
	case EDIT_MODE_MENU:
	{
		bTileMode = false;
		bSpriteMode = false;
		bMusicMode = false;
		bHardnessMode = false;
		bMenuMode = true;
		bHUDMode = false;

		FillEditModeSubPicker();

		EnableWindow(m_EditModeSubPicker.GetHwnd(), true);
	}
	break;
	case EDIT_MODE_HUD:
	{
		bTileMode = false;
		bSpriteMode = false;
		bMusicMode = false;
		bHardnessMode = false;
		bMenuMode = false;
		bHUDMode = true;

		FillEditModeSubPicker();

		EnableWindow(m_EditModeSubPicker.GetHwnd(), true);
	}
	break;
	case EDITOR_TYPE_MAP:
	{
		bEditorTypeMap = true;
		bEditorTypeMenu = false;
		bEditorTypeHUD = false;

		m_menuTab->MenuItemNone() = false;
		m_menuTab->MenuItemButton() = false;
		m_menuTab->MenuItemText() = false;

		m_hudTab->HUDItemNone() = false;
		m_hudTab->HUDItemImage() = false;
		m_hudTab->HUDItemText() = false;

		//m_EditModes.Enable(true);
		//m_EditModeSubPicker.Enable(true);

		EnableWindow(m_EditModes.GetButtonHandle(0), true);
		EnableWindow(m_EditModes.GetButtonHandle(1), true);
		EnableWindow(m_EditModes.GetButtonHandle(2), true);
		EnableWindow(m_EditModes.GetButtonHandle(3), true);
		EnableWindow(m_EditModes.GetButtonHandle(4), false);
		EnableWindow(m_EditModes.GetButtonHandle(5), false);

		GameState::GetInstance()->SetState(GS_INGAME);
	}
	break;
	case EDITOR_TYPE_MENU:
	{
		bEditorTypeMap = false;
		bEditorTypeMenu = true;
		bEditorTypeHUD = false;

		m_menuTab->MenuItemNone() = true;
		m_menuTab->MenuItemButton() = false;
		m_menuTab->MenuItemText() = false;

		m_hudTab->HUDItemNone() = false;
		m_hudTab->HUDItemImage() = false;
		m_hudTab->HUDItemText() = false;

		//m_EditModes.Enable(false);
		//m_EditModeSubPicker.Enable(false);

		EnableWindow(m_EditModes.GetButtonHandle(0), false);
		EnableWindow(m_EditModes.GetButtonHandle(1), false);
		EnableWindow(m_EditModes.GetButtonHandle(2), false);
		EnableWindow(m_EditModes.GetButtonHandle(3), false);
		EnableWindow(m_EditModes.GetButtonHandle(4), true);
		EnableWindow(m_EditModes.GetButtonHandle(5), false);

		GameState::GetInstance()->SetState(GS_MAINMENU);
	}
	break;
	case EDITOR_TYPE_HUD:
	{
		bEditorTypeMap = false;
		bEditorTypeMenu = false;
		bEditorTypeHUD = true;

		m_menuTab->MenuItemNone() = false;
		m_menuTab->MenuItemButton() = false;
		m_menuTab->MenuItemText() = false;

		m_hudTab->HUDItemNone() = true;
		m_hudTab->HUDItemImage() = false;
		m_hudTab->HUDItemText() = false;

		//m_EditModes.Enable(false);
		//m_EditModeSubPicker.Enable(false);

		EnableWindow(m_EditModes.GetButtonHandle(0), false);
		EnableWindow(m_EditModes.GetButtonHandle(1), false);
		EnableWindow(m_EditModes.GetButtonHandle(2), false);
		EnableWindow(m_EditModes.GetButtonHandle(3), false);
		EnableWindow(m_EditModes.GetButtonHandle(4), false);
		EnableWindow(m_EditModes.GetButtonHandle(5), true);

		// HUD editor is done in game mode since it is an overlay in game mode
		GameState::GetInstance()->SetState(GS_INGAME);
	}
	break;
	case PLAY:
	{
		// Get a new handle to the rendering m_Window for the new game environment
		HWND oldWindow = m_EngineOptions->hwnd;
		string oldWindowName = EnvironmentFactory::Instance()->ActiveEnvironmentName();
		m_EngineOptions->m_inEditor = false;

		//m_EngineOptions->hwnd = iRenderWin->GetHwnd();


		ZShadeINIParser iniParser("ZShadeSandbox.ini", true);
		string env3DType = iniParser.GetString("Editor", "Env3D");
		string game_folder = iniParser.GetString("Editor", "GameFolder");

		// Have the option of playing the current map or the current game
		const int result = ZShadeMessageCenter::MsgBoxYesNo(NULL, "Would you like to play the current map?", "Play Game");

		switch (result)
		{
		case IDYES:// Play current map
		{
			// Launch the current map for testing
			//MapTestEnvironment2D e(iWin.get(), iRenderWin.get(), m_Instance, m_EngineOptions, env3DType,
			//	m_ActiveWorldName, m_ActiveMapName, game_folder);

			// Enter the game loop
			//e.Run();

			// Launch the current map for testing
			EnvironmentFactory::Instance()->CreateEnvironment("MapTestEnv", m_EngineOptions, "BaseGameWindow", "RenderGameWindow", true);

			EnvironmentFactory::Instance()->ActiveEnvironmentName() = "MapTestEnv";

			string mn = m_worldTab->ActiveMapName();
			string wn = m_worldTab->ActiveWorldName();

			// Load the world and set the map for testing
			//Environment2DMapHelper::LoadWorld(m_ActiveWorldName, false, 0);
			Environment2DMapHelper::SetActiveRenderedMap(wn, mn);

			EnvironmentFactory::Instance()->SelectEnvironment("MapTestEnv")->Run();

			// Get the handle to the original m_Window back
			// when the game environment returns from the run loop command
			m_EngineOptions->hwnd = oldWindow;
			EnvironmentFactory::Instance()->ActiveEnvironmentName() = oldWindowName;
			m_EngineOptions->m_inEditor = true;
		}
		break;
		case IDNO:// Start the entire game
		{
			// Launch a new game environment
			//Environment e(m_EngineOptions, "BaseGameWindow", "RenderGameWindow", env3DType, game_folder);

			// Enter the game loop
			//e.Run();

			EnvironmentFactory::Instance()->CreateEnvironment("PlayTestEnv", m_EngineOptions, "BaseGameWindow", "RenderGameWindow", true);

			EnvironmentFactory::Instance()->ActiveEnvironmentName() = "PlayTestEnv";

			EnvironmentFactory::Instance()->SelectEnvironment("PlayTestEnv")->Run();

			// Get the handle to the original m_Window back
			// when the game environment returns from the run loop command
			m_EngineOptions->hwnd = oldWindow;
			EnvironmentFactory::Instance()->ActiveEnvironmentName() = oldWindowName;
			m_EngineOptions->m_inEditor = true;
		}
		break;
		}
	}
	break;
	}

	if (gd_created)
	{
		m_EngineOptions->m_GD2D = m_gameDirectory;

		m_mapEditorSystem2D->SetGameDirectory(m_gameDirectory);

		m_GameDirectoryCreated = true;

		m_EngineOptions->m_inEditor = true;

		FillEditModeSubPicker();

		// Load the stamp tools and display sprites
		m_mapEditorSystem2D->LoadRequiredSprites();

		EnableTabItems();

		// Read the Game.ini file
		ZShadeINIParser iniParser(m_gameDirectory->m_game_ini_path, false);
		string gameType = iniParser.GetString("GAME", "GameType");

		SetGameTypeRC(gameType);

		// Update the ini file with the game folder
		ZShadeINIParser iniSandboxParser("ZShadeSandbox.ini", true);
		iniSandboxParser.WriteString("Editor", "GameFolder", m_GameNameTextbox.GetText());

		//AudioSystem::NewInstance(m_gameDirectory, m_Window->GetHwnd());
		//Scripting::NewInstance(m_mapEditorSystem2D->D3DSystem(), m_gameDirectory);
	}
	*/
	return true;
}
//================================================================================================================