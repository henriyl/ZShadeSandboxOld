//==================================================================================================================================
// PlatformerPhysicsBody.h
//
//==================================================================================================================================
// History
//
// -Created on 1/20/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __PLATFORMERPHYSICSBODY_H
#define __PLATFORMERPHYSICSBODY_H
//==================================================================================================================================
//==================================================================================================================================
#include "D3D.h"
#include "ZMath.h"
#include "XMMath.h"
#include "PhysicsType.h"
#include "Vertex.h"
#include "GJK.h"
//==================================================================================================================================
//http://www.wildbunny.co.uk/blog/2011/12/14/how-to-make-a-2d-platform-game-part-2-collision-detection/
//http://www.hobbygamedev.com/int/platformer-game-source-in-processing/
//https://katyscode.wordpress.com/2013/01/18/2d-platform-games-collision-detection-for-dummies/
class PlatformerPhysicsBody
{
protected:

	D3D*           m_D3DSystem;
	EngineOptions* m_EngineOptions;
	
	// Is this a static or dynamic physics body
	PhysicsType eType;
	
	// World matrix of the physics body
	XMFLOAT4X4 mWorld;
	
	int iBodyWidth;
	int iBodyHeight;
	
	// Top-left coordinate of the Physics body
	XMFLOAT2 m_TopLeftPosition;
	
	//
	// Check for eight points of collision
	// (2 head, 2 feet, 2 left, 2 right)
	//
	
	vector<XMFLOAT3> m_CollisionPoints;
	
	// Need the AABB Bounding Box and the geom for a tile to check collisions
	
	
public:
	
	PlatformerPhysicsBody(D3D* d3d, int body_width, int body_height);
	
	// Returns the position of the physics body (Top-left coordinate)
	XMFLOAT2& TopLeftPosition() { return m_TopLeftPosition; }
	XMFLOAT2 TopLeftPosition() const { return m_TopLeftPosition; }
	
	// Returns whether this is a static or dynamic physics body
	PhysicsType& EPhysicsType() { return eType; }
	PhysicsType EPhysicsType() const { return eType; }
	
	vector<XMFLOAT3> CollisionPoints() { return m_CollisionPoints; }
	
	// Does this physics body collide with another physics body
	bool Collides(PhysicsBody* body, DWORD tickCount) { return false; };
	
	void Clone(PhysicsBody* c) {}
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__PLATFORMERPHYSICSBODY_H