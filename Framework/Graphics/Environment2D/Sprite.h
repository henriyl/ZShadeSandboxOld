//==================================================================================================================================
// Sprite.h
//
//==================================================================================================================================
// History
//
// -Created in 2013 by Dustin Watson
// -Updated on 1/16/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __ZSPRITE_H
#define __ZSPRITE_H
//==================================================================================================================================
//==================================================================================================================================
//Includes
#include "D3DIncludes.h"
//#include "TextureShader.h"
//#include "TransparentShader.h"
#include "GameDirectory.h"
#include "D3D.h"
#include "QuadMesh.h"
#include "Animation2D.h"
#include <string>
//#include "Script.h"
//#include "lua.hpp"
//#include "luawrapper.hpp"
//#include "Font.h"
#include "MemoryMgr.h"
//#include "SpriteBatch.h"
//#include "PlatformerMap.h"
#include "PhysicsBody.h"
#include "Input.h"
class Tile;
#include "Text.h"
//using namespace GUIComponents;
//#include "LuaSpriteScript.h"
using namespace std;
class Section;
//class PlatformerMap;
class Script;
//==================================================================================================================================
//==================================================================================================================================
class Sprite : public PhysicsBody
{
public:
	Sprite(D3D* d3d);
	//Sprite(const Sprite& other);
	~Sprite();

	bool Initialize(string base_path, string textureFilename, XMFLOAT3 position, int bitmapWidth, int bitmapHeight, PhysicsType pt);//, Map* map);
	//bool Initialize(string base_path, string textureFilename, XMFLOAT3 position, int bitmapWidth, int bitmapHeight, PlatformerMap* map);
	
	void InitializeStats();

	// Update sprite physics body
	void Update(float dt, Keyboard* keyboard);
	void PlayerPlatformerInput(float dt, Keyboard* keyboard);
	void PlayerTopdownInput(float dt, Keyboard* keyboard);
	
	void CreateTexture(string base_path, string windowName, int screenWidth, int screenHeight);
	
	//bool Render10(TextureShader* textureShader, Camera* camera);
	//bool RenderTransparent10(TransparentShader* transparentShader, Camera* camera, float blendAmount);
	//bool Render11(TextureShader* textureShader, Camera* camera);
	//bool RenderTransparent11(TransparentShader* transparentShader, Camera* camera, float blendAmount);
	
	//bool RenderAnimation10(TextureShader* textureShader, Camera* camera);
	//bool RenderAnimationTransparent10(TransparentShader* transparentShader, Camera* camera, float blendAmount);
	//bool RenderAnimation11(TextureShader* textureShader, Camera* camera);
	//bool RenderAnimationTransparent11(TransparentShader* transparentShader, Camera* camera, float blendAmount);
	
	// Render non-transparent texture
	bool Render(Camera* camera);
	
	// Render transparent texture
	bool Render(Camera* camera, float blendAmount);
	
	// Render non-transparent animation
	bool RenderAnimation(Camera* camera);
	
	// Render transparent animation
	bool RenderAnimation(Camera* camera, float blendAmount);
	
	void UpdateAnimation();
	
	// Interfaces with the collision system of the body
	void OnCollision(bool bTop, bool bBottom, bool bLeft, bool bRight, int damageAmount);
	
	void say(string text);
	void say(string text, int font_size);
	void say(string text, int x, int y, int font_size);
	void say(string text, int r, int g, int b, int font_size);
	void say(string text, int x, int y, int r, int g, int b, int font_size);
	
	void say_stop(string text);
	void say_stop(string text, int font_size);
	void say_stop(string text, int x, int y, int font_size);
	void say_stop(string text, int r, int g, int b, int font_size);
	void say_stop(string text, int x, int y, int r, int g, int b, int font_size);
	
	bool CollidesRect(Sprite* spr);
	bool CollidesWith(Sprite* spr);
	
	void Move(float newPosX, float newPosY, float newPosHBX, float newPosHBY);
	
	void SetHardbox(float x, float y, int w, int h);
	
	Sprite* GetHardBoxSpr() { return m_hardBoxSpr; }
	
	void ClearAnimationList();
	void AddAnimationSequence(Animation2D* sequence);
	void SetSequence(int i);
	void NullSequence() { m_CurrentSequence = NULL; ClearAnimationList(); }
	Animation2D* CurrentSequenceExists() { return m_CurrentSequence; }
	int GetCurrentFrameIndex() { if (m_CurrentSequence != NULL) return m_CurrentSequence->GetCurrentFrameIndex(); else return -1; }
	bool IsCurrentSequenceOnLastFrame() { if (m_CurrentSequence != NULL) return m_CurrentSequence->IsOnLastFrame(); else return false; }
	void SetCurrentSequenceNotOnLastFrame() { if (m_CurrentSequence != NULL) m_CurrentSequence->SetNotOnLastFrame(); }
	
	void CreateHardBoxSpr(string editor_base);
	void GetHardbox(float& x, float& y, int& w, int& h);
	
	//LuaSpriteScript* GetSpriteScript() { return m_spriteScript; }
	Script* GetScript() { return m_script; }
	
	void AddScript(string name);

	bool MoveSpriteUp() { return m_move_up; }
	bool MoveSpriteDown() { return m_move_down; }
	bool MoveSpriteLeft() { return m_move_left; }
	bool MoveSpriteRight() { return m_move_right; }

	void SetMoveSpriteUp(bool up) { m_move_up = up; bMovingUp = up; }
	void SetMoveSpriteDown(bool down) { m_move_down = down; bMovingDown = down; }
	void SetMoveSpriteLeft(bool left) { m_move_left = left; bMovingLeft = left; }
	void SetMoveSpriteRight(bool right) { m_move_right = right; bMovingRight = right; }

	// Sets the section the sprite is in
	void SetSectionIn(Section* in) { m_SectionIn = in; }

	// Returns the section the sprite in in
	Section* SectionIn() { return m_SectionIn; }

	void InitFOWTiles(int radius);
	void AddFOWTile(Tile* tile) { m_FOWTiles.push_back(tile); }
	Tile* GetFOWTile(int i) { return m_FOWTiles[i]; }
	int FOWTileSize() { return m_FOWTiles.size(); }

	Sprite* Clone(GameDirectory2D* gd);
	void CloneMe(GameDirectory2D* gd, float x, float y, int w, int h);

	void AddAnimation();

	//--------------------------------------------------------------------
	//Attribute assignment
	//--------------------------------------------------------------------
	
	void SetCollidedSprite(Sprite* spr) { m_collided_with_sprite = spr; }
	Sprite* GetCollidedSprite() { return m_collided_with_sprite; }
	
	int& OldSpeed() { return m_old_speed; }
	int OldSpeed() const { return m_old_speed; }

	bool& CanDisplayHardbox() { return m_displayHardbox; }
	bool CanDisplayHardbox() const { return m_displayHardbox; }
	
	//XMFLOAT3& Position() { return position; }
	//XMFLOAT3 Position() const { return position; }
	
	int& Width() { return m_width; }
	int Width() const { return m_width; }
	
	int& Height() { return m_height; }
	int Height() const { return m_height; }
	
	int& DirectionMoved() { return m_DirectionMoved; }
	int DirectionMoved() const { return m_DirectionMoved; }
	
	bool& Attacking() { return mAttacking; }
	bool Attacking() const { return mAttacking; }
	
	bool& IsPlayer() { return m_player; }
	bool IsPlayer() const { return m_player; }
	
	bool& IsDisplaySprite() { return m_displaySprite; }
	bool IsDisplaySprite() const { return m_displaySprite; }
	
	int& ID() { return m_id; }
	int ID() const { return m_id; }
	
	float& X() { return m_TopLeftPosition.x; }//m_TopLeftPosition.x; }
	float X() const { return m_TopLeftPosition.x; }// m_TopLeftPosition.x; }
	
	float& Y() { return m_TopLeftPosition.y; }// m_TopLeftPosition.y; }
	float Y() const { return m_TopLeftPosition.y; } //m_TopLeftPosition.y; }
	
	int& Vision() { return m_vision; }
	int Vision() const { return m_vision; }
	
	int& Depth() { return m_depth; }
	int Depth() const { return m_depth; }
	
	string& BaseTextureFilename() { return m_baseTextureFilename; }
	string BaseTextureFilename() const { return m_baseTextureFilename; }
	
	int& IsHard() { return m_hard; }
	int IsHard() const { return m_hard; }
	
	string& AnimationProfileName() { return m_animationProfileName; }
	string AnimationProfileName() const { return m_animationProfileName; }
	
	int& HP() { return m_hp; }
	int HP() const { return m_hp; }
	
	int& HPMax() { return m_hp_max; }
	int HPMax() const { return m_hp_max; }
	
	bool& IsStatic() { return m_static; }
	bool IsStatic() const { return m_static; }
	
	int& Gold() { return m_gold; }
	int Gold() const { return m_gold; }

	int& Coin() { return m_coin; }
	int Coin() const { return m_coin; }
	
	int& Strength() { return m_strength; }
	int Strength() const { return m_strength; }
	
	float& Speed() { return m_speed; }
	float Speed() const { return m_speed; }
	
	int& Mana() { return m_mana; }
	int Mana() const { return m_mana; }
	
	int& Defense() { return m_defense; }
	int Defense() const { return m_defense; }
	
	int& Experience() { return m_experience; }
	int Experience() const { return m_experience; }
	
	int& Level() { return m_level; }
	int Level() const { return m_level; }
	
	bool& IsNoHit() { return m_nohit; }
	bool IsNoHit() const { return m_nohit; }
	
	bool& IsTouch() { return m_touch; }
	bool IsTouch() const { return m_touch; }
	
	bool& IsTouched() { return m_touched; }
	bool IsTouched() const { return m_touched; }
	
	string& ScriptName() { return m_script_name; }
	string ScriptName() const { return m_script_name; }
	
	bool& CanTeleport() { return m_can_teleport; }
	bool CanTeleport() const { return m_can_teleport; }
	
	string& TeleportMapName() { return m_teleport_map; }
	string TeleportMapName() const { return m_teleport_map; }
	
	int& TeleportX() { return m_teleport_x; }
	int TeleportX() const { return m_teleport_x; }
	
	int& TeleportY() { return m_teleport_y; }
	int TeleportY() const { return m_teleport_y; }
	
	bool& IsBackgroundSprite() { return m_background; }
	bool IsBackgroundSprite() const { return m_background; }
	
	bool& IsEntitySprite() { return m_entity; }
	bool IsEntitySprite() const { return m_entity; }
	
	bool& IsInvisibleSprite() { return m_invisible; }
	bool IsInvisibleSprite() const { return m_invisible; }
	
	int& DefaultSequence() { return default_sequence; }
	int DefaultSequence() const { return default_sequence; }
	
	int& AnimationSpeed() { return m_animationSpeed; }
	int AnimationSpeed() const { return m_animationSpeed; }
	
	int& DamageAmount() { return m_damageAmount; }
	int DamageAmount() const { return m_damageAmount; }
	
	string& Name() { return m_name; }
	string Name() const { return m_name; }
	
	bool& Moving() { return m_moving; }
	bool Moving() const { return m_moving; }
	
	bool& Thinking() { return m_thinking; }
	bool Thinking() const { return m_thinking; }
	
	bool& Talking() { return m_talking; }
	bool Talking() const { return m_talking; }
	
	bool& IsDying() { return m_dying; }
	bool IsDying() const { return m_dying; }
	
	bool& IsHit() { return m_hit; }
	bool IsHit() const { return m_hit; }
	
	bool& Attacked() { return m_attacked; }
	bool Attacked() const { return m_attacked; }
	
	bool& Targeted() { return m_targeted; }
	bool Targeted() const { return m_targeted; }
	
	string& CollideMsg() { return m_collide_msg; }
	string CollideMsg() const { return m_collide_msg; }
	
	bool& PlatformerSprite() { return m_platformerSprite; }
	bool PlatformerSprite() const { return m_platformerSprite; }
	//--------------------------------------------------------------------
	
	//XMFLOAT3& TopLeftPosition() { return m_TopLeftPosition; }
	//XMFLOAT3 TopLeftPosition() const { return m_TopLeftPosition; }

	// Special Methods
	
	//bool m_HasWorldCollision() const { return true; }
	//bool m_ApplyGravity() const { return true; }
	//bool m_ApplyFriction() const { return !bTryToMove; }
	
	void MakeTemporarilyInvincible() { iHurtTimer = kHurtFrames; }
	
	bool IsHurt() { return iHurtTimer != 0; }
	
	XMMATRIX& SpriteWorld() { return mWorld; }
	XMMATRIX SpriteWorld() const { return mWorld; }
	
protected:
	D3D* m_D3DSystem;
	EngineOptions* m_EngineOptions;

	ZShadeSandboxGraphics::Text m_text;
	XMMATRIX m_text_transform;
	XMFLOAT4 m_text_color;
	bool m_display_text;
	
	SleepTimer* m_text_timer;
	int m_old_speed;
	
	XMMATRIX mWorld;
	
	//is it static or dynamic
	bool m_static;

	string m_collide_msg;

	int m_id;

	//XMFLOAT3 m_TopLeftPosition;
	//int iBodyWidth;
	//int iBodyHeight;

	//XMFLOAT3 position;
	
	int m_width;
	int m_height;

	//Depth que is based on sorting the y position of the sprites on the map if it has a default depth of 0
	//otherwise use assigned sprite depth (range 1 - 2000)(default 0)

	//Does the sprite exist
	bool m_exists;

	bool m_move_up;
	bool m_move_down;
	bool m_move_left;
	bool m_move_right;

	Sprite* m_collided_with_sprite;

	//--------------------------------------------------------------------
	//Sprite Attributes
	//--------------------------------------------------------------------
	//The vision of this sprite
	int m_vision;

	//The depth of this sprite
	int m_depth;

	//Base texture of the sprite
	string m_baseTextureFilename;
	
	bool m_platformerSprite;
	
	//Hardness of the sprite - 1 for hard, 0 for not hard
	int m_hard;

	string m_animationProfileName;

	int m_hp;
	int m_hp_max;
	int m_mana;
	int m_gold;
	int m_coin;
	int m_strength;
	float m_speed;
	int m_defense;
	int m_experience;
	int m_level;
	int m_animationSpeed;
	int m_damageAmount;
	bool m_nohit;
	bool m_background;
	bool m_entity;
	bool m_invisible;
	int default_sequence;
	
	//Is this a touchable sprite
	bool m_touch;
	
	//The script the sprite will use
	string m_script_name;
	
	string m_name;
	bool m_moving;
	bool m_thinking;
	bool m_talking;
	bool m_dying;
	bool m_hit;
	//Was this sprite touched
	bool m_touched;
	bool m_attacked;
	bool m_targeted;

	//The sprite script for the sprite
	//LuaSpriteScript* m_spriteScript;
	Script* m_script;

	//Teleport
	bool m_can_teleport;
	string m_teleport_map;
	int m_teleport_x;
	int m_teleport_y;

	struct HARD_BOX
	{
		float m_x;
		float m_y;
		int width;
		int height;
	};

	//Do not need a poly just need custom rectangles with SAT for collision
	//The base hard box the sprite will use
	HARD_BOX m_hard_box;
	//--------------------------------------------------------------------

	Sprite* m_hardBoxSpr;

	bool m_displayHardbox;
	bool m_displaySprite;
	bool m_player;

	//The current rendered bitmap for the sprite
	ZShadeSandboxMesh::QuadMesh* m_bitmap;

	int m_DirectionMoved;
	bool mAttacking;

	Animation2D* m_CurrentSequence;

	//The animation sequences for the sprite
	vector<Animation2D*> mAnimationSequences;

	// The section the sprite is in
	Section* m_SectionIn;

	// 2D array of tiles for the FOW
	//SmartArray2D<Tile*> m_FOWTiles;

	// FOW Circle with radius of 3
	//Tile* m_FOWTiles[7][7];
	std::vector<Tile*> m_FOWTiles;
	
	// Special Attributes
	
	// How high can the sprite jump
	float kPlatformerJumpVelocity;// = 900.0f * 1.2f;
	
	// Sprite flashes for 120 frames if hurt
	const static int kHurtFrames = 120;
	const static int kEnemyHurtFrames = 10;
	
	// Controls how fast the sprites velocity moves towards the target velocity
	// 1 = in one frame and 0 = never
	float kReachTargetScale;// = 0.7f;
	
	// How fast can the sprite walk
	const static int kWalkSpeed = 80;
	
	int iHurtTimer;
	
	bool bVisible;
	bool bTryToMove;
	bool bFlyMode;
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__ZSPRITE_H