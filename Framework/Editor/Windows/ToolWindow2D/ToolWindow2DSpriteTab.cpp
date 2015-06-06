#include "ToolWindow2DSpriteTab.h"
#include "EditorTypeID.h"
//================================================================================================================
//================================================================================================================
ToolWindow2DSpriteTab::ToolWindow2DSpriteTab(HWND hwnd, EngineOptions* eo, D3D* d3d)
:   ToolWindow2DTab(hwnd, eo, d3d)
{
	BuildTab();
	EnableTab();
}
//================================================================================================================
ToolWindow2DSpriteTab::~ToolWindow2DSpriteTab()
{
}
//================================================================================================================
void ToolWindow2DSpriteTab::BuildTab()
{
	int x = 10;
	int y = 30;

	HWND hwnd = m_hwnd;

	int ItemSize = 20;
	m_SpritesList.Init(hwnd, x, y, 100, ItemSize * 3, EEditorTypeID::eSpritesTabSpritesComboBox);
	m_SpritesList.AddItem("Item 1");
	m_SpritesList.AddItem("Item 2");
	m_SpritesList.SetDisplayItem("Item 1", 0);

	y += 40;

	//m_OpenSpritesProperties.Init(hwnd, "Sprite Properties", x, y, 120, 30, EEditorTypeID::eSpritesTabUpdateSpritePropertiesButton);
}
//================================================================================================================
void ToolWindow2DSpriteTab::VisibleTabItems(bool visible)
{
	m_SpritesList.SetVisible(visible);
	//m_OpenSpritesProperties.SetVisible(visible);
}
//================================================================================================================
void ToolWindow2DSpriteTab::EnableTabItems(bool enable)
{
	m_SpritesList.Enable(enable);
	//m_OpenSpritesProperties.Enable(enable);
}
//================================================================================================================
void ToolWindow2DSpriteTab::Update()
{
	
}
//================================================================================================================
LRESULT ToolWindow2DSpriteTab::Command(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{


	return true;
}
//================================================================================================================