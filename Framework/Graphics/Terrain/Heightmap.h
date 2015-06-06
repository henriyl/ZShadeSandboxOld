//=====================================================================================
//Heightmap.h
//=====================================================================================
#ifndef __HEIGHTMAP_H
#define __HEIGHTMAP_H
//===================================================================================================================
#include "D3D.h"
//===================================================================================================================
namespace ZShadeSandboxTerrain {
struct HEIGHT_DATA
{
	float x;
	float y;
	float z;
};
//===================================================================================================================
namespace EHeightExtension
{
	enum
	{
		HENONE,
		BMP,
		RAW
	};
}
//===================================================================================================================
class Heightmap
{
public:
	Heightmap(string heightmap, int extension, int width, int height, float cellSpacing, float heightScale, int leafSize);
	Heightmap(const Heightmap& o);
	~Heightmap();

	float SampleHeight(int x, int z);
	float SampleHeight(int index);

	void UpdateHeightValues(float heightScale, float zScale);

	bool LoadElevation(string heightmap);
	
	////This will help the heightmap not look to spikey, (DONT KNOW IF NEED)
	//void Normalize();
	////This is used by the data loaded in by RAW
	//float GetHeight(float x, float z);
	//void Smooth();
	//float Average(int x, int z);

	// Returns the height in a safe way
	float GetHeight(float x, float z);
	bool InBounds(int x, int z);
	bool InHeightmap(float x, float z);

	int Width() { return m_heightmap_width; }
	int Height() { return m_heightmap_height; }

private:
	int m_ext;//What extention is the heightmap

	float m_cellSpacing;
	float m_height_scale;
	int m_heightmap_width;
	int m_heightmap_height;
	
	//float** m_height_data;

	vector<HEIGHT_DATA> m_heightmap;

	//HEIGHT_DATA* m_height_data;
};
}
//===================================================================================================================
#endif//__HEIGHTMAP_H