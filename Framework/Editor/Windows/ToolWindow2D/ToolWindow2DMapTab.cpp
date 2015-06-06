#include "ToolWindow2DMapTab.h"
#include "EditorTypeID.h"
//================================================================================================================
//================================================================================================================
ToolWindow2DMapTab::ToolWindow2DMapTab(HWND hwnd, EngineOptions* eo, D3D* d3d)
:   ToolWindow2DTab(hwnd, eo, d3d)
{
	BuildTab();
	EnableTab();
}
//================================================================================================================
ToolWindow2DMapTab::~ToolWindow2DMapTab()
{

}
//================================================================================================================
void ToolWindow2DMapTab::BuildTab()
{
	int x = 10;
	int y = 30;

	HWND hwnd = m_hwnd;

	m_MapNameLabel.Init(hwnd, "Edit Name", x, y, 100, 30, EEditorTypeID::eMapTabMapNameLabel);
	m_EditMapName.Init(hwnd, x + 110, y, 150, 20, EEditorTypeID::eMapTabMapNameTextbox);

	y += 30;

	m_MapSizeLabel.Init(hwnd, "Edit Size", x, y, 100, 30, EEditorTypeID::eMapTabMapSizeLabel);
	m_EditMapSize.Init(hwnd, x + 110, y, 150, 20, EEditorTypeID::eMapTabMapSizeTextbox);

	y += 30;

	m_MapVisionLabel.Init(hwnd, "Edit Vision", x, y, 100, 30, EEditorTypeID::eMapTabMapVisionLabel);
	m_EditMapVision.Init(hwnd, x + 110, y, 150, 20, EEditorTypeID::eMapTabMapVisionTextbox);

	y += 30;

	m_MapMusicLabel.Init(hwnd, "Edit Music", x, y, 100, 30, EEditorTypeID::eMapTabMapMusicNameLabel);
	m_EditMapMusic.Init(hwnd, x + 110, y, 150, 20, EEditorTypeID::eMapTabMapMusicNameTextbox);

	y += 30;

	m_ToggleHardness.Init(hwnd, "Hardness", x, y, 100, 30, EEditorTypeID::eMapTabToggleHardnessCheckbox);
	m_ToggleHardness.Check(false);

	y = 60;
	x = 300;
	m_fowEnabledLabel.Init(hwnd, "Enable FOW", x, y, 100, 16, EEditorTypeID::eMapTabFowEnabledLabel);
	y += 20;
	m_fowEnabledRadioContainer.Init(hwnd, x, y, 0, 0, EEditorTypeID::eMapTabFowEnabledRadioContainer);
	m_fowEnabledRadioContainer.AddRadioButton(hwnd, true, "Yes", x, y, 50, 50, EEditorTypeID::eMapTabFowYesRadioButton);
	m_fowEnabledRadioContainer.AddRadioButton(hwnd, false, "No", x + 50, y, 50, 50, EEditorTypeID::eMapTabFowNoRadioButton);
	SendMessage(m_fowEnabledRadioContainer.GetButtonHandle(1), BM_SETCHECK, BST_CHECKED, 0);

	y += 55;
	m_fowFlashlightEnabled.Init(hwnd, "FOW Flashlight", x, y, 130, 20, EEditorTypeID::eMapTabFowFlashlightCheckbox);
	m_fowFlashlightEnabled.Check(false);
	EnableWindow(m_fowFlashlightEnabled.GetHwnd(), false);

	y += 20;
	m_fowRadiusLabel.Init(hwnd, "FOW Radius", x, y + 5, 100, 16, EEditorTypeID::eMapTabFowRadiusLabel);
	m_fowRadiusComboBox.Init(hwnd, x, y + 23, 80, 500, EEditorTypeID::eMapTabFowRadiusComboBox);
	m_fowRadiusComboBox.AddItem("3");
	m_fowRadiusComboBox.AddItem("4");
	m_fowRadiusComboBox.AddItem("5");
	m_fowRadiusComboBox.AddItem("6");
	m_fowRadiusComboBox.AddItem("7");
	m_fowRadiusComboBox.AddItem("8");
	m_fowRadiusComboBox.AddItem("9");
	m_fowRadiusComboBox.SetDisplayItem("3", 0);

	EnableWindow(m_fowRadiusComboBox.GetHwnd(), false);

	m_UpdateAttributes.Init(hwnd, "Update", 30, 500, 100, 30, EEditorTypeID::eMapTabUpdteAttributesButton);
	
	x = 30;
	y += 50;
	
	m_tileSizeText.Init(hwnd, "Tile Size", x, y, 140, 16, EEditorTypeID::eMapTabTileSizeLabel);
	m_tileSizeTextbox.Init(hwnd, x + 142, y, 80, 20, EEditorTypeID::eMapTabTileSizeTextbox);
	EnableWindow(m_tileSizeTextbox.GetHwnd(), false);
	
	y += 25;
	m_numTilesText.Init(hwnd, "Num Tiles", x, y, 140, 16, EEditorTypeID::eMapTabNumTilesLabel);
	m_numTilesTextbox.Init(hwnd, x + 142, y, 80, 20, EEditorTypeID::eMapTabNumTilesTextbox);
	EnableWindow(m_numTilesTextbox.GetHwnd(), false);

	y += 25;
	m_numSectionsText.Init(hwnd, "Num Sections", x, y, 140, 16, EEditorTypeID::eMapTabNumSectionsLabel);
	m_numSectionsTextbox.Init(hwnd, x + 142, y, 80, 20, EEditorTypeID::eMapTabNumSectionsTextbox);
	EnableWindow(m_numSectionsTextbox.GetHwnd(), false);

	y += 25;
	m_mapRowSizeText.Init(hwnd, "Map Row Size", x, y, 140, 16, EEditorTypeID::eMapTabRowSizeLabel);
	m_mapRowSizeTextbox.Init(hwnd, x + 142, y, 80, 20, EEditorTypeID::eMapTabRowSizeTextbox);
	EnableWindow(m_mapRowSizeTextbox.GetHwnd(), false);

	y += 25;
	m_mapColSizeText.Init(hwnd, "Map Col Size", x, y, 140, 16, EEditorTypeID::eMapTabColSizeLabel);
	m_mapColSizeTextbox.Init(hwnd, x + 142, y, 80, 20, EEditorTypeID::eMapTabColSizeTextbox);
	EnableWindow(m_mapColSizeTextbox.GetHwnd(), false);

	y += 25;
	m_sectionRowSizeText.Init(hwnd, "Section Row Size", x, y, 140, 16, EEditorTypeID::eMapTabSectionRowSizeLabel);
	m_sectionRowSizeTextbox.Init(hwnd, x + 142, y, 80, 20, EEditorTypeID::eMapTabSectionRowSizeTextbox);
	EnableWindow(m_sectionRowSizeTextbox.GetHwnd(), false);

	y += 25;
	m_sectionColSizeText.Init(hwnd, "Section Col Size", x, y, 140, 16, EEditorTypeID::eMapTabSectionColSizeLabel);
	m_sectionColSizeTextbox.Init(hwnd, x + 142, y, 80, 20, EEditorTypeID::eMapTabSectionColSizeTextbox);
	EnableWindow(m_sectionColSizeTextbox.GetHwnd(), false);
	
	y += 25;
	m_spritesRenderedText.Init(hwnd, "Sprites Rendered", x, y, 140, 16, EEditorTypeID::eMapTabSpritesRenderedLabel);
	m_spritesRenderedTextbox.Init(hwnd, x + 142, y, 80, 20, EEditorTypeID::eMapTabSpritesRenderedTextbox);
	EnableWindow(m_spritesRenderedTextbox.GetHwnd(), false);

	y += 25;
	m_spritesTotalText.Init(hwnd, "Total Sprites", x, y, 140, 16, EEditorTypeID::eMapTabTotalSpritesRenderedLabel);
	m_spritesTotalTextbox.Init(hwnd, x + 142, y, 80, 20, EEditorTypeID::eMapTabTotalSpritesRenderedTextbox);
	EnableWindow(m_spritesTotalTextbox.GetHwnd(), false);
}
//================================================================================================================
void ToolWindow2DMapTab::VisibleTabItems(bool visible)
{
	m_MapNameLabel.SetVisible(visible);
	m_EditMapName.SetVisible(visible);
	m_MapSizeLabel.SetVisible(visible);
	m_EditMapSize.SetVisible(visible);
	m_MapVisionLabel.SetVisible(visible);
	m_EditMapVision.SetVisible(visible);
	m_MapMusicLabel.SetVisible(visible);
	m_EditMapMusic.SetVisible(visible);
	m_ToggleHardness.SetVisible(visible);
	m_UpdateAttributes.SetVisible(visible);
	m_fowEnabledLabel.SetVisible(visible);
	m_fowEnabledRadioContainer.SetVisible(visible);
	m_fowRadiusLabel.SetVisible(visible);
	m_fowRadiusComboBox.SetVisible(visible);
	m_fowFlashlightEnabled.SetVisible(visible);
	m_tileSizeText.SetVisible(visible);
	m_tileSizeTextbox.SetVisible(visible);
	m_numTilesText.SetVisible(visible);
	m_numTilesTextbox.SetVisible(visible);
	m_numSectionsText.SetVisible(visible);
	m_numSectionsTextbox.SetVisible(visible);
	m_mapRowSizeText.SetVisible(visible);
	m_mapRowSizeTextbox.SetVisible(visible);
	m_mapColSizeText.SetVisible(visible);
	m_mapColSizeTextbox.SetVisible(visible);
	m_sectionRowSizeText.SetVisible(visible);
	m_sectionRowSizeTextbox.SetVisible(visible);
	m_sectionColSizeText.SetVisible(visible);
	m_sectionColSizeTextbox.SetVisible(visible);
	m_spritesRenderedText.SetVisible(visible);
	m_spritesRenderedTextbox.SetVisible(visible);
	m_spritesTotalText.SetVisible(visible);
	m_spritesTotalTextbox.SetVisible(visible);
}
//================================================================================================================
void ToolWindow2DMapTab::EnableTabItems(bool enable)
{
	m_MapNameLabel.Enable(enable);
	m_EditMapName.Enable(enable);
	m_MapSizeLabel.Enable(enable);
	m_EditMapSize.Enable(enable);
	m_MapVisionLabel.Enable(enable);
	m_EditMapVision.Enable(enable);
	m_MapMusicLabel.Enable(enable);
	m_EditMapMusic.Enable(enable);
	m_ToggleHardness.Enable(enable);
	m_UpdateAttributes.Enable(enable);
	m_fowEnabledLabel.Enable(enable);
	m_fowEnabledRadioContainer.Enable(enable);
	m_fowRadiusLabel.Enable(enable);
	m_fowRadiusComboBox.Enable(enable);
	m_fowFlashlightEnabled.Enable(enable);
	m_tileSizeText.Enable(enable);
	m_tileSizeTextbox.Enable(enable);
	m_numTilesText.Enable(enable);
	m_numTilesTextbox.Enable(enable);
	m_numSectionsText.Enable(enable);
	m_numSectionsTextbox.Enable(enable);
	m_mapRowSizeText.Enable(enable);
	m_mapRowSizeTextbox.Enable(enable);
	m_mapColSizeText.Enable(enable);
	m_mapColSizeTextbox.Enable(enable);
	m_sectionRowSizeText.Enable(enable);
	m_sectionRowSizeTextbox.Enable(enable);
	m_sectionColSizeText.Enable(enable);
	m_sectionColSizeTextbox.Enable(enable);
	m_spritesRenderedText.Enable(enable);
	m_spritesRenderedTextbox.Enable(enable);
	m_spritesTotalText.Enable(enable);
	m_spritesTotalTextbox.Enable(enable);
}
//================================================================================================================
#pragma region "Set Attributes"
void ToolWindow2DMapTab::SetMapName(string name)
{
	m_EditMapName.SetText(name);
}
//================================================================================================================
void ToolWindow2DMapTab::SetMapSize(string size)
{
	m_EditMapSize.SetText(size);
}
//================================================================================================================
void ToolWindow2DMapTab::SetMapVision(string vision)
{
	m_EditMapVision.SetText(vision);
}
//================================================================================================================
void ToolWindow2DMapTab::SetMapMusic(string music)
{
	m_EditMapMusic.SetText(music);
}
//================================================================================================================
void ToolWindow2DMapTab::CheckFOW(bool fow)
{
	if (fow)
		m_fowEnabledRadioContainer.Check(0, true);
	else
		m_fowEnabledRadioContainer.Check(1, true);
}
//================================================================================================================
void ToolWindow2DMapTab::FOWFlashlightEnabled(bool enabled)
{
	m_fowFlashlightEnabled.Check(enabled);
}
//================================================================================================================
void ToolWindow2DMapTab::SetFOWRadius(int radius)
{
	string fowr = ZShadeSandboxGlobal::Convert::ConvertIntToString(radius);

	int did = 0;
	if (radius == 3) did = 0;
	else if (radius == 4) did = 1;
	else if (radius == 5) did = 2;
	else if (radius == 6) did = 3;
	else if (radius == 7) did = 4;
	else if (radius == 8) did = 5;
	else if (radius == 9) did = 6;

	m_fowRadiusComboBox.SetDisplayItem(fowr, did);
}
//================================================================================================================
void ToolWindow2DMapTab::SetTileSize(string val)
{
	m_tileSizeTextbox.SetText(val);
}
//================================================================================================================
void ToolWindow2DMapTab::SetNumTiles(string val)
{
	m_numTilesTextbox.SetText(val);
}
//================================================================================================================
void ToolWindow2DMapTab::SetNumSections(string val)
{
	m_numSectionsTextbox.SetText(val);
}
//================================================================================================================
void ToolWindow2DMapTab::SetRowSize(string val)
{
	m_mapRowSizeTextbox.SetText(val);
}
//================================================================================================================
void ToolWindow2DMapTab::SetColSize(string val)
{
	m_mapColSizeTextbox.SetText(val);
}
//================================================================================================================
void ToolWindow2DMapTab::SetSectionRowSize(string val)
{
	m_sectionRowSizeTextbox.SetText(val);
}
//================================================================================================================
void ToolWindow2DMapTab::SetSectionColSize(string val)
{
	m_sectionColSizeTextbox.SetText(val);
}
//================================================================================================================
void ToolWindow2DMapTab::SetNumSpritesRendered(string val)
{
	m_spritesRenderedTextbox.SetText(val);
}
//================================================================================================================
void ToolWindow2DMapTab::SetTotalSprites(string val)
{
	m_spritesTotalTextbox.SetText(val);
}
#pragma endregion
//================================================================================================================
void ToolWindow2DMapTab::Update()
{
	
}
//================================================================================================================
LRESULT ToolWindow2DMapTab::Command(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{


	return true;
}
//================================================================================================================