#include "Sprite.h"
#include "MaterialManager.h"
#include "Convert.h"
#include "SAT.h"
#include "Scalar.h"
#include "Forces.h"
#include "PlatformerMap.h"
#include "Scripting.h"
//==================================================================================================================================
//==================================================================================================================================
Sprite::Sprite(D3D* d3d)
:   PhysicsBody(d3d),
	m_hardBoxSpr(NULL),
	//m_text(0),
	m_D3DSystem(d3d)
{
	m_EngineOptions = m_D3DSystem->GetEngineOptions();
	
	InitializeStats();
}
//==================================================================================================================================
//Sprite::Sprite(const Sprite& other)
//{
//}
//==================================================================================================================================
Sprite::~Sprite()
{
}
//==================================================================================================================================
bool Sprite::Initialize(string base_path, string textureFilename, XMFLOAT3 position, int bitmapWidth, int bitmapHeight, PhysicsType pt)
{
	PhysicsBody::Initialize(position, pt, bitmapWidth, bitmapHeight);
	
	// Initialize the body with platformer or topdown stats in movement
	if (m_EngineOptions->m_GameType2D == TOPDOWN)
	{
		PhysicsBody::InitializeParametersTopdown();
	}
	else if (m_EngineOptions->m_GameType2D == PLATFORMER)
	{
		PhysicsBody::InitializeParametersPlatformer();
	}
	
	m_width = bitmapWidth;
	m_height = bitmapHeight;

	m_baseTextureFilename = textureFilename;

	InitializeStats();

	SetHardbox(m_TopLeftPosition.x, m_TopLeftPosition.y + iBodyHeight / 2, iBodyWidth, iBodyHeight / 2);

	MaterialManager::Instance()->AddTransparentDiffuse(m_D3DSystem, base_path, textureFilename, textureFilename, textureFilename, XMFLOAT3(255, 0, 255));

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
	mp.material = MaterialManager::Instance()->GetMaterial(textureFilename);
	//mp.texturePath = base_path;
	//mp.textureName = textureFilename;
	mp.textureWidth = iBodyWidth;
	mp.textureHeight = iBodyHeight;
	//mp.tt = TextureType::PNG;
	mp.in2D = true;

	m_bitmap = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp, true);

	if (!m_bitmap) return false;

	//bool result = m_bitmap->Initialize("Render", base_path, textureFilename, iBodyWidth, iBodyHeight);

	//if (!result) return false;
	
	m_text.SetD3D(m_D3DSystem);
	m_text.Initialize();
	m_text.CreateShadow();
	m_text.ChangeFont(L"Times New Roman", 24.0f, FontStyle::BoldItalic, true);
	m_text.TextName() = "I am a Sprite";
	
	m_text_transform = XMMatrixTranslation(position.x, position.y + 20, 0);
	m_text_color = XMFLOAT4(1, 0, 0, 1);
	
	m_display_text = false;
	
	return true;
}
//==================================================================================================================================
//bool Sprite::Initialize(string base_path, string textureFilename, XMFLOAT3 position, int bitmapWidth, int bitmapHeight, PlatformerMap* map)
//{
//	DynamicPhysicsBody::Initialize(position, bitmapWidth, bitmapHeight, map);
//
//	m_width = bitmapWidth;
//	m_height = bitmapHeight;
//
//	m_baseTextureFilename = textureFilename;
//
//	InitializeStats();
//
//	SetHardbox(m_TopLeftPosition.x, m_TopLeftPosition.y + iBodyHeight / 2, iBodyWidth, iBodyHeight / 2);
//
//	m_bitmap = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem);
//
//	if (!m_bitmap) return false;
//
//	bool result = m_bitmap->Initialize("Render", base_path, textureFilename, iBodyWidth, iBodyHeight);
//
//	if (!result) return false;
//
//	return true;
//}
//==================================================================================================================================
void Sprite::InitializeStats()
{
	kPlatformerJumpVelocity = 900.0f * 1.2f;
	kReachTargetScale = 0.7f;
	
	m_move_up = false;
	m_move_down = false;
	m_move_left = false;
	m_move_right = false;

	m_CurrentSequence = NULL;
	m_displaySprite = false;
	m_displayHardbox = true;
	m_player = false;
	m_static = true;
	m_platformerSprite = false;
	bFlyMode = false;
	bVisible = true;

	m_SectionIn = NULL;

	m_text_timer = new SleepTimer();
	
	m_script = NULL;

	m_old_speed = 0;
	m_depth = 0;
	m_damageAmount = 0;
	m_vision = 0;
	m_hard = 0;
	m_nohit = 0;
	m_animationProfileName = "NONE";
	default_sequence = 0;
	m_animationSpeed = 0;
	m_hp_max = 100;
	m_hp = 100;
	m_mana = 100;
	m_coin = 0;
	m_gold = 0;
	m_strength = 0;
	m_speed = 0;
	m_defense = 0;
	m_experience = 0;
	m_level = 1;
	m_script_name = "NONE";
	m_touch = 0;
	m_background = 0;
	m_entity = 1;
	m_invisible = 0;
	m_can_teleport = 0;
	m_teleport_map = "NONE";
	m_teleport_x = -1;
	m_teleport_y = -1;
	m_name = "NONE";

	m_moving = false;
	m_thinking = false;
	m_talking = false;
	m_dying = false;
	m_hit = false;
	m_touched = false;
	m_attacked = false;
	m_targeted = false;
}
//==================================================================================================================================
void Sprite::OnCollision(bool bTop, bool bBottom, bool bLeft, bool bRight, int damageAmount)
{
	// I am hurt so leave me alone until I am not invincible
	if (IsHurt()) return;
	
	// This sprite can be hit
	if ((bTop && VulnerableFromAbove()) ||
	    (bBottom && VulnerableFromBelow()) ||
		(bLeft && VulnerableFromLeft()) ||
		(bRight && VulnerableFromRight()))
	{
		// I was hit make sure I don't take too much damage too fast
		MakeTemporarilyInvincible();
		
		// Tell the AI part of me that I was hit
		m_hit = true;
		
		// Tell Lua how much damage I am suppose to take after being hit
		m_damageAmount = damageAmount;
	}
}
//==================================================================================================================================
void Sprite::Update(float dt, Keyboard* keyboard)
{
	/*
	if (m_platformerSprite)
	{
		if (m_PlatformerMap == NULL)
			return;
	}
	else
	{
		if (m_TopdownMap == NULL)
			return;
	}

	// Need to edit keyboard control for the player, maybe add player control in sprite class
	if (m_player && m_platformerSprite)
	{
		PlayerPlatformerInput(dt, keyboard);
	}

	if (bFlyMode)
	{
		//m_Velocity.x *= 0.5f;
		//m_Velocity.y *= 0.5f;
	}
	else
	{
		//m_Velocity.y += GRAVITATION_MARS;
	}
	
	// Clamp the speed
	m_Velocity.x = Scalar::Clamp(m_Velocity.x, -MAX_SPEED, MAX_SPEED);
	m_Velocity.y = min( m_Velocity.y, MAX_SPEED * 2 );
	
	PhysicsBody::Update(dt);
	*/
	
	bPlayer = m_player;
	
	if (m_player)
	{
		if (m_platformerSprite)
			PlayerPlatformerInput(dt, keyboard);
		else
			PlayerTopdownInput(dt, keyboard);
	}
	
	// Move the body if it is dynamic
	PhysicsBody::Move(dt);
	
	if (iHurtTimer > 0)
	{
		bVisible = (iHurtTimer & 1) == 1;
		
		iHurtTimer--;
	}
}
//==================================================================================================================================
void Sprite::PlayerPlatformerInput(float dt, Keyboard* keyboard)
{
	//
	// normal walking controls
	//

	if (keyboard->RisingEdge(Keyboard::Key::A))
	{
		bMovingLeft = true;

		// Face Left
		SetMoveSpriteLeft(true);
		SetMoveSpriteRight(false);
	}
	else if (keyboard->RisingEdge(Keyboard::Key::D))
	{
		bMovingRight = true;

		// Face Right
		SetMoveSpriteRight(true);
		SetMoveSpriteLeft(false);
	}
	else
	{
		bMovingLeft = false;
		bMovingRight = false;
		
		SetMoveSpriteLeft(false);
		SetMoveSpriteRight(false);
	}
}
//==================================================================================================================================
void Sprite::PlayerTopdownInput(float dt, Keyboard* keyboard)
{
	//
	// normal walking controls
	//
	
	if (keyboard->IsKeyDown(Keyboard::Key::W))
	{
		bMovingUp = true;

		// Face Up
		SetMoveSpriteUp(true);
		SetMoveSpriteDown(false);
		
		accY = Speed();
	}
	else if (keyboard->IsKeyDown(Keyboard::Key::S))
	{
		bMovingDown = true;

		// Face Down
		SetMoveSpriteUp(true);
		SetMoveSpriteDown(false);
		
		accY = Speed();
	}
	else
	{
		bMovingUp = false;
		bMovingDown = false;
		
		SetMoveSpriteUp(false);
		SetMoveSpriteDown(false);
		
		// Reset the speed to 0, so that the sprite does not slide
		speedY = 0;
	}
	
	if (keyboard->IsKeyDown(Keyboard::Key::A))
	{
		bMovingLeft = true;

		// Face Left
		SetMoveSpriteLeft(true);
		SetMoveSpriteRight(false);
		
		accX = Speed();
	}
	else if (keyboard->IsKeyDown(Keyboard::Key::D))
	{
		bMovingRight = true;

		// Face Right
		SetMoveSpriteRight(true);
		SetMoveSpriteLeft(false);
		
		accX = Speed();
	}
	else
	{
		bMovingLeft = false;
		bMovingRight = false;
		
		SetMoveSpriteLeft(false);
		SetMoveSpriteRight(false);
		
		// Reset the acceleration speed to 0, so that the sprite does not slide
		speedX = 0;
	}
}
//==================================================================================================================================
void Sprite::InitFOWTiles(int radius)
{
	//m_FOWTiles.resize(radius, radius);
}
//==================================================================================================================================
void Sprite::CreateTexture(string base_path, string windowName, int screenWidth, int screenHeight)
{
	SetHardbox(m_TopLeftPosition.x, m_TopLeftPosition.y + m_height / 2, m_width, m_height / 2);
	
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
	//mp.texturePath = base_path;
	//mp.textureName = m_baseTextureFilename;
	mp.textureWidth = m_width;
	mp.textureHeight = m_height;
	//mp.tt = TextureType::PNG;
	mp.in2D = true;
	
	m_bitmap = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp);
	//m_bitmap->Initialize(windowName, base_path, m_baseTextureFilename, m_width, m_height);
}
//==================================================================================================================================
void Sprite::say(string text)
{
	m_text.TextName() = text;
	m_text_transform = XMMatrixTranslation(m_TopLeftPosition.x, m_TopLeftPosition.y + 20, 0);
	m_display_text = true;
}
//==================================================================================================================================
void Sprite::say(string text, int font_size)
{
	m_text.TextName() = text;
	m_text.ChangeFont(L"Times New Roman", font_size, FontStyle::BoldItalic, true);
	m_text_transform = XMMatrixTranslation(m_TopLeftPosition.x, m_TopLeftPosition.y + 20, 0);
	m_display_text = true;
}
//==================================================================================================================================
void Sprite::say(string text, int x, int y, int font_size)
{
	m_text.TextName() = text;
	m_text.ChangeFont(L"Times New Roman", font_size, FontStyle::BoldItalic, true);
	m_text_transform = XMMatrixTranslation(x, y + 20, 0);
	m_display_text = true;
}
//==================================================================================================================================
void Sprite::say(string text, int r, int g, int b, int font_size)
{
	m_text.TextName() = text;
	m_text.ChangeFont(L"Times New Roman", font_size, FontStyle::BoldItalic, true);
	m_text_transform = XMMatrixTranslation(m_TopLeftPosition.x, m_TopLeftPosition.y + 20, 0);
	m_text_color = XMFLOAT4(r, g, b, 1);
	m_display_text = true;
}
//==================================================================================================================================
void Sprite::say(string text, int x, int y, int r, int g, int b, int font_size)
{
	m_text.TextName() = text;
	m_text.ChangeFont(L"Times New Roman", font_size, FontStyle::BoldItalic, true);
	m_text_transform = XMMatrixTranslation(x, y + 20, 0);
	m_text_color = XMFLOAT4(r, g, b, 1);
	m_display_text = true;
}
//==================================================================================================================================
void Sprite::say_stop(string text)
{
	m_old_speed = m_speed;
	m_speed = 0;
	say(text);
}
//==================================================================================================================================
void Sprite::say_stop(string text, int font_size)
{
	m_old_speed = m_speed;
	m_speed = 0;
	say(text, font_size);
}
//==================================================================================================================================
void Sprite::say_stop(string text, int x, int y, int font_size)
{
	m_old_speed = m_speed;
	m_speed = 0;
	say(text, x, y, font_size);
}
//==================================================================================================================================
void Sprite::say_stop(string text, int r, int g, int b, int font_size)
{
	m_old_speed = m_speed;
	m_speed = 0;
	say(text, r, g, b, font_size);
}
//==================================================================================================================================
void Sprite::say_stop(string text, int x, int y, int r, int g, int b, int font_size)
{
	m_old_speed = m_speed;
	m_speed = 0;
	say(text, x, y, r, g, b, font_size);
}
//==================================================================================================================================
bool valueInRange(int value, int min, int max)
{ return (value >= min) && (value <= max); }
//==================================================================================================================================
bool Sprite::CollidesRect(Sprite* spr)
{
	if (!IsHard()) return false;
	if (!spr->IsHard()) return false;
	if (spr == this) return false;

	bool collided = false;

	Sprite* player_hb = GetHardBoxSpr();
	SAT_RECT p_rect(player_hb->TopLeftPosition().x, player_hb->TopLeftPosition().y, player_hb->Width(), player_hb->Height());

	Sprite* hb = spr->GetHardBoxSpr();
	if (hb != NULL)
	{
		SAT_RECT s_rect(hb->TopLeftPosition().x, hb->TopLeftPosition().y, hb->Width(), hb->Height());

		SAT sat;
		if (sat.collides2( p_rect, s_rect ))
		{
			//Flag both as touched
			m_touched = true;
			spr->IsTouched() = true;
			return true;
		}

		//bool xOverlap = valueInRange(p_rect.x, s_rect.x, s_rect.x + s_rect.width) ||
		//				valueInRange(s_rect.x, p_rect.x, p_rect.x + p_rect.width);

		//bool yOverlap = valueInRange(p_rect.y, s_rect.y, s_rect.y + s_rect.height) ||
		//				valueInRange(s_rect.y, p_rect.y, p_rect.y + s_rect.height);

		//if (xOverlap && yOverlap)
		//{
		//	//Flag both as touched
		//	SetTouched(true);
		//	spr->SetTouched(true);
		//}

		//return xOverlap && yOverlap;
	}

	return false;
}
//==================================================================================================================================
bool Sprite::CollidesWith(Sprite* spr)
{
	if (!IsHard()) return false;
	if (!spr->IsHard()) return false;
	if (spr == this) return false;

	bool collided = false;

	SAT sat;

	Sprite* player_hb = GetHardBoxSpr();

	SAT_RECT p_rect(player_hb->TopLeftPosition().x, player_hb->TopLeftPosition().y, player_hb->Width(), player_hb->Height());

	XMFLOAT3 p_vec = player_hb->TopLeftPosition();

	Sprite* hb = spr->GetHardBoxSpr();

	if (hb != NULL)
	{
		SAT_RECT s_rect(hb->TopLeftPosition().x, hb->TopLeftPosition().y, hb->Width(), hb->Height());
				
		collided = sat.collides( p_rect, s_rect );
		
		if (collided)
		{
			//Flag both as touched
			m_touched = true;
			spr->IsTouched() = true;
		}

		MTV mtv = sat.GetMTV();
		SAT_POINT mtv_vec;

		while (collided)
		{
			mtv = sat.GetMTV();

			collided = sat.collides( p_rect, s_rect );

			//collided = sat.collides2( p_rect, s_rect );
			//if (!sat.collides2( p_rect, s_rect ))
			//	break;

			//Move the player with the MTV
			//Normalize the MTV vector and multiply by the overlap

			mtv_vec = mtv._smallest.normalize();

			if (MoveSpriteUp())
			{
				mtv_vec.y = -mtv_vec.y;
			}

			if (MoveSpriteLeft())
			{
				mtv_vec.x = -mtv_vec.x;
			}

			//project the new m_TopLeftPosition to push the player away from the other sprite
			p_vec.x += mtv_vec.x;
			p_vec.y += mtv_vec.y;

			p_rect.x = p_vec.x;
			p_rect.y = p_vec.y;

			//Update the players m_TopLeftPosition and hardbox
			XMFLOAT3 p_pos = TopLeftPosition();

			p_pos.x += mtv_vec.x;
			p_pos.y += mtv_vec.y;

			Move(p_pos.x, p_pos.y, p_vec.x, p_vec.y);
		}
	}
	
	return collided;
}
//==================================================================================================================================
void Sprite::Move(float newPosX, float newPosY, float newPosHBX, float newPosHBY)
{
	m_TopLeftPosition = XMFLOAT3(newPosX, newPosY, 0);

	if (m_hardBoxSpr != 0)
	{
		float hb_x, hb_y;
		int hb_w, hb_h;
		GetHardbox(hb_x, hb_y, hb_w, hb_h);
		SetHardbox(newPosHBX, newPosHBY + hb_h, hb_w, hb_h);
		m_hardBoxSpr->Move(newPosHBX, newPosHBY, 0, 0);
	}
}
//==================================================================================================================================
/*void Sprite::SetScript(string script)
{
	m_script = script;

	if (m_script != "NONE")
	{
		m_spriteScript = new LuaSpriteScript();
		m_spriteScript->SetScript( m_script );
	}
	else
	{
		m_spriteScript = 0;
	}
}*/
void Sprite::AddScript(string name)
{
	m_script_name = name;
	
	if (m_script_name != "NONE")
		m_script = Scripting::GetScript(name);
	else
		m_script = 0;
}
//==================================================================================================================================
void Sprite::GetHardbox(float& x, float& y, int& w, int& h)
{
	x = m_hard_box.m_x;
	y = m_hard_box.m_y;
	w = m_hard_box.width;
	h = m_hard_box.height;
}
//==================================================================================================================================
void Sprite::SetHardbox(float x, float y, int w, int h)
{
	m_hard_box.m_x = x;
	m_hard_box.m_y = y;
	m_hard_box.width = w;
	m_hard_box.height = h;
}
//==================================================================================================================================
void Sprite::CreateHardBoxSpr(string editor_base)
{
	m_hardBoxSpr = new Sprite(m_D3DSystem);

	m_hardBoxSpr->Initialize(editor_base, "white.png", XMFLOAT3(m_hard_box.m_x, m_hard_box.m_y, 0), m_hard_box.width, m_hard_box.height, DYNAMIC);
}
//==================================================================================================================================
void Sprite::AddAnimationSequence(Animation2D* sequence)
{
	if (sequence == NULL) return;
	sequence->LoadFrameTextures();
	mAnimationSequences.push_back(sequence);
}
//==================================================================================================================================
void Sprite::ClearAnimationList()
{
	if (mAnimationSequences.size() > 0) mAnimationSequences.clear();
}
//==================================================================================================================================
void Sprite::SetSequence(int i)
{
	m_CurrentSequence = mAnimationSequences[i];
}
//==================================================================================================================================
void Sprite::UpdateAnimation()
{
	if (m_CurrentSequence == NULL) return;

	m_CurrentSequence->Update(m_animationSpeed);
}
//==================================================================================================================================
void Sprite::AddAnimation()
{
	//Load a animation profile
	AnimationProfile2D* profile = new AnimationProfile2D(m_D3DSystem);

	bool something_wrong = false;

	if (profile->ReadProfile(m_animationProfileName, "SPRITE", Scripting::GetGameDirectory()->m_sprites_path, Scripting::GetGameDirectory()->GAME_FOLDER))
	{
		vector<Animation2D*> anims = profile->GetAnimations();

		ClearAnimationList();

		for (int i = 0; i < anims.size(); i++)
		{
			AddAnimationSequence(anims[i]);
		}

		SetSequence(default_sequence);
	}
	else something_wrong = true;

	if (something_wrong)
	{
		MessageBox(NULL, "The Animation2D does not exist", "Add Animations", MB_OK);
	}
}
//==================================================================================================================================
Sprite* Sprite::Clone(GameDirectory2D* gd)
{
	string filename = BaseTextureFilename();
	int w = Width();
	int h = Height();
	bool is_player = IsPlayer();
	bool is_platformer_sprite = m_platformerSprite;
	int vision = Vision();
	int depth = Depth();
	int hard = IsHard();
	int nohit = IsNoHit();
	int hp = HP();
	int mana = Mana();
	int gold = Gold();
	int strength = Strength();
	float speed = Speed();
	int defense = Defense();
	int exp = Experience();
	int level = Level();
	int touch = IsTouch();
	int background = IsBackgroundSprite();
	int entity = IsEntitySprite();
	int invisible = IsInvisibleSprite();
	int teleport = CanTeleport();
	int teleport_map_x = TeleportX();
	int teleport_map_y = TeleportY();
	int default_seq = DefaultSequence();
	int anim_speed = AnimationSpeed();
	int id = ID();
	int max_hp = HPMax();
	int is_static = IsStatic();
	string name_str = Name();
	string animation_str = AnimationProfileName();
	string script_str = ScriptName();
	string teleportMap_str = TeleportMapName();
	
	Section* s_in = 0;
	if (!m_platformerSprite)
		;// s_in = this->SectionIn()->Clone();
	
	string path;
	if (filename == "player_cover.png")
	{
		path = gd->m_required_textures;
	}
	else
	{
		path = gd->m_sprites_path;
	}
	
	//Add the sprite back in
	std::unique_ptr<Sprite> spr;
	spr.reset(new Sprite(m_D3DSystem));
	spr->Initialize(path, filename, XMFLOAT3(TopLeftPosition().x, TopLeftPosition().y, 0), w, h, EPhysicsType());
	spr->IsDisplaySprite() = false;
	
	if (!m_platformerSprite)
		spr->SetSectionIn(s_in);
	
	//Need to update all attributes of the sprite
	spr->PlatformerSprite() = is_platformer_sprite;
	spr->ID() = id;
	spr->Vision() = vision;
	spr->Depth() = depth;
	spr->IsHard() = hard;
	spr->IsNoHit() = nohit;
	spr->AnimationProfileName() = animation_str;
	spr->DefaultSequence() = default_seq;
	spr->AnimationSpeed() = anim_speed;
	spr->HP() = hp;
	spr->Mana() = mana;
	spr->Gold() = gold;
	spr->Strength() = strength;
	spr->Speed() = speed;
	spr->Defense() = defense;
	spr->Experience() = exp;
	spr->Level() = level;
	spr->ScriptName() = script_str;
	spr->IsTouch() = touch;
	spr->IsBackgroundSprite() = background;
	spr->IsEntitySprite() = entity;
	spr->IsInvisibleSprite() = invisible;
	spr->CanTeleport() = teleport;
	spr->TeleportMapName() = teleportMap_str;
	spr->TeleportX() = teleport_map_x;
	spr->TeleportY() = teleport_map_y;
	spr->Name() = name_str;
	spr->HPMax() = max_hp;
	spr->IsStatic() = is_static;
	
	//spr->AddScript(script_str);
	
	//Create the new hard box
	string p = gd->m_required_textures;
	
	//Set the sprites hardbox
	spr->SetHardbox(
		TopLeftPosition().x, TopLeftPosition().y + h / 2, w, h / 2
	);
	
	spr->CreateHardBoxSpr(p);
	
	return spr.get();
}
//==================================================================================================================================
void Sprite::CloneMe(GameDirectory2D* gd, float x, float y, int w, int h)
{
	string filename = BaseTextureFilename();
	
	float hbox_x;
	float hbox_y;
	int hbox_w;
	int hbox_h;
	GetHardbox(hbox_x, hbox_y, hbox_w, hbox_h);
	
	bool is_player = IsPlayer();
	bool is_platformer_sprite = m_platformerSprite;
	int vision = Vision();
	int depth = Depth();
	int hard = IsHard();
	int nohit = IsNoHit();
	int hp = HP();
	int mana = Mana();
	int gold = Gold();
	int strength = Strength();
	float speed = Speed();
	int defense = Defense();
	int exp = Experience();
	int level = Level();
	int touch = IsTouch();
	int background = IsBackgroundSprite();
	int entity = IsEntitySprite();
	int invisible = IsInvisibleSprite();
	int teleport = CanTeleport();
	int teleport_map_x = TeleportX();
	int teleport_map_y = TeleportY();
	int default_seq = DefaultSequence();
	int anim_speed = AnimationSpeed();
	int id = ID();
	int max_hp = HPMax();
	int is_static = IsStatic();
	string name_str = Name();
	string animation_str = AnimationProfileName();
	string script_str = ScriptName();
	string teleportMap_str = TeleportMapName();
	
	Section* s_in = 0;
	if (!m_platformerSprite)
		;// s_in = SectionIn()->Clone();
	
	string path;
	if (filename == "player_cover.png")
	{
		path = gd->m_required_textures;
	}
	else
	{
		path = gd->m_sprites_path;
	}
	
	//Add the sprite back in
	Initialize(path, filename, XMFLOAT3(x, y, 0), w, h, EPhysicsType());

	IsDisplaySprite() = false;
	
	if (!m_platformerSprite)
		SetSectionIn(s_in);
	
	//Need to update all attributes of the sprite
	PlatformerSprite() = is_platformer_sprite;
	ID() = id;
	Vision() = vision;
	Depth() = depth;
	IsHard() = hard;
	IsNoHit() = nohit;
	AnimationProfileName() = animation_str;
	DefaultSequence() = default_seq;
	AnimationSpeed() = anim_speed;
	HP() = hp;
	Mana() = mana;
	Gold() = gold;
	Strength() = strength;
	Speed() = speed;
	Defense() = defense;
	Experience() = exp;
	Level() = level;
	ScriptName() = script_str;
	IsTouch() = touch;
	IsBackgroundSprite() = background;
	IsEntitySprite() = entity;
	IsInvisibleSprite() = invisible;
	CanTeleport() = teleport;
	TeleportMapName() = teleportMap_str;
	TeleportX() = teleport_map_x;
	TeleportY() = teleport_map_y;
	Name() = name_str;
	HPMax() = max_hp;
	IsStatic() = is_static;
	
	//AddScript(script_str);
	
	//Create the new hard box
	string p = gd->m_required_textures;
	
	SetHardbox(hbox_x, hbox_y, hbox_w, hbox_h);
	CreateHardBoxSpr(p);
}
//==================================================================================================================================
bool Sprite::Render(Camera* camera)
{
	// !bVisible and m_invisible should be the same, why do I need both ??
	
	if (!bVisible) return true;
	
	if (m_invisible && m_hard)
	{
		if (m_hardBoxSpr != NULL)
		{
			if (m_displayHardbox) m_hardBoxSpr->Render(camera);
		}

		return true;
	}

	if (m_invisible) return true;

	if (m_CurrentSequence == NULL)
	{
		m_bitmap->UpdateBuffers(m_TopLeftPosition.x, m_TopLeftPosition.y);

		ZShadeSandboxMesh::MeshRenderParameters mrp;
		mrp.pCamera = camera;
		mrp.bSpecifyWorld = true;
		mrp.world = XMMatrixIdentity();
		m_bitmap->Render(mrp);

		if (m_hardBoxSpr != NULL)
		{
			if (m_displayHardbox && m_hard) m_hardBoxSpr->Render(camera);
		}
	}
	else
	{
		RenderAnimation(camera);
	}

	if (m_display_text)
	{
		m_text_timer->Sleep( 300 );
		if (m_text_timer->IsTimeUp())
		{
			m_display_text = false;
			Speed() = m_old_speed;
		}
		else
		{
			m_text.TextTransform() = m_text_transform;
			m_text.TextColor() = m_text_color;
			m_text.Render11();
		}
	}

	return true;
}
//==================================================================================================================================
bool Sprite::Render(Camera* camera, float blendAmount)
{
	if (!bVisible) return true;
	
	if (m_invisible && m_hard)
	{
		if (m_hardBoxSpr != NULL)
		{
			if (m_displayHardbox) m_hardBoxSpr->Render(camera, blendAmount);
		}

		return true;
	}

	if (m_invisible) return true;

	if (m_CurrentSequence == NULL)
	{
		m_bitmap->UpdateBuffers(m_TopLeftPosition.x, m_TopLeftPosition.y);

		ZShadeSandboxMesh::MeshRenderParameters mrp;
		mrp.pCamera = camera;
		//mrp.bTransparent = true;
		//mrp.blendAmount = blendAmount;
		mrp.bSpecifyWorld = true;
		mrp.world = XMMatrixIdentity();
		m_bitmap->Render(mrp);

		if (m_hardBoxSpr != NULL)
		{
			if (m_displayHardbox && m_hard) m_hardBoxSpr->Render(camera, blendAmount);
		}
	}
	else
	{
		RenderAnimation(camera, blendAmount);
	}
	
	if (m_display_text)
	{
		m_text_timer->Sleep( 300 );
		if (m_text_timer->IsTimeUp())
		{
			m_display_text = false;
			Speed() = m_old_speed;
		}
		else
		{
			m_text.TextTransform() = m_text_transform;
			m_text.TextColor() = m_text_color;
			m_text.Render11();
		}
	}

	return true;
}
//==================================================================================================================================
bool Sprite::RenderAnimation(Camera* camera)
{
	if (!bVisible) return true;
	
	if (m_invisible && m_hard)
	{
		if (m_hardBoxSpr != NULL)
		{
			if (m_displayHardbox) m_hardBoxSpr->Render(camera);
		}

		return true;
	}

	if (m_invisible) return true;

	if (m_CurrentSequence->GetCurrentFrame() == NULL)
		return false;
	
	ZShadeSandboxMesh::QuadMesh* texture = m_CurrentSequence->GetCurrentFrame()->GetFrameTexture();
	
	texture->UpdateBuffers(m_TopLeftPosition.x, m_TopLeftPosition.y);

	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.pCamera = camera;
	mrp.bSpecifyWorld = true;
	mrp.world = XMMatrixIdentity();
	texture->Render(mrp);

	if (m_hardBoxSpr != NULL)
	{
		if (m_displayHardbox && m_hard) m_hardBoxSpr->Render(camera);
	}

	return true;
}
//==================================================================================================================================
bool Sprite::RenderAnimation(Camera* camera, float blendAmount)
{
	if (!bVisible) return true;
	
	if (m_invisible && m_hard)
	{
		if (m_hardBoxSpr != NULL)
		{
			if (m_displayHardbox) m_hardBoxSpr->Render(camera, blendAmount);
		}

		return true;
	}

	if (m_invisible) return true;

	
	AnimationFrame2D* frame = m_CurrentSequence->GetCurrentFrame();
	if (frame == NULL) return false;
	ZShadeSandboxMesh::QuadMesh* texture = frame->GetFrameTexture();
	
	texture->UpdateBuffers(m_TopLeftPosition.x, m_TopLeftPosition.y);

	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.pCamera = camera;
	//mrp.bTransparent = true;
	//mrp.blendAmount = blendAmount;
	mrp.bSpecifyWorld = true;
	mrp.world = XMMatrixIdentity();
	texture->Render(mrp);

	if (m_hardBoxSpr != NULL)
	{
		if (m_displayHardbox && m_hard) m_hardBoxSpr->Render(camera, blendAmount);
	}

	return true;
}
//==================================================================================================================================
/*bool Sprite::Render10(TextureShader* textureShader, Camera* camera)
{
	if (!bVisible) return true;
	
	if (m_invisible && m_hard)
	{
		if (m_hardBoxSpr != NULL)
		{
			if (m_displayHardbox) m_hardBoxSpr->Render10(textureShader, camera);
		}

		return true;
	}
	
	if (m_invisible) return true;

	if (m_CurrentSequence == NULL)
	{
		m_bitmap->Update(m_TopLeftPosition.x, m_TopLeftPosition.y);

		ZShadeSandboxMesh::MeshRenderParameters mrp;
		mrp.pCamera = camera;
		m_bitmap->Render(mrp);

		//result = textureShader->Render10(m_bitmap->GetIndexCount(), XMMath4(0,0,0,0), camera, m_bitmap->GetTexture10());
		
		//if (!result)
		//{
		//	return false;
		//}

		if (m_hardBoxSpr != NULL)
		{
			if (m_displayHardbox && m_hard) m_hardBoxSpr->Render10(textureShader, camera);
		}
	}
	else
	{
		RenderAnimation10(textureShader, camera);
	}

	//if (m_text != 0)
	//{
	//	m_text_timer->Sleep( 300 );
	//	if (m_text_timer->IsTimeUp())
	//	{
	//		SetSpeed(m_old_speed);
	//		m_text->ChangeText("");
	//	}
	//	
	//	m_text->SetXPos( GetPosition().x );
	//	m_text->SetYPos( GetPosition().y - 20 );
	//	//m_text->DrawFont10(false, textureShader, 0, worldMatrix, orthoMatrix, viewMatrix);
	//}

	return true;
}
//==================================================================================================================================
bool Sprite::RenderTransparent10(TransparentShader* transparentShader, Camera* camera, float blendAmount)
{
	if (!bVisible) return true;
	
	if (m_invisible && m_hard)
	{
		if (m_hardBoxSpr != NULL)
		{
			if (m_displayHardbox) m_hardBoxSpr->RenderTransparent10(transparentShader, camera, blendAmount);
		}

		return true;
	}

	if (m_invisible) return true;

	if (m_CurrentSequence == NULL)
	{
		/*bool result = m_bitmap->Render(m_TopLeftPosition.x, m_TopLeftPosition.y);

		if (!result)
		{
			return false;
		}

		//result = transparentShader->Render10(m_bitmap->GetIndexCount(), camera, m_bitmap->GetTexture10(), blendAmount);
		
		if (!result)
		{
			return false;
		}
		*

		m_bitmap->Update(m_TopLeftPosition.x, m_TopLeftPosition.y);

		ZShadeSandboxMesh::MeshRenderParameters mrp;
		mrp.pCamera = camera;
		m_bitmap->Render(mrp);

		if (m_hardBoxSpr != NULL)
		{
			if (m_displayHardbox && m_hard) m_hardBoxSpr->RenderTransparent10(transparentShader, camera, blendAmount);
		}
	}
	else
	{
		RenderAnimationTransparent10(transparentShader, camera, blendAmount);
	}

	//if (m_text != 0)
	//{
	//	m_text_timer->Sleep( 300 );
	//	if (m_text_timer->IsTimeUp())
	//	{
	//		SetSpeed(m_old_speed);
	//		m_text->ChangeText("");
	//	}
	//	
	//	m_text->SetXPos( GetPosition().x );
	//	m_text->SetYPos( GetPosition().y - 20 );
	//	//m_text->DrawFont10(true, 0, transparentShader, worldMatrix, orthoMatrix, viewMatrix);
	//}

	return true;
}
//==================================================================================================================================
bool Sprite::RenderAnimation10(TextureShader* textureShader, Camera* camera)
{
	if (!bVisible) return true;
	
	if (m_invisible && m_hard)
	{
		if (m_hardBoxSpr != NULL)
		{
			if (m_displayHardbox) m_hardBoxSpr->Render10(textureShader, camera);
		}

		return true;
	}

	if (m_invisible) return true;

	m_CurrentSequence->GetCurrentFrame()->GetFrameTexture()->Update(m_TopLeftPosition.x, m_TopLeftPosition.y);

	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.pCamera = camera;
	m_CurrentSequence->GetCurrentFrame()->GetFrameTexture()->Render(mrp);
	
	//if (!m_CurrentSequence->GetCurrentFrame()->GetFrameTexture()->GetTexture11())
	//	return false;

	/*bool result = m_CurrentSequence->GetCurrentFrame()->GetFrameTexture()->Render(m_TopLeftPosition.x, m_TopLeftPosition.y);

	if (!result)
	{
		return false;
	}

	//result = textureShader->Render10(m_CurrentSequence->GetCurrentFrame()->GetFrameTexture()->GetIndexCount(),
	//	camera, m_CurrentSequence->GetCurrentFrame()->GetFrameTexture()->GetTexture10());

	if (!result)
	{
		return false;
	}
	*

	if (m_hardBoxSpr != NULL)
	{
		if (m_displayHardbox && m_hard) m_hardBoxSpr->Render10(textureShader, camera);
	}

	return true;
}
//==================================================================================================================================
bool Sprite::RenderAnimationTransparent10(TransparentShader* transparentShader, Camera* camera, float blendAmount)
{
	if (!bVisible) return true;
	
	if (m_invisible && m_hard)
	{
		if (m_hardBoxSpr != NULL)
		{
			if (m_displayHardbox) m_hardBoxSpr->RenderTransparent10(transparentShader, camera, blendAmount);
		}

		return true;
	}

	if (m_invisible) return true;

	AnimationFrame2D* frame = m_CurrentSequence->GetCurrentFrame();
	if (frame == NULL) return false;
	ZShadeSandboxMesh::QuadMesh* texture = frame->GetFrameTexture();

	texture->Update(m_TopLeftPosition.x, m_TopLeftPosition.y);

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
	}
	*

	if (m_hardBoxSpr != NULL)
	{
		if (m_displayHardbox && m_hard) m_hardBoxSpr->RenderTransparent10(transparentShader, camera, blendAmount);
	}

	return true;
}
//==================================================================================================================================
bool Sprite::Render11(TextureShader* textureShader, Camera* camera)
{
	if (!bVisible) return true;
	
	if (m_invisible && m_hard)
	{
		if (m_hardBoxSpr != NULL)
		{
			if (m_displayHardbox) m_hardBoxSpr->Render11(textureShader, camera);
		}

		return true;
	}

	if (m_invisible) return true;

	if (m_CurrentSequence == NULL)
	{
		/*bool result = m_bitmap->Render(m_TopLeftPosition.x, m_TopLeftPosition.y);

		if (!result)
		{
			return false;
		}

		//textureShader->UseCustomWorld(true);
		//textureShader->SetCustomWorld(mWorld);

		result = textureShader->Render11(m_bitmap->GetIndexCount(), XMMath4(0, 0, 0, 0), camera, m_bitmap->GetTexture11());

		if (!result)
		{
			return false;
		}
		*

		m_bitmap->Update(m_TopLeftPosition.x, m_TopLeftPosition.y);

		ZShadeSandboxMesh::MeshRenderParameters mrp;
		mrp.pCamera = camera;
		m_bitmap->Render(mrp);

		if (m_hardBoxSpr != NULL)
		{
			if (m_displayHardbox && m_hard) m_hardBoxSpr->Render11(textureShader, camera);
		}
	}
	else
	{
		RenderAnimation11(textureShader, camera);
	}

	if (m_display_text)
	{
		m_text_timer->Sleep( 300 );
		if (m_text_timer->IsTimeUp())
		{
			m_display_text = false;
			Speed() = m_old_speed;
		}
		else
		{
			m_text.TextTransform() = m_text_transform;
			m_text.TextColor() = m_text_color;
			m_text.Render11();
		}
	}

	return true;
}
//==================================================================================================================================
bool Sprite::RenderTransparent11(TransparentShader* transparentShader, Camera* camera, float blendAmount)
{
	if (!bVisible) return true;
	
	if (m_invisible && m_hard)
	{
		if (m_hardBoxSpr != NULL)
		{
			if (m_displayHardbox) m_hardBoxSpr->RenderTransparent11(transparentShader, camera, blendAmount);
		}

		return true;
	}

	if (m_invisible) return true;

	if (m_CurrentSequence == NULL)
	{
		/*bool result = m_bitmap->Render(m_TopLeftPosition.x, m_TopLeftPosition.y);

		if (!result)
		{
			return false;
		}

		result = transparentShader->Render11(m_bitmap->GetIndexCount(), XMMath4(0, 0, 0, 0), camera, m_bitmap->GetTexture11(), blendAmount);
	
		if (!result)
		{
			return false;
		}*

		m_bitmap->Update(m_TopLeftPosition.x, m_TopLeftPosition.y);

		ZShadeSandboxMesh::MeshRenderParameters mrp;
		mrp.pCamera = camera;
		m_bitmap->Render(mrp);

		if (m_hardBoxSpr != NULL)
		{
			if (m_displayHardbox && m_hard) m_hardBoxSpr->RenderTransparent11(transparentShader, camera, blendAmount);
		}
	}
	else
	{
		RenderAnimationTransparent11(transparentShader, camera, blendAmount);
	}
	
	if (m_display_text)
	{
		m_text_timer->Sleep( 300 );
		if (m_text_timer->IsTimeUp())
		{
			m_display_text = false;
			Speed() = m_old_speed;
		}
		else
		{
			m_text.TextTransform() = m_text_transform;
			m_text.TextColor() = m_text_color;
			m_text.Render11();
		}
	}

	return true;
}
//==================================================================================================================================
bool Sprite::RenderAnimation11(TextureShader* textureShader, Camera* camera)
{
	if (!bVisible) return true;
	
	if (m_invisible && m_hard)
	{
		if (m_hardBoxSpr != NULL)
		{
			if (m_displayHardbox) m_hardBoxSpr->Render11(textureShader, camera);
		}

		return true;
	}

	if (m_invisible) return true;

	if (m_CurrentSequence->GetCurrentFrame() == NULL)
		return false;
	
	/*if (!m_CurrentSequence->GetCurrentFrame()->GetFrameTexture()->GetTexture11())
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

	m_CurrentSequence->GetCurrentFrame()->GetFrameTexture()->Update(m_TopLeftPosition.x, m_TopLeftPosition.y);

	ZShadeSandboxMesh::MeshRenderParameters mrp;
	mrp.pCamera = camera;
	m_CurrentSequence->GetCurrentFrame()->GetFrameTexture()->Render(mrp);

	if (m_hardBoxSpr != NULL)
	{
		if (m_displayHardbox && m_hard) m_hardBoxSpr->Render11(textureShader, camera);
	}

	return true;
}
//==================================================================================================================================
bool Sprite::RenderAnimationTransparent11(TransparentShader* transparentShader, Camera* camera, float blendAmount)
{
	if (!bVisible) return true;
	
	if (m_invisible && m_hard)
	{
		if (m_hardBoxSpr != NULL)
		{
			if (m_displayHardbox) m_hardBoxSpr->RenderTransparent11(transparentShader, camera, blendAmount);
		}

		return true;
	}

	if (m_invisible) return true;

	
	AnimationFrame2D* frame = m_CurrentSequence->GetCurrentFrame();
	if (frame == NULL) return false;
	ZShadeSandboxMesh::QuadMesh* texture = frame->GetFrameTexture();
	
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

	if (m_hardBoxSpr != NULL)
	{
		if (m_displayHardbox && m_hard) m_hardBoxSpr->RenderTransparent11(transparentShader, camera, blendAmount);
	}

	return true;
}*/
//==================================================================================================================================