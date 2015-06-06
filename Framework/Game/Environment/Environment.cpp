#include "Environment.h"
#include "ZShadeResource.h"
#include "ZShadeMessageCenter.h"
#include <functional>
#include "EnvironmentFactory.h"
#include "ToolWindow2D.h"
#include "ToolWindow3D.h"
#include "ToolWindowMenu.h"
#include "ToolWindowHUD.h"
#include "MapEditorSystem2D.h"
#include "MenuEditorSystem.h"
#include "HUDEditorSystem.h"
using std::tr1::bind;
using std::tr1::mem_fn;
using namespace std::tr1::placeholders;
//================================================================================================================
//================================================================================================================
Environment::Environment(EngineOptions* engineOptions, LPCSTR base_window_name, LPCSTR render_window_name, bool init_lua)
	: m_EngineOptions(engineOptions)
,  env2D(NULL)
,  env3D(NULL)
{
	Init(base_window_name, render_window_name, init_lua);
}
//================================================================================================================
bool Environment::Init(LPCSTR base_window_name, LPCSTR render_window_name, bool init_lua)
{
	switch (m_EngineOptions->m_DimType)
	{
		case ZSHADE_2D:
		{
			env2D = new Environment2D(base_window_name, render_window_name, m_EngineOptions, init_lua);
			
			if (m_EngineOptions->m_inEditor)
			{
				// Create a separate window for the tab panes
				m_ToolWindow2D = new ToolWindow2D(m_EngineOptions, env2D->GetD3DSystem(), env2D, env2D->GetBaseEditWindow());
				
				m_menuTool = new ToolWindowMenu(m_EngineOptions, env2D->GetD3DSystem(), env2D, 0, env2D->GetBaseEditWindow());
				m_hudTool = new ToolWindowHUD(m_EngineOptions, env2D->GetD3DSystem(), env2D, 0, env2D->GetBaseEditWindow());
				
				env2D->SetMapEditorSystem(m_ToolWindow2D->GetMapEditorSystem2D());
				env2D->SetMenuEditorSystem(m_menuTool->GetMenuEditorSystem());
				env2D->SetHUDEditorSystem(m_hudTool->GetHUDEditorSystem());
				env2D->SetToolWindow2D(m_ToolWindow2D);
				env2D->SetToolMenu(m_menuTool);
				env2D->SetToolHUD(m_hudTool);
				
				//m_TabEditWindow2D->AddMapEditorSystem(m_MapEditorSystem2D);
				//m_TabEditWindow2D->AddMenuEditorSystem(m_MenuEditorSystem);
				//m_TabEditWindow2D->AddHUDEditorSystem(m_HUDEditorSystem);
				//
				//// Loads the game folder from the ini file if there is one
				//m_TabEditWindow2D->LoadGameFolder();
			}
		}
		break;
		case ZSHADE_3D:
		{
			if (m_EngineOptions->m_Env3DType == "Outdoor")
				env3D = new OutdoorEnvironment(base_window_name, render_window_name, m_EngineOptions, init_lua);
			else if (m_EngineOptions->m_Env3DType == "CubeReflection")
				env3D = new CubeReflectionOutdoorEnvironment(base_window_name, render_window_name, m_EngineOptions, init_lua);
			else if (m_EngineOptions->m_Env3DType == "Ocean")
				env3D = new OceanEnvironment(base_window_name, render_window_name, m_EngineOptions, init_lua);
			else if (m_EngineOptions->m_Env3DType == "Blurred")
				env3D = new BlurredEnvironment(base_window_name, render_window_name, m_EngineOptions, init_lua);
			else if (m_EngineOptions->m_Env3DType == "GrassField")
				env3D = new GrassFieldEnvironment(base_window_name, render_window_name, m_EngineOptions, init_lua);
			else if (m_EngineOptions->m_Env3DType == "Plane")
				env3D = new PlaneEnvironment(base_window_name, render_window_name, m_EngineOptions, init_lua);
			else if (m_EngineOptions->m_Env3DType == "Volume")
				env3D = new VolumeEnvironment(base_window_name, render_window_name, m_EngineOptions, init_lua);
			else if (m_EngineOptions->m_Env3DType == "Tessellation")
				env3D = new TessellationEnvironment(base_window_name, render_window_name, m_EngineOptions, init_lua);
			else if (m_EngineOptions->m_Env3DType == "Model")
				env3D = new ModelEnvironment(base_window_name, render_window_name, m_EngineOptions, init_lua);
			else if (m_EngineOptions->m_Env3DType == "Terrain")
				env3D = new TerrainEnvironment(base_window_name, render_window_name, m_EngineOptions, init_lua);
			//else if (env3DType == "Editor")
			//	env3D = new Editor3DEnvironment(base_window_name, render_window_name, m_EngineOptions, init_lua);
			
			//env3D->SetFullscreen( env3D->GetBaseWindow()->IsFullscreen() );
			
			if (m_EngineOptions->m_inEditor)
			{
				// Create a separate window for the tab panes
				m_ToolWindow3D = new ToolWindow3D(m_EngineOptions, env3D->GetD3DSystem(), env3D, env3D->GetBaseEditWindow());
				
				m_menuTool = new ToolWindowMenu(m_EngineOptions, env3D->GetD3DSystem(), 0, env3D, env3D->GetBaseEditWindow());
				m_hudTool = new ToolWindowHUD(m_EngineOptions, env3D->GetD3DSystem(), 0, env3D, env3D->GetBaseEditWindow());
				
				//env3D->SetMenuEditorSystem(m_ToolWindow3D->GetMenuEditorSystem());
				//env3D->SetHUDEditorSystem(m_ToolWindow3D->GetHUDEditorSystem());
			}
		}
		break;
	}
	
	return true;
}
//================================================================================================================
Environment::~Environment()
{
}
//================================================================================================================
int Environment::Run()
{
	int ret = 0;
	
	switch (m_EngineOptions->m_DimType)
	{
		case ZSHADE_2D:
		{
			ret = env2D->Run();
		}
		break;
		case ZSHADE_3D:
		{
			ret = env3D->Run();
		}
		break;
	}
	
	return ret;
}
//================================================================================================================