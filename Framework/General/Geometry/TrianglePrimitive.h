//==============================================================================================================================
// TrianglePrimitive.h
//==============================================================================================================================
#ifndef TRIANGLEPRIMITIVE_H
#define TRIANGLEPRIMITIVE_H
//==============================================================================================================================
//==============================================================================================================================
#include "Vec.h"
//==============================================================================================================================
//==============================================================================================================================
namespace ZShadeSandboxMath {
class TrianglePrimitive
{
	XMFLOAT3 point1;
	XMFLOAT3 point2;
	XMFLOAT3 point3;
	
public:
	
	TrianglePrimitive();
	TrianglePrimitive(XMFLOAT3 p1, XMFLOAT3 p2, XMFLOAT3 p3);
	
	XMFLOAT3 Point1() const;
	XMFLOAT3 Point2() const;
	XMFLOAT3 Point3() const;
	
	// Compute the area of the triangle
	float Area();
	
	bool operator == (const TrianglePrimitive& tri);

	// Does a point lie inside or outside the triangle
	bool Inside(XMFLOAT3 point);
};
}
//==============================================================================================================================
//==============================================================================================================================
#endif// TRIANGLEPRIMITIVE_H