#include "PhysicsBody.h"
#include "Forces.h"
#include "XMMath.h"
#include "ShapeContact.h"
#include "AABB.h"
#include "GJK.h"
//#include "Map.h"
//#include "PlatformerMap.h"
//==================================================================================================================================
//==================================================================================================================================
PhysicsBody::PhysicsBody(D3D* d3d)
:   PhysicsBodyBase(d3d)
{
}
//===============================================================================================================================
void PhysicsBody::InitializeParametersTopdown()
{
	// Set acceleration and speed
	float mScale = 60.f;
	
	// Set jump and gravity forces
	jumpStartSpeedY = 0;
	accY = 0;
	
	// acceleration of x and y are same since there is no gravity
	// and we are moving up, down, left or right
	accX = 0.2f * mScale;
	accY = 0.2f * mScale;
	decX = 0.3f * mScale;
	maxSpeedX = 5.0f * mScale;
	maxSpeedY = 10.0f * mScale;
	
	bGravityOn = false;
	
	bJumpEnabled = false;
	bFrictionEnabled = false;
}
//===============================================================================================================================
void PhysicsBody::InitializeParametersPlatformer()
{
	// Set acceleration and speed
	float mScale = 60.f;
	
	accX = 0.2f * mScale;
	decX = 0.3f * mScale;
	maxSpeedX = 5.0f * mScale;
	maxSpeedY = 10.0f * mScale;
	
	// Set jump and gravity forces
	jumpStartSpeedY = 8.f * mScale;
	accY = 0.5f * mScale;
	
	bGravityOn = true;
	
	bJumpEnabled = true;
	bFrictionEnabled = true;
}
//===============================================================================================================================
void PhysicsBody::InitializeBaseParameters()
{
	// Initialize the body to not move
	eType = STATIC;
	
	jumping = false;
	jumpKeyDown = false;
	bJumpKey = false;
	
	bMovingLeft = false;
	bMovingRight = false;
	bMovingUp = false;
	bMovingDown = false;
	
	speedX = 0.f;
	speedY = 0.f;
	
	iDamageToOther = 0;
	
	bVulnerableFromLeft = false;
	bVulnerableFromRight = false;
	bVulnerableFromAbove = false;
	bVulnerableFromBelow = false;
	
	bPlayer = false;
}
//===============================================================================================================================
void PhysicsBody::Initialize(XMFLOAT3 pos, PhysicsType pt, int body_width, int body_height)
{
	InitializeBaseParameters();
	
	m_TopLeftPosition = pos;
	
	eType = pt;
	
	iBodyWidth = body_width;
	iBodyHeight = body_height;
	
	InitCollisionPoints(iBodyWidth, iBodyHeight);
	
	// Generate a new body for static geometry
	GenerateBody();
}
//===============================================================================================================================
void PhysicsBody::Move(float dt)
{
	// A static body cannot move
	if (eType == STATIC) return;
	
	m_TopLeftPosition.x += speedX * dt;
	m_TopLeftPosition.y += speedY * dt;

	bool moveRequest = false;

	// Accelerate Leftwards
	if (bMovingLeft)
	{
		speedX -= accX;
		moveRequest = true;
	}

	// Accelerate Rightwards
	if (bMovingRight)
	{
		speedX += accX;
		moveRequest = true;
	}
	
	// If gravity is disabled this will accelerate the body at the request
	// of the user
	
	if (bMovingUp)
	{
		speedY -= accY;
	}
	
	if (bMovingDown)
	{
		speedY += accY;
	}
	
	// Jump if not already jumping and the jump key was released earlier
	if (JumpEnabled())
	{
		if (GetAsyncKeyState(' ') && !jumping && !jumpKeyDown)
		{
			jumping = true;
			jumpKeyDown = true;
			speedY = -jumpStartSpeedY;
		}

		// Jump key released
		if (!GetAsyncKeyState(' '))
			jumpKeyDown = false;
	}
	
	// Toggle display of geometry bounding boxes
	// ...

	//
	// Apply forces of gravity and friction
	//
	
	if (FrictionEnabled())
	{
		// Limit the sideways acceleration of the body
		
		if (speedX > maxSpeedX)
			speedX = maxSpeedX;

		if (speedX < -maxSpeedX)
			speedX = -maxSpeedX;
	}
	
	// Apply the force of gravity if it is turned on
	if (bGravityOn)
	{
		// Limit the force of gravity (terminal velocity)
		
		if (speedY > maxSpeedY)
			speedY = maxSpeedY;

		if (speedY < -maxSpeedY)
			speedY = -maxSpeedY;
		
		speedY += accY;
	}
	
	if (FrictionEnabled())
	{
		// Decelerate the body's sideways movement if left or right was not pressed
		if (!moveRequest)
		{
			if (speedX < 0)
				speedX += decX;

			if (speedX > 0)
				speedX -= decX;

			// Deceleration may produce a speed that is greater than zero but
			// smaller than the smallest unit of deceleration. These lines ensure
			// that the body does not keep travelling at slow speed forever after
			// decelerating.

			if (speedX > 0 && speedX < decX || speedX < 0 && speedX > -decX)
				speedX = 0;
		}
	}
	
	// Generate a new body based on movement
	GenerateBody();
}
//===============================================================================================================================
void PhysicsBody::CollideTiles(vector<PhysicsBody*> tileBodies, vector<int> &boundBodies, ContactStats* cs, float dt)
{
	// A static body cannot collide since it is not moving
	if (eType == STATIC) return;
	
	// What kind of collision has occured
	bool contactX = true, contactYBottom = true, contactYTop = true;
	
	static const int iterations = 3;
	
	// Keep interating the contact solver until the maximum number of iterations is reached
	// or no collisions are detected
	for (int iteration = 0; iteration < iterations && (contactX || contactYBottom || contactYTop); iteration++)
	{
		// Clear the list of bounding objects the physics body is in contact with
		if (boundBodies.size() > 0) boundBodies.clear();
		
		// Calculate the amount of X and Y movement expected by the physics body
		float nextMoveX = m_TopLeftPosition.x + speedX * dt;
		float nextMoveY = m_TopLeftPosition.y + speedY * dt;

		// Create to bounding body for this physics body's next move
		ZShadeSandboxMath::BoundingBox body_box = BodyBoundsPredict(XMFLOAT3(nextMoveX, nextMoveY, 0));

		// Check each tile to see if it intersects with the body bounds
		for (int o = 0; o < tileBodies.size(); o++)
		{
			// We don't need to check all four corners on each object and body by hand
			// Using a neat quirk of mathematics, we can take the combined bounding box
			// of the body and the object. If it's width or height exceeds the total
			// width or height of the object + the body, then the bounding boxes do
			// not intersect, otherwise they do.

			PhysicsBody* pb = tileBodies[o];

			if (pb == NULL) continue;
			
			if (body_box.Intersect(pb->Body()))
			{
				boundBodies.push_back(o);
				
				if (bPlayer) cs[iteration].boundingBoxes++;
			}

			/*BodyBox bb;

			bb.left = min(BBodyBox().left, pb->BBodyBox().left);
			bb.top = min(BBodyBox().top, pb->BBodyBox().top);
			bb.right = min(BBodyBox().right, pb->BBodyBox().right);
			bb.bottom = min(BBodyBox().bottom, pb->BBodyBox().bottom);

			// If a bounding box intersects with the player's bounding box, add the
			// geometry to the list of geometries to check for collisions
			if (bb.right - bb.left < pb->BBodyBox().right - pb->BBodyBox().left + BBodyBox().right - BBodyBox().left &&
				bb.bottom - bb.top < pb->BBodyBox().bottom - pb->BBodyBox().top + BBodyBox().bottom - BBodyBox().top)
			{
				boundBodies.push_back(o);
				
				if (bPlayer) cs[iteration].boundingBoxes++;
			}*/
		}

		// No collisions found yet
		contactX = contactYBottom = contactYTop = false;

		float projectedMoveX, projectedMoveY;
		float originalMoveX, originalMoveY;

		float vectorLength;

		int segments;

		// Store the original final expected movement of the player so we can
		// see if it has been modified due to a collision or potential collision later
		originalMoveX = nextMoveX;
		originalMoveY = nextMoveY;

		// Iterate over each object whose bounding box intersects with the player's bounding box
		// until a collision is found
		for (vector<int>::iterator it = boundBodies.begin(); it != boundBodies.end() && !contactX && !contactYBottom && !contactYTop; it++)
		{
			// ================================================================================
			// Speculative contacts section
			//
			// We will traverse along the movement vector of the body from its current
			// position until the final position for the frame to check if any geometry lies
			// in the way. If so, the vector is adjusted to end at the geometry's intersection
			// with the physics bodies movement vector. This solves the so-called 'bullet through
			// paper' problem.
			// ================================================================================

			if (tileBodies[(*it)] == NULL) continue;

			// We will test the four possible directions of the physics body movement individually
			// dir: 0 = top, 1 = bottom, 2 = left, 3 = right
			for (int dir = 0; dir < 4; dir++)
			{
				// Skip the test if the expected direction of movement makes the test irrelevant
				// For example, we only want to test the top of the body's head when movement is
				// upwards, not downwards. This is really important! If we don't do this, we can
				// get stuck in a ceiling when falling for example.

				if (dir == 0 && nextMoveY > 0) continue;
				if (dir == 1 && nextMoveY < 0) continue;
				if (dir == 2 && nextMoveX > 0) continue;
				if (dir == 3 && nextMoveX < 0) continue;

				// Our current position along the anticipated movement vector of the body this frame
				projectedMoveX = projectedMoveY = 0;

				// Calculate the length of the movement vector using Pythagoras Therom
				vectorLength = sqrt(SQR(nextMoveX) + SQR(nextMoveY));

				segments = 0;

				// Advance along the vector until it intersects with one of the tiles or we are done
				while (!tileBodies[(*it)]->Body().Inside(XMFLOAT3(CollisionPoint(dir * 2).x + m_TopLeftPosition.x + projectedMoveX, CollisionPoint(dir * 2).y + m_TopLeftPosition.y + projectedMoveY, 0))
					&& !tileBodies[(*it)]->Body().Inside(XMFLOAT3(CollisionPoint(dir * 2 + 1).x + m_TopLeftPosition.x + projectedMoveX, CollisionPoint(dir * 2 + 1).y + m_TopLeftPosition.y + projectedMoveY, 0))
					&& segments < vectorLength)
				{
					projectedMoveX += nextMoveX / vectorLength;
					projectedMoveY += nextMoveY / vectorLength;

					segments++;
					
					if (bPlayer) cs[iteration].speculativeContacts++;
				}

				// If we have a intersection
				if (segments < vectorLength)
				{
					// Apply correction for over-movement
					if (segments > 0)
					{
						projectedMoveX -= nextMoveX / vectorLength;
						projectedMoveY -= nextMoveY / vectorLength;
					}

					// Adjust the X or Y component of the vector depending on which
					// direction we are testing

					if (dir >= 2 && dir <= 3)
						nextMoveX = projectedMoveX;

					if (dir >= 0 && dir <= 1)
						nextMoveY = projectedMoveY;
				}
			}

			// ================================================================================
			// Penetration resolution
			//
			// Here we look for existing collisions and nudge the physics body in the opposite
			// direction until the collision is solved. The purpose of iteration is because
			// the correction may cause collisions with other pieces of geometry. Iteration
			// also solves the so-called 'jitter' problem where a collision between the
			// body and the geometry is constantly solved and then re-introduced as the
			// body's position is nudged backwards and forwards between the same two points
			// repeatedly, creating jitter in the body's movement.
			// ================================================================================

			// We will test the four possible directions of movement for the physics body
			// dir: 0 = top, 1 = bottom, 2 = left, 3 = right
			for (int dir = 0; dir < 4; dir++)
			{
				// Skip the test if the expected direction of movement makes the test irrelevant
				// For example, we only want to test the top of the body's head when movement is
				// upwards, not downwards. This is really important! If we don't do this, we can
				// make corrections in the wrong direction, causing the body to magically jump
				// up to platforms or stick to ceilings.

				if (dir == 0 && nextMoveY > 0) continue;
				if (dir == 1 && nextMoveY < 0) continue;
				if (dir == 2 && nextMoveX > 0) continue;
				if (dir == 3 && nextMoveX < 0) continue;

				projectedMoveX = (dir >= 2 ? nextMoveX : 0);
				projectedMoveY = (dir <  2 ? nextMoveY : 0);

				// Traverse backwards in X or Y (but not both at the same time)
				// until the body is no longer colliding with the geometry

				// Note: This code also enables walking up gently sloping surfaces:
				// as the force of gravity pulls down on the player and causes surface contact,
				// the correction pushes the body away from the inside of the platform up
				// to the surface. Without this, the body would get stuck in any sloping
				// platform.
				// This same code also prevents walking up platforms that are too steeply sloped.
				// The act of falling causes collisions with the left or right side of the
				// body, causing him/her to be nudged left or right and be free to continue
				// falling further. The maximum incline of the slope the body can walk up
				// is directly related to the selected gravitational force.

				// Advance along the vector until it intersects with one of the tiles or we are done
				while (!tileBodies[(*it)]->Body().Inside(XMFLOAT3(CollisionPoint(dir * 2).x + m_TopLeftPosition.x + projectedMoveX, CollisionPoint(dir * 2).y + m_TopLeftPosition.y + projectedMoveY, 0))
					&& !tileBodies[(*it)]->Body().Inside(XMFLOAT3(CollisionPoint(dir * 2 + 1).x + m_TopLeftPosition.x + projectedMoveX, CollisionPoint(dir * 2 + 1).y + m_TopLeftPosition.y + projectedMoveY, 0)))
				{
					if (dir == 0) projectedMoveY++;
					if (dir == 1) projectedMoveY--;
					if (dir == 2) projectedMoveX++;
					if (dir == 3) projectedMoveX--;
					
					if (bPlayer) cs[iteration].penetrationCorrections++;
				}

				if (dir >= 2 && dir <= 3)
					nextMoveX = projectedMoveX;

				if (dir >= 0 && dir <= 1)
					nextMoveY = projectedMoveY;
			}

			// Detect what type of contact has occured based on a comparison of the original
			// expected movement vector and the new one
			
			if (nextMoveY > originalMoveY && originalMoveY < 0)
				contactYTop = true;

			if (nextMoveY < originalMoveY && originalMoveY > 0)
				contactYBottom = true;

			if (abs(nextMoveX - originalMoveX) > 0.01f)
				contactX = true;

			// The body cannot continue jumping if we hit the side of something, must fall instead
			// Without this, a body hitting a wall during a jump will continue traveling upwards.
			if (contactX && speedY < 0)
				speedY = nextMoveY = 0;

			if (contactX || contactYTop || contactYBottom)
				if (bPlayer) cs[iteration].contacts++;
		}

		// If a contact has been detected, apply the re-calculated movement vector
		// and disable any further movement this frame (in either X or Y as appropriate)

		if (contactYBottom || contactYTop)
		{
			m_TopLeftPosition.y += nextMoveY;

			speedY = 0;

			// The body is not jumping if it is touching the ground
			if (contactYBottom)
				jumping = false;
		}

		if (contactX)
		{
			m_TopLeftPosition.x += nextMoveX;

			speedX = 0;
		}
	}
}
//===============================================================================================================================
bool PhysicsBody::CollidesBody(PhysicsBody* body, float dt)
{
	// A static body cannot collide since it is not moving
	if (eType == STATIC) return false;
	
	if (body == NULL) return false;
	
	// What direction did the collision come from
	bool bTop, bBottom, bRight, bLeft;
	
	// What kind of collision has occured
	bool contactX = true, contactYBottom = true, contactYTop = true;
	
	static const int iterations = 3;
	
	// Keep interating the contact solver until the maximum number of iterations is reached
	// or no collisions are detected
	for (int iteration = 0; iteration < iterations && (contactX || contactYBottom || contactYTop); iteration++)
	{
		// Calculate the amount of X and Y movement expected by the physics body
		float nextMoveX = m_TopLeftPosition.x + speedX * dt;
		float nextMoveY = m_TopLeftPosition.y + speedY * dt;

		// Create to bounding body for this physics body's next move
		ZShadeSandboxMath::BoundingBox body_box = BodyBoundsPredict(XMFLOAT3(nextMoveX, nextMoveY, 0));
		
		// Check to see if the two bodies are intersecting
		bool intersecting = false;
		
		// We don't need to check all four corners on each body and current body by hand
		// Using a neat quirk of mathematics, we can take the combined bounding box
		// of the body and the other body. If it's width or height exceeds the total
		// width or height of the other body + the current body, then the bounding boxes do
		// not intersect, otherwise they do.

		/*BodyBox bb;

		bb.left = min(BBodyBox().left, body->BBodyBox().left);
		bb.top = min(BBodyBox().top, body->BBodyBox().top);
		bb.right = min(BBodyBox().right, body->BBodyBox().right);
		bb.bottom = min(BBodyBox().bottom, body->BBodyBox().bottom);

		// If a bounding box intersects with the player's bounding box, add the
		// geometry to the list of geometries to check for collisions
		if (bb.right - bb.left < body->BBodyBox().right - body->BBodyBox().left + BBodyBox().right - BBodyBox().left &&
			bb.bottom - bb.top < body->BBodyBox().bottom - body->BBodyBox().top + BBodyBox().bottom - BBodyBox().top)
		{
			intersecting = true;
		}*/
		
		if (body_box.Intersect(body->Body()))
		{
			intersecting = true;
		}

		
		// No collisions found yet
		contactX = contactYBottom = contactYTop = false;

		float projectedMoveX, projectedMoveY;
		float originalMoveX, originalMoveY;

		float vectorLength;

		int segments;

		// Store the original final expected movement of the player so we can
		// see if it has been modified due to a collision or potential collision later
		originalMoveX = nextMoveX;
		originalMoveY = nextMoveY;
		
		// If the boxes are not intersecting then do nothing
		if (intersecting)
		{
			// Check Speculative contacts and see what side the collision is occuring
			
			// We will test the four possible directions of the physics body movement individually
			// dir: 0 = top, 1 = bottom, 2 = left, 3 = right
			for (int dir = 0; dir < 4; dir++)
			{
				// Skip the test if the expected direction of movement makes the test irrelevant
				// For example, we only want to test the top of the body's head when movement is
				// upwards, not downwards. This is really important! If we don't do this, we can
				// get stuck in a ceiling when falling for example.

				if (dir == 0 && nextMoveY > 0) continue;
				if (dir == 1 && nextMoveY < 0) continue;
				if (dir == 2 && nextMoveX > 0) continue;
				if (dir == 3 && nextMoveX < 0) continue;

				// Our current position along the anticipated movement vector of the body this frame
				projectedMoveX = projectedMoveY = 0;

				// Calculate the length of the movement vector using Pythagoras Therom
				vectorLength = sqrt(SQR(nextMoveX) + SQR(nextMoveY));

				segments = 0;

				// Advance along the vector until it intersects with one of the tiles or we are done
				while (!body->Body().Inside(XMFLOAT3(CollisionPoint(dir * 2).x + m_TopLeftPosition.x + projectedMoveX, CollisionPoint(dir * 2).y + m_TopLeftPosition.y + projectedMoveY, 0))
					&& !body->Body().Inside(XMFLOAT3(CollisionPoint(dir * 2 + 1).x + m_TopLeftPosition.x + projectedMoveX, CollisionPoint(dir * 2 + 1).y + m_TopLeftPosition.y + projectedMoveY, 0))
					&& segments < vectorLength)
				{
					projectedMoveX += nextMoveX / vectorLength;
					projectedMoveY += nextMoveY / vectorLength;

					segments++;
				}

				// If we have a intersection
				if (segments < vectorLength)
				{
					// Apply correction for over-movement
					if (segments > 0)
					{
						projectedMoveX -= nextMoveX / vectorLength;
						projectedMoveY -= nextMoveY / vectorLength;
					}

					// Adjust the X or Y component of the vector depending on which
					// direction we are testing

					if (dir >= 2 && dir <= 3)
						nextMoveX = projectedMoveX;

					if (dir >= 0 && dir <= 1)
						nextMoveY = projectedMoveY;
				}
			}
			
			// Penetration resolution
			
			// We will test the four possible directions of movement for the physics body
			// dir: 0 = top, 1 = bottom, 2 = left, 3 = right
			for (int dir = 0; dir < 4; dir++)
			{
				// Skip the test if the expected direction of movement makes the test irrelevant
				// For example, we only want to test the top of the body's head when movement is
				// upwards, not downwards. This is really important! If we don't do this, we can
				// make corrections in the wrong direction, causing the body to magically jump
				// up to platforms or stick to ceilings.

				if (dir == 0 && nextMoveY > 0) continue;
				if (dir == 1 && nextMoveY < 0) continue;
				if (dir == 2 && nextMoveX > 0) continue;
				if (dir == 3 && nextMoveX < 0) continue;

				projectedMoveX = (dir >= 2 ? nextMoveX : 0);
				projectedMoveY = (dir <  2 ? nextMoveY : 0);

				// Traverse backwards in X or Y (but not both at the same time)
				// until the body is no longer colliding with the geometry

				// Note: This code also enables walking up gently sloping surfaces:
				// as the force of gravity pulls down on the player and causes surface contact,
				// the correction pushes the body away from the inside of the platform up
				// to the surface. Without this, the body would get stuck in any sloping
				// platform.
				// This same code also prevents walking up platforms that are too steeply sloped.
				// The act of falling causes collisions with the left or right side of the
				// body, causing him/her to be nudged left or right and be free to continue
				// falling further. The maximum incline of the slope the body can walk up
				// is directly related to the selected gravitational force.

				// Advance along the vector until it intersects with one of the tiles or we are done
				while (!body->Body().Inside(XMFLOAT3(CollisionPoint(dir * 2).x + m_TopLeftPosition.x + projectedMoveX, CollisionPoint(dir * 2).y + m_TopLeftPosition.y + projectedMoveY, 0))
					&& !body->Body().Inside(XMFLOAT3(CollisionPoint(dir * 2 + 1).x + m_TopLeftPosition.x + projectedMoveX, CollisionPoint(dir * 2 + 1).y + m_TopLeftPosition.y + projectedMoveY, 0)))
				{
					if (dir == 0) projectedMoveY++;
					if (dir == 1) projectedMoveY--;
					if (dir == 2) projectedMoveX++;
					if (dir == 3) projectedMoveX--;
				}

				if (dir >= 2 && dir <= 3)
					nextMoveX = projectedMoveX;

				if (dir >= 0 && dir <= 1)
					nextMoveY = projectedMoveY;
			}
			
			// Detect what type of contact has occured based on a comparison of the original
			// expected movement vector and the new one
			
			if (nextMoveY > originalMoveY && originalMoveY < 0)
				contactYTop = true;

			if (nextMoveY < originalMoveY && originalMoveY > 0)
				contactYBottom = true;

			if (abs(nextMoveX - originalMoveX) > 0.01f)
				contactX = true;

			// The body cannot continue jumping if we hit the side of something, must fall instead
			// Without this, a body hitting a wall during a jump will continue traveling upwards.
			if (contactX && speedY < 0)
				speedY = nextMoveY = 0;
		}
		else break;
		
		// If a contact has been detected, apply the re-calculated movement vector
		// and disable any further movement this frame (in either X or Y as appropriate)

		if (contactYBottom || contactYTop)
		{
			m_TopLeftPosition.y += nextMoveY;

			speedY = 0;

			// The body is not jumping if it is touching the ground
			// Body has made contact on bottom of other body
			if (contactYBottom)
			{
				jumping = false;
				bBottom = true;
			}
			
			// Body has made contact on top of other body
			if (contactYTop)
				bTop = true;
		}

		if (contactX)
		{
			m_TopLeftPosition.x += nextMoveX;

			// Which direction is the body facing
			if (speedX < 0)
				bLeft = true;
			else
				bRight = true;
			
			speedX = 0;
		}
	}
	
	// If we have any contact
	if (bTop || bBottom || bLeft || bRight)
	{
		// Update the body as to what part it has been hit: top, bottom, left, right
		OnCollision(bTop, bBottom, bLeft, bRight, body->DamageToOther());
		
		// Also update the other body to notify where it is being hit
		// The other body is hit on the opposite direction the current body is being hit
		body->OnCollision(bBottom, bTop, bRight, bLeft, iDamageToOther);
		
		// Found a contact
		return true;
	}
	
	// Did not find any contact
	return false;
}
//===============================================================================================================================
bool PhysicsBody::CollidesGJK(PhysicsBody* body)
{
	GJK gjk(m_EngineOptions);
	if (gjk.Intersect(this, body))
	{
		// Solve this collision
		for (;;)
		{
			// Move the physics body in the direction of the collision normal until it is free
			break;// Not Implemented Yet
		}

		return true;
	}

	return false;
}
//===============================================================================================================================
/*
void PhysicsBody::CollisionResponse(XMFLOAT2 normal, float dist, float dt)
{
	// Get the separation and penetration separately, stop objects from pinging apart
	float separation = max( dist, 0 );
	float penetration = min( dist, 0 );
	
	XMMath2 velNorm(m_Velocity.x, m_Velocity.y);
	
	// Compute the relative normal velocity so the object will stop at the surface
	float nv = velNorm.Dot(normal) + separation / dt;
	
	// Accumulate the penetration correction
	// applied in Update() and ensures no energy is added to the system
	m_PosCorrect.x -= normal.x * penetration / dt;
	m_PosCorrect.y -= normal.y * penetration / dt;
	
	if (nv < 0)
	{
		// Remove the normal velocity
		m_Velocity.x -= normal.x * nv;
		m_Velocity.y -= normal.y * nv;
		
		// Did we hit ground
		if (normal.y < 0)
		{
			bOnGround = true;
			
			// Apply some Friction on the surface
			if (m_ApplyFriction())
			{
				// Tangent vector from the norml
				XMFLOAT2 tangent = XMFLOAT2(-normal.y, normal.x);
				
				XMMath2 vTan(m_Velocity.x, m_Velocity.y);
				
				// Compute the tangential velocity and scale by friction
				float tv = vTan.Dot(tangent) * GROUND_FRICTION;
				
				// Subtract this from the velocity to slow down the body
				m_Velocity.x -= tangent.x * tv;
				m_Velocity.y -= tangent.y * tv;
			}
			
			// this transition occurs when this object 'lands' on the ground
			if (!bOnGroundLast)
			{
				LandingTransition();
			}
		}
	}
}
*/
//===============================================================================================================================