#include "PlatformerTile.h"
#include "Scalar.h"
#include "MaterialManager.h"
//-----------------------------------------------------------------------------------------------------------------
PlatformerTile::PlatformerTile(D3D* d3d)
:	PhysicsBody(d3d),
	m_D3DSystem(d3d),
	mSize(64),
	mHard(false),
	m_displayHardbox(false),
	m_hardBoxSpr(NULL),
	m_animationProfileName("NONE"),
	m_animationSpeed(0),
	mTexture(0),
	//mPosition(0, 0, 0),
	m_CurrentSequence(NULL),
	fRotationAngle(5.0f),
	bRotate(false)
{
	m_EngineOptions = d3d->GetEngineOptions();

	mRotation.x = 0.0f;
	mRotation.y = 0.0f;
	mRotation.z = 0.0f;
	
	mWorld = XMMatrixIdentity();
}
//-----------------------------------------------------------------------------------------------------------------
PlatformerTile::~PlatformerTile()
{
}
//-----------------------------------------------------------------------------------------------------------------
void PlatformerTile::Initialize(int size, bool hard, XMFLOAT3 pos)
{
	PhysicsBody::Initialize(pos, STATIC, size, size);
	
	mSize = size;
	mHard = hard;
	//mPosition = pos;
	
	//m_TileCenterOffset = XMFLOAT3(size / 2, size / 2, 0);
	//m_TileHalfExtents = XMFLOAT3(size / 2, size / 2, 0);
}
//-----------------------------------------------------------------------------------------------------------------
string PlatformerTile::GetTileTextureName()
{
	if (mTexture != NULL)
	{
		return mTexture->TextureName();
	}

	return "";
}
//-----------------------------------------------------------------------------------------------------------------
bool PlatformerTile::InitializeTexture(string windowName, string base_tile_path, string textureFilename, int bitmapWidth, int bitmapHeight)
{
	//Create a bitmap object

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
	//mp.tt = TextureType::PNG;
	mp.material = MaterialManager::Instance()->GetMaterial(textureFilename);
	mp.in2D = true;

	mTexture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp, true);

	if (!mTexture)
	{
		return false;
	}

	/*bool result = mTexture->Initialize(windowName, base_tile_path, textureFilename, bitmapWidth, bitmapHeight);

	if (!result)
	{
		return false;
	}*/
	
	return true;
}
//-----------------------------------------------------------------------------------------------------------------
void PlatformerTile::Shutdown()
{
	/*if (mTexture != 0)
	{
		mTexture->Shutdown();
		mTexture = 0;
	}*/
}
//-----------------------------------------------------------------------------------------------------------------
void PlatformerTile::CreateHardBoxSpr(string editor_base)
{
	m_hard_box.m_x = m_TopLeftPosition.x;
	m_hard_box.m_y = m_TopLeftPosition.y;
	m_hard_box.width = mSize;
	m_hard_box.height = mSize;

	m_hardBoxSpr = new Sprite(m_D3DSystem);
	m_hardBoxSpr->Initialize(editor_base, "white.png", XMFLOAT3(m_hard_box.m_x, m_hard_box.m_y, 0), m_hard_box.width, m_hard_box.height, EPhysicsType());
}
//-----------------------------------------------------------------------------------------------------------------
void PlatformerTile::DisableTexture()
{
	if (mTexture != NULL)
	{
		delete mTexture;
		mTexture = NULL;
	}
}
//-----------------------------------------------------------------------------------------------------------------
void PlatformerTile::UpdateAnimation()
{
	if (m_CurrentSequence == NULL) return;

	m_CurrentSequence->Update(m_animationSpeed);
}
//-----------------------------------------------------------------------------------------------------------------
void PlatformerTile::AddAnimationSequence(Animation2D* sequence)
{
	if (sequence == NULL) return;

	sequence->LoadFrameTextures();
	mAnimationSequences.push_back(sequence);
}
//-----------------------------------------------------------------------------------------------------------------
void PlatformerTile::SetSequence(int i)
{
	m_CurrentSequence = mAnimationSequences[i];
}
//-----------------------------------------------------------------------------------------------------------------
void PlatformerTile::UpdatePosition(XMFLOAT3 pos)
{
	m_TopLeftPosition = pos;
}
//-----------------------------------------------------------------------------------------------------------------
PlatformerTile* PlatformerTile::Clone(GameDirectory2D* gd)
{
	PlatformerTile* t = new PlatformerTile(m_D3DSystem);

	t->Initialize(mSize, mHard, XMFLOAT3(m_TopLeftPosition.x, m_TopLeftPosition.y, 0));

	string base_path = "";
	base_path = gd->m_tiles_path;

	if (mTexture != 0)
	{
		t->InitializeTexture("Render", base_path, mTexture->TextureName(),
			m_EngineOptions->TILE_SIZE, m_EngineOptions->TILE_SIZE);
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

				PlatformerTile* tile = t;

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
void PlatformerTile::RotateTile()
{
	bRotate = true;
}
//-----------------------------------------------------------------------------------------------------------------
XMMATRIX CalculateWorldMatrix(XMFLOAT2 position, XMFLOAT2 size, XMFLOAT2 scale, float rotation)
{
	//XMVECTOR origin = XMLoadFloat2(&XMFLOAT2(position.x, position.y));
	XMVECTOR pos = XMLoadFloat2(&position);
	XMVECTOR scaling = XMLoadFloat2(&scale);

	XMMATRIX finalMatrix = XMMatrixTransformation2D(pos, 0, scaling, pos, rotation, pos);
	finalMatrix = XMMatrixTranspose(finalMatrix);

	return finalMatrix;
}
//-----------------------------------------------------------------------------------------------------------------
bool PlatformerTile::Render(Camera* camera)
{
	/*if (bRotate)
	{
		mRotation.z += 360.0f;

		XMMATRIX scale, rx, ry, rz;

		scale = XMMatrixScaling(1, 1, 1);
		//rx = XMMatrixRotationX(mRotation.x);
		//ry = XMMatrixRotationY(mRotation.y);
		//rz = XMMatrixRotationZ(mRotation.z);
		
		XMVECTOR rotaxis = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		rz = XMMatrixRotationAxis(rotaxis, mRotation.z);

		//float tileX = PlatformerMap::PixelsToTiles((mPosition.x + m_hard_box.width) / 2);
		//float tileY = PlatformerMap::PixelsToTiles((mPosition.y + m_hard_box.height) / 2);

		float middleX = mPosition.x + 32;
		float middleY = mPosition.y + 32;

		XMMATRIX translate = XMMatrixTranslation(middleX, middleY, 0);

		mWorld = XMMatrixIdentity() * scale * rz * translate;

		//mWorld = CalculateWorldMatrix(XMFLOAT2(mPosition.x, mPosition.y),
		//	XMFLOAT2(1, 1), XMFLOAT2(1, 1), mRotation.z);
	}
	else
	{
		mWorld = XMMatrixIdentity();
	}*/

	bRotate = false;

	if (m_CurrentSequence == NULL)
	{
		if (mTexture == NULL) return false;
		
		mTexture->UpdateBuffers(m_TopLeftPosition.x, m_TopLeftPosition.y);
		
		ZShadeSandboxMesh::MeshRenderParameters mrp;
		mrp.camera = camera;
		mTexture->Render(mrp);
		
		/*bool result = mTexture->Render(m_TopLeftPosition.x, m_TopLeftPosition.y);

		if (!result)
		{
			return false;
		}

		if (m_D3DSystem->GetEngineOptions()->m_d3dVersion == DIRECTX10)
			;// result = textureShader->Render10(mTexture->GetIndexCount(), worldMatrix, viewMatrix, orthoMatrix, mTexture->GetTexture10());
		else if (m_D3DSystem->GetEngineOptions()->m_d3dVersion == DIRECTX11)
		{
			//textureShader->UseCustomWorld(true);
			//textureShader->SetCustomWorld(mWorld);
			result = textureShader->Render11(mTexture->GetIndexCount(), XMMath4(0, 0, 0, 0), camera, mTexture->GetTexture11());
		}

		if (!result)
		{
			return false;
		}*/
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
bool PlatformerTile::Render(Camera* camera, float blendAmount)
{
	if (m_CurrentSequence == NULL)
	{
		if (mTexture == NULL) return false;
		
		mTexture->UpdateBuffers(m_TopLeftPosition.x, m_TopLeftPosition.y);
		
		// Use the transparent shader to render the mesh
		ZShadeSandboxMesh::MeshRenderParameters mrp;
		mrp.camera = camera;
		//mrp.bTransparent = true;
		//mrp.blendAmount = blendAmount;
		mTexture->Render(mrp);
	}
	else
	{
		//Render Tile Animation2D
		RenderAnimation(camera, blendAmount);
	}

	if (m_hardBoxSpr != NULL)
	{
		if (m_displayHardbox && mHard) m_hardBoxSpr->Render(camera, blendAmount);
	}
	
	return true;
}
//-----------------------------------------------------------------------------------------------------------------
bool PlatformerTile::RenderAnimation(Camera* camera)
{
	/*if (m_EngineOptions->m_d3dVersion == DIRECTX10)
	{
		if (m_CurrentSequence == NULL) return false;
		if (m_CurrentSequence->GetCurrentFrame() == NULL) return false;
		if (m_CurrentSequence->GetCurrentFrame()->GetFrameTexture() == NULL) return false;
		
		ZShadeSandboxMesh::QuadMesh* texture = m_CurrentSequence->GetCurrentFrame()->GetFrameTexture();
		
		if (texture == NULL) return false;
		
		texture->Update(m_TopLeftPosition.x, m_TopLeftPosition.y);
		
		// Use the texture shader to render the mesh
		ZShadeSandboxMesh::MeshParameters mp = texture->MeshParams();
		mp.vertexType = ZShadeSandboxMesh::EVertexType::VT_Tex;
		texture->MeshParams() = mp;
		
		ZShadeSandboxMesh::MeshRenderParameters mrp;
		mrp.pCamera = camera;
		texture->Render(mrp);
		
		/*if (!m_CurrentSequence->GetCurrentFrame()->GetFrameTexture()->GetTexture10())
			return false;

		bool result = m_CurrentSequence->GetCurrentFrame()->GetFrameTexture()->Render(m_TopLeftPosition.x, m_TopLeftPosition.y);

		if (!result)
		{
			return false;
		}

		//result = textureShader->Render10(m_CurrentSequence->GetCurrentFrame()->GetFrameTexture()->GetIndexCount(),
		//	XMMath4(0,0,0,0), camera, m_CurrentSequence->GetCurrentFrame()->GetFrameTexture()->GetTexture10());

		if (!result)
		{
			return false;
		}*
	}
	else if (m_EngineOptions->m_d3dVersion == DIRECTX11)
	{
		AnimationFrame2D* frame = m_CurrentSequence->GetCurrentFrame();
		if (frame == NULL) return false;
		ZShadeSandboxMesh::QuadMesh* texture = frame->GetFrameTexture();
		
		if (texture == NULL) return false;
		
		texture->Update(m_TopLeftPosition.x, m_TopLeftPosition.y);
		
		// Use the texture shader to render the mesh
		ZShadeSandboxMesh::MeshParameters mp = texture->MeshParams();
		mp.vertexType = ZShadeSandboxMesh::EVertexType::VT_Tex;
		texture->MeshParams() = mp;
		
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

		textureShader->UseCustomWorld(true);
		textureShader->SetCustomWorld(mWorld);

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
	mrp.camera = camera;
	texture->Render(mrp);
	
	return true;
}
//-----------------------------------------------------------------------------------------------------------------
bool PlatformerTile::RenderAnimation(Camera* camera, float blendAmount)
{
	/*if (m_EngineOptions->m_d3dVersion == DIRECTX10)
	{
		AnimationFrame2D* frame = m_CurrentSequence->GetCurrentFrame();
		if (frame == NULL) return false;
		ZShadeSandboxMesh::QuadMesh* texture = frame->GetFrameTexture();
		if (texture == NULL) return false;
		
		texture->Update(m_TopLeftPosition.x, m_TopLeftPosition.y);
		
		// Use the transparent shader to render the mesh
		ZShadeSandboxMesh::MeshParameters mp = texture->MeshParams();
		mp.vertexType = ZShadeSandboxMesh::EVertexType::VT_Trans;
		texture->MeshParams() = mp;
		
		ZShadeSandboxMesh::MeshRenderParameters mrp;
		mrp.pCamera = camera;
		texture->Render(mrp);
		
		/*if (texture != NULL)
		{
			if (!texture->GetTexture10())
				return false;
		}
		else
			return false;

		bool result = m_CurrentSequence->GetCurrentFrame()->GetFrameTexture()->Render(m_TopLeftPosition.x, m_TopLeftPosition.y);

		if (!result)
		{
			return false;
		}

		//result = transparentShader->Render10(m_CurrentSequence->GetCurrentFrame()->GetFrameTexture()->GetIndexCount(),
		//	camera, m_CurrentSequence->GetCurrentFrame()->GetFrameTexture()->GetTexture10(), blendAmount);

		if (!result)
		{
			return false;
		}*
	}
	else if (m_EngineOptions->m_d3dVersion == DIRECTX11)
	{
		AnimationFrame2D* frame = m_CurrentSequence->GetCurrentFrame();
		if (frame == NULL) return false;
		ZShadeSandboxMesh::QuadMesh* texture = frame->GetFrameTexture();
		if (texture == NULL) return false;
		
		texture->Update(m_TopLeftPosition.x, m_TopLeftPosition.y);
		
		// Use the transparent shader to render the mesh
		ZShadeSandboxMesh::MeshParameters mp = texture->MeshParams();
		mp.vertexType = ZShadeSandboxMesh::EVertexType::VT_Trans;
		texture->MeshParams() = mp;
		
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

		transparentShader->UseCustomWorld(true);
		transparentShader->SetCustomWorld(mWorld);

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
	mrp.camera = camera;
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
RECT PlatformerTile::GetBox()
{
	RECT rect;

	rect.top = m_TopLeftPosition.x;
	rect.left = m_TopLeftPosition.y;
	rect.right = rect.left + 64;
	rect.bottom = rect.top + 64;

	return rect;
}
//-----------------------------------------------------------------------------------------------------------------
XMFLOAT3 PlatformerTile::GetPosition()
{
	return m_TopLeftPosition;
}
//-----------------------------------------------------------------------------------------------------------------