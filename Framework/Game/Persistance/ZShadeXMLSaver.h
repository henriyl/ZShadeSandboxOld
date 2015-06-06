//===============================================================================================================================
// ZShadeXMLSaver.h
//
//===============================================================================================================================
// History
//
// -Created 5/20/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __ZSHADEXMLSAVER_H
#define __ZSHADEXMLSAVER_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//

#include "ShaderMaterial.h"
#include "TopdownMap.h"
#include "PlatformerMap.h"
#include "World.h"
#include "PlatformerWorld.h"
#include "GameDirectory.h"
#include "MenuSystem.h"
#include "HUDSystem.h"
#include "tinyxml2.h"
#include <string>
using namespace std;
using namespace tinyxml2;

//===============================================================================================================================
//===============================================================================================================================
class ZShadeXMLSaver
{
public:
	
	static bool SaveMaterialXML(string filename, ZShadeSandboxLighting::ShaderMaterial* material);
	static bool SaveHUDXML(string filename, HUDSystem* hs);
	static bool SaveWorldXML(SWorld* sworld, string filename);
	static bool SaveWorldXML(SPlatformerWorld* sworld, string filename);
	static bool SaveSpritesXML(TopdownMap* map, string spriteMapPath, string spriteMapfilename, HWND hwndPB);
	static bool SaveSpritesXML(PlatformerMap* map, string spriteMapPath, string spriteMapfilename, HWND hwndPB);
	static bool SaveMapXML(TopdownMap* map, string filename, GameDirectory2D* gd, HWND hwndPB);
	static bool SaveMapXML(PlatformerMap* map, string filename, GameDirectory2D* gd, HWND hwndPB);
	static bool SaveMenuXML(string menufilename, MenuSystem* ms);
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__ZSHADEXMLSAVER_H