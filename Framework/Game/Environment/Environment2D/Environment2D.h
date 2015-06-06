//================================================================================================================
// Environment2D.h
//
//================================================================================================================
// History
//
// Created on 10/3/2014 by Dustin Watson
//================================================================================================================
#ifndef __ENVIRONMENT2D_H
#define __ENVIRONMENT2D_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//

#include <memory>
#include <sstream>
#include <Windows.h>
#include <windowsx.h>
#include <string>
#include <map>
#include <vector>
#include "D3D.h"
#include "Input.h"
#include "Camera.h"
#include "TopdownMap.h"
#include "FogData.h"
#include "GameState.h"
#include "GameDirectory.h"
#include "SpriteBatch.h"
#include "ZShadeINIParser.h"
#include "Window.h"
#include "TextureManager.h"
#include "CreateGameDirectory.h"
#include "ZShadeMessageCenter.h"
#include "MemoryMgr.h"
#include "ShapeContact.h"
#include "WinToolbar.h"
#include "ZShadeResource.h"
#include "PlatformerBackground.h"
#include "Timer.h"
#include "MenuSystem.h"
#include "HUDSystem.h"
#include "EnvironmentBase.h"
#include "World.h"
#include "PlatformerWorld.h"
//class TabEditWindow2D;
class MapEditorSystem2D;
class MenuEditorSystem;
class HUDEditorSystem;
class ToolWindow2D;
class ToolWindowMenu;
class ToolWindowHUD;
using namespace std;
using namespace GUIComponents;

//===============================================================================================================================
//===============================================================================================================================
class Environment2D : public EnvironmentBase
{
public:
	
	Environment2D(LPCSTR base_window_name, LPCSTR render_window_name, EngineOptions* engineOptions, bool init_lua);
	~Environment2D();
	
	bool Init(LPCSTR base_window_name, LPCSTR render_window_name, bool init_lua);
	void Shutdown();
	int Run();
	
	// Get the specified world
	World* GetTopdownWorld(string world_name);
	PlatformerWorld* GetPlatformerWorld(string world_name);
	SWorld* GetSWorld(string world_name);
	SPlatformerWorld* GetSPlatformerWorld(string world_name);
	// Take a map off the world
	void RemoveMap(string activeMapName, string activeWorldName);
	// Gets the name of all worlds and maps available
	void GrabWorldNames(vector<string>& names);
	void GrabMapNames(string world_name, vector<string>& names);
	void LoadWorld(string world_name, SWorld* sw);
	void LoadWorld(string world_name, SPlatformerWorld* sw);
	
	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);
	
	MapEditorSystem2D* GetMapEditorSystem2D() { return m_MapEditorSystem2D; }
	MenuEditorSystem* GetMenuEditorSystem() { return m_MenuEditorSystem; }
	HUDEditorSystem* GetHUDEditorSystem() { return m_HUDEditorSystem; }
	ToolWindow2D* GetToolEditWindow2D() { return m_ToolWindow2D; }
	ToolWindowMenu* GetToolWindowMenu() { return m_menuTool; }
	ToolWindowHUD* GetToolWindowHUD() { return m_hudTool; }
	
	void SetMapEditorSystem(MapEditorSystem2D* mapEditorSystem) { m_MapEditorSystem2D = mapEditorSystem; }
	void SetMenuEditorSystem(MenuEditorSystem* menuEditorSystem) { m_MenuEditorSystem = menuEditorSystem; }
	void SetHUDEditorSystem(HUDEditorSystem* hudEditorSystem) { m_HUDEditorSystem = hudEditorSystem; }
	void SetToolWindow2D(ToolWindow2D* toolEditWindow2D) { m_ToolWindow2D = toolEditWindow2D; }
	void SetToolMenu(ToolWindowMenu* toolMenu) { m_menuTool = toolMenu; }
	void SetToolHUD(ToolWindowHUD* toolHUD) { m_hudTool = toolHUD; }
	
	map<string, SWorld*> WorldsTopdown() { return m_WorldsTopdown; }
	map<string, SPlatformerWorld*> WorldsPlatformer() { return m_WorldsPlatformer; }
	
	string& ActiveWorldName() { return m_ActiveWorldName; }
	string ActiveWorldName() const { return m_ActiveWorldName; }
	
	string& ActiveMapName() { return m_ActiveMapName; }
	string ActiveMapName() const { return m_ActiveMapName; }
	
	bool& WorldLoaded() { return m_WorldLoaded; }
	bool WorldLoaded() const { return m_WorldLoaded; }
	
	bool& MapLoaded() { return m_MapLoaded; }
	bool MapLoaded() const { return m_MapLoaded; }
	
	bool& InSectionPicker() { return m_InSectionPicker; }
	bool InSectionPicker() const { return m_InSectionPicker; }
	
	//Engine stuff
protected:
	
	//TabEditWindow2D* m_TabEditWindow2D;
	MapEditorSystem2D* m_MapEditorSystem2D;
	MenuEditorSystem* m_MenuEditorSystem;
	HUDEditorSystem* m_HUDEditorSystem;
	
	ToolWindow2D* m_ToolWindow2D;
	ToolWindowMenu* m_menuTool;
	ToolWindowHUD* m_hudTool;
	
	bool Update();
	bool RenderMaster();
	
	//Game related stuff
protected:
	
	// If the empty constructor is called then we need to call InitBase from Init
	bool m_EnvBaseInit;
	
	bool bInEditorMap;
	bool bInEditorMenu;
	bool bInEditorHUD;
	
	string m_ActiveWorldName;
	string m_ActiveMapName;
	bool m_WorldLoaded;
	bool m_MapLoaded;
	bool m_InSectionPicker;
	PlatformerBackground* m_Background;
	
	World* m_ActiveTopdownWorld;
	TopdownMap* m_ActiveTopdownMap;

	PlatformerWorld* m_ActivePlatformerWorld;
	PlatformerMap* m_ActivePlatformerMap;

	map<string, SWorld*> m_WorldsTopdown;
	map<string, SPlatformerWorld*> m_WorldsPlatformer;
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__ENVIRONMENT2D_H