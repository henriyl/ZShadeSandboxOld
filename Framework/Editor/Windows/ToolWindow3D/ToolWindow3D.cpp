#include "ToolWindow3D.h"
#include <functional>
using std::tr1::bind;
using std::tr1::mem_fn;
using namespace std::tr1::placeholders;
//================================================================================================================
//================================================================================================================
ToolWindow3D::ToolWindow3D(EngineOptions* eo, D3D* d3d, Environment3D* env2D, SplitterWindow* editWindow)
:   m_D3DSystem(d3d)
,   m_EngineOptions(eo)
,   m_BaseEditWindowRef(editWindow)
,   m_Env3DRef(env2D)
{
	Init();
}
//================================================================================================================
ToolWindow3D::~ToolWindow3D()
{

}
//================================================================================================================
void ToolWindow3D::Init()
{
	bEditorTab = true;
	bWorldTab = false;
	bMapTab = false;
	bTilesTab = false;
	bSpritesTab = false;

	m_BaseEditWindowRef->SetUserMessageFunctionRight(WM_COMMAND, bind(mem_fn(&ToolWindow3D::Command), this, _1, _2, _3, _4));
	m_BaseEditWindowRef->SetUserMessageFunctionRight(WM_MOVE, bind(mem_fn(&ToolWindow3D::WindowMoved), this, _1, _2, _3, _4));
	m_BaseEditWindowRef->SetUserMessageFunctionRight(WM_NOTIFY, bind(mem_fn(&ToolWindow3D::NotifyTab), this, _1, _2, _3, _4));

	m_tabControl = new WindowTabControl(m_BaseEditWindowRef->GetHwndRight());

	//m_MenuEditorSystem = new MenuEditorSystem(m_Env2DRef);
	//m_HUDEditorSystem = new HUDEditorSystem(m_Env2DRef);

	//m_NewMenuWindow = new NewMenuWindow(m_MenuEditorSystem, m_EngineOptions->hinstance, m_D3DSystem);
	//m_NewMenuWindow->Show(false);

	//m_NewHUDWindow = new NewHUDWindow(m_HUDEditorSystem, m_EngineOptions->hinstance, m_D3DSystem);
	//m_NewHUDWindow->Show(false);

	m_tabControl->InsertTab("Editor");
	m_tabControl->InsertTab("Terrain");
	m_tabControl->InsertTab("Water");
	m_tabControl->InsertTab("Vegitation");
	m_tabControl->InsertTab("Environment");
	m_tabControl->InsertTab("Post Processing");
	m_tabControl->InsertTab("Lighting");
	m_tabControl->InsertTab("Model");

	m_editorTab = new ToolWindow3DEditorTab(m_tabControl->GetHwnd(), m_EngineOptions, m_D3DSystem);
	//m_editorTab->AddNewMenuWindow(m_NewMenuWindow);
	//m_editorTab->AddNewHUDWindow(m_NewHUDWindow);
	m_editorTab->BuildTab();


}
//================================================================================================================
LRESULT ToolWindow3D::Command(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (!m_BaseEditWindowRef->HasFocus())
	{
		return true;
	}

	return 0;
}
//================================================================================================================
LRESULT ToolWindow3D::WindowMoved(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	return 0;
}
//================================================================================================================
LRESULT ToolWindow3D::NotifyTab(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	return 0;
}
//================================================================================================================
