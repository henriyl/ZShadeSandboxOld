//================================================================================================================
// ToolWindow2DTileCacheTab.h
//
//================================================================================================================
// History
//
// Created on 5/26/2015 by Dustin Watson
//================================================================================================================
#ifndef __TOOLWINDOW2DTILECACHETAB_H
#define __TOOLWINDOW2DTILECACHETAB_H
//================================================================================================================
//================================================================================================================

//
// Includes
//

#include "ToolWindow2DTab.h"
using namespace GUIComponents;

//================================================================================================================
//================================================================================================================

class ToolWindow2DTileCacheTab : public ToolWindow2DTab
{
public:

	ToolWindow2DTileCacheTab(HWND hwnd, EngineOptions* eo, D3D* d3d);
	~ToolWindow2DTileCacheTab();

public:
	
	void FillCache();
	
	void BuildTab();
	void Update();
	void VisibleTabItems(bool visible = true);
	void EnableTabItems(bool enable = true);
	
	LRESULT Command(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
private:
	
	string m_SelectedTileName;
	
	WinListbox m_TileCacheList;
	WinButton m_TileCacheButton;
	WinButton m_TileCacheUpdateButton;
	WinLabel m_TileNameLabel;
	WinLabel m_TileNameLabel2;
};

//================================================================================================================
//================================================================================================================
#endif//__TOOLWINDOW2DTILECACHETAB_H