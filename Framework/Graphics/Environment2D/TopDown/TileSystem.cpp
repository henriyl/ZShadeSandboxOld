#include "TileSystem.h"
#include "FogData.h"
//-----------------------------------------------------------------------------------------------------------------
TileSystem::TileSystem(D3D* d3d)
:	m_D3DSystem(d3d),
	m_bFOW(false),
	m_iFOWRadius(5),
	m_bFOWFlashlight(false)
{
	m_engineOptions = d3d->GetEngineOptions();
}
//-----------------------------------------------------------------------------------------------------------------
TileSystem::TileSystem(const TileSystem& other)
{
}
//-----------------------------------------------------------------------------------------------------------------
TileSystem::~TileSystem()
{
}
//-----------------------------------------------------------------------------------------------------------------
bool TileSystem::Initialize()
{
	m_Sections.resize(m_engineOptions->TILE_SECTION_MAX);
	return true;
}
//-----------------------------------------------------------------------------------------------------------------
void TileSystem::UpdateTileHardness(int x, int y, bool bHard)
{
	for (int i = 0; i < m_Sections.size(); i++)
	{
		if (m_Sections[i])
		{
			for (int sy = 0; sy < m_engineOptions->COL_SIZE; sy++)
			{
				for (int sx = 0; sx < m_engineOptions->ROW_SIZE; sx++)
				{
					Tile* tile = m_Sections[i]->GetTile(sx, sy);

					if (tile->GetPosition().x == x && tile->GetPosition().y == y)
					{
						tile->SetHard(bHard);

						return;
					}
				}
			}
		}
	}
}
//-----------------------------------------------------------------------------------------------------------------
void TileSystem::MakeTileHard(int x, int y)
{
	for (int i = 0; i < m_Sections.size(); i++)
	{
		if (m_Sections[i])
		{
			for (int sy = 0; sy < m_engineOptions->COL_SIZE; sy++)
			{
				for (int sx = 0; sx < m_engineOptions->ROW_SIZE; sx++)
				{
					Tile* tile = m_Sections[i]->GetTile(sx, sy);

					if (tile->GetPosition().x == x && tile->GetPosition().y == y)
					{
						if (tile->IsHard())
							tile->SetHard(false);
						else
							tile->SetHard(true);

						return;
					}
				}
			}
		}
	}
}
//-----------------------------------------------------------------------------------------------------------------
void TileSystem::EnableFOW(bool bEnable)
{
	m_bFOW = bEnable;

	for (int i = 0; i < m_Sections.size(); i++)
	{
		if (m_Sections[i])
		{
			m_Sections[i]->EnableFOW(bEnable);

			for (int sy = 0; sy < m_engineOptions->COL_SIZE; sy++)
			{
				for (int sx = 0; sx < m_engineOptions->ROW_SIZE; sx++)
				{
					Tile* tile = m_Sections[i]->GetTile(sx, sy);

					tile->EnableFOW(bEnable);
				}
			}
		}
	}
}
//-----------------------------------------------------------------------------------------------------------------
void TileSystem::SetFOWRadius(int bRadius)
{
	for (int i = 0; i < m_Sections.size(); i++)
	{
		if (m_Sections[i])
		{
			m_Sections[i]->SetFOWRadius(bRadius);

			for (int sy = 0; sy < m_engineOptions->COL_SIZE; sy++)
			{
				for (int sx = 0; sx < m_engineOptions->ROW_SIZE; sx++)
				{
					Tile* tile = m_Sections[i]->GetTile(sx, sy);

					tile->SetFOWRadius(bRadius);
				}
			}
		}
	}
}
//-----------------------------------------------------------------------------------------------------------------
void TileSystem::EnableFOWFlashlight(bool bEnable)
{
	m_bFOWFlashlight = bEnable;

	for (int i = 0; i < m_Sections.size(); i++)
	{
		if (m_Sections[i])
		{
			m_Sections[i]->EnableFOWFlashlight(bEnable);

			for (int sy = 0; sy < m_engineOptions->COL_SIZE; sy++)
			{
				for (int sx = 0; sx < m_engineOptions->ROW_SIZE; sx++)
				{
					Tile* tile = m_Sections[i]->GetTile(sx, sy);

					tile->EnableFOWFlashlight(bEnable);
				}
			}
		}
	}
}
//-----------------------------------------------------------------------------------------------------------------
bool TileSystem::CreateTileSection(int id, int x, int y, int size, bool hard, int vision, string musicName,
		XMFLOAT3 pos, GameDirectory2D* gd, string textureFilename, bool init, string animProfileName, int animSpeed, int bitmapWidth, int bitmapHeight)
{
	Section* section = NULL;

	if (m_Sections[id] == NULL)
	{
		section = new Section(m_D3DSystem);

		section->Initialize(id);

		section->SetVision(vision);
		section->SetMusicName(musicName);
	}
	else//Get the section with the id
	{
		section = m_Sections[id];
	}

	section->CreateTile(x, y, size, hard, pos, gd, textureFilename, init, animProfileName, animSpeed, bitmapWidth, bitmapHeight);

	m_Sections[id] = section;
	return true;
}
//-----------------------------------------------------------------------------------------------------------------
void TileSystem::SetTileDisplayHardbox(bool visible)
{
	for (int i = 0; i < m_Sections.size(); i++)
	{
		Section* sec = m_Sections[i];
		sec->SetTileDisplayHardbox(visible);
	}
}
//-----------------------------------------------------------------------------------------------------------------
void TileSystem::InitializeOverworldTiles(int x, int y, int size, string base_tile_path, string textureFilename, int bitmapWidth, int bitmapHeight)
{
	Tile*** pTiles;

	pTiles = new Tile**[x];
	for (int i = 0; i < x; i++)
	{
		pTiles[i] = new Tile*[y];
	}

	//Initialize the overworld tiles

	for (int j = 0; j < y; j++)
	{
		for (int i = 0; i < x; i++)
		{
			XMFLOAT3 pos;

			int xs = i * m_engineOptions->TILE_SIZE / 2;
			int ys = j * m_engineOptions->TILE_SIZE / 2;

			//xs = xs + (secCol * (m_engineOptions->ROW_SIZE * m_engineOptions->TILE_SIZE));
			//ys = ys + (secRow * (m_engineOptions->COL_SIZE * m_engineOptions->TILE_SIZE));

			pTiles[i][j] = new Tile(m_D3DSystem);
			pTiles[i][j]->Initialize(size, false, XMFLOAT3(xs, ys, 0));
			pTiles[i][j]->InitializeTexture("Render", base_tile_path, textureFilename, bitmapWidth, bitmapHeight);

			m_OverworldTiles.push_back( pTiles[i][j] );
		}
	}
}
//-----------------------------------------------------------------------------------------------------------------
void TileSystem::RenderOverworldTiles(Camera* camera)
{
	for (int i = 0; i < m_OverworldTiles.size(); i++)
	{
		m_OverworldTiles[i]->Render(camera);
	}
}
//-----------------------------------------------------------------------------------------------------------------
void TileSystem::ReloadSprites(string base_tile_path)
{
	//Reset the map tiles
	for (int i = 0; i < m_Sections.size(); i++)
	{
		Section* sec = m_Sections[i];
		sec->ReloadSprites(base_tile_path);
	}

	//Reset the overworld tiles
	for (int i = 0; i < m_OverworldTiles.size(); i++)
	{
		Tile* t = m_OverworldTiles[i];
		XMFLOAT3 pos = t->GetPosition();
		bool hard = t->IsHard();
		int size = t->GetSize();
		string textureFilename = t->GetTileTextureName();
		int bitmapWidth = t->GetWidth();
		int bitmapHeight = t->GetHeight();
		t->Initialize(size, hard, pos);
		t->InitializeTexture("Render", base_tile_path, textureFilename, 32, 32);
	}
}
//-----------------------------------------------------------------------------------------------------------------
void TileSystem::MoveTiles(float speed, int dirUD, int dirLR)
{
	for (int i = 0; i < m_Sections.size(); i++)
	{
		Section* section = m_Sections[i];

		section->MoveTiles(speed, dirUD, dirLR);
	}
}
//-----------------------------------------------------------------------------------------------------------------
Section* TileSystem::SelectSection(int x, int y)
{
	for (int i = 0; i < m_Sections.size(); i++)
	{
		Section* section = m_Sections[i];

		if (section->InBounds(x, y))
		{
			return section;
		}
	}

	return NULL;
}
//-----------------------------------------------------------------------------------------------------------------
Tile* TileSystem::SelectTile(float x, float y)
{
	Section* sec = SelectSection(x, y);
	if (sec != NULL)
	{
		return sec->SelectTile(x, y);
	}
	
	return NULL;
}
//-----------------------------------------------------------------------------------------------------------------
void TileSystem::SetNewTexture(int id, string base_tile_path, string textureFilename, int bitmapWidth, int bitmapHeight)
{
	if (m_Sections[id])
		m_Sections[id]->SetNewTexture(base_tile_path, textureFilename, bitmapWidth, bitmapHeight);
}
//-----------------------------------------------------------------------------------------------------------------
void TileSystem::Shutdown()
{
	for (int i = 0; i < m_Sections.size(); i++)
	{
		if (m_Sections[i])
			m_Sections[i]->Shutdown();
	}
}
//-----------------------------------------------------------------------------------------------------------------
XMFLOAT3 TileSystem::GetTilePos(int id, int x, int y)
{
	if (m_Sections[id])
		return m_Sections[id]->GetTilePos(x, y);
	else
		return XMFLOAT3(0,0,0);
}
//-----------------------------------------------------------------------------------------------------------------
void TileSystem::UpdatePosition(int id, int x, int y, XMFLOAT3 pos)
{
	if (m_Sections[id])
		m_Sections[id]->UpdatePosition(x, y, pos);
}
//-----------------------------------------------------------------------------------------------------------------
Section* TileSystem::GetSection(int id)
{
	if (m_Sections[id])
		return m_Sections[id];
	return NULL;
}
//-----------------------------------------------------------------------------------------------------------------
bool TileSystem::SectionExists(int dir)
{
	if (dir < 0 || dir > m_Sections.size() - 1) return false;

	if (!m_Sections[dir]) return false;

	return true;
}
//-----------------------------------------------------------------------------------------------------------------
vector<Section*> TileSystem::GetSections()
{
	return m_Sections;
}
//-----------------------------------------------------------------------------------------------------------------
void TileSystem::UpdateTileTexture(int x, int y, string base_tile_path, string textureFilename, int bitmapWidth, int bitmapHeight)
{
	for (int i = 0; i < m_Sections.size(); i++)
	{
		if (m_Sections[i])
		{
			for (int sy = 0; sy < m_engineOptions->COL_SIZE; sy++)
			{
				for (int sx = 0; sx < m_engineOptions->ROW_SIZE; sx++)
				{
					Tile* tile = m_Sections[i]->GetTile(sx, sy);

					XMFLOAT3 tilePos = tile->GetPosition();

					if (tilePos.x == x && tilePos.y == y)
					{
						m_Sections[i]->UpdateTileTexture(sx, sy, base_tile_path, textureFilename, bitmapWidth, bitmapHeight);
					}
				}
			}
		}
	}
}
//-----------------------------------------------------------------------------------------------------------------
Tile* TileSystem::GetTile(int x, int y)
{
	for (int i = 0; i < m_Sections.size(); i++)
	{
		if (m_Sections[i])
		{
			for (int sy = 0; sy < m_engineOptions->COL_SIZE; sy++)
			{
				for (int sx = 0; sx < m_engineOptions->ROW_SIZE; sx++)
				{
					Tile* tile = m_Sections[i]->GetTile(sx, sy);

					XMFLOAT3 tilePos = tile->GetPosition();

					if (tilePos.x == x && tilePos.y == y)
					{
						return tile;
					}
				}
			}
		}
	}

	return NULL;
}
//-----------------------------------------------------------------------------------------------------------------
bool TileSystem::IsPosInTile(XMFLOAT2 pos, XMFLOAT2 &r, bool pInSectionPicker, vector<int> sectionRenderList)
{
	if (pInSectionPicker)
	{
		for (int i = 0; i < m_OverworldTiles.size(); i++)
		{
			if (m_OverworldTiles[i])
			{
				for (int y = 0; y < m_engineOptions->SECTION_COL_SIZE; y++)
				{
					for (int x = 0; x < m_engineOptions->SECTION_ROW_SIZE; x++)
					{
						Tile* tile = m_OverworldTiles[i];

						XMFLOAT3 tilePos = tile->GetPosition();

						if ((pos.x >= tilePos.x && pos.x <= tilePos.x + (m_engineOptions->TILE_SIZE / 2)) && (pos.y >= tilePos.y && pos.y <= tilePos.y + (m_engineOptions->TILE_SIZE / 2)))
						{
							r.x = tilePos.x;
							r.y = tilePos.y;

							return true;
						}
					}
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < m_Sections.size(); i++)
		{
			if (m_Sections[i])
			{
				for (int y = 0; y < m_engineOptions->COL_SIZE; y++)
				{
					for (int x = 0; x < m_engineOptions->ROW_SIZE; x++)
					{
						Tile* tile = m_Sections[i]->GetTile(x, y);

						XMFLOAT3 tilePos = tile->GetPosition();

						if (pos.x > tilePos.x && pos.x < tilePos.x + m_engineOptions->TILE_SIZE &&
							pos.y > tilePos.y && pos.y < tilePos.y + m_engineOptions->TILE_SIZE)
						{
							r.x = tilePos.x;
							r.y = tilePos.y;

							return true;
						}
					}
				}
			}
		}
	}

	r.x = -1;
	r.y = -1;

	return false;
}
//-----------------------------------------------------------------------------------------------------------------
void TileSystem::FowToggled(vector<int> sectionRenderList)
{
	//Each time fow is toggled, need to look at the map to set the tile bits to either fow_non or fow_all

	//for (int i = 0; i < sectionRenderList.size(); i++)
	for (int i = 0; i < m_Sections.size(); i++)
	{
		if (m_Sections[i])
		{
			if (m_bFOW)
			{
				m_Sections[i]->UpdateFogBit(fow_all);
			}
			else
			{
				m_Sections[i]->UpdateFogBit(fow_non);
			}

			m_Sections[i]->UpdateTexture();
		}
	}
}
//-----------------------------------------------------------------------------------------------------------------
void TileSystem::RenderTopFog(vector<int> sectionRenderList, Camera* camera)
{
	/*for (int i = 0; i < m_FogTiles.size(); i++)
	{
		Tile* tile = m_FogTiles[i];

		tile->Render(textureShader, transparentShader, camera);
	}*/

	for (int i = 0; i < sectionRenderList.size(); i++)
	{
		int sec = sectionRenderList[i];
		if (m_Sections[sec])
		{
			for (int y = 0; y < m_engineOptions->COL_SIZE; y++)
			{
				for (int x = 0; x < m_engineOptions->ROW_SIZE; x++)
				{
					Tile* tile = m_Sections[sec]->GetTile(x, y);

					if (tile->GetFogBit() != fow_non && tile->GetFogBit() == fow_all)
					{
						//m_FogTiles.push_back(tile);
						tile->Render(camera);
					}
				}
			}
		}
	}
}
//-----------------------------------------------------------------------------------------------------------------
void TileSystem::AssessFog(vector<int> sectionRenderList)
{
	/*if (m_FogTiles.size() > 0) m_FogTiles.clear();

	for (int i = 0; i < sectionRenderList.size(); i++)
	{
		int sec = sectionRenderList[i];
		if (m_Sections[sec])
		{
			for (int y = 0; y < m_engineOptions->COL_SIZE; y++)
			{
				for (int x = 0; x < m_engineOptions->ROW_SIZE; x++)
				{
					Tile* tile = m_Sections[sec]->GetTile(x, y);

					if (tile->GetFogBit() != fow_non)
					{
						m_FogTiles.push_back( tile );
					}
				}
			}
		}
	}*/
}
//-----------------------------------------------------------------------------------------------------------------
Tile* TileSystem::InitPlayerFOWInSection(Section* section, XMFLOAT3 realpos, unsigned short mask)
{
	if (section->InBounds(realpos.x, realpos.y))
	{
		XMFLOAT3 p_pos = section->GetTile(0, 0)->GetPosition();

		float xDiff = realpos.x - p_pos.x;
		float yDiff = realpos.y - p_pos.y;

		if (!m_bFOWFlashlight)
		{
			if (section->GetTile(xDiff, yDiff)->GetFogBit() != fow_non)
			{
				section->GetTile(xDiff, yDiff)->UpdateFogBit(mask);
				return 0;
			}
		}
		else//Update every bit since we are using flashlight based fog
		{
			section->GetTile(xDiff, yDiff)->UpdateFogBit(mask);
			return 0;
		}
	}
	/*Tile* rettile = NULL;

	bool breakOut = false;
	for (int yyy = 0; yyy < m_engineOptions->COL_SIZE; yyy++)
	{
		for (int xxx = 0; xxx < m_engineOptions->ROW_SIZE; xxx++)
		{
			Tile* tile2 = section->GetTile(xxx, yyy);

			if (realpos.x == tile2->GetPosition().x && realpos.y == tile2->GetPosition().y)
			{
				rettile = tile2;

				breakOut = true;
				break;
			}
		}

		if (breakOut) break;
	}*/

	return 0;
}
//-----------------------------------------------------------------------------------------------------------------
void TileSystem::UpdateFog(vector<int> sectionRenderList, Sprite* player, int radius)
{
	//return;

	//Create a window area based on the fow circle
	if (m_bFOWFlashlight)
		FowToggled(sectionRenderList);

	//The fog circle will stay around the players position
	const int length = (radius * 2) + 1;
	const unsigned short* mask = &circle_mask[radius - FOW_RADIUS_MIN][0];

	Sprite* sprite = player;

	float playerX = sprite->TopLeftPosition().x;
	float playerY = sprite->TopLeftPosition().y;

	sprite->InitFOWTiles(length);

	//Figure out what tile the player is within
	Section* section_in = sprite->SectionIn();
	if (section_in == NULL) return;

	SectionAdjacency* sa = section_in->AdjacencyInfo();
	if (sa == NULL) return;

	Section* northSection = NULL;
	Section* southSection = NULL;
	Section* eastSection = NULL;
	Section* westSection = NULL;
	Section* NWSection = NULL;
	Section* NESection = NULL;
	Section* SWSection = NULL;
	Section* SESection = NULL;

	for (int i = 0; i < sa->adjSections.size(); i++)
	{
		AdjacentSection as = sa->adjSections[i];

		if (as.neighborPos == NORTH)
		{
			northSection = m_Sections[as.ID];
		}
		if (as.neighborPos == SOUTH)
		{
			southSection = m_Sections[as.ID];
		}
		if (as.neighborPos == EAST)
		{
			eastSection = m_Sections[as.ID];
		}
		if (as.neighborPos == WEST)
		{
			westSection = m_Sections[as.ID];
		}
		if (as.neighborPos == NW)
		{
			NWSection = m_Sections[as.ID];
		}
		if (as.neighborPos == NE)
		{
			NESection = m_Sections[as.ID];
		}
		if (as.neighborPos == SW)
		{
			SWSection = m_Sections[as.ID];
		}
		if (as.neighborPos == SE)
		{
			SESection = m_Sections[as.ID];
		}
	}

	bool breakOut = false;
	Tile* playerTile = NULL;
	for (int y = 0; y < m_engineOptions->COL_SIZE; y++)
	{
		for (int x = 0; x < m_engineOptions->ROW_SIZE; x++)
		{
			Tile* tile = section_in->GetTile(x, y);

			XMFLOAT3 tilePos = tile->GetPosition();

			// Found the tile the player is in, Fan the circle out from this tile
			if (playerX >= (tilePos.x - m_engineOptions->TILE_SIZE) && playerX <= (tilePos.x + m_engineOptions->TILE_SIZE) &&
				playerY >= (tilePos.y - m_engineOptions->TILE_SIZE) && playerY <= (tilePos.y + m_engineOptions->TILE_SIZE))
			{
				if (x + 1 < m_engineOptions->ROW_SIZE && y + 1 < m_engineOptions->COL_SIZE)
					playerTile = section_in->GetTile(x + 1, y + 1);
				else
					playerTile = tile;
				breakOut = true;
				break;
			}
		}

		if (breakOut) break;
	}

	if (playerTile != NULL)
	{
		//spr->AddFOWTile(m_engineOptions->m_fow_radius, m_engineOptions->m_fow_radius, playerTile);

		float xTo = (playerTile->GetPosition().x + (m_iFOWRadius * m_engineOptions->TILE_SIZE));
		float xBack = (playerTile->GetPosition().x - (m_iFOWRadius * m_engineOptions->TILE_SIZE));

		float yTo = (playerTile->GetPosition().y + (m_iFOWRadius * m_engineOptions->TILE_SIZE));
		float yBack = (playerTile->GetPosition().y - (m_iFOWRadius * m_engineOptions->TILE_SIZE));

		float yy = yBack;
		float xx = xBack;

		int fowY = 0;
		int fowX = 0;

		XMFLOAT2 realSpriteTilePos(playerTile->GetPosition().x, playerTile->GetPosition().y);

		for (fowY = 0, yy = yBack; fowY < length, yy <= yTo; fowY++, yy += m_engineOptions->TILE_SIZE)
		{
			for (fowX = 0, xx = xBack; fowX < length, xx <= xTo; fowX++, xx += m_engineOptions->TILE_SIZE)
			{
				XMFLOAT2 realTilePos(xx, yy);

				bool found = UpdateSectionFogMask(section_in, realTilePos, realSpriteTilePos, sprite, *mask);

				if (!found && northSection != NULL)
				{
					found = UpdateSectionFogMask(northSection, realTilePos, realSpriteTilePos, sprite, *mask);
				}

				if (!found && southSection != NULL)
				{
					found = UpdateSectionFogMask(southSection, realTilePos, realSpriteTilePos, sprite, *mask);
				}

				if (!found && eastSection != NULL)
				{
					found = UpdateSectionFogMask(eastSection, realTilePos, realSpriteTilePos, sprite, *mask);
				}

				if (!found && westSection != NULL)
				{
					found = UpdateSectionFogMask(westSection, realTilePos, realSpriteTilePos, sprite, *mask);
				}

				if (!found && NWSection != NULL)
				{
					found = UpdateSectionFogMask(NWSection, realTilePos, realSpriteTilePos, sprite, *mask);
				}

				if (!found && NESection != NULL)
				{
					found = UpdateSectionFogMask(NESection, realTilePos, realSpriteTilePos, sprite, *mask);
				}

				if (!found && SWSection != NULL)
				{
					found = UpdateSectionFogMask(SWSection, realTilePos, realSpriteTilePos, sprite, *mask);
				}

				if (!found && SESection != NULL)
				{
					found = UpdateSectionFogMask(SESection, realTilePos, realSpriteTilePos, sprite, *mask);
				}

				mask++;
			}
		}
	}
	
	//for (int i = 0; i < player->FOWTileSize(); i++)
	//{
	//	Tile* tile = player->GetFOWTile(i);

	//	if (tile != NULL)
	//	{
	//		if (!m_engineOptions->m_fow_flashlight)
	//		{
	//			if (tile->GetFogBit() != fow_non)
	//			{
	//				tile->UpdateFogBit(*mask);
	//			}
	//		}
	//		else//Update every bit since we are using flashlight based fog
	//		{
	//			tile->UpdateFogBit(*mask);
	//		}
	//	}

	//	mask++;
	//}

	/*for (int y = 0; y < player->FOWTilesHeight(); y++)
	{
		for (int x = 0; x < player->FOWTilesWidth(); x++)
		{
			Tile* tile = player->GetFOWTile(x, y);

			if (tile != NULL)
			{
				if (!m_engineOptions->m_fow_flashlight)
				{
					if (tile->GetFogBit() != fow_non)
					{
						tile->UpdateFogBit(*mask);
					}
				}
				else//Update every bit since we are using flashlight based fog
				{
					tile->UpdateFogBit(*mask);
				}
			}

			mask++;
		}
	}*/

	/*
	vector<XMFLOAT3> tilePositions;
	int tileNum = 0;
	
	//Figure out what tile the player is within
	Section* section_in = player->SectionIn();
	if (section_in == NULL) return;
	
	SectionAdjacency* sa = section_in->AdjacencyInfo();
	if (sa == NULL) return;

	Section* northSection = NULL;
	Section* southSection = NULL;
	Section* eastSection = NULL;
	Section* westSection = NULL;
	Section* NWSection = NULL;
	Section* NESection = NULL;
	Section* SWSection = NULL;
	Section* SESection = NULL;

	for (int i = 0; i < sa->adjSections.size(); i++)
	{
		AdjacentSection as = sa->adjSections[i];

		if (as.neighborPos == NORTH)
		{
			northSection = m_Sections[as.ID];
		}
		if (as.neighborPos == SOUTH)
		{
			southSection = m_Sections[as.ID];
		}
		if (as.neighborPos == EAST)
		{
			eastSection = m_Sections[as.ID];
		}
		if (as.neighborPos == WEST)
		{
			westSection = m_Sections[as.ID];
		}
		if (as.neighborPos == NW)
		{
			NWSection = m_Sections[as.ID];
		}
		if (as.neighborPos == NE)
		{
			NESection = m_Sections[as.ID];
		}
		if (as.neighborPos == SW)
		{
			SWSection = m_Sections[as.ID];
		}
		if (as.neighborPos == SE)
		{
			SESection = m_Sections[as.ID];
		}
	}

	float playerX = player->GetPosition().x;
	float playerY = player->GetPosition().y;

	float playerW = player->GetWidth();
	float playerH = player->GetHeight();

	// This holds the actaual tile values faned out from the player
	std::vector< XMFLOAT2 > fowFan;
	*/
	// Find the nearest tile to the player
	//int tilePlayerX = ceil(playerX);
	//int tilePlayerY = ceil(playerY);
	
	//section_in->GetTile(tilePlayerX, tilePlayerY)->UpdateFogBit(fow_non);

	/*bool breakOut = false;
	Tile* playerTile = NULL;
	for (int y = 0; y < m_engineOptions->COL_SIZE; y++)
	{
		for (int x = 0; x < m_engineOptions->ROW_SIZE; x++)
		{
			Tile* tile = section_in->GetTile(x, y);

			XMFLOAT3 tilePos = tile->GetPosition();

			// Found the tile the player is in, Fan the circle out from this tile
			if (playerX >(tilePos.x) && playerX < (tilePos.x + m_engineOptions->TILE_SIZE) &&
				playerY >(tilePos.y) && playerY < (tilePos.y + m_engineOptions->TILE_SIZE))
			{
				playerTile = tile;
				tile->UpdateFogBit(fow_non);
				breakOut = true;
				break;
			}
		}

		if (breakOut) break;
	}

	if (playerTile == NULL) return;

	float xTo = (playerTile->GetPosition().x + radius);
	float xBack = (playerTile->GetPosition().x - radius);

	float yTo = (playerTile->GetPosition().y + radius);
	float yBack = (playerTile->GetPosition().y - radius);

	for (float yy = yBack; yy <= yTo; yy++)
	{
		for (float xx = xBack; xx <= xTo; xx++)
		{
			bool found = false;

			XMFLOAT2 realTilePos(xx, yy);

			found = UpdateSectionFogMask(section_in, realTilePos, *mask);

			if (!found && northSection != NULL)
			{
				found = UpdateSectionFogMask(northSection, realTilePos, *mask);
			}

			if (!found && southSection != NULL)
			{
				found = UpdateSectionFogMask(southSection, realTilePos, *mask);
			}

			if (!found && eastSection != NULL)
			{
				found = UpdateSectionFogMask(eastSection, realTilePos, *mask);
			}

			if (!found && westSection != NULL)
			{
				found = UpdateSectionFogMask(westSection, realTilePos, *mask);
			}

			if (!found && NWSection != NULL)
			{
				found = UpdateSectionFogMask(NWSection, realTilePos, *mask);
			}

			if (!found && NESection != NULL)
			{
				found = UpdateSectionFogMask(NESection, realTilePos, *mask);
			}

			if (!found && SWSection != NULL)
			{
				found = UpdateSectionFogMask(SWSection, realTilePos, *mask);
			}

			if (!found && SESection != NULL)
			{
				found = UpdateSectionFogMask(SESection, realTilePos, *mask);
			}

			// Update the mask for the fow
			mask++;
		}
	}*/

	/*for (int y = 0; y < m_engineOptions->COL_SIZE; y++)
	{
		for (int x = 0; x < m_engineOptions->ROW_SIZE; x++)
		{
			Tile* tile = section_in->GetTile(x, y);

			XMFLOAT3 tilePos = tile->GetPosition();

			// Found the tile the player is in, Fan the circle out from this tile
			if (playerX > (tilePos.x - playerW) && playerX < (tilePos.x + m_engineOptions->TILE_SIZE) + playerW &&
				playerY > (tilePos.y - playerH) && playerY < (tilePos.y + m_engineOptions->TILE_SIZE) + playerH)
			{
				//Get all the tiles in the radius of the circle

				tilePositions.push_back(tilePos);

				tile->UpdateFogBit(fow_non);

				int xTo = (playerX + radius);// + 1;
				int xBack = (playerX - radius);// + 1;

				int yTo = (playerY + radius);// + 1;
				int yBack = (playerY - radius);// + 1;

				for (int yy = yBack; yy <= yTo; yy++)
				{
					for (int xx = xBack; xx <= xTo; xx++)
					{
						bool found = false;

						XMFLOAT2 realTilePos(xx, yy);

						found = UpdateSectionFogMask(section_in, realTilePos, *mask);

						if (!found && northSection != NULL)
						{
							found = UpdateSectionFogMask(northSection, realTilePos, *mask);
						}

						if (!found && southSection != NULL)
						{
							found = UpdateSectionFogMask(southSection, realTilePos, *mask);
						}

						if (!found && eastSection != NULL)
						{
							found = UpdateSectionFogMask(eastSection, realTilePos, *mask);
						}

						if (!found && westSection != NULL)
						{
							found = UpdateSectionFogMask(westSection, realTilePos, *mask);
						}

						if (!found && NWSection != NULL)
						{
							found = UpdateSectionFogMask(NWSection, realTilePos, *mask);
						}

						if (!found && NESection != NULL)
						{
							found = UpdateSectionFogMask(NESection, realTilePos, *mask);
						}

						if (!found && SWSection != NULL)
						{
							found = UpdateSectionFogMask(SWSection, realTilePos, *mask);
						}

						if (!found && SESection != NULL)
						{
							found = UpdateSectionFogMask(SESection, realTilePos, *mask);
						}

						// Update the mask for the fow
						mask++;
					}
				}
			}
		}
	}*/

	/*for (int i = 0; i < sectionRenderList.size(); i++)
	{
		int sec = sectionRenderList[i];
		if (m_Sections[sec])
		{
			for (int y = 0; y < m_engineOptions->COL_SIZE; y++)
			{
				for (int x = 0; x < m_engineOptions->ROW_SIZE; x++)
				{
					Tile* tile = m_Sections[sec]->GetTile(x, y);

					XMFLOAT3 tilePos = tile->GetPosition();

					//If the tile the player position is in
					//matches this tile then use it in the center
					//of the circle
					POINT pt;
					pt.x = playerX;
					pt.y = playerY;

					//if (PtInRect(&tile->GetBox(), pt))
					if (playerX > tilePos.x && playerX < tilePos.x + m_engineOptions->TILE_SIZE &&
						playerY > tilePos.y && playerY < tilePos.y + m_engineOptions->TILE_SIZE)
					{
						//Get all the tiles in the radius of the circle

						tilePositions.push_back( tilePos );

						tile->UpdateFogBit( fow_non );

						int xTo = (x + radius);// + 1;
						int xBack = (x - radius);// + 1;

						int yTo = (y + radius);// + 1;
						int yBack = (y - radius);// + 1;

						//Figure out what values belong to current section and what values belong to
						//neighboring sections.
						*/

						//Circle with a radius of 3
						//{x - 3, y + 3}, {x - 2,  y + 3}, {x - 1, y + 3}, {x + 0, y + 3}, {x + 1, y + 3}, {x + 2, y + 3}, {x + 3, y + 3}
						//{x - 3, y + 2}, {x - 2,  y + 2}, {x - 1, y + 2}, {x + 0, y + 2}, {x + 1, y + 2}, {x + 2, y + 2}, {x + 3, y + 2}
						//{x - 3, y + 1}, {x - 2,  y + 1}, {x - 1, y + 1}, {x + 0, y + 1}, {x + 1, y + 1}, {x + 2, y + 1}, {x + 3, y + 1}
						//{x - 3, y + 0}, {x - 2,  y + 0}, {x - 1, y + 0}, {x + 0, y + 0}, {x + 1, y + 0}, {x + 2, y + 0}, {x + 3, y + 0}
						//{x - 3, y - 1}, {x - 2,  y - 1}, {x - 1, y - 1}, {x + 0, y - 1}, {x + 1, y - 1}, {x + 2, y - 1}, {x + 3, y - 1}
						//{x - 3, y - 2}, {x - 2,  y - 2}, {x - 1, y - 2}, {x + 0, y - 2}, {x + 1, y - 2}, {x + 2, y - 2}, {x + 3, y - 2}
						//{x - 3, y - 3}, {x - 2,  y - 3}, {x - 1, y - 3}, {x + 0, y - 3}, {x + 1, y - 3}, {x + 2, y - 3}, {x + 3, y - 3}

						//Lets say x = 100 and y = 100

						//{97, 103}, {98, 103}, {99, 103}, {100, 103}, {101, 103}, {102, 103}, {103, 103}
						//{97, 102}, {98, 102}, {99, 102}, {100, 102}, {101, 102}, {102, 102}, {103, 102}
						//{97, 101}, {98, 101}, {99, 101}, {100, 101}, {101, 101}, {102, 101}, {103, 101}
						//{97, 100}, {98, 100}, {99, 100}, {100, 100}, {101, 100}, {102, 100}, {103, 100}
						//{97, 099}, {98, 099}, {99, 099}, {100, 099}, {101, 099}, {102, 099}, {103, 099}
						//{97, 098}, {98, 098}, {99, 098}, {100, 098}, {101, 098}, {102, 098}, {103, 098}
						//{97, 097}, {98, 097}, {99, 097}, {100, 097}, {101, 097}, {102, 097}, {103, 097}

						//x = 0 and y = 0

						//{-3,  3}, {-2,  3}, {-1,  3}, {0,  3}, {1,  3}, {2,  3}, {3,  3}
						//{-3,  2}, {-2,  2}, {-1,  2}, {0,  2}, {1,  2}, {2,  2}, {3,  2}
						//{-3,  1}, {-2,  1}, {-1,  1}, {0,  1}, {1,  1}, {2,  1}, {3,  1}
						//{-3,  0}, {-2,  0}, {-1,  0}, {0,  0}, {1,  0}, {2,  0}, {3,  0}
						//{-3, -1}, {-2, -1}, {-1, -1}, {0, -1}, {1, -1}, {2, -1}, {3, -1}
						//{-3, -2}, {-2, -2}, {-1, -2}, {0, -2}, {1, -2}, {2, -2}, {3, -2}
						//{-3, -3}, {-2, -3}, {-1, -3}, {0, -3}, {1, -3}, {2, -3}, {3, -3}

						//Put the animation timing values into the profile system
						//FIGURE OUT WHY IS THE ANIMATION SLOW
						//WHY IS THE ALPHA BLENDING LEAVING LINES ON THE SCREEN
						
						//Work on coordinating the speed of screen movement, animation and sprite movement with or without fog, to a good managable speed.
						//Add a hotkey toggle to make the camera return to where the player is (usful in map editor and in game, especially if fog is applied)

						//This is not the actual spots, It is the illegal spots on
						//the current section, they should translate to the spots in the direction
						/*vector<XMFLOAT2> westernSectionSpots;
						vector<XMFLOAT2> northernSectionSpots;
						vector<XMFLOAT2> easternSectionSpots;
						vector<XMFLOAT2> southernSectionSpots;
						//Need to get the corner sections also

						vector<unsigned short> westernSectionSpotMask;
						vector<unsigned short> northernSectionSpotMask;
						vector<unsigned short> easternSectionSpotMask;
						vector<unsigned short> southernSectionSpotMask;

						for (int yy = yBack; yy <= yTo; yy++)
						{
							for (int xx = xBack; xx <= xTo; xx++)
							{
								//If xx < 0 in western section
								if (xx < 0)
								{
									easternSectionSpots.push_back(XMFLOAT2(xx, yy));
									easternSectionSpotMask.push_back( *mask );
									mask++;
									continue;
								}
								//If yy < 0 in northern section
								if (yy < 0)
								{
									northernSectionSpots.push_back(XMFLOAT2(xx, yy));
									northernSectionSpotMask.push_back( *mask );
									mask++;
									continue;
								}
								if (xx > m_engineOptions->ROW_SIZE-1)
								{
									westernSectionSpots.push_back(XMFLOAT2(xx, yy));
									westernSectionSpotMask.push_back( *mask );
									mask++;
									continue;
								}
								if (yy > m_engineOptions->COL_SIZE-1)
								{
									southernSectionSpots.push_back(XMFLOAT2(xx, yy));
									southernSectionSpotMask.push_back( *mask );
									mask++;
									continue;
								}

								if (!m_engineOptions->m_fow_flashlight)
								{
									if (m_Sections[sec]->GetTile(xx, yy)->GetFogBit() != fow_non)
									{
										m_Sections[sec]->GetTile(xx, yy)->UpdateFogBit( *mask );
									}
								}
								else//Update every bit since we are using flashlight based fog
								{
									m_Sections[sec]->GetTile(xx, yy)->UpdateFogBit( *mask );
								}

								mask++;
							}
						}*/

						//It would be nice to know what direction the neighbor is in compared to the current section
						//NOT FULLY DONE (NEED CORNERS AND DIRECTION)
						/*vector<Section::NeighborIDS> neighbors = m_Sections[sec]->GetNeighbors();

						for (int i = 0; i < southernSectionSpots.size(); i++)
						{
							XMFLOAT2 val = southernSectionSpots[i];
							unsigned short m = southernSectionSpotMask[i];

							XMFLOAT2 realVal = TranslateToSpot(val);

							int sec_id = GetSectionIDFromDir(neighbors, "S");

							if (sec_id > -1)
							{
								if (m_Sections[sec_id])
								{
									if (m_Sections[sec_id]->GetTile(realVal.x, realVal.y)->GetFogBit() != fow_non)
										m_Sections[sec_id]->GetTile(realVal.x, realVal.y)->UpdateFogBit( m );
								}
							}
						}

						for (int i = 0; i < northernSectionSpots.size(); i++)
						{
							XMFLOAT2 val = northernSectionSpots[i];
							unsigned short m = northernSectionSpotMask[i];

							XMFLOAT2 realVal = TranslateToSpot(val);

							int sec_id = GetSectionIDFromDir(neighbors, "N");

							if (sec_id > -1)
							{
								if (m_Sections[sec_id])
								{
									if (m_Sections[sec_id]->GetTile(realVal.x, realVal.y)->GetFogBit() != fow_non)
										m_Sections[sec_id]->GetTile(realVal.x, realVal.y)->UpdateFogBit( m );
								}
							}
						}

						for (int i = 0; i < easternSectionSpots.size(); i++)
						{
							XMFLOAT2 val = easternSectionSpots[i];
							unsigned short m = easternSectionSpotMask[i];

							XMFLOAT2 realVal = TranslateToSpot(val);

							int sec_id = GetSectionIDFromDir(neighbors, "E");

							if (sec_id > -1)
							{
								if (m_Sections[sec_id])
								{
									if (m_Sections[sec_id]->GetTile(realVal.x, realVal.y)->GetFogBit() != fow_non)
										m_Sections[sec_id]->GetTile(realVal.x, realVal.y)->UpdateFogBit( m );
								}
							}
						}

						for (int i = 0; i < westernSectionSpots.size(); i++)
						{
							XMFLOAT2 val = westernSectionSpots[i];
							unsigned short m = westernSectionSpotMask[i];

							XMFLOAT2 realVal = TranslateToSpot(val);

							int sec_id = GetSectionIDFromDir(neighbors, "W");

							if (sec_id > -1)
							{
								if (m_Sections[sec_id])
								{
									if (m_Sections[sec_id]->GetTile(realVal.x, realVal.y)->GetFogBit() != fow_non)
										m_Sections[sec_id]->GetTile(realVal.x, realVal.y)->UpdateFogBit( m );
								}
							}
						}*/
					//}
				//}
			//}
		//}
	//}
}
//-----------------------------------------------------------------------------------------------------------------
bool TileSystem::UpdateSectionFogMask(Section* section, XMFLOAT2 realTilePos, XMFLOAT2 realSpriteTilePos, Sprite* sprite, unsigned short mask)
{
	bool found = false;
	
	//XMFLOAT2 realVal = TranslateToSpot(realTilePos);

	//if (section->InBounds(realVal.x, realVal.y))
	//{
	//	if (!m_engineOptions->m_fow_flashlight)
	//	{
	//		if (section->GetTile(realVal.x, realVal.y)->GetFogBit() != fow_non)
	//		{
	//			section->GetTile(realVal.x, realVal.y)->UpdateFogBit(mask);
	//			found = true;
	//		}
	//	}
	//	else//Update every bit since we are using flashlight based fog
	//	{
	//		section->GetTile(realVal.x, realVal.y)->UpdateFogBit(mask);
	//		found = true;
	//	}
	//}

	/*if (section->InBounds(realTilePos.x, realTilePos.y))
	{
		int xDiff = (realTilePos.x / m_engineOptions->TILE_SIZE);
		int yDiff = (realTilePos.y / m_engineOptions->TILE_SIZE);

		bool inRange = false;
		while (!inRange)
		{
			if (xDiff >= m_engineOptions->ROW_SIZE) xDiff -= (m_engineOptions->ROW_SIZE);
			if (yDiff >= m_engineOptions->COL_SIZE) yDiff -= (m_engineOptions->COL_SIZE);

			if (xDiff < m_engineOptions->ROW_SIZE && yDiff < m_engineOptions->COL_SIZE)
				inRange = true;
			else
			{
				if (xDiff > m_engineOptions->TILE_SIZE * 2) xDiff = (xDiff / m_engineOptions->TILE_SIZE);
				if (yDiff > m_engineOptions->TILE_SIZE * 2) yDiff = (yDiff / m_engineOptions->TILE_SIZE);
			}
		}

		if (!m_engineOptions->m_fow_flashlight)
		{
			if (section->GetTile(xDiff, yDiff)->GetFogBit() != fow_non)
			{
				section->GetTile(xDiff, yDiff)->UpdateFogBit(mask);
				return true;
			}
		}
		else//Update every bit since we are using flashlight based fog
		{
			section->GetTile(xDiff, yDiff)->UpdateFogBit(mask);
			return true;
		}
	}

	return false;*/

	Tile* aroundTiles[8];

	aroundTiles[0] = NULL;
	aroundTiles[1] = NULL;
	aroundTiles[2] = NULL;
	aroundTiles[3] = NULL;
	aroundTiles[4] = NULL;
	aroundTiles[5] = NULL;
	aroundTiles[6] = NULL;
	aroundTiles[7] = NULL;

	bool cull = false;
	Tile* tile2 = NULL;
	bool breakOut = false;
	for (int yyy = 0; yyy < m_engineOptions->COL_SIZE; yyy++)
	{
		for (int xxx = 0; xxx < m_engineOptions->ROW_SIZE; xxx++)
		{
			tile2 = section->GetTile(xxx, yyy);

			if (realTilePos.x == tile2->GetPosition().x && realTilePos.y == tile2->GetPosition().y)
			{
				found = true;

				if (xxx + 1 < m_engineOptions->ROW_SIZE) aroundTiles[0] = section->GetTile(xxx + 1, yyy);
				if (yyy + 1 < m_engineOptions->COL_SIZE) aroundTiles[1] = section->GetTile(xxx, yyy + 1);
				if (xxx + 1 < m_engineOptions->ROW_SIZE && yyy + 1 < m_engineOptions->COL_SIZE) aroundTiles[2] = section->GetTile(xxx + 1, yyy + 1);
				if (xxx > 0) aroundTiles[3] = section->GetTile(xxx - 1, yyy);
				if (yyy > 0) aroundTiles[4] = section->GetTile(xxx, yyy - 1);
				if (xxx > 0 && yyy > 0) aroundTiles[5] = section->GetTile(xxx - 1, yyy - 1);
				if (xxx + 1 < m_engineOptions->ROW_SIZE && yyy > 0) aroundTiles[6] = section->GetTile(xxx + 1, yyy - 1);
				if (xxx > 0 && yyy + 1 < m_engineOptions->COL_SIZE) aroundTiles[7] = section->GetTile(xxx - 1, yyy + 1);

				if (!m_bFOWFlashlight)
				{
					// Any tile that is behind the sprite should not have it's mask updated
					XMFLOAT2 tilePos(tile2->GetPosition().x, tile2->GetPosition().y);
					cull = false;

					if (sprite->MoveSpriteUp())
					{
						// Compare y to find any tile that has a y value that is greater
						if (tilePos.y > realSpriteTilePos.y)
						{
							cull = true;
						}
					}

					if (sprite->MoveSpriteDown())
					{
						// Compare y to find any tile that has a y value that is smaller
						if (tilePos.y < realSpriteTilePos.y)
						{
							cull = true;
						}
					}

					if (sprite->MoveSpriteLeft())
					{
						// Compare x to find any tile that has a x value that is greater
						if (tilePos.x > realSpriteTilePos.x)
						{
							cull = true;
						}
					}

					if (sprite->MoveSpriteRight())
					{
						// Compare x to find any tile that has a x value that is smaller
						if (tilePos.x < realSpriteTilePos.x)
						{
							cull = true;
						}
					}

					//// If this tile is a connector
					//if (tile2->GetFogBit() != fow_all && tile2->GetFogBit() != fow_non)
					//{
					//	int tally = 0;
					//	for (int i = 0; i < 8; i++)
					//	{
					//		if (aroundTiles[i] == NULL) continue;

					//		if (aroundTiles[i]->GetFogBit() != fow_non && aroundTiles[i]->GetFogBit() != fow_all)
					//		{
					//			tally++;
					//		}
					//	}

					//	if (tally == 1)
					//	{
					//		// The neighboring tile is a connector so this tile should have fow_non
					//		tile2->UpdateFogBit(fow_non);
					//		cull = true;
					//	}
					//}

					if (!cull)
					{
						if (tile2->GetFogBit() != fow_non)
						{
							tile2->UpdateFogBit(mask);
						}
					}
				}
				else//Update every bit since we are using flashlight based fog
				{
					tile2->UpdateFogBit(mask);
				}

				breakOut = true;
				break;
			}
		}

		if (breakOut) break;
	}

	return found;
}
//-----------------------------------------------------------------------------------------------------------------
/*int TileSystem::GetSectionIDFromDir(vector<Section::NeighborIDS> neighbors, char* dir)
{
	int sec_id = -1;

	if (neighbors.size() == 3)
	{
		if (strcmp(neighbors[0].dir, dir) == 0) sec_id = neighbors[0].id;
		if (strcmp(neighbors[1].dir, dir) == 0) sec_id = neighbors[1].id;
		if (strcmp(neighbors[2].dir, dir) == 0) sec_id = neighbors[2].id;
	}
	else if (neighbors.size() == 5)
	{
		if (strcmp(neighbors[0].dir, dir) == 0) sec_id = neighbors[0].id;
		if (strcmp(neighbors[1].dir, dir) == 0) sec_id = neighbors[1].id;
		if (strcmp(neighbors[2].dir, dir) == 0) sec_id = neighbors[2].id;
		if (strcmp(neighbors[3].dir, dir) == 0) sec_id = neighbors[3].id;
		if (strcmp(neighbors[4].dir, dir) == 0) sec_id = neighbors[4].id;
	}
	else if (neighbors.size() == 8)
	{
		if (strcmp(neighbors[0].dir, dir) == 0) sec_id = neighbors[0].id;
		if (strcmp(neighbors[1].dir, dir) == 0) sec_id = neighbors[1].id;
		if (strcmp(neighbors[2].dir, dir) == 0) sec_id = neighbors[2].id;
		if (strcmp(neighbors[3].dir, dir) == 0) sec_id = neighbors[3].id;
		if (strcmp(neighbors[4].dir, dir) == 0) sec_id = neighbors[4].id;
		if (strcmp(neighbors[5].dir, dir) == 0) sec_id = neighbors[5].id;
		if (strcmp(neighbors[6].dir, dir) == 0) sec_id = neighbors[6].id;
		if (strcmp(neighbors[7].dir, dir) == 0) sec_id = neighbors[7].id;
	}

	return sec_id;
}*/
//-----------------------------------------------------------------------------------------------------------------
XMFLOAT2 TileSystem::TranslateToSpot(XMFLOAT2 value)
{
	int begin = 0;
	int xEnd = 12;
	int yEnd = 8;

	float x = value.x;
	float y = value.y;

	XMFLOAT2 v = value;

	/*
		x is 0 - 11
		y is 0 - 7

		-1  and 12

		-1 - 12 = -13 (no)

		12 - (-1) = 13 (no)

		(xEnd - 1) - x

		11 - (-1) = 12 (yes)
		11 - (-2) = 13 (no)

		(no)

		xEnd - abs(x)

		12 - abs(-1) = 12 - 1 = 11 (yes)
		12 - abs(-2) = 12 - 2 = 10 (yes)

	*/

	if (x < begin)
	{
		v.x = xEnd - abs(x);
	}

	if (y < begin)
	{
		v.y = yEnd - abs(y);
	}

	if (x >= xEnd)
	{
		v.x = x - xEnd;
	}

	if (y >= yEnd)
	{
		v.y = y - yEnd;
	}

	return v;
}
//-----------------------------------------------------------------------------------------------------------------
void TileSystem::UpdateTextures(vector<int> sectionRenderList)
{
	for (int i = 0; i < sectionRenderList.size(); i++)
	{
		int sec = sectionRenderList[i];
		if (m_Sections[sec])
		{
			//Update the textures of each tile
			m_Sections[sec]->UpdateTexture();
		}
	}
}
//-----------------------------------------------------------------------------------------------------------------
bool TileSystem::Render(Camera* camera, vector<int> sectionRenderList)
{
	//for (int i = 0; i < m_Sections.size(); i++)
	for (int i = 0; i < sectionRenderList.size(); i++)
	{
		int sec = sectionRenderList[i];
		if (m_Sections[sec])
			m_Sections[sec]->Render(camera);
	}
	return true;
}
//-----------------------------------------------------------------------------------------------------------------