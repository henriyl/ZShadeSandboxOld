//================================================================================================================
// ToolWindow2DSpriteTab.h
//
//================================================================================================================
// History
//
// Created on 5/26/2015 by Dustin Watson
//================================================================================================================
#ifndef __TOOLWINDOW2DSPRITETAB_H
#define __TOOLWINDOW2DSPRITETAB_H
//================================================================================================================
//================================================================================================================

//
// Includes
//

#include "ToolWindow2DTab.h"
using namespace GUIComponents;

//================================================================================================================
//================================================================================================================

class ToolWindow2DSpriteTab : public ToolWindow2DTab
{
public:

	ToolWindow2DSpriteTab(HWND hwnd, EngineOptions* eo, D3D* d3d);
	~ToolWindow2DSpriteTab();

public:

	void BuildTab();
	void Update();
	void VisibleTabItems(bool visible = true);
	void EnableTabItems(bool enable = true);
	
	LRESULT Command(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
private:
	
	WinComboBox m_SpritesList;
	//WinButton m_OpenSpritesProperties;
};

//================================================================================================================
//================================================================================================================
#endif//__TOOLWINDOW2DTILETAB_H