//================================================================================================================
// ToolWindowMenu.h
//
//================================================================================================================
// History
//
// Created on 5/22/2015 by Dustin Watson
//================================================================================================================
#ifndef __TOOLWINDOWMENU_H
#define __TOOLWINDOWMENU_H
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
//#include "NewMenuWindow.h"
#include "OpenFileDialog.h"
#include "SaveFileDialog.h"
#include "ImageSampleWindow.h"
#include "Environment2D.h"
#include "Environment3D.h"
#include "SplitterWindow.h"
using namespace GUIComponents;

//================================================================================================================
//================================================================================================================

class ToolWindowMenu
{
public:

	ToolWindowMenu(EngineOptions* eo, D3D* d3d, Environment2D* env2D, Environment3D* env3D, SplitterWindow* editWindow);
	~ToolWindowMenu();

public:

	void Init();
	void Update();
	void FillCache();
	void SetImageSampleWindow(ImageSampleWindow* isw) { mImageSampleWindow = isw; }

	bool& MenuCreated() { return m_MenuCreated; }
	bool MenuCreated() const { return m_MenuCreated; }
	bool& MenuItemNone() { return bMenuItemNone; }
	bool MenuItemNone() const { return bMenuItemNone; }
	bool& MenuItemButton() { return bMenuItemButton; }
	bool MenuItemButton() const { return bMenuItemButton; }
	bool& MenuItemText() { return bMenuItemText; }
	bool MenuItemText() const { return bMenuItemText; }
	
	MenuEditorSystem* GetMenuEditorSystem() { return m_MenuEditorSystem; }
	
	bool DoHideWindow();
	void SetDoHideWindow(bool set);
	void ShowWindow();
	void HideWindow();
	void ShowItems(bool visible);
	void EnableItems(bool enable);
	
private:

	LRESULT Command(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
	MenuEditorSystem* m_MenuEditorSystem;
	//NewMenuWindow* m_NewMenuWindow;
	
	GameDirectory2D* m_gameDirectory2D;
	GameDirectory3D* m_gameDirectory3D;
	D3D* m_D3DSystem;
	EngineOptions* m_EngineOptions;
	SplitterWindow* m_BaseEditWindowRef;
	Environment2D* m_Env2DRef;
	Environment3D* m_Env3DRef;
	
	bool bHideWindow;
	
	bool bMenuItemNone;
	bool bMenuItemButton;
	bool bMenuItemText;
	bool m_MenuCreated;
	string m_SelectedMenuItemName;

	// Menu Editor tab stuff
	WinLabel m_SelectMenuItemText;
	WinLabel m_SelectMenuItemText2;
	WinButton m_SelectMenuButton;
	WinButton m_UpdateMenuButton;
	WinListbox m_MenuItemListbox;
	WinLabel m_MenuItemText;
	WinRadioGroup m_MenuItemContainer;
	//WinButton m_CreateMenu;
	WinButton m_LoadMenu;
	WinButton m_SaveMenu;
	WinButton m_RenderedMenu;
	WinButton m_RemoveMenu;
	WinLabel m_MenuListText;
	WinComboBox m_MenuNameList;
	WinButton m_AddMenuBackground;
	WinButton m_ToMapEditorButton;
	
	WinButton m_createButton;
	WinLabel m_menuSizeText;
	WinLabel m_menuNameText;
	WinLabel m_menuTypeText;
	WinRadioGroup m_menuSizeContainer;
	WinRadioGroup m_menuTypeContainer;
	WinTextbox m_menuNameTextbox;

	OpenFileDialog m_OpenFileDialog;
	SaveFileDialog m_SaveFileDialog;

	ImageSampleWindow* mImageSampleWindow;
};

//================================================================================================================
//================================================================================================================
#endif//__TOOLWINDOWMENU_H