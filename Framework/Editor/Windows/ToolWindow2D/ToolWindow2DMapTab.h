//================================================================================================================
// ToolWindow2DMapTab.h
//
//================================================================================================================
// History
//
// Created on 5/26/2015 by Dustin Watson
//================================================================================================================
#ifndef __TOOLWINDOW2DMAPTAB_H
#define __TOOLWINDOW2DMAPTAB_H
//================================================================================================================
//================================================================================================================

//
// Includes
//

#include "ToolWindow2DTab.h"
using namespace GUIComponents;

//================================================================================================================
//================================================================================================================

class ToolWindow2DMapTab : public ToolWindow2DTab
{
public:

	ToolWindow2DMapTab(HWND hwnd, EngineOptions* eo, D3D* d3d);
	~ToolWindow2DMapTab();

public:

	void BuildTab();
	void Update();
	void VisibleTabItems(bool visible = true);
	void EnableTabItems(bool enable = true);
	
	LRESULT Command(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
	void SetMapName(string name);
	void SetMapSize(string size);
	void SetMapVision(string vision);
	void SetMapMusic(string music);
	void CheckFOW(bool fow);
	void FOWFlashlightEnabled(bool enabled);
	void SetFOWRadius(int radius);
	void SetTileSize(string val);
	void SetNumTiles(string val);
	void SetNumSections(string val);
	void SetRowSize(string val);
	void SetColSize(string val);
	void SetSectionRowSize(string val);
	void SetSectionColSize(string val);
	void SetNumSpritesRendered(string val);
	void SetTotalSprites(string val);

	bool& FowEnabled() { return m_fowEnabled; }
	bool FowEnabled() const { return m_fowEnabled; }
	int& FowRadius() { return m_fowRadius; }
	int FowRadius() const { return m_fowRadius; }
	bool& FowFlashlight() { return m_fowFlashlight; }
	bool FowFlashlight() const { return m_fowFlashlight; }

private:
	
	bool m_fowEnabled;
	int m_fowRadius;
	bool m_fowFlashlight;
	
	// Map tab stuff
	WinLabel m_MapNameLabel;
	WinTextbox m_EditMapName;
	WinLabel m_MapSizeLabel;
	WinTextbox m_EditMapSize;
	WinLabel m_MapVisionLabel;
	WinTextbox m_EditMapVision;
	WinLabel m_MapMusicLabel;
	WinTextbox m_EditMapMusic;
	WinCheckbox m_ToggleHardness;
	WinButton m_UpdateAttributes;
	WinLabel m_fowEnabledLabel;
	WinRadioGroup m_fowEnabledRadioContainer;
	WinLabel m_fowRadiusLabel;
	WinComboBox m_fowRadiusComboBox;
	WinCheckbox m_fowFlashlightEnabled;
	WinLabel m_tileSizeText;
	WinTextbox m_tileSizeTextbox;
	WinLabel m_numTilesText;
	WinTextbox m_numTilesTextbox;
	WinLabel m_numSectionsText;
	WinTextbox m_numSectionsTextbox;
	WinLabel m_mapRowSizeText;
	WinTextbox m_mapRowSizeTextbox;
	WinLabel m_mapColSizeText;
	WinTextbox m_mapColSizeTextbox;
	WinLabel m_sectionRowSizeText;
	WinTextbox m_sectionRowSizeTextbox;
	WinLabel m_sectionColSizeText;
	WinTextbox m_sectionColSizeTextbox;
	WinLabel m_spritesRenderedText;
	WinTextbox m_spritesRenderedTextbox;
	WinLabel m_spritesTotalText;
	WinTextbox m_spritesTotalTextbox;
};

//================================================================================================================
//================================================================================================================
#endif//__TOOLWINDOW2DMAPTAB_H