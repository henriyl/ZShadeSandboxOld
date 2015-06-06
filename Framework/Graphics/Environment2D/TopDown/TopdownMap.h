//======================================================================================================
// TopdownMap.h
//
// Loads the map for 2D game
//======================================================================================================
// History
//
// Created in August 2013 by Dustin Watson
// Updated 9/15/2014 by Dustin Watson
// Updated 1/28/2015 by Dustin Watson changed name to TopdownMap
//======================================================================================================
#ifndef __TOPDOWNMAP_H
#define __TOPDOWNMAP_H
//=================================================================================================================
//=================================================================================================================
//Includes
#include <algorithm>
#include "tinyxml2.h"
#include "TileSystem.h"
#include "SleepTimer.h"
#include "Input.h"
#include "Sprite.h"
#include "Camera.h"
#include "SAT.h"
#include "AISprite.h"
#include "GameDirectory.h"
//=================================================================================================================
//=================================================================================================================
//Defines
//Turn this on to test out how the neighbors of the sections look
//and what will potentially be rendered
#define RENDER_SECTION_TEX_TEST 0
//This will define the section to begin the different texture rendering of sections
#define NEIGHBOR_RENDER_TEST_SECTION 3
//=================================================================================================================
#ifndef MAXSPRITES
#define MAXSPRITES
#define MAX_SPRITES 5000
#endif//MAXSPRITES
//=================================================================================================================
//=================================================================================================================
//=================================================================================================================
struct SectionTileData
{
	vector<string> imageNames;
	vector<string> animProfileNames;
	vector<int> animSpeeds;
	vector<int> hards;
};
//=================================================================================================================
struct MapLoadData
{
	vector<int> sectionVision;
	vector<string> sectionMusicName;
	vector<SectionTileData> sectionTileData;
};
//=================================================================================================================
//=================================================================================================================
class TopdownMap
{
public:
	TopdownMap(D3D* d3d, Map2DType mt, Camera* cam);
	TopdownMap(const TopdownMap& other);
	~TopdownMap();

	// Initialize the map
	bool Initialize(GameDirectory2D* gd, HWND hwndPB);
	bool InitializeTiles(MapLoadData mld, GameDirectory2D* gd, HWND hwndPB);
	bool InitializePositionsAndPlayerAndRenderingSections(int startingSection, string editor_path);

	void LoadSpriteFOWTiles(Sprite* sprite);
	Tile* InitPlayerFOWInSection(Section* section, XMFLOAT3 realpos);

	void InitAISprite(GameDirectory2D* gd);
	void LoadSpritesMain();
	AISprite* GetSpriteByID(int id);
	AISprite* getSprite(int i) { return m_visionSprites[i]; }
	AISprite* GetPlayer();
	AISprite* ClippedSprite(int i);

	//If a sprite is not in any rendering section do not render it
	void ClipSprites();
	
	//Adds a sprite or player to the map
	void AddSprite(AISprite* s);
	void AddSpriteNoIndex(AISprite* s);
	void AddPlayer(AISprite* s);
	void AddPlayerNoIndex(AISprite* s);

	//Move the player on the map
	void MovePlayer(Keyboard* keyboard);

	int SectionsRendered();

	void PlayerMovingDir(bool& moved, int& dirUD, int& dirLR);

	XMFLOAT3 MatchingTilePos(float x, float y);

	void MoveTiles(float speed, int dirUD, int dirLR);

	//Getter variables for the map editor
	void SetInEditor(bool set) { mIsEditor = set; }
	void SetResized(bool set) { mResized = set; }

	//XMFLOAT2 CameraSectionPosition(int sec, int x, int y);

	//int GetCameraDownBounds(int x, int y);
	//int GetCameraRightBounds(int x, int y);

	//void ResetMapCameraPosition(int x, int y, bool isSectionPicker);

	//Take camera position + screen size and locate all sections inside
	void ClipSections(XMFLOAT3 pos);

	void TransformMap(HWND hwnd, Keyboard* keyboard, bool isSectionPicker);

	void ClipSpriteToMap(Sprite* spr, float speed, float& newX, float& newY, float& newHBX, float& newHBY);

	void BuildSectionNeighbors();

	bool IsInSectorCenter(int map_size, SectorType type, int id);
	bool IsInSpecialCaseSectorCenter(int map_size, SectorType type, int id);
	int GetCornerSectionNumberByMapSize(int map_size, SAType type);
	vector<int> BuildSector(int map_size, int begin);

	//Get a tile at the specified position
	Tile* GetTile(int x, int y);

	void UpdateTileHardness(int x, int y, bool bHard);
	void MakeTileHard(int x, int y);

	//Transforms the world camera position based on four directions
	//void TransformWorld(int dir);

	void UndoTileTextures(vector<string> revertNames, vector<Tile*> tiles, string base_tile_path);
	void RedoTileTextures(vector<string> revertNames, vector<Tile*> tiles, string base_tile_path);

	XMFLOAT2 GetFirstTilePos(int section);
	bool SelectRenderingSection(int section);
	bool IsPosInTile(XMFLOAT2 pos, XMFLOAT2 &r, bool pInSectionPicker);
	void UpdateTileTexture(int x, int y, string base_tile_path, string textureFilename, int bitmapWidth, int bitmapHeight);

	//Locate the starting section based on where the player is
	bool LocateStartSection();

	void ToggleFow();

	bool DoesPlayerExist();

	//bool PlayerCollidingWithAnything();

	//If the player moves from one section to another
	//We will load the new render section and its neighbors
	void RenderSectionChange(XMFLOAT3 cameraPos);
	void LoadRenderSections(int section);

	//bool Render10(TextureShader* textureShader, TransparentShader* transparentShader, Camera* camera);
	//bool Render11(SpriteBatch* sb, TextureShader* textureShader, TransparentShader* transparentShader, Camera* camera);
	bool Render(Camera* camera);
	
	EngineOptions* GetEngineOptions() { return m_engineOptions; }

	vector<Section*> GetSections();
	
	vector<Section*> GetRenderedSections();

	void InitializeSectionPicker(string base_path);
	void RenderSectionPicker(Camera* camera);

	XMFLOAT3 GetCameraPosition();

	int getNumSpritesRendered() { return m_numSpritesRendered; }

	void ReloadSprites(string base_editor_path, string base_sprite_path, string base_tile_path);

	void UpdateEngineOptions(EngineOptions* eo) { m_engineOptions = eo; }

	void SetCurrentVision(int vis) { m_currentVision = vis; }
	void SetCurrentMusic(string music) { m_currentMusic = music; }

	int GetCurrentVision() { return m_currentVision; }
	string GetCurrentMusic() { return m_currentMusic; }

	Section* SelectSection(int x, int y);

	void SortSprites();

	//void CreateSpritesText(GameDirectory2D* gd, HWND hwnd);

	int getMaxSprites() { return spriteNum; }//m_visionSprites.size(); }
	
	int TotalClippedSprites();

	void AddSpriteAnimations(string profileName, string base_game_folder, string base_sprite_path, int id);
	void AddTileAnimations(string profileName, string base_game_folder, string base_tile_path, float tile_x, float tile_y);

	bool SpriteClicked(AISprite*& clickedSprite, int mouse_x, int mouse_y);

	Tile* SelectTile(float x, float y);

	void DeleteDisplaySprite();
	void DeletePlayerSprite();

	void MoveSprite(AISprite*& clickedSprite, int mouse_x, int mouse_y);
	void RemoveSprite(AISprite* clickedSprite);

	bool IsTransforming() { return m_transforming; }
	bool IsUpEdge() { return m_upEdge; }
	bool IsDownEdge() { return m_downEdge; }
	bool IsLeftEdge() { return m_leftEdge; }
	bool IsRightEdge() { return m_rightEdge; }

	void SetSpriteHardboxVisible(bool visible);

	void SetRenderDisplaySprite(bool can_render);

	Map2DType GetMapType() { return m_MapType; }

	void SetMapName(string name) { m_MapName = name; }
	string GetMapName() { return m_MapName; }

	void SetMapSize(int size) { m_MapSize = size; }
	int GetMapSize() { return m_MapSize; }

	// Toggle the hardness of the tiles and sprites
	void ToggleHardness(bool bEnable);

	void EnableFOW(bool bEnable)
	{
		m_bFOW = bEnable;
		m_tileSystem->EnableFOW(bEnable);
	}

	bool IsFOWEnabled() { return m_bFOW; }

	void SetFOWRadius(int bRadius)
	{
		m_iFOWRadius = bRadius;
		m_tileSystem->SetFOWRadius(bRadius);
	}

	int GetFOWRadius() { return m_iFOWRadius; }

	void EnableFOWFlashlight(bool bEnable)
	{
		m_bFOWFlashlight = bEnable;
		m_tileSystem->EnableFOWFlashlight(bEnable);
	}
	
	bool IsFOWFlashlightEnabled() { return m_bFOWFlashlight; }

private:
	TileSystem* m_tileSystem;
	SleepTimer* m_sleepTimer;
	Camera* m_camera;
	//FogCover* m_fow;

	bool m_bFOW;
	int m_iFOWRadius;
	bool m_bFOWFlashlight;

	string m_MapName;

	D3D* m_D3DSystem;
	EngineOptions* m_engineOptions;

	Map2DType m_MapType;

	bool m_transforming;
	bool m_upEdge;
	bool m_downEdge;
	bool m_leftEdge;
	bool m_rightEdge;

	bool m_canRenderDisplaySprite;

	//Are we in the editor or the game
	bool mIsEditor;
	//Did the editor just resize
	bool mResized;

	Sprite* player;
	Sprite* mapSprite;

	int m_MapSize;

	//What is the current vision for the map
	int m_currentVision;

	//What is the current music for the map
	string m_currentMusic;

	static int MAX_VISION;//Max vision will be 5

	//Test sprites
	Sprite* sword;

	//MapStartInfo m_mapstartinfo;

	vector<int> mNorthSections;
	vector<int> mSouthSections;
	vector<int> mEastSections;
	vector<int> mWestSections;

	int m_numSpritesRendered;

	float m_insertSpeed;

	int spriteNum;

	//List of section numbers to be sent to the renderer
	//The max number of sections rendered at one time is 9 (center)
	//and the min number of sections rendered at one time is 4 (corner)
	vector<int> mSectionRenderList;

	//List of sprites on the map
	//The int is the vision for the map
	vector< AISprite* > m_visionSprites;

	struct SPRQUE
	{
		AISprite* sprite;
		int depth;
		int sprite_num;
	};

	int partition(vector <SPRQUE> &arr, int left, int right);
	void quickSort(vector <SPRQUE> &arr, int left, int right);

	vector <SPRQUE> sprite_list;

	bool sorted;
};
//=================================================================================================================
//=================================================================================================================
#endif//__TOPDOWNMAP_H