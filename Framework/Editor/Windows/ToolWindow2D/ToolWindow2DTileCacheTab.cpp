#include "ToolWindow2DTileCacheTab.h"
#include "CGlobal.h"
#include "EditorTypeID.h"
//================================================================================================================
//================================================================================================================
ToolWindow2DTileCacheTab::ToolWindow2DTileCacheTab(HWND hwnd, EngineOptions* eo, D3D* d3d)
:   ToolWindow2DTab(hwnd, eo, d3d)
{
	BuildTab();
	EnableTab();
}
//================================================================================================================
ToolWindow2DTileCacheTab::~ToolWindow2DTileCacheTab()
{
}
//================================================================================================================
void ToolWindow2DTileCacheTab::BuildTab()
{
	int x = 10;
	int y = 30;

	HWND hwnd = m_hwnd;

	m_TileCacheList.Init(hwnd, x, y, 200, 400, EEditorTypeID::eTileCacheTabCacheList);

	y += 400;

	m_TileNameLabel.Init(hwnd, "Selected Tile: ", x, y, 100, 30, EEditorTypeID::eTileCacheTabSelectedTileLabel);
	m_TileNameLabel2.Init(hwnd, "", x + 110, y, 150, 30, EEditorTypeID::eTileCacheTabSelectedTileLabel2);

	y += 40;

	m_TileCacheButton.Init(hwnd, "Select Tile", x, y, 120, 30, EEditorTypeID::eTileCacheTabSelectTileButton);
	m_TileCacheUpdateButton.Init(hwnd, "Update Cache", x + 130, y, 120, 30, EEditorTypeID::eTileCacheTabUpdateCacheButton);
}
//================================================================================================================
void ToolWindow2DTileCacheTab::VisibleTabItems(bool visible)
{
	m_TileCacheList.SetVisible(visible);
	m_TileCacheButton.SetVisible(visible);
	m_TileCacheUpdateButton.SetVisible(visible);
	m_TileNameLabel.SetVisible(visible);
	m_TileNameLabel2.SetVisible(visible);
}
//================================================================================================================
void ToolWindow2DTileCacheTab::EnableTabItems(bool enable)
{
	m_TileCacheList.Enable(enable);
	m_TileCacheButton.Enable(enable);
	m_TileCacheUpdateButton.Enable(enable);
	m_TileNameLabel.Enable(enable);
	m_TileNameLabel2.Enable(enable);
}
//================================================================================================================
void ToolWindow2DTileCacheTab::FillCache()
{
	if (m_EngineOptions->m_GD2D == NULL) return;
	
	vector<string> names = CGlobal::CacheNames(m_EngineOptions->m_GD2D->m_tiles_path);
	
	for (int i = 0; i < names.size(); i++)
		m_TileCacheList.AddItem(names[i]);
}
//================================================================================================================
void ToolWindow2DTileCacheTab::Update()
{
	
}
//================================================================================================================
LRESULT ToolWindow2DTileCacheTab::Command(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (HIWORD(wParam))
	{
		case LBN_DBLCLK:
		{
			// Selects the current tile highlighted in the listbox
			m_SelectedTileName = m_TileCacheList.GetItem(wParam);

			m_TileNameLabel2.SetText(m_SelectedTileName);

			//mImageSampleWindow->LoadImageSample(m_gameDirectory->m_tiles_path, m_SelectedTileName);
			//mImageSampleWindow->Invalidate();
		}
		break;
	}
	
	switch (LOWORD(wParam))
	{
		case EEditorTypeID::eTileCacheTabSelectTileButton:
		{
			// Selects the current tile highlighted in the listbox
			m_SelectedTileName = m_TileCacheList.GetItem(wParam);

			m_TileNameLabel2.SetText(m_SelectedTileName);

			//mImageSampleWindow->LoadImageSample(m_gameDirectory->m_tiles_path, m_SelectedTileName);
			//mImageSampleWindow->Invalidate();
		}
		break;
		case EEditorTypeID::eTileCacheTabUpdateCacheButton:
		{
			m_TileCacheList.DeleteList();

			// Updates the cache
			FillCache();
		}
		break;
	}
	
	return true;
}
//================================================================================================================