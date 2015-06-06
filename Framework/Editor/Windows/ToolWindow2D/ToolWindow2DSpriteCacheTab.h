//================================================================================================================
// ToolWindow2DSpriteCacheTab.h
//
//================================================================================================================
// History
//
// Created on 5/26/2015 by Dustin Watson
//================================================================================================================
#ifndef __TOOLWINDOW2DSPRITECACHETAB_H
#define __TOOLWINDOW2DSPRITECACHETAB_H
//================================================================================================================
//================================================================================================================

//
// Includes
//

#include "ToolWindow2DTab.h"
using namespace GUIComponents;

//================================================================================================================
//================================================================================================================

class ToolWindow2DSpriteCacheTab : public ToolWindow2DTab
{
public:

	ToolWindow2DSpriteCacheTab(HWND hwnd, EngineOptions* eo, D3D* d3d);
	~ToolWindow2DSpriteCacheTab();

public:
	
	void FillCache();
	
	void BuildTab();
	void Update();
	void VisibleTabItems(bool visible = true);
	void EnableTabItems(bool enable = true);
	
	LRESULT Command(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
private:
	
	string m_SelectedSpriteName;
	bool m_displaySpriteLoaded;
	
	WinListbox m_SpriteCacheList;
	WinButton m_SpriteCacheButton;
	WinButton m_SpriteCacheUpdateButton;
	WinLabel m_SpriteNameLabel;
	WinLabel m_SpriteNameLabel2;
};

//================================================================================================================
//================================================================================================================
#endif//__TOOLWINDOW2DSPRITECACHETAB_H