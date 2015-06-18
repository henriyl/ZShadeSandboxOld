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
#include "WaterErosion.h"

//===============================================================================================================================
//===============================================================================================================================
namespace ZShadeSandboxTerrain {
namespace EProceduralType
{
	enum Type
	{
		eRandom,
		ePerlinNoise,
		eFieldNoise,
		eDiamondSquare,
		eNone
	};
}

namespace EErosionType
{
	enum Type
	{
		eHeight,
		eWater,
		eNone
	};
}

struct ProceduralParameters
{
	int terrainSize;
	float seaLevel;
	float maxHeight;
	float erosionValue;
	int smoothingPassCount;
	float normalizeFactor;
	int diamondSquareFeatureSize;
	int diamondSquareScale;
	EProceduralType::Type proceduralType;
	EErosionType::Type erosionType;
	bool useErosion;
	bool useSmoothing;
	bool normalize;

	WaterErosionParameters waterErosionParameters;
	
	ProceduralParameters()
	{
		terrainSize = 256;
		seaLevel = 0;
		maxHeight = 1024;
		erosionValue = 0.3f;
		smoothingPassCount = 1.0f;
		normalizeFactor = 1.0f;
		diamondSquareFeatureSize = 16;
		diamondSquareScale = 1;
		proceduralType = EProceduralType::Type::eNone;
		erosionType = EErosionType::Type::eNone;
		useErosion = false;
		useSmoothing = false;
		normalize = false;
	}
};

class ProceduralGenerator
{
public:
	
	ProceduralGenerator(ProceduralParameters pp);
	~ProceduralGenerator();
	
	void BuildRandomHeightmap();
	void BuildPerlinNoiseHeightmap();
	void BuildFieldNoiseHeightmap();
	void BuildCustumNoiseHeightmap();
	void BuildDiamondSquare();

	// These are added to an existing heightmap input
	void AddRandomHeightmap(vector<HeightData> heightmapInput);
	void AddPerlinNoiseHeightmap(vector<HeightData> heightmapInput);
	void AddFieldNoiseHeightmap(vector<HeightData> heightmapInput);
	void AddCustumNoiseHeightmap(vector<HeightData> heightmapInput);

	// Returns the generated procedural map
	vector<HeightData> GetProceduralMap() { return mProceduralHeightmap; }
	
	float ReadProceduralHeight(int x, int z);
	float ReadProceduralHeight(int index);
	void UpdateProceduralHeight(int x, int z, float height);
	
private:
	
	float PerlinValue(int x, int y, int random);
	
private:
	
	// The resulting height map after it has been affected procedurally
	vector<HeightData> mProceduralHeightmap;
	
	ProceduralParameters mProceduralParameters;
};
}
//===============================================================================================================================
#endif//__PROCEDURALGENERATOR_H