//================================================================================================================
// ToolWindow2DTab.h
//
//================================================================================================================
// History
//
// Created on 5/22/2015 by Dustin Watson
//================================================================================================================
#ifndef __TOOLWINDOW2DTAB_H
#define __TOOLWINDOW2DTAB_H
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

class ToolWindow2DTab
{
public:

	ToolWindow2DTab(HWND hwnd, EngineOptions* eo, D3D* d3d);
	~ToolWindow2DTab();

public:
	
	virtual void BuildTab() = 0;
	virtual void DisableTab();
	virtual void EnableTab();
	// Updates events from this tab
	virtual void Update();
	// Process events from this tab
	virtual LRESULT Command(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
	virtual void VisibleTabItems(bool visible = true) {}
	// Toggles whether or not all tab items in the window are usable
	// Depending on if a Game Directory has been created
	virtual void EnableTabItems(bool enable = true) {}
	bool IsEnabled();
	
	void AddMapEditorSystem(MapEditorSystem2D* mapEditorSystem);
	
protected:

	GameDirectory2D* m_gameDirectory;
	D3D* m_D3DSystem;
	EngineOptions* m_EngineOptions;
	
	MapEditorSystem2D* m_mapEditorSystem2D;
	
	// Base window handle for this tab
	HWND m_hwnd;
	
	OpenFileDialog m_OpenFileDialog;
	SaveFileDialog m_SaveFileDialog;

	bool m_enabled;
};

//================================================================================================================
//================================================================================================================
#endif//__TOOLWINDOW2DTAB_H