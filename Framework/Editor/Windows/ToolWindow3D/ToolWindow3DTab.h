//================================================================================================================
// ToolWindow3DTab.h
//
//================================================================================================================
// History
//
// Created on 5/22/2015 by Dustin Watson
//================================================================================================================
#ifndef __TOOLWINDOW3DTAB_H
#define __TOOLWINDOW3DTAB_H
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
#include "Window.h"
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
#include "NewHUDWindow.h"
#include "NewPlatformerMapWindow.h"
#include "OpenFileDialog.h"
#include "SaveFileDialog.h"
#include "ImageSampleWindow.h"
#include "Window.h"

//================================================================================================================
//================================================================================================================

class ToolWindow3DTab
{
public:

	ToolWindow3DTab(HWND hwnd, EngineOptions* eo, D3D* d3d);
	~ToolWindow3DTab();

public:

	virtual void BuildTab() = 0;
	virtual void DisableTab() = 0;
	virtual void EnableTab() = 0;
	// Updates events from this tab
	virtual void Update();
	// Process events from this tab
	virtual LRESULT Command(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
	// Toggles whether or not all tab items in the window are usable
	// Depending on if a Game Directory has been created
	void EnableTabItems(bool enable = true);
	
	void AddMenuEditorSystem(MenuEditorSystem* menuEditorSystem);
	void AddHUDEditorSystem(HUDEditorSystem* hudEditorSystem);

	void AddNewMapWindow(NewMapWindow* win) { m_NewMapWindow = win; }
	void AddNewMapPlatformerWindow(NewMapWindowPlatformer* win) { m_NewPlatformerMapWindow = win; }
	void AddNewMenuWindow(NewMenuWindow* win) { m_NewMenuWindow = win; }
	void AddNewHUDWindow(NewHUDWindow* win) { m_NewHUDWindow = win; }

protected:

	GameDirectory3D* m_gameDirectory;
	D3D* m_D3DSystem;
	EngineOptions* m_EngineOptions;

	MenuEditorSystem* m_menuEditorSystem;
	HUDEditorSystem* m_hudEditorSystem;

	NewMapWindow* m_NewMapWindow;
	NewMapWindowPlatformer* m_NewPlatformerMapWindow;
	NewMenuWindow* m_NewMenuWindow;
	NewHUDWindow* m_NewHUDWindow;

	// Base window handle for this tab
	HWND m_hwnd;
	//Window m_hwnd;

	OpenFileDialog m_OpenFileDialog;
	SaveFileDialog m_SaveFileDialog;

	bool m_enabled;
};

//================================================================================================================
//================================================================================================================
#endif//__TOOLWINDOW2DTAB_H