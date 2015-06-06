//================================================================================================================
// MapEditorSystem2D.h
//
// Includes everything needed for the map editor
//================================================================================================================
// History
//
// Created on 10/9/2014 by Dustin Watson
//================================================================================================================
#ifndef __MAPEDITORSYSTEM2D_H
#define __MAPEDITORSYSTEM2D_H
//================================================================================================================
//================================================================================================================
//#include "EditorEnvironment.h"
#include "Sprite.h"
#include "TopdownMap.h"
#include "PlatformerMap.h"
//#include "OpenFileDialog.h"
//#include "SaveFileDialog.h"
#include "Environment2D.h"
//#include "MemoryMgr.h"
//================================================================================================================
class MapEditorSystem2D
{
	typedef enum
	{
		EM_Tiles = 0,
		EM_Sprites = 1,
		EM_Music = 2,
		EM_Hardness = 3
	} EEditMode;
	
	typedef enum
	{
		TA_Regular = 0,
		TA_MultiStamp = 1,
		TA_AddTile = 2,
		TA_RemoveTile = 3,
		TA_RemoveTexture = 4,
		TA_Rotate = 5
	} ETileActions;
	
	typedef enum
	{
		SA_None = 0,
		SA_Move = 1,
		SA_Delete = 2,
		SA_Place = 3,
		SA_EditSize = 4
	} ESpriteActions;
	
	typedef enum
	{
		HA_Tiles = 0,
		HA_Sprites = 1
	} EHardnessActions;
	
	typedef enum
	{
		GM_Free = 0,
		GM_Snap = 1
	} EGridMode;
	
	typedef enum
	{
		MSS_1X1 = 0,
		MSS_2X2 = 1,
		MSS_3X3 = 2,
		MSS_4X4 = 3,
		MSS_5X5 = 4,
		MSS_6X6 = 5,
		MSS_7X7 = 6,
		MSS_8X8 = 7,
		MSS_9X9 = 8,
		MSS_10X10 = 9,
	} EMultiStampSize;
	
public:

	MapEditorSystem2D(Environment2D* env2D);
	~MapEditorSystem2D();

	void Init();

public:
	
	XMFLOAT2 SnapToGrid(int x, int y);
	
	void CreateWorld(string world_name);
	void LoadWorld(string& worldName, string filename, bool InEditor, HWND hwndPB);
	void SaveWorld(string activeWorldName, string filename);
	void RemoveMap(string activeMapName, string activeWorldName);

	void NewMapTopdown(string world_name, string name, int x, int y, bool fow, int fowRadius, bool fowFlashlight, bool bIso, bool InEditor, HWND hwndPB);
	void NewMapPlatformer(string world_name, string name, int x, int y, bool InEditor, HWND hwndPB);
	void LoadMap(string& mapName, string activeWorldName, string filename, bool InEditor, HWND hwndPB);
	void SaveMap(string activeMapName, string activeWorldName, string filename, HWND hwndPB);

	void GrabWorldNames(vector<string>& names);
	void GrabMapNames(string world_name, vector<string>& names);

	void SetActiveRenderedMap(string world_name, string map_name);

	void LoadRequiredSprites();
	
	void RevertTileAnim(string activeMapName, string activeWorldName);
	void UpdateTileAnimation(string activeMapName, string activeWorldName, string animProfileName, int animSpeed);
	void UpdateTileHardness(string activeMapName, string activeWorldName, bool bHard);
	Tile* GetSelectedTopdownTile(string activeMapName, string activeWorldName);
	PlatformerTile* GetSelectedPlatformerTile(string activeMapName, string activeWorldName);

	void MakeTileHard(string activeMapName, string activeWorldName);

	void RotateTile(string activeMapName, string activeWorldName);

	// Creates a new default tile to the platformer map
	void AddTile(string activeMapName, string activeWorldName);
	void RemoveTile(string activeMapName, string activeWorldName);
	// Removes a tile from the platformer map
	void RemoveTexture(string activeMapName, string activeWorldName);
	
	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);
	
	// Updates the stamp sprites when the mouse is moved
	void UpdateStampSprites();
	void RenderStampSprites(Camera* camera);

	// Update the text in the title bar of the base window
	void UpdateTitleBar();

	// Updates the map from the map properties window
	void UpdateMapProperties();

	// Opens the sprite properties window for a clicked on sprite
	void UpdateSpriteProperties();

	// Highlights a sprite if the mouse if over it
	void HighlightSprite();

	// Moves a sprite when the user is in sprite move mode and clicks on a sprite
	void UpdateSpriteMove();

	// Deletes a sprite from the map when the user is in sprite delete mode and clicks on a sprite
	void UpdateSpriteDelete();

	// Changes the size of a sprite's size and position based on the pressing of keys
	void UpdateSpriteSize();

	// A hotkey to enter sprite mode none
	void SpriteModeNoneHotkey();

	// Opens the tile properties window for a clicked on tile
	void UpdateTileProperties();

	// Change the image of the tile selected
	void UpdateTileStamp(string activeMapName, string activeWorldName, string tileCacheImage);

	void SwitchToSpriteModeMove();
	void SwitchToSpriteModeDelete();
	void SwitchToSpriteModePlace();
	void SwitchToSpriteModeSize();
	void SwitchToSpriteModeNone();
	
	void SwitchToTileModeRegular();
	void SwitchToTileModeMultiStamp();
	void SwitchToTileModeAddTile();
	void SwitchToTileModeRemoveTile();
	void SwitchToTileModeRemoveTexture();
	void SwitchToTileModeRotateTile();
	
	void SwitchToHardnessModeTiles();
	void SwitchToHardnessModeSprites();
	
	void SwitchToGridModeFree();
	void SwitchToGridModeSnap();
	
	void SwitchToStampSize1x1();
	void SwitchToStampSize2x2();
	void SwitchToStampSize3x3();
	void SwitchToStampSize4x4();
	void SwitchToStampSize5x5();
	void SwitchToStampSize6x6();
	void SwitchToStampSize7x7();
	void SwitchToStampSize8x8();
	void SwitchToStampSize9x9();
	void SwitchToStampSize10x10();

	bool& IsInSectionPicker() { return m_InSectionPicker; }
	bool IsInSectionPicker() const { return m_InSectionPicker; }

	bool IsMapCreated() { return m_MapCreated; }
	bool IsWorldLoaded();

	void SetMapCreated(bool created) { m_MapCreated = created; }
	void SetGameDirectory(GameDirectory2D* gd);
	void GoFullscreen(bool bEnable);
	void ToggleMapHardness(string activeMapName, string activeWorldName, bool bEnable);

	void GetWorldAndMapName(string& activeMapName, string& activeWorldName);
	void GetMapWidthAttribute(string activeMapName, string activeWorldName, int& width);
	void GetMapHeightAttribute(string activeMapName, string activeWorldName, int& height);
	void GetMapSizeAttribute(string activeMapName, string activeWorldName, int& size);
	void GetMapVisionAttribute(string activeMapName, string activeWorldName, int& vision);
	void GetMapMusicAttribute(string activeMapName, string activeWorldName, string& music);
	void GetMapFOWAttribute(string activeMapName, string activeWorldName, bool& fow);
	void GetMapFOWRadiusAttribute(string activeMapName, string activeWorldName, int& fowRadius);
	void GetMapFOWFlashlightAttribute(string activeMapName, string activeWorldName, bool& fowFlashlight);
	void GetNumSpritesRendered(string activeMapName, string activeWorldName, int& spritesRendered);
	void GetSpriteTotal(string activeMapName, string activeWorldName, int& spriteTotal);

	//bool Update();
	//bool Render(XMMATRIX worldMatrix, XMMATRIX viewMatrix, XMMATRIX projectionMatrix, XMMATRIX orthoMatrix);
	
	bool HasMaps(string worldName);

	GameDirectory2D* GetGD() { return m_mainGameDirectory; }
	
	D3D* D3DSystem() { return m_D3DSystem; }

	XMFLOAT2 GetScrollOffset() const { return ScrollOffset; }

	void UpdateInput(Keyboard* keyboard);

	string& ActiveWorldName() { return m_ActiveWorldName; }
	string ActiveWorldName() const { return m_ActiveWorldName; }
	
	string& ActiveMapName() { return m_ActiveMapName; }
	string ActiveMapName() const { return m_ActiveMapName; }

	string& SelectedTileName() { return m_SelectedTileName; }
	string SelectedTileName() const { return m_SelectedTileName; }

	string& SelectedSpriteName() { return m_SelectedSpriteName; }
	string SelectedSpriteName() const { return m_SelectedSpriteName; }

private:
	
	Environment2D* m_Env2D;
	
	D3D* m_D3DSystem;
	EngineOptions* m_EngineOptions;

	GameDirectory2D* m_mainGameDirectory;

	string m_ActiveWorldName;
	string m_ActiveMapName;
	string m_SelectedSpriteName;
	string m_SelectedTileName;
	
	//Controls how fast the camera will move when transforming
	float m_camera_speed;
	
	//When the user makes a change on the map then this flag becomes true
	bool m_Updated;
	
	XMFLOAT2 m_SelectedTilePos;

	// This is the main stamp tool to select a tile
	Sprite* m_StampNormal;
	// The stamp tool that shows when the left mouse button is clicked
	Sprite* m_StampHighlight;

	// This is the main stamp tool to select a tile
	Sprite* m_StampNormal32x32;
	// The stamp tool that shows when the left mouse button is clicked
	Sprite* m_StampHighlight32x32;

	//Preview sprite of the selected sprite
	Sprite* m_SelectedSprite;
	//Preview sprite of the selected tile
	Sprite* m_SelectedTile;
	
	//Stamp Tool sizes of how many tiles it can select
	Sprite* m_TileEdit1x1Sprite;
	Sprite* m_TileEdit2x2Sprite;
	Sprite* m_TileEdit3x3Sprite;
	Sprite* m_TileEdit4x4Sprite;
	Sprite* m_TileEdit5x5Sprite;
	Sprite* m_TileEdit6x6Sprite;
	Sprite* m_TileEdit7x7Sprite;
	Sprite* m_TileEdit8x8Sprite;
	Sprite* m_TileEdit9x9Sprite;
	Sprite* m_TileEdit10x10Sprite;
	
	Sprite* m_TileEdit1x1Sprite2;
	Sprite* m_TileEdit2x2Sprite2;
	Sprite* m_TileEdit3x3Sprite2;
	Sprite* m_TileEdit4x4Sprite2;
	Sprite* m_TileEdit5x5Sprite2;
	Sprite* m_TileEdit6x6Sprite2;
	Sprite* m_TileEdit7x7Sprite2;
	Sprite* m_TileEdit8x8Sprite2;
	Sprite* m_TileEdit9x9Sprite2;
	Sprite* m_TileEdit10x10Sprite2;

	Sprite* m_displaySpriteCover;
	Sprite* m_sectionSprite;

	int m_currentSection;
	
	// Mouse position at start of drag operation
	int dragStartX, dragStartY;

	// Mouse position in previous frame of drag operation
	int dragPrevX, dragPrevY;

	// Currently panning with RMB
	bool panning;
	
	// X and Y grid square size
	int gridSizeX, gridSizeY;
	
	// Edge distance of window client area at which to auto-scroll
	int edgeScrollW, edgeScrollH;

	// Amount to auto-scroll at window edge
	int edgeScrollX, edgeScrollY;
	
	// If in the editor do not want to get the offset from the player in the map
	
	// Needs to be set in an update loop with the map
	XMFLOAT2 ScrollOffset;
	
	EEditMode editMode;
	ETileActions tileAction;
	ESpriteActions spriteAction;
	EHardnessActions hardnessAction;
	EGridMode gridMode;
	EMultiStampSize multiStampSize;
	
	bool m_leftButtonDown;

	bool m_InSectionPicker;
	
	//Are we rendering the overview tileset of the map or in a section of the map
	bool m_OverworldTiles;
	
	//Is the map created
	bool m_MapCreated;
	
	// Do we have a world
	bool m_WorldLoaded;
	
	bool stampPressed;
	bool stampPressed2;
	
	bool usingGrid;
	
	bool m_startDelayTimer;
	
	bool m_platformer;
	
	SleepTimer* m_ClickDelayTimer;
	SleepTimer* m_SelectionTimer;

	//OpenFileDialog* m_MapOpenFileDialog;
	//SaveFileDialog* m_MapSaveFileDialog;
};
//================================================================================================================
#endif//__MAPEDITORSYSTEM2D_H