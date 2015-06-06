#include "ToolWindow2DTab.h"
#include <functional>
using std::tr1::bind;
using std::tr1::mem_fn;
using namespace std::tr1::placeholders;
//================================================================================================================
//================================================================================================================
ToolWindow2DTab::ToolWindow2DTab(HWND hwnd, EngineOptions* eo, D3D* d3d)
:   m_EngineOptions(eo)
,   m_D3DSystem(d3d)
,   m_enabled(true)
{
	m_hwnd = hwnd;
}
//================================================================================================================
ToolWindow2DTab::~ToolWindow2DTab()
{
}
//================================================================================================================
void ToolWindow2DTab::AddMapEditorSystem(MapEditorSystem2D* mapEditorSystem)
{
	m_mapEditorSystem2D = mapEditorSystem;
}
//================================================================================================================
void ToolWindow2DTab::DisableTab()
{
	m_enabled = false;
	EnableTabItems(m_enabled);
	VisibleTabItems(m_enabled);
}
//================================================================================================================
void ToolWindow2DTab::EnableTab()
{
	m_enabled = true;
	EnableTabItems(m_enabled);
	VisibleTabItems(m_enabled);
}
//================================================================================================================
bool ToolWindow2DTab::IsEnabled()
{
	return m_enabled;
}
//================================================================================================================
void ToolWindow2DTab::Update()
{
}
//================================================================================================================