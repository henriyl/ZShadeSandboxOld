//================================================================================================================
// ToolWindow3D.h
//
//================================================================================================================
// History
//
// Created on 5/22/2015 by Dustin Watson
//================================================================================================================
#ifndef __TOOLWINDOW3D_H
#define __TOOLWINDOW3D_H
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
#include "NewMapWindow.h"
#include "NewMenuWindow.h"
#include "NewPlatformerMapWindow.h"
#include "OpenFileDialog.h"
#include "SaveFileDialog.h"
#include "ImageSampleWindow.h"
#include "NewHUDWindow.h"
#include "HUDEditorSystem.h"
#include "Environment3D.h"
#include "SplitterWindow.h"
#include "ToolWindow3DEditorTab.h"
using namespace GUIComponents;

//================================================================================================================
//================================================================================================================

class ToolWindow3D
{
public:

	ToolWindow3D(EngineOptions* eo, D3D* d3d, Environment3D* env2D, SplitterWindow* editWindow);
	~ToolWindow3D();

public:

	void Init();

	void SetEngineOptions(EngineOptions* eo) { m_EngineOptions = eo; }

	void Update();


public:

	MenuEditorSystem* GetMenuEditorSystem() { return m_MenuEditorSystem; }
	HUDEditorSystem* GetHUDEditorSystem() { return m_HUDEditorSystem; }

private:

	LRESULT Command(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT WindowMoved(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT NotifyTab(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
	MenuEditorSystem* m_MenuEditorSystem;
	HUDEditorSystem* m_HUDEditorSystem;

	NewMapWindow* m_NewMapWindow;
	NewMapWindowPlatformer* m_NewPlatformerMapWindow;
	NewMenuWindow* m_NewMenuWindow;
	NewHUDWindow* m_NewHUDWindow;

	GameDirectory3D* m_gameDirectory;
	D3D* m_D3DSystem;
	EngineOptions* m_EngineOptions;
	SplitterWindow* m_BaseEditWindowRef;
	Environment3D* m_Env3DRef;
	// Controls the tabs
	WindowTabControl* m_tabControl;

	ToolWindow3DEditorTab* m_editorTab;

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
#endif//__TOOLWINDOW3D_H