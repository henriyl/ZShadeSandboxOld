//===============================================================================================================================
// Procedural Generator
//
//===============================================================================================================================
// History
//
// -Created on 6/16/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __PROCEDURALGENERATOR_H
#define __PROCEDURALGENERATOR_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//

#include "HeightErosion.h"

//===============================================================================================================================
//===============================================================================================================================
namespace ZShadeSandboxTerrain {
class ProceduralGenerator
{
public:
	
	// Can put an existing heightmap in a erode it or procedurally create one from a flat plane
	
	ProceduralGenerator(float terrainSize, float seaLevel, float maxHeight);
	ProceduralGenerator(float terrainSize, float seaLevel, float maxHeight, vector<HeightData> heightMapInput);
	~ProceduralGenerator();
	
	void BuildRandomHeightmap();
	void BuildPerlinNoiseHeightmap();
	void BuildFieldNoiseHeightmap();
	void BuildCustumNoiseHeightmap();
	void BuildDiamondSquare();

	// These are added to an existing heightmap input
	void AddRandomHeightmap();
	void AddPerlinNoiseHeightmap();
	void AddFieldNoiseHeightmap();
	void AddCustumNoiseHeightmap();

	// Perform a simple smooth of the map so it is not choppy
	void Smooth(int smoothingPassCount);
	void Smooth();

	void Normalize(float normalizeFactor);

	// Perform height erosion in four directions by using a linear interpolation from an erosion value
	// erosionValue if between 0 and 1 inclusive
	void ErodeHeightmapInput(float erosionValue);
	void ErodeProceduralMap(float erosionValue);
	
	// Returns the generated procedural map
	vector<HeightData> GetProceduralMap() { return mProceduralHeightmap; }
	
	float ReadProceduralHeight(int x, int z);
	float ReadProceduralHeight(int index);

private:
	
	float PerlinValue(int x, int y, int random);
	
	
private:
	
	vector<HeightData> mHeightmapInput;
	
	// The resulting height map after it has been affected procedurally
	vector<HeightData> mProceduralHeightmap;
	
	float fSeaLevel;
	float fMaxHeight;// = 70
	float fTerrainSize;
};
}
//===============================================================================================================================
#endif//__PROCEDURALGENERATOR_H