//==================================================================================================================================
// Ray.h
//
//==================================================================================================================================
// History
//
// -Created on 4/28/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __RAY_H
#define __RAY_H
//==================================================================================================================================
//==================================================================================================================================
#include "ZMath.h"
#include "AABB.h"
//==================================================================================================================================
namespace ZShadeSandboxMath {
struct Ray
{
	XMFLOAT3 position;
	XMFLOAT3 direction;
	
	Ray();
	Ray(XMFLOAT3 pos, XMFLOAT3 dir);
};
}
//==================================================================================================================================
//==================================================================================================================================
#endif//__RAY_H