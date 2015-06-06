#include "ToolWindow3DTab.h"
#include <functional>
using std::tr1::bind;
using std::tr1::mem_fn;
using namespace std::tr1::placeholders;
//================================================================================================================
//================================================================================================================
ToolWindow3DTab::ToolWindow3DTab(HWND hwnd, EngineOptions* eo, D3D* d3d)
:   m_EngineOptions(eo)
,   m_D3DSystem(d3d)
,   m_enabled(true)
{
	m_hwnd = hwnd;
}
//================================================================================================================
ToolWindow3DTab::~ToolWindow3DTab()
{

}
//================================================================================================================
void ToolWindow3DTab::EnableTabItems(bool enable)
{
}
//================================================================================================================
void ToolWindow3DTab::AddMenuEditorSystem(MenuEditorSystem* menuEditorSystem)
{
	m_menuEditorSystem = menuEditorSystem;
	m_NewMenuWindow->AddMenuEditorSystem(m_menuEditorSystem);
}
//================================================================================================================
void ToolWindow3DTab::AddHUDEditorSystem(HUDEditorSystem* hudEditorSystem)
{
	m_hudEditorSystem = hudEditorSystem;
	m_NewHUDWindow->AddHUDEditorSystem(m_hudEditorSystem);
}
//================================================================================================================
void ToolWindow3DTab::Update()
{
//#pragma region "Editor Type HUD"
//	if (m_editorTab->IsEditorTypeHUD())
//	{
//
//	}
//#pragma endregion
//#pragma region "Editor Type Menu"
//	if (m_editorTab->IsEditorTypeMenu())
//	{
//
//	}
//#pragma endregion
//#pragma region "Editor Type Map"
//	if (m_editorTab->IsEditorTypeMap())
//	{
//		if (!m_editorTab->GameDirectoryCreated())
//		{
//			EnableTabItems(false);
//		}
//	}
//#pragma endregion
}
//================================================================================================================