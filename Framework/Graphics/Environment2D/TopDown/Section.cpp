#include "Section.h"
#include "FogData.h"
//-----------------------------------------------------------------------------------------------------------------
Section::Section(D3D* d3d) :
	m_D3DSystem(d3d),
	m_vision(0),
	m_musicName("Name.mid"),
	mAdjacencyInfo(NULL),
	m_bFOW(false),
	m_iFOWRadius(5),
	m_bFOWFlashlight(false)
{
	m_engineOptions = d3d->GetEngineOptions();
}
//-----------------------------------------------------------------------------------------------------------------
Section::Section(const Section& other)
{
}
//-----------------------------------------------------------------------------------------------------------------
Section::~Section()
{
}
//-----------------------------------------------------------------------------------------------------------------
bool Section::Initialize(int id)
{
	/*m_Tiles = new Tile**[m_engineOptions->ROW_SIZE];

	for (int i = 0; i < m_engineOptions->ROW_SIZE; i++)
	{
		m_Tiles[i] = new Tile*[m_engineOptions->COL_SIZE];
	}
	*/

	mAdjacencyInfo = NULL;

	m_Tiles.resize(m_engineOptions->ROW_SIZE, m_engineOptions->COL_SIZE);
	
	ID = id;

	//NB = -1;
	//SB = -1;
	//EB = -1;
	//WB = -1;

	return true;
}
//-----------------------------------------------------------------------------------------------------------------
bool Section::CreateTile(int x, int y, int size, bool hard,
		XMFLOAT3 pos, GameDirectory2D* gd, string textureFilename, bool init,
		string animProfileName, int animSpeed, int bitmapWidth, int bitmapHeight)
{
	//m_Tiles(x, y) = new Tile(m_D3DSystem);
	//m_Tiles(x, y)->Initialize(size, hard, pos);
	m_Tiles.insert(x, y, new Tile(m_D3DSystem));
	m_Tiles(x, y)->Initialize(size, hard, pos);

	string base_path = "";
	
	if (init)
	{
		base_path = gd->m_required_textures;
	}
	else
	{
		base_path = gd->m_tiles_path;
	}
	
	m_Tiles(x, y)->InitializeTexture("Render", base_path, textureFilename, bitmapWidth, bitmapHeight);
	m_Tiles(x, y)->SetAnimationProfileName(animProfileName);
	m_Tiles(x, y)->SetAnimationSpeed(animSpeed);
	
	string path = gd->m_required_textures;
	//path.append("\\");
	m_Tiles(x, y)->CreateHardBoxSpr(path);
	//If there are any animations for the tile set it
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

				Tile* tile = m_Tiles(x, y);

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
	return true;
}
//-----------------------------------------------------------------------------------------------------------------
void Section::MoveTiles(float speed, int dirUD, int dirLR)
{
	Tile* tile;
	for (int y = 0; y < m_engineOptions->COL_SIZE; y++)
	{
		for (int x = 0; x < m_engineOptions->ROW_SIZE; x++)
		{
			tile = GetTile(x, y);
			
			float newX = tile->GetPosition().x;
			float newY = tile->GetPosition().y;

			// Move the tile up
			if (dirUD == 0)
			{
				newY += speed;
			}

			// Move the tile down
			if (dirUD == 1)
			{
				newY -= speed;
			}

			// Move the tile left
			if (dirLR == 2)
			{
				newX += speed;
			}

			// Move the tile right
			if (dirLR == 3)
			{
				newX -= speed;
			}

			XMFLOAT3 pos(newX, newY, 0);

			tile->UpdatePosition(pos);
		}
	}
}
//-----------------------------------------------------------------------------------------------------------------
void Section::UpdateBounds()
{
	int startboundx = m_Tiles(0, 0)->GetPosition().x;
	int startboundy = m_Tiles(0, 0)->GetPosition().y;

	int endboundx = m_Tiles(m_engineOptions->ROW_SIZE-1, m_engineOptions->COL_SIZE-1)->GetPosition().x;
	int endboundy = m_Tiles(m_engineOptions->ROW_SIZE-1, m_engineOptions->COL_SIZE-1)->GetPosition().y;

	m_BoundingBox.firstTileX = startboundx;
	m_BoundingBox.firstTileY = startboundy;
	m_BoundingBox.lastTileX = endboundx;
	m_BoundingBox.lastTileY = endboundy;
}
//-----------------------------------------------------------------------------------------------------------------
void Section::SetTileDisplayHardbox(bool visible)
{
	Tile* tile;
	for (int y = 0; y < m_engineOptions->COL_SIZE; y++)
	{
		for (int x = 0; x < m_engineOptions->ROW_SIZE; x++)
		{
			tile = GetTile(x, y);
			tile->SetDisplayHardbox(visible);
		}
	}
}
//-----------------------------------------------------------------------------------------------------------------
void Section::ReloadSprites(string base_tile_path)
{
	for (int y = 0; y < m_engineOptions->COL_SIZE; y++)
	{
		for (int x = 0; x < m_engineOptions->ROW_SIZE; x++)
		{
			Tile* t = m_Tiles(x, y);
			XMFLOAT3 pos = t->GetPosition();
			bool hard = t->IsHard();
			int size = t->GetSize();
			string textureFilename = t->GetTileTextureName();
			int bitmapWidth = t->GetWidth();
			int bitmapHeight = t->GetHeight();

			m_Tiles(x, y)->Initialize(size, hard, pos);
			m_Tiles(x, y)->InitializeTexture("Render", base_tile_path, textureFilename, bitmapWidth, bitmapHeight);
		}
	}
}
//-----------------------------------------------------------------------------------------------------------------
Tile* Section::SelectTile(float x, float y)
{
	for (int i = 0; i < m_engineOptions->COL_SIZE; i++)
	{
		for (int j = 0; j < m_engineOptions->ROW_SIZE; j++)
		{
			Tile* tile = GetTile(j, i);

			float xPos = tile->GetPosition().x;
			float yPos = tile->GetPosition().y;
			int width = tile->GetWidth();
			int height = tile->GetHeight();

			if (x == xPos && y == yPos)
			{
				return tile;
			}

			/*if (x >= xPos && x <= xPos + width)
			{
				if (y >= yPos && y <= yPos + height)
				{
					return tile;
				}
			}*/
		}
	}

	return NULL;
}
//-----------------------------------------------------------------------------------------------------------------
bool Section::InBounds(int x, int y)
{
	if (x >= m_BoundingBox.firstTileX && x <= m_BoundingBox.lastTileX)
	{
		if (y >= m_BoundingBox.firstTileY && y <= m_BoundingBox.lastTileY)
		{
			return true;
		}
	}

	return false;
}
//-----------------------------------------------------------------------------------------------------------------
void Section::Shutdown()
{
	for (int y = 0; y < m_engineOptions->COL_SIZE; y++)
	{
		for (int x = 0; x < m_engineOptions->ROW_SIZE; x++)
		{
			m_Tiles(x, y)->Shutdown();
		}
	}
}
//-----------------------------------------------------------------------------------------------------------------
/*void Section::ReTexBounds(int bitmapWidth, int bitmapHeight)
{
	for (int y = 0; y < m_engineOptions->COL_SIZE; y++)
	{
		for (int x = 0; x < m_engineOptions->ROW_SIZE; x++)
		{
			int yPos = GetTilePos(x, y).y;
			int xPos = GetTilePos(x, y).x;

			//North
			if (yPos == NB)
				m_Tiles(x, y)->InitializeTexture("Render", "Textures", "Background\\MossyRock00.png", bitmapWidth, bitmapHeight);
			//South
			if (yPos == SB)
				m_Tiles(x, y)->InitializeTexture("Render", "Textures", "Background\\MossyRock01.png", bitmapWidth, bitmapHeight);
			//East
			if (xPos == EB)
				m_Tiles(x, y)->InitializeTexture("Render", "Textures", "Background\\Dirt00.png", bitmapWidth, bitmapHeight);
			//West
			if (xPos == WB)
				m_Tiles(x, y)->InitializeTexture("Render", "Textures", "Background\\Dirt01.png", bitmapWidth, bitmapHeight);
		}
	}
}*/
//-----------------------------------------------------------------------------------------------------------------
void Section::ReSet(int bitmapWidth, int bitmapHeight)
{
	for (int y = 0; y < m_engineOptions->COL_SIZE; y++)
	{
		for (int x = 0; x < m_engineOptions->ROW_SIZE; x++)
		{
			m_Tiles(x, y)->InitializeTexture("Render", "Textures", "Background\\PatchyGrass04.png", bitmapWidth, bitmapHeight);
		}
	}
}
//-----------------------------------------------------------------------------------------------------------------
void Section::SetNewTexture(string base_tile_path, string textureFilename, int bitmapWidth, int bitmapHeight)
{
	for (int y = 0; y < m_engineOptions->COL_SIZE; y++)
	{
		for (int x = 0; x < m_engineOptions->ROW_SIZE; x++)
		{
			m_Tiles(x, y)->InitializeTexture("Render", base_tile_path, textureFilename, bitmapWidth, bitmapHeight);
		}
	}
}
//-----------------------------------------------------------------------------------------------------------------
XMFLOAT3 Section::GetTilePos(int x, int y)
{
	return m_Tiles(x, y)->GetPosition();
}
//-----------------------------------------------------------------------------------------------------------------
void Section::UpdatePosition(int x, int y, XMFLOAT3 pos)
{
	m_Tiles(x, y)->UpdatePosition(pos);
}
//-----------------------------------------------------------------------------------------------------------------
/*vector<Section::NeighborIDS> Section::GetNeighbors()
{
	return neighborids;
}
//-----------------------------------------------------------------------------------------------------------------
void Section::AddNeighborID(NeighborIDS id)
{
	neighborids.push_back(id);
}*/
//-----------------------------------------------------------------------------------------------------------------
/*vector<int> Section::GetRenderSections()
{
	return mRenderSections;
}
//-----------------------------------------------------------------------------------------------------------------
void Section::AddRenderSection(int id)
{
	mRenderSections.push_back( id );
}*/
//-----------------------------------------------------------------------------------------------------------------
vector<Tile*> Section::GetTiles()
{
	vector<Tile*> tiles;

	for (int y = 0; y < m_engineOptions->COL_SIZE; y++)
	{
		for (int x = 0; x < m_engineOptions->ROW_SIZE; x++)
		{
			tiles.push_back(m_Tiles(x, y));
		}
	}

	return tiles;
}
//-----------------------------------------------------------------------------------------------------------------
void Section::UpdateTileTexture(int x, int y, string base_tile_path, string textureFilename, int bitmapWidth, int bitmapHeight)
{
	m_Tiles(x, y)->InitializeTexture("Render", base_tile_path, textureFilename, bitmapWidth, bitmapHeight);
}
//-----------------------------------------------------------------------------------------------------------------
void Section::UpdateTexture()
{
	for (int y = 0; y < m_engineOptions->COL_SIZE; y++)
	{
		for (int x = 0; x < m_engineOptions->ROW_SIZE; x++)
		{
			m_Tiles(x, y)->UpdateTexture();
		}
	}
}
//-----------------------------------------------------------------------------------------------------------------
void Section::UpdateFogBit(unsigned short bit)
{
	for (int y = 0; y < m_engineOptions->COL_SIZE; y++)
	{
		for (int x = 0; x < m_engineOptions->ROW_SIZE; x++)
		{
			m_Tiles(x, y)->UpdateFogBit(bit);
		}
	}
}
//-----------------------------------------------------------------------------------------------------------------
bool Section::Render(Camera* camera)
{
	for (int y = 0; y < m_engineOptions->COL_SIZE; y++)
	{
		for (int x = 0; x < m_engineOptions->ROW_SIZE; x++)
		{
			//Update tile animations if they exist
			m_Tiles(x, y)->UpdateAnimation();

			if (m_bFOW)
			{
				if (m_Tiles(x, y)->GetFogBit() != fow_all)
					m_Tiles(x, y)->Render(camera, 0);
			}
			else
			{
				m_Tiles(x, y)->Render(camera);
			}
		}
	}

	//if (ID == 0)
	{
		int startboundx = m_Tiles(0, 0)->GetPosition().x;
		int startboundy = m_Tiles(0, 0)->GetPosition().y;

		int endboundx = m_Tiles(m_engineOptions->ROW_SIZE-1, m_engineOptions->COL_SIZE-1)->GetPosition().x;
		int endboundy = m_Tiles(m_engineOptions->ROW_SIZE-1, m_engineOptions->COL_SIZE-1)->GetPosition().y;

		m_BoundingBox.firstTileX = startboundx;
		m_BoundingBox.firstTileY = startboundy;
		m_BoundingBox.lastTileX = endboundx;
		m_BoundingBox.lastTileY = endboundy;
	}
	
	return true;
}
//-----------------------------------------------------------------------------------------------------------------
Section* Section::Clone()
{
	std::unique_ptr<Section> sec;
	sec.reset(new Section(m_D3DSystem));

	sec->Initialize(ID);

	for (int x = 0; x < m_Tiles.width(); x++)
	{
		for (int y = 0; y < m_Tiles.height(); y++)
		{
			Tile* tile = m_Tiles(x, y);

			sec->CreateTile(x, y, tile->GetSize(), tile->IsHard(), tile->GetPosition(), m_engineOptions->m_GD2D, tile->GetTileTextureName(), true,
				tile->GetAnimationProfileName(), tile->GetAnimationSpeed(), tile->GetWidth(), tile->GetHeight());
		}
	}

	sec->SetAdjacencyInfo(AdjacencyInfo()->Clone());

	sec->SetVision(m_vision);
	sec->SetMusicName(m_musicName);

	sec->SetBounds(m_BoundingBox);

	return sec.get();
}
//-----------------------------------------------------------------------------------------------------------------