#include "BSPLine.h"
//==================================================================================================================================
//==================================================================================================================================
BSPLine::BSPLine(D3D* d3d)
:   ZShadeSandboxMath::LinePrimitive()
,   m_D3DSystem(d3d)
{
	m_EngineOptions = m_D3DSystem->GetEngineOptions();
}
//==================================================================================================================================
void BSPLine::Normalize()
{
	float length = (float)sqrt(nx * nx + ny * ny);
	
	nx /= length;
	ny /= length;
}
//==================================================================================================================================
int BSPLine::GetSideThin(float x, float y)
{
	XMFLOAT3 point = point1;// .Vector();

	// Dot Product between vector to the point and the normal
	float side = (x - point.x) * nx + (y - point.y) * ny;
	
	return (side < 0) ? BSPLineSide::BACK : (side > 0) ? BSPLineSide::FRONT : BSPLineSide::COLLINEAR;
}
//==================================================================================================================================
int BSPLine::GetSideThick(float x, float y)
{
	int frontSide = GetSideThin(x - nx / 2, y - ny / 2);
	
	if (frontSide == BSPLineSide::FRONT)
		return BSPLineSide::FRONT;
	else if (frontSide == BSPLineSide::BACK)
	{
		int backSide = GetSideThin(x + nx / 2, y + ny / 2);
		
		if (backSide == BSPLineSide::BACK)
			return BSPLineSide::BACK;
	}
	
	return BSPLineSide::COLLINEAR;
}
//==================================================================================================================================
int BSPLine::GetSide(LinePrimitive segment)
{
	LinePrimitive l(point1, point2);

	if (l == segment)
		return BSPLineSide::COLLINEAR;
	
	int p1Side = GetSideThick(segment.Point1().x, segment.Point1().y);
	int p2Side = GetSideThick(segment.Point2().x, segment.Point2().y);
	
	if (p1Side == p2Side)
		return p1Side;
	else if (p1Side == BSPLineSide::COLLINEAR)
		return p2Side;
	else if (p2Side == BSPLineSide::COLLINEAR)
		return p1Side;
	else
		return BSPLineSide::SPANNING;
}
//==================================================================================================================================
int BSPLine::GetSide(BSPPolygon poly)
{
	bool onFront = false;
	bool onBack = false;
	
	// check every point
	for (int i=0; i< poly.GetNumVertices(); i++)
	{
		XMFLOAT3 v = poly.GetVertex(i);
		
		int side = GetSideThick(v.x, v.z);
		
		if (side == BSPLineSide::FRONT)
			onFront = true;
		else if (side == BSPLineSide::BACK)
			onBack = true;
	}
	
	// classify the polygon
	if (onFront && onBack)
		return BSPLineSide::SPANNING;
	else if (onFront)
		return BSPLineSide::FRONT;
	else if (onBack)
		return BSPLineSide::BACK;
	else
		return BSPLineSide::COLLINEAR;
}
//==================================================================================================================================