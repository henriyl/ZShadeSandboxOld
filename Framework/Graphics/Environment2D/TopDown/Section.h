//******************************************************************************************************
// Section.h
//******************************************************************************************************
#ifndef __SECTION_H
#define __SECTION_H
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//Includes
#include "Tile.h"
#include "Sprite.h"
#include <map>
#include "GameDirectory.h"
#include "AnimationProfile2D.h"
#include "MemoryMgr.h"
using namespace std;
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//This will hold a plane of tiles (12x8)
class Section
{
public:
	/*
	struct NeighborIDS
	{
		char* dir;
		int id;
	};
	*/
	struct BB
	{
		int firstTileX;
		int firstTileY;
		int lastTileX;
		int lastTileY;
	};

	BB m_BoundingBox;
	
private:
	D3D* m_D3DSystem;
	EngineOptions* m_engineOptions;

	//Tile*** m_Tiles;
	SmartArray2D<Tile*> m_Tiles;
	//vector<NeighborIDS> neighborids;
	//vector<int> mRenderSections;

	SectionAdjacency* mAdjacencyInfo;

	int ID;
	
	bool m_bFOW;
	int m_iFOWRadius;
	bool m_bFOWFlashlight;

	/*
	int NB;
	int SB;
	int EB;
	int WB;

	int NWB;
	int NEB;
	int SWB;
	int SEB;
	*/
	
	//What is the current vision for the section
	int m_vision;

	string m_musicName;

public:
	Section(D3D* d3d);
	Section(const Section&);
	~Section();

	void EnableFOW(bool bEnable) { m_bFOW = bEnable; }
	bool IsFOWEnabled() { return m_bFOW; }

	void SetFOWRadius(int bRadius) { m_iFOWRadius = bRadius; }
	int GetFOWRadius() { return m_iFOWRadius; }

	void EnableFOWFlashlight(bool bEnable) { m_bFOWFlashlight = bEnable; }
	bool IsFOWFlashlightEnabled() { return m_bFOWFlashlight; }

	void SetAdjacencyInfo(SectionAdjacency* adj) { mAdjacencyInfo = adj; }
	SectionAdjacency* AdjacencyInfo() { return mAdjacencyInfo; }

	bool Initialize(int id);

	bool CreateTile(int x, int y, int size, bool hard,
		XMFLOAT3 pos, GameDirectory2D* gd, string textureFilename, bool init,
		string animProfileName, int animSpeed, int bitmapWidth, int bitmapHeight);

	void Shutdown();

	void MoveTiles(float speed, int dirUD, int dirLR);

	void SetNewTexture(string base_tile_path, string textureFilename, int bitmapWidth, int bitmapHeight);

	void UpdateTileTexture(int x, int y, string base_tile_path, string textureFilename, int bitmapWidth, int bitmapHeight);

	Tile* SelectTile(float x, float y);

	int GetSectionID() { return ID; }

	void SetTileDisplayHardbox(bool visible);

	Tile* GetTile(int x, int y) { return m_Tiles(x, y); }

	vector<Tile*> GetTiles();

	XMFLOAT3 GetTilePos(int x, int y);
	void UpdatePosition(int x, int y, XMFLOAT3 pos);

	void UpdateTexture();

	void SetVision(int vision) { m_vision = vision; }
	int GetVision() { return m_vision; }

	void SetMusicName(string musicName) { m_musicName = musicName; }
	string GetMusicName() { return m_musicName; }

	//vector<NeighborIDS> GetNeighbors();
	//void AddNeighborID(NeighborIDS id);

	//vector<int> GetRenderSections();
	//void AddRenderSection(int id);

	void UpdateFogBit(unsigned short bit);

	void ReloadSprites(string base_tile_path);

	BB GetBounds() { return m_BoundingBox; }
	bool InBounds(int x, int y);
	void SetBounds(BB box) { m_BoundingBox = box; }
	void UpdateBounds();

	/*
	int NorthBoundary() { return NB; }
	int SouthBoundary() { return SB; }
	int EastBoundary() { return EB; }
	int WestBoundary() { return WB; }
	int NorthWestBoundary() { return NWB; }
	int NorthEastBoundary() { return NEB; }
	int SouthWestBoundary() { return SWB; }
	int SouthEastBoundary() { return SEB; }

	void AddNorthBoundary(int B) { if (NB == -1) NB = B; }
	void AddSouthBoundary(int B) { if (SB == -1) SB = B; }
	void AddEastBoundary(int B) { if (EB == -1) EB = B; }
	void AddWestBoundary(int B) { if (WB == -1) WB = B; }
	void AddNorthWestBoundary(int B) { if (NWB == -1) NWB = B; }
	void AddNorthEastBoundary(int B) { if (NEB == -1) NEB = B; }
	void AddSouthWestBoundary(int B) { if (SWB == -1) SWB = B; }
	void AddSouthEastBoundary(int B) { if (SEB == -1) SEB = B; }
	*/

	void ReTexBounds(int bitmapWidth, int bitmapHeight);

	void ReSet(int bitmapWidth, int bitmapHeight);

	bool Render(Camera* camera);

	Section* Clone();
};
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
#endif//__SECTION_H