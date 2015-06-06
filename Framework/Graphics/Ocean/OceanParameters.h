//===============================================================================================================================
// OceanParameters
//
//===============================================================================================================================
// History
//
// -Created on 5/30/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __OCEANPARAMETERS_H
#define __OCEANPARAMETERS_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//

#include "ZMath.h"

//===============================================================================================================================
//===============================================================================================================================
struct OceanParameters
{
	float 		g_WaveHeight;
	float 		g_SpecularShininess;
	XMFLOAT3 	g_LightDirection;
	XMFLOAT3 	g_CamPos;
	float 		g_Time;
	XMFLOAT4 	g_RefractionTint;
	XMFLOAT4	g_DullColor;
	int			g_OceanSize;
	int			g_PatchSize;
	float		g_SeaLevel;
	float		g_MinDist;
	float		g_MaxDist;
	float		g_MinTess;
	float		g_MaxTess;
	XMFLOAT2	g_WindDir;
	float		g_WaveAmplitude;
	float		g_WindSpeed;
	float		g_WindDependency;
	float		g_TimeScale;
	float		g_ChoppyScale;
};

//===============================================================================================================================
//===============================================================================================================================
#endif//__OCEANPARAMETERS_H