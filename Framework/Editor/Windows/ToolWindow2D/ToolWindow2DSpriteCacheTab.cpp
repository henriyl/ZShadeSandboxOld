#include "ToolWindow2DSpriteCacheTab.h"
#include "CGlobal.h"
#include "EditorTypeID.h"
//================================================================================================================
//================================================================================================================
ToolWindow2DSpriteCacheTab::ToolWindow2DSpriteCacheTab(HWND hwnd, EngineOptions* eo, D3D* d3d)
:   ToolWindow2DTab(hwnd, eo, d3d)
{
	BuildTab();
	EnableTab();
}
//================================================================================================================
ToolWindow2DSpriteCacheTab::~ToolWindow2DSpriteCacheTab()
{
}
//================================================================================================================
void ToolWindow2DSpriteCacheTab::BuildTab()
{
	int x = 10;
	int y = 30;

	HWND hwnd = m_hwnd;

	m_SpriteCacheList.Init(hwnd, x, y, 200, 400, EEditorTypeID::eSpriteCacheTabCacheList);

	y += 400;

	m_SpriteNameLabel.Init(hwnd, "Selected Sprite: ", x, y, 130, 30, EEditorTypeID::eSpriteCacheTabSelectedSpriteLabel);
	m_SpriteNameLabel2.Init(hwnd, "", x + 110, y, 150, 30, EEditorTypeID::eSpriteCacheTabSelectedSpriteLabel2);

	y += 40;

	m_SpriteCacheButton.Init(hwnd, "Select Sprite", x, y, 120, 30, EEditorTypeID::eSpriteCacheTabSelectSpriteButton);
	m_SpriteCacheUpdateButton.Init(hwnd, "Update Cache", x + 130, y, 120, 30, EEditorTypeID::eSpriteCacheTabUpdateCacheButton);
}
//================================================================================================================
void ToolWindow2DSpriteCacheTab::VisibleTabItems(bool visible)
{
	m_SpriteCacheList.SetVisible(visible);
	m_SpriteCacheButton.SetVisible(visible);
	m_SpriteCacheUpdateButton.SetVisible(visible);
	m_SpriteNameLabel.SetVisible(visible);
	m_SpriteNameLabel2.SetVisible(visible);
}
//================================================================================================================
void ToolWindow2DSpriteCacheTab::EnableTabItems(bool enable)
{
	m_SpriteCacheList.Enable(enable);
	m_SpriteCacheButton.Enable(enable);
	m_SpriteCacheUpdateButton.Enable(enable);
	m_SpriteNameLabel.Enable(enable);
	m_SpriteNameLabel2.Enable(enable);
}
//================================================================================================================
void ToolWindow2DSpriteCacheTab::FillCache()
{
	if (m_EngineOptions->m_GD2D == NULL) return;

	vector<string> names = CGlobal::CacheNames(m_EngineOptions->m_GD2D->m_sprites_path);

	for (int i = 0; i < names.size(); i++)
		m_SpriteCacheList.AddItem(names[i]);
}
//================================================================================================================
void ToolWindow2DSpriteCacheTab::Update()
{
	
}
//================================================================================================================
LRESULT ToolWindow2DSpriteCacheTab::Command(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (HIWORD(wParam))
	{
		case LBN_DBLCLK:
		{
			// Selects the current tile highlighted in the listbox
			m_SelectedSpriteName = m_SpriteCacheList.GetItem(wParam);
			m_displaySpriteLoaded = true;

			m_SpriteNameLabel2.SetText(m_SelectedSpriteName);

			//mImageSampleWindow->LoadImageSample(m_gameDirectory->m_sprites_path, m_SelectedSpriteName);
			//mImageSampleWindow->Invalidate();
		}
		break;
	}

	switch (LOWORD(wParam))
	{
		case EEditorTypeID::eSpriteCacheTabSelectSpriteButton:
		{
			// Selects the current sprite highlighted in the listbox
			m_SelectedSpriteName = m_SpriteCacheList.GetItem(wParam);
			m_displaySpriteLoaded = true;

			m_SpriteNameLabel2.SetText(m_SelectedSpriteName);

			//mImageSampleWindow->LoadImageSample(m_gameDirectory->m_sprites_path, m_SelectedSpriteName);
			//mImageSampleWindow->Invalidate();
		}
		break;
		case EEditorTypeID::eSpriteCacheTabUpdateCacheButton:
		{
			m_SpriteCacheList.DeleteList();
			
			// Updates the cache
			FillCache();
		}
		break;
	}
	
	return true;
}
//================================================================================================================