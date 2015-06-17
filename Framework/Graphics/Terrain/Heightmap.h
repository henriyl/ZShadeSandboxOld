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
//===================================================================================================================
namespace ZShadeSandboxTerrain {
class Heightmap
{
public:
	
	Heightmap(string heightmap, int width, int height, float heightScale, float seaLevel, float maxHeight);
	Heightmap(int width, int height, float heightScale, float seaLevel, float maxHeight);
	Heightmap(const Heightmap& o);
	~Heightmap();

	float SampleHeight(int x, int z);
	float SampleHeight(int index);
	void UpdateHeightValues(float heightScale, float zScale);
	bool LoadElevation(string heightmap);
	void LoadProceduralElevation();

	// Returns the height in a safe way
	float GetHeight(float x, float z);
	bool InBounds(int x, int z);
	bool InHeightmap(float x, float z);

	int Width() { return m_heightmap_width; }
	int Height() { return m_heightmap_height; }

	vector<HeightData> GetHeightmap() { return m_heightmap; }

private:
	
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