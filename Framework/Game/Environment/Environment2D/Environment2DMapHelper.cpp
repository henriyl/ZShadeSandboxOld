#include "Environment2DMapHelper.h"
#include "World.h"
#include "Convert.h"
#include "SectionAdjacency.h"
#include "SectorBuilder.h"
#include "MapEditorSystem2D.h"
#include "MenuEditorSystem.h"
#include "GDI.h"
#include "Scripting.h"
#include "AudioSystem.h"
#include "ZShadeXMLLoader.h"
#include "ZShadeXMLSaver.h"
#include "EnvironmentFactory.h"
//================================================================================================================
//================================================================================================================
Environment2DMapHelper::Environment2DMapHelper()
{
}
//================================================================================================================
Environment2DMapHelper::~Environment2DMapHelper()
{
}
//================================================================================================================
void Environment2DMapHelper::GetWorldAndMapName(string& activeMapName, string& activeWorldName)
{
	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	activeMapName = env2D->ActiveMapName();
	activeWorldName = env2D->ActiveWorldName();
}
//================================================================================================================
void Environment2DMapHelper::SetActiveMapName(string name)
{
	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->WorldLoaded())
	{
		if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
		{
			World* world = env2D->GetTopdownWorld(env2D->ActiveWorldName());
			world->SetMapToRender(name);
			env2D->ActiveMapName() = world->MapToRender();
			world->SetRenderSection(0);
		}
		else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
		{
			PlatformerWorld* world = env2D->GetPlatformerWorld(env2D->ActiveWorldName());
			world->SetMapToRender(name);
			env2D->ActiveMapName() = world->MapToRender();
		}

		env2D->MapLoaded() = true;

		//if (Scripting::GetEngineOptions()->m_inEditor)
		//	env2D->GetTabEditWindow2D()->InitWorldLoaded();
	}
}
//================================================================================================================
void Environment2DMapHelper::SetActiveRenderedMap(string world_name, string map_name)
{
	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	env2D->WorldLoaded() = true;
	env2D->ActiveWorldName() = world_name;
	SetActiveMapName(map_name);
}
//================================================================================================================
XMFLOAT2 Environment2DMapHelper::SelectRenderingSection(int section)
{
	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return XMFLOAT2(0, 0);

	World* world = env2D->GetTopdownWorld(env2D->ActiveWorldName());
	world->SetRenderSection(section);
	return world->GetFirstTilePos(section);
}
//================================================================================================================
XMFLOAT3 Environment2DMapHelper::GetPlayerPosition()
{
	float x = 0;
	float y = 0;

	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return XMFLOAT3(0, 0, 0);

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		World* world = env2D->GetTopdownWorld(env2D->ActiveWorldName());
		world->GetPlayerPos(x, y);
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerWorld* world = env2D->GetPlatformerWorld(env2D->ActiveWorldName());
		world->GetPlayerPos(x, y);
	}

	return XMFLOAT3(x, y, 0);
}
//================================================================================================================
void Environment2DMapHelper::PlayerMovingDir(bool& moved, int& dirUD, int& dirLR)
{
	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		World* world = env2D->GetTopdownWorld(env2D->ActiveWorldName());
		world->PlayerMovingDir(moved, dirUD, dirLR);
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerWorld* world = env2D->GetPlatformerWorld(env2D->ActiveWorldName());
		world->PlayerMovingDir(moved, dirUD, dirLR);
	}
}
//================================================================================================================
XMFLOAT3 Environment2DMapHelper::MatchingTilePos(float x, float y)
{
	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return XMFLOAT3(0, 0, 0);

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		World* world = env2D->GetTopdownWorld(env2D->ActiveWorldName());
		return world->MatchingTilePos(x, y);
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerWorld* world = env2D->GetPlatformerWorld(env2D->ActiveWorldName());
		return world->MatchingTilePos(x, y);
	}
}
//================================================================================================================
void Environment2DMapHelper::ClipSpriteToMap(Sprite* spr, float speed, float& newX, float& newY, float& newHBX, float& newHBY)
{
	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		World* world = env2D->GetTopdownWorld(env2D->ActiveWorldName());
		world->ClipSpriteToMap(spr, speed, newX, newY, newHBX, newHBY);
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		PlatformerWorld* world = env2D->GetPlatformerWorld(env2D->ActiveWorldName());
		world->ClipSpriteToMap(spr, speed, newX, newY, newHBX, newHBY);
	}
}
/*
//================================================================================================================
void Environment2DMapHelper::AddMenuButton(string activeMenuName, Button* button)
{
	m_MenuSystem->AddMenuButton(activeMenuName, button);
}
//================================================================================================================
void Environment2DMapHelper::AddMenuText(string activeMenuName, Text* text)
{
	m_MenuSystem->AddMenuText(activeMenuName, text);
}
//================================================================================================================
void Environment2DMapHelper::CreateMenu(string menu_name)
{
	Menu* m = new Menu();
	m->SetMenuName(menu_name);
	m_MenuSystem->AddMenu(m);
}
*/
//================================================================================================================
void Environment2DMapHelper::AddTile(string activeMapName, string activeWorldName, int x, int y)
{
	if (BetterString(activeWorldName) == "")
		return;
	if (BetterString(activeMapName) == "")
		return;

	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		if (!env2D->GetPlatformerWorld(activeWorldName))
			return;
		if (!env2D->GetPlatformerWorld(activeWorldName))
			return;
		if (!env2D->GetPlatformerWorld(activeWorldName)->GetMap(activeMapName))
			return;

		PlatformerMap* map = env2D->GetPlatformerWorld(activeWorldName)->GetMap(activeMapName);

		map->AddTile(env2D->GetGD2D(), x, y);
	}
}
//================================================================================================================
void Environment2DMapHelper::RemoveTile(string activeMapName, string activeWorldName, int x, int y)
{
	if (BetterString(activeWorldName) == "")
		return;
	if (BetterString(activeMapName) == "")
		return;

	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		if (!env2D->GetPlatformerWorld(activeWorldName))
			return;
		if (!env2D->GetPlatformerWorld(activeWorldName))
			return;
		if (!env2D->GetPlatformerWorld(activeWorldName)->GetMap(activeMapName))
			return;

		PlatformerMap* map = env2D->GetPlatformerWorld(activeWorldName)->GetMap(activeMapName);

		map->RemoveTile(x, y);
	}
}
//================================================================================================================
void Environment2DMapHelper::RotateTile(string activeMapName, string activeWorldName, int x, int y)
{
	if (BetterString(activeWorldName) == "")
		return;
	if (BetterString(activeMapName) == "")
		return;

	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		if (!env2D->GetPlatformerWorld(activeWorldName))
			return;
		if (!env2D->GetPlatformerWorld(activeWorldName))
			return;
		if (!env2D->GetPlatformerWorld(activeWorldName)->GetMap(activeMapName))
			return;

		PlatformerMap* map = env2D->GetPlatformerWorld(activeWorldName)->GetMap(activeMapName);

		map->RotateTile(x, y);
	}
}
//================================================================================================================
void Environment2DMapHelper::DisableTileTexture(string activeMapName, string activeWorldName, int x, int y)
{
	if (BetterString(activeWorldName) == "")
		return;
	if (BetterString(activeMapName) == "")
		return;

	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		if (!env2D->GetPlatformerWorld(activeWorldName))
			return;
		if (!env2D->GetPlatformerWorld(activeWorldName))
			return;
		if (!env2D->GetPlatformerWorld(activeWorldName)->GetMap(activeMapName))
			return;

		PlatformerMap* map = env2D->GetPlatformerWorld(activeWorldName)->GetMap(activeMapName);

		map->DisableTexture(x, y);
	}
}
//================================================================================================================
void Environment2DMapHelper::UpdateTileTexture(string activeMapName, string activeWorldName, string textureName, int x, int y)
{
	if (BetterString(activeWorldName) == "")
		return;
	if (BetterString(activeMapName) == "")
		return;

	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		if (!env2D->GetTopdownWorld(activeWorldName))
			return;
		if (!env2D->GetTopdownWorld(activeWorldName))
			return;
		if (!env2D->GetTopdownWorld(activeWorldName)->GetMap(activeMapName))
			return;

		TopdownMap* map = env2D->GetTopdownWorld(activeWorldName)->GetMap(activeMapName);

		Tile* tile = map->SelectTile(x, y);

		if (tile != NULL)
		{
			map->UpdateTileTexture(x, y, env2D->GetGD2D()->m_tiles_path, textureName,
				env2D->GetEngineOptions()->TILE_SIZE, env2D->GetEngineOptions()->TILE_SIZE);
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		if (!env2D->GetPlatformerWorld(activeWorldName))
			return;
		if (!env2D->GetPlatformerWorld(activeWorldName))
			return;
		if (!env2D->GetPlatformerWorld(activeWorldName)->GetMap(activeMapName))
			return;

		PlatformerMap* map = env2D->GetPlatformerWorld(activeWorldName)->GetMap(activeMapName);

		PlatformerTile* tile = map->SelectTile(x, y);

		if (tile != NULL)
		{
			map->UpdateTileTexture(x, y, env2D->GetGD2D()->m_tiles_path, textureName,
				env2D->GetEngineOptions()->TILE_SIZE, env2D->GetEngineOptions()->TILE_SIZE);
		}
	}
}
/*
//================================================================================================================
void Environment2DMapHelper::RemoveMap(string activeMapName, string activeWorldName)
{
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		map<string, SWorld*>::iterator iter = m_WorldsTopdown.begin();

		bool found = false;

		for (; iter != m_WorldsTopdown.end(); iter++)
		{
			if ((*iter).first == activeWorldName)
			{
				World* world = (*iter).second->m_World;

				world->RemoveMap(activeMapName);

				vector<string, string>::iterator MapNameIter = (*iter).second->m_mapNames.begin();
				vector<string, string>::iterator KillIter;

				for (; MapNameIter != (*iter).second->m_mapNames.end(); MapNameIter++)
				{
					string name = (*MapNameIter);

					if (BetterString(name) == BetterString(activeMapName))
					{
						KillIter = MapNameIter;
						found = true;
						break;
					}
				}

				if (found)
				{
					(*iter).second->m_mapNames.erase(KillIter);
				}

				break;
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		map<string, SPlatformerWorld*>::iterator iter = m_WorldsPlatformer.begin();

		bool found = false;

		for (; iter != m_WorldsPlatformer.end(); iter++)
		{
			if ((*iter).first == activeWorldName)
			{
				PlatformerWorld* world = (*iter).second->m_World;

				world->RemoveMap(activeMapName);

				vector<string, string>::iterator MapNameIter = (*iter).second->m_mapNames.begin();
				vector<string, string>::iterator KillIter;

				for (; MapNameIter != (*iter).second->m_mapNames.end(); MapNameIter++)
				{
					string name = (*MapNameIter);

					if (BetterString(name) == BetterString(activeMapName))
					{
						KillIter = MapNameIter;
						found = true;
						break;
					}
				}

				if (found)
				{
					(*iter).second->m_mapNames.erase(KillIter);
				}

				break;
			}
		}
	}
}
*/
//================================================================================================================
void Environment2DMapHelper::NullTileAnimation(string activeMapName, string activeWorldName, int x, int y)
{
	if (BetterString(activeWorldName) == "")
		return;
	if (BetterString(activeMapName) == "")
		return;

	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		if (!env2D->GetTopdownWorld(activeWorldName))
			return;
		if (!env2D->GetTopdownWorld(activeWorldName))
			return;
		if (!env2D->GetTopdownWorld(activeWorldName)->GetMap(activeMapName))
			return;

		TopdownMap* map = env2D->GetTopdownWorld(activeWorldName)->GetMap(activeMapName);

		Tile* tile = map->SelectTile(x, y);

		if (tile == NULL) return;

		tile->SetAnimationProfileName("NONE");
		tile->NullSequence();
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		if (!env2D->GetPlatformerWorld(activeWorldName))
			return;
		if (!env2D->GetPlatformerWorld(activeWorldName))
			return;
		if (!env2D->GetPlatformerWorld(activeWorldName)->GetMap(activeMapName))
			return;

		PlatformerMap* map = env2D->GetPlatformerWorld(activeWorldName)->GetMap(activeMapName);

		PlatformerTile* tile = map->SelectTile(x, y);

		if (tile == NULL) return;

		tile->SetAnimationProfileName("NONE");
		tile->NullSequence();
	}
}
//================================================================================================================
void Environment2DMapHelper::UpdateTileAnimation(string activeMapName, string activeWorldName, string animProfileName, int animSpeed, int x, int y)
{
	if (BetterString(activeWorldName) == "")
		return;
	if (BetterString(activeMapName) == "")
		return;

	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		if (!env2D->GetTopdownWorld(activeWorldName))
			return;
		if (!env2D->GetTopdownWorld(activeWorldName))
			return;
		if (!env2D->GetTopdownWorld(activeWorldName)->GetMap(activeMapName))
			return;

		TopdownMap* map = env2D->GetTopdownWorld(activeWorldName)->GetMap(activeMapName);

		Tile* tile = map->SelectTile(x, y);

		if (tile == NULL) return;

		map->AddTileAnimations(animProfileName, env2D->GetGD2D()->GAME_FOLDER, env2D->GetGD2D()->m_tiles_path, x, y);
		tile->SetAnimationProfileName(animProfileName);
		tile->SetAnimationSpeed(animSpeed);
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		if (!env2D->GetPlatformerWorld(activeWorldName))
			return;
		if (!env2D->GetPlatformerWorld(activeWorldName))
			return;
		if (!env2D->GetPlatformerWorld(activeWorldName)->GetMap(activeMapName))
			return;

		PlatformerMap* map = env2D->GetPlatformerWorld(activeWorldName)->GetMap(activeMapName);

		PlatformerTile* tile = map->SelectTile(x, y);

		if (tile == NULL) return;

		map->AddTileAnimations(animProfileName, env2D->GetGD2D()->GAME_FOLDER, env2D->GetGD2D()->m_tiles_path, x, y);
		tile->SetAnimationProfileName(animProfileName);
		tile->SetAnimationSpeed(animSpeed);
	}
}
//================================================================================================================
void Environment2DMapHelper::UpdateTileHardness(string activeMapName, string activeWorldName, bool bHard, int x, int y)
{
	if (BetterString(activeWorldName) == "")
		return;
	if (BetterString(activeMapName) == "")
		return;

	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		if (!env2D->GetTopdownWorld(activeWorldName))
			return;
		if (!env2D->GetTopdownWorld(activeWorldName))
			return;
		if (!env2D->GetTopdownWorld(activeWorldName)->GetMap(activeMapName))
			return;

		TopdownMap* map = env2D->GetTopdownWorld(activeWorldName)->GetMap(activeMapName);
		
		map->UpdateTileHardness(x, y, bHard);
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		if (!env2D->GetPlatformerWorld(activeWorldName))
			return;
		if (!env2D->GetPlatformerWorld(activeWorldName))
			return;
		if (!env2D->GetPlatformerWorld(activeWorldName)->GetMap(activeMapName))
			return;

		PlatformerMap* map = env2D->GetPlatformerWorld(activeWorldName)->GetMap(activeMapName);
		
		map->UpdateTileHardness(x, y, bHard);
	}
}
//================================================================================================================
void Environment2DMapHelper::MakeTileHard(string activeMapName, string activeWorldName, int x, int y)
{
	if (BetterString(activeWorldName) == "")
		return;
	if (BetterString(activeMapName) == "")
		return;

	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		if (!env2D->GetTopdownWorld(activeWorldName))
			return;
		if (!env2D->GetTopdownWorld(activeWorldName))
			return;
		if (!env2D->GetTopdownWorld(activeWorldName)->GetMap(activeMapName))
			return;

		TopdownMap* map = env2D->GetTopdownWorld(activeWorldName)->GetMap(activeMapName);
		
		map->MakeTileHard(x, y);
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		if (!env2D->GetPlatformerWorld(activeWorldName))
			return;
		if (!env2D->GetPlatformerWorld(activeWorldName))
			return;
		if (!env2D->GetPlatformerWorld(activeWorldName)->GetMap(activeMapName))
			return;

		PlatformerMap* map = env2D->GetPlatformerWorld(activeWorldName)->GetMap(activeMapName);
		
		map->MakeTileHard(x, y);
	}
}
//================================================================================================================
Tile* Environment2DMapHelper::GetSelectedTopdownTile(string activeMapName, string activeWorldName, int x, int y)
{
	if (BetterString(activeWorldName) == "")
		return NULL;
	if (BetterString(activeMapName) == "")
		return NULL;

	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return NULL;

	if (!env2D->GetTopdownWorld(activeWorldName))
		return NULL;
	if (!env2D->GetTopdownWorld(activeWorldName))
		return NULL;
	if (!env2D->GetTopdownWorld(activeWorldName)->GetMap(activeMapName))
		return NULL;

	TopdownMap* map = env2D->GetTopdownWorld(activeWorldName)->GetMap(activeMapName);

	Tile* tile = map->SelectTile(x, y);

	if (tile == NULL) return NULL;

	return tile;
}
//================================================================================================================
PlatformerTile* Environment2DMapHelper::GetSelectedPlatformerTile(string activeMapName, string activeWorldName, int x, int y)
{
	if (BetterString(activeWorldName) == "")
		return NULL;
	if (BetterString(activeMapName) == "")
		return NULL;

	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return NULL;

	if (!env2D->GetPlatformerWorld(activeWorldName))
		return NULL;
	if (!env2D->GetPlatformerWorld(activeWorldName))
		return NULL;
	if (!env2D->GetPlatformerWorld(activeWorldName)->GetMap(activeMapName))
		return NULL;

	PlatformerMap* map = env2D->GetPlatformerWorld(activeWorldName)->GetMap(activeMapName);

	PlatformerTile* tile = map->SelectTile(x, y);

	if (tile == NULL) return NULL;

	return tile;
}
//================================================================================================================
bool Environment2DMapHelper::HasMaps(string worldName)
{
	if (BetterString(worldName) == "")
		return false;

	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return false;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		if (!env2D->GetTopdownWorld(worldName))
			return false;

		return env2D->GetSWorld(worldName)->m_mapNames.size() != 0;
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		if (!env2D->GetPlatformerWorld(worldName))
			return false;

		return env2D->GetSPlatformerWorld(worldName)->m_mapNames.size() != 0;
	}
}
//================================================================================================================
void Environment2DMapHelper::GetMapWidthAttribute(string activeMapName, string activeWorldName, int& width)
{
	if (BetterString(activeWorldName) == "")
		return;
	if (BetterString(activeMapName) == "")
		return;

	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		if (!env2D->GetPlatformerWorld(activeWorldName))
			return;
		if (!env2D->GetPlatformerWorld(activeWorldName))
			return;
		if (!env2D->GetPlatformerWorld(activeWorldName)->GetMap(activeMapName))
			return;

		width = env2D->GetPlatformerWorld(activeWorldName)->GetMap(activeMapName)->Width();
	}
}
//================================================================================================================
void Environment2DMapHelper::GetMapHeightAttribute(string activeMapName, string activeWorldName, int& height)
{
	if (BetterString(activeWorldName) == "")
		return;
	if (BetterString(activeMapName) == "")
		return;

	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		if (!env2D->GetPlatformerWorld(activeWorldName))
			return;
		if (!env2D->GetPlatformerWorld(activeWorldName))
			return;
		if (!env2D->GetPlatformerWorld(activeWorldName)->GetMap(activeMapName))
			return;

		height = env2D->GetPlatformerWorld(activeWorldName)->GetMap(activeMapName)->Height();
	}
}
//================================================================================================================
void Environment2DMapHelper::ToggleMapHardness(string activeMapName, string activeWorldName, bool bEnable)
{
	if (BetterString(activeWorldName) == "")
		return;
	if (BetterString(activeMapName) == "")
		return;

	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		if (!env2D->GetTopdownWorld(activeWorldName))
			return;
		if (!env2D->GetTopdownWorld(activeWorldName))
			return;
		if (!env2D->GetTopdownWorld(activeWorldName)->GetMap(activeMapName))
			return;

		env2D->GetTopdownWorld(activeWorldName)->GetMap(activeMapName)->ToggleHardness(bEnable);
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		if (!env2D->GetPlatformerWorld(activeWorldName))
			return;
		if (!env2D->GetPlatformerWorld(activeWorldName))
			return;
		if (!env2D->GetPlatformerWorld(activeWorldName)->GetMap(activeMapName))
			return;

		env2D->GetPlatformerWorld(activeWorldName)->GetMap(activeMapName)->ToggleHardness(bEnable);
	}
}
//================================================================================================================
void Environment2DMapHelper::GetMapSizeAttribute(string activeMapName, string activeWorldName, int& size)
{
	if (BetterString(activeWorldName) == "")
		return;
	if (BetterString(activeMapName) == "")
		return;

	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		if (!env2D->GetTopdownWorld(activeWorldName))
			return;
		if (!env2D->GetTopdownWorld(activeWorldName))
			return;
		if (!env2D->GetTopdownWorld(activeWorldName)->GetMap(activeMapName))
			return;

		size = env2D->GetTopdownWorld(activeWorldName)->GetMap(activeMapName)->GetMapSize();
	}
}
//================================================================================================================
void Environment2DMapHelper::GetMapVisionAttribute(string activeMapName, string activeWorldName, int& vision)
{
	if (BetterString(activeWorldName) == "")
		return;
	if (BetterString(activeMapName) == "")
		return;

	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		if (!env2D->GetTopdownWorld(activeWorldName))
			return;
		if (!env2D->GetTopdownWorld(activeWorldName))
			return;
		if (!env2D->GetTopdownWorld(activeWorldName)->GetMap(activeMapName))
			return;

		vision = env2D->GetTopdownWorld(activeWorldName)->GetMap(activeMapName)->GetCurrentVision();
	}
}
//================================================================================================================
void Environment2DMapHelper::GetMapMusicAttribute(string activeMapName, string activeWorldName, string& music)
{
	if (BetterString(activeWorldName) == "")
		return;
	if (BetterString(activeMapName) == "")
		return;

	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		if (!env2D->GetTopdownWorld(activeWorldName))
			return;
		if (!env2D->GetTopdownWorld(activeWorldName))
			return;
		if (!env2D->GetTopdownWorld(activeWorldName)->GetMap(activeMapName))
			return;

		music = env2D->GetTopdownWorld(activeWorldName)->GetMap(activeMapName)->GetCurrentMusic();
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		if (!env2D->GetPlatformerWorld(activeWorldName))
			return;
		if (!env2D->GetPlatformerWorld(activeWorldName))
			return;
		if (!env2D->GetPlatformerWorld(activeWorldName)->GetMap(activeMapName))
			return;

		music = env2D->GetPlatformerWorld(activeWorldName)->GetMap(activeMapName)->MusicName();
	}
}
//================================================================================================================
void Environment2DMapHelper::GetMapFOWAttribute(string activeMapName, string activeWorldName, bool& fow)
{
	if (BetterString(activeWorldName) == "")
		return;
	if (BetterString(activeMapName) == "")
		return;

	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		if (!env2D->GetTopdownWorld(activeWorldName))
			return;
		if (!env2D->GetTopdownWorld(activeWorldName))
			return;
		if (!env2D->GetTopdownWorld(activeWorldName)->GetMap(activeMapName))
			return;

		fow = env2D->GetTopdownWorld(activeWorldName)->GetMap(activeMapName)->IsFOWEnabled();
	}
}
//================================================================================================================
void Environment2DMapHelper::GetMapFOWRadiusAttribute(string activeMapName, string activeWorldName, int& fowRadius)
{
	if (BetterString(activeWorldName) == "")
		return;
	if (BetterString(activeMapName) == "")
		return;

	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		if (!env2D->GetTopdownWorld(activeWorldName))
			return;
		if (!env2D->GetTopdownWorld(activeWorldName))
			return;
		if (!env2D->GetTopdownWorld(activeWorldName)->GetMap(activeMapName))
			return;

		fowRadius = env2D->GetTopdownWorld(activeWorldName)->GetMap(activeMapName)->GetFOWRadius();
	}
}
//================================================================================================================
void Environment2DMapHelper::GetMapFOWFlashlightAttribute(string activeMapName, string activeWorldName, bool& fowFlashlight)
{
	if (BetterString(activeWorldName) == "")
		return;
	if (BetterString(activeMapName) == "")
		return;

	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		if (!env2D->GetTopdownWorld(activeWorldName))
			return;
		if (!env2D->GetTopdownWorld(activeWorldName))
			return;
		if (!env2D->GetTopdownWorld(activeWorldName)->GetMap(activeMapName))
			return;

		fowFlashlight = env2D->GetTopdownWorld(activeWorldName)->GetMap(activeMapName)->IsFOWFlashlightEnabled();
	}
}
//================================================================================================================
void Environment2DMapHelper::GetNumSpritesRendered(string activeMapName, string activeWorldName, int& spritesRendered)
{
	if (BetterString(activeWorldName) == "")
		return;
	if (BetterString(activeMapName) == "")
		return;
	
	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	spritesRendered = 0;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		if (!env2D->GetTopdownWorld(activeWorldName))
			return;
		if (!env2D->GetTopdownWorld(activeWorldName))
			return;
		if (!env2D->GetTopdownWorld(activeWorldName)->GetMap(activeMapName))
			return;

		spritesRendered = env2D->GetTopdownWorld(activeWorldName)->GetMap(activeMapName)->getNumSpritesRendered();
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		if (!env2D->GetPlatformerWorld(activeWorldName))
			return;
		if (!env2D->GetPlatformerWorld(activeWorldName))
			return;
		if (!env2D->GetPlatformerWorld(activeWorldName)->GetMap(activeMapName))
			return;

		spritesRendered = env2D->GetPlatformerWorld(activeWorldName)->GetMap(activeMapName)->NumSpritesRendered();
	}
}
//================================================================================================================
int Environment2DMapHelper::GetNumSpritesInMap()
{
	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return 0;

	if (BetterString(env2D->ActiveWorldName()) == "")
		return 0;
	if (BetterString(env2D->ActiveMapName()) == "")
		return 0;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		if (!env2D->GetTopdownWorld(env2D->ActiveWorldName()))
			return 0;
		if (!env2D->GetTopdownWorld(env2D->ActiveWorldName()))
			return 0;
		if (!env2D->GetTopdownWorld(env2D->ActiveWorldName())->GetMap(env2D->ActiveMapName()))
			return 0;

		return env2D->GetTopdownWorld(env2D->ActiveWorldName())->GetMap(env2D->ActiveMapName())->getNumSpritesRendered();
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		if (!env2D->GetPlatformerWorld(env2D->ActiveWorldName()))
			return 0;
		if (!env2D->GetPlatformerWorld(env2D->ActiveWorldName()))
			return 0;
		if (!env2D->GetPlatformerWorld(env2D->ActiveWorldName())->GetMap(env2D->ActiveMapName()))
			return 0;

		return env2D->GetPlatformerWorld(env2D->ActiveWorldName())->GetMap(env2D->ActiveMapName())->NumSpritesRendered();
	}

	return 0;
}
//================================================================================================================
void Environment2DMapHelper::GetSpriteTotal(string activeMapName, string activeWorldName, int& spriteTotal)
{
	if (BetterString(activeWorldName) == "")
		return;
	if (BetterString(activeMapName) == "")
		return;

	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		if (!env2D->GetTopdownWorld(activeWorldName))
			return;
		if (!env2D->GetTopdownWorld(activeWorldName))
			return;
		if (!env2D->GetTopdownWorld(activeWorldName)->GetMap(activeMapName))
			return;

		spriteTotal = env2D->GetTopdownWorld(activeWorldName)->GetMap(activeMapName)->getMaxSprites();
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		if (!env2D->GetPlatformerWorld(activeWorldName))
			return;
		if (!env2D->GetPlatformerWorld(activeWorldName))
			return;
		if (!env2D->GetPlatformerWorld(activeWorldName)->GetMap(activeMapName))
			return;

		spriteTotal = env2D->GetPlatformerWorld(activeWorldName)->GetMap(activeMapName)->NumSprites();
	}
}
//================================================================================================================
AISprite* Environment2DMapHelper::GetSpriteInMap(int i)
{
	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return 0;

	if (BetterString(env2D->ActiveWorldName()) == "")
		return 0;
	if (BetterString(env2D->ActiveMapName()) == "")
		return 0;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		if (!env2D->GetTopdownWorld(env2D->ActiveWorldName()))
			return 0;
		if (!env2D->GetTopdownWorld(env2D->ActiveWorldName()))
			return 0;
		if (!env2D->GetTopdownWorld(env2D->ActiveWorldName())->GetMap(env2D->ActiveMapName()))
			return 0;

		return env2D->GetTopdownWorld(env2D->ActiveWorldName())->GetMap(env2D->ActiveMapName())->GetSpriteByID(i);
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		if (!env2D->GetPlatformerWorld(env2D->ActiveWorldName()))
			return 0;
		if (!env2D->GetPlatformerWorld(env2D->ActiveWorldName()))
			return 0;
		if (!env2D->GetPlatformerWorld(env2D->ActiveWorldName())->GetMap(env2D->ActiveMapName()))
			return 0;

		return env2D->GetPlatformerWorld(env2D->ActiveWorldName())->GetMap(env2D->ActiveMapName())->GetSpriteByID(i);
	}

	return 0;
}
//================================================================================================================
void Environment2DMapHelper::AddSpriteInMap(AISprite* spr)
{
	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (BetterString(env2D->ActiveWorldName()) == "")
		return;
	if (BetterString(env2D->ActiveMapName()) == "")
		return;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		if (!env2D->GetTopdownWorld(env2D->ActiveWorldName()))
			return;
		if (!env2D->GetTopdownWorld(env2D->ActiveWorldName()))
			return;
		if (!env2D->GetTopdownWorld(env2D->ActiveWorldName())->GetMap(env2D->ActiveMapName()))
			return;

		env2D->GetTopdownWorld(env2D->ActiveWorldName())->GetMap(env2D->ActiveMapName())->AddSprite(spr);
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		if (!env2D->GetPlatformerWorld(env2D->ActiveWorldName()))
			return;
		if (!env2D->GetPlatformerWorld(env2D->ActiveWorldName()))
			return;
		if (!env2D->GetPlatformerWorld(env2D->ActiveWorldName())->GetMap(env2D->ActiveMapName()))
			return;

		env2D->GetPlatformerWorld(env2D->ActiveWorldName())->GetMap(env2D->ActiveMapName())->AddSprite(spr);
	}
}
//================================================================================================================
void Environment2DMapHelper::RemoveSpriteInMap(AISprite* spr)
{
	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (BetterString(env2D->ActiveWorldName()) == "")
		return;
	if (BetterString(env2D->ActiveMapName()) == "")
		return;
	
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		if (!env2D->GetTopdownWorld(env2D->ActiveWorldName()))
			return;
		if (!env2D->GetTopdownWorld(env2D->ActiveWorldName()))
			return;
		if (!env2D->GetTopdownWorld(env2D->ActiveWorldName())->GetMap(env2D->ActiveMapName()))
			return;

		env2D->GetTopdownWorld(env2D->ActiveWorldName())->GetMap(env2D->ActiveMapName())->RemoveSprite(spr);
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		if (!env2D->GetPlatformerWorld(env2D->ActiveWorldName()))
			return;
		if (!env2D->GetPlatformerWorld(env2D->ActiveWorldName()))
			return;
		if (!env2D->GetPlatformerWorld(env2D->ActiveWorldName())->GetMap(env2D->ActiveMapName()))
			return;

		env2D->GetPlatformerWorld(env2D->ActiveWorldName())->GetMap(env2D->ActiveMapName())->RemoveSprite(spr);
	}
}
/*
//================================================================================================================
void Environment2DMapHelper::GrabWorldNames(vector<string>& names)
{
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		map<string, SWorld*>::iterator iter = m_WorldsTopdown.begin();

		for (; iter != m_WorldsTopdown.end(); iter++)
		{
			names.push_back((*iter).first);
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		map<string, SPlatformerWorld*>::iterator iter = m_WorldsPlatformer.begin();

		for (; iter != m_WorldsPlatformer.end(); iter++)
		{
			names.push_back((*iter).first);
		}
	}
}
//================================================================================================================
void Environment2DMapHelper::GrabMapNames(string world_name, vector<string>& names)
{
	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		map<string, SWorld*>::iterator iter = m_WorldsTopdown.begin();

		for (; iter != m_WorldsTopdown.end(); iter++)
		{
			if ((*iter).first == world_name)
			{
				for (int i = 0; i < (*iter).second->m_mapNames.size(); i++)
					names.push_back((*iter).second->m_mapNames[i]);
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		map<string, SPlatformerWorld*>::iterator iter = m_WorldsPlatformer.begin();

		for (; iter != m_WorldsPlatformer.end(); iter++)
		{
			if ((*iter).first == world_name)
			{
				for (int i = 0; i < (*iter).second->m_mapNames.size(); i++)
					names.push_back((*iter).second->m_mapNames[i]);
			}
		}
	}
}
*/
//================================================================================================================
//void Environment2DMapHelper::GrabMenuNames(vector<string>& names)
//{
//	m_MenuSystem->GetMenuNames(names);
//}
//================================================================================================================
void Environment2DMapHelper::LoadWorld(string& worldName, string filename, bool InEditor, HWND hwndPB)
{
	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		SWorld* sw = NULL;

		if (!CGlobal::DoesFileExist(filename))
		{
			string path = env2D->GetGD2D()->m_xml_worlds_path;
			path.append("\\");
			path.append(filename);
			path.append(".xml");
			filename = path;
		}

		ZShadeXMLLoader::LoadWorldXML(filename, sw);
		
		env2D->LoadWorld(sw->sWorldName, sw);

		worldName = sw->sWorldName;
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		SPlatformerWorld* sw = NULL;

		if (!CGlobal::DoesFileExist(filename))
		{
			string path = env2D->GetGD2D()->m_xml_worlds_path;
			path.append("\\");
			path.append(filename);
			path.append(".xml");
			filename = path;
		}

		ZShadeXMLLoader::LoadWorldXML(filename, sw);

		env2D->LoadWorld(sw->sWorldName, sw);

		worldName = sw->sWorldName;
	}

	// Load all the maps in the world if they exist
	Environment2DMapHelper::LoadWorld(worldName, InEditor, hwndPB);
}
//================================================================================================================
void Environment2DMapHelper::SaveWorld(string activeWorldName, string filename)
{
	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
		ZShadeXMLSaver::SaveWorldXML(env2D->GetSWorld(activeWorldName), filename);
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
		ZShadeXMLSaver::SaveWorldXML(env2D->GetSPlatformerWorld(activeWorldName), filename);
}
//================================================================================================================
void Environment2DMapHelper::LoadMap(string& mapName, string activeWorldName, string filename, bool InEditor, HWND hwndPB)
{
	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		if (env2D->GetTopdownWorld(activeWorldName) != NULL)
			env2D->GetTopdownWorld(activeWorldName)->LoadMap(mapName, filename,
				env2D->GetGD2D(), env2D->GetCamera(), InEditor, hwndPB);
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		if (env2D->GetPlatformerWorld(activeWorldName) != NULL)
			env2D->GetPlatformerWorld(activeWorldName)->LoadMap(mapName, filename,
				env2D->GetGD2D(), env2D->GetCamera(), InEditor, hwndPB);
	}
}
//================================================================================================================
void Environment2DMapHelper::SaveMap(string activeMapName, string activeWorldName, string filename, HWND hwndPB)
{
	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
		ZShadeXMLSaver::SaveMapXML(env2D->GetTopdownWorld(activeWorldName)->GetMap(activeMapName), filename, env2D->GetGD2D(), hwndPB);
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
		ZShadeXMLSaver::SaveMapXML(env2D->GetPlatformerWorld(activeWorldName)->GetMap(activeMapName), filename, env2D->GetGD2D(), hwndPB);
}
/*
//================================================================================================================
void Environment2DMapHelper::LoadAMenu(string menu_filename, string& menuName)
{
	MenuLoader ml;
	ml.LoadXMLMenu(menu_filename,
		env2D->GetGD2D(), env2D->GetEngineOptions(), env2D->GetD3DSystem(),
		env2D->GetWindow()->GetHwnd(), env2D->GetMenuSystem());
}
//================================================================================================================
void Environment2DMapHelper::SaveMenu(string menu_filename)
{
	MenuWriter mw;
	mw.SaveXMLMenu(menu_filename, m_MenuSystem);
}
//================================================================================================================
void Environment2DMapHelper::SetActiveRenderedMenu(string menu_name)
{
	m_MenuSystem->SetMenuToRender(menu_name);
}
//================================================================================================================
void Environment2DMapHelper::RemoveMenu(string menu_name)
{
	m_MenuSystem->RemoveMenu(menu_name);
}
*/
//================================================================================================================
void Environment2DMapHelper::CreateMapTopdown(int x, int y, bool fow, int fowRadius, bool fowFlashlight, string world_name, string name, bool bIso, bool InEditor, HWND hwndPB)
{
	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	env2D->GetEngineOptions()->SECTION_ROW_SIZE = x;
	env2D->GetEngineOptions()->SECTION_COL_SIZE = y;
	env2D->GetEngineOptions()->SetTileMax();

	//if (fow == true)
	//{
	//	env2D->GetEngineOptions()->m_fow = fow;
	//	env2D->GetEngineOptions()->m_fow_radius = fowRadius;
	//}

	Map2DType mt;

	if (bIso)
		mt = Map2DType::ISOMETRIC;
	else
		mt = Map2DType::REGULAR;

	Environment2DMapHelper::AddMapName(world_name, name);

	Environment2DMapHelper::NewMapTopdown(world_name, name, mt, InEditor, hwndPB);

	env2D->GetTopdownWorld(world_name)->GetMap(name)->SetMapSize(x);

	// Set the fog attributes
	env2D->GetTopdownWorld(world_name)->GetMap(name)->EnableFOW(fow);
	env2D->GetTopdownWorld(world_name)->GetMap(name)->SetFOWRadius(fowRadius);
	env2D->GetTopdownWorld(world_name)->GetMap(name)->EnableFOWFlashlight(fowFlashlight);
}
//================================================================================================================
void Environment2DMapHelper::CreateMapPlatformer(int x, int y, string world_name, string name, bool InEditor, HWND hwndPB)
{
	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	env2D->GetEngineOptions()->SECTION_ROW_SIZE = x;
	env2D->GetEngineOptions()->SECTION_COL_SIZE = y;
	env2D->GetEngineOptions()->SetTileMax();

	Environment2DMapHelper::AddMapName(world_name, name);

	Environment2DMapHelper::NewMapPlatformer(world_name, name, InEditor, hwndPB);

	env2D->GetPlatformerWorld(world_name)->GetMap(name)->Width() = x;
	env2D->GetPlatformerWorld(world_name)->GetMap(name)->Height() = y;
}
//================================================================================================================
void Environment2DMapHelper::NewMapTopdown(string world_name, string name, Map2DType mt, bool InEditor, HWND hwndPB)
{
	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	//Get a copy of the world if it exists
	if (env2D->GetTopdownWorld(world_name) != NULL)
	{
		SWorld* sw = env2D->GetSWorld(world_name);

		if (sw == NULL) return;

		// If the world does not exist then create it
		World* world = sw->m_World;
		
		if (world == NULL)
		{
			world = new World(env2D->GetD3DSystem());

			sw->m_World = world;
		}

		world->SetD3DSystem(env2D->GetD3DSystem());

		sw->m_World->NewMap(name, env2D->GetGD2D(), mt, env2D->GetCamera(), InEditor, hwndPB);

		sw->m_World->GetMap(name)->SetMapName(name);
	}
}
//================================================================================================================
void Environment2DMapHelper::NewMapPlatformer(string world_name, string name, bool InEditor, HWND hwndPB)
{
	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	//Get a copy of the world if it exists
	if (env2D->GetPlatformerWorld(world_name) != NULL)
	{
		SPlatformerWorld* sw = env2D->GetSPlatformerWorld(world_name);

		if (sw == NULL) return;

		// If the world does not exist then create it
		PlatformerWorld* world = sw->m_World;

		if (world == NULL)
		{
			world = new PlatformerWorld(env2D->GetD3DSystem());

			sw->m_World = world;
		}

		world->SetD3DSystem(env2D->GetD3DSystem());

		sw->m_World->NewMap(name, env2D->GetGD2D(), env2D->GetCamera(), InEditor, hwndPB);

		sw->m_World->GetMap(name)->MapName() = name;
	}
}
//================================================================================================================
void Environment2DMapHelper::AddMapName(string world_name, string name)
{
	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		SWorld* sw;

		// Create a new world
		if (env2D->GetSWorld(world_name) == NULL)
		{
			sw = new SWorld();
		}
		else//Get the world
		{
			sw = env2D->GetSWorld(world_name);
		}

		if (sw == NULL) return;

		sw->m_mapNames.push_back(name);

		//m_Worlds[world_name] = sw;
		//m_WorldsTopdown[world_name] = sw;
		env2D->LoadWorld(world_name, sw);
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		SPlatformerWorld* sw;

		// Create a new world
		if (env2D->GetSPlatformerWorld(world_name) == NULL)
		{
			sw = new SPlatformerWorld();
		}
		else//Get the world
		{
			sw = env2D->GetSPlatformerWorld(world_name);
		}

		if (sw == NULL) return;

		sw->m_mapNames.push_back(name);

		//m_WorldsPlatformer[world_name] = sw;
		env2D->LoadWorld(world_name, sw);
	}
}
//================================================================================================================
void Environment2DMapHelper::LoadWorld(string world_name, bool InEditor, HWND hwndPB)
{
	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	string mapName;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		//Get a copy of the world if it exists
		if (env2D->GetSWorld(world_name) != NULL)
		{
			SWorld* sw = env2D->GetSWorld(world_name);

			if (sw == NULL) return;

			sw->m_World = new World(env2D->GetD3DSystem());

			for (int i = 0; i < sw->m_mapNames.size(); i++)
			{
				string name = sw->m_mapNames[i];

				sw->m_World->LoadMap(name, name, env2D->GetGD2D(), env2D->GetCamera(), InEditor, hwndPB);
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		//Get a copy of the world if it exists
		if (env2D->GetSPlatformerWorld(world_name) != NULL)
		{
			SPlatformerWorld* sw = env2D->GetSPlatformerWorld(world_name);

			if (sw == NULL) return;

			sw->m_World = new PlatformerWorld(env2D->GetD3DSystem());

			for (int i = 0; i < sw->m_mapNames.size(); i++)
			{
				string name = sw->m_mapNames[i];

				sw->m_World->LoadMap(name, name, env2D->GetGD2D(), env2D->GetCamera(), InEditor, hwndPB);
			}
		}
	}
}
//================================================================================================================
void Environment2DMapHelper::LoadMapInWorld(string world_name, string name, Map2DType mt, bool InEditor, HWND hwndPB)
{
	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	//string mapName;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		//Get a copy of the world if it exists
		if (env2D->GetSWorld(world_name) != NULL)
		{
			SWorld* sw = env2D->GetSWorld(world_name);

			if (sw == NULL) return;

			// If the world does not exist then create it
			if (sw->m_World == NULL)
				sw->m_World = new World(env2D->GetD3DSystem());

			for (int i = 0; i < sw->m_mapNames.size(); i++)
			{
				string map_name = sw->m_mapNames[i];

				// Make sure it is a valid map
				if (map_name == name)
				{
					sw->m_World->LoadMap(map_name, name, env2D->GetGD2D(), env2D->GetCamera(), InEditor, hwndPB);

					break;
				}
			}
		}
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		//Get a copy of the world if it exists
		if (env2D->GetSPlatformerWorld(world_name) != NULL)
		{
			SPlatformerWorld* sw = env2D->GetSPlatformerWorld(world_name);

			if (sw == NULL) return;

			// If the world does not exist then create it
			if (sw->m_World == NULL)
				sw->m_World = new PlatformerWorld(env2D->GetD3DSystem());

			for (int i = 0; i < sw->m_mapNames.size(); i++)
			{
				string map_name = sw->m_mapNames[i];

				// Make sure it is a valid map
				if (map_name == name)
				{
					sw->m_World->LoadMap(map_name, name, env2D->GetGD2D(), env2D->GetCamera(), InEditor, hwndPB);

					break;
				}
			}
		}
	}
}
//================================================================================================================
/*
World* Environment2DMapHelper::GetTopdownWorld(string world_name)
{
	SWorld* sw = NULL;

	//Get a copy of the world if it exists
	if (m_WorldsTopdown[world_name] != NULL)
	{
		sw = m_WorldsTopdown[world_name];
	}

	if (sw == NULL) return NULL;

	return sw->m_World;
}
//================================================================================================================
PlatformerWorld* Environment2DMapHelper::GetPlatformerWorld(string world_name)
{
	SPlatformerWorld* sw = NULL;

	//Get a copy of the world if it exists
	if (m_WorldsPlatformer[world_name] != NULL)
	{
		sw = m_WorldsPlatformer[world_name];
	}

	if (sw == NULL) return NULL;

	return sw->m_World;
}*/
//================================================================================================================
void Environment2DMapHelper::CreateWorld(string world_name)
{
	Environment2D* env2D = EnvironmentFactory::Instance()->SelectEnvironment(EnvironmentFactory::Instance()->ActiveEnvironmentName())->Env2D();

	// Did not find an environment
	if (env2D == 0)
		return;

	if (env2D->GetEngineOptions()->m_GameType2D == TOPDOWN)
	{
		SWorld* sw = new SWorld();

		sw->sWorldName = world_name;

		World* world = new World(env2D->GetD3DSystem());

		sw->m_World = world;

		env2D->LoadWorld(world_name, sw);
	}
	else if (env2D->GetEngineOptions()->m_GameType2D == PLATFORMER)
	{
		SPlatformerWorld* sw = new SPlatformerWorld();

		sw->sWorldName = world_name;

		PlatformerWorld* world = new PlatformerWorld(env2D->GetD3DSystem());

		sw->m_World = world;

		env2D->LoadWorld(world_name, sw);
	}
}
//================================================================================================================