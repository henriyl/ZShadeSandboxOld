//******************************************************************************************************
// Tile.h
//******************************************************************************************************
#ifndef __TILE_H
#define __TILE_H
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//Includes
#include "QuadMesh.h"
//#include "TextureShader.h"
//#include "TransparentShader.h"
#include "Sprite.h"
#include "D3D.h"
#include "Animation2D.h"
#include "AnimationProfile2D.h"
#include "SectionAdjacency.h"
#include "PhysicsBody.h"
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
class Tile : public PhysicsBody
{
	//Tile Attributes
	int mSize;
	ZShadeSandboxMesh::QuadMesh *mTexture;
	ZShadeSandboxMesh::QuadMesh *mTextureCopy;
	//XMFLOAT3 mPosition;

	D3D* m_D3DSystem;
	D3D_VERSION m_d3dvers;
	EngineOptions* m_engineOptions;

	bool mHard;
	
	// Tiles world
	XMMATRIX mWorld;

	string m_animationProfileName;

	Animation2D* m_CurrentSequence;
	//The animation sequences for the sprite
	vector<Animation2D*> mAnimationSequences;

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

	Sprite* m_hardBoxSpr;

	bool m_displayHardbox;

	int m_animationSpeed;

	//New fog bit - No Fog, All Fog, In Fog Circle
	unsigned short m_fogBit;

	bool m_bFOW;
	int m_iFOWRadius;
	bool m_bFOWFlashlight;

public:
	Tile(D3D* d3d);
	~Tile();

	void EnableFOW(bool bEnable) { m_bFOW = bEnable; }
	bool IsFOWEnabled() { return m_bFOW; }

	void SetFOWRadius(int bRadius) { m_iFOWRadius = bRadius; }
	int GetFOWRadius() { return m_iFOWRadius; }

	void EnableFOWFlashlight(bool bEnable) { m_bFOWFlashlight = bEnable; }
	bool IsFOWFlashlightEnabled() { return m_bFOWFlashlight; }

	bool Initialize(int size, bool hard, XMFLOAT3 pos);
	
	bool InitializeTexture(string windowName, string base_tile_path, string textureFilename, int bitmapWidth, int bitmapHeight);

	void Shutdown();

	ZShadeSandboxMesh::QuadMesh* GetBitmap() { return mTexture; }
	
	void DisableTexture() { if (mTexture != NULL) delete mTexture; mTexture = NULL; }

	void UpdateFogBit(unsigned short bit);
	unsigned short GetFogBit();

	string GetTileTextureName();
	string GetTileTextureCopyName();

	void SetHard(bool hard) { mHard = hard; }

	bool IsHard() { return mHard; }
	int GetSize() { return mSize; }
	int GetWidth() { return m_engineOptions->TILE_SIZE; }
	int GetHeight() { return m_engineOptions->TILE_SIZE; }

	void SetAnimationSpeed(int speed) { m_animationSpeed = speed; }
	int GetAnimationSpeed() { return m_animationSpeed; }

	void SetDisplayHardbox(bool display) { m_displayHardbox = display; }
	bool CanDisplayHardbox() { return m_displayHardbox; }

	void CreateHardBoxSpr(string editor_base);

	void SetAnimationProfileName(string name) { m_animationProfileName = name; }
	string GetAnimationProfileName() { return m_animationProfileName; }

	void ClearAnimationList() { if (mAnimationSequences.size() > 0) mAnimationSequences.clear(); }

	Animation2D* CurrentSequenceExists() { return m_CurrentSequence; }

	int GetCurrentFrameIndex() { if (m_CurrentSequence != NULL) return m_CurrentSequence->GetCurrentFrameIndex(); else return -1; }
	bool IsCurrentSequenceOnLastFrame() { if (m_CurrentSequence != NULL) return m_CurrentSequence->IsOnLastFrame(); else return false; }
	void SetCurrentSequenceNotOnLastFrame() { if (m_CurrentSequence != NULL) m_CurrentSequence->SetNotOnLastFrame(); }

	void UpdateAnimation();

	void AddAnimationSequence(Animation2D* sequence);

	void SetSequence(int i);

	void NullSequence() { m_CurrentSequence = NULL; ClearAnimationList(); }

	//If a fog bit is present the texture will be updated
	//No Fog, All Fog, In Fog Circle
	void UpdateTexture();

	// Render non-transparent texture
	bool Render(Camera* camera);
	
	// Render transparent texture
	bool Render(Camera* camera, float blendAmount);
	
	// Render non-transparent animation
	bool RenderAnimation(Camera* camera);
	
	// Render transparent animation
	bool RenderAnimation(Camera* camera, float blendAmount);

	Tile* Clone(GameDirectory2D* gd);

	RECT GetBox();

	void UpdatePosition(XMFLOAT3 pos);
	XMFLOAT3 GetPosition();
};
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
#endif//__TILE_H