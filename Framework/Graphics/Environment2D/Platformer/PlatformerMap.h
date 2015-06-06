//==================================================================================================================================
// PlatformerMap.h
//
//==================================================================================================================================
// History
//
// -Created on 1/15/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __PLATFORMERMAP_H
#define __PLATFORMERMAP_H
//==================================================================================================================================
//==================================================================================================================================
#include "D3D.h"
#include "ZMath.h"
#include "MemoryMgr.h"
#include "PlatformerTile.h"
#include "AISprite.h"
#include "Input.h"
#include "ZShadeMap.h"
//==================================================================================================================================
#ifndef MAXSPRITES
#define MAXSPRITES
#define MAX_SPRITES 5000
#endif//MAXSPRITES
//==================================================================================================================================
//==================================================================================================================================
struct XMFLOAT2EXT
{
	float x;
	float y;

	XMFLOAT2EXT(float x, float y) : x(x), y(y) {}

	bool operator == (const XMFLOAT2EXT o)
	{
		return x == o.x && y == o.y;
	}

	//friend bool operator < (const XMFLOAT2EXT o1, const XMFLOAT2EXT o2)
	//{
	//	return o1.x < o2.x && o1.y < o2.y;
	//}
};
//==================================================================================================================================
struct PlatformerMapLoadData
{
	vector<string> imageNames;
	vector<string> animProfileNames;
	vector<int> animSpeeds;
	vector<int> hards;
	vector<float> xs;
	vector<float> ys;
	ZShadeMap<XMFLOAT2EXT, int> exists;
};
//==================================================================================================================================
//==================================================================================================================================
class PlatformerMap
{
	D3D* m_D3DSystem;
	EngineOptions* m_EngineOptions;
	Camera* m_camera;
	
	SmartArray2D<PlatformerTile*> m_Tiles;
	//SmartArray2D<PlatformerTile*> m_TempTiles;
	vector< AISprite* > m_Sprites;
	vector<string> m_SubMapNames;
	
	vector< AISprite* > m_ClippedSprites;
	
	SleepTimer timer;
	
	//float scrollX;
	//float scrollY;
	
	// The amount of screens the background takes up
	// IMPLEMENT THIS
	int m_BackgroundScreenWidth;
	
	// Create a single world transform for scrolling that gets applied to all objects
	XMMATRIX mMapWorld;
	
	string m_MapName;
	string m_MusicName;
	int m_MapWidth;
	int m_MapHeight;
	int m_numSpritesRendered;
	int spriteNum;
	XMFLOAT2 m_Offset;
	XMFLOAT2 kWorldHalfExtents;

	int m_TileRemovedX;
	int m_TileRemovedY;

	// Every in the map moves around this item in editor
	//Sprite* m_MapSprite;
	bool bEditor;

	PlatformerTile* startTile;
	PlatformerTile* endTile;

	struct BB
	{
		int firstTileX;
		int firstTileY;
		int lastTileX;
		int lastTileY;
	};

	BB m_BoundingBox;

public:
	
	PlatformerMap(D3D* d3d);
	
	void Initialize(GameDirectory2D* gd, HWND hwndPB);
	void InitializeTiles(PlatformerMapLoadData pmld, GameDirectory2D* gd, HWND hwndPB);
	void AddTile
	(
		int x,
		int y,
		int size,
		bool hard,
		bool init,
		GameDirectory2D* gd,
		string textureFilename,
		string animProfileName,
		int animSpeed,
		int bitmapWidth,
		int bitmapHeight
	);
	
	void AddSubMapName(string name) { m_SubMapNames.push_back(name); }
	string SubMapName(int i) const { return m_SubMapNames[i]; }
	
	SmartArray2D<PlatformerTile*> Tiles() const { return m_Tiles; }
	
	int TotalClippedSprites() { return m_ClippedSprites.size(); }
	AISprite* ClippedSprite(int i) { return m_ClippedSprites[i]; }
	
	bool& InEditor() { return bEditor; }
	bool InEditor() const { return bEditor; }

	int& TileRemovedX() { return m_TileRemovedX; }
	int TileRemovedX() const { return m_TileRemovedX; }

	int& TileRemovedY() { return m_TileRemovedY; }
	int TileRemovedY() const { return m_TileRemovedY; }

	XMFLOAT2& Offset() { return m_Offset; }
	XMFLOAT2 Offset() const { return m_Offset; }
	
	string& MusicName() { return m_MusicName; }
	string MusicName() const { return m_MusicName; }

	string& MapName() { return m_MapName; }
	string MapName() const { return m_MapName; }
	
	int& BackgroundScreenWidth() { return m_BackgroundScreenWidth; }
	int BackgroundScreenWidth() const { return m_BackgroundScreenWidth; }
	
	int& Width() { return m_MapWidth; }
	int& Height() { return m_MapHeight; }
	int& NumSpritesRendered() { return m_numSpritesRendered; }
	int& NumSprites() { return spriteNum; }
	
	int Width() const { return m_MapWidth; }
	int Height() const { return m_MapHeight; }
	int NumSpritesRendered() const { return m_numSpritesRendered; }
	int NumSprites() const { return spriteNum; }
	
	void AddTile(GameDirectory2D* gd, int x, int y);
	void RemoveTile(int x, int y);

	//Get a tile at the specified m_TopLeftPosition
	PlatformerTile* SelectTile(int x, int y);

	PlatformerTile* GetTile(int x, int y);
	PlatformerTile* GetTileSafe(int x, int y);

	//Adds a sprite or player to the map
	void AddSprite(AISprite* s);
	void AddSpriteNoIndex(AISprite* s);
	void AddPlayer(AISprite* s);
	void AddPlayerNoIndex(AISprite* s);
	
	void RemoveSprite(AISprite* s);
	
	void ClipSprites();
	
	void AddSpriteAnimations(string profileName, string base_game_folder, string base_sprite_path, int id);
	void AddTileAnimations(string profileName, string base_game_folder, string base_tile_path, float tile_x, float tile_y);
	
	//void InitAISprite(GameDirectory2D* gd);
	void LoadSpritesMain();

	AISprite* GetSpriteByID(int id);
	AISprite* getSprite(int i) { return m_Sprites[i]; }
	AISprite* GetPlayer();
	
	bool InBounds(float x, float y);
	bool InScreen(float x, float y);
	XMFLOAT2 PlayerOffset();
	void PlayerMovingDir(bool& moved, int& dirUD, int& dirLR);
	XMFLOAT3 MatchingTilePos(float x, float y);
	void ClipSpriteToMap(Sprite* spr, float speed, float& newX, float& newY, float& newHBX, float& newHBY);
	void UpdateTileTexture(int x, int y, string base_tile_path, string textureFilename, int bitmapWidth, int bitmapHeight);
	void UpdateTileHardness(int x, int y, bool bHard);
	void MakeTileHard(int x, int y);
	void MoveTiles(float speed, int dirUD, int dirLR);
	void DisableTexture(int x, int y);

	void RotateTile(int x, int y);

	// Toggle the hardness of the tiles and sprites
	void ToggleHardness(bool bEnable);

	void TransformMap(HWND hwnd, float dt, Keyboard keyboard);
	
	bool Render(Camera* camera);
	
	void Clone(PlatformerMap* c) {}
	
public:
	
	/*
	float TileCoordsToWorldX(int i)
	{
		return i * m_EngineOptions->TILE_SIZE - kWorldHalfExtents.x;
	}

	float TileCoordsToWorldY(int j)
	{
		return j * m_EngineOptions->TILE_SIZE - kWorldHalfExtents.y;
	}

	int WorldCoordToTileX(float worldX)
	{
		return (worldX + kWorldHalfExtents.x) / m_EngineOptions->TILE_SIZE;
	}

	int WorldCoordToTileY(float worldY)
	{
		return (worldY + kWorldHalfExtents.y) / m_EngineOptions->TILE_SIZE;
	}

	PlatformerTile* GetTileFromPos(float x, float y)
	{
		int i = WorldCoordToTileX(x);
		int j = WorldCoordToTileY(y);

		return GetTile(i, j);
	}*/

	const static int TILESIZEBITS = 6;
	
	static int PixelsToTiles(float pixels)
	{
		return PixelsToTiles((int)ZShadeSandboxMath::ZMath::DoRound(pixels));
	}
	
	static int PixelsToTiles(int pixels)
	{
		return pixels >> TILESIZEBITS;
	}
	
	static int TilesToPixels(int numTiles)
	{
		return numTiles << TILESIZEBITS;
	}
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__PLATFORMERMAP_H