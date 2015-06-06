#include "Tile.h"
#include "FogData.h"
#include "D3D.h"
#include "MaterialManager.h"
//-----------------------------------------------------------------------------------------------------------------
Tile::Tile(D3D* d3d)
:   PhysicsBody(d3d),
	m_D3DSystem(d3d),
	mSize(64),
	mHard(false),
	m_displayHardbox(false),
	m_hardBoxSpr(NULL),
	m_animationProfileName("NONE"),
	m_animationSpeed(0),
	mTexture(0),
	mTextureCopy(0),
	//mPosition(0, 0, 0),
	m_fogBit(fow_all),
	m_CurrentSequence(NULL),
	m_bFOW(false),
	m_iFOWRadius(5),
	m_bFOWFlashlight(false)
{
	m_engineOptions = d3d->GetEngineOptions();

	mWorld = XMMatrixIdentity();
}
//-----------------------------------------------------------------------------------------------------------------
Tile::~Tile()
{
}
//-----------------------------------------------------------------------------------------------------------------
bool Tile::Initialize(int size, bool hard, XMFLOAT3 pos)
{
	PhysicsBody::Initialize(pos, STATIC, size, size);
	
	mSize = size;
	mHard = hard;
	//mPosition = pos;
	m_d3dvers = m_D3DSystem->GetEngineOptions()->m_d3dVersion;
	return true;
}
//-----------------------------------------------------------------------------------------------------------------
string Tile::GetTileTextureName()
{
	if (mTexture != NULL)
		return mTexture->TextureName();
	return "";
}
//-----------------------------------------------------------------------------------------------------------------
string Tile::GetTileTextureCopyName()
{
	if (mTextureCopy != NULL)
		return mTextureCopy->TextureName();
	return "";
}
//-----------------------------------------------------------------------------------------------------------------
/*bool Tile::InitializeTexture(D3D* d3d, int screenWidth, int screenHeight, string textureFilename, int bitmapWidth, int bitmapHeight)
{
	if (m_engineOptions->m_fow)
	{
		//Load default fog cover for the texture

		//Create a bitmap object
		mTexture = new ZShadeSandboxMesh::QuadMesh();

		if (!mTexture)
		{
			return false;
		}

		//Initialize the bitmap
		bool result = mTexture->Initialize(device, screenWidth, screenHeight, "fow\\fow_all.png", bitmapWidth, bitmapHeight);

		if (!result)
		{
			return false;
		}
	}
	else
	{
		//Create a bitmap object
		mTexture = new ZShadeSandboxMesh::QuadMesh();

		if (!mTexture)
		{
			return false;
		}

		//Initialize the bitmap
		bool result = mTexture->Initialize(device, screenWidth, screenHeight, textureFilename, bitmapWidth, bitmapHeight);

		if (!result)
		{
			return false;
		}
	}

	//Create the copy of the texture

	//Create a bitmap object
	mTextureCopy = new ZShadeSandboxMesh::QuadMesh();

	if (!mTexture)
	{
		return false;
	}

	//Initialize the bitmap
	bool result = mTextureCopy->Initialize(device, screenWidth, screenHeight, textureFilename, bitmapWidth, bitmapHeight);

	if (!result)
	{
		return false;
	}
}*/
//-----------------------------------------------------------------------------------------------------------------
bool Tile::InitializeTexture(string windowName, string base_tile_path, string textureFilename, int bitmapWidth, int bitmapHeight)
{
	if (m_bFOW)
	{
		//Load default fog cover for the texture

		ZShadeSandboxMesh::MeshParameters mp;
		//mp.color = XMFLOAT4(0, 0, 0, 0);
		mp.pos = XMFLOAT3(0, 0, 0);
		mp.rot = XMFLOAT3(0, 0, 0);
		mp.scale = XMFLOAT3(1, 1, 1);
		mp.rotationAxisX = false;
		mp.rotationAxisY = false;
		mp.rotationAxisZ = false;
		mp.vertexType = ZShadeSandboxMesh::EVertexType::VT_Tex;
		mp.useCustomShader = false;
		//mp.texturePath = base_tile_path;
		//mp.textureName = "fow\\fow_all.png";
		mp.textureWidth = bitmapWidth;
		mp.textureHeight = bitmapHeight;
		mp.material = MaterialManager::Instance()->GetMaterial("FOW ALL");
		//mp.tt = TextureType::PNG;
		mp.in2D = true;

		//Create a bitmap object
		mTexture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp, true);

		if (!mTexture)
		{
			return false;
		}

		////Initialize the bitmap "fow\\fow_all.png"
		//bool result = mTexture->Initialize(windowName, base_tile_path, "fow\\fow_all.png", bitmapWidth, bitmapHeight);

		//if (!result)
		//{
		//	return false;
		//}
	}
	else
	{
		MaterialManager::Instance()->AddDiffuse(m_D3DSystem, base_tile_path, textureFilename, textureFilename);

		ZShadeSandboxMesh::MeshParameters mp;
		//mp.color = XMFLOAT4(0, 0, 0, 0);
		mp.pos = XMFLOAT3(0, 0, 0);
		mp.rot = XMFLOAT3(0, 0, 0);
		mp.scale = XMFLOAT3(1, 1, 1);
		mp.rotationAxisX = false;
		mp.rotationAxisY = false;
		mp.rotationAxisZ = false;
		mp.vertexType = ZShadeSandboxMesh::EVertexType::VT_Tex;
		mp.useCustomShader = false;
		//mp.texturePath = base_tile_path;
		//mp.textureName = textureFilename;
		mp.textureWidth = bitmapWidth;
		mp.textureHeight = bitmapHeight;
		mp.material = MaterialManager::Instance()->GetMaterial(textureFilename);
		//mp.tt = TextureType::PNG;
		mp.in2D = true;

		//Create a bitmap object
		mTexture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp, true);

		if (!mTexture)
		{
			return false;
		}

		//Initialize the bitmap "fow\\fow_all.png"
		//bool result = mTexture->Initialize(windowName, base_tile_path, textureFilename, bitmapWidth, bitmapHeight);

		//if (!result)
		//{
		//	return false;
		//}
	}

	//Create the copy of the texture

	ZShadeSandboxMesh::MeshParameters mp;
	//mp.color = XMFLOAT4(0, 0, 0, 0);
	mp.pos = XMFLOAT3(0, 0, 0);
	mp.rot = XMFLOAT3(0, 0, 0);
	mp.scale = XMFLOAT3(1, 1, 1);
	mp.rotationAxisX = false;
	mp.rotationAxisY = false;
	mp.rotationAxisZ = false;
	mp.vertexType = ZShadeSandboxMesh::EVertexType::VT_Tex;
	mp.useCustomShader = false;
	//mp.texturePath = base_tile_path;
	//mp.textureName = textureFilename;
	mp.material = MaterialManager::Instance()->GetMaterial(textureFilename);
	mp.textureWidth = bitmapWidth;
	mp.textureHeight = bitmapHeight;
	//mp.tt = TextureType::PNG;
	mp.in2D = true;

	//Create a bitmap object
	mTextureCopy = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp, true);

	if (!mTextureCopy)
	{
		return false;
	}

	////Initialize the bitmap
	//bool result = mTextureCopy->Initialize(windowName, base_tile_path, textureFilename, bitmapWidth, bitmapHeight);

	//if (!result)
	//{
	//	return false;
	//}
	
	return true;
}
//-----------------------------------------------------------------------------------------------------------------
void Tile::Shutdown()
{
	if (mAnimationSequences.size() > 0) mAnimationSequences.clear();

	/*if (mTexture)
	{
		mTexture->Shutdown();
		mTexture = 0;
	}*/
}
//-----------------------------------------------------------------------------------------------------------------
void Tile::CreateHardBoxSpr(string editor_base)
{
	m_hard_box.m_x = m_TopLeftPosition.x;
	m_hard_box.m_y = m_TopLeftPosition.y;
	m_hard_box.width = mSize;
	m_hard_box.height = mSize;

	m_hardBoxSpr = new Sprite(m_D3DSystem);
	m_hardBoxSpr->Initialize(editor_base, "white.png", XMFLOAT3(m_hard_box.m_x, m_hard_box.m_y, 0), m_hard_box.width, m_hard_box.height, EPhysicsType());
}
//-----------------------------------------------------------------------------------------------------------------
void Tile::UpdateAnimation()
{
	if (m_CurrentSequence == NULL) return;

	m_CurrentSequence->Update(m_animationSpeed);
}
//-----------------------------------------------------------------------------------------------------------------
void Tile::AddAnimationSequence(Animation2D* sequence)
{
	if (sequence == NULL) return;

	sequence->LoadFrameTextures();
	mAnimationSequences.push_back(sequence);
}
//-----------------------------------------------------------------------------------------------------------------
void Tile::SetSequence(int i)
{
	m_CurrentSequence = mAnimationSequences[i];
}
//-----------------------------------------------------------------------------------------------------------------
void Tile::UpdatePosition(XMFLOAT3 pos)
{
	m_TopLeftPosition = pos;
}
//-----------------------------------------------------------------------------------------------------------------
void Tile::UpdateFogBit(unsigned short bit)
{
	m_fogBit = bit;
}
//-----------------------------------------------------------------------------------------------------------------
unsigned short Tile::GetFogBit()
{
	return m_fogBit;
}
//-----------------------------------------------------------------------------------------------------------------
Tile* Tile::Clone(GameDirectory2D* gd)
{
	Tile* t = new Tile(m_D3DSystem);

	t->Initialize(mSize, mHard, XMFLOAT3(m_TopLeftPosition.x, m_TopLeftPosition.y, 0));

	string base_path = "";
	base_path = gd->m_tiles_path;

	if (mTexture != 0)
	{
		//if (mTexture->MeshTexture() != 0)
		if (mTexture->GetMaterial() != 0)
		{
			t->InitializeTexture("Render", base_path, mTexture->TextureName(), m_engineOptions->TILE_SIZE, m_engineOptions->TILE_SIZE);
		}
	}

	t->SetAnimationProfileName(m_animationProfileName);
	t->SetAnimationSpeed(m_animationSpeed);

	string path = gd->m_required_textures;
	t->CreateHardBoxSpr(path);
	
	//If there are any animations for the tile set it
	if (m_animationProfileName != "NONE")
	{
		//Load a animation profile
		AnimationProfile2D* profile = new AnimationProfile2D(m_D3DSystem);

		bool something_wrong = false;
		
		if (profile->LoadAnimationProfileNames(m_animationProfileName, "TILE", gd->GAME_FOLDER))
		{
			if (profile->ReadProfile(m_animationProfileName, "TILE", gd->m_tiles_path, gd->GAME_FOLDER))
			{
				vector<Animation2D*> anims = profile->GetAnimations();

				Tile* tile = t;

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

	return t;
}
//-----------------------------------------------------------------------------------------------------------------
void Tile::UpdateTexture()
{
	switch (m_fogBit)
	{
		//Reveal the tile image
		case fow_non:
		{
			mTexture = mTextureCopy;
		}
		break;
		//Completley black tile
		case fow_all:
		{
			mTexture = FogData::fow_all_texture;
		}
		break;
		//Straights
		case fow_NNN:
		{
			mTexture = FogData::fow_NNN_texture;
		}
		break;
		case fow_SSS:
		{
			mTexture = FogData::fow_SSS_texture;
		}
		break;
		case fow_EEE:
		{
			mTexture = FogData::fow_EEE_texture;
		}
		break;
		case fow_WWW:
		{
			mTexture = FogData::fow_WWW_texture;
		}
		break;
		//Corners
		case fow_CNW:
		{
			mTexture = FogData::fow_CNW_texture;
		}
		break;
		case fow_CNE:
		{
			mTexture = FogData::fow_CNE_texture;
		}
		break;
		case fow_CSW:
		{
			mTexture = FogData::fow_CSW_texture;
		}
		break;
		case fow_CSE:
		{
			mTexture = FogData::fow_CSE_texture;
		}
		break;
		//Joints
		case fow_JNW:
		{
			mTexture = FogData::fow_JNW_texture;
		}
		break;
		case fow_JNE:
		{
			mTexture = FogData::fow_JNE_texture;
		}
		break;
		case fow_JSW:
		{
			mTexture = FogData::fow_JSW_texture;
		}
		break;
		case fow_JSE:
		{
			mTexture = FogData::fow_JSE_texture;
		}
		break;
	}
}
//-----------------------------------------------------------------------------------------------------------------
bool Tile::Render(Camera* camera)
{
	/*XMMATRIX scale, rx, ry, rz, translate;

	scale = XMMatrixScaling(1, 1, 1);
	rx = XMMatrixRotationX(0);
	ry = XMMatrixRotationY(0);
	rz = XMMatrixRotationZ(0);
	translate = XMMatrixTranslation(mPosition.x, mPosition.y, 0);

	XMMATRIX world = mWorld;

	world = XMMatrixMultiply(world, scale);
	world = XMMatrixMultiply(world, rx);
	world = XMMatrixMultiply(world, ry);
	world = XMMatrixMultiply(world, rz);
	world = XMMatrixMultiply(world, translate);

	textureShader->UseCustomWorld(true);
	textureShader->SetCustomWorld(mWorld);*/

	if (m_CurrentSequence == NULL)
	{
		if (mTexture == NULL) return false;

		mTexture->UpdateBuffers(m_TopLeftPosition.x, m_TopLeftPosition.y);

		ZShadeSandboxMesh::MeshRenderParameters mrp;
		mrp.pCamera = camera;
		mTexture->Render(mrp);

		//if (m_D3DSystem->GetEngineOptions()->m_d3dVersion == DIRECTX10)
		//	;// result = textureShader->Render10(mTexture->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, mTexture->GetTexture10());
		//else if (m_D3DSystem->GetEngineOptions()->m_d3dVersion == DIRECTX11)
		//	result = textureShader->Render11(mTexture->GetIndexCount(), XMMath4(0,0,0,0), camera, mTexture->GetTexture11());

		//if (!result)
		//{
		//	return false;
		//}
	}
	else
	{
		//Render Tile Animation2D
		RenderAnimation(camera);
	}

	if (m_hardBoxSpr != NULL)
	{
		if (m_displayHardbox && mHard) m_hardBoxSpr->Render(camera);
	}

	return true;
}
//-----------------------------------------------------------------------------------------------------------------
bool Tile::Render(Camera* camera, float blendAmount)
{
	/*XMMATRIX scale, rx, ry, rz, translate;

	scale = XMMatrixScaling(1, 1, 1);
	rx = XMMatrixRotationX(0);
	ry = XMMatrixRotationY(0);
	rz = XMMatrixRotationZ(0);
	translate = XMMatrixTranslation(mPosition.x, mPosition.y, 0);

	XMMATRIX world = mWorld;

	world = XMMatrixMultiply(world, scale);
	world = XMMatrixMultiply(world, rx);
	world = XMMatrixMultiply(world, ry);
	world = XMMatrixMultiply(world, rz);
	world = XMMatrixMultiply(world, translate);

	textureShader->UseCustomWorld(true);
	textureShader->SetCustomWorld(mWorld);

	transparentShader->UseCustomWorld(true);
	transparentShader->SetCustomWorld(mWorld);*/

	//Render the regular texture below a fog texture that needs transparency
	if (m_fogBit != fow_non && m_fogBit != fow_all)
	{
		bool result = true;
		if (mTextureCopy == NULL) return false;

		//result = mTextureCopy->Render(m_TopLeftPosition.x, m_TopLeftPosition.y);

		//if (!result)
		//{
		//	return false;
		//}

		//if (m_D3DSystem->GetEngineOptions()->m_d3dVersion == DIRECTX10)
		//	;// result = textureShader->Render10(mTextureCopy->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, mTextureCopy->GetTexture10());
		//else if (m_D3DSystem->GetEngineOptions()->m_d3dVersion == DIRECTX11)
		//	result = textureShader->Render11(mTextureCopy->GetIndexCount(), XMMath4(0, 0, 0, 0), camera, mTextureCopy->GetTexture11());

		//if (!result)
		//{
		//	return false;
		//}
		//
		////Render the fog if transparency is needed
		//if (mTexture == NULL) return false;

		//result = mTexture->Render(m_TopLeftPosition.x, m_TopLeftPosition.y);

		//if (!result)
		//{
		//	return false;
		//}

		//if (m_D3DSystem->GetEngineOptions()->m_d3dVersion == DIRECTX10)
		//	;// result = transparentShader->Render10(mTexture->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, mTexture->GetTexture10(), 0.9f);
		//else if (m_D3DSystem->GetEngineOptions()->m_d3dVersion == DIRECTX11)
		//	//result = textureShader->Render11(mTexture->GetIndexCount(), XMMath4(0, 0, 0, 0), camera, mTexture->GetTexture11());
		//	result = transparentShader->Render11(mTexture->GetIndexCount(), XMMath4(0, 0, 0, 0), camera, mTexture->GetTexture11(), 0.0f);

		//if (!result)
		//{
		//	return false;
		//}

		mTextureCopy->UpdateBuffers(m_TopLeftPosition.x, m_TopLeftPosition.y);
		
		ZShadeSandboxMesh::MeshRenderParameters mrp;
		mrp.pCamera = camera;
		//mrp.bTransparent = true;
		//mrp.blendAmount = blendAmount;
		mTextureCopy->Render(mrp);

		if (m_hardBoxSpr != NULL)
		{
			if (m_displayHardbox && mHard) m_hardBoxSpr->Render(camera, blendAmount);
		}
	}
	else//Regular rendering
	{
		Render(camera);
	}

	return true;
}
//-----------------------------------------------------------------------------------------------------------------
bool Tile::RenderAnimation(Camera* camera)
{
	/*XMMATRIX scale, rx, ry, rz, translate;

	scale = XMMatrixScaling(1, 1, 1);
	rx = XMMatrixRotationX(0);
	ry = XMMatrixRotationY(0);
	rz = XMMatrixRotationZ(0);
	translate = XMMatrixTranslation(mPosition.x, mPosition.y, 0);

	XMMATRIX world = mWorld;

	world = XMMatrixMultiply(world, scale);
	world = XMMatrixMultiply(world, rx);
	world = XMMatrixMultiply(world, ry);
	world = XMMatrixMultiply(world, rz);
	world = XMMatrixMultiply(world, translate);

	textureShader->UseCustomWorld(true);
	textureShader->SetCustomWorld(mWorld);*/

	/*if (m_engineOptions->m_d3dVersion == DIRECTX10)
	{
		if (m_CurrentSequence == NULL) return false;
		if (m_CurrentSequence->GetCurrentFrame() == NULL) return false;
		if (m_CurrentSequence->GetCurrentFrame()->GetFrameTexture() == NULL) return false;

		//if (!m_CurrentSequence->GetCurrentFrame()->GetFrameTexture()->GetTexture10())
		//	return false;

		//bool result = m_CurrentSequence->GetCurrentFrame()->GetFrameTexture()->Render(m_TopLeftPosition.x, m_TopLeftPosition.y);

		//if (!result)
		//{
		//	return false;
		//}

		////result = textureShader->Render10(m_CurrentSequence->GetCurrentFrame()->GetFrameTexture()->GetIndexCount(),
		////	XMMath4(0,0,0,0), camera, m_CurrentSequence->GetCurrentFrame()->GetFrameTexture()->GetTexture10());

		//if (!result)
		//{
		//	return false;
		//}
	}
	else if (m_engineOptions->m_d3dVersion == DIRECTX11)
	{
		AnimationFrame2D* frame = m_CurrentSequence->GetCurrentFrame();
		if (frame == NULL) return false;
		ZShadeSandboxMesh::QuadMesh* texture = frame->GetFrameTexture();
		/*if (texture != NULL)
		{
			if (!texture->GetTexture11())
				return false;
		}
		else
			return false;

		bool result = m_CurrentSequence->GetCurrentFrame()->GetFrameTexture()->Render(m_TopLeftPosition.x, m_TopLeftPosition.y);

		if (!result)
		{
			return false;
		}

		result = textureShader->Render11(m_CurrentSequence->GetCurrentFrame()->GetFrameTexture()->GetIndexCount(),
			XMMath4(0,0,0,0), camera, m_CurrentSequence->GetCurrentFrame()->GetFrameTexture()->GetTexture11());

		if (!result)
		{
			return false;
		}*
	}*/
	
	AnimationFrame2D* frame = m_CurrentSequence->GetCurrentFrame();
	if (frame == NULL) return false;
	ZShadeSandboxMesh::QuadMesh* texture = frame->GetFrameTexture();
	if (texture == NULL) return false;
	
	texture->UpdateBuffers(m_TopLeftPosition.x, m_TopLeftPosition.y);
	
	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.pCamera = camera;
	texture->Render(mrp);
	
	return true;
}
//-----------------------------------------------------------------------------------------------------------------
bool Tile::RenderAnimation(Camera* camera, float blendAmount)
{
	/*XMMATRIX scale, rx, ry, rz, translate;

	scale = XMMatrixScaling(1, 1, 1);
	rx = XMMatrixRotationX(0);
	ry = XMMatrixRotationY(0);
	rz = XMMatrixRotationZ(0);
	translate = XMMatrixTranslation(mPosition.x, mPosition.y, 0);

	XMMATRIX world = mWorld;

	world = XMMatrixMultiply(world, scale);
	world = XMMatrixMultiply(world, rx);
	world = XMMatrixMultiply(world, ry);
	world = XMMatrixMultiply(world, rz);
	world = XMMatrixMultiply(world, translate);

	transparentShader->UseCustomWorld(true);
	transparentShader->SetCustomWorld(mWorld);*/

	/*if (m_engineOptions->m_d3dVersion == DIRECTX10)
	{
		AnimationFrame2D* frame = m_CurrentSequence->GetCurrentFrame();
		if (frame == NULL) return false;
		ZShadeSandboxMesh::QuadMesh* texture = frame->GetFrameTexture();
		//if (texture != NULL)
		//{
		//	if (!texture->GetTexture10())
		//		return false;
		//}
		//else
		//	return false;

		//bool result = m_CurrentSequence->GetCurrentFrame()->GetFrameTexture()->Render(m_TopLeftPosition.x, m_TopLeftPosition.y);

		//if (!result)
		//{
		//	return false;
		//}

		////result = transparentShader->Render10(m_CurrentSequence->GetCurrentFrame()->GetFrameTexture()->GetIndexCount(),
		////	camera, m_CurrentSequence->GetCurrentFrame()->GetFrameTexture()->GetTexture10(), blendAmount);

		//if (!result)
		//{
		//	return false;
		//}
	}
	else if (m_engineOptions->m_d3dVersion == DIRECTX11)
	{
		AnimationFrame2D* frame = m_CurrentSequence->GetCurrentFrame();
		if (frame == NULL) return false;
		ZShadeSandboxMesh::QuadMesh* texture = frame->GetFrameTexture();

		if (texture != NULL)
		{
			if (texture->MeshTexture() == NULL)
				return false;
		}
		else return false;

		texture->Update(m_TopLeftPosition.x, m_TopLeftPosition.y);

		ZShadeSandboxMesh::MeshRenderParameters mrp;
		mrp.pCamera = camera;
		texture->Render(mrp);

		/*if (texture != NULL)
		{
			if (!texture->GetTexture11())
				return false;
		}
		else
			return false;

		bool result = m_CurrentSequence->GetCurrentFrame()->GetFrameTexture()->Render(m_TopLeftPosition.x, m_TopLeftPosition.y);

		if (!result)
		{
			return false;
		}

		result = transparentShader->Render11(m_CurrentSequence->GetCurrentFrame()->GetFrameTexture()->GetIndexCount(),
			XMMath4(0, 0, 0, 0), camera, m_CurrentSequence->GetCurrentFrame()->GetFrameTexture()->GetTexture11(), blendAmount);

		if (!result)
		{
			return false;
		}*
	}*/
	
	AnimationFrame2D* frame = m_CurrentSequence->GetCurrentFrame();
	if (frame == NULL) return false;
	ZShadeSandboxMesh::QuadMesh* texture = frame->GetFrameTexture();
	if (texture == NULL) return false;
	
	texture->UpdateBuffers(m_TopLeftPosition.x, m_TopLeftPosition.y);
	
	// Use the transparent shader to render the mesh
	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.pCamera = camera;
	//mrp.bTransparent = true;
	//mrp.blendAmount = blendAmount;
	texture->Render(mrp);
	
	if (m_hardBoxSpr != NULL)
	{
		if (m_displayHardbox && mHard) m_hardBoxSpr->Render(camera, blendAmount);
	}

	return true;
}
//-----------------------------------------------------------------------------------------------------------------
RECT Tile::GetBox()
{
	RECT rect;

	rect.top = m_TopLeftPosition.x;
	rect.left = m_TopLeftPosition.y;
	rect.right = rect.left + 64;
	rect.bottom = rect.top + 64;

	return rect;
}
//-----------------------------------------------------------------------------------------------------------------
XMFLOAT3 Tile::GetPosition()
{
	return m_TopLeftPosition;
}
//-----------------------------------------------------------------------------------------------------------------