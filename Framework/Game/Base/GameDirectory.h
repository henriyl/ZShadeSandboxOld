//===============================================================================================================================
// GameDirectory.h
//===============================================================================================================================
#ifndef __GAMEDIRECTORY_H
#define __GAMEDIRECTORY_H
//===============================================================================================================================
//===============================================================================================================================
//Includes
#include <string>
#include "MemoryMgr.h"
using namespace std;
//===============================================================================================================================
//===============================================================================================================================
/* 2D Game Directory

	GAME_FOLDER\\RequiredTextures\\
	GAME_FOLDER\\Sprites\\
	GAME_FOLDER\\MenuSprites\\
	GAME_FOLDER\\HUD\\
	GAME_FOLDER\\Tiles\\
	GAME_FOLDER\\Scripts\\
	GAME_FOLDER\\Music\\
	GAME_FOLDER\\AnimationProfiles\\Sprite\\
	GAME_FOLDER\\AnimationProfiles\\Tile\\
	GAME_FOLDER\\XML\\Maps\\
	GAME_FOLDER\\XML\\Sprites\\
	GAME_FOLDER\\XML\\Menu\\
	GAME_FOLDER\\XML\\HUD\\
	GAME_FOLDER\\XML\\Worlds\\
	GAME_FOLDER\\Game.ini
	GAME_FOLDER\\SpriteCacheNames.txt
	GAME_FOLDER\\TileCacheNames.txt
	GAME_FOLDER\\MenuAssetsNames.txt
*/
struct GameDirectory2D
{
	string GAME_FOLDER;
	string m_required_textures;
	string m_menu_sprites_path;
	string m_hud_path;
	string m_sprites_path;
	string m_tiles_path;
	string m_scripts_path;
	string m_music_path;
	string m_animation_profiles_path;
	string m_animation_profile_sprites_path;
	string m_animation_profile_tiles_path;
	string m_xml_path;
	string m_xml_maps_path;
	string m_xml_sprites_path;
	string m_xml_menu_path;
	string m_xml_hud_path;
	string m_xml_worlds_path;
	string m_game_ini_path;
};
//===============================================================================================================================
//===============================================================================================================================
/* 3D Game Directory

	GAME_FOLDER\\RequiredTextures\\
	GAME_FOLDER\\MenuSprites\\
	GAME_FOLDER\\HUD\\
	GAME_FOLDER\\Scripts\\
	GAME_FOLDER\\Music\\
	GAME_FOLDER\\Model\\
	GAME_FOLDER\\Textures\\
	GAME_FOLDER\\XML\\HUD\\
	GAME_FOLDER\\XML\\Menu\\
	GAME_FOLDER\\XML\\Maps\\
	GAME_FOLDER\\Game.ini
*/
struct GameDirectory3D
{
	string GAME_FOLDER;
	string m_required_textures;
	string m_menu_sprites_path;
	string m_hud_path;
	string m_scripts_path;
	string m_music_path;
	string m_materials_path;
	string m_models_path;
	string m_textures_path;
	string m_xml_path;
	string m_xml_menu_path;
	string m_xml_hud_path;
	string m_xml_maps_path;
	string m_game_ini_path;
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__GAMEDIRECTORY_H