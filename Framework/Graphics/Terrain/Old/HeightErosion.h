//===============================================================================================================================
// Simple Terrain Heightmap Erosion Generator
//
// http://www.dreamincode.net/forums/blog/2250/entry-4550-terrain-erosion/
//===============================================================================================================================
// History
//
// -Created on 6/15/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __HEIGHTEROSION_H
#define __HEIGHTEROSION_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//

#include "D3D.h"
#include "HeightmapData.h"

//===============================================================================================================================
// To use this add the heightmap data and width of the terrain
// and set the erosion value between 0 and 1 inclusive (percentage of how flat the heightmap will be)
// then call Erode and assign the erosion map to the heightmap.
namespace ZShadeSandboxTerrain {
class HeightErosion
{
public:
	
	HeightErosion(vector<HeightData> heightMapInput, int terrainSize, float erosionValue);
	~HeightErosion();
	
	// Performs the erosion
	void Erode();
	
	// Returns the generated erosion map
	vector<HeightData> GetErosionMap() { return mErosionMap; }
	
	float ReadErosionHeight(int x, int z);
	
	// Sets the erosion value
	float& ErosionValue() { return mErosionValue; }
	
private:
	
	void RebuildHeightmap();
	
	void UpdateErosion(int xUpdate0, int xUpdate1, int zUpdate0, int zUpdate1);
	
	// Get the height from the heightmap
	float ReadHeight(int x, int z);
	
	void UpdateErosionHeight(int x, int z, float height);
	
	// Does an interpolation of current height value and previous height value with
	// the height erosion value
	float CalculateErosionValue(float currHeight, float prevHeight);
	
private:
	
	// Erosion Output
	vector<HeightData> mErosionMap;
	
	// Heightmap input
	vector<HeightData> mHeightMap;
	
	int		mTerrainSize;
	
	// value between 0 and 1
	// 1 is flat
	// 0 is no change
	float 	mErosionValue;
};
}
//===============================================================================================================================
#endif//__HEIGHTEROSION_H