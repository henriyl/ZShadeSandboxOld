//================================================================================================================
// ToolWindowHUD.h
//
//================================================================================================================
// History
//
// Created on 5/22/2015 by Dustin Watson
//================================================================================================================
#ifndef __TOOLWINDOWHUD_H
#define __TOOLWINDOWHUD_H
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
#include "OpenFileDialog.h"
#include "SaveFileDialog.h"
#include "ImageSampleWindow.h"
//#include "NewHUDWindow.h"
#include "HUDEditorSystem.h"
#include "Environment2D.h"
#include "Environment3D.h"
#include "SplitterWindow.h"
using namespace GUIComponents;

//================================================================================================================
//================================================================================================================

class ToolWindowHUD
{
public:

	ToolWindowHUD(EngineOptions* eo, D3D* d3d, Environment2D* env2D, Environment3D* env3D, SplitterWindow* editWindow);
	~ToolWindowHUD();

public:

	void Init();
	void Update();
	void FillCache();
	void SetImageSampleWindow(ImageSampleWindow* isw) { mImageSampleWindow = isw; }

	bool& HUDCreated() { return m_HUDCreated; }
	bool HUDCreated() const { return m_HUDCreated; }
	bool& HUDItemNone() { return bHUDItemNone; }
	bool HUDItemNone() const { return bHUDItemNone; }
	bool& HUDItemImage() { return bHUDItemImage; }
	bool HUDItemImage() const { return bHUDItemImage; }
	bool& HUDItemText() { return bHUDItemText; }
	bool HUDItemText() const { return bHUDItemText; }
	
	HUDEditorSystem* GetHUDEditorSystem() { return m_HUDEditorSystem; }
	
	bool DoHideWindow();
	void SetDoHideWindow(bool set);
	void ShowWindow();
	void HideWindow();
	void ShowItems(bool visible);
	void EnableItems(bool enable);
	
private:

	LRESULT Command(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
	HUDEditorSystem* m_HUDEditorSystem;
	//NewHUDWindow* m_NewHUDWindow;
	
	GameDirectory2D* m_gameDirectory2D;
	GameDirectory3D* m_gameDirectory3D;
	D3D* m_D3DSystem;
	EngineOptions* m_EngineOptions;
	SplitterWindow* m_BaseEditWindowRef;
	Environment2D* m_Env2DRef;
	Environment3D* m_Env3DRef;
	
	bool bHideWindow;
	
	bool bHUDItemNone;
	bool bHUDItemImage;
	bool bHUDItemText;
	bool m_HUDCreated;
	string m_SelectedHUDItemName;

	// HUD Editor tab stuff
	WinLabel m_SelectHUDItemText;
	WinLabel m_SelectHUDItemText2;
	WinButton m_SelectHUDButton;
	WinButton m_UpdateHUDButton;
	WinListbox m_HUDItemListbox;
	WinLabel m_HUDItemText;
	WinRadioGroup m_HUDItemContainer;
	//WinButton m_CreateHUD;
	WinButton m_LoadHUD;
	WinButton m_SaveHUD;
	WinButton m_RenderedHUD;
	WinButton m_RemoveHUD;
	WinLabel m_HUDListText;
	WinComboBox m_HUDNameList;
	WinButton m_ToMapEditorButton;
	
	WinButton m_createButton;
	WinLabel m_hudSizeText;
	WinRadioGroup m_hudSizeContainer;

	OpenFileDialog m_OpenFileDialog;
	SaveFileDialog m_SaveFileDialog;

	ImageSampleWindow* mImageSampleWindow;
};

//================================================================================================================
//================================================================================================================
#endif//__TOOLWINDOWHUD_H