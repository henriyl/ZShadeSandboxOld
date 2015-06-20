//===================================================================================================================
// Heightmap
//
//===================================================================================================================
// History
//
// -Created on 6/18/2014 by Dustin Watson
//===================================================================================================================
#ifndef __HEIGHTMAP_H
#define __HEIGHTMAP_H
//===================================================================================================================
#include "D3D.h"
#include "HeightmapData.h"
#include "ProceduralGenerator.h"
//===================================================================================================================
namespace ZShadeSandboxTerrain {
class Heightmap
{
public:
	
	Heightmap(string heightmap, int width, int height, float heightScale, float seaLevel, float maxHeight);
	Heightmap(ZShadeSandboxTerrain::ProceduralParameters pp, int width, int height, float heightScale, float seaLevel, float maxHeight);
	Heightmap(const Heightmap& o);
	~Heightmap();

	float SampleHeight(int x, int z);
	float SampleHeight(int index);
	void UpdateHeightValues(float heightScale, float zScale);
	bool LoadElevation(string heightmap);
	
	void LoadProceduralElevation(ZShadeSandboxTerrain::ProceduralParameters pp);
	
	void UpdateHeight(int x, int z, float height);
	
	// Returns the height in a safe way
	float GetHeight(float x, float z);
	bool InBounds(int x, int z);
	bool InHeightmap(float x, float z);

	int Width() { return m_heightmap_width; }
	int Height() { return m_heightmap_height; }

	vector<HeightData> GetHeightmap() { return m_heightmap; }
	
	//
	// Procedural Generation for the heightmap
	//
	
	void BuildRandomHeightmap();
	void BuildPerlinNoiseHeightmap();
	void BuildFieldNoiseHeightmap();
	void BuildDiamondSquare();

	// These are added to an existing heightmap input
	void AddRandomHeightmap();
	void AddPerlinNoiseHeightmap();
	void AddFieldNoiseHeightmap();

	// Perform a simple smooth of the map so it is not choppy
	void Smooth(int smoothingPassCount);
	void Smooth();

	void Normalize(float normalizeFactor);

	// Perform height erosion in four directions by using a linear interpolation from an erosion value
	// erosionValue if between 0 and 1 inclusive
	void ErodeHeight(float erosionValue);
	
	void ErodeWater(ZShadeSandboxTerrain::WaterErosionParameters wep);

	void Flatten(float flatHeight);
	
private:
	
	void BuildFromProcGen();
	
	ProceduralGenerator* mProcGen;
	
	EHeightExtension::Type m_ext;//What extention is the heightmap
	
	float m_maxHeight;
	float m_seaLevel;
	float m_height_scale;
	int m_heightmap_width;
	int m_heightmap_height;
	
	vector<HeightData> m_heightmap;
};
}
//===================================================================================================================
#endif//__HEIGHTMAP_H