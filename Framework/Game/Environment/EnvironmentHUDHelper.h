//================================================================================================================
// EnvironmentHUDHelper.h
//
//================================================================================================================
// History
//
// Created on 3/11/2015 by Dustin Watson
//================================================================================================================
#ifndef __ENVIRONMENTHUDHELPER_H
#define __ENVIRONMENTHUDHELPER_H
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
class EnvironmentHUDHelper
{
public:
	EnvironmentHUDHelper();
	~EnvironmentHUDHelper();
	
	// Functions needed by LUA scripting engine and map editor as well as creating a menu
	
	static void AddHUDImage(ZShadeSandboxGraphics::Image* image);
	static void AddHUDText(ZShadeSandboxGraphics::Text* text);
	//static void GrabHUDNames(vector<string>& names);
	static void LoadHUD(string hud_filename, string& hud_name);
	static void SaveHUD(string hud_filename);
	static void CreateHUD(string hud_name);
};
//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
#endif//__ENVIRONMENTHUDHELPER_H