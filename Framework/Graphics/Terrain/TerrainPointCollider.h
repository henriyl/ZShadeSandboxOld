//===============================================================================================================================
// TerrainPointCollider
//
// This class will allow a point to collide against the terrain and slide against it
//===============================================================================================================================
// History
//
// -Created on 6/19/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __TERRAINPOINTCOLLIDER_H
#define __TERRAINPOINTCOLLIDER_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//

#include "Vertex.h"
#include "QNodeType.h"
#include "D3D.h"
#include "AABB.h"
#include "QMeshNode.h"

//===============================================================================================================================
namespace ZShadeSandboxTerrain {
class TerrainPointCollider
{
	struct CollisionPacket
	{
		// World Space Ellipsoid Info
		XMVECTOR	ellipsoidSpace;
		XMVECTOR	wPosition;
		XMVECTOR	wVelocity;
		
		// Ellipsoid Space Ellipsoid Info
		XMVECTOR	ePosition;
		XMVECTOR	eVelocity;
		XMVECTOR	eNormalizedVelocity;
		
		// Collision Info
		bool		collisionFound;
		float		nearestDistance;
		XMVECTOR	intersectionPoint;
		int			collisionRecursionDepth;
	};
	
public:
	
	TerrainPointCollider();
	~TerrainPointCollider();
	
	// The camera class needs to call TerrainCollisionUpdateEnd() after the intersectionPoint was found from the entire Quad Tree
	// and the position of the camera has been updated.
	bool CameraColliding(float unitsPerMeter, float gravity, Camera* camera, ZShadeSandboxTerrain::QMeshNode* node, XMFLOAT3& intersectionPoint);
	
	//bool CameraColliding(float unitsPerMeter, float gravity, Camera* camera, vector<Triangle*> internalTriangles, XMFLOAT3& intersectionPoint);

private:
	
	XMVECTOR CollisionSlide(vector<XMFLOAT3> internalVertices, vector<UINT> internalIndices);
	XMVECTOR CollisionWithWorld(vector<XMFLOAT3> internalVertices, vector<UINT> internalIndices);
	bool SphereCollidingWithTriangle(XMVECTOR& vertex0, XMVECTOR& vertex1, XMVECTOR& vertex2, XMVECTOR& normal);
	bool CheckPointInTriangle(const XMVECTOR& point, XMVECTOR& vertex0, XMVECTOR& vertex1, XMVECTOR& vertex2);
	
	// Returns the lowest root of the quadratic equation
	bool GetLowestRoot(float a, float b, float c, float maxR, float* root);
	
private:
	
	float fUnitsPerMeter;
	float fGravity;
	
	vector<XMFLOAT3> mCollidablePositions;
	
	CollisionPacket mCollisionPacket;
};
}
//===============================================================================================================================
#endif//__TERRAINPOINTCOLLIDER_H