//================================================================================================================
// ToolWindow3DEditorTab.h
//
//================================================================================================================
// History
//
// Created on 5/22/2015 by Dustin Watson
//================================================================================================================
#ifndef __TOOLWINDOW3DEDITORTAB_H
#define __TOOLWINDOW3DEDITORTAB_H
//================================================================================================================
//================================================================================================================

//
// Includes
//

#include "ToolWindow3DTab.h"
using namespace GUIComponents;

//================================================================================================================
//================================================================================================================

namespace EEditorTypeID3D
{
	enum Type
	{
		eCreateGameDirectory,
		eOpenGameDirectory,
		eGameDirectoryNameLabel,
		eGameDirectoryNameTextbox,
		eGameTypeRC,
		eGameTypeTopdown,
		eGameTypePlatformer,
		eGameTypeLabel,
		eEditModeRG,
		eEditModeTile,
		eEditModeSprite,
		eEditModeMusic,
		eEditModeHardness,
		eEditModeMenu,
		eEditModeHUD,
		eEditModeCB,
		eEditModeLabel,
		eEditSubModeLabel,
		eEditTypeRC,
		eEditTypeMap,
		eEditTypeMenu,
		eEditTypeHUD,
		eEditTypeLabel,
		ePlay
	};
}

class ToolWindow3DEditorTab : public ToolWindow3DTab
{
public:

	ToolWindow3DEditorTab(HWND hwnd, EngineOptions* eo, D3D* d3d);
	~ToolWindow3DEditorTab();

public:

	void BuildTab();
	void DisableTab();
	void EnableTab();
	void Update();
	void FillEditModeSubPicker();
	LRESULT Command(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
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

private:

	bool m_GameDirectoryCreated;
	bool m_platformer;
	bool bTileMode;
	bool bSpriteMode;
	bool bMusicMode;
	bool bHardnessMode;
	bool bMenuMode;
	bool bHUDMode;
	bool bEditorTypeMap;
	bool bEditorTypeMenu;
	bool bEditorTypeHUD;

	// Editor tab stuff
	// Add in Grid Size
	WinLabel m_GameNameLabel;
	WinTextbox m_GameNameTextbox;
	WinButton m_CreateGameDirectory;
	WinButton m_OpenGameDirectory;
	WinButton m_Undo;
	WinButton m_Redo;
	WinLabel m_EditModeLabel;
	WinRadioGroup m_EditModes;
	WinLabel m_EditSubModeLabel;
	WinComboBox m_EditModeSubPicker;
	WinButton m_Play;
	WinLabel m_GameTypeLabel;
	WinRadioGroup m_GameTypeRC;
	WinLabel m_EditorTypeLabel;
	WinRadioGroup m_EditorTypeRC;
};

//================================================================================================================
//================================================================================================================
#endif//__TOOLWINDOW3DEDITORTAB_H