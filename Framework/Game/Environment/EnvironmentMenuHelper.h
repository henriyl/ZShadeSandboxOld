//================================================================================================================
// EnvironmentMenuHelper.h
//
//================================================================================================================
// History
//
// Created on 3/11/2015 by Dustin Watson
//================================================================================================================
#ifndef __ENVIRONMENTMENUHELPER_H
#define __ENVIRONMENTMENUHELPER_H
//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
//C/C++ Includes
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
#include "ZShadeINIParser.h"
#include "TextureManager.h"
#include "CreateGameDirectory.h"
#include "ZShadeMessageCenter.h"
#include "MemoryMgr.h"
#include "ShapeContact.h"
#include "PlatformerBackground.h"
#include "Timer.h"
#include "MenuSystem.h"
#include "HUDSystem.h"
using namespace std;

//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
class EnvironmentMenuHelper
{
public:
	EnvironmentMenuHelper();
	~EnvironmentMenuHelper();
	
	// Functions needed by LUA scripting engine and map editor as well as creating a menu
	
	static void AddMenuBackground(string activeMenuName, ZShadeSandboxGraphics::Image* image);
	static void AddMenuButton(string activeMenuName, ZShadeSandboxGraphics::Button* button);
	static void AddMenuText(string activeMenuName, ZShadeSandboxGraphics::Text* text);
	static void GrabMenuNames(vector<string>& names);
	static void LoadAMenu(string menu_filename, string& menuName);
	static void SaveMenu(string menu_filename);
	static void CreateMenu(string menu_name);
	static void SetActiveRenderedMenu(string menu_name);
	static void RemoveMenu(string menu_name);
};
//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
#endif//__ENVIRONMENTMENUHELPER_H