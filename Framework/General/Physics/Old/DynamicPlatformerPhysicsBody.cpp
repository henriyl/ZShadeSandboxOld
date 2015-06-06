#include "DynamicPlatformerPhysicsBody.h"
//==================================================================================================================================
//==================================================================================================================================
DynamicPlatformerPhysicsBody::DynamicPlatformerPhysicsBody(D3D* d3d, int body_width, int body_height)
:  PlatformerPhysicsBody(d3d, body_width, body_height)
{
	// Set acceleration and speed
	float mScale = 60.f;
	
	fAccX = 0.2f * mScale;
	fDecX = 0.3f * mScale;
	fMaxSpeedX = 5.0f * mScale;
	fMaxSpeedY = 10.0f * mScale;
	
	fSpeedX = 0.f;
	fSpeedY = 0.f;
	
	// Set jump and gravity forces
	fJumpStartSpeedY = 8.f * mScale;
	fAccY = 0.5f * mScale;
	
	bJumping = false;
	bJumpKeyDown = false;
}
//===============================================================================================================================
void DynamicPlatformerPhysicsBody::UpdateMovement(DWORD tickCount)
{
	// When collisions are detected they get solved before the body moves
	if (!tickCount) tickCount = GetTickCount();
	
	int updateTick = tickCount;//GetTickCount();
	
	// Update the position
	m_TopLeftPosition.x += LinearMovement(fSpeedX, updateTick);
	m_TopLeftPosition.y += LinearMovement(fSpeedY, updateTick);
	
	// Allows the body to not fall through the floor when gravity is applied
	if (bPreventFalling)
	{
		if (m_TopLeftPosition.y > m_EngineOptions->m_screenHeight - iBodyHeight)
		{
			m_TopLeftPosition.y = m_EngineOptions->m_screenHeight - iBodyHeight;
			bJumping = false;
		}
	}
	
	bool bMoveRequest = false;
	
	if (bMoveLeft)
	{
		fSpeedX -= fAccX;
		bMoveRequest = true;
	}
	
	if (bMoveRight)
	{
		fSpeedX += fAccX;
		bMoveRequest = true;
	}
	
	if (bPlayer)
	{
		// Jump if not already jumping and the jump key was released earlier
		if (GetAsyncKeyState(' ') && !bJumping && !bJumpKeyDown)
		{
			bJumping = true;
			bJumpKeyDown = true;
			fSpeedY = -fJumpStartSpeedY;
		}
		
		// Jump key was released
		if (!GetAsyncKeyState(' '))
		{
			bJumpKeyDown = false;
		}
	}
	else// Game Entity might want to jump
	{
		
	}
	
	// Limit the sideways acceleration of the body
	if (fSpeedX > fMaxSpeedX)
		fSpeedX = fMaxSpeedX;
	
	if (fSpeedX < -fMaxSpeedX)
		fSpeedX = -fMaxSpeedX;
	
	// Limit the force of gravity (terminal velocity) if (speedY > maxSpeedY) speedY = maxSpeedY;
	if (fSpeedY < -fMaxSpeedY)
		fSpeedY = -fMaxSpeedY;
	
	// Apply the force of gravity
	fSpeedY += fAccY;
	
	// Decelerate the body's sideways movement if left or right movement did not happen
	if (!bMoveRequest)
	{
		if (fSpeedX < 0)
			fSpeedX += fDecX;
		
		if (fSpeedX > 0)
			fSpeedX -= fDecX;
		
		// Deceleration may produce a speed that is greater than zero but
		// smaller than the smallest unit of deceleration. These lines
		// ensure that the player does not keep travelling at slow speed
		// forever after decelerating.
		
		if (fSpeedX > 0 && fSpeedX < fDecX)
			fSpeedX = 0;
		
		if (fSpeedX < 0 && fSpeedX > -fDecX)
			fSpeedX = 0;
	}
}
//===============================================================================================================================
float DynamicPlatformerPhysicsBody::LinearMovement(float pixelsPerSecond, DWORD tickCount)
{
	if (!tickCount) tickCount = GetTickCount();
	
	DWORD LastUpdateTime = tickCount;

	//LastUpdateTime is set to GetTickCount() every time the application receives the WM_PAINT message
	//in the Windows message pump, indicating the start of rendering of the next frame.
	float secs_elapsed = static_cast<float>(tickCount - LastUpdateTime) / 1000.0f;
    return secs_elapsed * pixelsPerSecond;
}
//===============================================================================================================================
bool DynamicPlatformerPhysicsBody::Collides(PhysicsBody* body, DWORD tickCount)
{
	if (!tickCount) tickCount = GetTickCount();
	
	// Maximum number of iterations for contact solver to run per frame
	const int iterations = 3;
	
	// What kind of collision has occurred
	bool contactX = true;
	bool contactYBottom = true;
	bool contactYTop = true;
	
	// Iterate the contact solver until the maximum number of iterations is reached
	// or no collisions are found
	for (int iteration = 0; iteration < iterations && (contactX || contactYBottom || contactYTop); iteration++)
	{
		// Calculate the amount of X and Y movement expected by the player this frame
		float nextMoveX = LinearMovement(fSpeedX, tickCount);
		float nextMoveY = LinearMovement(fSpeedY, tickCount);
		
		// No collisions found yet
		contactX = contactYBottom = contactYTop = false;
		
		float projectedMoveX;
		float projectedMoveY;
		
		float originalMoveX;
		float originalMoveY;
		
		// Store the original final expected movement of the body so we can see if iter_swap
		// has been modified due to a collision.
		originalMoveX = nextMoveX;
		originalMoveY = nextMoveY;
		
		
	}
	
	return false;
}
//===============================================================================================================================