//================================================================================================================
//World.h
//================================================================================================================
#ifndef __WORLD_H
#define __WORLD_H
//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
//C/C++ Includes
#include <Windows.h>
#include <string>
#include <map>
#include "D3D.h"
#include "Input.h"
#include "Camera.h"
#include "TopdownMap.h"
#include "GameDirectory.h"
#include "MemoryMgr.h"
#include "PhysicsWorld.h"
using namespace std;
//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
class World
{
public:
	MemRef reference;

	World(D3D* d3d);
	World(const World& l);
	~World();

	void NewMap(string map_name, GameDirectory2D* gd, Map2DType mt, Camera* camera, bool InEditor, HWND hwndPB);
	void LoadMap(string& map_name, string filename, GameDirectory2D* gd, Camera* camera, bool InEditor, HWND hwndPB);
	void RemoveMap(string activeMapName);

	void TransformWorld(int dir);

	void Update(float dt, Keyboard* keyboard, HWND hwnd, bool isSectionPicker);
	
	//bool Render10(TextureShader* textureShader, TransparentShader* transparentShader, Camera* camera);
	bool Render(Camera* camera);
	bool RenderSectionPicker(Camera* camera);

	void PlayerMovingDir(bool& moved, int& dirUD, int& dirLR);
	XMFLOAT2 GetFirstTilePos(int section);
	XMFLOAT3 MatchingTilePos(float x, float y);
	void ClipSpriteToMap(Sprite* spr, float speed, float& newX, float& newY, float& newHBX, float& newHBY);

	void AddSprite(AISprite* s);
	void AddSpriteAnimation(string profile_name, GameDirectory2D* gd, AISprite* s);
	void LoadSpritesMain();
	void RunSpritesMain();
	void GetPlayerPos(float& x, float &y);
	void SetMapToRender(string p_map_to_render);
	void SetRenderSection(int section);
	void InitAISprites(GameDirectory2D* gd);
	void RemoveSprite(AISprite* spr);
	//void CreateSpritesText(GameDirectory2D* gd, HWND hwnd);
	int NumSpritesInMap();

	AISprite* GetSpriteByID(int id);

	TopdownMap* GetMap(string mapName);

	map<string, TopdownMap*> GetWorldMaps() { return m_world_maps; }

	void SetD3DSystem(D3D* d3d) { m_D3DSystem = d3d; }

	D3D* D3DSystem() { return m_D3DSystem; }

	string MapToRender() const { return m_map_to_render; }

private:
	D3D* m_D3DSystem;
	unique_ptr<PhysicsWorld> m_physicsWorld;

	string m_map_to_render;
	
	bool m_sprites_main_init;

	map<string, TopdownMap*> m_world_maps;
};
//----------------------------------------------------------------------------------------------------------------
struct SWorld
{
	vector<string> m_mapNames;
	World* m_World;
	string sWorldName;
};
//----------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------
#endif//__WORLD_H