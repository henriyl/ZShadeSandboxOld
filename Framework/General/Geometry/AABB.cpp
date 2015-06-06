#include "AABB.h"
#include "ZShadeMessageCenter.h"
#include "Scalar.h"
using ZShadeSandboxMath::AABB;
//==================================================================================================================================
//==================================================================================================================================
AABB::AABB()
:   vMin(XMFLOAT3(0, 0, 0))
,   vMax(XMFLOAT3(0, 0, 0))
{
}
//==================================================================================================================================
AABB::AABB(XMFLOAT3 min, XMFLOAT3 max)
:   vMin(min)
,   vMax(max)
{
	// Compute box center and extents
	vCenter = (XMMath3(vMin) - XMMath3(vMax)) * 0.5f;
	float fExtentX = (vMax.x - vMin.x) * 0.5f;
	float fExtentY = (vMax.y - vMin.y) * 0.5f;
	float fExtentZ = (vMax.z - vMin.z) * 0.5f;
	vExtent = XMFLOAT3(fExtentX, fExtentY, fExtentZ);
}
//==================================================================================================================================
void AABB::Construct(XMFLOAT3 center, XMFLOAT3 scale)
{
	vCenter = center;
	vSize = scale;

	vMin = XMFLOAT3(center.x - scale.x, center.y - scale.y, center.z - scale.z);
	vMax = XMFLOAT3(center.x + scale.x, center.y + scale.y, center.z + scale.z);
	
	// extent
	float fExtentX = (vMax.x - vMin.x) * 0.5f;
	float fExtentY = (vMax.y - vMin.y) * 0.5f;
	float fExtentZ = (vMax.z - vMin.z) * 0.5f;
	vExtent = XMFLOAT3(fExtentX, fExtentY, fExtentZ);
}
//==================================================================================================================================
bool AABB::ContainsPoint2D(XMFLOAT3 p)
{
	if (p.x >= vMin.x && p.x <= vMax.x &&
		p.y >= vMin.y && p.y <= vMax.y)
		return true;
	return false;
}
//==================================================================================================================================
bool AABB::ContainsPoint3D(XMFLOAT3 p)
{
	if (p.x >= vMin.x && p.x <= vMax.x &&
		p.y >= vMin.y && p.y <= vMax.y &&
		p.z >= vMin.z && p.z <= vMax.z)
		return true;
	return false;
}
//==================================================================================================================================
bool AABB::ContainsPoint3DOmitY(XMFLOAT3 p)
{
	if (p.x >= vMin.x && p.x <= vMax.x &&
		p.z >= vMin.z && p.z <= vMax.z)
		return true;
	return false;
}
//==================================================================================================================================
float AABB::DistanceToPoint2D(XMFLOAT3 p)
{
	float dx = max(vMin.x - p.x, p.x - vMax.x);
	float dy = max(vMin.y - p.y, p.y - vMax.y);
	return sqrt(dx * dx + dy * dy);
}
//==================================================================================================================================
float AABB::DistanceToPoint3D(XMFLOAT3 p)
{
	float dx = max(vMin.x - p.x, p.x - vMax.x);
	float dy = max(vMin.y - p.y, p.y - vMax.y);
	float dz = max(vMin.z - p.z, p.z - vMax.z);
	return sqrt(dx * dx + dy * dy + dz * dz);
}
//==================================================================================================================================