#include "World.h"
#include "ZShadeXMLLoader.h"
#include "ZShadeXMLSaver.h"
//----------------------------------------------------------------------------------------------------------------
//SmartPointer<World> World::m_world_global = 0;
//----------------------------------------------------------------------------------------------------------------
World::World(D3D* d3d)
:	m_D3DSystem(d3d)
{
	m_physicsWorld.reset(NULL);
	m_sprites_main_init = false;
}
//----------------------------------------------------------------------------------------------------------------
World::World(const World& l)
{
}
//----------------------------------------------------------------------------------------------------------------
World::~World()
{
}
//----------------------------------------------------------------------------------------------------------------
XMFLOAT2 World::GetFirstTilePos(int section)
{
	map<string, TopdownMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		return (*iter).second->GetFirstTilePos(section);
	}

	return XMFLOAT2(0, 0);
}
//----------------------------------------------------------------------------------------------------------------
void World::PlayerMovingDir(bool& moved, int& dirUD, int& dirLR)
{
	map<string, TopdownMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		(*iter).second->PlayerMovingDir(moved, dirUD, dirLR);
	}
}
//----------------------------------------------------------------------------------------------------------------
XMFLOAT3 World::MatchingTilePos(float x, float y)
{
	map<string, TopdownMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		return (*iter).second->MatchingTilePos(x, y);
	}
}
//----------------------------------------------------------------------------------------------------------------
void World::ClipSpriteToMap(Sprite* spr, float speed, float& newX, float& newY, float& newHBX, float& newHBY)
{
	map<string, TopdownMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		(*iter).second->ClipSpriteToMap(spr, speed, newX, newY, newHBX, newHBY);
	}
}
//----------------------------------------------------------------------------------------------------------------
int World::NumSpritesInMap()
{
	map<string, TopdownMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		return (*iter).second->getMaxSprites();
	}

	return 0;
}
//----------------------------------------------------------------------------------------------------------------
void World::RemoveSprite(AISprite* spr)
{
	map<string, TopdownMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		return (*iter).second->RemoveSprite(spr);
	}
}
//----------------------------------------------------------------------------------------------------------------
void World::AddSprite(AISprite* s)
{
	map<string, TopdownMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		(*iter).second->AddSprite(s);
	}
}
//----------------------------------------------------------------------------------------------------------------
void World::AddSpriteAnimation(string profile_name, GameDirectory2D* gd, AISprite* s)
{
	map<string, TopdownMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		(*iter).second->AddSpriteAnimations(profile_name, gd->GAME_FOLDER, gd->m_sprites_path, s->ID());
	}
}
//----------------------------------------------------------------------------------------------------------------
AISprite* World::GetSpriteByID(int id)
{
	map<string, TopdownMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		return (*iter).second->GetSpriteByID(id);
	}

	return NULL;
}
//----------------------------------------------------------------------------------------------------------------
void World::GetPlayerPos(float& x, float &y)
{
	map<string, TopdownMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		Sprite* player = (*iter).second->GetPlayer();
		x = player->TopLeftPosition().x;
		y = player->TopLeftPosition().y;
	}
}
//----------------------------------------------------------------------------------------------------------------
/*void World::CreateSpritesText(GameDirectory2D* gd, HWND hwnd)
{
	map<string, TopdownMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		(*iter).second->CreateSpritesText(gd, hwnd);
	}
}*/
//----------------------------------------------------------------------------------------------------------------
void World::LoadSpritesMain()
{
	//Using this one
	map<string, TopdownMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		(*iter).second->LoadSpritesMain();
	}
}
//----------------------------------------------------------------------------------------------------------------
void World::InitAISprites(GameDirectory2D* gd)
{
	map<string, TopdownMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		(*iter).second->InitAISprite(gd);
	}
}
//----------------------------------------------------------------------------------------------------------------
void World::RunSpritesMain()
{
	map<string, TopdownMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		//Load the main function for each sprite
		(*iter).second->LoadSpritesMain();
	}
}
//----------------------------------------------------------------------------------------------------------------
void World::SetMapToRender(string p_map_to_render)
{
	m_map_to_render = p_map_to_render;

	map<string, TopdownMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		//Find the starting section based on where the player is
		(*iter).second->LocateStartSection();
	}
}
//----------------------------------------------------------------------------------------------------------------
void World::SetRenderSection(int section)
{
	map<string, TopdownMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		(*iter).second->SelectRenderingSection(section);
	}
}
//----------------------------------------------------------------------------------------------------------------
void World::TransformWorld(int dir)
{
	map<string, TopdownMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		//(*iter).second->TransformWorld(dir);
	}
}
//----------------------------------------------------------------------------------------------------------------
void World::NewMap(string map_name, GameDirectory2D* gd, Map2DType mt, Camera* camera, bool InEditor, HWND hwndPB)
{
	TopdownMap* map = new TopdownMap(m_D3DSystem, mt, camera);

	string path = gd->m_required_textures;
	
	map->Initialize(gd, hwndPB);

	map->SetInEditor(false);

	if (InEditor)
	{
		map->SetInEditor(true);
		map->InitializeSectionPicker(gd->m_required_textures);
	}

	m_world_maps.insert(make_pair(map_name, map));
}
//----------------------------------------------------------------------------------------------------------------
void World::LoadMap(string& map_name, string filename, GameDirectory2D* gd, Camera* camera, bool InEditor, HWND hwndPB)
{
	TopdownMap* map;// = new Map(m_D3DSystem, mt, camera);

	string path = gd->m_xml_maps_path;
	path.append("\\").append(filename).append(".xml");

	ZShadeXMLLoader::LoadMapXML(map_name, path, gd, camera, m_D3DSystem, map, InEditor, hwndPB);

	map->SetInEditor(false);

	if (InEditor)
	{
		map->SetInEditor(true);
		//map->InitializeSectionPicker(gd->m_required_textures);
	}

	m_world_maps.insert( make_pair(map_name, map) );
}
//----------------------------------------------------------------------------------------------------------------
void World::RemoveMap(string activeMapName)
{
	// Get the iterator to remove
	map<string, TopdownMap*>::iterator iter = m_world_maps.find(activeMapName);
	
	// Clear the rendered map
	m_map_to_render = "";

	// Remove the map
	m_world_maps.erase(activeMapName);
}
//----------------------------------------------------------------------------------------------------------------
TopdownMap* World::GetMap(string mapName)
{
	return m_world_maps[mapName];
}
//----------------------------------------------------------------------------------------------------------------
void World::Update(float dt, Keyboard* keyboard, HWND hwnd, bool isSectionPicker)
{
	map<string, TopdownMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		//(*iter).second->TransformMap(hwnd, keyboard, isSectionPicker);

		if (m_physicsWorld.get() != NULL)
		{
			// A topdown world has no gravity
			m_physicsWorld->IsGravityOn() = false;
			
			m_physicsWorld->Update(dt, keyboard);
		}
	}
}
//----------------------------------------------------------------------------------------------------------------
/*bool World::Render10(TextureShader* textureShader, TransparentShader* transparentShader, Camera* camera)
{
	map<string, TopdownMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		(*iter).second->Render10(textureShader, transparentShader, camera);
	}

	return true;
}*/
//----------------------------------------------------------------------------------------------------------------
bool World::RenderSectionPicker(Camera* camera)
{
	map<string, TopdownMap*>::iterator iter = m_world_maps.find(m_map_to_render);
	if (iter != m_world_maps.end())
	{
		(*iter).second->RenderSectionPicker(camera);
	}

	return true;
}
//----------------------------------------------------------------------------------------------------------------
bool World::Render(Camera* camera)
{
	map<string, TopdownMap*>::iterator iter = m_world_maps.find(m_map_to_render);
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
//----------------------------------------------------------------------------------------------------------------
