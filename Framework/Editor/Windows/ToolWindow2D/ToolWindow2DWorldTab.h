//================================================================================================================
// ToolWindow2DWorldTab.h
//
//================================================================================================================
// History
//
// Created on 5/22/2015 by Dustin Watson
//================================================================================================================
#ifndef __TOOLWINDOW2DWORLDTAB_H
#define __TOOLWINDOW2DWORLDTAB_H
//================================================================================================================
//================================================================================================================

//
// Includes
//

#include "ToolWindow2DTab.h"
using namespace GUIComponents;

//================================================================================================================
//================================================================================================================

class ToolWindow2DWorldTab : public ToolWindow2DTab
{
public:

	ToolWindow2DWorldTab(HWND hwnd, EngineOptions* eo, D3D* d3d);
	~ToolWindow2DWorldTab();

public:

	void BuildTab();
	void Update();
	void VisibleTabItems(bool visible = true);
	void EnableTabItems(bool enable = true);
	
	LRESULT Command(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void SetCreateWorldName(string val) { m_CreateWorldName.SetText(val); }

	bool& WorldCreated() { return m_WorldCreated; }
	bool WorldCreated() const { return m_WorldCreated; }

	string ActiveWorldName() const { return m_ActiveWorldName; }
	string ActiveMapName() const { return m_ActiveMapName; }

	bool& IsPlatformer() { return m_IsPlatformer; }

private:

	string m_ActiveWorldName;
	string m_ActiveMapName;

	bool m_WorldCreated;
	bool m_IsPlatformer;

	// World tab stuff
	WinLabel createWorldNamelabel;
	WinTextbox m_CreateWorldName;
	WinButton m_CreateWorld;
	WinButton m_LoadWorld;
	WinButton m_SaveWorld;
	WinButton m_CreateMap;
	WinButton m_LoadMap;
	WinButton m_SaveMap;
	WinButton m_RenderedMap;
	WinButton m_RemoveMap;
	WinLabel m_WorldName;
	WinLabel m_WTMapNameLabel;
	WinComboBox m_MapNameList;

	WinButton m_createTopdownMapButton;
	WinLabel m_TopdownMapSizeLabel;
	WinComboBox m_TopdownMapSizeCombobox;
	WinLabel m_TopdownIsoStandLabel;
	WinRadioGroup m_TopdownIsoStandRadioContainer;
	WinLabel m_TopdownMapNameLabel;
	WinTextbox m_TopdownMapNameTextbox;
	WinLabel m_TopdownWorldNameLabel;
	WinComboBox m_TopdownWorldNameComboBox;
	WinLabel m_TopdownFowEnabledLabel;
	WinRadioGroup m_TopdownFowEnabledRadioContainer;
	WinLabel m_TopdownFowRadiusLabel;
	WinComboBox m_TopdownFowRadiusComboBox;
	WinCheckbox m_TopdownFowFlashlightEnabled;

	WinButton m_createPlatformerMapButton;
	WinLabel m_PlatformerMapWidthLabel;
	WinTextbox m_PlatformerMapWidthTextbox;
	WinLabel m_PlatformerMapHeightLabel;
	WinTextbox m_PlatformerMapHeightTextbox;
	WinLabel m_PlatformerMapNameLabel;
	WinTextbox m_PlatformerMapNameTextbox;
	WinLabel m_PlatformerWorldNameLabel;
	WinComboBox m_PlatformerWorldNameComboBox;
};

//================================================================================================================
//================================================================================================================
#endif//__TOOLWINDOW2DWORLDTAB_H