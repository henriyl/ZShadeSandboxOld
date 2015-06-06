//==================================================================================================================================
// PlatformerTile.h
//
//==================================================================================================================================
// History
//
// -Created on 1/15/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __PLATFORMERTILE_H
#define __PLATFORMERTILE_H
//==================================================================================================================================
//==================================================================================================================================
#include "D3D.h"
#include "ZMath.h"
#include "QuadMesh.h"
//#include "TextureShader.h"
//#include "TransparentShader.h"
#include "Sprite.h"
#include "Animation2D.h"
#include "AnimationProfile2D.h"
#include "PhysicsBody.h"
class Sprite;
//==================================================================================================================================
class PlatformerTile : public PhysicsBody
{
	D3D* m_D3DSystem;
	EngineOptions* m_EngineOptions;

public:
	
	PlatformerTile(D3D* d3d);
	~PlatformerTile();

	void Initialize(int size, bool hard, XMFLOAT3 pos);
	
	bool InitializeTexture(string windowName, string base_tile_path, string textureFilename, int bitmapWidth, int bitmapHeight);
	
	void Shutdown();
	
	RECT GetBox();

	void UpdatePosition(XMFLOAT3 pos);
	XMFLOAT3 GetPosition();
	
	ZShadeSandboxMesh::QuadMesh* GetBitmap() { return mTexture; }
	
	void DisableTexture();
	
	string GetTileTextureName();
	string GetTileTextureCopyName();

	void SetHard(bool hard) { mHard = hard; }

	bool IsHard() { return mHard; }
	int GetSize() { return mSize; }
	int GetWidth() { return m_EngineOptions->TILE_SIZE; }
	int GetHeight() { return m_EngineOptions->TILE_SIZE; }

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

	void RotateTile();

	// Render non-transparent texture
	bool Render(Camera* camera);
	
	// Render transparent texture
	bool Render(Camera* camera, float blendAmount);
	
	// Render non-transparent animation
	bool RenderAnimation(Camera* camera);
	
	// Render transparent animation
	bool RenderAnimation(Camera* camera, float blendAmount);
	
	bool& CanRotate() { return bRotate; }
	bool CanRotate() const { return bRotate; }

	float& RotationAngle() { return fRotationAngle; }
	float RotationAngle() const { return fRotationAngle; }
	
	XMMATRIX& TileWorld() { return mWorld; }
	XMMATRIX TileWorld() const { return mWorld; }
	
	PlatformerTile* Clone(GameDirectory2D* gd);
	
protected:
	
	bool bUseSlope;
	int iSlopeDir;
	
	// Can this be a broken block
	bool bCanBreak;
	
	// Ignore flags used with collision
	bool bIgnoreLeft;
	bool bIgnoreRight;
	bool bIgnoreTop;
	bool bIgnoreBottom;
	
	// Can this tile rotate from the editor
	bool bRotate;
	float fRotationAngle;
	XMFLOAT3 mRotation;

	XMMATRIX mWorld;
	
	int mSize;
	ZShadeSandboxMesh::QuadMesh *mTexture;
	//XMFLOAT3 mPosition;
	bool mHard;
	
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
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__PLATFORMERTILE_H