#include "PlatformerWorld.h"
#include "ZShadeXMLLoader.h"
#include "ZShadeXMLSaver.h"
//==================================================================================================================================
//==================================================================================================================================
PlatformerWorld::PlatformerWorld(D3D* d3d)
:	m_D3DSystem(d3d)
{
	m_physicsWorld.reset(NULL);
	m_sprites_main_init = false;
}
//==================================================================================================================================
PlatformerWorld::~PlatformerWorld()
{
}
//==================================================================================================================================
void PlatformerWorld::UpdateScrollOffset(XMFLOAT2 offset)
{
	map<string, PlatformerMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		if ((*iter).second->InEditor())
			(*iter).second->Offset() = offset;
	}
}
//==================================================================================================================================
void PlatformerWorld::UpdateScrollOffset(float x, float y)
{
	map<string, PlatformerMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		if ((*iter).second->InEditor())
			(*iter).second->Offset() = XMFLOAT2(x, y);
	}
}
//==================================================================================================================================
void PlatformerWorld::PlayerMovingDir(bool& moved, int& dirUD, int& dirLR)
{
	map<string, PlatformerMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		(*iter).second->PlayerMovingDir(moved, dirUD, dirLR);
	}
}
//==================================================================================================================================
XMFLOAT3 PlatformerWorld::MatchingTilePos(float x, float y)
{
	map<string, PlatformerMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		return (*iter).second->MatchingTilePos(x, y);
	}
	
	return XMFLOAT3(0, 0, 0);
}
//==================================================================================================================================
void PlatformerWorld::ClipSpriteToMap(Sprite* spr, float speed, float& newX, float& newY, float& newHBX, float& newHBY)
{
	map<string, PlatformerMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		(*iter).second->ClipSpriteToMap(spr, speed, newX, newY, newHBX, newHBY);
	}
}
//==================================================================================================================================
int PlatformerWorld::NumSpritesInMap()
{
	map<string, PlatformerMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		return (*iter).second->NumSprites();
	}

	return 0;
}
//==================================================================================================================================
void PlatformerWorld::RemoveSprite(AISprite* spr)
{
	map<string, PlatformerMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		return (*iter).second->RemoveSprite(spr);
	}
}
//==================================================================================================================================
void PlatformerWorld::AddSprite(AISprite* s)
{
	map<string, PlatformerMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		(*iter).second->AddSprite(s);
	}
}
//==================================================================================================================================
void PlatformerWorld::AddSpriteAnimation(string profile_name, GameDirectory2D* gd, AISprite* s)
{
	map<string, PlatformerMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		(*iter).second->AddSpriteAnimations(profile_name, gd->GAME_FOLDER, gd->m_sprites_path, s->ID());
	}
}
//==================================================================================================================================
AISprite* PlatformerWorld::GetSpriteByID(int id)
{
	map<string, PlatformerMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		return (*iter).second->GetSpriteByID(id);
	}

	return NULL;
}
//==================================================================================================================================
void PlatformerWorld::GetPlayerPos(float& x, float &y)
{
	map<string, PlatformerMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		Sprite* player = (*iter).second->GetPlayer();
		x = player->TopLeftPosition().x;
		y = player->TopLeftPosition().y;
	}
}
//==================================================================================================================================
void PlatformerWorld::CreateSpritesText(GameDirectory2D* gd, HWND hwnd)
{
	map<string, PlatformerMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		//(*iter).second->CreateSpritesText(gd, hwnd);
	}
}
//==================================================================================================================================
void PlatformerWorld::LoadSpritesMain()
{
	//Using this one
	if (!m_sprites_main_init)
	{
		m_sprites_main_init = true;

		map<string, PlatformerMap*>::iterator iter = m_world_maps.find(m_map_to_render);
		if (iter != m_world_maps.end())
		{
			(*iter).second->LoadSpritesMain();
		}
	}
}
//==================================================================================================================================
void PlatformerWorld::InitAISprites(GameDirectory2D* gd)
{
	map<string, PlatformerMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		//(*iter).second->InitAISprite(gd);
	}
}
//==================================================================================================================================
void PlatformerWorld::RunSpritesMain()
{
	map<string, PlatformerMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		//Load the main function for each sprite
		//(*iter).second->LoadSpritesMain();
	}
}
//==================================================================================================================================
void PlatformerWorld::InitPhysicsWorld()
{
	// Initialize the physics world for the map
	m_physicsWorld.reset(new PhysicsWorld(m_D3DSystem, GetMap(m_map_to_render)));
	m_physicsWorld->BuildTileBodies();
}
//==================================================================================================================================
void PlatformerWorld::SetMapToRender(string p_map_to_render)
{
	m_map_to_render = p_map_to_render;
	
	InitPhysicsWorld();
}
//==================================================================================================================================
void PlatformerWorld::NewMap(string map_name, GameDirectory2D* gd, Camera* camera, bool InEditor, HWND hwndPB)
{
	PlatformerMap* map = new PlatformerMap(m_D3DSystem);

	map->InEditor() = InEditor;

	string path = gd->m_required_textures;
	
	map->Initialize(gd, hwndPB);

	m_world_maps.insert(make_pair(map_name, map));
}
//==================================================================================================================================
void PlatformerWorld::LoadMap(string& map_name, string filename, GameDirectory2D* gd, Camera* camera, bool InEditor, HWND hwndPB)
{
	PlatformerMap* map;

	string path = gd->m_xml_maps_path;
	path.append("\\").append(filename).append(".xml");

	ZShadeXMLLoader::LoadMapXML(map_name, path, gd, camera, m_D3DSystem, map, InEditor, hwndPB);

	map->InEditor() = InEditor;

	m_world_maps.insert( make_pair(map_name, map) );
}
//==================================================================================================================================
void PlatformerWorld::RemoveMap(string activeMapName)
{
	// Get the iterator to remove
	map<string, PlatformerMap*>::iterator iter = m_world_maps.find(activeMapName);
	
	// Clear the rendered map
	m_map_to_render = "";

	// Remove the map
	m_world_maps.erase(activeMapName);
}
//==================================================================================================================================
PlatformerMap* PlatformerWorld::GetMap(string mapName)
{
	return m_world_maps[mapName];
}
//==================================================================================================================================
void PlatformerWorld::Update(float dt, Keyboard* keyboard, HWND hwnd)
{
	map<string, PlatformerMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		if (m_physicsWorld.get() != NULL)
		{
			if ((*iter).second->InEditor())
				m_physicsWorld->IsGravityOn() = false;
			
			m_physicsWorld->Update(dt, keyboard);
		}
	}
}
//==================================================================================================================================
/*bool PlatformerWorld::Render10(TextureShader* textureShader, TransparentShader* transparentShader, Camera* camera)
{
	map<string, PlatformerMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		//(*iter).second->Render10(textureShader, transparentShader, camera);
	}

	return true;
}*/
//==================================================================================================================================
bool PlatformerWorld::Render(Camera* camera)
{
	map<string, PlatformerMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		(*iter).second->Render(camera);
		
		if (m_physicsWorld.get() != NULL)
		{
			//m_physicsWorld->RenderContactStats();
		}
	}

	return true;
}
//==================================================================================================================================