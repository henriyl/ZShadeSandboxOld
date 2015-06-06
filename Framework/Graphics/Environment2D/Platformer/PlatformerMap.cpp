#include "PlatformerMap.h"
//==================================================================================================================================
//==================================================================================================================================
PlatformerMap::PlatformerMap(D3D* d3d)
:   m_D3DSystem(d3d)
{
	//m_EngineOptions->SECTION_ROW_SIZE = 10;
	//m_EngineOptions->SECTION_COL_SIZE = 15;
	
	m_BackgroundScreenWidth = 1;
	m_Offset.x = m_Offset.y = 0;
	m_EngineOptions = d3d->GetEngineOptions();
	m_MapWidth = m_EngineOptions->SECTION_ROW_SIZE;
	m_MapHeight = m_EngineOptions->SECTION_COL_SIZE;
	m_Tiles.resize(m_EngineOptions->SECTION_ROW_SIZE, m_EngineOptions->SECTION_COL_SIZE);
	m_ClippedSprites.resize(MAX_SPRITES);
	m_TileRemovedX = -1;
	m_TileRemovedY = -1;
	bEditor = false;
	kWorldHalfExtents = XMFLOAT2(m_EngineOptions->SECTION_ROW_SIZE * m_EngineOptions->TILE_SIZE * 0.5f, m_EngineOptions->SECTION_ROW_SIZE * m_EngineOptions->TILE_SIZE * 0.5f);
}
//==================================================================================================================================
void PlatformerMap::Initialize(GameDirectory2D* gd, HWND hwndPB)
{
	// Loads a new map that has tiles with no texture
	
	int tile_id = 0;
	int size = m_EngineOptions->TILE_SIZE;
	
	//if (bEditor)
	{
		int tileX = PlatformerMap::PixelsToTiles(m_EngineOptions->m_screenWidth / 2 - m_EngineOptions->TILE_SIZE);
		int tileY = PlatformerMap::PixelsToTiles(m_EngineOptions->m_screenHeight / 2 - m_EngineOptions->TILE_SIZE);

		//int pixelX = PlatformerMap::TilesToPixels(tileX);
		//int pixelY = PlatformerMap::TilesToPixels(tileY);

		XMFLOAT3 pp(tileX, tileY, 0);
		AISprite* m_MapSprite = new AISprite(m_D3DSystem);
		m_MapSprite->Initialize(gd->m_required_textures, "player_cover.png", pp, m_EngineOptions->TILE_SIZE, m_EngineOptions->TILE_SIZE, DYNAMIC);
		m_MapSprite->CreateHardBoxSpr(gd->m_required_textures);
		m_MapSprite->Speed() = 2;
		m_MapSprite->PlatformerSprite() = true;
		AddPlayer(m_MapSprite);
	}

	int tileX = PlatformerMap::PixelsToTiles(0);
	int tileY = PlatformerMap::PixelsToTiles(0);

	startTile = new PlatformerTile(m_D3DSystem);
	startTile->Initialize(size, false, XMFLOAT3(tileX, tileY, 0));

	tileX = PlatformerMap::PixelsToTiles(m_MapWidth - 1);
	tileY = PlatformerMap::PixelsToTiles(m_MapHeight - 1);

	endTile = new PlatformerTile(m_D3DSystem);
	endTile->Initialize(size, false, XMFLOAT3(tileX, tileY, 0));

	/*int mapTiles[15][10] =
	{
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, },
		{ 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, },
		{ 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, },
		{ 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, },
		{ 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, },
		{ 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, },
		{ 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, },
		{ 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, },
		{ 1, 2, 0, 0, 0, 0, 0, 0, 0, 1, },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 }
	};*/

	/*for (int y = 0; y < m_MapHeight; y++)
	{
		for (int x = 0; x < m_MapWidth; x++)
		{
			//int tileNum = mapTiles[y][x];

			//if (tileNum == 1)
			AddTile(x, y, size, true, true, gd, "PlatformTile.png", "NONE", 0, size, size);
			
			tile_id++;
		}
	}*/
}
//==================================================================================================================================
void PlatformerMap::InitializeTiles(PlatformerMapLoadData pmld, GameDirectory2D* gd, HWND hwndPB)
{
	int tile_id = 0;
	int size = m_EngineOptions->TILE_SIZE;
	
	//if (bEditor)
	{
		/*int tileX = PlatformerMap::PixelsToTiles(m_EngineOptions->m_screenWidth / 2 - m_EngineOptions->TILE_SIZE);
		int tileY = PlatformerMap::PixelsToTiles(m_EngineOptions->m_screenHeight / 2 - m_EngineOptions->TILE_SIZE);

		int pixelX = PlatformerMap::TilesToPixels(tileX);
		int pixelY = PlatformerMap::TilesToPixels(tileY);

		XMFLOAT3 pp(pixelX, pixelY, 0);
		AISprite* m_MapSprite = new AISprite(m_D3DSystem);
		m_MapSprite->Initialize(gd->m_required_textures, "player_cover.png", pp, m_EngineOptions->TILE_SIZE, m_EngineOptions->TILE_SIZE, DYNAMIC);
		m_MapSprite->CreateHardBoxSpr(gd->m_required_textures);
		m_MapSprite->Speed() = 2;
		m_MapSprite->PlatformerSprite() = true;
		AddPlayer(m_MapSprite);*/
	}

	int tileX = PlatformerMap::PixelsToTiles(0);
	int tileY = PlatformerMap::PixelsToTiles(0);

	startTile = new PlatformerTile(m_D3DSystem);
	startTile->Initialize(size, false, XMFLOAT3(tileX, tileY, 0));

	tileX = PlatformerMap::PixelsToTiles(m_MapWidth - 1);
	tileY = PlatformerMap::PixelsToTiles(m_MapHeight - 1);

	endTile = new PlatformerTile(m_D3DSystem);
	endTile->Initialize(size, false, XMFLOAT3(tileX, tileY, 0));

	for (int y = 0; y < m_MapHeight; y++)
	{
		for (int x = 0; x < m_MapWidth; x++)
		{
			if (tile_id >= pmld.imageNames.size()) break;

			//if (pmld.exists[XMFLOAT2EXT(x, y)] == 0) continue;
			if (pmld.exists.get(XMFLOAT2EXT(x, y)) == 0) continue;

			string textureFilename = pmld.imageNames[tile_id];
			string animProfileName = pmld.animProfileNames[tile_id];
			int animSpeed = pmld.animSpeeds[tile_id];
			bool hard = pmld.hards[tile_id];

			int test_x = pmld.xs[tile_id];
			int test_y = pmld.ys[tile_id];
			
			//if (test_x == x && test_y == y)
			{
				AddTile(x, y, size, hard, false, gd, textureFilename, animProfileName, animSpeed, size, size);

				tile_id++;
			}
		}
	}
}
//==================================================================================================================================
void PlatformerMap::AddTile
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
)
{
	//int tileX = PlatformerMap::TilesToPixels(x);
	//int tileY = PlatformerMap::TilesToPixels(y);

	m_Tiles.insert(x, y, new PlatformerTile(m_D3DSystem));
	m_Tiles(x, y)->Initialize(size, hard, XMFLOAT3(x, y, 0));

	string base_path = "";

	if (init)
	{
		base_path = gd->m_required_textures;
	}
	else
	{
		base_path = gd->m_tiles_path;
	}

	if (textureFilename == "PlatformTile.png")
		base_path = gd->m_required_textures;

	m_Tiles(x, y)->InitializeTexture("Render", base_path, textureFilename, bitmapWidth, bitmapHeight);
	m_Tiles(x, y)->SetAnimationProfileName(animProfileName);
	m_Tiles(x, y)->SetAnimationSpeed(animSpeed);

	string path = gd->m_required_textures;

	m_Tiles(x, y)->CreateHardBoxSpr(path);

	// If an animation exists then load it
	if (animProfileName != "NONE")
	{
		//Load a animation profile
		AnimationProfile2D* profile = new AnimationProfile2D(m_D3DSystem);

		bool something_wrong = false;

		if (profile->LoadAnimationProfileNames(animProfileName, "TILE", gd->GAME_FOLDER))
		{
			if (profile->ReadProfile(animProfileName, "TILE", gd->m_tiles_path, gd->GAME_FOLDER))
			{
				vector<Animation2D*> anims = profile->GetAnimations();

				PlatformerTile* tile = m_Tiles(x, y);

				tile->ClearAnimationList();

				for (int i = 0; i < anims.size(); i++)
				{
					tile->AddAnimationSequence(anims[i]);
				}

				tile->SetSequence(0);
			}
			else something_wrong = true;
		}
		else something_wrong = true;

		if (something_wrong)
		{
			MessageBox(NULL, "The Animation2D does not exist", "Add Animations", MB_OK);
		}
	}
}
//=================================================================================================================
PlatformerTile* PlatformerMap::GetTile(int x, int y)
{
	return GetTileSafe(x, y);
}
//=================================================================================================================
PlatformerTile* PlatformerMap::GetTileSafe(int x, int y)
{
	if (x >= 0 && x < m_MapWidth)
	{
		if (y >= 0 && y < m_MapHeight)
		{
			return m_Tiles(x, y);
		}
	}

	return 0;
}
//=================================================================================================================
void PlatformerMap::AddTile(GameDirectory2D* gd, int x, int y)
{
	if (!InBounds(x, y)) return;
	
	int size = m_EngineOptions->TILE_SIZE;
	//int sX = PlatformerMap::PixelsToTiles(x);
	//int sY = PlatformerMap::PixelsToTiles(y);

	//int sX = (x + m_Offset.x) / size;
	//int sY = (y + m_Offset.y) / size;

	AddTile(x, y, size, true, true, gd, "PlatformTile.png", "NONE", 0, size, size);
	/*for (int tileY = 0; tileY < m_MapHeight; tileY++)
	{
		for (int tileX = 0; tileX < m_MapWidth; tileX++)
		{
			PlatformerTile* tile = m_TempTiles(tileX, tileY);

			if (tile == NULL) continue;

			//int realTileX = PlatformerMap::TilesToPixels(tileX) + m_Offset.x;
			//int realTileY = PlatformerMap::TilesToPixels(tileY) + m_Offset.y;
			float realTileX = tile->GetPosition().x;
			float realTileY = tile->GetPosition().y;
			
			// Look at the BodyBox for the point x,y to see if
			// it lies inside
			if (tile->BBodyBox().Inside(x, y))
			{
					AddTile(tile->GetPosition().x, tile->GetPosition().y, size, true, true, gd, "PlatformTile.png", "NONE", 0, size, size, false);
					return;
			}
			
			if (x >= realTileX && x <= realTileX + m_EngineOptions->TILE_SIZE)
			{
				if (y >= realTileY && y <= realTileY + m_EngineOptions->TILE_SIZE)
				{
					AddTile(tile->GetPosition().x, tile->GetPosition().y, size, true, true, gd, "PlatformTile.png", "NONE", 0, size, size, false);
					return;
				}
			}
		}
	}*/
}
//=================================================================================================================
void PlatformerMap::RemoveTile(int x, int y)
{
	if (!InBounds(x, y)) return;
	
	m_TileRemovedX = x;
	m_TileRemovedY = y;
	
	//int tileX = PlatformerMap::PixelsToTiles(x);// +m_Offset.x;
	//int tileY = PlatformerMap::PixelsToTiles(y);// +m_Offset.y;
	//delete m_Tiles(tileX, tileY);
	//m_Tiles(tileX, tileY) = NULL;

	int removeX = -1;
	int removeY = -1;

	bool out = false;

	/*for (int tileY = 0; tileY < m_MapHeight; tileY++)
	{
		for (int tileX = 0; tileX < m_MapWidth; tileX++)
		{
			PlatformerTile* tile = m_Tiles(tileX, tileY);

			if (tile == NULL) continue;

			//float realTileX = tile->GetPosition().x;
			//float realTileY = tile->GetPosition().y;
			
			// Look at the BodyBox for the point x,y to see if it lies inside
			if (tile->BBodyBox().InsideEquals(x, y))
			{
				removeX = tileX + 1;// Need to test
				removeY = tileY + 1;
				out = true;
				break;
			}

			/*if (x >= realTileX && x <= realTileX + m_EngineOptions->TILE_SIZE)
			{
				if (y >= realTileY && y <= realTileY + m_EngineOptions->TILE_SIZE)
				{
					removeX = tileX + 1;// Need to test
					removeY = tileY + 1;
					out = true;
					break;
				}
			}
		}
		
		if (out) break;
	}*/

	//if (removeX != -1 && removeY != -1)
	if (m_Tiles(x, y) != NULL)
	{
		delete m_Tiles(x, y);
		m_Tiles(x, y) = NULL;
	}
}
//=================================================================================================================
void PlatformerMap::PlayerMovingDir(bool& moved, int& dirUD, int& dirLR)
{
	AISprite* player = GetPlayer();

	if (player == NULL) return;

	dirUD = -1;
	dirLR = -1;
	
	moved = false;

	if (player->MoveSpriteUp())
	{
		dirUD = 0;
		moved = true;
	}
	if (player->MoveSpriteDown())
	{
		dirUD = 1;
		moved = true;
	}

	if (player->MoveSpriteLeft())
	{
		dirLR = 2;
		moved = true;
	}
	if (player->MoveSpriteRight())
	{
		dirLR = 3;
		moved = true;
	}
}
//==================================================================================================================================
XMFLOAT3 PlatformerMap::MatchingTilePos(float x, float y)
{
	XMFLOAT3 value = XMFLOAT3(x, y, 0);
	
	for (int tileY = 0; tileY < m_MapHeight; tileY++)
	{
		for (int tileX = 0; tileX < m_MapWidth; tileX++)
		{
			PlatformerTile* tile = m_Tiles(tileX, tileY);

			if (tile == NULL) continue;

			//int realTileX = PlatformerMap::TilesToPixels(tileX) + m_Offset.x;
			//int realTileY = PlatformerMap::TilesToPixels(tileY) + m_Offset.y;
			float realTileX = tile->GetPosition().x;
			float realTileY = tile->GetPosition().y;
			
			// Look at the BodyBox for the point x,y to see if it lies inside
			//if (tile->BBodyBox().InsideEquals(x, y))
			if (tile->Body().Inside(x, y))
			{
				return XMFLOAT3(realTileX, realTileY, 0);
			}
			
			/*if (x >= realTileX && x <= realTileX + m_EngineOptions->TILE_SIZE)
			{
				if (y >= realTileY && y <= realTileY + m_EngineOptions->TILE_SIZE)
				{
					value = XMFLOAT3(realTileX, realTileY, 0);
					return value;
				}
			}*/
		}
	}

	return value;
}
//==================================================================================================================================
bool PlatformerMap::InBounds(float x, float y)
{
	//int tileX = PlatformerMap::PixelsToTiles(x) + m_Offset.x;
	//int tileY = PlatformerMap::PixelsToTiles(y) + m_Offset.y;
	
	if (x >= 0 && x <= m_MapWidth - 1)
	{
		if (y >= 0 && y <= m_MapHeight - 1)
		{
			return true;
		}
	}
	
	return false;
}
//==================================================================================================================================
bool PlatformerMap::InScreen(float x, float y)
{
	//int ix = round(x);
	//int iy = round(y);
	
	if (x >= -m_EngineOptions->TILE_SIZE && x < m_EngineOptions->m_screenWidth)
	{
		if (y >= -m_EngineOptions->TILE_SIZE && y < m_EngineOptions->m_screenHeight)
		{
			return true;
		}
	}
	
	return false;
}
//==================================================================================================================================
void PlatformerMap::ClipSpriteToMap(Sprite* spr, float speed, float& newX, float& newY, float& newHBX, float& newHBY)
{
	// Need to update this to work with offset and physics system
	
	int w = spr->Width();
	int h = spr->Height();

	if (spr->MoveSpriteUp())
	{
		if (newY <= (m_BoundingBox.lastTileY + h) - m_MapHeight * m_EngineOptions->TILE_SIZE)
		{
			newY += speed;
			newHBY += speed;
			spr->SetMoveSpriteUp(false);
		}
	}
	
	if (spr->MoveSpriteDown())
	{
		if (newY >= (m_BoundingBox.firstTileY - h) + m_MapHeight * m_EngineOptions->TILE_SIZE)
		{
			newY -= speed;
			newHBY -= speed;
			spr->SetMoveSpriteDown(false);
		}
	}
	
	if (spr->MoveSpriteLeft())
	{
		if (newX <= (m_BoundingBox.lastTileX + w) - m_MapWidth * m_EngineOptions->TILE_SIZE)
		{
			newX += speed;
			newHBX += speed;
			spr->SetMoveSpriteLeft(false);
		}
	}
	
	if (spr->MoveSpriteRight())
	{
		if (newX >= (m_BoundingBox.firstTileX - w) +
			(m_MapWidth * m_EngineOptions->TILE_SIZE))
		{
			newX -= speed;
			newHBX -= speed;
			spr->SetMoveSpriteRight(false);
		}
	}
}
//==================================================================================================================================
void PlatformerMap::AddSprite(AISprite* s)
{
	s->IsPlayer() = false;
	s->ID() = spriteNum++;
	m_Sprites.push_back( s );
}
//==================================================================================================================================
void PlatformerMap::AddSpriteNoIndex(AISprite* s)
{
	s->IsPlayer() = false;
	m_Sprites.push_back( s );
}
//==================================================================================================================================
void PlatformerMap::AddPlayer(AISprite* s)
{
	s->IsPlayer() = true;
	s->ID() = spriteNum++;
	m_Sprites.push_back( s );
}
//==================================================================================================================================
void PlatformerMap::AddPlayerNoIndex(AISprite* s)
{
	s->IsPlayer() = true;
	m_Sprites.push_back( s );
}
//=================================================================================================================
void PlatformerMap::RotateTile(int x, int y)
{
	for (int tileY = 0; tileY < m_MapHeight; tileY++)
	{
		for (int tileX = 0; tileX < m_MapWidth; tileX++)
		{
			PlatformerTile* tile = m_Tiles(tileX, tileY);

			if (tile == NULL) continue;

			//float realTileX = tile->GetPosition().x;
			//float realTileY = tile->GetPosition().y;
			
			// Look at the BodyBox for the point x,y to see if it lies inside
			if (tile->Body().Inside(x, y))
			{
				m_Tiles(tileX, tileY)->RotateTile();
				return;
			}
			
			/*if (x >= realTileX && x <= realTileX + m_EngineOptions->TILE_SIZE)
			{
				if (y >= realTileY && y <= realTileY + m_EngineOptions->TILE_SIZE)
				{
					m_Tiles(tileX, tileY)->RotateTile();
					return;
				}
			}*/
		}
	}
}
//=================================================================================================================
PlatformerTile* PlatformerMap::SelectTile(int x, int y)
{
	if (!InBounds(x, y)) return 0;

	for (int tileY = 0; tileY < m_MapHeight; tileY++)
	{
		for (int tileX = 0; tileX < m_MapWidth; tileX++)
		{
			PlatformerTile* tile = m_Tiles(tileX, tileY);

			if (tile == NULL) continue;

			//float realTileX = tile->GetPosition().x;
			//float realTileY = tile->GetPosition().y;
			
			// Look at the BodyBox for the point x,y to see if it lies inside
			if (tile->Body().Inside(x, y))
				return tile;
			
			/*if (x >= realTileX && x <= realTileX + m_EngineOptions->TILE_SIZE)
			{
				if (y >= realTileY && y <= realTileY + m_EngineOptions->TILE_SIZE)
				{
					return tile;
				}
			}*/
		}
	}

	return 0;
}
//=================================================================================================================
void PlatformerMap::UpdateTileTexture(int x, int y, string base_tile_path, string textureFilename, int bitmapWidth, int bitmapHeight)
{
	if (!InBounds(x, y)) return;

	if (m_Tiles(x, y) != NULL)
		m_Tiles(x, y)->InitializeTexture("Render", base_tile_path, textureFilename, bitmapWidth, bitmapHeight);

	/*for (int tileY = 0; tileY < m_MapHeight; tileY++)
	{
		for (int tileX = 0; tileX < m_MapWidth; tileX++)
		{
			PlatformerTile* tile = m_Tiles(tileX, tileY);

			if (tile == NULL) continue;
			
			// Look at the BodyBox for the point x,y to see if it lies inside
			if (tile->BBodyBox().InsideEquals(x, y))
			{
				m_Tiles(tileX, tileY)->InitializeTexture("Render", base_tile_path, textureFilename, bitmapWidth, bitmapHeight);
				return;
			}
		}
	}*/
}
//=================================================================================================================
void PlatformerMap::UpdateTileHardness(int x, int y, bool bHard)
{
	if (!InBounds(x, y)) return;

	for (int tileY = 0; tileY < m_MapHeight; tileY++)
	{
		for (int tileX = 0; tileX < m_MapWidth; tileX++)
		{
			PlatformerTile* tile = m_Tiles(tileX, tileY);

			if (tile == NULL) continue;
			
			// Look at the BodyBox for the point x,y to see if it lies inside
			if (tile->Body().Inside(x, y))
			{
				m_Tiles(tileX, tileY)->SetHard(bHard);
				return;
			}
			
			/*float realTileX = tile->GetPosition().x;
			float realTileY = tile->GetPosition().y;

			if (x >= realTileX && x <= realTileX + m_EngineOptions->TILE_SIZE)
			{
				if (y >= realTileY && y <= realTileY + m_EngineOptions->TILE_SIZE)
				{
					m_Tiles(tileX, tileY)->SetHard(bHard);
					return;
				}
			}*/
		}
	}
}
//=================================================================================================================
void PlatformerMap::MakeTileHard(int x, int y)
{
	if (!InBounds(x, y)) return;

	for (int tileY = 0; tileY < m_MapHeight; tileY++)
	{
		for (int tileX = 0; tileX < m_MapWidth; tileX++)
		{
			PlatformerTile* tile = m_Tiles(tileX, tileY);

			if (tile == NULL) continue;
			
			// Look at the BodyBox for the point x,y to see if it lies inside
			if (tile->Body().Inside(x, y))
			{
				if (m_Tiles(tileX, tileY)->IsHard())
					m_Tiles(tileX, tileY)->SetHard(false);
				else
					m_Tiles(tileX, tileY)->SetHard(true);
				return;
			}
			
			/*float realTileX = tile->GetPosition().x;
			float realTileY = tile->GetPosition().y;

			if (x >= realTileX && x <= realTileX + m_EngineOptions->TILE_SIZE)
			{
				if (y >= realTileY && y <= realTileY + m_EngineOptions->TILE_SIZE)
				{
					if (m_Tiles(tileX, tileY)->IsHard())
						m_Tiles(tileX, tileY)->SetHard(false);
					else
						m_Tiles(tileX, tileY)->SetHard(true);
					return;
				}
			}*/
		}
	}
}
//=================================================================================================================
void PlatformerMap::DisableTexture(int x, int y)
{
	if (!InBounds(x, y)) return;

	for (int tileY = 0; tileY < m_MapHeight; tileY++)
	{
		for (int tileX = 0; tileX < m_MapWidth; tileX++)
		{
			PlatformerTile* tile = m_Tiles(tileX, tileY);

			if (tile == NULL) continue;
			
			// Look at the BodyBox for the point x,y to see if it lies inside
			if (tile->Body().Inside(x, y))
			{
				m_Tiles(tileX, tileY)->DisableTexture();
				return;
			}
			
			/*float realTileX = tile->GetPosition().x;
			float realTileY = tile->GetPosition().y;

			if (x >= realTileX && x <= realTileX + m_EngineOptions->TILE_SIZE)
			{
				if (y >= realTileY && y <= realTileY + m_EngineOptions->TILE_SIZE)
				{
					m_Tiles(tileX, tileY)->DisableTexture();
					return;
				}
			}*/
		}
	}
}
//=================================================================================================================
void PlatformerMap::ToggleHardness(bool bEnable)
{
	// Set hardness for the tiles
	for (int y = 0; y < m_MapHeight; y++)
	{
		for (int x = 0; x < m_MapWidth; x++)
		{
			PlatformerTile* tile = m_Tiles(x, y);

			if (tile == NULL) continue;

			tile->SetDisplayHardbox(bEnable);
		}
	}

	// Set hardness for the sprites
	for (int i = 0; i < m_Sprites.size(); i++)
	{
		AISprite* s = m_Sprites[i];
		s->CanDisplayHardbox() = bEnable;
	}
}
//==================================================================================================================================
void PlatformerMap::RemoveSprite(AISprite* s)
{
	bool deleteSpr = false;
	AISprite* spr;
	int value;
	
	for (int i = 0; i < m_Sprites.size(); i++)
	{
		spr = m_Sprites[i];

		if (spr != NULL)
		{
			if (spr->ID() == s->ID())
			{
				value = i;
				deleteSpr = true;
				break;
			}
		}
	}
	
	if (deleteSpr)
	{
		m_Sprites.erase(m_Sprites.begin() + value);

		if (m_ClippedSprites.size() > 0)
		{
			m_ClippedSprites.clear();
			m_ClippedSprites.resize(MAX_SPRITES);
		}

		spriteNum--;
		m_numSpritesRendered--;
		
		ClipSprites();
	}
}
//==================================================================================================================================
void PlatformerMap::ClipSprites()
{
	// if(x >= -150 && x < screenWidth)
	
	vector<AISprite*> renderList(MAX_SPRITES);
	
	for (int j = 0; j < m_Sprites.size(); j++)
	{
		if (m_Sprites[j] == NULL) continue;

		AISprite* spr = m_Sprites[j];
		
		if (InScreen(spr->X(), spr->Y()))
		{
			renderList[j] = spr;
		}
	}
	
	//Fix the render list based on the clipping of each sprite
	if (m_Sprites.size() > 0)
	{
		if (m_ClippedSprites.size() > 0) m_ClippedSprites.clear();

		m_numSpritesRendered = 0;

		for (int j = 0; j < renderList.size(); j++)
		{
			if (renderList[j] != NULL)
				m_numSpritesRendered++;

			m_ClippedSprites.push_back( renderList[j] );
		}
	}
}
//==================================================================================================================================
void PlatformerMap::AddSpriteAnimations(string profileName, string base_game_folder, string base_sprite_path, int id)
{
	//Load a animation profile
	AnimationProfile2D* profile = new AnimationProfile2D(m_D3DSystem);

	bool something_wrong = false;
	
	if (profile->ReadProfile(profileName, "SPRITE", base_sprite_path, base_game_folder))
	{
		vector<Animation2D*> anims = profile->GetAnimations();

		AISprite* spr;

		for (int sprite_num = 0; sprite_num < m_Sprites.size(); sprite_num++)
		{
			if (sprite_num == id)
			{
				spr = m_Sprites[sprite_num];

				spr->ClearAnimationList();

				for (int i = 0; i < anims.size(); i++)
				{
					spr->AddAnimationSequence(anims[i]);
				}

				spr->SetSequence(spr->DefaultSequence());

				break;
			}
		}
	}
	else something_wrong = true;

	if (something_wrong)
	{
		MessageBox(NULL, "The Animation2D does not exist", "Add Animations", MB_OK);
	}
}
//==================================================================================================================================
void PlatformerMap::AddTileAnimations(string profileName, string base_game_folder, string base_tile_path, float tile_x, float tile_y)
{
	//Load a animation profile
	AnimationProfile2D* profile = new AnimationProfile2D(m_D3DSystem);

	bool something_wrong = false;
	
	if (profile->ReadProfile(profileName, "TILE", base_tile_path, base_game_folder))
	{
		vector<Animation2D*> anims = profile->GetAnimations();

		PlatformerTile* tile = m_Tiles(tile_x, tile_y);

		tile->ClearAnimationList();

		for (int i = 0; i < anims.size(); i++)
		{
			tile->AddAnimationSequence(anims[i]);
		}

		tile->SetSequence(0);
	}
	else something_wrong = true;

	if (something_wrong)
	{
		MessageBox(NULL, "The Animation2D does not exist", "Add Animations", MB_OK);
	}
}
//==================================================================================================================================
AISprite* PlatformerMap::GetSpriteByID(int id)
{
	AISprite* spr;
	for (int sprite_num = 0; sprite_num < m_Sprites.size(); sprite_num++)
	{
		spr = m_Sprites[sprite_num];
		if (spr->ID() == id)
		{
			return spr;
		}
	}

	return NULL;
}
//==================================================================================================================================
AISprite* PlatformerMap::GetPlayer()
{
	AISprite* spr;
	for (int sprite_num = 0; sprite_num < m_Sprites.size(); sprite_num++)
	{
		spr = m_Sprites[sprite_num];

		if (spr->IsPlayer())
		{
			return spr;
		}
	}

	return NULL;
}
//==================================================================================================================================
XMFLOAT2 PlatformerMap::PlayerOffset()
{
	//AISprite* player = GetPlayer();
	//
	//if (player == NULL) return XMFLOAT2(0, 0);
	//
	////float px = PlatformerMap::PixelsToTiles(player->X());
	////float py = PlatformerMap::PixelsToTiles(player->Y());

	//float playerX = player->X();
	//float playerY = player->Y();
	//
	//int mapWidth = PlatformerMap::TilesToPixels(m_MapWidth);
	//int mapHeight = PlatformerMap::TilesToPixels(m_MapHeight);
	//
	//int offsetX = ((m_EngineOptions->m_screenWidth / 2) - round(playerX)) + m_EngineOptions->TILE_SIZE;
	//
	//offsetX = ZMath::Min<int>(offsetX, 0);
	//offsetX = ZMath::Max<int>(offsetX, m_EngineOptions->m_screenWidth - mapWidth);
	//
	//int offsetY = ((m_EngineOptions->m_screenHeight / 2) - round(playerY)) + m_EngineOptions->TILE_SIZE;
	//
	//offsetY = ZMath::Min<int>(offsetY, 0);
	//offsetY = ZMath::Max<int>(offsetY, m_EngineOptions->m_screenHeight - mapHeight);
	//
	//return XMFLOAT2(offsetX, offsetY);
	return XMFLOAT2(0, 0);
}
//==================================================================================================================================
void PlatformerMap::MoveTiles(float speed, int dirUD, int dirLR)
{
	/*PlatformerTile* tile;
	for (int y = 0; y < m_MapHeight; y++)
	{
		for (int x = 0; x < m_MapWidth; x++)
		{
			PlatformerTile* tile = m_Tiles(x, y);

			if (tile == NULL) continue;

			// Update the offsets for the tile
			int realTileX = tile->GetPosition().x;// PlatformerMap::TilesToPixels(x) + m_Offset.x;
			int realTileY = tile->GetPosition().y;//PlatformerMap::TilesToPixels(y) + m_Offset.y;

			if (dirUD == 0)
			{
				realTileY += speed;
			}
			else if (dirUD == 1)
			{
				realTileY -= speed;
			}

			if (dirLR == 2)
			{
				realTileX += speed;
			}
			else if (dirLR == 3)
			{
				realTileX -= speed;
			}

			XMFLOAT3 pos(realTileX, realTileY, 0);

			tile->UpdatePosition(pos);
		}
	}

	tile = NULL;
	for (int y = 0; y < m_MapHeight; y++)
	{
		for (int x = 0; x < m_MapWidth; x++)
		{
			PlatformerTile* tile = m_TempTiles(x, y);

			if (tile == NULL) continue;

			// Update the offsets for the tile
			int realTileX = tile->GetPosition().x;// PlatformerMap::TilesToPixels(x) + m_Offset.x;
			int realTileY = tile->GetPosition().y;//PlatformerMap::TilesToPixels(y) + m_Offset.y;

			if (dirUD == 0)
			{
				realTileY += speed;
			}
			else if (dirUD == 1)
			{
				realTileY -= speed;
			}

			if (dirLR == 2)
			{
				realTileX += speed;
			}
			else if (dirLR == 3)
			{
				realTileX -= speed;
			}

			XMFLOAT3 pos(realTileX, realTileY, 0);

			tile->UpdatePosition(pos);
		}
	}*/

	// Update the offsets for the start tile
	{
		int realTileX = startTile->GetPosition().x;// PlatformerMap::TilesToPixels(x) + m_Offset.x;
		int realTileY = startTile->GetPosition().y;//PlatformerMap::TilesToPixels(y) + m_Offset.y;

		if (dirUD == 0)
		{
			realTileY += speed;
		}
		else if (dirUD == 1)
		{
			realTileY -= speed;
		}

		if (dirLR == 2)
		{
			realTileX += speed;
		}
		else if (dirLR == 3)
		{
			realTileX -= speed;
		}

		XMFLOAT3 pos(realTileX, realTileY, 0);

		startTile->UpdatePosition(pos);
	}

	// Update the offsets for the end tile
	{
		int realTileX = endTile->GetPosition().x;// PlatformerMap::TilesToPixels(x) + m_Offset.x;
		int realTileY = endTile->GetPosition().y;//PlatformerMap::TilesToPixels(y) + m_Offset.y;

		if (dirUD == 0)
		{
			realTileY += speed;
		}
		else if (dirUD == 1)
		{
			realTileY -= speed;
		}

		if (dirLR == 2)
		{
			realTileX += speed;
		}
		else if (dirLR == 3)
		{
			realTileX -= speed;
		}

		XMFLOAT3 pos(realTileX, realTileY, 0);

		endTile->UpdatePosition(pos);
	}
}
//=================================================================================================================
void PlatformerMap::LoadSpritesMain()
{
	AISprite* spr;
	for (int sprite_num = 0; sprite_num < m_Sprites.size(); sprite_num++)
	{
		spr = m_Sprites[sprite_num];

		if (spr != NULL)
		{
			spr->AddScript(spr->ScriptName());
			if (spr->GetScript() != NULL)
				spr->Main();
		}
	}

	ClipSprites();
}
//=================================================================================================================
void PlatformerMap::TransformMap(HWND hwnd, float dt, Keyboard keyboard)
{
	//if (!bEditor) return;
	
	//AISprite* player = GetPlayer();
	
	//if (player == NULL) return;

	//XMFLOAT3 playerPos;
	//XMFLOAT3 playerHBPos;

	//playerPos = player->TopLeftPosition();
	//playerHBPos = player->GetHardBoxSpr()->TopLeftPosition();

	//float speed = player->Speed();
	
	//player->Update(dt, keyboard);

	/*if (keyboard.IsKeyDown(Keyboard::Key::W))
	{
		//playerPos.y -= speed;
		//playerHBPos.y -= speed;
		player->SetMoveSpriteUp(true);
		player->SetMoveSpriteDown(false);
	}
	else if (keyboard.IsKeyDown(Keyboard::Key::S))
	{
		//playerPos.y += speed;
		//playerHBPos.y += speed;
		player->SetMoveSpriteDown(true);
		player->SetMoveSpriteUp(false);
	}
	else
	{
		player->SetMoveSpriteUp(false);
		player->SetMoveSpriteDown(false);
	}

	if (keyboard.IsKeyDown(Keyboard::Key::A))
	{
		//playerPos.x -= speed;
		//playerHBPos.x -= speed;
		player->SetMoveSpriteLeft(true);
		player->SetMoveSpriteRight(false);
	}
	else if (keyboard.IsKeyDown(Keyboard::Key::D))
	{
		//playerPos.x += speed;
		//playerHBPos.x += speed;
		player->SetMoveSpriteRight(true);
		player->SetMoveSpriteLeft(false);
	}
	else
	{
		player->SetMoveSpriteLeft(false);
		player->SetMoveSpriteRight(false);
	}*/

	// Clip the map sprite to the map
	//ClipSpriteToMap(player, speed, playerPos.x, playerPos.y, playerHBPos.x, playerHBPos.y);

	//Move the map sprite
	//player->Move(playerPos.x, playerPos.y, playerHBPos.x, playerHBPos.y);

	/*int dirUD = -1;
	int dirLR = -1;
	bool moved = false;

	if (player->MoveSpriteUp())
	{
		dirUD = 0;
		moved = true;
	}
	if (player->MoveSpriteDown())
	{
		dirUD = 1;
		moved = true;
	}

	if (player->MoveSpriteLeft())
	{
		dirLR = 2;
		moved = true;
	}
	if (player->MoveSpriteRight())
	{
		dirLR = 3;
		moved = true;
	}

	if (moved)
	{
		MoveTiles(speed, dirUD, dirLR);

		for (int i = 0; i < m_ClippedSprites.size(); i++)
		{
			if (m_ClippedSprites[i] == NULL) continue;

			AISprite* s = m_ClippedSprites[i];

			if (s->IsPlayer()) continue;

			s->Update(dt, keyboard);

			float newX = s->Position().x;
			float newY = s->Position().y;

			float newHBX = s->GetHardBoxSpr()->Position().x;
			float newHBY = s->GetHardBoxSpr()->Position().y;

			if (dirUD == 0)
			{
				newY += speed;
				newHBY += speed;
			}
			else if (dirUD == 1)
			{
				newY -= speed;
				newHBY -= speed;
			}

			if (dirLR == 2)
			{
				newX += speed;
				newHBX += speed;
			}
			else if (dirLR == 3)
			{
				newX -= speed;
				newHBX -= speed;
			}

			s->Move(newX, newY, newHBX, newHBY);
		}
	}*/
}
//==================================================================================================================================
bool PlatformerMap::Render(Camera* camera)
{
	// Grab the offset of the player so we can properly setup tile rendering
	//m_Offset = PlayerOffset();
	
	//int firstTileX = PlatformerMap::PixelsToTiles(-m_Offset.x);
	//int lastTileX = firstTileX + PlatformerMap::PixelsToTiles(m_EngineOptions->m_screenWidth) + 1;
	
	//for (int y = 0; y < m_MapHeight; y++)
	//{
	//	for (int x = firstTileX; x <= lastTileX; x++)
	//	{
	//		if (x < 0) continue;
	//		
	//		if (x == m_TileRemovedX && y == m_TileRemovedY) continue;

	//		PlatformerTile* tile = m_Tiles(x, y);
	//		
	//		if (tile == NULL) continue;
	//		
	//		// Update the offsets for the tile
	//		int tileX = PlatformerMap::TilesToPixels(x) + m_Offset.x;
	//		int tileY = PlatformerMap::TilesToPixels(y) + m_Offset.y;
	//		
	//		tile->UpdatePosition(XMFLOAT3(tileX, tileY, 0));
	//		
	//		tile->Render(textureShader, transparentShader, camera);
	//	}
	//}
	
	AISprite* player = GetPlayer();
	
	if (!bEditor)
	{
		if (player != NULL)
		{
			// Keep the player in the center of the screen
			m_Offset.x = m_EngineOptions->m_screenWidth / 2 - player->Width() / 2 - player->TopLeftPosition().x;
			m_Offset.y = m_EngineOptions->m_screenHeight / 2 - player->Height() / 2 - player->TopLeftPosition().y;
			
			float levelExtentX = static_cast<float>(m_EngineOptions->m_screenWidth * m_BackgroundScreenWidth);
			float levelExtentY = static_cast<float>(m_EngineOptions->m_screenHeight) * 2.0f;
			
			// Limit horizontal scrolling
			if (player->TopLeftPosition().x < m_EngineOptions->m_screenWidth / 2 - player->Width() / 2)
				m_Offset.x = 0.0f;
			
			if (player->TopLeftPosition().x > levelExtentX - m_EngineOptions->m_screenWidth / 2 - player->Width() / 2)
				m_Offset.x = -(levelExtentX - m_EngineOptions->m_screenWidth);
			
			// Limit vertical scrolling
			if (player->TopLeftPosition().y >= m_EngineOptions->m_screenHeight / 2 - player->Height() / 2)
				m_Offset.y = 0.0f;
			
			if (player->TopLeftPosition().y < -levelExtentY + m_EngineOptions->m_screenHeight / 2 - player->Height() / 2)
				m_Offset.y = levelExtentY - m_EngineOptions->m_screenHeight;
		}
	}
	
	mMapWorld = XMMatrixTranslation(m_Offset.x, m_Offset.y, 0);
	
	int firstTileX = PlatformerMap::PixelsToTiles(-m_Offset.x);
	int lastTileX = firstTileX + PlatformerMap::PixelsToTiles(m_EngineOptions->m_screenWidth) + 1;

	for (int y = 0; y < m_MapHeight; y++)
	{
		for (int x = firstTileX; x <= lastTileX; x++)
		//for (int x = 0; x < m_MapWidth; x++)
		{
			if (!InBounds(x, y)) continue;
			
			PlatformerTile* tile = m_Tiles(x, y);

			if (tile == NULL) continue;

			tile->BodyBoundsPredict(-m_Offset.x, -m_Offset.y);
			
			int x_pos = PlatformerMap::TilesToPixels(tile->GetPosition().x);
			int y_pos = PlatformerMap::TilesToPixels(tile->GetPosition().y);

			if (!InScreen(x_pos, y_pos)) continue;

			//if (tile->BBodyBox().left < -m_Offset.x + m_EngineOptions->m_screenWidth && tile->BBodyBox().right >= -m_Offset.x &&
			//	tile->BBodyBox().top < -m_Offset.y + m_EngineOptions->m_screenHeight && tile->BBodyBox().bottom >= -m_Offset.y)
			if (tile->Body().x < -m_Offset.x + m_EngineOptions->m_screenWidth && tile->Body().x >= -m_Offset.x &&
				tile->Body().y < -m_Offset.y + m_EngineOptions->m_screenHeight && tile->Body().y >= -m_Offset.y)
			{
				//tile->TileWorld() = mMapWorld;
				
				float oldX = tile->TopLeftPosition().x;
				float oldY = tile->TopLeftPosition().y;
				
				float posx = PlatformerMap::TilesToPixels(tile->TopLeftPosition().x) + -m_Offset.x;
				float posy = PlatformerMap::TilesToPixels(tile->TopLeftPosition().y) + -m_Offset.y;
				
				tile->UpdatePosition(XMFLOAT3(posx, posy, 0));
				
				m_D3DSystem->TurnOnAlphaBlending();
				tile->Render(camera, 0);
				m_D3DSystem->TurnOffAlphaBlending();
				
				tile->UpdatePosition(XMFLOAT3(oldX, oldY, 0));
			}
		}
	}

	int startboundx = startTile->GetPosition().x;
	int startboundy = startTile->GetPosition().y;

	int endboundx = endTile->GetPosition().x;
	int endboundy = endTile->GetPosition().y;

	m_BoundingBox.firstTileX = startboundx;
	m_BoundingBox.firstTileY = startboundy;
	m_BoundingBox.lastTileX = endboundx;
	m_BoundingBox.lastTileY = endboundy;

	if (m_TileRemovedX != -1 && m_TileRemovedY != -1)
	{
		m_TileRemovedX = -1;
		m_TileRemovedY = -1;
	}

	ClipSprites();

	for (int i = 0; i < m_ClippedSprites.size(); i++)
	{
		if (m_ClippedSprites[i] == NULL) continue;

		AISprite* s = m_ClippedSprites[i];

		//Update the sprites behavior if their are any
		s->updateBehavior();

		//Update the animation if it exists
		s->UpdateAnimation();
		
		//s->SpriteWorld() = mMapWorld;
		
		float oldX = s->TopLeftPosition().x;
		float oldY = s->TopLeftPosition().y;
		XMFLOAT3 oldPos = XMFLOAT3(oldX, oldY, 0);
		
		if (!s->IsPlayer())
		{
			float posx = s->TopLeftPosition().x + -m_Offset.x;
			float posy = s->TopLeftPosition().y + -m_Offset.y;
			
			XMFLOAT3 pos = XMFLOAT3(posx, posy, 0);
			
			s->TopLeftPosition() = pos;
		}
		
		//Render the sprite
		m_D3DSystem->TurnOnAlphaBlending();
			//s->RenderTransparent11(transparentShader, camera, 0);
			s->Render(camera, 0);
		m_D3DSystem->TurnOffAlphaBlending();
		
		s->TopLeftPosition() = oldPos;
	}

	if (bEditor)
	{
		//m_D3DSystem->TurnOnAlphaBlending();
		//m_MapSprite->Render11(0, textureShader, camera);
		//m_D3DSystem->TurnOffAlphaBlending();
	}

	return true;
}
//==================================================================================================================================