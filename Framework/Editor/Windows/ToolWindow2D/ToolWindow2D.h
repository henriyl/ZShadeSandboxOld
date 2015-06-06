//================================================================================================================
// ToolWindow2D.h
//
//================================================================================================================
// History
//
// Created on 5/22/2015 by Dustin Watson
//================================================================================================================
#ifndef __TOOLWINDOW2D_H
#define __TOOLWINDOW2D_H
//================================================================================================================
//================================================================================================================

//
// Includes
//

#include <memory>
#include <Windows.h>
#include <string>
#include <windowsx.h>
#include <CommCtrl.h>
#include "EngineOptions.h"
#include "WindowTabControl.h"
#include "WinButton.h"
#include "WinTextbox.h"
#include "WinLabel.h"
#include "WinListbox.h"
#include "WinRadioGroup.h"
#include "WinCheckbox.h"
#include "WinComboBox.h"
#include "WinTimer.h"
//#include "NewMapWindow.h"
//#include "NewMenuWindow.h"
//#include "NewPlatformerMapWindow.h"
//#include "NewHUDWindow.h"
#include "OpenFileDialog.h"
#include "SaveFileDialog.h"
#include "ImageSampleWindow.h"
//#include "HUDEditorSystem.h"
#include "Environment2D.h"
#include "SplitterWindow.h"
#include "ToolWindow2DEditorTab.h"
#include "ToolWindow2DWorldTab.h"
#include "ToolWindow2DMapTab.h"
#include "ToolWindow2DTileTab.h"
#include "ToolWindow2DSpriteTab.h"
#include "ToolWindow2DTileCacheTab.h"
#include "ToolWindow2DSpriteCacheTab.h"
using namespace GUIComponents;

//================================================================================================================
//================================================================================================================

class ToolWindow2D
{
public:

	ToolWindow2D(EngineOptions* eo, D3D* d3d, Environment2D* env2D, SplitterWindow*& editWindow);
	~ToolWindow2D();

public:

	void Init();

	void SetEngineOptions(EngineOptions* eo) { m_EngineOptions = eo; }

	void Update();

	LRESULT Command(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT NotifyTab(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
	void ShowWindow();
	void HideWindow();
	
	bool DoHideWindow();
	void SetDoHideWindow(bool set);

	bool IsEditorTypeMap() { return bEditorTypeMap; }
	bool IsEditorTypeMenu() { return bEditorTypeMenu; }
	bool IsEditorTypeHUD() { return bEditorTypeHUD; }
	
public:

	MapEditorSystem2D* GetMapEditorSystem2D() { return m_MapEditorSystem2D; }
	//MenuEditorSystem* GetMenuEditorSystem() { return m_MenuEditorSystem; }
	//HUDEditorSystem* GetHUDEditorSystem() { return m_HUDEditorSystem; }

private:

	MapEditorSystem2D* m_MapEditorSystem2D;
	//MenuEditorSystem* m_MenuEditorSystem;
	//HUDEditorSystem* m_HUDEditorSystem;
	
	//NewMapWindow* m_NewMapWindow;
	//NewMapWindowPlatformer* m_NewPlatformerMapWindow;
	//NewMenuWindow* m_NewMenuWindow;
	//NewHUDWindow* m_NewHUDWindow;
	
	GameDirectory2D* m_gameDirectory;
	D3D* m_D3DSystem;
	EngineOptions* m_EngineOptions;
	SplitterWindow* m_BaseEditWindowRef;
	Environment2D* m_Env2DRef;
	// Controls the tabs
	WindowTabControl* m_tabControl;
	
	ToolWindow2DEditorTab* m_editorTab;
	ToolWindow2DWorldTab* m_worldTab;
	ToolWindow2DMapTab* m_mapTab;
	ToolWindow2DTileTab* m_tileTab;
	ToolWindow2DSpriteTab* m_spriteTab;
	ToolWindow2DTileCacheTab* m_tileCacheTab;
	ToolWindow2DSpriteCacheTab* m_spriteCacheTab;
	
	bool bEditorTypeMap;
	bool bEditorTypeMenu;
	bool bEditorTypeHUD;
	bool bEditorTab;
	bool bWorldTab;
	bool bMapTab;
	bool bTilesTab;
	bool bSpritesTab;
	bool bTileCacheTab;
	bool bSpriteCacheTab;
};

//================================================================================================================
//================================================================================================================
#endif//__TOOLWINDOW2D_H