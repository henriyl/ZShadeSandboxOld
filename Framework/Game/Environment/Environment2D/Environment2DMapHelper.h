//================================================================================================================
// Environment2DMapHelper.h
//
//================================================================================================================
// History
//
// Created on 3/10/2015 by Dustin Watson
//================================================================================================================
#ifndef __ENVIRONMENT2DMAPHELPER_H
#define __ENVIRONMENT2DMAPHELPER_H
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
class TabEditWindow2D;
class MapEditorSystem2D;
class MenuEditorSystem;
using namespace std;

//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
class Environment2DMapHelper
{
public:
	Environment2DMapHelper();
	~Environment2DMapHelper();
	
	// Functions needed by LUA scripting engine and map editor as well as creating the game
	
	static int GetNumSpritesInMap();
	// Adds a map name to the world
	static void AddMapName(string world_name, string name);
	// Loads all the maps in the world
	static void LoadWorld(string world_name, bool InEditor, HWND hwndPB);
	// Loads a map in the world
	static void LoadMapInWorld(string world_name, string name, Map2DType mt, bool InEditor, HWND hwndPB);
	static void LoadMap(string& mapName, string activeWorldName, string filename, bool InEditor, HWND hwndPB);
	static AISprite* GetSpriteInMap(int i);
	static void AddSpriteInMap(AISprite* spr);
	static void RemoveSpriteInMap(AISprite* spr);
	static void LoadWorld(string& worldName, string filename, bool InEditor, HWND hwndPB);
	static void SetActiveRenderedMap(string world_name, string map_name);
	static void SetActiveMapName(string name);
	
	
	static void ToggleMapHardness(string activeMapName, string activeWorldName, bool bEnable);
	static void GetMapSizeAttribute(string activeMapName, string activeWorldName, int& size);
	static void GetMapWidthAttribute(string activeMapName, string activeWorldName, int& width);
	static void GetMapHeightAttribute(string activeMapName, string activeWorldName, int& height);
	static void GetMapVisionAttribute(string activeMapName, string activeWorldName, int& vision);
	static void GetMapMusicAttribute(string activeMapName, string activeWorldName, string& music);
	static void GetMapFOWAttribute(string activeMapName, string activeWorldName, bool& fow);
	static void GetMapFOWRadiusAttribute(string activeMapName, string activeWorldName, int& fowRadius);
	static void GetMapFOWFlashlightAttribute(string activeMapName, string activeWorldName, bool& fowFlashlight);
	static void GetNumSpritesRendered(string activeMapName, string activeWorldName, int& spritesRendered);
	static void GetSpriteTotal(string activeMapName, string activeWorldName, int& spriteTotal);
	static void GetWorldAndMapName(string& activeMapName, string& activeWorldName);
	
	// Adds a new world into the world list
	static void CreateWorld(string world_name);
	static void CreateWorld();
	// Creates a brand new map in the world
	static void NewMapTopdown(string world_name, string name, Map2DType mt, bool InEditor, HWND hwndPB);
	static void NewMapPlatformer(string world_name, string name, bool InEditor, HWND hwndPB);
	static void SaveMap(string activeMapName, string activeWorldName, string filename, HWND hwndPB);
	//static void RemoveMap(string activeMapName, string activeWorldName);
	// Creates a new default tile to the platformer map
	static void AddTile(string activeMapName, string activeWorldName, int x, int y);
	static void RemoveTile(string activeMapName, string activeWorldName, int x, int y);
	// Removes a tile from the platformer map
	static void DisableTileTexture(string activeMapName, string activeWorldName, int x, int y);
	static void RotateTile(string activeMapName, string activeWorldName, int x, int y);
	
	// Gets the name of all worlds and maps available
	//static void GrabWorldNames(vector<string>& names);
	//static void GrabMapNames(string world_name, vector<string>& names);

	static bool HasMaps(string worldName);

	static void UpdateTileTexture(string activeMapName, string activeWorldName, string textureName, int x, int y);
	static void NullTileAnimation(string activeMapName, string activeWorldName, int x, int y);
	static void UpdateTileAnimation(string activeMapName, string activeWorldName, string animProfileName, int animSpeed, int x, int y);
	static void UpdateTileHardness(string activeMapName, string activeWorldName, bool bHard, int x, int y);
	static void MakeTileHard(string activeMapName, string activeWorldName, int x, int y);
	static Tile* GetSelectedTopdownTile(string activeMapName, string activeWorldName, int x, int y);
	static PlatformerTile* GetSelectedPlatformerTile(string activeMapName, string activeWorldName, int x, int y);

	// Creates a new map
	static void CreateMapTopdown(int x, int y, bool fow, int fowRadius, bool fowFlashlight, string world_name, string name, bool bIso, bool InEditor, HWND hwndPB);
	static void CreateMapPlatformer(int x, int y, string world_name, string name, bool InEditor, HWND hwndPB);
	
	static void SaveWorld(string activeWorldName, string filename);

	static void PlayerMovingDir(bool& moved, int& dirUD, int& dirLR);
	static XMFLOAT3 MatchingTilePos(float x, float y);
	static void ClipSpriteToMap(Sprite* spr, float speed, float& newX, float& newY, float& newHBX, float& newHBY);

	// Selects a rendering section from the section picker and returns the position of the first tile in the section
	static XMFLOAT2 SelectRenderingSection(int section);

	static XMFLOAT3 GetPlayerPosition();
	
	static void SetActivePlatformerWorld();
	
	//void SetActiveWorldName(string name) { m_ActiveWorldName = name; }

	//string GetActiveWorldName() { return m_ActiveWorldName; }
	//string GetActiveMapName() { return m_ActiveMapName; }

	//void SetWorldLoaded(bool loaded) { m_WorldLoaded = loaded; }
	//void SetMapLoaded(bool loaded) { m_MapLoaded = loaded; }

	//void SetInSectionPicker(bool inPicker) { m_InSectionPicker = inPicker; }

	//void SetMapEditorSystem(MapEditorSystem2D* mapEditorSystem) { m_MapEditorSystem2D = mapEditorSystem; }
};
//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
#endif//__ENVIRONMENT2DMAPHELPER_H