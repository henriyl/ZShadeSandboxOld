//******************************************************************************************************
// TileSystem.h
//******************************************************************************************************
#ifndef __TILESYSTEM_H
#define __TILESYSTEM_H
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//Includes
#include <vector>
using namespace std;
#include "Section.h"
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//This will hold a plane of sections
class TileSystem
{
	vector<Section*> m_Sections;
	vector<Tile*> m_OverworldTiles;
	vector<Tile*> m_FogTiles;

	D3D* m_D3DSystem;
	EngineOptions* m_engineOptions;
	
	bool m_bFOW;
	int m_iFOWRadius;
	bool m_bFOWFlashlight;

public:
	TileSystem(D3D* d3d);
	TileSystem(const TileSystem&);
	~TileSystem();

	void UpdateTileHardness(int x, int y, bool bHard);
	void MakeTileHard(int x, int y);

	void EnableFOW(bool bEnable);
	bool IsFOWEnabled() { return m_bFOW; }

	void SetFOWRadius(int bRadius);
	int GetFOWRadius() { return m_iFOWRadius; }

	void EnableFOWFlashlight(bool bEnable);
	bool IsFOWFlashlightEnabled() { return m_bFOWFlashlight; }

	int GetNumSections() { return m_Sections.size(); }

	bool Initialize();

	bool CreateTileSection(int id, int x, int y, int size, bool hard, int vision, string musicName,
		XMFLOAT3 pos, GameDirectory2D* gd, string textureFilename, bool init, string animProfileName, int animSpeed, int bitmapWidth, int bitmapHeight);

	void InitializeOverworldTiles(int x, int y, int size, string base_tile_path, string textureFilename, int bitmapWidth, int bitmapHeight);
	void RenderOverworldTiles(Camera* camera);

	void MoveTiles(float speed, int dirUD, int dirLR);

	Tile* SelectTile(float x, float y);

	void Shutdown();

	void SetTileDisplayHardbox(bool visible);

	vector<Section*> GetSections();

	void ReloadSprites(string base_tile_path);

	Section* SelectSection(int x, int y);

	//Update fog for the rendered sections with the player
	void UpdateFog(vector<int> sectionRenderList, Sprite* player, int radius);
	void AssessFog(vector<int> sectionRenderList);
	void RenderTopFog(vector<int> sectionRenderList, Camera* camera);
	void FowToggled(vector<int> sectionRenderList);
	Tile* InitPlayerFOWInSection(Section* section, XMFLOAT3 realpos, unsigned short mask);

	bool UpdateSectionFogMask(Section* section, XMFLOAT2 realTilePos, XMFLOAT2 realSpriteTilePos, Sprite* sprite, unsigned short mask);

	bool IsPosInTile(XMFLOAT2 pos, XMFLOAT2 &r, bool pInSectionPicker, vector<int> sectionRenderList);

	XMFLOAT3 GetTilePos(int id, int x, int y);
	void UpdatePosition(int id, int x, int y, XMFLOAT3 pos);

	void SetNewTexture(int id,string base_tile_path, string textureFilename, int bitmapWidth, int bitmapHeight);

	bool SectionExists(int dir);
	Section* GetSection(int id);

	void UpdateTileTexture(int x, int y, string base_tile_path, string textureFilename, int bitmapWidth, int bitmapHeight);

	Tile* GetTile(int x, int y);

	void UpdateTextures(vector<int> sectionRenderList);

	//Translate out of bounds spots to a spot on a coorisponding neighbor
	XMFLOAT2 TranslateToSpot(XMFLOAT2 value);

	//int GetSectionIDFromDir(vector<Section::NeighborIDS> neighbors, char* dir);

	bool Render(Camera* camera, vector<int> sectionRenderList);
};
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
#endif//__TILESYSTEM_H