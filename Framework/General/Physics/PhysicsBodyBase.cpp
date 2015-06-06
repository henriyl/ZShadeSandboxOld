#include "PhysicsBodyBase.h"
//==================================================================================================================================
//==================================================================================================================================
PhysicsBodyBase::PhysicsBodyBase(D3D* d3d)
:  m_D3DSystem(d3d)
,  bColorHull(false)
,  bTextureHull(false)
,  boundary(0)
{
	m_EngineOptions = d3d->GetEngineOptions();
}
//==================================================================================================================================
void PhysicsBodyBase::InitAABB(XMFLOAT3 center, XMFLOAT3 fScale)
{
	if (boundary == 0)
		boundary = new ZShadeSandboxMath::AABB();
	
	boundary->vMin = XMFLOAT3(center.x - fScale.x, center.y - fScale.y, center.z - fScale.z);
	boundary->vMax = XMFLOAT3(center.x + fScale.x, center.y + fScale.y, center.z + fScale.z);
}
//==================================================================================================================================
bool PhysicsBodyBase::IntersectsAABB(XMFLOAT3 point)
{
	if (boundary == 0) return false;
	
	// Calculate the dimensions of this node.
	float xMin = boundary->vMin.x;
	float xMax = boundary->vMax.x;

	float zMin = boundary->vMin.z;
	float zMax = boundary->vMax.z;

	// See if the x and z coordinate are in this node, if not then stop traversing this part of the tree.
	if ((point.x < xMin) || (point.x > xMax) || (point.z < zMin) || (point.z > zMax))
	{
		return false;
	}
	
	if (m_EngineOptions->m_DimType == ZSHADE_2D)
		return boundary->ContainsPoint2D(point);
	else if (m_EngineOptions->m_DimType == ZSHADE_3D)
		return boundary->ContainsPoint3D(point);
}
//==================================================================================================================================
float PhysicsBodyBase::DistanceAABBToPoint(XMFLOAT3 point)
{
	if (boundary == 0) return -1;
	if (m_EngineOptions->m_DimType == ZSHADE_2D)
		return boundary->DistanceToPoint2D(point);
	else if (m_EngineOptions->m_DimType == ZSHADE_3D)
		return boundary->DistanceToPoint3D(point);
}
//==================================================================================================================================
void PhysicsBodyBase::ColorTheHull(ZShadeSandboxMath::XMMath4 color)
{
	bColorHull = true;
	bTextureHull = false;
	
	vColor = color;
}
//==================================================================================================================================
void PhysicsBodyBase::TextureTheHull()
{
	bColorHull = false;
	bTextureHull = true;
}
//==================================================================================================================================
void PhysicsBodyBase::AddPoints(vector<ZShadeSandboxMesh::VertexTex> points)
{
	mMeshPoints = points;
	
	for (uint32 i = 0; i < mMeshPoints.size(); i++)
	{
		ZShadeSandboxMath::Vec3<XMFLOAT3> p(mMeshPoints[i].position.x, mMeshPoints[i].position.y, mMeshPoints[i].position.z);

		//ZShadeSandboxMesh::VertexTex nv = mMeshPoints[i].next;
		//Vec3<XMFLOAT3> nvp(nv.position.x, nv.position.y, nv.position.z);
		//p.next = &nvp;

		//ZShadeSandboxMesh::VertexTex pv = *mMeshPoints[i].prev;
		//Vec3<XMFLOAT3> pvp(pv.position.x, pv.position.y, pv.position.z);
		//p.prev = &pvp;

		mPoints.push_back(p);
	}
}
//==================================================================================================================================
void PhysicsBodyBase::AddPoints(vector< ZShadeSandboxMath::Vec3<XMFLOAT3> > points)
{
	for (uint32 i = 0; i < points.size(); i++)
	{
		mPoints.push_back(points[i]);
	}
}
//==================================================================================================================================
void PhysicsBodyBase::AddPoint(ZShadeSandboxMath::Vec3<XMFLOAT3> point)
{
	mPoints.push_back(point);
}
//==================================================================================================================================
void PhysicsBodyBase::CreateConvexHull()
{
	// Create the convex hull
	/*mHull = mConvexHull.Wrap( mPoints );
	
	for (uint32 i = 0; i < mHull.size(); i++)
	{
		//if (bColorHull)
		//{
		//	Vertex p(mHull[i].Vector().x, mHull[i].Vector().y, mHull[i].Vector().z, vColor.x, vColor.y, vColor.z, vColor.w);
		//	mRenderHull.push_back(p);
		//}
		
		if (bTextureHull)
		{
			ZShadeSandboxMesh::VertexTex meshPoint;
			
			// Find the matching point in mesh points
			for (uint32 j = 0; j < mMeshPoints.size(); j++)
			{
				Vec3<XMFLOAT3> p(mMeshPoints[j].position.x, mMeshPoints[j].position.y, mMeshPoints[j].position.z);
				if (mHull[i] == p)
				{
					meshPoint = mMeshPoints[j];
					break;
				}
			}
			
			ZShadeSandboxMesh::VertexTex p = ZShadeSandboxMesh::VertexUtil::LoadVertexTex(mHull[i].Vector().x, mHull[i].Vector().y, mHull[i].Vector().z, meshPoint.texture.x, meshPoint.texture.y);
			mRenderHull.push_back(p);
		}
	}
	
	// Get the center of the hull
	mCenter = mConvexHull.Center();
	
	m_hullCreated = true;
	*/
}
//===============================================================================================================================
ZShadeSandboxMath::BoundingBox PhysicsBodyBase::BodyBoundsPredict(XMFLOAT3 point)
{
	//m_boundingBox.left = min(m_TopLeftPosition.x, m_TopLeftPosition.x + point.x);
	//m_boundingBox.top = min(m_TopLeftPosition.y, m_TopLeftPosition.y + point.y);
	//m_boundingBox.right = max(m_TopLeftPosition.x + iBodyWidth, m_TopLeftPosition.x + iBodyWidth + point.x);
	//m_boundingBox.bottom = max(m_TopLeftPosition.y + iBodyHeight, m_TopLeftPosition.y + iBodyHeight + point.y);
	
	ZShadeSandboxMath::BoundingBox box;
	
	box.x = m_TopLeftPosition.x + point.x;
	box.y = m_TopLeftPosition.y + point.y;
	box.width = iBodyWidth;
	box.width = iBodyHeight;
	
	return box;
}
//===============================================================================================================================
ZShadeSandboxMath::BoundingBox PhysicsBodyBase::BodyBoundsPredict(float x, float y)
{
	//m_boundingBox.left = min(m_TopLeftPosition.x, m_TopLeftPosition.x + x);
	//m_boundingBox.top = min(m_TopLeftPosition.y, m_TopLeftPosition.y + y);
	//m_boundingBox.right = max(m_TopLeftPosition.x + iBodyWidth, m_TopLeftPosition.x + iBodyWidth + x);
	//m_boundingBox.bottom = max(m_TopLeftPosition.y + iBodyHeight, m_TopLeftPosition.y + iBodyHeight + y);
	
	ZShadeSandboxMath::BoundingBox box;
	
	box.x = m_TopLeftPosition.x + x;
	box.y = m_TopLeftPosition.y + y;
	box.width = iBodyWidth;
	box.width = iBodyHeight;
	
	return box;
}
//===============================================================================================================================
void PhysicsBodyBase::InitCollisionPoints(int body_width, int body_height)
{
	iBodyWidth = body_width;
	iBodyHeight = body_height;
	
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
	
	m_CenterOffset = XMFLOAT3(iBodyWidth / 2, iBodyHeight / 2, 0);
	m_HalfExtents = XMFLOAT3(iBodyWidth / 2, iBodyHeight / 2, 0);
}
//===============================================================================================================================
void PhysicsBodyBase::GenerateBody()
{
	//XMFLOAT3 center = XMFLOAT3(m_TopLeftPosition.x, m_TopLeftPosition.y, 0);

	//center.x += m_CenterOffset.x;
	//center.y += m_CenterOffset.y;

	//m_aabbBody.Initialize(center, m_HalfExtents);

	m_body.x = m_TopLeftPosition.x;
	m_body.y = m_TopLeftPosition.y;
	m_body.width = iBodyWidth;
	m_body.height = iBodyHeight;
}
//===============================================================================================================================
/*bool BodyBox::Inside(XMFLOAT2 pos)
{
	return (left < pos.x && right >= pos.x && top < pos.y && bottom >= pos.y);
}
//===============================================================================================================================
bool BodyBox::Inside(XMFLOAT3 pos)
{
	return (left < pos.x && right >= pos.x && top < pos.y && bottom >= pos.y);
}
//===============================================================================================================================
bool BodyBox::Inside(float x, float y)
{
	return (left < x && right >= x && top < y && bottom >= y);
}
//===============================================================================================================================
bool BodyBox::Inside(float x, float y, float z)
{
	return (left < x && right >= x && top < y && bottom >= y);
}
//===============================================================================================================================
bool BodyBox::InsideEquals(XMFLOAT2 pos)
{
	return (left <= pos.x && right >= pos.x && top <= pos.y && bottom >= pos.y);
}
//===============================================================================================================================
bool BodyBox::InsideEquals(XMFLOAT3 pos)
{
	return (left <= pos.x && right >= pos.x && top <= pos.y && bottom >= pos.y);
}
//===============================================================================================================================
bool BodyBox::InsideEquals(float x, float y)
{
	return (left <= x && right >= x && top <= y && bottom >= y);
}
//===============================================================================================================================
bool BodyBox::InsideEquals(float x, float y, float z)
{
	return (left <= x && right >= x && top <= y && bottom >= y);
}
//===============================================================================================================================
*/