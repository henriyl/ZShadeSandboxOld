#include "ToolWindow2D.h"
#include <functional>
using std::tr1::bind;
using std::tr1::mem_fn;
using namespace std::tr1::placeholders;
//================================================================================================================
//================================================================================================================
ToolWindow2D::ToolWindow2D(EngineOptions* eo, D3D* d3d, Environment2D* env2D, SplitterWindow*& editWindow)
:   m_D3DSystem(d3d)
,   m_EngineOptions(eo)
,   m_BaseEditWindowRef(editWindow)
,   m_Env2DRef(env2D)
{
	Init();
}
//================================================================================================================
ToolWindow2D::~ToolWindow2D()
{

}
//================================================================================================================
void ToolWindow2D::Init()
{
	bEditorTab = true;
	bWorldTab = false;
	bMapTab = false;
	bTilesTab = false;
	bSpritesTab = false;
	bTileCacheTab = false;
	bSpriteCacheTab = false;
	
	m_BaseEditWindowRef->SetUserMessageFunctionRight(WM_NOTIFY, bind(mem_fn(&ToolWindow2D::NotifyTab), this, _1, _2, _3, _4));
	m_BaseEditWindowRef->SetUserMessageFunctionRight(WM_COMMAND, bind(mem_fn(&ToolWindow2D::Command), this, _1, _2, _3, _4));

	m_tabControl = new WindowTabControl(m_BaseEditWindowRef->GetHwndRight());

	m_MapEditorSystem2D = new MapEditorSystem2D(m_Env2DRef);
	//m_MenuEditorSystem = new MenuEditorSystem(m_Env2DRef);
	//m_HUDEditorSystem = new HUDEditorSystem(m_Env2DRef);
	
	//m_NewMenuWindow = new NewMenuWindow(m_MenuEditorSystem, m_EngineOptions->hinstance, m_D3DSystem);
	//m_NewMenuWindow->Show(false);
	//m_NewHUDWindow = new NewHUDWindow(m_HUDEditorSystem, m_EngineOptions->hinstance, m_D3DSystem);
	//m_NewHUDWindow->Show(false);
	
	m_tabControl->InsertTab("Editor");
	m_tabControl->InsertTab("World");
	m_tabControl->InsertTab("Map");
	m_tabControl->InsertTab("Tiles");
	m_tabControl->InsertTab("Sprites");
	m_tabControl->InsertTab("Tile Cache");
	m_tabControl->InsertTab("Sprite Cache");
	
	m_editorTab = new ToolWindow2DEditorTab(m_BaseEditWindowRef->GetHwndRight(), m_EngineOptions, m_D3DSystem);
	m_worldTab = new ToolWindow2DWorldTab(m_BaseEditWindowRef->GetHwndRight(), m_EngineOptions, m_D3DSystem);
	m_mapTab = new ToolWindow2DMapTab(m_BaseEditWindowRef->GetHwndRight(), m_EngineOptions, m_D3DSystem);
	m_tileTab = new ToolWindow2DTileTab(m_BaseEditWindowRef->GetHwndRight(), m_EngineOptions, m_D3DSystem);
	m_spriteTab = new ToolWindow2DSpriteTab(m_BaseEditWindowRef->GetHwndRight(), m_EngineOptions, m_D3DSystem);
	m_tileCacheTab = new ToolWindow2DTileCacheTab(m_BaseEditWindowRef->GetHwndRight(), m_EngineOptions, m_D3DSystem);
	m_spriteCacheTab = new ToolWindow2DSpriteCacheTab(m_BaseEditWindowRef->GetHwndRight(), m_EngineOptions, m_D3DSystem);
	
	// Only the editor tab is visible at start
	m_worldTab->DisableTab();
	m_mapTab->DisableTab();
	m_tileTab->DisableTab();
	m_spriteTab->DisableTab();
	m_tileCacheTab->DisableTab();
	m_spriteCacheTab->DisableTab();
}
//================================================================================================================
void ToolWindow2D::ShowWindow()
{
	bEditorTab = true;
	bWorldTab = false;
	bMapTab = false;
	bTilesTab = false;
	bSpritesTab = false;
	bTileCacheTab = false;
	bSpriteCacheTab = false;
	
	m_editorTab->EnableTab();
	m_worldTab->DisableTab();
	m_mapTab->DisableTab();
	m_tileTab->DisableTab();
	m_spriteTab->DisableTab();
	m_tileCacheTab->DisableTab();
	m_spriteCacheTab->DisableTab();
	
	m_tabControl->SetVisible(true);
	m_tabControl->Enable(true);
}
//================================================================================================================
void ToolWindow2D::HideWindow()
{
	bEditorTab = false;
	bWorldTab = false;
	bMapTab = false;
	bTilesTab = false;
	bSpritesTab = false;
	bTileCacheTab = false;
	bSpriteCacheTab = false;
	
	m_editorTab->DisableTab();
	m_worldTab->DisableTab();
	m_mapTab->DisableTab();
	m_tileTab->DisableTab();
	m_spriteTab->DisableTab();
	m_tileCacheTab->DisableTab();
	m_spriteCacheTab->DisableTab();
	
	m_tabControl->SetVisible(false);
	m_tabControl->Enable(false);
}
//================================================================================================================
bool ToolWindow2D::DoHideWindow()
{
	return m_editorTab->DoHideWindow();
}
//================================================================================================================
void ToolWindow2D::SetDoHideWindow(bool set)
{
	m_editorTab->SetDoHideWindow(set);
}
//================================================================================================================
void ToolWindow2D::Update()
{
	//m_editorTab->UpdateEditSubModeTypeVisible();
	//m_editorTab->UpdateEditSubModeTypeEnable();
	
	bEditorTypeMenu = m_editorTab->IsEditorTypeMenu();
	bEditorTypeHUD = m_editorTab->IsEditorTypeHUD();
	
	if (!bEditorTypeMenu && !bEditorTypeHUD)
	{
		bEditorTypeMap = true;
	}
	else
	{
		bEditorTypeMap = false;
	}
	
	// Add all updates for all the tabs in here since most of the tabs work together to do something
	
}
//================================================================================================================
LRESULT ToolWindow2D::Command(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//if (!m_BaseEditWindowRef->HasFocus())
	//{
	//	return true;
	//}

	if (m_editorTab->IsEnabled())
		m_editorTab->Command(hwnd, msg, wParam, lParam);
	
	if (m_worldTab->IsEnabled())
		m_worldTab->Command(hwnd, msg, wParam, lParam);
	
	if (m_mapTab->IsEnabled())
		m_mapTab->Command(hwnd, msg, wParam, lParam);
	
	if (m_tileTab->IsEnabled())
		m_tileTab->Command(hwnd, msg, wParam, lParam);
	
	if (m_spriteTab->IsEnabled())
		m_spriteTab->Command(hwnd, msg, wParam, lParam);
	
	if (m_tileCacheTab->IsEnabled())
		m_tileCacheTab->Command(hwnd, msg, wParam, lParam);
	
	if (m_spriteCacheTab->IsEnabled())
		m_spriteCacheTab->Command(hwnd, msg, wParam, lParam);
	
	return true;
}
//================================================================================================================
LRESULT ToolWindow2D::NotifyTab(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//if (!m_BaseEditWindowRef->HasFocus())
	//{
	//	return false;
	//}

	switch (((LPNMHDR)lParam)->code)
	{
		case TCN_SELCHANGE:
		{
			int iPage = m_tabControl->TabID();

			// Clear what is on the screen meaning make buttons and stuff invisible
			m_editorTab->DisableTab();
			m_worldTab->DisableTab();
			m_mapTab->DisableTab();
			m_tileTab->DisableTab();
			m_spriteTab->DisableTab();
			m_tileCacheTab->DisableTab();
			m_spriteCacheTab->DisableTab();
			
			if (iPage == 0)
			{
				// Show content for this tab
				if (!m_editorTab->IsEnabled())
				{
					m_editorTab->EnableTab();
				}
			}
			else if (iPage == 1)
			{
				// Show content for this tab
				if (!m_worldTab->IsEnabled())
				{
					m_worldTab->EnableTab();
				}
			}
			else if (iPage == 2)
			{
				// Show content for this tab
				if (!m_mapTab->IsEnabled())
				{
					m_mapTab->EnableTab();
				}
			}
			else if (iPage == 3)
			{
				// Show content for this tab
				if (!m_tileTab->IsEnabled())
				{
					m_tileTab->EnableTab();
				}
			}
			else if (iPage == 4)
			{
				// Show content for this tab
				if (!m_spriteTab->IsEnabled())
				{
					m_spriteTab->EnableTab();
				}
			}
			else if (iPage == 5)
			{
				// Show content for this tab
				if (!m_tileCacheTab->IsEnabled())
				{
					m_tileCacheTab->EnableTab();
				}
			}
			else if (iPage == 6)
			{
				// Show content for this tab
				if (!m_spriteCacheTab->IsEnabled())
				{
					m_spriteCacheTab->EnableTab();
				}
			}
		}
		break;
	}

	return 0;
}
//================================================================================================================
