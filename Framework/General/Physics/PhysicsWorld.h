//==================================================================================================================================
// PhysicsWorld.h
//
//==================================================================================================================================
// History
//
// -Created on 12/17/2014 by Dustin Watson
//==================================================================================================================================
#ifndef __PHYSICSWORLD_H
#define __PHYSICSWORLD_H
//==================================================================================================================================
//==================================================================================================================================
#include "D3D.h"
#include "ZMath.h"
#include "GJK.h"
#include "Input.h"
#include "ContactStats.h"
#include "Text.h"
//==================================================================================================================================
class TopdownMap;
class PlatformerMap;
//==================================================================================================================================
class PhysicsWorld
{
	D3D* m_D3DSystem;
	EngineOptions* m_EngineOptions;
	
	// If we have a topdown map we want to collide against a hard tile but without gravity
	TopdownMap* m_TopdownMap;
	PlatformerMap* m_PlatformerMap;
	
	// Need to be able to toggle gravity in the tool window
	bool bGravityOn;
	
	vector<PhysicsBody*> m_TileBodies;
	vector<int> m_BoundBodies;
	
	ContactStats m_ContactStatsPlayerToTile[3];
	
	ZShadeSandboxGraphics::Text text[3][7];
	
public:
	
	// Creates a physics world depending on the environment
	PhysicsWorld(D3D* d3d, TopdownMap* map);
	PhysicsWorld(D3D* d3d, PlatformerMap* map);
	
	void BuildTileBodies();
	
	// Update the physics bodies in the world due to gravitation and other forces
	void Update(float dt, Keyboard* keyboard);
	
	void RenderContactStats();
	
	// Accessor methods
public:
	
	bool& IsGravityOn() { return bGravityOn; }
	bool IsGravityOn() const { return bGravityOn; }
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__PHYSICSWORLD_H