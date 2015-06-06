//===============================================================================================================================
// ZShadeXMLLoader.h
//
//===============================================================================================================================
// History
//
// -Created 5/20/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __ZSHADEXMLLOADER_H
#define __ZSHADEXMLLOADER_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//

#include "HUDSystem.h"
#include "Sprite.h"
#include "MenuSystem.h"
#include "TopdownMap.h"
#include "PlatformerMap.h"
#include "World.h"
#include "PlatformerWorld.h"
#include "tinyxml2.h"
#include <string>
using namespace std;
using namespace tinyxml2;

//===============================================================================================================================
//===============================================================================================================================

class ZShadeXMLLoader
{
public:
	
	static bool LoadMaterialXML(string filename, D3D* d3d);
	static bool LoadWorldXML(string worldname, SWorld*& sworld);
	static bool LoadWorldXML(string worldname, SPlatformerWorld*& sworld);
	static bool LoadMapXML(string& mapname, string filename, GameDirectory2D* gd, Camera* cam, D3D* d3d, TopdownMap*& map, bool inEditor, HWND hwndPB);
	static bool LoadMapXML(string& mapname, string filename, GameDirectory2D* gd, Camera* cam, D3D* d3d, PlatformerMap*& map, bool inEditor, HWND hwndPB);
	// Not ready for 3D map yet
	static bool LoadMap3DXML() { return true; }
	static bool LoadMenuXML(string menufilename, GameDirectory2D* gd, EngineOptions* eo, D3D* d3d, HWND hwnd, MenuSystem*& ms);
	static bool LoadSpritesXML(string spriteMapfilename, GameDirectory2D* gd, Camera* cam, D3D* d3d, TopdownMap*& map);
	static bool LoadSpritesXML(string spriteMapfilename, GameDirectory2D* gd, Camera* cam, D3D* d3d, PlatformerMap*& map);
	static bool LoadHUDXML(string filename, GameDirectory2D* gd, EngineOptions* eo, D3D* d3d, HWND hwnd, HUDSystem*& hs);
};

//===============================================================================================================================
//===============================================================================================================================
#endif//__ZSHADEXMLLOADER_H