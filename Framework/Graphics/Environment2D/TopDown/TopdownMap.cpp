#include "TopdownMap.h"
#include "AnimationProfile2D.h"
#include "GameDirectory.h"
#include <sstream>
//#include "System.h"
#include "ISOHelper.h"
#include <CommCtrl.h>
//#include "Commands.h"
#include "SectionAdjacency.h"
#include "SectorBuilder.h"
#include "BinarySearch.h"
//=================================================================================================================
int TopdownMap::MAX_VISION = 25;
//=================================================================================================================
TopdownMap::TopdownMap(D3D* d3d, Map2DType mt, Camera* cam) :
	m_D3DSystem(d3d),
	m_camera(cam),
	m_MapType(mt),
	mIsEditor(false),
	m_currentVision(0),
	sorted(true),
	m_insertSpeed(0),
	m_numSpritesRendered(0),
	spriteNum(0),
	m_transforming(false),
	m_upEdge(false),
	m_downEdge(false),
	m_leftEdge(false),
	m_rightEdge(false),
	m_currentMusic("NONE"),
	mResized(false),
	m_bFOW(false),
	m_iFOWRadius(5),
	m_bFOWFlashlight(false)
{
	m_engineOptions = d3d->GetEngineOptions();

	sprite_list.resize( MAX_SPRITES );
}
//=================================================================================================================
TopdownMap::TopdownMap(const TopdownMap& other)
{
}
//=================================================================================================================
TopdownMap::~TopdownMap()
{
}
//=================================================================================================================
bool TopdownMap::Initialize(GameDirectory2D* gd, HWND hwndPB)
{
	m_tileSystem = new TileSystem(m_D3DSystem);

	if (!m_tileSystem)
	{
		return false;
	}

	m_tileSystem->Initialize();

	string path = gd->m_required_textures;
	//path.append("\\Textures");

	int tileID = 0;
	int secid = 0;
	for (int secCol = 0; secCol < m_engineOptions->SECTION_COL_SIZE; secCol++)
	{
		for (int secRow = 0; secRow < m_engineOptions->SECTION_ROW_SIZE; secRow++)
		{
			for (int x = 0; x < m_engineOptions->ROW_SIZE; x++)
			{
				for (int y = 0; y < m_engineOptions->COL_SIZE; y++)
				{
					int xs = x * m_engineOptions->TILE_SIZE;
					int ys = y * m_engineOptions->TILE_SIZE;

					xs = xs + (secCol * (m_engineOptions->ROW_SIZE * m_engineOptions->TILE_SIZE));
					ys = ys + (secRow * (m_engineOptions->COL_SIZE * m_engineOptions->TILE_SIZE));

					if (m_MapType == ISOMETRIC)
					{
						XMFLOAT2 v2 = IsoTo2D(x, y);

						xs = v2.x;
						ys = v2.y;
					}

					string textureFilename = "PatchyGrass04.png";

					#pragma region Map Boundaries
					//North Boundary
					if (secRow == 0 && y == 0)
					{
						//textureFilename = "concrete.bmp";
						mNorthSections.push_back( secid );
					}

					//East Boundary
					if (secCol == 0 && x == 0)
					{
						//textureFilename = "stones.bmp";
						mEastSections.push_back( secid );
					}

					//South Boundry
					if (secRow == m_engineOptions->SECTION_ROW_SIZE - 1 && y == m_engineOptions->COL_SIZE - 1)
					{
						//textureFilename = "concrete.bmp";
						mSouthSections.push_back( secid );
					}

					//West Boundary
					if (secCol == m_engineOptions->SECTION_COL_SIZE - 1 && x == m_engineOptions->ROW_SIZE - 1)
					{
						//textureFilename = "concrete.bmp";
						mWestSections.push_back( secid );
					}
					#pragma endregion

					m_tileSystem->CreateTileSection(secid, x, y, m_engineOptions->TILE_SIZE, false, 0, "Name.mid", XMFLOAT3(xs, ys, 0),
						gd, textureFilename, true, "NONE", 0, m_engineOptions->TILE_SIZE, m_engineOptions->TILE_SIZE);

					/*#pragma region Load Section Boundaries
					Section* sec = m_tileSystem->GetSection(secid);
					if (y == 0)
						sec->AddNorthBoundary(ys);
					if (y == m_engineOptions->COL_SIZE - 1)
						sec->AddSouthBoundary(ys);
					if (x == 0)
						sec->AddEastBoundary(xs);
					if (x == m_engineOptions->ROW_SIZE - 1)
						sec->AddWestBoundary(xs);
					#pragma endregion
					*/
				}
			}

			if (mIsEditor) SendMessage(hwndPB, PBM_STEPIT, 0, 0);

			secid++;
		}
	}

	//Now update the bounds for each section
	vector<Section*> sections = m_tileSystem->GetSections();
	for (int sec_id = 0; sec_id < sections.size(); sec_id++)
	{
		sections[sec_id]->UpdateBounds();
	}

	//m_mapstartinfo.playerStartingSection = 0;

	Section* sec = m_tileSystem->GetSection(0);
	XMFLOAT3 p_pos = sec->GetTilePos(0,0);

	m_camera->SetPosition(0, 0, -15.0f);

	BuildSectionNeighbors();

	XMFLOAT3 pp(m_engineOptions->m_screenWidth / 2 - m_engineOptions->TILE_SIZE, m_engineOptions->m_screenHeight / 2 - m_engineOptions->TILE_SIZE, 0);
	AISprite* spr = new AISprite(m_D3DSystem);
	spr->Initialize(path, "player_cover.png", pp, m_engineOptions->TILE_SIZE, m_engineOptions->TILE_SIZE, DYNAMIC);
	spr->CreateHardBoxSpr(path);
	spr->Speed() = 2;
	AddPlayer(spr);

	ClipSections(GetPlayer()->TopLeftPosition());
	ToggleFow();

	/*#pragma region Neighbor Render Test
	if (RENDER_SECTION_TEX_TEST)
	{
		int startingSection = NEIGHBOR_RENDER_TEST_SECTION;

		Section* s = m_tileSystem->GetSection(startingSection);
	
		vector<Section::NeighborIDS> dirs = s->GetNeighbors();

		for (int i = 0; i < dirs.size(); i++)
		{
			int dir = dirs[i].id;

			Section* sdir = m_tileSystem->GetSection(dir);

			string tex = "Background\\Dirt00.png";

			if (dir == 0)
				tex = "Background\\Dirt00.png";
			else if (dir == 1)
				tex = "Background\\Dirt01.png";
			else if (dir == 2)
				tex = "Background\\MossyRock00.png";
			else if (dir == 3)
				tex = "Background\\MossyRock01.png";
			else if (dir == 4)
				tex = "Background\\Grass01Rock02.png";
			else if (dir == 5)
				tex = "Background\\Grass02.png";
			else if (dir == 6)
				tex = "Background\\Grass02Rock00.png";
			else if (dir == 7)
				tex = "Background\\wall.jpg";

			sdir->SetNewTexture(path, tex, m_engineOptions->TILE_SIZE, m_engineOptions->TILE_SIZE);
		}
	}
	#pragma endregion*/

	//Need to create a bounding box for each section to see if the player's x,y
	//lies inside the box.

	//Rendering Spill
	/*
		The player will start in a section each section will know its neighbors
		the section the player is in will be rendered along with 1 sections of neighbors deep
		maximum of about 9 sections (3x3 section area).

		Need to build the N,S,E,W boundaries for each section so we can tell when the
		player moves to a new section.

		Need to store the camera position of each section

		*the rendering is done according to the position of the camera, not the position of the player

		Need to figure out what kind of fog of war or masking thing to use in
		covering every one of the sections except for the section the player is in
	*/

	//Need to be able to toggle fog on and off
	
	m_sleepTimer = new SleepTimer();
	if (!m_sleepTimer)
	{
		return false;
	}

	//Create a sprite (player test) (300, 300)
	//player = new AISprite(m_D3DSystem);
	//player->Initialize(path, "player_cover.png", "Render", XMFLOAT3(p_pos.x + 100, p_pos.y + 100, 0), m_engineOptions->TILE_SIZE, m_engineOptions->TILE_SIZE);
	
	//Load a animation profile
	//AnimationProfile2D* profile = new AnimationProfile2D();
	//profile->LoadAnimationProfileNames("HUNTER", "SPRITE", gd->GAME_FOLDER);
	//profile->ReadProfile("HUNTER", "SPRITE", "Textures", gd->GAME_FOLDER);

	////Add the animation to the player
	//vector<Animation2D*> anims = profile->GetAnimations();
	//for (int i = 0; i < anims.size(); i++)
	//{
	//	player->AddAnimationSequence(anims[i], d3d, m_engineOptions, m_engineOptions->m_screenWidth, m_engineOptions->m_screenHeight);
	//}

	//player->SetSequence(IDLE_DOWN);
	
	//Create a sword and put it on the map
	//sword = new Sprite();
	//sword->Initialize("Textures", "sword.png", "Render", XMFLOAT3(600, 500, 0), m_engineOptions->m_screenWidth, m_engineOptions->m_screenHeight, d3d, 46, 21);

	return true;
}
//=================================================================================================================
bool TopdownMap::InitializeTiles(MapLoadData mld, GameDirectory2D* gd, HWND hwndPB)
{
	m_tileSystem = new TileSystem(m_D3DSystem);

	if (!m_tileSystem)
	{
		return false;
	}

	m_tileSystem->Initialize();

	if (m_MapType == ISOMETRIC)
	{
		//XMFLOAT2 v2 = IsoTo2D(x * m_engineOptions->TILE_SIZE, y * m_engineOptions->TILE_SIZE);


	}

	int secid = 0;
	for (int secCol = 0; secCol < m_engineOptions->SECTION_COL_SIZE; secCol++)
	{
		for (int secRow = 0; secRow < m_engineOptions->SECTION_ROW_SIZE; secRow++)
		{
			int tile_id = 0;
			for (int y = 0; y < m_engineOptions->COL_SIZE; y++)
			{
				for (int x = 0; x < m_engineOptions->ROW_SIZE; x++)
				{
					int xs = x * m_engineOptions->TILE_SIZE;
					int ys = y * m_engineOptions->TILE_SIZE;

					xs = xs + (secCol * (m_engineOptions->ROW_SIZE * m_engineOptions->TILE_SIZE));
					ys = ys + (secRow * (m_engineOptions->COL_SIZE * m_engineOptions->TILE_SIZE));

					//string textureFilename = "Background\\PatchyGrass04.png";

					#pragma region Map Boundaries
					//North Boundary
					if (secRow == 0 && y == 0)
					{
						//textureFilename = "concrete.bmp";
						mNorthSections.push_back( secid );
					}

					//East Boundary
					if (secCol == 0 && x == 0)
					{
						//textureFilename = "stones.bmp";
						mEastSections.push_back( secid );
					}

					//South Boundry
					if (secRow == m_engineOptions->SECTION_ROW_SIZE - 1 && y == m_engineOptions->COL_SIZE - 1)
					{
						//textureFilename = "concrete.bmp";
						mSouthSections.push_back( secid );
					}

					//West Boundary
					if (secCol == m_engineOptions->SECTION_COL_SIZE - 1 && x == m_engineOptions->ROW_SIZE - 1)
					{
						//textureFilename = "concrete.bmp";
						mWestSections.push_back( secid );
					}
					#pragma endregion

					int currentVision = mld.sectionVision[secid];
					string currentMusicName = mld.sectionMusicName[secid];

					SectionTileData std = mld.sectionTileData[secid];

					string textureFilename = std.imageNames[tile_id];
					string animProfileName = std.animProfileNames[tile_id];
					int animSpeed = std.animSpeeds[tile_id];
					int hard = std.hards[tile_id];

					m_tileSystem->CreateTileSection(secid, x, y, m_engineOptions->TILE_SIZE, hard, currentVision, currentMusicName, XMFLOAT3(xs, ys, 0),
						gd, textureFilename, false, animProfileName, animSpeed, m_engineOptions->TILE_SIZE, m_engineOptions->TILE_SIZE);

					/*#pragma region Load Section Boundaries
					Section* sec = m_tileSystem->GetSection(secid);
					if (y == 0)
						sec->AddNorthBoundary(ys);
					if (y == m_engineOptions->COL_SIZE - 1)
						sec->AddSouthBoundary(ys);
					if (x == 0)
						sec->AddEastBoundary(xs);
					if (x == m_engineOptions->ROW_SIZE - 1)
						sec->AddWestBoundary(xs);
					#pragma endregion*/

					tile_id++;
				}
			}

			//int pb_pos = SendMessage(hwndPB, PBM_GETPOS, 0, 0);
			//pb_pos += 1; // increase
			//SendMessage(hwndPB, PBM_SETPOS, pb_pos, 0);

			if (mIsEditor) SendMessage(hwndPB, PBM_STEPIT, 0, 0);
			
			secid++;
		}
	}

	//Now update the bounds for each section
	vector<Section*> sections = m_tileSystem->GetSections();
	for (int sec_id = 0; sec_id < sections.size(); sec_id++)
	{
		sections[sec_id]->UpdateBounds();
	}

	return true;
}
//=================================================================================================================
bool TopdownMap::InitializePositionsAndPlayerAndRenderingSections(int startingSection, string editor_path)
{
	//m_mapstartinfo.playerStartingSection = startingSection;

	Section* sec = m_tileSystem->GetSection(startingSection);
	XMFLOAT3 p_pos = sec->GetTilePos(0,0);

	m_camera->SetPosition(0, 0, -15.0f);

	//p_pos.x = 200;
	//p_pos.y = 200;

	//Create a sprite (player test) (300, 300)
	// XMFLOAT3(p_pos.x + 100, p_pos.y + 100, 0)
	XMFLOAT3 pp(m_engineOptions->m_screenWidth / 2 - m_engineOptions->TILE_SIZE, m_engineOptions->m_screenHeight / 2 - m_engineOptions->TILE_SIZE, 0);
	AISprite* spr = new AISprite(m_D3DSystem);
	spr->Initialize(editor_path, "player_cover.png", pp, m_engineOptions->TILE_SIZE, m_engineOptions->TILE_SIZE, DYNAMIC);
	spr->CreateHardBoxSpr(editor_path);
	spr->Speed() = 2;
	AddPlayer(spr);

	//player = new Sprite();
	//player->Initialize("Background\\SandyRock00.png", "Render", XMFLOAT3(p_pos.x + 100,
	//	p_pos.y + 100, 0), m_engineOptions->m_screenWidth, m_engineOptions->m_screenHeight, d3d,
	//	m_engineOptions->TILE_SIZE, m_engineOptions->TILE_SIZE);

	//float mpX = p_pos.x + 400;
	//float mpY = p_pos.y + 400;
	//mapSprite = new Sprite();//was (p_pos.x + 600)
	//mapSprite->Initialize(editor_path, "MossyRock00.png", "Render",
	//	XMFLOAT3(mpX, mpY, 0),
	//	m_engineOptions->m_screenWidth, m_engineOptions->m_screenHeight, d3d,
	//	m_engineOptions->TILE_SIZE, m_engineOptions->TILE_SIZE);

	//m_camera->setMapPosition(mpX, mpY, 0);

	//XMFLOAT2 secPos = CameraSectionPosition(startingSection, m_engineOptions->SECTION_ROW_SIZE, m_engineOptions->SECTION_COL_SIZE);

	//m_camera->SetPosition(secPos.x, secPos.y, -15.0f);

	//sec->ReTexBounds(d3d, screenWidth, screenHeight, engineOptions.TILE_SIZE, engineOptions.TILE_SIZE);

	BuildSectionNeighbors();

	//LoadRenderSections(m_mapstartinfo.playerStartingSection);

	ClipSections(GetPlayer()->TopLeftPosition());

	ToggleFow();

	//Create a sword and put it on the map
	//sword = new Sprite();
	//sword->Initialize("Textures", "sword.png", "Render", XMFLOAT3(600, 500, 0), m_engineOptions->m_screenWidth, m_engineOptions->m_screenHeight, d3d, 46, 21);

	return true;
}
//=================================================================================================================
void TopdownMap::LoadSpriteFOWTiles(Sprite* sprite)
{
	if (m_bFOW)
	{
		float playerX = sprite->TopLeftPosition().x;
		float playerY = sprite->TopLeftPosition().y;

		float length = (m_iFOWRadius * 2) + 1;

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
				northSection = m_tileSystem->GetSection(as.ID);
			}
			if (as.neighborPos == SOUTH)
			{
				southSection = m_tileSystem->GetSection(as.ID);
			}
			if (as.neighborPos == EAST)
			{
				eastSection = m_tileSystem->GetSection(as.ID);
			}
			if (as.neighborPos == WEST)
			{
				westSection = m_tileSystem->GetSection(as.ID);
			}
			if (as.neighborPos == NW)
			{
				NWSection = m_tileSystem->GetSection(as.ID);
			}
			if (as.neighborPos == NE)
			{
				NESection = m_tileSystem->GetSection(as.ID);
			}
			if (as.neighborPos == SW)
			{
				SWSection = m_tileSystem->GetSection(as.ID);
			}
			if (as.neighborPos == SE)
			{
				SESection = m_tileSystem->GetSection(as.ID);
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
			float xTo = (playerTile->GetPosition().x + (m_iFOWRadius * m_engineOptions->TILE_SIZE));
			float xBack = (playerTile->GetPosition().x - (m_iFOWRadius * m_engineOptions->TILE_SIZE));

			float yTo = (playerTile->GetPosition().y + (m_iFOWRadius * m_engineOptions->TILE_SIZE));
			float yBack = (playerTile->GetPosition().y - (m_iFOWRadius * m_engineOptions->TILE_SIZE));

			float yy = yBack;
			float xx = xBack;

			int fowY = 0;
			int fowX = 0;

			for (fowY = 0, yy = yBack; fowY < length, yy <= yTo; fowY++, yy += m_engineOptions->TILE_SIZE)
			{
				for (fowX = 0, xx = xBack; fowX < length, xx <= xTo; fowX++, xx += m_engineOptions->TILE_SIZE)
				{
					XMFLOAT3 realTilePos(xx, yy, 0);

					Tile* tile = InitPlayerFOWInSection(section_in, realTilePos);

					if (tile == NULL && northSection != NULL)
					{
						tile = InitPlayerFOWInSection(northSection, realTilePos);
					}

					if (tile == NULL && southSection != NULL)
					{
						tile = InitPlayerFOWInSection(southSection, realTilePos);
					}

					if (tile == NULL && eastSection != NULL)
					{
						tile = InitPlayerFOWInSection(eastSection, realTilePos);
					}

					if (tile == NULL && westSection != NULL)
					{
						tile = InitPlayerFOWInSection(westSection, realTilePos);
					}

					if (tile == NULL && NWSection != NULL)
					{
						tile = InitPlayerFOWInSection(NWSection, realTilePos);
					}

					if (tile == NULL && NESection != NULL)
					{
						tile = InitPlayerFOWInSection(NESection, realTilePos);
					}

					if (tile == NULL && SWSection != NULL)
					{
						tile = InitPlayerFOWInSection(SWSection, realTilePos);
					}

					if (tile == NULL && SESection != NULL)
					{
						tile = InitPlayerFOWInSection(SESection, realTilePos);
					}

					sprite->AddFOWTile(tile);
				}
			}
		}
	}
}
//=================================================================================================================
Tile* TopdownMap::InitPlayerFOWInSection(Section* section, XMFLOAT3 realpos)
{
	for (int yyy = 0; yyy < m_engineOptions->COL_SIZE; yyy++)
	{
		for (int xxx = 0; xxx < m_engineOptions->ROW_SIZE; xxx++)
		{
			Tile* tile2 = section->GetTile(xxx, yyy);

			if (realpos.x == tile2->GetPosition().x && realpos.y == tile2->GetPosition().y)
			{
				return tile2;
			}
		}
	}

	return 0;
}
//=================================================================================================================
bool TopdownMap::LocateStartSection()
{
	//does the player exist
	if (DoesPlayerExist())
	{
		//Grab the coordinates of the player
		float player_x;
		float player_y;
		int player_width;
		int player_height;

		AISprite* spr;
		for (int sprite_num = 0; sprite_num < m_visionSprites.size(); sprite_num++)
		{
			spr = m_visionSprites[sprite_num];

			if (spr->IsPlayer())
			{
				player_x = spr->TopLeftPosition().x;
				player_y = spr->TopLeftPosition().y;
				player_width = spr->Width();
				player_height = spr->Height();
				break;
			}
		}

		//Figure out what section the coordinates are in
		bool bailout = false;
		Tile* tile;
		Section* current_section;
		vector<Section*> sections = m_tileSystem->GetSections();
		//Loop through all the sections in the map
		for (int sec_id = 0; sec_id < sections.size(); sec_id++)
		{
			current_section = sections[sec_id];

			for (int tile_y = 0; tile_y < m_engineOptions->COL_SIZE; tile_y++)
			{
				for (int tile_x = 0; tile_x < m_engineOptions->ROW_SIZE; tile_x++)
				{
					tile = current_section->GetTile(tile_x, tile_y);

					float realTileX = tile->GetPosition().x;
					float realTileY = tile->GetPosition().y;

					if (realTileX >= player_x && realTileX <= player_x + player_width)
					{
						if (realTileY >= player_y && realTileY <= player_y + player_height)
						{
							SelectRenderingSection(sec_id);
							bailout = true;
							break;
						}
					}
				}

				if (bailout) break;
			}
		}

		return true;
	}

	return false;
}
//=================================================================================================================
/*void TopdownMap::TransformWorld(int dir)
{
	float camera_speed = 3.0f;
	bool needToClip = false;

	switch (dir)
	{
		case 0:
		{
			XMMath3 cameraPos = m_camera->Position();

			if (cameraPos.y < 0)
			{
				needToClip = true;
				cameraPos.y += camera_speed;
			}

			m_camera->SetPosition(cameraPos.x, cameraPos.y, cameraPos.z);
			if (needToClip) ClipSections(XMFLOAT3(cameraPos.x, cameraPos.y, cameraPos.z));
			ResetMapCameraPosition(m_engineOptions->m_screenWidth, m_engineOptions->m_screenHeight, false);
		}
		//MessageBox(m_hwnd, "Mouse Going Up", "Mouse Offscreen", MB_OK);
		break;
		case 1:
		{
			XMMath3 cameraPos = m_camera->Position();

			int val = GetCameraDownBounds(m_engineOptions->SECTION_ROW_SIZE, m_engineOptions->SECTION_COL_SIZE);
			if (val != 0)
			{
				needToClip = true;
				cameraPos.y -= camera_speed;
			}

			m_camera->SetPosition(cameraPos.x, cameraPos.y, cameraPos.z);
			if (needToClip) ClipSections(XMFLOAT3(cameraPos.x, cameraPos.y, cameraPos.z));
			ResetMapCameraPosition(m_engineOptions->m_screenWidth, m_engineOptions->m_screenHeight, false);
		}
		//MessageBox(m_hwnd, "Mouse Going Down", "Mouse Offscreen", MB_OK);
		break;
		case 2:
		{
			XMMath3 cameraPos = m_camera->Position();

			if (cameraPos.x > 0)
			{
				needToClip = true;
				cameraPos.x -= camera_speed;
			}

			m_camera->SetPosition(cameraPos.x, cameraPos.y, cameraPos.z);
			if (needToClip) ClipSections(XMFLOAT3(cameraPos.x, cameraPos.y, cameraPos.z));
			ResetMapCameraPosition(m_engineOptions->m_screenWidth, m_engineOptions->m_screenHeight, false);
		}
		//MessageBox(m_hwnd, "Mouse Going Left", "Mouse Offscreen", MB_OK);
		break;
		case 3:
		{
			XMMath3 cameraPos = m_camera->Position();

			int val = GetCameraRightBounds(m_engineOptions->SECTION_ROW_SIZE, m_engineOptions->SECTION_COL_SIZE);
			if (val != 0)
			{
				needToClip = true;
				cameraPos.x += camera_speed;
			}

			m_camera->SetPosition(cameraPos.x, cameraPos.y, cameraPos.z);
			if (needToClip) ClipSections(XMFLOAT3(cameraPos.x, cameraPos.y, cameraPos.z));
			ResetMapCameraPosition(m_engineOptions->m_screenWidth, m_engineOptions->m_screenHeight, false);
		}
		//MessageBox(m_hwnd, "Mouse Going Right", "Mouse Offscreen", MB_OK);
		break;
		default: break;
	}
}
*/
//=================================================================================================================
AISprite* TopdownMap::GetSpriteByID(int id)
{
	AISprite* spr;
	for (int sprite_num = 0; sprite_num < m_visionSprites.size(); sprite_num++)
	{
		spr = m_visionSprites[sprite_num];
		if (spr->ID() == id)
		{
			return spr;
		}
	}

	return NULL;
}
//=================================================================================================================
AISprite* TopdownMap::ClippedSprite(int i)
{
	return sprite_list[i].sprite;
}
//=================================================================================================================
void TopdownMap::LoadSpritesMain()
{
	AISprite* spr;
	for (int sprite_num = 0; sprite_num < m_visionSprites.size(); sprite_num++)
	{
		spr = m_visionSprites[sprite_num];
		//spr->Main(Commands::GetInstance()->GetLuaState(), Commands::GetInstance()->GetGD());
	}
}
//=================================================================================================================
bool TopdownMap::DoesPlayerExist()
{
	Sprite* spr;
	for (int sprite_num = 0; sprite_num < m_visionSprites.size(); sprite_num++)
	{
		spr = m_visionSprites[sprite_num];

		if (spr->IsPlayer())
		{
			return true;
		}
	}

	return false;
}
//=================================================================================================================
void TopdownMap::AddPlayer(AISprite* s)
{
	s->IsPlayer() = true;
	s->ID() = spriteNum++;
	m_visionSprites.push_back( s );
}
//=================================================================================================================
void TopdownMap::AddPlayerNoIndex(AISprite* s)
{
	s->IsPlayer() = true;
	m_visionSprites.push_back( s );
}
//=================================================================================================================
AISprite* TopdownMap::GetPlayer()
{
	AISprite* spr;
	for (int sprite_num = 0; sprite_num < m_visionSprites.size(); sprite_num++)
	{
		spr = m_visionSprites[sprite_num];

		if (spr->IsPlayer())
		{
			return spr;
		}
	}

	return NULL;
}
//=================================================================================================================
void TopdownMap::AddSpriteAnimations(string profileName, string base_game_folder, string base_sprite_path, int id)
{
	//Load a animation profile
	AnimationProfile2D* profile = new AnimationProfile2D(m_D3DSystem);

	bool something_wrong = false;
	
	//if (profile->LoadAnimationProfileNames(profileName, "SPRITE", base_game_folder))
	//{
		if (profile->ReadProfile(profileName, "SPRITE", base_sprite_path, base_game_folder))
		{
			vector<Animation2D*> anims = profile->GetAnimations();

			AISprite* spr;

			for (int sprite_num = 0; sprite_num < m_visionSprites.size(); sprite_num++)
			{
				if (sprite_num == id)
				{
					spr = m_visionSprites[sprite_num];

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
	//}
	//else something_wrong = true;

	if (something_wrong)
	{
		MessageBox(NULL, "The Animation2D does not exist", "Add Animations", MB_OK);
	}
}
//=================================================================================================================
void TopdownMap::AddTileAnimations(string profileName, string base_game_folder, string base_tile_path, float tile_x, float tile_y)
{
	//Load a animation profile
	AnimationProfile2D* profile = new AnimationProfile2D(m_D3DSystem);

	bool something_wrong = false;
	
	//if (profile->LoadAnimationProfileNames(profileName, "TILE", base_game_folder))
	//{
		if (profile->ReadProfile(profileName, "TILE", base_tile_path, base_game_folder))
		{
			vector<Animation2D*> anims = profile->GetAnimations();

			Tile* tile = SelectTile(tile_x, tile_y);

			tile->ClearAnimationList();

			for (int i = 0; i < anims.size(); i++)
			{
				tile->AddAnimationSequence(anims[i]);
			}

			tile->SetSequence(0);
		}
		else something_wrong = true;
	//}
	//else something_wrong = true;

	if (something_wrong)
	{
		MessageBox(NULL, "The Animation2D does not exist", "Add Animations", MB_OK);
	}
}
//=================================================================================================================
void TopdownMap::UpdateTileHardness(int x, int y, bool bHard)
{
	m_tileSystem->UpdateTileHardness(x, y, bHard);
}
//=================================================================================================================
void TopdownMap::MakeTileHard(int x, int y)
{
	m_tileSystem->MakeTileHard(x, y);
}
//=================================================================================================================
void TopdownMap::InitializeSectionPicker(string base_path)
{
	//Will have tiles based on SECTION_ROW_SIZE and SECTION_COL_SIZE.
	//If you click on a tile it will take you to that section in the map.

	//Will have it where you cannot move the screen around on a tile section

	/*
	The reason for doing this is because the selection tool is not following the map right
	when I move the camera around the map.

	Could have a edit mode where I can only edit the chosen section of the map
	but then if I wanted to I could exit edit mode and free roam the map to see
	what it looks like in the editor.
	Then have a button to press that says go back to selected section.
	*/

	m_camera->SetPosition(0, 0, -15.0f);

	m_tileSystem->InitializeOverworldTiles(m_engineOptions->SECTION_ROW_SIZE, m_engineOptions->SECTION_COL_SIZE,
		m_engineOptions->TILE_SIZE / 2, base_path, "overtile.png",
		m_engineOptions->TILE_SIZE / 2, m_engineOptions->TILE_SIZE / 2);
}
//=================================================================================================================
void TopdownMap::RenderSectionPicker(Camera* camera)
{
	m_tileSystem->RenderOverworldTiles(camera);
}
//=================================================================================================================
XMFLOAT2 TopdownMap::GetFirstTilePos(int section)
{
	Section* sec = m_tileSystem->GetSection(section);
	XMFLOAT3 p_pos = sec->GetTilePos(0,0);
	return XMFLOAT2(p_pos.x, p_pos.y);
}
//=================================================================================================================
bool TopdownMap::SelectRenderingSection(int section)
{
	AISprite* player = GetPlayer();
	if (player == NULL) return false;

	ClipSections(player->TopLeftPosition());

	for (;;)
	{
		// Find the new section based on what section the player is in
		Section* section_in = player->SectionIn();
		if (section_in == NULL) return false;
		//SectionAdjacency* as = section_in->AdjacencyInfo();
		//if (as == NULL) return false;

		Section* sec = m_tileSystem->GetSection(section);
		XMFLOAT3 to_sec_pos = sec->GetTilePos(0, 0);
		XMFLOAT3 player_sec_in_pos = section_in->GetTilePos(0, 0);

		// The player is in the new wanted section so we are done
		if (section_in->GetSectionID() == sec->GetSectionID())
		{
			break;
		}

		// 0: north, 1: south, 2: west, 3: east
		int dirUD = -1; // 0 or 1
		int dirLR = -1; // 2 or 3
		
		if (to_sec_pos.x > player_sec_in_pos.x)
		{
			// Head East
			dirLR = 3;
		}

		if (to_sec_pos.x < player_sec_in_pos.x)
		{
			// Head West
			dirLR = 2;
		}

		if (to_sec_pos.y > player_sec_in_pos.y)
		{
			// Head South
			dirUD = 1;
		}

		if (to_sec_pos.y < player_sec_in_pos.y)
		{
			// Head North
			dirUD = 0;
		}

		int speed = 1;

		// Need to make sure the section is the one being rendered on screen
		Section* current_section;
		vector<Section*> sections = m_tileSystem->GetSections();
		//Loop through all the sections in the map
		for (int sec_id = 0; sec_id < sections.size(); sec_id++)
		{
			current_section = sections[sec_id];

			current_section->MoveTiles(speed, dirUD, dirLR);
		}

		// Also move the sprites
		for (int i = 0; i < m_visionSprites.size(); i++)
		{
			if (m_visionSprites[i] == NULL) continue;

			AISprite* s = m_visionSprites[i];

			if (s->IsPlayer()) continue;

			float newX = s->TopLeftPosition().x;
			float newY = s->TopLeftPosition().y;

			float newHBX = s->GetHardBoxSpr()->TopLeftPosition().x;
			float newHBY = s->GetHardBoxSpr()->TopLeftPosition().y;

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

		// Make sure the player knows what section it is in so we can get closer to the new section
		ClipSections(player->TopLeftPosition());
	}
	//XMFLOAT2 secPos = CameraSectionPosition(section, m_engineOptions->SECTION_ROW_SIZE, m_engineOptions->SECTION_COL_SIZE);

	//m_camera->SetPosition(secPos.x, secPos.y, -15.0f);

	//ClipSections(XMFLOAT3(m_camera->Position().x, m_camera->Position().y, m_camera->Position().z));
	
	//ResetMapCameraPosition(m_engineOptions->m_screenWidth, m_engineOptions->m_screenHeight, false);

	return true;
}
//=================================================================================================================
bool TopdownMap::IsPosInTile(XMFLOAT2 pos, XMFLOAT2 &r, bool pInSectionPicker)
{
	return m_tileSystem->IsPosInTile(pos, r, pInSectionPicker, mSectionRenderList);
}
//=================================================================================================================
void TopdownMap::UpdateTileTexture(int x, int y, string base_tile_path, string textureFilename, int bitmapWidth, int bitmapHeight)
{
	m_tileSystem->UpdateTileTexture(x, y, base_tile_path, textureFilename, bitmapWidth, bitmapHeight);
}
//=================================================================================================================
Tile* TopdownMap::GetTile(int x, int y)
{
	return m_tileSystem->GetTile(x, y);
}
//=================================================================================================================
void TopdownMap::UndoTileTextures(vector<string> revertNames, vector<Tile*> tiles, string base_tile_path)
{
	//Place textures at x, y to which the tiles list contains
	//Also if the tile does not contain any texture it will be NULL
	for (int i = 0; i < tiles.size(); i++)
	{
		Tile* tile = tiles[i];

		ZShadeSandboxMesh::QuadMesh* bitmap = tile->GetBitmap();
		//We want to remove the texture from the new stamped tile

		if (revertNames[i] != "")
		{
			UpdateTileTexture(tile->GetPosition().x, tile->GetPosition().y, base_tile_path, revertNames[i], m_engineOptions->TILE_SIZE, m_engineOptions->TILE_SIZE);
		}
		else
		{
			//Nullify the texture
			Tile* tileToReplace = m_tileSystem->SelectTile(tile->GetPosition().x, tile->GetPosition().y);
			tileToReplace->DisableTexture();
		}
	}
}
//=================================================================================================================
void TopdownMap::RedoTileTextures(vector<string> revertNames, vector<Tile*> tiles, string base_tile_path)
{
	for (int i = 0; i < tiles.size(); i++)
	{
		Tile* tile = tiles[i];

		ZShadeSandboxMesh::QuadMesh* bitmap = tile->GetBitmap();
		//We want to remove the texture from the new stamped tile

		if (tile->GetTileTextureName() != "")
		{
			UpdateTileTexture(tile->GetPosition().x, tile->GetPosition().y, base_tile_path, tile->GetTileTextureName(), m_engineOptions->TILE_SIZE, m_engineOptions->TILE_SIZE);
		}
		else
		{
			//Nullify the texture
			Tile* tileToReplace = m_tileSystem->SelectTile(tile->GetPosition().x, tile->GetPosition().y);
			tileToReplace->DisableTexture();
		}
	}
}
//=================================================================================================================
void TopdownMap::ReloadSprites(string base_editor_path, string base_sprite_path, string base_tile_path)
{
	sword = new Sprite(m_D3DSystem);
	sword->Initialize("Textures", "sword.png", XMFLOAT3(600, 500, 0), 46, 21, STATIC);

	//Reload the map sprite and player
	float msX = mapSprite->TopLeftPosition().x;
	float msY = mapSprite->TopLeftPosition().y;
	mapSprite = new Sprite(m_D3DSystem);
	mapSprite->Initialize("Textures", "Background\\MossyRock00.png", XMFLOAT3(msX, msY, 0), 64, 64, DYNAMIC);

	//int pX = player->GetPosition().x;
	//int pY = player->GetPosition().y;
	//player = new Sprite();
	//player->Initialize("Background\\MossyRock00.png", "Render", XMFLOAT3(pX, pY, 0), m_engineOptions->m_screenWidth, m_engineOptions->m_screenHeight, d3d, 64, 64);

	//Reload the sprites in this section
	for (int j = 0; j < m_visionSprites.size(); j++)
	{
		Sprite* s = m_visionSprites[j];
		float X = s->TopLeftPosition().x;
		float Y = s->TopLeftPosition().y;
		string textureFilename = s->BaseTextureFilename();
		int bitmapWidth = s->Width();
		int bitmapHeight = s->Height();
		s->Initialize(base_sprite_path, textureFilename,
			XMFLOAT3(X, Y, 0),
			bitmapWidth, bitmapHeight, s->EPhysicsType());
		s->CreateHardBoxSpr(base_editor_path);
	}

	//Reload all the tiles in the map
	m_tileSystem->ReloadSprites(base_tile_path);
}
//-----------------------------------------------------------------------------------------------------------------
void TopdownMap::SetSpriteHardboxVisible(bool visible)
{
	for (int j = 0; j < m_visionSprites.size(); j++)
	{
		Sprite* s = m_visionSprites[j];
		s->CanDisplayHardbox() = visible;
	}

	//Toggle the tile hardbox
	m_tileSystem->SetTileDisplayHardbox(visible);
}
//=================================================================================================================
bool IsStraightLine(float sx, float sy, float dx, float dy, int &dir)
{
	dir = 0;

	if (sx == dx && sy != dy)
	{
		if (sy < dy) dir = 3;
		if (sy > dy) dir = 4;

		return true;
	}

	if (sy == dy && sx != dx)
	{
		if (sx < dx) dir = 1;
		if (sx > dx) dir = 2;

		return true;
	}

	return false;
}
//=================================================================================================================
bool IsDiagonalLine(float sx, float sy, float dx, float dy)
{
	if (dx - sx == sy - dy)
		return true;

	if (dx - sx == dy - sy)
		return true;

	return false;
}
//=================================================================================================================
void TopdownMap::BuildSectionNeighbors()
{
	// Load the adjacency information for each section
	
	Section* current_section;
	vector<Section*> sections = m_tileSystem->GetSections();
	//Loop through all the sections in the map
	for (int sec_id = 0; sec_id < sections.size(); sec_id++)
	{
		current_section = sections[sec_id];

		SectionAdjacency* adj = NULL;

		if (m_engineOptions->SECTION_COL_SIZE == 3 && m_engineOptions->SECTION_ROW_SIZE == 3)
		{
			adj = TileAdjInfo3x3::tileAdjInfo[sec_id].get();
		}
		if (m_engineOptions->SECTION_COL_SIZE == 6 && m_engineOptions->SECTION_ROW_SIZE == 6)
		{
			adj = TileAdjInfo6x6::tileAdjInfo[sec_id].get();
		}
		if (m_engineOptions->SECTION_COL_SIZE == 8 && m_engineOptions->SECTION_ROW_SIZE == 8)
		{
			adj = TileAdjInfo8x8::tileAdjInfo[sec_id].get();
		}
		if (m_engineOptions->SECTION_COL_SIZE == 10 && m_engineOptions->SECTION_ROW_SIZE == 10)
		{
			adj = TileAdjInfo10x10::tileAdjInfo[sec_id].get();
		}
		if (m_engineOptions->SECTION_COL_SIZE == 20 && m_engineOptions->SECTION_ROW_SIZE == 20)
		{
			adj = TileAdjInfo20x20::tileAdjInfo[sec_id].get();
		}
		if (m_engineOptions->SECTION_COL_SIZE == 30 && m_engineOptions->SECTION_ROW_SIZE == 30)
		{
			adj = TileAdjInfo30x30::tileAdjInfo[sec_id].get();
		}

		current_section->SetAdjacencyInfo(adj);
	}
}
//=================================================================================================================
void TopdownMap::LoadRenderSections(int section)
{
	/*if (mSectionRenderList.size() > 0) mSectionRenderList.clear();
	mSectionRenderList.push_back( section );
	//Load all the neighbors for the section render list
	Section* s = m_tileSystem->GetSection( section );
	vector<Section::NeighborIDS> dirs = s->GetNeighbors();
	for (int i = 0; i < dirs.size(); i++)
	{
		mSectionRenderList.push_back( dirs[i].id );
	}*/
}
//=================================================================================================================
void TopdownMap::ClipSprites()
{
	vector<SPRQUE> renderList(MAX_SPRITES);

	for (int j = 0; j < sprite_list.size(); j++)
	{
		if (sprite_list[j].sprite == NULL) continue;

		AISprite* s = sprite_list[j].sprite;
		int depth = sprite_list[j].depth;
		int sn = sprite_list[j].sprite_num;

		/*if (s->IsPlayer() || s->IsDisplaySprite())
		{
			SPRQUE sque;
			sque.sprite = s;
			sque.depth = depth;
			sque.sprite_num = sn;
			renderList[j] = sque;
			continue;
		}*/

		//If a sprite is not in any rendering section do not render it
		//Check to see if the sprite is in any one of the rendering sections
		for (int t = 0; t < mSectionRenderList.size(); t++)
		{
			int currSection = mSectionRenderList[t];

			Section* sec = m_tileSystem->GetSection(currSection);

			Section::BB bounds = sec->m_BoundingBox;
			
			//If moveable sprites move off the map
			//  we still need to see them since they will
			//  be running against the edge of the map

			int w = s->Width();
			int h = s->Height();

			if (s->TopLeftPosition().x >= bounds.firstTileX - w && s->TopLeftPosition().x <= bounds.lastTileX + w)
			{
				if (s->TopLeftPosition().y >= bounds.firstTileY - h && s->TopLeftPosition().y <= bounds.lastTileY + h)
				{
					//The sprite is in the section
					SPRQUE sque;
					sque.sprite = s;
					sque.depth = depth;
					sque.sprite_num = sn;
					renderList[j] = sque;
					break;
				}
			}
		}
	}

	//Fix the render list based on the clipping of each sprite
	if (sprite_list.size() > 0)
	{
		sprite_list.clear();

		m_numSpritesRendered = 0;

		for (int j = 0; j < renderList.size(); j++)
		{
			if (renderList[j].sprite != NULL)
				m_numSpritesRendered++;

			sprite_list.push_back( renderList[j] );
		}
	}
}
//=================================================================================================================
void TopdownMap::DeleteDisplaySprite()
{
	bool deleteSpr = false;
	Sprite* spr;
	int value;
	for (int i = 0; i < m_visionSprites.size(); i++)
	{
		spr = m_visionSprites[i];

		if (spr != NULL)
		{
			if (spr->IsDisplaySprite())
			{
				value = i;
				deleteSpr = true;
				break;
			}
		}
	}
	
	if (deleteSpr)
	{
		m_visionSprites.erase(m_visionSprites.begin() + value);
		
		if (sprite_list.size() > 0)
		{
			sprite_list.clear();
			sprite_list.resize(MAX_SPRITES);
		}

		spriteNum--;
		m_numSpritesRendered--;

		SortSprites();
		ClipSprites();
	}
}
//=================================================================================================================
void TopdownMap::DeletePlayerSprite()
{
	bool deleteSpr = false;
	Sprite* spr;
	int value;
	for (int i = 0; i < m_visionSprites.size(); i++)
	{
		spr = m_visionSprites[i];

		if (spr != NULL)
		{
			if (spr->IsPlayer())
			{
				value = i;
				deleteSpr = true;
				break;
			}
		}
	}
	
	if (deleteSpr)
	{
		m_visionSprites.erase(m_visionSprites.begin() + value);
		
		if (sprite_list.size() > 0)
		{
			sprite_list.clear();
			sprite_list.resize(MAX_SPRITES);
		}

		spriteNum--;
		m_numSpritesRendered--;

		SortSprites();
		ClipSprites();
	}
}
//=================================================================================================================
bool TopdownMap::SpriteClicked(AISprite*& clickedSprite, int mouse_x, int mouse_y)
{
	AISprite* spr;
	for (int sprite_num = 0; sprite_num < m_visionSprites.size(); sprite_num++)
	{
		spr = m_visionSprites[sprite_num];

		if (!spr->IsDisplaySprite())
		{
			if (mouse_x >= spr->TopLeftPosition().x && mouse_x <= spr->TopLeftPosition().x + spr->Width())
			{
				if (mouse_y >= spr->TopLeftPosition().y && mouse_y <= spr->TopLeftPosition().y + spr->Height())
				{
					clickedSprite = spr;
					return true;
				}
			}
		}
	}

	return false;
}
//=================================================================================================================
void TopdownMap::MoveSprite(AISprite*& clickedSprite, int mouse_x, int mouse_y)
{
	AISprite* spr;
	for (int sprite_num = 0; sprite_num < m_visionSprites.size(); sprite_num++)
	{
		spr = m_visionSprites[sprite_num];

		if (spr == clickedSprite)
		{
			spr->Move(mouse_x, mouse_y, 0, 0);
			return;
		}
	}
}
//=================================================================================================================
void TopdownMap::RemoveSprite(AISprite* clickedSprite)
{
	bool deleteSpr = false;
	AISprite* spr;
	int value;
	for (int i = 0; i < m_visionSprites.size(); i++)
	{
		spr = m_visionSprites[i];

		if (spr != NULL)
		{
			if (spr->ID() == clickedSprite->ID())
			{
				value = i;
				deleteSpr = true;
				break;
			}
		}
	}
	
	if (deleteSpr)
	{
		m_visionSprites.erase(m_visionSprites.begin() + value);

		if (sprite_list.size() > 0)
		{
			sprite_list.clear();
			sprite_list.resize(MAX_SPRITES);
		}

		spriteNum--;
		m_numSpritesRendered--;

		SortSprites();
		ClipSprites();
	}
}
//=================================================================================================================
/*void TopdownMap::CreateSpritesText(GameDirectory2D* gd, HWND hwnd)
{
	AISprite* spr;
	for (int sprite_num = 0; sprite_num < m_visionSprites.size(); sprite_num++)
	{
		spr = m_visionSprites[sprite_num];
		
		if (spr != NULL)
		{
			spr->CreateText(gd, hwnd);
		}
	}
}*/
//=================================================================================================================
void TopdownMap::SetRenderDisplaySprite(bool can_render)
{
	AISprite* spr;
	for (int i = 0; i < m_visionSprites.size(); i++)
	{
		spr = m_visionSprites[i];

		if (spr->IsDisplaySprite())
		{
			m_canRenderDisplaySprite = can_render;
			break;
		}
	}
}
//=================================================================================================================
void TopdownMap::RenderSectionChange(XMFLOAT3 cameraPos)
{
	//If the player moves from one section to another
	//We will load the new render section and its neighbors
	int newSection = -1;
	int currSection = mSectionRenderList[0];

	Section* sec = m_tileSystem->GetSection(currSection);

	bool good = false;

	#pragma region One
	//Check [1] section, whatever it is, if it exists
	if (mSectionRenderList.size() <= 1) return;
	newSection = mSectionRenderList[1];

	//Check if player is in the bounds of the section
	Section* newSec = m_tileSystem->GetSection(newSection);
	Section::BB bounds = newSec->m_BoundingBox;
	
	if (cameraPos.x >= bounds.firstTileX && cameraPos.x <= bounds.lastTileX)
	{
		if (cameraPos.y >= bounds.firstTileY && cameraPos.y <= bounds.lastTileY)
		{
			//The player is in the section
			good = true;
		}
	}

	if (!good) newSection = -1;
	#pragma endregion

	if (!good)
	{
	#pragma region Two
	//Check [2] section, whatever it is, if it exists
	if (mSectionRenderList.size() <= 2) return;
	newSection = mSectionRenderList[2];

	//Check if player is in the bounds of the section
	Section* newSec = m_tileSystem->GetSection(newSection);
	Section::BB bounds = newSec->m_BoundingBox;
		
	if (cameraPos.x >= bounds.firstTileX && cameraPos.x <= bounds.lastTileX)
	{
		if (cameraPos.y >= bounds.firstTileY && cameraPos.y <= bounds.lastTileY)
		{
			//The player is in the section
			good = true;
		}
	}

	if (!good) newSection = -1;
	#pragma endregion
	}

	if (!good)
	{
	#pragma region Three
	//Check [3] section, whatever it is, if it exists
	if (mSectionRenderList.size() <= 3) return;
	newSection = mSectionRenderList[3];

	//Check if player is in the bounds of the section
	Section* newSec = m_tileSystem->GetSection(newSection);
	Section::BB bounds = newSec->m_BoundingBox;
		
	if (cameraPos.x >= bounds.firstTileX && cameraPos.x <= bounds.lastTileX)
	{
		if (cameraPos.y >= bounds.firstTileY && cameraPos.y <= bounds.lastTileY)
		{
			//The player is in the section
			good = true;
		}
	}

	if (!good) newSection = -1;
	#pragma endregion
	}

	if (!good)
	{
	#pragma region Four
	//Check [4] section, whatever it is, if it exists
	if (mSectionRenderList.size() <= 4) return;
	newSection = mSectionRenderList[4];

	//Check if player is in the bounds of the section
	Section* newSec = m_tileSystem->GetSection(newSection);
	Section::BB bounds = newSec->m_BoundingBox;
		
	if (cameraPos.x >= bounds.firstTileX && cameraPos.x <= bounds.lastTileX)
	{
		if (cameraPos.y >= bounds.firstTileY && cameraPos.y <= bounds.lastTileY)
		{
			//The player is in the section
			good = true;
		}
	}

	if (!good) newSection = -1;
	#pragma endregion
	}

	if (!good)
	{
	#pragma region Five
	//Check [5] section, whatever it is, if it exists
	if (mSectionRenderList.size() <= 5) return;
	newSection = mSectionRenderList[5];

	//Check if player is in the bounds of the section
	Section* newSec = m_tileSystem->GetSection(newSection);
	Section::BB bounds = newSec->m_BoundingBox;
		
	if (cameraPos.x >= bounds.firstTileX && cameraPos.x <= bounds.lastTileX)
	{
		if (cameraPos.y >= bounds.firstTileY && cameraPos.y <= bounds.lastTileY)
		{
			//The player is in the section
			good = true;
		}
	}

	if (!good) newSection = -1;
	#pragma endregion
	}

	if (!good)
	{
	#pragma region Six
	//Check [6] section, whatever it is, if it exists
	if (mSectionRenderList.size() <= 6) return;
	newSection = mSectionRenderList[6];

	//Check if player is in the bounds of the section
	Section* newSec = m_tileSystem->GetSection(newSection);
	Section::BB bounds = newSec->m_BoundingBox;
		
	if (cameraPos.x >= bounds.firstTileX && cameraPos.x <= bounds.lastTileX)
	{
		if (cameraPos.y >= bounds.firstTileY && cameraPos.y <= bounds.lastTileY)
		{
			//The player is in the section
			good = true;
		}
	}

	if (!good) newSection = -1;
	#pragma endregion
	}

	if (!good)
	{
	#pragma region Seven
	//Check [7] section, whatever it is, if it exists
	if (mSectionRenderList.size() <= 7) return;
	newSection = mSectionRenderList[7];

	//Check if player is in the bounds of the section
	Section* newSec = m_tileSystem->GetSection(newSection);
	Section::BB bounds = newSec->m_BoundingBox;
		
	if (cameraPos.x >= bounds.firstTileX && cameraPos.x <= bounds.lastTileX)
	{
		if (cameraPos.y >= bounds.firstTileY && cameraPos.y <= bounds.lastTileY)
		{
			//The player is in the section
			good = true;
		}
	}

	if (!good) newSection = -1;
	#pragma endregion
	}

	if (!good)
	{
	#pragma region Eight
	//Check [8] section, whatever it is, if it exists
	if (mSectionRenderList.size() <= 8) return;
	newSection = mSectionRenderList[8];

	//Check if player is in the bounds of the section
	Section* newSec = m_tileSystem->GetSection(newSection);
	Section::BB bounds = newSec->m_BoundingBox;
		
	if (cameraPos.x >= bounds.firstTileX && cameraPos.x <= bounds.lastTileX)
	{
		if (cameraPos.y >= bounds.firstTileY && cameraPos.y <= bounds.lastTileY)
		{
			//The player is in the section
			good = true;
		}
	}

	if (!good) newSection = -1;
	#pragma endregion
	}

	if (newSection != -1)
	{
		//Section* nsec = m_tileSystem->GetSection(currSection);
		//nsec->ReSet(d3d, m_engineOptions->m_screenWidth, m_engineOptions->m_screenHeight, 64, 64);
		//Section* nsec2 = m_tileSystem->GetSection(newSection);
		//nsec2->ReTexBounds(d3d, m_engineOptions->m_screenWidth, m_engineOptions->m_screenHeight, 64, 64);

		LoadRenderSections(newSection);
	}
}
//=================================================================================================================
Tile* TopdownMap::SelectTile(float x, float y)
{
	return m_tileSystem->SelectTile(x, y);
}
//=================================================================================================================
void TopdownMap::InitAISprite(GameDirectory2D* gd)
{
	AISprite* spr;
	for (int sprite_num = 0; sprite_num < m_visionSprites.size(); sprite_num++)
	{
		spr = m_visionSprites[sprite_num];
		
		if (spr != NULL)
		{
			//spr->AddScriptPath(gd->m_scripts_path);
			//spr->SetLuaState(Commands::GetInstance()->GetLuaState());
		}
	}
}
//=================================================================================================================
/*bool TopdownMap::PlayerCollidingWithAnything()
{
	SAT sat;

	Sprite* player_hb = GetPlayer()->GetHardBoxSpr();

	SAT_RECT p_rect(player_hb->GetPosition().x, player_hb->GetPosition().y, player_hb->GetWidth(), player_hb->GetHeight());

	XMFLOAT3 p_vec = player_hb->GetPosition();

	AISprite* spr;
	for (int sprite_num = 0; sprite_num < m_visionSprites.size(); sprite_num++)
	{
		spr = m_visionSprites[sprite_num];
		
		if (spr != NULL)
		{
			if (spr->IsPlayer()) continue;

			Sprite* hb = spr->GetHardBoxSpr();

			if (hb != NULL)
			{
				SAT_RECT s_rect(hb->GetPosition().x, hb->GetPosition().y, hb->GetWidth(), hb->GetHeight());
				
				bool touching = sat.collides( p_rect, s_rect );
				
				MTV mtv = sat.GetMTV();

				while (touching)
				{
					if (!sat.collides2( p_rect, s_rect ))
						break;

					//Move the player with the MTV
					//Normalize the MTV vector and multiply by the overlap

					SAT_POINT mtv_vec = mtv._smallest.normalize();

					if (GetPlayer()->MoveSpriteUp())
					{
						mtv_vec.y = -mtv_vec.y;
					}

					//project the new position to push the player away from the other sprite
					p_vec.x += mtv_vec.x;
					p_vec.y += mtv_vec.y;

					p_rect.x = p_vec.x;
					p_rect.y = p_vec.y;

					//Update the players position and hardbox
					XMFLOAT3 p_pos = GetPlayer()->GetPosition();

					p_pos.x += mtv_vec.x;
					p_pos.y += mtv_vec.y;

					GetPlayer()->Move(p_pos.x, p_pos.y, p_vec.x, p_vec.y);
				}
			}
		}
	}

	return false;
}*/
//=================================================================================================================
int TopdownMap::SectionsRendered()
{
	//return m_tileSystem->GetNumSections();
	return mSectionRenderList.size();
}
//=================================================================================================================
void TopdownMap::PlayerMovingDir(bool& moved, int& dirUD, int& dirLR)
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
//=================================================================================================================
XMFLOAT3 TopdownMap::MatchingTilePos(float x, float y)
{
	Tile* tile;
	Section* current_section;
	int curr_sec = 0;
	vector<Section*> sections = m_tileSystem->GetSections();
	//Loop through all the sections in the map
	for (int sec_id = 0; sec_id < sections.size(); sec_id++)
	{
		current_section = sections[sec_id];

		for (int tile_y = 0; tile_y < m_engineOptions->COL_SIZE; tile_y++)
		{
			for (int tile_x = 0; tile_x < m_engineOptions->ROW_SIZE; tile_x++)
			{
				tile = current_section->GetTile(tile_x, tile_y);

				float realTileX = tile->GetPosition().x;
				float realTileY = tile->GetPosition().y;

				//if (realTileX >= x && realTileX <= x + playerW)
				if (x >= realTileX && x <= realTileX + m_engineOptions->TILE_SIZE)
				{
					//if (realTileY >= y && realTileY <= y + playerH)
					if (y >= realTileY && y <= realTileY + m_engineOptions->TILE_SIZE)
					{
						return tile->GetPosition();
					}
				}
			}
		}
	}

	// Not found
	return XMFLOAT3(x, y, 0);
}
//=================================================================================================================
void TopdownMap::MovePlayer(Keyboard* keyboard)
{
	AISprite* player = GetPlayer();

	if (player == NULL) return;

	float playerWidth = 64;
	float playerHeight = 64;
	float pspeed = 0;

	XMFLOAT3 playerPos;
	XMFLOAT3 playerHBPos;
	int hb_wDummy;
	int hb_hDummy;

	//Get the width and height of the player
	playerWidth = player->Width();
	playerHeight = player->Height();

	//Get the position of the player
	playerPos = player->TopLeftPosition();
	//player->GetHardbox(playerHBPos.x, playerHBPos.y, hb_wDummy, hb_hDummy);
	playerHBPos = player->GetHardBoxSpr()->TopLeftPosition();

	//Bound the player to the map
	int player_xy_min = 0;
	int player_x_max = m_engineOptions->SECTION_COL_SIZE * (m_engineOptions->ROW_SIZE * m_engineOptions->TILE_SIZE) - playerWidth;
	int player_y_max = m_engineOptions->SECTION_ROW_SIZE * (m_engineOptions->COL_SIZE * m_engineOptions->TILE_SIZE) - playerHeight;

	////See if we are not attacking anymore
	//if (GetPlayer()->IsCurrentSequenceOnLastFrame())
	//{
	//	GetPlayer()->SetAttacking(false);
	//	GetPlayer()->SetCurrentSequenceNotOnLastFrame();
	//}

	////If the player is not attacking
	//if (!GetPlayer()->IsAttacking())
	//{
	//	if (GetPlayer()->CurrentSequenceExists())
	//	{
	//		//Set idle animation for the player
	//		switch (GetPlayer()->GetDirectionMoved())
	//		{
	//			case 0: { GetPlayer()->SetSequence(IDLE_UP); } break;
	//			case 1: { GetPlayer()->SetSequence(IDLE_DOWN); } break;
	//			case 2: { GetPlayer()->SetSequence(IDLE_LEFT); } break;
	//			case 3: { GetPlayer()->SetSequence(IDLE_RIGHT); } break;
	//		}
	//	}
	//}

	pspeed = player->Speed();

	XMFLOAT3 cameraPos = m_camera->Position();

	//if (input->KeyDown(DIK_W))
	if (keyboard->IsKeyDown(Keyboard::Key::W))
	{
		//if (playerPos.y > player_xy_min)
		{
			//player->MoveScript(Commands::GetInstance()->GetLuaState(), Commands::GetInstance()->GetGD());
			playerPos.y -= pspeed;
			playerHBPos.y -= pspeed;
			player->SetMoveSpriteUp( true );
			player->SetMoveSpriteDown( false );
		}
	}
	//else if (input->KeyDown(DIK_S))
	else if (keyboard->IsKeyDown(Keyboard::Key::S))
	{
		//if (playerPos.y < player_y_max)
		{
			//player->MoveScript(Commands::GetInstance()->GetLuaState(), Commands::GetInstance()->GetGD());
			playerPos.y += pspeed;
			playerHBPos.y += pspeed;
			player->SetMoveSpriteDown( true );
			player->SetMoveSpriteUp( false );
		}
	}
	else
	{
		player->SetMoveSpriteUp( false );
		player->SetMoveSpriteDown( false );
	}

	//if (input->KeyDown(DIK_A))
	if (keyboard->IsKeyDown(Keyboard::Key::A))
	{
		//if (playerPos.x > player_xy_min)
		{
			//player->MoveScript(Commands::GetInstance()->GetLuaState(), Commands::GetInstance()->GetGD());
			playerPos.x -= pspeed;
			playerHBPos.x -= pspeed;
			player->SetMoveSpriteLeft( true );
			player->SetMoveSpriteRight( false );
		}
	}
	//else if (input->KeyDown(DIK_D))
	else if (keyboard->IsKeyDown(Keyboard::Key::D))
	{
		//if (playerPos.x < player_x_max)
		{
			//player->MoveScript(Commands::GetInstance()->GetLuaState(), Commands::GetInstance()->GetGD());
			playerPos.x += pspeed;
			playerHBPos.x += pspeed;
			player->SetMoveSpriteRight( true );
			player->SetMoveSpriteLeft( false );
		}
	}
	else
	{
		player->SetMoveSpriteLeft( false );
		player->SetMoveSpriteRight( false );
	}

	//Move the player if key was pressed
	/*
	if (input->KeyDown(DIK_W))
	{
		if (playerPos.y > player_xy_min)
		{
			if (GetPlayer()->CurrentSequenceExists())
			{
				GetPlayer()->SetDirectionMoved(0);
				GetPlayer()->SetSequence(WALK_UP);
			}

			playerPos.y -= pspeed;
			movedUp = true;
			movedDown = false;
		}
	}
	else if (input->KeyDown(DIK_S))
	{
		if (playerPos.y < player_y_max)
		{
			if (GetPlayer()->CurrentSequenceExists())
			{
				GetPlayer()->SetDirectionMoved(1);
				GetPlayer()->SetSequence(WALK_DOWN);
			}

			playerPos.y += pspeed;
			movedUp = false;
			movedDown = true;
		}
	}
	else
	{
		movedUp = false;
		movedDown = false;
	}
		
	if (input->KeyDown(DIK_A))
	{
		if (playerPos.x > player_xy_min)
		{
			if (GetPlayer()->CurrentSequenceExists())
			{
				GetPlayer()->SetDirectionMoved(2);
				GetPlayer()->SetSequence(WALK_LEFT);
			}

			playerPos.x -= pspeed;
			movedLeft = true;
			movedRight = false;
		}
	}
	else if (input->KeyDown(DIK_D))
	{
		if (playerPos.x < player_x_max)
		{
			if (GetPlayer()->CurrentSequenceExists())
			{
				GetPlayer()->SetDirectionMoved(3);
				GetPlayer()->SetSequence(WALK_RIGHT);
			}

			playerPos.x += pspeed;
			movedLeft = false;
			movedRight = true;
		}
	}
	else
	{
		movedLeft = false;
		movedRight = false;
	}
	*/

	//Find out if the player is in the middle of the screen
	/*if ((playerPos.x > (m_engineOptions->m_screenWidth / 2) - playerWidth))
	{
		playerInCenterOfScreenX = true;
	}
	else
	{
		playerInCenterOfScreenX = false;

		//camera.x is 938
	}
	
	if ((playerPos.y > (m_engineOptions->m_screenHeight / 2) - playerHeight))// && -cameraPos.y != m_engineOptions->m_screenHeight)
	{
		playerInMiddleY = true;
		playerInCenterOfScreenY = true;
	}
	else
	{
		playerInCenterOfScreenY = false;

		//If player is moving up and cameraPos.y == -768
		//  when player gets to middle of the screen then set playerInCenterOfScreenY = true;
	}*/

	//if (m_upEdge)
	//{
	//	playerInCenterOfScreenY = false;
	//	//if (GetPlayer()->GetPosition().y < (m_engineOptions->m_screenHeight / 2) - playerHeight)
	//	//	playerInCenterOfScreenY = true;
	//}

	//if (m_leftEdge)
	//{
	//	playerInCenterOfScreenX = false;
	//	//if (GetPlayer()->GetPosition().x < (m_engineOptions->m_screenWidth / 2) - playerWidth)
	//	//	playerInCenterOfScreenX = true;
	//}



	//Need to make the attack sequence run until the last frame
	//and then switch back to last sequence after.

	//Player is attacking
	//if (input->KeyPressed(DIK_RCONTROL))
	//{
	//	if (GetPlayer()->CurrentSequenceExists())
	//	{
	//		//Player is attacking
	//		GetPlayer()->SetAttacking(true);
	//		switch (GetPlayer()->GetDirectionMoved())
	//		{
	//			case 0: { GetPlayer()->SetSequence(ATTACK_UP); } break;
	//			case 1: { GetPlayer()->SetSequence(ATTACK_DOWN); } break;
	//			case 2: { GetPlayer()->SetSequence(ATTACK_LEFT); } break;
	//			case 3: { GetPlayer()->SetSequence(ATTACK_RIGHT); } break;
	//		}
	//	}
	//}
	
	// Clip the player to the map
	ClipSpriteToMap(player, pspeed, playerPos.x, playerPos.y, playerHBPos.x, playerHBPos.y);

	//Move the player
	player->Move(playerPos.x, playerPos.y, playerHBPos.x, playerHBPos.y);

	bool moved = player->MoveSpriteUp() || player->MoveSpriteDown() || player->MoveSpriteLeft() || player->MoveSpriteRight();

	if (moved && m_bFOW)
	{
		m_tileSystem->UpdateFog(mSectionRenderList, player, m_iFOWRadius);
		m_tileSystem->UpdateTextures(mSectionRenderList);
	}
}
//=================================================================================================================
void TopdownMap::ClipSpriteToMap(Sprite* spr, float speed, float& newX, float& newY, float& newHBX, float& newHBY)
{
	// Clip the sprite to the map
	Section* section_in = spr->SectionIn();

	if (section_in != NULL)
	{
		SectionAdjacency* sa = section_in->AdjacencyInfo();

		// Use the section bounds to check to see if the sprite is outside of the section along the edge of the map

		Section::BB bounds = section_in->m_BoundingBox;

		int w = spr->Width();
		int h = spr->Height();

		// Figure out what edge the sprite is on

		// The sprite is at the north edge
		if (sa->type == NORTH_EDGE)
		{
			if (spr->MoveSpriteUp())
			{
				if (newY <= (bounds.lastTileY + h) - m_engineOptions->COL_SIZE * m_engineOptions->TILE_SIZE)
				{
					newY += speed;
					newHBY += speed;
					spr->SetMoveSpriteUp(false);
				}
			}
		}

		// The sprite is at the south edge
		if (sa->type == SOUTH_EDGE)
		{
			if (spr->MoveSpriteDown())
			{
				if (newY >= (bounds.firstTileY - h) + m_engineOptions->COL_SIZE * m_engineOptions->TILE_SIZE)
				{
					newY -= speed;
					newHBY -= speed;
					spr->SetMoveSpriteDown(false);
				}
			}
		}

		// The sprite is at the west edge
		if (sa->type == WEST_EDGE)
		{
			if (spr->MoveSpriteLeft())
			{
				if (newX <= (bounds.lastTileX + w) - m_engineOptions->ROW_SIZE * m_engineOptions->TILE_SIZE)
				{
					newX += speed;
					newHBX += speed;
					spr->SetMoveSpriteLeft(false);
				}
			}
		}

		// The sprite is at the east edge
		if (sa->type == EAST_EDGE)
		{
			if (spr->MoveSpriteRight())
			{
				if (newX >= (bounds.firstTileX - w) + m_engineOptions->ROW_SIZE * m_engineOptions->TILE_SIZE)
				{
					newX -= speed;
					newHBX -= speed;
					spr->SetMoveSpriteRight(false);
				}
			}
		}

		// The sprite is at the nw corner
		if (sa->type == NW_CORNER)
		{
			if (spr->MoveSpriteUp())
			{
				if (newY <= (bounds.lastTileY + h) - m_engineOptions->COL_SIZE * m_engineOptions->TILE_SIZE)
				{
					newY += speed;
					newHBY += speed;
					spr->SetMoveSpriteUp(false);
				}
			}
			if (spr->MoveSpriteLeft())
			{
				if (newX <= (bounds.lastTileX + w) - m_engineOptions->ROW_SIZE * m_engineOptions->TILE_SIZE)
				{
					newX += speed;
					newHBX += speed;
					spr->SetMoveSpriteLeft(false);
				}
			}
		}

		// The sprite is at the sw corner
		if (sa->type == SW_CORNER)
		{
			if (spr->MoveSpriteDown())
			{
				if (newY >= (bounds.firstTileY - h) + m_engineOptions->COL_SIZE * m_engineOptions->TILE_SIZE)
				{
					newY -= speed;
					newHBY -= speed;
					spr->SetMoveSpriteDown(false);
				}
			}
			if (spr->MoveSpriteLeft())
			{
				if (newX <= (bounds.lastTileX + w) - m_engineOptions->ROW_SIZE * m_engineOptions->TILE_SIZE)
				{
					newX += speed;
					newHBX += speed;
					spr->SetMoveSpriteLeft(false);
				}
			}
		}

		// The sprite is at the ne corner
		if (sa->type == NE_CORNER)
		{
			if (spr->MoveSpriteUp())
			{
				if (newY <= (bounds.lastTileY + h) - m_engineOptions->COL_SIZE * m_engineOptions->TILE_SIZE)
				{
					newY += speed;
					newHBY += speed;
					spr->SetMoveSpriteUp(false);
				}
			}
			if (spr->MoveSpriteRight())
			{
				if (newX >= (bounds.firstTileX - w) + m_engineOptions->ROW_SIZE * m_engineOptions->TILE_SIZE)
				{
					newX -= speed;
					newHBX -= speed;
					spr->SetMoveSpriteRight(false);
				}
			}
		}

		// The sprite is at the se corner
		if (sa->type == SE_CORNER)
		{
			if (spr->MoveSpriteDown())
			{
				if (newY >= (bounds.firstTileY - h) + m_engineOptions->COL_SIZE * m_engineOptions->TILE_SIZE)
				{
					newY -= speed;
					newHBY -= speed;
					spr->SetMoveSpriteDown(false);
				}
			}
			if (spr->MoveSpriteRight())
			{
				if (newX >= (bounds.firstTileX - w) + m_engineOptions->ROW_SIZE * m_engineOptions->TILE_SIZE)
				{
					newX -= speed;
					newHBX -= speed;
					spr->SetMoveSpriteRight(false);
				}
			}
		}
	}
}
//=================================================================================================================
/*void TopdownMap::ResetMapCameraPosition(int x, int y, bool isSectionPicker)
{
	XMMath3 cameraPos = m_camera->Position();
	//XMFLOAT3 cameraMapPos = m_camera->getMapPosition();

	//Adjust the map sprite based on screen size
	if (isSectionPicker)
	{
		if (x == 800 && y == 600)
		{
			if (cameraPos.x > 170)
				cameraPos.x = 170;
			if (cameraPos.y < -395)
				cameraPos.y = -395;
		}

		if (x == 1024 && y == 768)
		{
			if (cameraPos.x > 0)
				cameraPos.x = 0;
			if (cameraPos.y < -270)
				cameraPos.y = -270;
		}

		if (x == 1366 && y == 768)
		{
			if (cameraPos.x > 0)
				cameraPos.x = 0;
			if (cameraPos.y < -290)
				cameraPos.y = -290;
		}

		if (x == 1920 && y == 1080)
		{
			if (cameraPos.x > 0)
				cameraPos.x = 0;
			if (cameraPos.y < 0)
				cameraPos.y = 0;
		}
	}
	else
	{
		#pragma region "800x600"
		if (x == 800 && y == 600)
		{
			if (m_engineOptions->SECTION_COL_SIZE == 3 && m_engineOptions->SECTION_ROW_SIZE == 3)
			{
				if (cameraPos.x > 1504)
					cameraPos.x = 1504;

				if (mIsEditor)
				{
					if (cameraPos.y < -938)//-962)
						cameraPos.y = -938;//-962;
				}
				else
				{
					if (cameraPos.y < -936)
						cameraPos.y = -936;
				}
			}
			if (m_engineOptions->SECTION_COL_SIZE == 6 && m_engineOptions->SECTION_ROW_SIZE == 6)
			{
				if (cameraPos.x > 3808)
					cameraPos.x = 3808;

				if (mIsEditor)
				{
					if (cameraPos.y < -2472)//-2498)
						cameraPos.y = -2472;//-2498;
				}
				else
				{
					if (cameraPos.y < -2472)
						cameraPos.y = -2472;
				}
			}
			if (m_engineOptions->SECTION_COL_SIZE == 8 && m_engineOptions->SECTION_ROW_SIZE == 8)
			{
				if (cameraPos.x > 5344)
					cameraPos.x = 5344;

				if (mIsEditor)
				{
					if (cameraPos.y < -3496)//-3522)
						cameraPos.y = -3496;//-3522;
				}
				else
				{
					if (cameraPos.y < -3496)
						cameraPos.y = -3496;
				}
			}
			if (m_engineOptions->SECTION_COL_SIZE == 10 && m_engineOptions->SECTION_ROW_SIZE == 10)
			{
				if (cameraPos.x > 6880)
					cameraPos.x = 6880;

				if (mIsEditor)
				{
					if (cameraPos.y < -4520)//-4546)
						cameraPos.y = -4520;//-4546;
				}
				else
				{
					if (cameraPos.y < -4520)
						cameraPos.y = -4520;
				}
			}
			if (m_engineOptions->SECTION_COL_SIZE == 20 && m_engineOptions->SECTION_ROW_SIZE == 20)
			{
				if (cameraPos.x > 14560)
					cameraPos.x = 14560;

				if (mIsEditor)
				{
					if (cameraPos.y < -9640)//-9666)
						cameraPos.y = -9640;//-9666;
				}
				else
				{
					if (cameraPos.y < -9640)
						cameraPos.y = -9640;
				}
			}
			if (m_engineOptions->SECTION_COL_SIZE == 30 && m_engineOptions->SECTION_ROW_SIZE == 30)
			{
				if (cameraPos.x > 22240)
					cameraPos.x = 22240;

				if (mIsEditor)
				{
					if (cameraPos.y < -14760)//-14786)
						cameraPos.y = -14760;//-14786;
				}
				else
				{
					if (cameraPos.y < -14760)
						cameraPos.y = -14760;
				}
			}
		}
		#pragma endregion

		#pragma region "1024x768"
		if (x == 1024 && y == 768)
		{
			if (m_engineOptions->SECTION_COL_SIZE == 3 && m_engineOptions->SECTION_ROW_SIZE == 3)
			{
				if (cameraPos.x > 1280)
					cameraPos.x = 1280;

				if (mIsEditor)
				{
					if (cameraPos.y < -768)//-794)
						cameraPos.y = -768;//-794;
				}
				else
				{
					if (cameraPos.y < -768)
						cameraPos.y = -768;
				}
			}
			if (m_engineOptions->SECTION_COL_SIZE == 6 && m_engineOptions->SECTION_ROW_SIZE == 6)
			{
				if (cameraPos.x > 3584)
					cameraPos.x = 3584;

				if (mIsEditor)
				{
					if (cameraPos.y < -2304)//-2330)
						cameraPos.y = -2304;//-2330;
				}
				else
				{
					if (cameraPos.y < -2304)
						cameraPos.y = -2304;
				}
			}
			if (m_engineOptions->SECTION_COL_SIZE == 8 && m_engineOptions->SECTION_ROW_SIZE == 8)
			{
				if (cameraPos.x > 5120)
					cameraPos.x = 5120;

				if (mIsEditor)
				{
					if (cameraPos.y < -3328)//-3354)
						cameraPos.y = -3328;//-3354;
				}
				else
				{
					if (cameraPos.y < -3328)
						cameraPos.y = -3328;
				}
			}
			if (m_engineOptions->SECTION_COL_SIZE == 10 && m_engineOptions->SECTION_ROW_SIZE == 10)
			{
				if (cameraPos.x > 6656)
					cameraPos.x = 6656;
			
				if (mIsEditor)
				{
					if (cameraPos.y < -4352)//-4378)
						cameraPos.y = -4352;//-4378;
				}
				else
				{
					if (cameraPos.y < -4352)
						cameraPos.y = -4352;
				}
			}
			if (m_engineOptions->SECTION_COL_SIZE == 20 && m_engineOptions->SECTION_ROW_SIZE == 20)
			{
				if (cameraPos.x > 14336)
					cameraPos.x = 14336;

				if (mIsEditor)
				{
					if (cameraPos.y < -9472)//-9498)
						cameraPos.y = -9472;//-9498;
				}
				else
				{
					if (cameraPos.y < -9472)
						cameraPos.y = -9472;
				}
			}
			if (m_engineOptions->SECTION_COL_SIZE == 30 && m_engineOptions->SECTION_ROW_SIZE == 30)
			{
				if (cameraPos.x > 22016)
					cameraPos.x = 22016;

				if (mIsEditor)
				{
					if (cameraPos.y < -14592)//-14618)
						cameraPos.y = -14592;//-14618;
				}
				else
				{
					if (cameraPos.y < -14592)
						cameraPos.y = -14592;
				}
			}
		}
		#pragma endregion

		#pragma region "1366x768"
		if (x == 1366 && y == 768)
		{
			if (m_engineOptions->SECTION_COL_SIZE == 3 && m_engineOptions->SECTION_ROW_SIZE == 3)
			{
				if (cameraPos.x > 938)
					cameraPos.x = 938;

				if (mIsEditor)
				{
					if (cameraPos.y < -768)//-706)//-794)
						cameraPos.y = -768;//-706;//-794;
				}
				else
				{
					if (cameraPos.y < -768)
						cameraPos.y = -768;
				}
			}
			if (m_engineOptions->SECTION_COL_SIZE == 6 && m_engineOptions->SECTION_ROW_SIZE == 6)
			{
				if (cameraPos.x > 3242)
					cameraPos.x = 3242;

				if (mIsEditor)
				{
					if (cameraPos.y < -2304)//-2330)
						cameraPos.y = -2304;//-2330;
				}
				else
				{
					if (cameraPos.y < -2304)
						cameraPos.y = -2304;
				}
			}
			if (m_engineOptions->SECTION_COL_SIZE == 8 && m_engineOptions->SECTION_ROW_SIZE == 8)
			{
				if (cameraPos.x > 4778)
					cameraPos.x = 4778;

				if (mIsEditor)
				{
					if (cameraPos.y < -3328)//-3354)
						cameraPos.y = -3328;//-3354;
				}
				else
				{
					if (cameraPos.y < -3328)
						cameraPos.y = -3328;
				}
			}
			if (m_engineOptions->SECTION_COL_SIZE == 10 && m_engineOptions->SECTION_ROW_SIZE == 10)
			{
				if (cameraPos.x > 6314)
					cameraPos.x = 6314;
			
				if (mIsEditor)
				{
					if (cameraPos.y < -4352)//-4378)
						cameraPos.y = -4352;//-4378;
				}
				else
				{
					if (cameraPos.y < -4352)
						cameraPos.y = -4352;
				}
			}
			if (m_engineOptions->SECTION_COL_SIZE == 20 && m_engineOptions->SECTION_ROW_SIZE == 20)
			{
				if (cameraPos.x > 13994)
					cameraPos.x = 13994;
			
				if (mIsEditor)
				{
					if (cameraPos.y < -9472)//-9498)
						cameraPos.y = -9472;//-9498;
				}
				else
				{
					if (cameraPos.y < -9472)
						cameraPos.y = -9472;
				}
			}
			if (m_engineOptions->SECTION_COL_SIZE == 30 && m_engineOptions->SECTION_ROW_SIZE == 30)
			{
				if (cameraPos.x > 21674)
					cameraPos.x = 21674;
			
				if (mIsEditor)
				{
					if (cameraPos.y < -14592)//-14618)
						cameraPos.y = -14592;//-14618;
				}
				else
				{
					if (cameraPos.y < -14592)
						cameraPos.y = -14592;
				}
			}
		}
		#pragma endregion

		#pragma region "1920x1080"
		if (x == 1920 && y == 1080)
		{
			if (m_engineOptions->SECTION_COL_SIZE == 3 && m_engineOptions->SECTION_ROW_SIZE == 3)
			{
				if (cameraPos.x > 402)
					cameraPos.x = 402;

				//y = -457

				if (mIsEditor)
				{
					if (cameraPos.y < -456)//-544)
						cameraPos.y = -456;//-544;
				}
				else
				{
					if (cameraPos.y < -518)
						cameraPos.y = -518;
				}
			}
			if (m_engineOptions->SECTION_COL_SIZE == 6 && m_engineOptions->SECTION_ROW_SIZE == 6)
			{
				if (cameraPos.x > 2705)
					cameraPos.x = 2705;

				if (mIsEditor)
				{
					if (cameraPos.y < -1904)//-1992)//-2080)
						cameraPos.y = -1904;//-1992;//-2080;
				}
				else
				{
					if (cameraPos.y < -2054)
						cameraPos.y = -2054;
				}
			}
			if (m_engineOptions->SECTION_COL_SIZE == 8 && m_engineOptions->SECTION_ROW_SIZE == 8)
			{
				if (cameraPos.x > 4242)
					cameraPos.x = 4242;

				if (mIsEditor)
				{
					if (cameraPos.y < -3016)//-3104)
						cameraPos.y = -3016;//-3104;
				}
				else
				{
					if (cameraPos.y < -3078)
						cameraPos.y = -3078;
				}
			}
			if (m_engineOptions->SECTION_COL_SIZE == 10 && m_engineOptions->SECTION_ROW_SIZE == 10)
			{
				if (cameraPos.x > 5778)
					cameraPos.x = 5778;

				if (mIsEditor)
				{
					if (cameraPos.y < -4040)//-4128)
						cameraPos.y = -4040;//-4128;
				}
				else
				{
					if (cameraPos.y < -4102)
						cameraPos.y = -4102;
				}
			}
			if (m_engineOptions->SECTION_COL_SIZE == 20 && m_engineOptions->SECTION_ROW_SIZE == 20)
			{
				if (cameraPos.x > 13458)
					cameraPos.x = 13458;

				if (mIsEditor)
				{
					if (cameraPos.y < -9160)//-9248)
						cameraPos.y = -9160;//-9248;
				}
				else
				{
					if (cameraPos.y < -9222)
						cameraPos.y = -9222;
				}
			}
			if (m_engineOptions->SECTION_COL_SIZE == 30 && m_engineOptions->SECTION_ROW_SIZE == 30)
			{
				if (cameraPos.x > 21138)
					cameraPos.x = 21138;

				if (mIsEditor)
				{
					if (cameraPos.y < -14280)//-14368)
						cameraPos.y = -14280;//-14368;
				}
				else
				{
					if (cameraPos.y < -14342)
						cameraPos.y = -14342;
				}
			}
		}
		#pragma endregion
	}

	m_camera->SetPosition(cameraPos.x, cameraPos.y, cameraPos.z);
}*/
//=================================================================================================================
vector<Section*> TopdownMap::GetRenderedSections()
{
	vector<Section*> rend_secs;
	vector<Section*> sections = m_tileSystem->GetSections();
	//Loop through all the sections in the map
	for (int sec_id = 0; sec_id < sections.size(); sec_id++)
	{
		int sec = mSectionRenderList[sec_id];
		if (sections[sec])
		{
			rend_secs.push_back(sections[sec]);
		}
	}
	return rend_secs;
}
//=================================================================================================================
void TopdownMap::ClipSections(XMFLOAT3 pos)
{
	//Setup the rendering bounds for the screen and camera
	float camera_screen_x = pos.x;
	float camera_screen_y = pos.y;
	
	AISprite* player = GetPlayer();

	if (player == NULL) return;

	float playerX = player->TopLeftPosition().x;
	float playerY = player->TopLeftPosition().y;
	int playerW = player->Width();
	int playerH = player->Height();

	if (mSectionRenderList.size() > 0) mSectionRenderList.clear();

	// Locate the section the player is in
	bool bailout = false;
	Tile* tile;
	Section::BB bounds;
	Section* current_section;
	Section* player_in = NULL;
	int curr_sec = 0;
	vector<Section*> sections = m_tileSystem->GetSections();
	//Loop through all the sections in the map
	for (int sec_id = 0; sec_id < sections.size(); sec_id++)
	{
		bailout = false;

		current_section = sections[sec_id];

		for (int tile_y = 0; tile_y < m_engineOptions->COL_SIZE; tile_y++)
		{
			for (int tile_x = 0; tile_x < m_engineOptions->ROW_SIZE; tile_x++)
			{
				tile = current_section->GetTile(tile_x, tile_y);

				float realTileX = tile->GetPosition().x;
				float realTileY = tile->GetPosition().y;

				if (realTileX >= playerX && realTileX <= playerX + playerW)
				{
					if (realTileY >= playerY && realTileY <= playerY + playerH)
					{
						curr_sec = sec_id;
						player_in = current_section;
						player->SetSectionIn(player_in);
						bailout = true;
						break;
					}
				}
			}

			if (bailout) break;
		}

		if (bailout) break;
	}

	if (player_in == NULL) return;

	mSectionRenderList.push_back(curr_sec);
	
	// Render distance toggling
	// 1 -> 1 sections
	// 2 -> 9 sections
	// 3 -> 16 sections

	// If low render distance is used then we are done
	if (m_engineOptions->m_renderDistance2D == 1)
		return;

	SectionAdjacency* adj = player_in->AdjacencyInfo();

	if (adj != NULL)
	{
		for (int i = 0; i < adj->adjSections.size(); i++)
		{
			AdjacentSection at = adj->adjSections[i];

			mSectionRenderList.push_back(at.ID);
		}

		int NW_Corner_east1 = 0;
		int NW_Corner_east2 = 0;
		int NW_Corner_se = 0;
		int NW_Corner_south1 = 0;
		int NW_Corner_south2 = 0;

		int NE_Corner_west1 = 0;
		int NE_Corner_west2 = 0;
		int NE_Corner_sw = 0;
		int NE_Corner_south1 = 0;
		int NE_Corner_south2 = 0;

		int SW_Corner_east1 = 0;
		int SW_Corner_east2 = 0;
		int SW_Corner_ne = 0;
		int SW_Corner_north1 = 0;
		int SW_Corner_north2 = 0;

		int SE_Corner_west1 = 0;
		int SE_Corner_west2 = 0;
		int SE_Corner_nw = 0;
		int SE_Corner_north1 = 0;
		int SE_Corner_north2 = 0;

		int North_Edge_south1 = 0;
		int North_Edge_south2 = 0;
		int North_Edge_south3 = 0;

		int South_Edge_north1 = 0;
		int South_Edge_north2 = 0;
		int South_Edge_north3 = 0;

		int West_Edge_east1 = 0;
		int West_Edge_east2 = 0;
		int West_Edge_east3 = 0;

		int East_Edge_west1 = 0;
		int East_Edge_west2 = 0;
		int East_Edge_west3 = 0;

		// Corner section
		if (adj->adjSections.size() == 3)
		{
			// There is 4 total so we want to add 5 more (The ones that are adjacent to the adjacent sections)

			#pragma region "NW Corner"
			if (adj->type == NW_CORNER)
			{
				for (int i = 0; i < adj->adjSections.size(); i++)
				{
					AdjacentSection at = adj->adjSections[i];

					if (at.neighborPos == EAST)
					{
						NW_Corner_east1 = at.ID + m_engineOptions->SECTION_ROW_SIZE;
					}

					if (at.neighborPos == SE)
					{
						NW_Corner_east2 = NW_Corner_east1 + 1;
						NW_Corner_se = NW_Corner_east2 + 1;
						NW_Corner_south1 = at.ID + 1;
					}

					if (at.neighborPos == SOUTH)
					{
						NW_Corner_south2 = at.ID + 1;
					}
				}

				mSectionRenderList.push_back(NW_Corner_east1);
				mSectionRenderList.push_back(NW_Corner_east2);
				mSectionRenderList.push_back(NW_Corner_se);
				mSectionRenderList.push_back(NW_Corner_south1);
				mSectionRenderList.push_back(NW_Corner_south2);
			}
			#pragma endregion
			#pragma region "NE Corner"
			if (adj->type == NE_CORNER)
			{
				for (int i = 0; i < adj->adjSections.size(); i++)
				{
					AdjacentSection at = adj->adjSections[i];

					if (at.neighborPos == WEST)
					{
						NE_Corner_west1 = at.ID - m_engineOptions->SECTION_ROW_SIZE;
					}

					if (at.neighborPos == SW)
					{
						NE_Corner_west2 = NE_Corner_west1 + 1;
						NE_Corner_sw = NE_Corner_west2 + 1;
						NE_Corner_south1 = at.ID + 1;
					}

					if (at.neighborPos == SOUTH)
					{
						NE_Corner_south2 = at.ID + 1;
					}
				}

				mSectionRenderList.push_back(NE_Corner_west1);
				mSectionRenderList.push_back(NE_Corner_west2);
				mSectionRenderList.push_back(NE_Corner_sw);
				mSectionRenderList.push_back(NE_Corner_south1);
				mSectionRenderList.push_back(NE_Corner_south2);
			}
			#pragma endregion
			#pragma region "SW Corner"
			if (adj->type == SW_CORNER)
			{
				for (int i = 0; i < adj->adjSections.size(); i++)
				{
					AdjacentSection at = adj->adjSections[i];

					if (at.neighborPos == EAST)
					{
						SW_Corner_east1 = at.ID + m_engineOptions->SECTION_ROW_SIZE;
					}

					if (at.neighborPos == NE)
					{
						SW_Corner_east2 = at.ID + m_engineOptions->SECTION_ROW_SIZE;
						SW_Corner_ne = SW_Corner_east2 - 1;
						SW_Corner_north1 = at.ID - 1;
					}

					if (at.neighborPos == NORTH)
					{
						SW_Corner_north2 = at.ID - 1;
					}
				}

				mSectionRenderList.push_back(SW_Corner_east1);
				mSectionRenderList.push_back(SW_Corner_east2);
				mSectionRenderList.push_back(SW_Corner_ne);
				mSectionRenderList.push_back(SW_Corner_north1);
				mSectionRenderList.push_back(SW_Corner_north2);
			}
			#pragma endregion
			#pragma region "SE Corner"
			if (adj->type == SE_CORNER)
			{
				for (int i = 0; i < adj->adjSections.size(); i++)
				{
					AdjacentSection at = adj->adjSections[i];

					if (at.neighborPos == WEST)
					{
						SE_Corner_west1 = at.ID - m_engineOptions->SECTION_ROW_SIZE;
					}

					if (at.neighborPos == NW)
					{
						SE_Corner_west2 = at.ID - m_engineOptions->SECTION_ROW_SIZE;
						SE_Corner_nw = SE_Corner_west2 - 1;
						SE_Corner_north1 = at.ID - 1;
					}

					if (at.neighborPos == NORTH)
					{
						SE_Corner_north2 = at.ID - 1;
					}
				}

				mSectionRenderList.push_back(SE_Corner_west1);
				mSectionRenderList.push_back(SE_Corner_west2);
				mSectionRenderList.push_back(SE_Corner_nw);
				mSectionRenderList.push_back(SE_Corner_north1);
				mSectionRenderList.push_back(SE_Corner_north2);
			}
			#pragma endregion
		}
		
		// Edge section
		if (adj->adjSections.size() == 5)
		{
			// There is 6 total so we want to add 3 more

			#pragma region "North Edge"
			if (adj->type == NORTH_EDGE)
			{
				for (int i = 0; i < adj->adjSections.size(); i++)
				{
					AdjacentSection at = adj->adjSections[i];

					if (at.neighborPos == SW)
					{
						North_Edge_south1 = at.ID + 1;
					}

					if (at.neighborPos == SOUTH)
					{
						North_Edge_south2 = at.ID + 1;
					}

					if (at.neighborPos == SE)
					{
						North_Edge_south3 = at.ID + 1;
					}
				}

				mSectionRenderList.push_back(North_Edge_south1);
				mSectionRenderList.push_back(North_Edge_south2);
				mSectionRenderList.push_back(North_Edge_south3);
			}
			#pragma endregion
			#pragma region "South Edge"
			if (adj->type == SOUTH_EDGE)
			{
				for (int i = 0; i < adj->adjSections.size(); i++)
				{
					AdjacentSection at = adj->adjSections[i];

					if (at.neighborPos == NW)
					{
						South_Edge_north1 = at.ID - 1;
					}

					if (at.neighborPos == NORTH)
					{
						South_Edge_north2 = at.ID - 1;
					}

					if (at.neighborPos == NE)
					{
						South_Edge_north3 = at.ID - 1;
					}
				}

				mSectionRenderList.push_back(South_Edge_north1);
				mSectionRenderList.push_back(South_Edge_north2);
				mSectionRenderList.push_back(South_Edge_north3);
			}
			#pragma endregion
			#pragma region "West Edge"
			if (adj->type == WEST_EDGE)
			{
				for (int i = 0; i < adj->adjSections.size(); i++)
				{
					AdjacentSection at = adj->adjSections[i];

					if (at.neighborPos == NE)
					{
						West_Edge_east1 = at.ID + m_engineOptions->SECTION_ROW_SIZE;
					}

					if (at.neighborPos == EAST)
					{
						West_Edge_east2 = at.ID + m_engineOptions->SECTION_ROW_SIZE;
					}

					if (at.neighborPos == SE)
					{
						West_Edge_east3 = at.ID + m_engineOptions->SECTION_ROW_SIZE;
					}
				}

				mSectionRenderList.push_back(West_Edge_east1);
				mSectionRenderList.push_back(West_Edge_east2);
				mSectionRenderList.push_back(West_Edge_east3);
			}
			#pragma endregion
			#pragma region "East Edge"
			if (adj->type == EAST_EDGE)
			{
				for (int i = 0; i < adj->adjSections.size(); i++)
				{
					AdjacentSection at = adj->adjSections[i];

					if (at.neighborPos == NW)
					{
						East_Edge_west1 = at.ID - m_engineOptions->SECTION_ROW_SIZE;
					}

					if (at.neighborPos == WEST)
					{
						East_Edge_west2 = at.ID - m_engineOptions->SECTION_ROW_SIZE;
					}

					if (at.neighborPos == SW)
					{
						East_Edge_west3 = at.ID - m_engineOptions->SECTION_ROW_SIZE;
					}
				}

				mSectionRenderList.push_back(East_Edge_west1);
				mSectionRenderList.push_back(East_Edge_west2);
				mSectionRenderList.push_back(East_Edge_west3);
			}
			#pragma endregion
		}

		// If render distance is 2 then we are done because we have 9 sections available
		if (m_engineOptions->m_renderDistance2D == 2)
			return;

		// If the map size is 3x3 then we are done because there is only 9 sections available
		if (m_engineOptions->SECTION_COL_SIZE == 3 && m_engineOptions->SECTION_ROW_SIZE == 3)
			return;

		// Continue building up to 16 sections if render distance is 3

		// Corner section
		if (adj->adjSections.size() == 3)
		{
			// Add 7 more sections to the 5 new sections that were added

			#pragma region "Corner"
			if (adj->type == NW_CORNER)
			{
				int east1 = NW_Corner_east1 + m_engineOptions->SECTION_COL_SIZE;
				int east2 = east1 + 1;
				int east3 = east2 + 1;
				int se = east3 + 1;
				int south1 = NW_Corner_se + 1;
				int south2 = NW_Corner_south1 + 1;
				int south3 = NW_Corner_south2 + 1;

				mSectionRenderList.push_back(east1);
				mSectionRenderList.push_back(east2);
				mSectionRenderList.push_back(east3);
				mSectionRenderList.push_back(se);
				mSectionRenderList.push_back(south1);
				mSectionRenderList.push_back(south2);
				mSectionRenderList.push_back(south3);
			}

			if (adj->type == SW_CORNER)
			{
				int east1 = SW_Corner_east1 + m_engineOptions->SECTION_COL_SIZE;
				int east2 = east1 - 1;
				int east3 = east2 - 1;
				int se = east3 - 1;
				int north1 = SW_Corner_ne - 1;
				int north2 = SW_Corner_north1 - 1;
				int north3 = SW_Corner_north2 - 1;

				mSectionRenderList.push_back(east1);
				mSectionRenderList.push_back(east2);
				mSectionRenderList.push_back(east3);
				mSectionRenderList.push_back(se);
				mSectionRenderList.push_back(north1);
				mSectionRenderList.push_back(north2);
				mSectionRenderList.push_back(north3);
			}

			if (adj->type == NE_CORNER)
			{
				int west1 = NE_Corner_west1 - m_engineOptions->SECTION_COL_SIZE;
				int west2 = west1 + 1;
				int west3 = west2 + 1;
				int sw = west3 + 1;
				int south1 = NE_Corner_sw + 1;
				int south2 = NE_Corner_south1 + 1;
				int south3 = NE_Corner_south2 + 1;

				mSectionRenderList.push_back(west1);
				mSectionRenderList.push_back(west2);
				mSectionRenderList.push_back(west3);
				mSectionRenderList.push_back(sw);
				mSectionRenderList.push_back(south1);
				mSectionRenderList.push_back(south2);
				mSectionRenderList.push_back(south3);
			}

			if (adj->type == SE_CORNER)
			{
				int west1 = SE_Corner_west1 - m_engineOptions->SECTION_COL_SIZE;
				int west2 = west1 - 1;
				int west3 = west2 - 1;
				int nw = west3 - 1;
				int north1 = SE_Corner_nw - 1;
				int north2 = SE_Corner_north1 - 1;
				int north3 = SE_Corner_north2 - 1;

				mSectionRenderList.push_back(west1);
				mSectionRenderList.push_back(west2);
				mSectionRenderList.push_back(west3);
				mSectionRenderList.push_back(nw);
				mSectionRenderList.push_back(north1);
				mSectionRenderList.push_back(north2);
				mSectionRenderList.push_back(north3);
			}
			#pragma endregion
		}

		// Edge section
		if (adj->adjSections.size() == 5)
		{
			// Add 7 more to the 3 new sections that were added

			#pragma region "North Edge"
			if (adj->type == NORTH_EDGE)
			{
				int nw_corner = adj->ID - m_engineOptions->SECTION_COL_SIZE;
				int ne_corner = adj->ID + m_engineOptions->SECTION_COL_SIZE;

				bool InMiddle = false;
				bool found_nw_corner = false;

				if (nw_corner == GetCornerSectionNumberByMapSize(m_engineOptions->SECTION_COL_SIZE, NW_CORNER))
				{
					found_nw_corner = true;

					for (int i = 0; i < adj->adjSections.size(); i++)
					{
						AdjacentSection at = adj->adjSections[i];

						if (at.neighborPos == EAST)
						{
							mSectionRenderList.push_back(at.ID + m_engineOptions->SECTION_COL_SIZE);
						}

						if (at.neighborPos == SE)
						{
							mSectionRenderList.push_back(at.ID + m_engineOptions->SECTION_COL_SIZE);
						}
					}

					mSectionRenderList.push_back(North_Edge_south3 + m_engineOptions->SECTION_COL_SIZE);
					mSectionRenderList.push_back(North_Edge_south3 + (m_engineOptions->SECTION_COL_SIZE + 1));

					mSectionRenderList.push_back(North_Edge_south1 + 1);
					mSectionRenderList.push_back(North_Edge_south2 + 1);
					mSectionRenderList.push_back(North_Edge_south3 + 1);
				}
				else
				{
					InMiddle = true;
				}
				
				if (ne_corner == GetCornerSectionNumberByMapSize(m_engineOptions->SECTION_COL_SIZE, NE_CORNER))
				{
					InMiddle = false;

					for (int i = 0; i < adj->adjSections.size(); i++)
					{
						AdjacentSection at = adj->adjSections[i];

						if (at.neighborPos == WEST)
						{
							mSectionRenderList.push_back(at.ID - m_engineOptions->SECTION_COL_SIZE);
						}

						if (at.neighborPos == SW)
						{
							mSectionRenderList.push_back(at.ID - m_engineOptions->SECTION_COL_SIZE);
						}
					}

					mSectionRenderList.push_back(North_Edge_south1 - m_engineOptions->SECTION_COL_SIZE);
					mSectionRenderList.push_back(North_Edge_south1 - (m_engineOptions->SECTION_COL_SIZE - 1));

					mSectionRenderList.push_back(North_Edge_south1 + 1);
					mSectionRenderList.push_back(North_Edge_south2 + 1);
					mSectionRenderList.push_back(North_Edge_south3 + 1);
				}
				else
				{
					if (!found_nw_corner) InMiddle = true;
				}

				if (InMiddle)
				{
					for (int i = 0; i < adj->adjSections.size(); i++)
					{
						AdjacentSection at = adj->adjSections[i];

						if (at.neighborPos == WEST)
						{
							mSectionRenderList.push_back(at.ID - m_engineOptions->SECTION_COL_SIZE);
						}

						if (at.neighborPos == SW)
						{
							mSectionRenderList.push_back(at.ID - m_engineOptions->SECTION_COL_SIZE);
						}
					}

					mSectionRenderList.push_back(North_Edge_south1 - m_engineOptions->SECTION_COL_SIZE);
					//mSectionRenderList.push_back(North_Edge_south1 - (m_engineOptions->SECTION_COL_SIZE - 1));

					for (int i = 0; i < adj->adjSections.size(); i++)
					{
						AdjacentSection at = adj->adjSections[i];

						if (at.neighborPos == EAST)
						{
							mSectionRenderList.push_back(at.ID + m_engineOptions->SECTION_COL_SIZE);
						}

						if (at.neighborPos == SE)
						{
							mSectionRenderList.push_back(at.ID + m_engineOptions->SECTION_COL_SIZE);
						}
					}

					mSectionRenderList.push_back(North_Edge_south3 + m_engineOptions->SECTION_COL_SIZE);
					mSectionRenderList.push_back(North_Edge_south3 + (m_engineOptions->SECTION_COL_SIZE + 1));
				}
			}
			#pragma endregion
			#pragma region "South Edge"
			if (adj->type == SOUTH_EDGE)
			{
				int sw_corner = adj->ID - m_engineOptions->SECTION_COL_SIZE;
				int se_corner = adj->ID + m_engineOptions->SECTION_COL_SIZE;

				bool InMiddle = false;
				bool found_sw_corner = false;

				if (sw_corner == GetCornerSectionNumberByMapSize(m_engineOptions->SECTION_COL_SIZE, SW_CORNER))
				{
					found_sw_corner = true;

					for (int i = 0; i < adj->adjSections.size(); i++)
					{
						AdjacentSection at = adj->adjSections[i];

						if (at.neighborPos == EAST)
						{
							mSectionRenderList.push_back(at.ID + m_engineOptions->SECTION_COL_SIZE);
						}

						if (at.neighborPos == NE)
						{
							mSectionRenderList.push_back(at.ID + m_engineOptions->SECTION_COL_SIZE);
						}
					}

					mSectionRenderList.push_back(South_Edge_north3 + m_engineOptions->SECTION_COL_SIZE);
					mSectionRenderList.push_back(South_Edge_north3 + (m_engineOptions->SECTION_COL_SIZE - 1));

					mSectionRenderList.push_back(South_Edge_north1 - 1);
					mSectionRenderList.push_back(South_Edge_north2 - 1);
					mSectionRenderList.push_back(South_Edge_north3 - 1);
				}
				else
				{
					InMiddle = true;
				}

				if (se_corner == GetCornerSectionNumberByMapSize(m_engineOptions->SECTION_COL_SIZE, SE_CORNER))
				{
					InMiddle = false;

					for (int i = 0; i < adj->adjSections.size(); i++)
					{
						AdjacentSection at = adj->adjSections[i];

						if (at.neighborPos == WEST)
						{
							mSectionRenderList.push_back(at.ID - m_engineOptions->SECTION_COL_SIZE);
						}

						if (at.neighborPos == NW)
						{
							mSectionRenderList.push_back(at.ID - m_engineOptions->SECTION_COL_SIZE);
						}
					}

					mSectionRenderList.push_back(South_Edge_north1 - m_engineOptions->SECTION_COL_SIZE);
					mSectionRenderList.push_back(South_Edge_north1 - (m_engineOptions->SECTION_COL_SIZE + 1));

					mSectionRenderList.push_back(South_Edge_north1 - 1);
					mSectionRenderList.push_back(South_Edge_north2 - 1);
					mSectionRenderList.push_back(South_Edge_north3 - 1);
				}
				else
				{
					if (!found_sw_corner) InMiddle = true;
				}

				if (InMiddle)
				{
					for (int i = 0; i < adj->adjSections.size(); i++)
					{
						AdjacentSection at = adj->adjSections[i];

						if (at.neighborPos == WEST)
						{
							mSectionRenderList.push_back(at.ID - m_engineOptions->SECTION_COL_SIZE);
						}

						if (at.neighborPos == NW)
						{
							mSectionRenderList.push_back(at.ID - m_engineOptions->SECTION_COL_SIZE);
						}
					}

					mSectionRenderList.push_back(South_Edge_north1 - m_engineOptions->SECTION_COL_SIZE);
					//mSectionRenderList.push_back(South_Edge_north1 - (m_engineOptions->SECTION_COL_SIZE + 1));

					for (int i = 0; i < adj->adjSections.size(); i++)
					{
						AdjacentSection at = adj->adjSections[i];

						if (at.neighborPos == EAST)
						{
							mSectionRenderList.push_back(at.ID + m_engineOptions->SECTION_COL_SIZE);
						}

						if (at.neighborPos == NE)
						{
							mSectionRenderList.push_back(at.ID + m_engineOptions->SECTION_COL_SIZE);
						}
					}

					mSectionRenderList.push_back(South_Edge_north3 + m_engineOptions->SECTION_COL_SIZE);
					mSectionRenderList.push_back(South_Edge_north3 + (m_engineOptions->SECTION_COL_SIZE - 1));
				}
			}
			#pragma endregion
			#pragma region "West Edge"
			if (adj->type == WEST_EDGE)
			{
				int nw_corner = adj->ID - 1;
				int sw_corner = adj->ID + 1;

				bool InMiddle = false;
				bool found_nw_corner = false;

				if (nw_corner == GetCornerSectionNumberByMapSize(m_engineOptions->SECTION_COL_SIZE, NW_CORNER))
				{
					found_nw_corner = true;

					for (int i = 0; i < adj->adjSections.size(); i++)
					{
						AdjacentSection at = adj->adjSections[i];

						if (at.neighborPos == SOUTH)
						{
							mSectionRenderList.push_back(at.ID + 1);
						}

						if (at.neighborPos == SE)
						{
							mSectionRenderList.push_back(at.ID + 1);
						}
					}

					mSectionRenderList.push_back(West_Edge_east3 + 1);
					mSectionRenderList.push_back(West_Edge_east3 + (m_engineOptions->SECTION_COL_SIZE + 1));

					mSectionRenderList.push_back(West_Edge_east1 + m_engineOptions->SECTION_COL_SIZE);
					mSectionRenderList.push_back(West_Edge_east2 + m_engineOptions->SECTION_COL_SIZE);
					mSectionRenderList.push_back(West_Edge_east3 + m_engineOptions->SECTION_COL_SIZE);
				}
				else
				{
					InMiddle = true;
				}

				if (sw_corner == GetCornerSectionNumberByMapSize(m_engineOptions->SECTION_COL_SIZE, SW_CORNER))
				{
					InMiddle = false;

					for (int i = 0; i < adj->adjSections.size(); i++)
					{
						AdjacentSection at = adj->adjSections[i];

						if (at.neighborPos == NORTH)
						{
							mSectionRenderList.push_back(at.ID - 1);
						}

						if (at.neighborPos == NE)
						{
							mSectionRenderList.push_back(at.ID - 1);
						}
					}

					mSectionRenderList.push_back(West_Edge_east1 - 1);
					mSectionRenderList.push_back(West_Edge_east1 - (m_engineOptions->SECTION_COL_SIZE - 1));

					mSectionRenderList.push_back(West_Edge_east1 + m_engineOptions->SECTION_COL_SIZE);
					mSectionRenderList.push_back(West_Edge_east2 + m_engineOptions->SECTION_COL_SIZE);
					mSectionRenderList.push_back(West_Edge_east3 + m_engineOptions->SECTION_COL_SIZE);
				}
				else
				{
					if (!found_nw_corner) InMiddle = true;
				}

				if (InMiddle)
				{
					for (int i = 0; i < adj->adjSections.size(); i++)
					{
						AdjacentSection at = adj->adjSections[i];

						if (at.neighborPos == NORTH)
						{
							mSectionRenderList.push_back(at.ID - 1);
						}

						if (at.neighborPos == NE)
						{
							mSectionRenderList.push_back(at.ID - 1);
						}
					}

					mSectionRenderList.push_back(West_Edge_east1 - 1);
					//mSectionRenderList.push_back(West_Edge_east1 - (m_engineOptions->SECTION_COL_SIZE - 1));

					for (int i = 0; i < adj->adjSections.size(); i++)
					{
						AdjacentSection at = adj->adjSections[i];

						if (at.neighborPos == SOUTH)
						{
							mSectionRenderList.push_back(at.ID + 1);
						}

						if (at.neighborPos == SE)
						{
							mSectionRenderList.push_back(at.ID + 1);
						}
					}

					mSectionRenderList.push_back(West_Edge_east3 + 1);
					mSectionRenderList.push_back(West_Edge_east3 + (m_engineOptions->SECTION_COL_SIZE + 1));
				}
			}
			#pragma endregion
			#pragma region "East Edge"
			if (adj->type == EAST_EDGE)
			{
				int ne_corner = adj->ID - 1;
				int se_corner = adj->ID + 1;

				bool InMiddle = false;
				bool found_ne_corner = false;

				if (ne_corner == GetCornerSectionNumberByMapSize(m_engineOptions->SECTION_COL_SIZE, NE_CORNER))
				{
					found_ne_corner = true;

					for (int i = 0; i < adj->adjSections.size(); i++)
					{
						AdjacentSection at = adj->adjSections[i];

						if (at.neighborPos == SOUTH)
						{
							mSectionRenderList.push_back(at.ID + 1);
						}

						if (at.neighborPos == SW)
						{
							mSectionRenderList.push_back(at.ID + 1);
						}
					}

					mSectionRenderList.push_back(East_Edge_west3 + 1);
					mSectionRenderList.push_back(East_Edge_west3 - (m_engineOptions->SECTION_COL_SIZE - 1));

					mSectionRenderList.push_back(East_Edge_west1 - m_engineOptions->SECTION_COL_SIZE);
					mSectionRenderList.push_back(East_Edge_west2 - m_engineOptions->SECTION_COL_SIZE);
					mSectionRenderList.push_back(East_Edge_west3 - m_engineOptions->SECTION_COL_SIZE);
				}
				else
				{
					InMiddle = true;
				}

				if (se_corner == GetCornerSectionNumberByMapSize(m_engineOptions->SECTION_COL_SIZE, SE_CORNER))
				{
					InMiddle = false;

					for (int i = 0; i < adj->adjSections.size(); i++)
					{
						AdjacentSection at = adj->adjSections[i];

						if (at.neighborPos == NORTH)
						{
							mSectionRenderList.push_back(at.ID - 1);
						}

						if (at.neighborPos == NW)
						{
							mSectionRenderList.push_back(at.ID - 1);
						}
					}

					mSectionRenderList.push_back(East_Edge_west1 - 1);
					mSectionRenderList.push_back(East_Edge_west1 - (m_engineOptions->SECTION_COL_SIZE + 1));

					mSectionRenderList.push_back(East_Edge_west1 - m_engineOptions->SECTION_COL_SIZE);
					mSectionRenderList.push_back(East_Edge_west2 - m_engineOptions->SECTION_COL_SIZE);
					mSectionRenderList.push_back(East_Edge_west3 - m_engineOptions->SECTION_COL_SIZE);
				}
				else
				{
					if (!found_ne_corner) InMiddle = true;
				}

				if (InMiddle)
				{
					for (int i = 0; i < adj->adjSections.size(); i++)
					{
						AdjacentSection at = adj->adjSections[i];

						if (at.neighborPos == NORTH)
						{
							mSectionRenderList.push_back(at.ID - 1);
						}

						if (at.neighborPos == NW)
						{
							mSectionRenderList.push_back(at.ID - 1);
						}
					}

					mSectionRenderList.push_back(East_Edge_west1 - 1);
					//mSectionRenderList.push_back(East_Edge_west1 - (m_engineOptions->SECTION_COL_SIZE + 1));

					for (int i = 0; i < adj->adjSections.size(); i++)
					{
						AdjacentSection at = adj->adjSections[i];

						if (at.neighborPos == SOUTH)
						{
							mSectionRenderList.push_back(at.ID + 1);
						}

						if (at.neighborPos == SW)
						{
							mSectionRenderList.push_back(at.ID + 1);
						}
					}

					mSectionRenderList.push_back(East_Edge_west3 + 1);
					mSectionRenderList.push_back(East_Edge_west3 - (m_engineOptions->SECTION_COL_SIZE - 1));
				}
			}
			#pragma endregion
		}

		// Center section
		if (adj->adjSections.size() == 8)
		{
			// Add 7 sections to the 9 that are initially rendered

			// Add to the west and to the north for the south east side of the map
			// Add to the east and to the south for the north east side of the map
			// Add to the west and to the south for the north west side of the map
			// Add to the east and to the north for the south west side of the map

			bool InSESide = IsInSectorCenter(m_engineOptions->SECTION_COL_SIZE, ST_SOUTH_EAST, adj->ID);
			bool InNESide = IsInSectorCenter(m_engineOptions->SECTION_COL_SIZE, ST_NORTH_EAST, adj->ID);
			bool InNWSide = IsInSectorCenter(m_engineOptions->SECTION_COL_SIZE, ST_NORTH_WEST, adj->ID);
			bool InSWSide = IsInSectorCenter(m_engineOptions->SECTION_COL_SIZE, ST_SOUTH_WEST, adj->ID);

			#pragma region "NW Sector"
			if (InNWSide)
			{
				if (IsInSpecialCaseSectorCenter(m_engineOptions->SECTION_COL_SIZE, ST_NORTH_WEST, adj->ID))
				{
					int east1 = 0;
					int east2 = 0;
					int east3 = 0;
					int se = 0;
					int south1 = 0;
					int south2 = 0;
					int south3 = 0;

					for (int i = 0; i < adj->adjSections.size(); i++)
					{
						AdjacentSection at = adj->adjSections[i];

						if (at.neighborPos == NE)
						{
							east1 = at.ID + m_engineOptions->SECTION_ROW_SIZE;
						}

						if (at.neighborPos == EAST)
						{
							east2 = at.ID + m_engineOptions->SECTION_ROW_SIZE;
						}

						if (at.neighborPos == SE)
						{
							east3 = at.ID + m_engineOptions->SECTION_ROW_SIZE;
							se = east3 + 1;
							south1 = at.ID + 1;
						}

						if (at.neighborPos == SOUTH)
						{
							south2 = at.ID + 1;
						}

						if (at.neighborPos == SW)
						{
							south3 = at.ID + 1;
						}
					}

					mSectionRenderList.push_back(east1);
					mSectionRenderList.push_back(east2);
					mSectionRenderList.push_back(east3);
					mSectionRenderList.push_back(se);
					mSectionRenderList.push_back(south1);
					mSectionRenderList.push_back(south2);
					mSectionRenderList.push_back(south3);
				}
				else
				{
					int west1 = 0;
					int west2 = 0;
					int west3 = 0;
					int nw = 0;
					int north1 = 0;
					int north2 = 0;
					int north3 = 0;

					for (int i = 0; i < adj->adjSections.size(); i++)
					{
						AdjacentSection at = adj->adjSections[i];

						if (at.neighborPos == SW)
						{
							west1 = at.ID - m_engineOptions->SECTION_ROW_SIZE;
						}

						if (at.neighborPos == WEST)
						{
							west2 = at.ID - m_engineOptions->SECTION_ROW_SIZE;
						}

						if (at.neighborPos == NW)
						{
							west3 = at.ID - m_engineOptions->SECTION_ROW_SIZE;
							nw = west3 - 1;
							north1 = at.ID - 1;
						}

						if (at.neighborPos == NORTH)
						{
							north2 = at.ID - 1;
						}

						if (at.neighborPos == NE)
						{
							north3 = at.ID - 1;
						}
					}

					mSectionRenderList.push_back(west1);
					mSectionRenderList.push_back(west2);
					mSectionRenderList.push_back(west3);
					mSectionRenderList.push_back(nw);
					mSectionRenderList.push_back(north1);
					mSectionRenderList.push_back(north2);
					mSectionRenderList.push_back(north3);
				}
			}
			#pragma endregion
			#pragma region "NE Sector"
			if (InNESide)
			{
				if (IsInSpecialCaseSectorCenter(m_engineOptions->SECTION_COL_SIZE, ST_NORTH_EAST, adj->ID))
				{
					int west1 = 0;
					int west2 = 0;
					int west3 = 0;
					int sw = 0;
					int south1 = 0;
					int south2 = 0;
					int south3 = 0;

					for (int i = 0; i < adj->adjSections.size(); i++)
					{
						AdjacentSection at = adj->adjSections[i];

						if (at.neighborPos == NW)
						{
							west1 = at.ID - m_engineOptions->SECTION_ROW_SIZE;
						}

						if (at.neighborPos == WEST)
						{
							west2 = at.ID - m_engineOptions->SECTION_ROW_SIZE;
						}

						if (at.neighborPos == SW)
						{
							west3 = at.ID - m_engineOptions->SECTION_ROW_SIZE;
							sw = west3 + 1;
							south1 = at.ID + 1;
						}

						if (at.neighborPos == SOUTH)
						{
							south2 = at.ID + 1;
						}

						if (at.neighborPos == SE)
						{
							south3 = at.ID + 1;
						}
					}

					mSectionRenderList.push_back(west1);
					mSectionRenderList.push_back(west2);
					mSectionRenderList.push_back(west3);
					mSectionRenderList.push_back(sw);
					mSectionRenderList.push_back(south1);
					mSectionRenderList.push_back(south2);
					mSectionRenderList.push_back(south3);
				}
				else
				{
					int west1 = 0;
					int west2 = 0;
					int west3 = 0;
					int nw = 0;
					int north1 = 0;
					int north2 = 0;
					int north3 = 0;

					for (int i = 0; i < adj->adjSections.size(); i++)
					{
						AdjacentSection at = adj->adjSections[i];

						if (at.neighborPos == SW)
						{
							west1 = at.ID - m_engineOptions->SECTION_ROW_SIZE;
						}

						if (at.neighborPos == WEST)
						{
							west2 = at.ID - m_engineOptions->SECTION_ROW_SIZE;
						}

						if (at.neighborPos == NW)
						{
							west3 = at.ID - m_engineOptions->SECTION_ROW_SIZE;
							nw = west3 - 1;
							north1 = at.ID - 1;
						}

						if (at.neighborPos == NORTH)
						{
							north2 = at.ID - 1;
						}

						if (at.neighborPos == NE)
						{
							north3 = at.ID - 1;
						}
					}

					mSectionRenderList.push_back(west1);
					mSectionRenderList.push_back(west2);
					mSectionRenderList.push_back(west3);
					mSectionRenderList.push_back(nw);
					mSectionRenderList.push_back(north1);
					mSectionRenderList.push_back(north2);
					mSectionRenderList.push_back(north3);
				}
			}
			#pragma endregion
			#pragma region "SW Sector"
			if (InSWSide)
			{
				if (IsInSpecialCaseSectorCenter(m_engineOptions->SECTION_COL_SIZE, ST_SOUTH_WEST, adj->ID))
				{
					int east1 = 0;
					int east2 = 0;
					int east3 = 0;
					int ne = 0;
					int north1 = 0;
					int north2 = 0;
					int north3 = 0;

					for (int i = 0; i < adj->adjSections.size(); i++)
					{
						AdjacentSection at = adj->adjSections[i];

						if (at.neighborPos == SE)
						{
							east1 = at.ID + m_engineOptions->SECTION_ROW_SIZE;
						}

						if (at.neighborPos == EAST)
						{
							east2 = at.ID + m_engineOptions->SECTION_ROW_SIZE;
						}

						if (at.neighborPos == NE)
						{
							east3 = at.ID + m_engineOptions->SECTION_ROW_SIZE;
							ne = east3 - 1;
							north1 = at.ID - 1;
						}

						if (at.neighborPos == NORTH)
						{
							north2 = at.ID - 1;
						}

						if (at.neighborPos == NW)
						{
							north3 = at.ID - 1;
						}
					}

					mSectionRenderList.push_back(east1);
					mSectionRenderList.push_back(east2);
					mSectionRenderList.push_back(east3);
					mSectionRenderList.push_back(ne);
					mSectionRenderList.push_back(north1);
					mSectionRenderList.push_back(north2);
					mSectionRenderList.push_back(north3);
				}
				else
				{
					int west1 = 0;
					int west2 = 0;
					int west3 = 0;
					int nw = 0;
					int north1 = 0;
					int north2 = 0;
					int north3 = 0;

					for (int i = 0; i < adj->adjSections.size(); i++)
					{
						AdjacentSection at = adj->adjSections[i];

						if (at.neighborPos == SW)
						{
							west1 = at.ID - m_engineOptions->SECTION_ROW_SIZE;
						}

						if (at.neighborPos == WEST)
						{
							west2 = at.ID - m_engineOptions->SECTION_ROW_SIZE;
						}

						if (at.neighborPos == NW)
						{
							west3 = at.ID - m_engineOptions->SECTION_ROW_SIZE;
							nw = west3 - 1;
							north1 = at.ID - 1;
						}

						if (at.neighborPos == NORTH)
						{
							north2 = at.ID - 1;
						}

						if (at.neighborPos == NE)
						{
							north3 = at.ID - 1;
						}
					}

					mSectionRenderList.push_back(west1);
					mSectionRenderList.push_back(west2);
					mSectionRenderList.push_back(west3);
					mSectionRenderList.push_back(nw);
					mSectionRenderList.push_back(north1);
					mSectionRenderList.push_back(north2);
					mSectionRenderList.push_back(north3);
				}
			}
			#pragma endregion
			#pragma region "SE Sector"
			if (InSESide)
			{
				int west1 = 0;
				int west2 = 0;
				int west3 = 0;
				int nw = 0;
				int north1 = 0;
				int north2 = 0;
				int north3 = 0;

				for (int i = 0; i < adj->adjSections.size(); i++)
				{
					AdjacentSection at = adj->adjSections[i];

					if (at.neighborPos == SW)
					{
						west1 = at.ID - m_engineOptions->SECTION_ROW_SIZE;
					}

					if (at.neighborPos == WEST)
					{
						west2 = at.ID - m_engineOptions->SECTION_ROW_SIZE;
					}

					if (at.neighborPos == NW)
					{
						west3 = at.ID - m_engineOptions->SECTION_ROW_SIZE;
						nw = west3 - 1;
						north1 = at.ID - 1;
					}

					if (at.neighborPos == NORTH)
					{
						north2 = at.ID - 1;
					}

					if (at.neighborPos == NE)
					{
						north3 = at.ID - 1;
					}
				}

				mSectionRenderList.push_back(west1);
				mSectionRenderList.push_back(west2);
				mSectionRenderList.push_back(west3);
				mSectionRenderList.push_back(nw);
				mSectionRenderList.push_back(north1);
				mSectionRenderList.push_back(north2);
				mSectionRenderList.push_back(north3);
			}
			#pragma endregion
		}
	}
}
//=================================================================================================================
bool TopdownMap::IsInSpecialCaseSectorCenter(int map_size, SectorType type, int id)
{
	vector<int> arrNW;
	vector<int> arrNE;
	vector<int> arrSW;

	if (map_size == 3)
	{
		// Not valid since there are only 9 sections
		return false;
	}
	else if (map_size == 6)
	{
		arrNW = SectorBuilder::arrNWSpecialCase6x6;
		arrNE = SectorBuilder::arrNESpecialCase6x6;
		arrSW = SectorBuilder::arrSWSpecialCase6x6;
	}
	else if (map_size == 8)
	{
		arrNW = SectorBuilder::arrNWSpecialCase8x8;
		arrNE = SectorBuilder::arrNESpecialCase8x8;
		arrSW = SectorBuilder::arrSWSpecialCase8x8;
	}
	else if (map_size == 10)
	{
		arrNW = SectorBuilder::arrNWSpecialCase10x10;
		arrNE = SectorBuilder::arrNESpecialCase10x10;
		arrSW = SectorBuilder::arrSWSpecialCase10x10;
	}
	else if (map_size == 20)
	{
		arrNW = SectorBuilder::arrNWSpecialCase20x20;
		arrNE = SectorBuilder::arrNESpecialCase20x20;
		arrSW = SectorBuilder::arrSWSpecialCase20x20;
	}
	else if (map_size == 30)
	{
		arrNW = SectorBuilder::arrNWSpecialCase30x30;
		arrNE = SectorBuilder::arrNESpecialCase30x30;
		arrSW = SectorBuilder::arrSWSpecialCase30x30;
	}

	if (type == ST_NORTH_WEST)
	{
		if (arrNW.size() == 0) return false;

		if (BinarySearch::Search(arrNW, id))
			return true;
	}

	if (type == ST_NORTH_EAST)
	{
		if (arrNE.size() == 0) return false;

		if (BinarySearch::Search(arrNE, id))
			return true;
	}

	if (type == ST_SOUTH_WEST)
	{
		if (arrSW.size() == 0) return false;

		if (BinarySearch::Search(arrSW, id))
			return true;
	}

	return false;
}
//=================================================================================================================
bool TopdownMap::IsInSectorCenter(int map_size, SectorType type, int id)
{
	vector<int> arrNW;
	vector<int> arrNE;
	vector<int> arrSW;
	vector<int> arrSE;

	if (map_size == 3)
	{
		// Not valid since there are only 9 sections
		return false;
	}
	else if (map_size == 6)
	{
		arrNW = SectorBuilder::arrNW6x6;
		arrNE = SectorBuilder::arrNE6x6;
		arrSW = SectorBuilder::arrSW6x6;
		arrSE = SectorBuilder::arrSE6x6;
	}
	else if (map_size == 8)
	{
		arrNW = SectorBuilder::arrNW8x8;
		arrNE = SectorBuilder::arrNE8x8;
		arrSW = SectorBuilder::arrSW8x8;
		arrSE = SectorBuilder::arrSE8x8;
	}
	else if (map_size == 10)
	{
		arrNW = SectorBuilder::arrNW10x10;
		arrNE = SectorBuilder::arrNE10x10;
		arrSW = SectorBuilder::arrSW10x10;
		arrSE = SectorBuilder::arrSE10x10;
	}
	else if (map_size == 20)
	{
		arrNW = SectorBuilder::arrNW20x20;
		arrNE = SectorBuilder::arrNE20x20;
		arrSW = SectorBuilder::arrSW20x20;
		arrSE = SectorBuilder::arrSE20x20;
	}
	else if (map_size == 30)
	{
		arrNW = SectorBuilder::arrNW30x30;
		arrNE = SectorBuilder::arrNE30x30;
		arrSW = SectorBuilder::arrSW30x30;
		arrSE = SectorBuilder::arrSE30x30;
	}

	if (type == ST_NORTH_WEST)
	{
		if (arrNW.size() == 0) return false;

		if (BinarySearch::Search(arrNW, id))
			return true;
	}

	if (type == ST_NORTH_EAST)
	{
		if (arrNE.size() == 0) return false;

		if (BinarySearch::Search(arrNE, id))
			return true;
	}

	if (type == ST_SOUTH_WEST)
	{
		if (arrSW.size() == 0) return false;

		if (BinarySearch::Search(arrSW, id))
			return true;
	}

	if (type == ST_SOUTH_EAST)
	{
		if (arrSE.size() == 0) return false;

		if (BinarySearch::Search(arrSE, id))
			return true;
	}

	return false;
}
//=================================================================================================================
vector<int> TopdownMap::BuildSector(int map_size, int begin)
{
	vector<int> vals;

	int perSector = 0;

	if (map_size == 8)
	{
		perSector = 3;
	}

	if (map_size == 10)
	{
		perSector = 4;
	}

	if (map_size == 20)
	{
		perSector = 9;
	}

	if (map_size == 30)
	{
		perSector = 14;
	}

	int t = begin;
	
	for (int j = 0; j < perSector; j++)
	{
		for (int i = t; i < t * perSector; i += perSector)
		{
			vals.push_back(i);
		}

		t++;
	}

	return vals;
}
//=================================================================================================================
int TopdownMap::GetCornerSectionNumberByMapSize(int map_size, SAType type)
{
	if (map_size == 3)
	{
		if (type == NW_CORNER)
			return 0;
		else if (type == NE_CORNER)
			return 6;
		else if (type == SW_CORNER)
			return 2;
		else if (type == SE_CORNER)
			return 8;
	}
	else if (map_size == 6)
	{
		if (type == NW_CORNER)
			return 0;
		else if (type == NE_CORNER)
			return 30;
		else if (type == SW_CORNER)
			return 5;
		else if (type == SE_CORNER)
			return 35;
	}
	else if (map_size == 8)
	{
		if (type == NW_CORNER)
			return 0;
		else if (type == NE_CORNER)
			return 56;
		else if (type == SW_CORNER)
			return 7;
		else if (type == SE_CORNER)
			return 63;
	}
	else if (map_size == 10)
	{
		if (type == NW_CORNER)
			return 0;
		else if (type == NE_CORNER)
			return 90;
		else if (type == SW_CORNER)
			return 9;
		else if (type == SE_CORNER)
			return 99;
	}
	else if (map_size == 20)
	{
		if (type == NW_CORNER)
			return 0;
		else if (type == NE_CORNER)
			return 380;
		else if (type == SW_CORNER)
			return 19;
		else if (type == SE_CORNER)
			return 399;
	}
	else if (map_size == 30)
	{
		if (type == NW_CORNER)
			return 0;
		else if (type == NE_CORNER)
			return 870;
		else if (type == SW_CORNER)
			return 29;
		else if (type == SE_CORNER)
			return 899;
	}
}
//=================================================================================================================
void TopdownMap::MoveTiles(float speed, int dirUD, int dirLR)
{
	Tile* tile;
	Section* current_section;
	vector<Section*> sections = m_tileSystem->GetSections();
	//Loop through all the sections in the map
	for (int sec_id = 0; sec_id < sections.size(); sec_id++)
	{
		current_section = sections[sec_id];

		for (int tile_y = 0; tile_y < m_engineOptions->COL_SIZE; tile_y++)
		{
			for (int tile_x = 0; tile_x < m_engineOptions->ROW_SIZE; tile_x++)
			{
				tile = current_section->GetTile(tile_x, tile_y);

				float realTileX = tile->GetPosition().x;
				float realTileY = tile->GetPosition().y;

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
	}
}
//=================================================================================================================
void TopdownMap::TransformMap(HWND hwnd, Keyboard* keyboard, bool isSectionPicker)
{
	m_transforming = false;

	bool needToClip = false;

	/*AISprite* spr1;
	AISprite* spr2;
	//Check if any sprites are colliding
	for (int i = 0; i < sprite_list.size(); i++)
	{
		spr1 = sprite_list[i].sprite;

		if (spr1 != NULL)
		{
			//Check if any sprites are colliding
			for (int j = 0; j < sprite_list.size(); j++)
			{
				spr2 = sprite_list[j].sprite;

				if (spr2 != NULL)
				{
					if (spr1 != spr2)
					{
						if (spr1->CollidesRect( spr2 ))
						{
							spr1->SetCollidedSprite( spr2 );
							spr2->SetCollidedSprite( spr1 );

							//Only move the sprite if it is dynamic
							//Can also use this to push sprites
							if (!spr1->IsStatic())
							{
								//Uses SAT to stop the sprite from moving
								spr1->CollidesWith( spr2 );
							}
						}
					}
				}
			}
		}
	}*/

	//Move the player
	MovePlayer(keyboard);

	// Transform the other sprites if they can move
	AISprite* spr = NULL;
	for (int i = 0; i < sprite_list.size(); i++)
	{
		spr = sprite_list[i].sprite;

		if (spr != NULL)
		{
			/*
			//Move the sprite (The AI Script will be moving other sprites)
			spr->MoveScript(Commands::GetInstance()->GetLuaState(), Commands::GetInstance()->GetGD());

			float sprX = spr->GetPosition().x;
			float sprY = spr->GetPosition().y;

			float sprHBX = spr->GetHardBoxSpr()->GetPosition().x;
			float sprHBY = spr->GetHardBoxSpr()->GetPosition().y;

			// Clip the sprite
			ClipSpriteToMap(spr, spr->GetSpeed(), sprX, sprY, sprHBX, sprHBY);

			// Update the sprites position in case it has been clipped to the map
			spr->Move(sprX, sprY, sprHBX, sprHBY);
			*/
		}
	}

	AISprite* player = GetPlayer();

	float speed = player->Speed();

	if (player == NULL) return;

	int dirUD = -1;
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

		for (int i = 0; i < m_visionSprites.size(); i++)
		{
			if (m_visionSprites[i] == NULL) continue;

			AISprite* s = m_visionSprites[i];

			float newX = s->TopLeftPosition().x;
			float newY = s->TopLeftPosition().y;

			float newHBX = s->GetHardBoxSpr()->TopLeftPosition().x;
			float newHBY = s->GetHardBoxSpr()->TopLeftPosition().y;

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

		ClipSections(player->TopLeftPosition());
	}
}
//=================================================================================================================
XMFLOAT3 TopdownMap::GetCameraPosition()
{
	XMFLOAT3 pos(m_camera->Position().x, m_camera->Position().y, m_camera->Position().z);
	return pos;
}
//=================================================================================================================
void TopdownMap::ToggleHardness(bool bEnable)
{
	// Set hardness for the tiles
	Tile* tile;
	Section* current_section;
	int curr_sec = 0;
	vector<Section*> sections = m_tileSystem->GetSections();
	//Loop through all the sections in the map
	for (int sec_id = 0; sec_id < sections.size(); sec_id++)
	{
		current_section = sections[sec_id];

		for (int tile_y = 0; tile_y < m_engineOptions->COL_SIZE; tile_y++)
		{
			for (int tile_x = 0; tile_x < m_engineOptions->ROW_SIZE; tile_x++)
			{
				tile = current_section->GetTile(tile_x, tile_y);
				tile->SetDisplayHardbox(bEnable);
			}
		}
	}

	// Set hardness for the sprites
	for (int i = 0; i < m_visionSprites.size(); i++)
	{
		AISprite* s = m_visionSprites[i];
		s->CanDisplayHardbox() = bEnable;
	}
}
//=================================================================================================================
void TopdownMap::ToggleFow()
{
	//Each time fow is toggled, need to look at the map to set the tile bits to either fow_non or fow_all
	
	if (m_bFOW)
	{
		LoadSpriteFOWTiles(GetPlayer());

		m_tileSystem->FowToggled(mSectionRenderList);

		m_tileSystem->UpdateFog(mSectionRenderList, GetPlayer(), m_iFOWRadius);
		m_tileSystem->UpdateTextures(mSectionRenderList);
	}
}
//=================================================================================================================
/*
XMFLOAT2 TopdownMap::CameraSectionPosition(int sec, int x, int y)
{
	XMFLOAT2 pos;

	Section* p_section = m_tileSystem->GetSection(sec);
	XMFLOAT3 p_pos = p_section->GetTilePos(0,0);

	pos.x = p_pos.x;
	pos.y = -p_pos.y;

	//Fix camera position on edge of map
	int val_x = GetCameraRightBounds(x, y);
	int val_y = GetCameraDownBounds(x, y);
	
	if (m_engineOptions->m_screenWidth == 1366)
		val_x -= 342;

	if (pos.x > val_x)
		pos.x = val_x;

	if (pos.y < val_y)
		pos.y = val_y;

	if (pos.x == -0)
		pos.x = 0;
	if (pos.y == -0)
		pos.y = 0;

	return pos;
}
//=================================================================================================================
int Map::GetCameraDownBounds(int x, int y)
{
	int val = 0;
	if (y == 3 && x == 3)
	{
		if (mIsEditor)
			val = -794;//-768;
		else
			val = -768;
	}
	
	if (y == 6 && x == 6)
	{
		if (mIsEditor)
			val = -2330;//-2300;
		else
			val = -2300;
	}
	
	if (y == 8 && x == 8)
	{
		val = -3325;
	}
	
	if (y == 10 && x == 10)
	{
		if (mIsEditor)
			val = -4378;
		else
			val = -4350;
	}
	
	if (y == 20 && x == 20)
	{
		if (mIsEditor)
			val = -9497;
		else
			val = -9470;
	}
	
	if (y == 30 && x == 30)
	{
		if (mIsEditor)
			val = -14618;
		else
			val = -14591;//-14344;
	}

	return val;
}
//=================================================================================================================
int Map::GetCameraRightBounds(int x, int y)
{
	int val = 0;
	if (y == 3 && x == 3)
	{
		val = 938;//1280;
	}

	if (y == 6 && x == 6)
	{
		if (mIsEditor)
			val = 3242;
		else
			val = 3239;//3584
	}

	if (y == 8 && x == 8)
	{
		if (mIsEditor)
			val = 5120;
		else
			val = 4778;
	}

	if (y == 10 && x == 10)
	{
		if (mIsEditor)
			val = 6315;
		else
			val = 6314;//6655;
	}

	if (y == 20 && x == 20)
	{
		val = 13994;
	}

	if (y == 30 && x == 30)
	{
		if (mIsEditor)
			val = 21675;
		else
			val = 21675;//21142;
	}

	return val;
}
*/
//=================================================================================================================
void TopdownMap::AddSprite(AISprite* s)
{
	sorted = false;

	s->IsPlayer() = false;
	s->ID() = spriteNum++;
	m_visionSprites.push_back( s );

	m_insertSpeed += 0.5f;
}
//=================================================================================================================
void TopdownMap::AddSpriteNoIndex(AISprite* s)
{
	sorted = false;

	s->IsPlayer() = false;
	m_visionSprites.push_back( s );

	m_insertSpeed += 0.5f;
}
//=================================================================================================================
void TopdownMap::SortSprites()
{
	//Depth que is based on sorting the y position of the sprites on the map if it has a default depth of 0
	//otherwise use assigned sprite depth (range 1 - 2000)(default 0)

	//Get the depth of each sprite and put each sprite in a list

	//if (sorted) return;

	//sprite_list.clear();

	if (sprite_list.size() > 0)
	{
		sprite_list.clear();
		sprite_list.resize(MAX_SPRITES);
	}

	SPRQUE sque;
	AISprite* s;
	int temp_depth;
	int num = 0;
	for (int j = 0; j < m_visionSprites.size(); j++)
	{
		s = m_visionSprites[j];

		//Make sure the sprites vision match the current vision of the section
		if (s->Vision() != m_currentVision)
		{
			continue;
		}

		//If depth que of the sprite is 0 then use its y position
		//otherwise use its assigned depth
		if (s->Depth() == 0)
			temp_depth = s->TopLeftPosition().y + s->Height();
		else
			temp_depth = s->Depth();

		sque.sprite = s;
		sque.depth = temp_depth;
		sque.sprite_num = num;

		sprite_list[j] = sque;

		num++;
	}
	
	//quickSort(sprite_list, 0, sprite_list.size() - 1);

	//Use insersion sort to sort the sprite_list
	SPRQUE X;
	int j;
	for (int i = 1; i < sprite_list.size(); i++)
	{
		if (sprite_list[i].sprite == NULL) continue;

		X = sprite_list[i];
		
		for (j = i; j > 0 && sprite_list[j - 1].depth > X.depth; j--)
		{
			sprite_list[j] = sprite_list[j - 1];
		}

		sprite_list[j] = X;
	}

	//sorted = true;
}
//=================================================================================================================
int TopdownMap::TotalClippedSprites()
{
	return sprite_list.size();
}
//=================================================================================================================
Section* TopdownMap::SelectSection(int x, int y)
{
	return m_tileSystem->SelectSection(x, y);
}
//-----------------------------------------------------------------------------------------------------------------
/*bool TopdownMap::Render10(TextureShader* textureShader, TransparentShader* transparentShader, Camera* camera)
{
	//Render the tiles
	m_D3DSystem->TurnOnAlphaBlending();
		m_tileSystem->Render(textureShader, transparentShader, viewMatrix, worldMatrix, orthoMatrix, mSectionRenderList);
	m_D3DSystem->TurnOffAlphaBlending();

	//Render a sword
	//d3d->TurnOnAlphaBlending();
	//	sword->RenderTransparent(transparentShader, d3d, viewMatrix, worldMatrix, orthoMatrix, 0);
	//d3d->TurnOffAlphaBlending();

	//Render the sprites from the list after sorting the sprites into a list
	SortSprites();
	ClipSprites();
	for (int i = 0; i < sprite_list.size(); i++)
	{
		if (sprite_list[i].sprite == NULL) continue;

		AISprite* s = sprite_list[i].sprite;

		if (s->IsDisplaySprite())
		{
			if (!m_canRenderDisplaySprite)
			{
				continue;
			}
		}

		//Update the sprites behavior if their are any
		s->updateBehavior();

		//Update the animation if it exists
		s->UpdateAnimation();

		//Render the sprite
		m_D3DSystem->TurnOnAlphaBlending();
			s->RenderTransparent10(transparentShader, viewMatrix, worldMatrix, orthoMatrix, 0);
		m_D3DSystem->TurnOffAlphaBlending();
	}

	//Render the player with animation
	//player->UpdateAnimation();
	//d3d->TurnOnAlphaBlending();
	//	player->RenderTransparent(transparentShader, d3d, viewMatrix, worldMatrix, orthoMatrix, 0);
	//d3d->TurnOffAlphaBlending();
	//player->Render(textureShader, d3d, viewMatrix, worldMatrix, orthoMatrix);
	
	//If Fog of War is turned on check the map and render the fog in front of everything else
	if (m_engineOptions->m_fow)
	{
		//Any tile that is a fog tile will be rendered here instead of with the tiles
		//so that it can be rendered on top of sprites
		//we assess the fog tiles and then render them
		m_tileSystem->AssessFog(mSectionRenderList);
		m_D3DSystem->TurnOnAlphaBlending();
			m_tileSystem->RenderTopFog(textureShader, transparentShader, viewMatrix, worldMatrix, orthoMatrix);
		m_D3DSystem->TurnOffAlphaBlending();
	}

	//mapSprite->Render(textureShader, d3d, viewMatrix, worldMatrix, orthoMatrix);
	
	return true;
}*/
//-----------------------------------------------------------------------------------------------------------------
//bool TopdownMap::Render11(SpriteBatch* sb, TextureShader* textureShader, TransparentShader* transparentShader, Camera* camera)
bool TopdownMap::Render(Camera* camera)
{
	//Render the tiles
	m_D3DSystem->TurnOnAlphaBlending();
		m_tileSystem->Render(camera, mSectionRenderList);
	m_D3DSystem->TurnOffAlphaBlending();

	//Render a sword
	//d3d->TurnOnAlphaBlending();
	//	sword->RenderTransparent(transparentShader, d3d, viewMatrix, worldMatrix, orthoMatrix, 0);
	//d3d->TurnOffAlphaBlending();

	//Render the sprites from the list after sorting the sprites into a list
	SortSprites();
	ClipSprites();
	for (int i = 0; i < sprite_list.size(); i++)
	{
		if (sprite_list[i].sprite == NULL) continue;

		AISprite* s = sprite_list[i].sprite;

		if (s->IsDisplaySprite())
		{
			if (!m_canRenderDisplaySprite)
			{
				continue;
			}
		}

		//Update the sprites behavior if their are any
		s->updateBehavior();

		//Update the animation if it exists
		s->UpdateAnimation();

		//Render the sprite
		m_D3DSystem->TurnOnAlphaBlending();
			//s->RenderTransparent11(sb, transparentShader, camera, 0);
			s->Render(camera);
		m_D3DSystem->TurnOffAlphaBlending();
	}

	//Render the player with animation
	//player->UpdateAnimation();
	//d3d->TurnOnAlphaBlending();
	//	player->RenderTransparent(transparentShader, d3d, viewMatrix, worldMatrix, orthoMatrix, 0);
	//d3d->TurnOffAlphaBlending();
	//player->Render(textureShader, d3d, viewMatrix, worldMatrix, orthoMatrix);
	
	//If Fog of War is turned on check the map and render the fog in front of everything else
	if (m_bFOW)
	{
		//Any tile that is a fog tile will be rendered here instead of with the tiles
		//so that it can be rendered on top of sprites
		//we assess the fog tiles and then render them
		//m_tileSystem->AssessFog(mSectionRenderList);
		
		m_D3DSystem->TurnOnAlphaBlending();
			m_tileSystem->RenderTopFog(mSectionRenderList, camera);
		m_D3DSystem->TurnOffAlphaBlending();
	}

	//mapSprite->Render(textureShader, d3d, viewMatrix, worldMatrix, orthoMatrix);

	return true;
}
//=================================================================================================================
vector<Section*> TopdownMap::GetSections()
{
	return m_tileSystem->GetSections();
}
//=================================================================================================================