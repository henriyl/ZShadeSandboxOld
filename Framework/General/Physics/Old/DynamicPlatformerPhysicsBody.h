//==================================================================================================================================
// DynamicPlatformerPhysicsBody.h
//
// Physics Body that can move
//==================================================================================================================================
// History
//
// -Created on 1/21/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __DYNAMICPLATFORMERPHYSICSBODY_H
#define __DYNAMICPLATFORMERPHYSICSBODY_H
//==================================================================================================================================
//==================================================================================================================================
#include "PlatformerPhysicsBody.h"
//==================================================================================================================================
class DynamicPlatformerPhysicsBody : public PlatformerPhysicsBody
{
	// Amount of X acceleration to apply when the body moves left or right
	float fAccX;
	
	// Amount of X deceleration to apply when the body moves left or right
	float fDecX;
	
	// Amount of X movement to occur on the next frame
	float fSpeedX;
	
	// Amount of Y movement to occur on the next frame
	float fSpeedY;
	
	// Maximum X Movement speed allowed for the body
	float fMaxSpeedX;
	
	// Maximum Y Movement speed allowed for the body
	float fMaxSpeedY;
	
	// Amount of upward force to apply when the body jumps in the air
	float fJumpStartSpeedY;
	
	// Amount of Y acceleration to apply each frame due to the force of gravity
	float fAccY;
	
	// True if the body jumps so it is not jumping in mid-air
	bool bJumping;
	
	// True if the jump key is held down (Key must be released to jump again)
	bool bJumpKeyDown;
	
	// True if the body is moving left
	bool bMoveLeft;
	
	// True if the body is moving right
	bool bMoveRight;
	
	// True if the body is the player
	bool bPlayer;
	
	// True for debugging
	bool bPreventFalling;
	
public:
	
	DynamicPlatformerPhysicsBody(D3D* d3d, int body_width, int body_height);
	
	void Clone(DynamicPlatformerPhysicsBody* c) {}
	
	void UpdateMovement(DWORD tickCount);
	float LinearMovement(float pixelsPerSecond, DWORD tickCount);
	bool Collides(PhysicsBody* body, DWORD tickCount);
	
	// Accessor methods
public:
	
	float& AccelerationX() { return fAccX; }
	float AccelerationX() const { return fAccX; }
	
	float& DecelerationX() { return fDecX; }
	float DecelerationX() const { return fDecX; }
	
	float& SpeedX() { return fSpeedX; }
	float SpeedX() const { return fSpeedX; }
	
	float& SpeedY() { return fSpeedY; }
	float SpeedY() const { return fSpeedY; }
	
	float& MaxSpeedX() { return fMaxSpeedX; }
	float MaxSpeedX() const { return fMaxSpeedX; }
	
	float& MaxSpeedY() { return fMaxSpeedY; }
	float MaxSpeedY() const { return fMaxSpeedY; }
	
	float& JumpStartSpeedY() { return fJumpStartSpeedY; }
	float JumpStartSpeedY() const { return fJumpStartSpeedY; }
	
	float& AccelerationY() { return fAccY; }
	float AccelerationY() const { return fAccY; }
	
	bool& IsJumping() { return bJumping; }
	bool IsJumping() const { return bJumping; }
	
	bool& IsJumpKeyDown() { return bJumpKeyDown; }
	bool IsJumpKeyDown() const { return bJumpKeyDown; }
	
	bool& IsMovingLeft() { return bMoveLeft; }
	bool IsMovingLeft() const { return bMoveLeft; }
	
	bool& IsMovingRight() { return bMoveRight; }
	bool IsMovingRight() const { return bMoveRight; }
	
	bool& IsPlayer() { return bPlayer; }
	bool IsPlayer() const { return bPlayer; }
	
	bool& CanPreventFalling() { return bPreventFalling; }
	bool CanPreventFalling() const { return bPreventFalling; }
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__DYNAMICPLATFORMERPHYSICSBODY_H