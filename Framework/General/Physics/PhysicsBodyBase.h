//==================================================================================================================================
// PhysicsBodyBase.h
//
//http://www.wildbunny.co.uk/blog/2011/12/14/how-to-make-a-2d-platform-game-part-2-collision-detection/
//http://www.hobbygamedev.com/int/platformer-game-source-in-processing/
//https://katyscode.wordpress.com/2013/01/18/2d-platform-games-collision-detection-for-dummies/
//==================================================================================================================================
// History
//
// -Created on 12/17/2014 by DW
// -Modified on 1/28/2015 by DW Changed name to PhysicsBodyBase
// -Modified on 4/13/2015 by DW Added AABB collision data
//==================================================================================================================================
#ifndef __PHYSICSBODYBASE_H
#define __PHYSICSBODYBASE_H
//==================================================================================================================================
//==================================================================================================================================
#include "D3D.h"
#include "ZMath.h"
#include "XMMath.h"
#include "PhysicsType.h"
#include "Vertex.h"
//#include "GJK.h"
#include "AABB.h"
#include "BoundingBox.h"
//==================================================================================================================================
//==================================================================================================================================
class PhysicsBodyBase
{
protected:

	D3D*           m_D3DSystem;
	EngineOptions* m_EngineOptions;
	
	// Is this a static or dynamic physics body
	PhysicsType eType;
	
	// Axis aligned bounding box of the body
	ZShadeSandboxMath::AABB* boundary;
	
	//
	// 3D Physics Section
	//

	bool bColorHull;
	bool bTextureHull;
	
	ZShadeSandboxMath::XMMath4 vColor;
	
	// World matrix of the physics body (Not sure if need this here)
	XMFLOAT4X4 mWorld;
	
	// Convex hull algorithm
	//QuickHull mConvexHull;
	
	// The vertices from the mesh
	vector<ZShadeSandboxMesh::VertexTex> mMeshPoints;
	
	// The vertices that make up the physics body
	vector< ZShadeSandboxMath::Vec3<XMFLOAT3> > mPoints;
	
	// The Convex hull of the physics body ready to render
	vector<ZShadeSandboxMesh::VertexTex> mRenderHull;
	
	// The Convex hull of the physics body
	vector< ZShadeSandboxMath::Vec3<XMFLOAT3> > mHull;
	
	// The center of the physics body
	XMFLOAT3 mCenter;
	
	// Is the convex hull created
	bool m_hullCreated;
	
	// Is the convex hull needed
	bool m_hullNeeded;
	
	//
	// 2D Physics Section
	//

	int iBodyWidth;
	int iBodyHeight;

	XMFLOAT3 m_CenterOffset;
	XMFLOAT3 m_HalfExtents;

	// Top-left coordinate of the Physics body
	XMFLOAT3 m_TopLeftPosition;

	// AABB Bounding Box to check collisions
	//AABB m_aabbBody;
	ZShadeSandboxMath::BoundingBox m_body;
	
	// Temp Body bounding box for the physics body
	//BodyBox m_boundingBox;
	
	//
	// Check for eight points of collision
	// (2 head, 2 feet, 2 left, 2 right)
	//
	
	vector<XMFLOAT3> m_CollisionPoints;
	
public:
	
	PhysicsBodyBase(D3D* d3d);
	
	// Returns whether this is a static or dynamic physics body
	PhysicsType& EPhysicsType() { return eType; }
	PhysicsType EPhysicsType() const { return eType; }
	
	// Check whether or not a point will enter the AABB of this body
	bool IntersectsAABB(XMFLOAT3 point);
	// Gets the distance from the AABB of this body to a point
	float DistanceAABBToPoint(XMFLOAT3 point);
	
	// Creates an Axis-Aligned bounding box by displacing a point by a value
	void InitAABB(XMFLOAT3 center, XMFLOAT3 fScale);
	
	ZShadeSandboxMath::AABB* GetAABB() { return boundary; }

	//
	// 3D Physics Section
	//

	void ColorTheHull(ZShadeSandboxMath::XMMath4 color);
	void TextureTheHull();
	
	// Returns the center of the physics body
	XMFLOAT3 Center() { return mCenter; }
	
	// Creates the convex hull of the physics body
	void CreateConvexHull();
	
	// Determines if the convex hull was created
	bool IsHullCreated() { return m_hullCreated; }
	
	// Determine if the convex hull is needed
	bool IsHullNeeded() { return m_hullNeeded; }
	void SetHullNeeded(bool need) { m_hullNeeded = need; }
	
	// Adds the points to the physics body from a mesh that has a renderable vertex
	void AddPoints(vector<ZShadeSandboxMesh::VertexTex> points);
	
	// Adds the points to the physics body from any mesh
	void AddPoints(vector< ZShadeSandboxMath::Vec3<XMFLOAT3> > points);
	
	// Obtain a point into the point list, useful for something like a 2D Circle
	void AddPoint(ZShadeSandboxMath::Vec3<XMFLOAT3> point);
	
	// Gets the convex hull of the physics body for a vertex shader
	vector< ZShadeSandboxMesh::VertexTex > GetRenderHull() { return mRenderHull; }
	
	// Gets the points that make up the physics body
	vector< ZShadeSandboxMath::Vec3<XMFLOAT3> > GetPoints() { return mPoints; }
	
	// Gets the convex hull of the physics body
	vector< ZShadeSandboxMath::Vec3<XMFLOAT3> > GetHull() { return mHull; }
	
	//
	// 2D Physics Section
	//
	
	void InitCollisionPoints(int body_width, int body_height);
	
	void GenerateBody();
	
	// Create a bounds that extends to a predicted point
	ZShadeSandboxMath::BoundingBox BodyBoundsPredict(XMFLOAT3 point);
	ZShadeSandboxMath::BoundingBox BodyBoundsPredict(float x, float y);
	
	ZShadeSandboxMath::BoundingBox& Body() { return m_body; }
	ZShadeSandboxMath::BoundingBox Body() const { return m_body; }

	// Returns the m_TopLeftPosition of the physics body (Top-left coordinate)
	XMFLOAT3& TopLeftPosition() { return m_TopLeftPosition; }
	XMFLOAT3 TopLeftPosition() const { return m_TopLeftPosition; }

	int& BodyWidth() { return iBodyWidth; }
	int BodyWidth() const { return iBodyWidth; }

	int& BodyHeight() { return iBodyHeight; }
	int BodyHeight() const { return iBodyHeight; }
	
	//BodyBox& BBodyBox() { return m_boundingBox; }
	//BodyBox BBodyBox() const { return m_boundingBox; }
	
	XMFLOAT3 CollisionPoint(int i) { return m_CollisionPoints[i]; }

	void Clone(PhysicsBodyBase* c) {}
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__PHYSICSBODYBASE_H