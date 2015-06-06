#include "PlatformerPhysicsBody.h"
//==================================================================================================================================
//==================================================================================================================================
PlatformerPhysicsBody::PlatformerPhysicsBody(D3D* d3d, int body_width, int body_height)
:  m_D3DSystem(d3d)
,  iBodyWidth(body_width)
,  iBodyHeight(body_height)
{
	m_EngineOptions = d3d->GetEngineOptions();
	
	/* Create the eight contact points based on the center point and the size of the entity
		  h1  h2
		  
		l1      r1
		
		l2      r2
		
		  f1  f2
	*/
	
	m_CollisionPoints.resize(8);
	
	XMFLOAT2 points[8] =
	{
		{ (float)(iBodyWidth / 4), 0.0f }, { (float)(iBodyWidth - (iBodyWidth / 4)), 0.0f }, 		   						// Top of head
		{ (float)(iBodyWidth / 4), (float)iBodyHeight }, { (float)(iBodyWidth - (iBodyWidth / 4)), (float)iBodyHeight }, 	// Feet
		{ 0.0f, (float)(iBodyHeight / 4) }, { 0.0f, (float)(iBodyHeight - (iBodyHeight / 4)) },		   						// Left Arm
		{ (float)iBodyWidth, (float)(iBodyHeight / 4) }, { (float)iBodyWidth, (float)(iBodyHeight - (iBodyHeight / 4)) }	// Right Arm
	};
	
	for (int i = 0; i < 8; i++)
	{
		m_CollisionPoints[i] = XMFLOAT3(points[i].x, points[i].y, 0);
	}
}
//===============================================================================================================================