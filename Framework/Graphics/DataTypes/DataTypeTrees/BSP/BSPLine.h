//==================================================================================================================================
// BSPLine.h
//
//==================================================================================================================================
// History
//
// -Created on 1/7/2014 by Dustin Watson
//==================================================================================================================================
#ifndef __BSPLINE_H
#define __BSPLINE_H
//==================================================================================================================================
//==================================================================================================================================
#include "D3D.h"
#include "ZMath.h"
#include "LinePrimitive.h"
#include "BSPPolygon.h"
//==================================================================================================================================
namespace BSPLineSide
{
	enum
	{
		BACK,
		COLLINEAR,
		FRONT,
		SPANNING
	};
};
//==================================================================================================================================
class BSPLine : public ZShadeSandboxMath::LinePrimitive
{
	EngineOptions* m_EngineOptions;
	D3D*           m_D3DSystem;
	
	// Represents the line normal
	float nx;
	float ny;
	
public:
	
	BSPLine(D3D* d3d);
	
	// Normalizes the normal of this line
	void Normalize();
	
	// Gets the side of this line the specified point is on
	int GetSideThin(float x, float y);
	
	// Gets the side of this line the specified point is on
	int GetSideThick(float x, float y);
	
	// Gets the side of this line that the specified line segment is on
	int GetSide(LinePrimitive segment);
	
	// Gets the side of this line that the specified polygon is on
	int GetSide(BSPPolygon poly);
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__BSPLINE_H