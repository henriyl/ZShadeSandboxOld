#include "ToolWindow2DEditorTab.h"
#include "EditorTypeID.h"
#include "EnvironmentFactory.h"
//================================================================================================================
//================================================================================================================
ToolWindow2DEditorTab::ToolWindow2DEditorTab(HWND hwnd, EngineOptions* eo, D3D* d3d)
:   ToolWindow2DTab(hwnd, eo, d3d)
,   m_GameDirectoryCreated(false)
{
	BuildTab();
	EnableTab();
}
//================================================================================================================
ToolWindow2DEditorTab::~ToolWindow2DEditorTab()
{

}
//================================================================================================================
void ToolWindow2DEditorTab::BuildTab()
{
	int x = 10;
	int y = 40;

	HWND hwnd = m_hwnd;

	m_GamePathLabel.Init(hwnd, "Game Path", x, y, 90, 16, EEditorTypeID::eEditorTabGamePathLabel);
	m_GamePathLabel2.Init(hwnd, "", x + 100, y, 200, 16, EEditorTypeID::eEditorTabGamePathLabel2);

	y = 100;
	m_GameNameLabel.Init(hwnd, "Game Name", x, y, 90, 16, EEditorTypeID::eEditorTabGameDirectoryNameLabel);
	m_GameNameTextbox.Init(hwnd, x + 100, y, 100, 20, EEditorTypeID::eEditorTabGameDirectoryNameTextbox);

	y += 40;
	m_CreateGameDirectory.Init(hwnd, "Create Game Directory", x, y, 180, 30, EEditorTypeID::eEditorTabCreateGameDirectory);
	y += 40;
	m_OpenGameDirectory.Init(hwnd, "Open Game Directory", x, y, 180, 30, EEditorTypeID::eEditorTabOpenGameDirectory);

	//y += 70;
	//m_Undo.Init(hwnd, "Undo", x, y, 100, 30, UNDO);
	//y += 40;
	//m_Redo.Init(hwnd, "Redo", x, y, 100, 30, REDO);

	//x = 400;
	y += 70;
	//y = 30;
	m_GameTypeLabel.Init(hwnd, "Game Type", x, y, 100, 30, EEditorTypeID::eEditorTabGameTypeLabel);
	y += 10;
	m_GameTypeRC.Init(hwnd, x, y, 0, 0, EEditorTypeID::eEditorTabGameTypeRadioContainer);// 120, 80
	m_GameTypeRC.AddRadioButton(hwnd, true, "Top Down", x + 10, y + 10, 100, 30, EEditorTypeID::eEditorTabGameTypeTopdown);
	m_GameTypeRC.AddRadioButton(hwnd, false, "Platformer", x + 10, y + 40, 100, 30, EEditorTypeID::eEditorTabGameTypePlatformer);
	SendMessage(m_GameTypeRC.GetButtonHandle(0), BM_SETCHECK, BST_CHECKED, 0);

	//x = 300;
	x = 30;
	y = 360;
	m_EditModeLabel.Init(hwnd, "Edit Mode", x, y, 100, 30, EEditorTypeID::eEditorTabEditModeLabel);
	y += 20;
	m_EditModes.Init(hwnd, x, y, 0, 0, EEditorTypeID::eEditorTabEditModeRadioContainer);
	//y += 10;
	m_EditModes.AddRadioButton(hwnd, true, "Tile", x + 10, y, 110, 30, EEditorTypeID::eEditorTabEditModeTile);
	y += 23;
	m_EditModes.AddRadioButton(hwnd, false, "Sprite", x + 10, y, 110, 30, EEditorTypeID::eEditorTabEditModeSprite);
	y += 23;
	m_EditModes.AddRadioButton(hwnd, false, "Music", x + 10, y, 110, 30, EEditorTypeID::eEditorTabEditModeMusic);
	y += 23;
	m_EditModes.AddRadioButton(hwnd, false, "Hardness", x + 10, y, 110, 30, EEditorTypeID::eEditorTabEditModeHardness);
	SendMessage(m_EditModes.GetButtonHandle(0), BM_SETCHECK, BST_CHECKED, 0);

	x = 200;
	y = 360;
	m_EditSubModeTopdownTileLabel.Init(hwnd, "Edit Sub Mode Topdown Tile", x, y, 300, 30, EEditorTypeID::eEditorTabEditSubModeTopdownTileLabel);
	y += 20;
	m_EditModesSubModeTopdownTileRadioContainer.Init(hwnd, x, y, 0, 0, EEditorTypeID::eEditorTabEditSubModeTopdownTileRadioContainer);
	//y += 10;
	m_EditModesSubModeTopdownTileRadioContainer.AddRadioButton(hwnd, true, "Regular", x + 10, y, 110, 30, EEditorTypeID::eEditorTabEditSubModeTopdownTileRegular);
	y += 23;
	m_EditModesSubModeTopdownTileRadioContainer.AddRadioButton(hwnd, false, "Multi-Stamp", x + 10, y, 110, 30, EEditorTypeID::eEditorTabEditSubModeTopdownTileMultiStamp);
	SendMessage(m_EditModesSubModeTopdownTileRadioContainer.GetButtonHandle(0), BM_SETCHECK, BST_CHECKED, 0);

	x = 200;
	y = 360;
	m_EditSubModePlatformerTileLabel.Init(hwnd, "Edit Sub Mode Platformer Tile", x, y, 200, 30, EEditorTypeID::eEditorTabEditSubModePlatformerTileLabel);
	y += 20;
	m_EditModesSubModePlatformerTileRadioContainer.Init(hwnd, x, y, 0, 0, EEditorTypeID::eEditorTabEditSubModePlatformerTileRadioContainer);
	//y += 10;
	m_EditModesSubModePlatformerTileRadioContainer.AddRadioButton(hwnd, true, "Regular", x + 10, y, 110, 30, EEditorTypeID::eEditorTabEditSubModePlatformerTileRegular);
	y += 23;
	m_EditModesSubModePlatformerTileRadioContainer.AddRadioButton(hwnd, false, "Multi-Stamp", x + 10, y, 110, 30, EEditorTypeID::eEditorTabEditSubModePlatformerTileMultiStamp);
	y += 23;
	m_EditModesSubModePlatformerTileRadioContainer.AddRadioButton(hwnd, false, "Add Tile", x + 10, y, 110, 30, EEditorTypeID::eEditorTabEditSubModePlatformerTileAddTile);
	y += 23;
	m_EditModesSubModePlatformerTileRadioContainer.AddRadioButton(hwnd, false, "Remove Tile", x + 10, y, 110, 30, EEditorTypeID::eEditorTabEditSubModePlatformerTileRemoveTile);
	y += 23;
	m_EditModesSubModePlatformerTileRadioContainer.AddRadioButton(hwnd, false, "Remove Texture", x + 10, y, 150, 30, EEditorTypeID::eEditorTabEditSubModePlatformerTileRemoveTexture);
	SendMessage(m_EditModesSubModePlatformerTileRadioContainer.GetButtonHandle(0), BM_SETCHECK, BST_CHECKED, 0);

	x = 200;
	y = 360;
	m_EditSubModeSpriteLabel.Init(hwnd, "Edit Sub Mode Sprite", x, y, 150, 30, EEditorTypeID::eEditorTabEditSubModeSpriteLabel);
	y += 20;
	m_EditModesSubModeSpriteRadioContainer.Init(hwnd, x, y, 0, 0, EEditorTypeID::eEditorTabEditSubModeSpriteRadioContainer);
	//y += 10;
	m_EditModesSubModeSpriteRadioContainer.AddRadioButton(hwnd, true, "None", x + 10, y, 110, 30, EEditorTypeID::eEditorTabEditSubModeSpriteNone);
	y += 23;
	m_EditModesSubModeSpriteRadioContainer.AddRadioButton(hwnd, false, "Move", x + 10, y, 110, 30, EEditorTypeID::eEditorTabEditSubModeSpriteMove);
	y += 23;
	m_EditModesSubModeSpriteRadioContainer.AddRadioButton(hwnd, false, "Delete", x + 10, y, 110, 30, EEditorTypeID::eEditorTabEditSubModeSpriteDelete);
	y += 23;
	m_EditModesSubModeSpriteRadioContainer.AddRadioButton(hwnd, false, "Place", x + 10, y, 110, 30, EEditorTypeID::eEditorTabEditSubModeSpritePlace);
	y += 23;
	m_EditModesSubModeSpriteRadioContainer.AddRadioButton(hwnd, false, "Size", x + 10, y, 110, 30, EEditorTypeID::eEditorTabEditSubModeSpriteSize);
	SendMessage(m_EditModesSubModeSpriteRadioContainer.GetButtonHandle(0), BM_SETCHECK, BST_CHECKED, 0);

	x = 200;
	y = 360;
	m_EditSubModeHardnessLabel.Init(hwnd, "Edit Sub Mode Hardness", x, y, 200, 30, EEditorTypeID::eEditorTabEditSubModeHardnessLabel);
	y += 20;
	m_EditModesSubModeHardnessRadioContainer.Init(hwnd, x, y, 0, 0, EEditorTypeID::eEditorTabEditSubModeHardnessRadioContainer);
	//y += 10;
	m_EditModesSubModeHardnessRadioContainer.AddRadioButton(hwnd, true, "Tiles", x + 10, y, 110, 30, EEditorTypeID::eEditorTabEditSubModeHardnessTiles);
	y += 23;
	m_EditModesSubModeHardnessRadioContainer.AddRadioButton(hwnd, false, "Sprites", x + 10, y, 110, 30, EEditorTypeID::eEditorTabEditSubModeHardnessSprites);
	SendMessage(m_EditModesSubModeHardnessRadioContainer.GetButtonHandle(0), BM_SETCHECK, BST_CHECKED, 0);

	y += 300;
	m_MenuButton.Init(hwnd, "To Menu Editor", 30, y, 150, 30, EEditorTypeID::eEditorTabMenuButton);
	m_HUDButton.Init(hwnd, "To HUD Editor", 40 + 150, y, 150, 30, EEditorTypeID::eEditorTabHUDButton);
	
	y += 40;
	m_Play.Init(hwnd, "Play", 30, y, 100, 30, EEditorTypeID::eEditorTabPlay);

	m_EditSubModeTopdownTileLabel.SetVisible(false);
	m_EditModesSubModeTopdownTileRadioContainer.SetVisible(false);
	m_EditSubModePlatformerTileLabel.SetVisible(false);
	m_EditModesSubModePlatformerTileRadioContainer.SetVisible(false);
	m_EditSubModeSpriteLabel.SetVisible(false);
	m_EditModesSubModeSpriteRadioContainer.SetVisible(false);
	m_EditSubModeHardnessLabel.SetVisible(false);
	m_EditModesSubModeHardnessRadioContainer.SetVisible(false);

	m_platformer = false;
	bEditorTypeMap = true;
	bEditorTypeMenu = false;
	bEditorTypeHUD = false;
	bTileMode = true;
	bSpriteMode = false;
	bMusicMode = false;
	bHardnessMode = false;
}
//================================================================================================================
void ToolWindow2DEditorTab::DisableTab()
{
	ToolWindow2DTab::DisableTab();
	DisableEditSubModeType();
}
//================================================================================================================
void ToolWindow2DEditorTab::EnableTab()
{
	ToolWindow2DTab::EnableTab();
	UpdateEditSubModeTypeVisible();
	UpdateEditSubModeTypeEnable();
}
//================================================================================================================
void ToolWindow2DEditorTab::UpdateEditSubModeTypeVisible()
{
	if (bTileMode)
	{
		if (!m_platformer)
		{
			m_EditSubModeTopdownTileLabel.SetVisible(true);
			m_EditModesSubModeTopdownTileRadioContainer.SetVisible(true);

			m_EditSubModePlatformerTileLabel.SetVisible(false);
			m_EditModesSubModePlatformerTileRadioContainer.SetVisible(false);
			m_EditSubModeSpriteLabel.SetVisible(false);
			m_EditModesSubModeSpriteRadioContainer.SetVisible(false);
			m_EditSubModeHardnessLabel.SetVisible(false);
			m_EditModesSubModeHardnessRadioContainer.SetVisible(false);
		}
		else
		{
			m_EditSubModePlatformerTileLabel.SetVisible(true);
			m_EditModesSubModePlatformerTileRadioContainer.SetVisible(true);

			m_EditSubModeTopdownTileLabel.SetVisible(false);
			m_EditModesSubModeTopdownTileRadioContainer.SetVisible(false);
			m_EditSubModeSpriteLabel.SetVisible(false);
			m_EditModesSubModeSpriteRadioContainer.SetVisible(false);
			m_EditSubModeHardnessLabel.SetVisible(false);
			m_EditModesSubModeHardnessRadioContainer.SetVisible(false);
		}
	}

	if (bSpriteMode)
	{
		m_EditSubModeSpriteLabel.SetVisible(true);
		m_EditModesSubModeSpriteRadioContainer.SetVisible(true);

		m_EditSubModeTopdownTileLabel.SetVisible(false);
		m_EditModesSubModeTopdownTileRadioContainer.SetVisible(false);
		m_EditSubModePlatformerTileLabel.SetVisible(false);
		m_EditModesSubModePlatformerTileRadioContainer.SetVisible(false);
		m_EditSubModeHardnessLabel.SetVisible(false);
		m_EditModesSubModeHardnessRadioContainer.SetVisible(false);
	}

	if (bHardnessMode)
	{
		m_EditSubModeHardnessLabel.SetVisible(true);
		m_EditModesSubModeHardnessRadioContainer.SetVisible(true);

		m_EditSubModeTopdownTileLabel.SetVisible(false);
		m_EditModesSubModeTopdownTileRadioContainer.SetVisible(false);
		m_EditSubModePlatformerTileLabel.SetVisible(false);
		m_EditModesSubModePlatformerTileRadioContainer.SetVisible(false);
		m_EditSubModeSpriteLabel.SetVisible(false);
		m_EditModesSubModeSpriteRadioContainer.SetVisible(false);
	}
}
//================================================================================================================
void ToolWindow2DEditorTab::UpdateEditSubModeTypeEnable()
{
	if (bTileMode)
	{
		if (!m_platformer)
		{
			m_EditSubModeTopdownTileLabel.Enable(true);
			m_EditModesSubModeTopdownTileRadioContainer.Enable(true);

			m_EditSubModePlatformerTileLabel.Enable(false);
			m_EditModesSubModePlatformerTileRadioContainer.Enable(false);
			m_EditSubModeSpriteLabel.Enable(false);
			m_EditModesSubModeSpriteRadioContainer.Enable(false);
			m_EditSubModeHardnessLabel.Enable(false);
			m_EditModesSubModeHardnessRadioContainer.Enable(false);
		}
		else
		{
			m_EditSubModePlatformerTileLabel.Enable(true);
			m_EditModesSubModePlatformerTileRadioContainer.Enable(true);

			m_EditSubModeTopdownTileLabel.Enable(false);
			m_EditModesSubModeTopdownTileRadioContainer.Enable(false);
			m_EditSubModeSpriteLabel.Enable(false);
			m_EditModesSubModeSpriteRadioContainer.Enable(false);
			m_EditSubModeHardnessLabel.Enable(false);
			m_EditModesSubModeHardnessRadioContainer.Enable(false);
		}
	}

	if (bSpriteMode)
	{
		m_EditSubModeSpriteLabel.Enable(true);
		m_EditModesSubModeSpriteRadioContainer.Enable(true);

		m_EditSubModeTopdownTileLabel.Enable(false);
		m_EditModesSubModeTopdownTileRadioContainer.Enable(false);
		m_EditSubModePlatformerTileLabel.Enable(false);
		m_EditModesSubModePlatformerTileRadioContainer.Enable(false);
		m_EditSubModeHardnessLabel.Enable(false);
		m_EditModesSubModeHardnessRadioContainer.Enable(false);
	}

	if (bHardnessMode)
	{
		m_EditSubModeHardnessLabel.Enable(true);
		m_EditModesSubModeHardnessRadioContainer.Enable(true);

		m_EditSubModeTopdownTileLabel.Enable(false);
		m_EditModesSubModeTopdownTileRadioContainer.Enable(false);
		m_EditSubModePlatformerTileLabel.Enable(false);
		m_EditModesSubModePlatformerTileRadioContainer.Enable(false);
		m_EditSubModeSpriteLabel.Enable(false);
		m_EditModesSubModeSpriteRadioContainer.Enable(false);
	}
}
//================================================================================================================
void ToolWindow2DEditorTab::DisableEditSubModeType()
{
	m_EditSubModeTopdownTileLabel.SetVisible(false);
	m_EditModesSubModeTopdownTileRadioContainer.SetVisible(false);
	m_EditSubModePlatformerTileLabel.SetVisible(false);
	m_EditModesSubModePlatformerTileRadioContainer.SetVisible(false);
	m_EditSubModeSpriteLabel.SetVisible(false);
	m_EditModesSubModeSpriteRadioContainer.SetVisible(false);
	m_EditSubModeHardnessLabel.SetVisible(false);
	m_EditModesSubModeHardnessRadioContainer.SetVisible(false);

	m_EditSubModeTopdownTileLabel.Enable(false);
	m_EditModesSubModeTopdownTileRadioContainer.Enable(false);
	m_EditSubModePlatformerTileLabel.Enable(false);
	m_EditModesSubModePlatformerTileRadioContainer.Enable(false);
	m_EditSubModeSpriteLabel.Enable(false);
	m_EditModesSubModeSpriteRadioContainer.Enable(false);
	m_EditSubModeHardnessLabel.Enable(false);
	m_EditModesSubModeHardnessRadioContainer.Enable(false);
}
//================================================================================================================
void ToolWindow2DEditorTab::VisibleTabItems(bool visible)
{
	m_GamePathLabel.SetVisible(visible);
	m_GamePathLabel2.SetVisible(visible);
	m_GameNameLabel.SetVisible(visible);
	m_GameNameTextbox.SetVisible(visible);
	m_CreateGameDirectory.SetVisible(visible);
	m_OpenGameDirectory.SetVisible(visible);
	//m_Undo.SetVisible(visible);
	//m_Redo.SetVisible(visible);
	m_EditModeLabel.SetVisible(visible);
	m_EditModes.SetVisible(visible);

	m_MenuButton.SetVisible(visible);
	m_HUDButton.SetVisible(visible);
	m_Play.SetVisible(visible);
	m_GameTypeLabel.SetVisible(visible);
	m_GameTypeRC.SetVisible(visible);
	m_EditorTypeLabel.SetVisible(visible);
	m_EditorTypeRC.SetVisible(visible);
}
//================================================================================================================
void ToolWindow2DEditorTab::EnableTabItems(bool enable)
{
	m_GamePathLabel.Enable(enable);
	m_GamePathLabel2.Enable(enable);
	m_GameNameLabel.Enable(enable);
	m_GameNameTextbox.Enable(enable);
	m_CreateGameDirectory.Enable(enable);
	m_OpenGameDirectory.Enable(enable);
	//m_Undo.Enable(enable);
	//m_Redo.Enable(enable);
	m_EditModeLabel.Enable(enable);
	m_EditModes.Enable(enable);

	m_MenuButton.Enable(enable);
	m_HUDButton.Enable(enable);
	m_Play.Enable(enable);
	m_GameTypeLabel.Enable(enable);
	m_GameTypeRC.Enable(enable);
	m_EditorTypeLabel.Enable(enable);
	m_EditorTypeRC.Enable(enable);
}
//================================================================================================================
bool ToolWindow2DEditorTab::DoHideWindow()
{
	return bHideWindow;
}
//================================================================================================================
void ToolWindow2DEditorTab::SetDoHideWindow(bool set)
{
	bHideWindow = set;
}
//================================================================================================================
void ToolWindow2DEditorTab::SetGameTypeRC(string val)
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
void ToolWindow2DEditorTab::Update()
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
LRESULT ToolWindow2DEditorTab::Command(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (!IsEnabled()) return true;

	//m_EditModeSubPicker.UpdateCommand(wParam, lParam);

	bool gd_created = false;

	switch (LOWORD(wParam))
	{
		case EEditorTypeID::eEditorTabCreateGameDirectory:
		{
			if (m_GameNameTextbox.GetText() == "")
			{
				ZShadeMessageCenter::MsgBoxOK(hwnd, "Must have a game name !!!", "Create Game Directory");
			}
			else
			{
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
		case EEditorTypeID::eEditorTabOpenGameDirectory:
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
		case EEditorTypeID::eEditorTabEditModeTile:
		{
			bTileMode = true;
			bSpriteMode = false;
			bMusicMode = false;
			bHardnessMode = false;
			
			UpdateEditSubModeTypeVisible();
			UpdateEditSubModeTypeEnable();
		}
		break;
		case EEditorTypeID::eEditorTabEditModeSprite:
		{
			bTileMode = false;
			bSpriteMode = true;
			bMusicMode = false;
			bHardnessMode = false;
			
			UpdateEditSubModeTypeVisible();
			UpdateEditSubModeTypeEnable();
		}
		break;
		case EEditorTypeID::eEditorTabEditModeMusic:
		{
			bTileMode = false;
			bSpriteMode = false;
			bMusicMode = true;
			bHardnessMode = false;
			
			UpdateEditSubModeTypeVisible();
			UpdateEditSubModeTypeEnable();
			DisableEditSubModeType();
		}
		break;
		case EEditorTypeID::eEditorTabEditModeHardness:
		{
			bTileMode = false;
			bSpriteMode = false;
			bMusicMode = false;
			bHardnessMode = true;
			
			UpdateEditSubModeTypeVisible();
			UpdateEditSubModeTypeEnable();
		}
		break;
		case EEditorTypeID::eEditorTabPlay:
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
					//// Launch the current map for testing
					//EnvironmentFactory::Instance()->CreateEnvironment("MapTestEnv", m_EngineOptions, "BaseGameWindow", "RenderGameWindow", true);

					//EnvironmentFactory::Instance()->ActiveEnvironmentName() = "MapTestEnv";

					//string mn = m_worldTab->ActiveMapName();
					//string wn = m_worldTab->ActiveWorldName();

					//// Load the world and set the map for testing
					////Environment2DMapHelper::LoadWorld(m_ActiveWorldName, false, 0);
					//Environment2DMapHelper::SetActiveRenderedMap(wn, mn);

					//EnvironmentFactory::Instance()->SelectEnvironment("MapTestEnv")->Run();

					//// Get the handle to the original m_Window back
					//// when the game environment returns from the run loop command
					//m_EngineOptions->hwnd = oldWindow;
					//EnvironmentFactory::Instance()->ActiveEnvironmentName() = oldWindowName;
					//m_EngineOptions->m_inEditor = true;
				}
				break;
				case IDNO:// Start the entire game
				{
					// Launch a new game environment
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
		case EEditorTypeID::eEditorTabMenuButton:
		{
			// Hide the map tool window and go to the menu tool window
			bHideWindow = true;
			
			bEditorTypeMenu = true;
			bEditorTypeHUD = false;
			bEditorTypeMap = false;
		}
		break;
		case EEditorTypeID::eEditorTabHUDButton:
		{
			// Hide the map tool window and go to the hud tool window
			bHideWindow = true;
			
			bEditorTypeHUD = true;
			bEditorTypeMenu = false;
			bEditorTypeMap = false;
		}
		break;
	}

	if (gd_created)
	{
		m_EngineOptions->m_GD2D = m_gameDirectory;

		m_mapEditorSystem2D->SetGameDirectory(m_gameDirectory);

		m_GameDirectoryCreated = true;

		m_EngineOptions->m_inEditor = true;

		UpdateEditSubModeTypeVisible();
		UpdateEditSubModeTypeEnable();

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
	
	return true;
}
//================================================================================================================