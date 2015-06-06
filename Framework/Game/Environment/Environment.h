//================================================================================================================
// Environment.h
//
// This will be the entry point for the game (Non-Editor)
//================================================================================================================
// History
//
// Created on 2/24/2015 by Dustin Watson
//================================================================================================================
#ifndef __ENVIRONMENT_H
#define __ENVIRONMENT_H
//================================================================================================================
//================================================================================================================
#include <memory>
#include <Windows.h>
#include <string>
#include <windowsx.h>
#include <CommCtrl.h>
#include "Environment2D.h"
#include "Environment3D.h"
#include "OutdoorEnvironment.h"
#include "BlurredEnvironment.h"
#include "CubeReflectionOutdoorEnvironment.h"
#include "GrassFieldEnvironment.h"
#include "OceanEnvironment.h"
#include "PlaneEnvironment.h"
#include "VolumeEnvironment.h"
#include "TessellationEnvironment.h"
#include "ModelEnvironment.h"
#include "TerrainEnvironment.h"
#include "Window.h"
class MapEditorSystem2D;
class MenuEditorSystem;
class ToolWindow2D;
class ToolWindow3D;
class ToolWindowMenu;
class ToolWindowHUD;
//class TabEditWindow2D;
//class TabEditWindow3D;
//================================================================================================================
class Environment
{
public:
	
	Environment(EngineOptions* engineOptions, LPCSTR base_window_name, LPCSTR render_window_name, bool init_lua);
	~Environment();
	
public:
	
	bool Init(LPCSTR base_window_name, LPCSTR render_window_name, bool init_lua);
	
	int Run();
	
	Environment2D* Env2D() { return env2D; }
	Environment3D* Env3D() { return env3D; }

private:
	
	EngineOptions* m_EngineOptions;
	
	// These are used only by the editor
	MapEditorSystem2D* m_MapEditorSystem2D;
	MenuEditorSystem* m_MenuEditorSystem;
	HUDEditorSystem* m_HUDEditorSystem;
	ToolWindow2D* m_ToolWindow2D;
	ToolWindow3D* m_ToolWindow3D;
	ToolWindowMenu* m_menuTool;
	ToolWindowHUD* m_hudTool;
	
	Environment2D* env2D;
	Environment3D* env3D;
};
//================================================================================================================
#endif//__ENVIRONMENT_H