#include "ToolWindow2DTileTab.h"
#include "EditorTypeID.h"
//================================================================================================================
//================================================================================================================
ToolWindow2DTileTab::ToolWindow2DTileTab(HWND hwnd, EngineOptions* eo, D3D* d3d)
:   ToolWindow2DTab(hwnd, eo, d3d)
{
	BuildTab();
	EnableTab();
}
//================================================================================================================
ToolWindow2DTileTab::~ToolWindow2DTileTab()
{
}
//================================================================================================================
void ToolWindow2DTileTab::BuildTab()
{
	int x = 10;
	int y = 30;
	
	HWND hwnd = m_hwnd;
	
	// Need to get the tiles in the map
	int ItemSize = 20;
	m_TilesList.Init(hwnd, x, y, 100, ItemSize * 3, EEditorTypeID::eTilesTabTilesComboBox);
	m_TilesList.AddItem("Item 1");
	m_TilesList.AddItem("Item 2");
	m_TilesList.SetDisplayItem("Item 1", 0);
	
	y += 40;
	
	m_xPosText.Init(hwnd, "X", x, y, 10, 16, EEditorTypeID::eTilesTabXPosLabel);
	m_xPosTextBox.Init(hwnd, x + 20, y, 50, 20, EEditorTypeID::eTilesTabXPosTextbox);
	EnableWindow(m_xPosTextBox.GetHwnd(), false);

	y += 25;
	m_yPosText.Init(hwnd, "Y", x, y, 10, 16, EEditorTypeID::eTilesTabYPosLabel);
	m_yPosTextBox.Init(hwnd, x + 20, y, 50, 20, EEditorTypeID::eTilesTabYPosTextbox);
	EnableWindow(m_yPosTextBox.GetHwnd(), false);

	y += 30;
	m_baseTextureText.Init(hwnd, "Texture", x, y, 50, 16, EEditorTypeID::eTilesTabBaseTextureLabel);
	m_baseTextureTextBox.Init(hwnd, x + 90, y, 100, 20, EEditorTypeID::eTilesTabBaseTextureTextbox);

	y += 25;
	m_animSpeedText.Init(hwnd, "Anim Speed", x, y, 80, 16, EEditorTypeID::eTilesTabAnimationSpeedLabel);
	m_animSpeedTextBox.Init(hwnd, x + 90, y, 100, 20, EEditorTypeID::eTilesTabAnimationSpeedTextbox);

	y += 25;
	m_animationProfileNameText.Init(hwnd, "Animation", x, y, 70, 16, EEditorTypeID::eTilesTabAnimationNameLabel);
	m_animationProfileNameTextBox.Init(hwnd, x + 90, y, 100, 20, EEditorTypeID::eTilesTabAnimationNameTextbox);
	m_updateAnimationButton.Init(hwnd, "Update Anim", x + 200, y, 90, 20, EEditorTypeID::eTilesTabUpdateAnimationButton);
	m_revertAnimationButton.Init(hwnd, "Revert Anim", x + 300, y, 90, 20, EEditorTypeID::eTilesTabRevertAnimationButton);

	y += 30;
	m_hardText.Init(hwnd, "Hard", x, y, 80, 16, EEditorTypeID::eTilesTabHardLabel);
	m_hardContainer.Init(hwnd, x, y + 17, 0, 0, EEditorTypeID::eTilesTabHardRadioContainer);
	m_hardContainer.AddRadioButton(hwnd, true, "Yes", x, y + 17, 50, 50, EEditorTypeID::eTilesTabHardYesRadioButton);
	m_hardContainer.AddRadioButton(hwnd, false, "No", x + 50, y + 17, 50, 50, EEditorTypeID::eTilesTabHardNoRadioButton);
	SendMessage(m_hardContainer.GetButtonHandle(1), BM_SETCHECK, BST_CHECKED, 0);

	m_UpdateTileProperties.Init(hwnd, "Update", 30, 500, 120, 30, EEditorTypeID::eTilesTabUpdateTilePropertiesButton);
}
//================================================================================================================
void ToolWindow2DTileTab::VisibleTabItems(bool visible)
{
	m_TilesList.SetVisible(visible);
	m_xPosText.SetVisible(visible);
	m_yPosText.SetVisible(visible);
	m_baseTextureText.SetVisible(visible);
	m_animSpeedText.SetVisible(visible);
	m_animationProfileNameText.SetVisible(visible);
	m_hardText.SetVisible(visible);
	m_hardContainer.SetVisible(visible);
	m_xPosTextBox.SetVisible(visible);
	m_yPosTextBox.SetVisible(visible);
	m_baseTextureTextBox.SetVisible(visible);
	m_animSpeedTextBox.SetVisible(visible);
	m_animationProfileNameTextBox.SetVisible(visible);
	m_updateAnimationButton.SetVisible(visible);
	m_revertAnimationButton.SetVisible(visible);
	m_UpdateTileProperties.SetVisible(visible);
}
//================================================================================================================
void ToolWindow2DTileTab::EnableTabItems(bool enable)
{
	m_TilesList.Enable(enable);
	m_xPosText.Enable(enable);
	m_yPosText.Enable(enable);
	m_baseTextureText.Enable(enable);
	m_animSpeedText.Enable(enable);
	m_animationProfileNameText.Enable(enable);
	m_hardText.Enable(enable);
	m_hardContainer.Enable(enable);
	m_xPosTextBox.Enable(enable);
	m_yPosTextBox.Enable(enable);
	m_baseTextureTextBox.Enable(enable);
	m_animSpeedTextBox.Enable(enable);
	m_animationProfileNameTextBox.Enable(enable);
	m_updateAnimationButton.Enable(enable);
	m_revertAnimationButton.Enable(enable);
	m_UpdateTileProperties.Enable(enable);
}
//================================================================================================================
void ToolWindow2DTileTab::Update()
{
	
}
//================================================================================================================
LRESULT ToolWindow2DTileTab::Command(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{


	return true;
}
//================================================================================================================