//================================================================================================================
// ToolWindow2DTileTab.h
//
//================================================================================================================
// History
//
// Created on 5/26/2015 by Dustin Watson
//================================================================================================================
#ifndef __TOOLWINDOW2DTILETAB_H
#define __TOOLWINDOW2DTILETAB_H
//================================================================================================================
//================================================================================================================

//
// Includes
//

#include "ToolWindow2DTab.h"
using namespace GUIComponents;

//================================================================================================================
//================================================================================================================

class ToolWindow2DTileTab : public ToolWindow2DTab
{
public:

	ToolWindow2DTileTab(HWND hwnd, EngineOptions* eo, D3D* d3d);
	~ToolWindow2DTileTab();

public:

	void BuildTab();
	void Update();
	void VisibleTabItems(bool visible = true);
	void EnableTabItems(bool enable = true);
	
	LRESULT Command(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
private:
	
	WinComboBox m_TilesList;
	WinLabel m_xPosText;
	WinLabel m_yPosText;
	WinLabel m_baseTextureText;
	WinLabel m_animSpeedText;
	WinLabel m_animationProfileNameText;
	WinLabel m_hardText;
	WinRadioGroup m_hardContainer;
	WinTextbox m_xPosTextBox;
	WinTextbox m_yPosTextBox;
	WinTextbox m_baseTextureTextBox;
	WinTextbox m_animSpeedTextBox;
	WinTextbox m_animationProfileNameTextBox;
	WinButton m_updateAnimationButton;
	WinButton m_revertAnimationButton;
	WinButton m_UpdateTileProperties;
};

//================================================================================================================
//================================================================================================================
#endif//__TOOLWINDOW2DTILETAB_H