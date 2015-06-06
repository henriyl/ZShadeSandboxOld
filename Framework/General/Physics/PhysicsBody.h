//==================================================================================================================================
// PhysicsBody.h
//
//==================================================================================================================================
// History
//
// -Created on 1/21/2015 by DW
// -Modified on 1/28/2015 by DW Changed name to PhysicsBody
//==================================================================================================================================
#ifndef __PHYSICSBODY_H
#define __PHYSICSBODY_H
//==================================================================================================================================
//==================================================================================================================================
#include "PhysicsBodyBase.h"
#include "ContactStats.h"
//#include "ContactManifold.h"
//class ContactManifold;
//class PhysicsBodyBase;
//==================================================================================================================================
//class Map;
//class PlatformerMap;
//==================================================================================================================================
class PhysicsBody : public PhysicsBodyBase
{
protected:
	
	//
	// 3D Physics Section
	//



	//
	// 2D Physics Section
	//

	//Map* m_TopdownMap;
	//PlatformerMap* m_PlatformerMap;
	
	//ContactManifold m_ContactManifold;
	
	// The amount of X acceleration to apply when the player moves left or right
	// The amount of X deceleration to apply when the player does not move left or right
	float accX, decX;

	// The amount of X and Y movement to occur on the next frame
	float speedX, speedY;

	// The maximum movement speed allowed for the player
	float maxSpeedX, maxSpeedY;

	// The amount of upward force to apply when the player first presses jump
	float jumpStartSpeedY;

	// The amount of Y acceleration to apply on each frame due to the force of gravity
	float accY;
	
	// True if currently jumping (prevents the player jumping while in mid-air)
	bool jumping;

	// True if the jump key is currently held down (key must be released to allow a new jump)
	bool jumpKeyDown;
	
	// Linear movement variables
	bool bMovingLeft;
	bool bMovingRight;
	bool bMovingUp;
	bool bMovingDown;

	// Maps any key to this bool to see if a jump happened
	bool bJumpKey;
	
	bool bGravityOn;
	
	bool bJumpEnabled;
	bool bFrictionEnabled;
	
	// The amount of damage done to another body on collision
	int iDamageToOther;
	
	// Sides from which the body is vulnerable to being attacked (collided) by another body
	bool bVulnerableFromLeft;
	bool bVulnerableFromRight;
	bool bVulnerableFromAbove;
	bool bVulnerableFromBelow;
	
	// Is this body the player
	bool bPlayer;
	
public:
	
	PhysicsBody(D3D* d3d);
	
	//
	// 3D Physics Section
	//
	
	// Does this physics body collide with another physics body using GJK
	bool CollidesGJK(PhysicsBody* body);
	
	//
	// 2D Physics Section
	//

	void Initialize(XMFLOAT3 pos, PhysicsType pt, int body_width, int body_height);
	
	void InitializeParametersTopdown();
	void InitializeParametersPlatformer();
	void InitializeBaseParameters();
	
	// First parameter is a collection of tiles in the map
	// Second parameter is a collection of the bound bodies the physics body is in contact with
	void CollideTiles(vector<PhysicsBody*> tileBodies, vector<int> &boundBodies, ContactStats* cs, float dt);
	
	// Checks to see if any point of this physics body touches the other physics body
	bool CollidesBody(PhysicsBody* body, float dt);
	
	// Need a function to collide with another dynamic body
	// Need Ladder and Platform collision
	
	// Movement must be done after the collision contact solver
	void Move(float dt);
	
	// Override this method in the main class for collision that happens to it. (If it takes damage)
	virtual void OnCollision(bool bTop, bool bBottom, bool bLeft, bool bRight, int damageAmount) {}
	
	// Accessor methods
public:
	
	//
	// 3D Physics Section
	//



	//
	// 2D Physics Section
	//
	
	bool& JumpEnabled() { return bJumpEnabled; }
	bool JumpEnabled() const { return bJumpEnabled; }
	
	bool& FrictionEnabled() { return bFrictionEnabled; }
	bool FrictionEnabled() const { return bFrictionEnabled; }
	
	bool& IsGravityOn() { return bGravityOn; }
	bool IsGravityOn() const { return bGravityOn; }
	
	bool& JumpKey() { return bJumpKey; }
	bool JumpKey() const { return bJumpKey; }

	bool& MovingLeft() { return bMovingLeft; }
	bool MovingLeft() const { return bMovingLeft; }

	bool& MovingRight() { return bMovingRight; }
	bool MovingRight() const { return bMovingRight; }

	bool& MovingUp() { return bMovingUp; }
	bool MovingUp() const { return bMovingUp; }

	bool& MovingDown() { return bMovingDown; }
	bool MovingDown() const { return bMovingDown; }

	float& AccX() { return accX; }
	float AccX() const { return accX; }
	
	float& AccY() { return accY; }
	float AccY() const { return accY; }
	
	float& SpeedX() { return speedX; }
	float SpeedX() const { return speedX; }
	
	float& SpeedY() { return speedY; }
	float SpeedY() const { return speedY; }
	
	float& MaxSpeedX() { return maxSpeedX; }
	float MaxSpeedX() const { return maxSpeedX; }
	
	float& MaxSpeedY() { return maxSpeedY; }
	float MaxSpeedY() const { return maxSpeedY; }
	
	float& JumpStartSpeedY() { return jumpStartSpeedY; }
	float JumpStartSpeedY() const { return jumpStartSpeedY; }
	
	bool& IsJumping() { return jumping; }
	bool IsJumping() const { return jumping; }
	
	bool& IsJumpKeyDown() { return jumpKeyDown; }
	bool IsJumpKeyDown() const { return jumpKeyDown; }
	
	int& DamageToOther() { return iDamageToOther; }
	int DamageToOther() const { return iDamageToOther; }
	
	bool& VulnerableFromLeft() { return bVulnerableFromLeft; }
	bool VulnerableFromLeft() const { return bVulnerableFromLeft; }
	
	bool& VulnerableFromRight() { return bVulnerableFromRight; }
	bool VulnerableFromRight() const { return bVulnerableFromRight; }
	
	bool& VulnerableFromAbove() { return bVulnerableFromAbove; }
	bool VulnerableFromAbove() const { return bVulnerableFromAbove; }
	
	bool& VulnerableFromBelow() { return bVulnerableFromBelow; }
	bool VulnerableFromBelow() const { return bVulnerableFromBelow; }
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__PHYSICSBODY_H