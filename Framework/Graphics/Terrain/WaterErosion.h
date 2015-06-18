//===============================================================================================================================
// WaterErosion
//
//===============================================================================================================================
// History
//
// -Created on 6/17/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __WATEREROSION_H
#define __WATEREROSION_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//

#include "D3D.h"
#include "HeightmapData.h"

//===============================================================================================================================
//===============================================================================================================================
namespace ZShadeSandboxTerrain {
struct VelocityVector
{
	float x;
	float y;

	VelocityVector()
	{
		x = y = 0;
	}

	VelocityVector(float px, float py)
	{
		x = px;
		y = py;
	}
};

struct OutflowValues
{
	float left;
	float right;
	float top;
	float bottom;

	OutflowValues()
	{
		left = right = top = bottom = 0;
	}

	OutflowValues(float l, float r, float t, float b)
	{
		left = l;
		right = r;
		top = t;
		bottom = b;
	}
};

struct ThermalWeatheringValues
{
	float topLeft;
	float top;
	float topRight;
	float right;
    float bottomRight;
    float bottom;
    float bottomLeft;
    float left;
};

struct WaterErosionParameters
{
	int		terrainSize;
	float	seaLevel;
	int 	waterSourceHeight;
	float	thermalPowerMultiplier;
	float	deltaT;
	float	pipeLength;
	float	pipeCrossectionArea;
	float	graviationalAcceleration;
	float	sedimentCapacityConstant;
	float	dissolvingConstant;
	float	depositionConstant;
	float	minimumComputedSurfaceTilt;
	float	talusAngle;
	bool	applyThermalWeathering;
	bool	applyEvaporation;
	float	erosionDuration;
	
	WaterErosionParameters()
	{
		terrainSize = 0;
		seaLevel = 0;
		waterSourceHeight = 0;
		thermalPowerMultiplier = 1.0f;
		deltaT = 0.005f;
		pipeLength = 1.0f;
		pipeCrossectionArea = 20.0f;
		graviationalAcceleration = 9.7f;
		sedimentCapacityConstant = 1.0f;
		dissolvingConstant = 0.5f;
		depositionConstant = 1.0f;
		minimumComputedSurfaceTilt = 0.1f;
		talusAngle = 0.5f;
		erosionDuration = 1.0f;
		applyThermalWeathering = false;
		applyEvaporation = false;
	}
};
//===============================================================================================================================
class WaterErosion
{
public:
	
	WaterErosion(vector<HeightData> heightMapInput, WaterErosionParameters wep);
	~WaterErosion();
	
	void Erode();
	void ThermalWeather();

	// Returns the generated erosion map
	vector<HeightData> GetErosionMap() { return mErosionMap; }
	
	float ReadErosionHeight(int x, int z);

private:
	
	void BuildVelocityVectorMap();
	void SetWaterSource();
	void ApplyEvaporation();
	void ApplyErosion();
	void ApplyThermalWeathering();
	
	float ReadWaterHeight(int x, int z);
	float ReadSedimentationHeight(int x, int z);
	float ReadHeightMapInput(int x, int z);

	OutflowValues ReadOutflowFluxMap(int x, int z);
	VelocityVector ReadVelocityVectorMap(int x, int z);
	
	void UpdateWaterMap(int x, int z, float value);
	void UpdateSedimentationMap(int x, int z, float value);
	void UpdateErosionMap(int x, int z, float value);
	void UpdateHeightMapInput(int x, int z, float value);
	
	void UpdateOutflowFluxMap(int x, int z, OutflowValues value);
	void UpdateVelocityVectorMap(int x, int z, VelocityVector value);
	
	float FlowValue(float direction, float heightDifference);
	
private:
	
	WaterErosionParameters mWaterErosionParameters;
	
	vector<HeightData> mHeightMapInput;
	vector<HeightData> mWaterMap;
	vector<HeightData> mSedimentationMap;
	vector<HeightData> mErosionMap;
	
	vector<OutflowValues> mOutflowFluxMap;
	vector<VelocityVector> mVelocityVectorMap;
};
}
//===============================================================================================================================
#endif//__WATEREROSION_H