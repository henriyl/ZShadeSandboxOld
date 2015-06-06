//================================================================================================================
// ToolWindow2DEditorTab.h
//
//================================================================================================================
// History
//
// Created on 5/22/2015 by Dustin Watson
//================================================================================================================
#ifndef __TOOLWINDOW2DEDITORTAB_H
#define __TOOLWINDOW2DEDITORTAB_H
//================================================================================================================
//================================================================================================================

//
// Includes
//

#include "ToolWindow2DTab.h"
using namespace GUIComponents;

//================================================================================================================
//================================================================================================================

class ToolWindow2DEditorTab : public ToolWindow2DTab
{
public:

	ToolWindow2DEditorTab(HWND hwnd, EngineOptions* eo, D3D* d3d);
	~ToolWindow2DEditorTab();

public:
	
	void BuildTab();
	void DisableTab();
	void EnableTab();
	void Update();
	void VisibleTabItems(bool visible = true);
	void EnableTabItems(bool enable = true);
	LRESULT Command(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
	void DisableEditSubModeType();
	void UpdateEditSubModeTypeVisible();
	void UpdateEditSubModeTypeEnable();
	
	void SetGameTypeRC(string val);
	void SetGameName(string val) { m_GameNameTextbox.SetText(val); }
	bool IsTileMode() const { return bTileMode; }
	bool IsSpriteMode() const { return bSpriteMode; }
	bool IsMusicMode() const { return bMusicMode; }
	bool IsHardnessMode() const { return bHardnessMode; }
	bool& GameDirectoryCreated() { return m_GameDirectoryCreated; }
	bool GameDirectoryCreated() const { return m_GameDirectoryCreated; }
	bool IsEditorTypeMap() const { return bEditorTypeMap; }
	bool IsEditorTypeMenu() const { return bEditorTypeMenu; }
	bool IsEditorTypeHUD() const { return bEditorTypeHUD; }
	bool& IsPlatformer() { return m_platformer; }
	bool IsPlatformer() const { return m_platformer; }
	
	bool DoHideWindow();
	void SetDoHideWindow(bool set);
	
private:

	bool m_GameDirectoryCreated;
	bool m_platformer;
	bool bTileMode;
	bool bSpriteMode;
	bool bMusicMode;
	bool bHardnessMode;
	bool bEditorTypeMap;
	bool bEditorTypeMenu;
	bool bEditorTypeHUD;
	
	// Create variables here specific for the toggling of map, menu, hud tool
	// so that the tool window is only shown or hidden once after the button click
	bool bHideWindow;
	
	// Editor tab stuff
	// Add in Grid Size
	WinLabel m_GamePathLabel;
	WinLabel m_GamePathLabel2;
	WinLabel m_GameNameLabel;
	WinTextbox m_GameNameTextbox;
	WinButton m_CreateGameDirectory;
	WinButton m_OpenGameDirectory;
	WinButton m_Undo;
	WinButton m_Redo;
	WinLabel m_EditModeLabel;
	WinRadioGroup m_EditModes;
	WinLabel m_EditSubModeTopdownTileLabel;
	WinRadioGroup m_EditModesSubModeTopdownTileRadioContainer;
	WinLabel m_EditSubModePlatformerTileLabel;
	WinRadioGroup m_EditModesSubModePlatformerTileRadioContainer;
	WinLabel m_EditSubModeSpriteLabel;
	WinRadioGroup m_EditModesSubModeSpriteRadioContainer;
	WinLabel m_EditSubModeHardnessLabel;
	WinRadioGroup m_EditModesSubModeHardnessRadioContainer;
	WinButton m_MenuButton;
	WinButton m_HUDButton;
	WinButton m_Play;
	WinLabel m_GameTypeLabel;
	WinRadioGroup m_GameTypeRC;
	WinLabel m_EditorTypeLabel;
	WinRadioGroup m_EditorTypeRC;
};

//================================================================================================================
//================================================================================================================
#endif//__TOOLWINDOW2DEDITORTAB_H