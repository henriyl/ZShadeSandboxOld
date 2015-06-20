#include "ProceduralGenerator.h"
#include "ZMath.h"
#include <stdlib.h>
using ZShadeSandboxTerrain::ProceduralGenerator;
//===============================================================================================================================
//===============================================================================================================================
ProceduralGenerator::ProceduralGenerator(ZShadeSandboxTerrain::ProceduralParameters pp)
:	mProceduralParameters(pp)
{
}
//===============================================================================================================================
ProceduralGenerator::~ProceduralGenerator()
{
}
//===============================================================================================================================
void ProceduralGenerator::BuildRandomHeightmap()
{
	if (mProceduralHeightmap.size() > 0)
		mProceduralHeightmap.clear();
	
	XMFLOAT2 point(mProceduralParameters.seaLevel, mProceduralParameters.maxHeight);
	
	int index = 0;
	
	ZMath::RandomSeed();

	mProceduralHeightmap.resize(mProceduralParameters.terrainSize * mProceduralParameters.terrainSize);
	
	for (int z = 0; z < mProceduralParameters.terrainSize; z++)
	{
		for (int x = 0; x < mProceduralParameters.terrainSize; x++)
		{
			ZShadeSandboxTerrain::HeightData hd;
			
			hd.x = x;
			hd.y = ZShadeSandboxMath::ZMath::RandF<float>(point.x, point.y) * 0.01f;
			hd.z = z;
			
			mProceduralHeightmap[index++] = hd;
		}
	}
}
//===============================================================================================================================
void ProceduralGenerator::BuildPerlinNoiseHeightmap()
{
	if (mProceduralHeightmap.size() > 0)
		mProceduralHeightmap.clear();
	
	int index = 0;
	
	ZMath::RandomSeed();

	mProceduralHeightmap.resize(mProceduralParameters.terrainSize * mProceduralParameters.terrainSize);

	for (int z = 0; z < mProceduralParameters.terrainSize; z++)
	{
		for (int x = 0; x < mProceduralParameters.terrainSize; x++)
		{
			ZShadeSandboxTerrain::HeightData hd;
			
			hd.x = x;
			hd.y = PerlinValue(x, z, 1000) * 5.0f;
			hd.z = z;
			
			mProceduralHeightmap[index++] = hd;
		}
	}
}
//===============================================================================================================================
void ProceduralGenerator::BuildFieldNoiseHeightmap()
{
	if (mProceduralHeightmap.size() > 0)
		mProceduralHeightmap.clear();
	
	int index = 0;
	
	mProceduralHeightmap.resize(mProceduralParameters.terrainSize * mProceduralParameters.terrainSize);

	for (int z = 0; z < mProceduralParameters.terrainSize; z++)
	{
		for (int x = 0; x < mProceduralParameters.terrainSize; x++)
		{
			ZShadeSandboxTerrain::HeightData hd;
			
			hd.x = x;
			hd.y = (float)sin(x);
			hd.z = z;
			
			mProceduralHeightmap[index++] = hd;
		}
	}
}
//===============================================================================================================================
void ProceduralGenerator::BuildCustumNoiseHeightmap()
{
	
}
//===============================================================================================================================
void ProceduralGenerator::BuildDiamondSquare()
{
	if (mProceduralHeightmap.size() > 0)
		mProceduralHeightmap.clear();

	ZMath::RandomSeed();

	int index = 0;

	int featureSize = (mProceduralParameters.diamondSquareFeatureSize / 2);

	mProceduralHeightmap.resize(mProceduralParameters.terrainSize * mProceduralParameters.terrainSize);

	for (int z = 0; z < mProceduralParameters.terrainSize; z += featureSize)
	{
		for (int x = 0; x < mProceduralParameters.terrainSize; x += featureSize)
		{
			ZShadeSandboxTerrain::HeightData hd;

			hd.x = x;
			hd.y = ZShadeSandboxMath::ZMath::RandF<float>(mProceduralParameters.seaLevel, mProceduralParameters.maxHeight);
			hd.z = z;

			mProceduralHeightmap[index++] = hd;
		}
	}

	float scale = mProceduralParameters.diamondSquareScale;

	while (featureSize > 1)
	{
		int halfStep = featureSize / 2;

		//
		// Perform the Square step
		//

		for (int z = halfStep; z < mProceduralParameters.terrainSize + halfStep; z += featureSize)
		{
			for (int x = halfStep; x < mProceduralParameters.terrainSize + halfStep; x += featureSize)
			{
				int hs = featureSize / 2;

				// a   b
				//   x
				// c   d

				float a = ReadProceduralHeight(x - hs, z - hs);
				float b = ReadProceduralHeight(x + hs, z - hs);
				float c = ReadProceduralHeight(x - hs, z + hs);
				float d = ReadProceduralHeight(x + hs, z + hs);

				float height = ((a + b + c + d) / 4) + (ZShadeSandboxMath::ZMath::RandF<float>(mProceduralParameters.seaLevel, mProceduralParameters.maxHeight) * scale);

				mProceduralHeightmap[(z * mProceduralParameters.terrainSize) + x].y = height;
			}
		}

		//
		// Perform the Diamond step
		//

		for (int z = 0; z < mProceduralParameters.terrainSize; z += featureSize)
		{
			for (int x = 0; x < mProceduralParameters.terrainSize; x += featureSize)
			{
				int hs = featureSize / 2;

				//   c
				// a x b
				//   d

				float a = ReadProceduralHeight((x + halfStep) - hs, z);
				float b = ReadProceduralHeight((x + halfStep) + hs, z);
				float c = ReadProceduralHeight((x + halfStep), z - hs);
				float d = ReadProceduralHeight((x + halfStep), z + hs);

				float height = ((a + b + c + d) / 4) + (ZShadeSandboxMath::ZMath::RandF<float>(mProceduralParameters.seaLevel, mProceduralParameters.maxHeight) * scale);

				mProceduralHeightmap[(z * mProceduralParameters.terrainSize) + (x + halfStep)].y = height;

				//   c
				// a x b
				//   d

				a = ReadProceduralHeight(x - hs, (z + halfStep));
				b = ReadProceduralHeight(x + hs, (z + halfStep));
				c = ReadProceduralHeight(x, (z + halfStep) - hs);
				d = ReadProceduralHeight(x, (z + halfStep) + hs);

				height = ((a + b + c + d) / 4) + (ZShadeSandboxMath::ZMath::RandF<float>(mProceduralParameters.seaLevel, mProceduralParameters.maxHeight) * scale);

				mProceduralHeightmap[((z + halfStep) * mProceduralParameters.terrainSize) + x].y = height;
			}
		}

		featureSize /= 2;
		scale /= 2.0f;
	}
}
//===============================================================================================================================
void ProceduralGenerator::AddRandomHeightmap(vector<ZShadeSandboxTerrain::HeightData> heightmapInput)
{
	if (mProceduralHeightmap.size() > 0)
		mProceduralHeightmap.clear();

	XMFLOAT2 point(mProceduralParameters.seaLevel, mProceduralParameters.maxHeight);

	int index = 0;
	
	ZMath::RandomSeed();
	
	mProceduralHeightmap.resize(mProceduralParameters.terrainSize * mProceduralParameters.terrainSize);

	for (int z = 0; z < mProceduralParameters.terrainSize; z++)
	{
		for (int x = 0; x < mProceduralParameters.terrainSize; x++)
		{
			ZShadeSandboxTerrain::HeightData hd;

			hd.x = x;
			hd.y = heightmapInput[(z * mProceduralParameters.terrainSize) + x].y + ZShadeSandboxMath::ZMath::RandF<float>(point.x, point.y) * 0.01f;
			hd.z = z;

			mProceduralHeightmap[index++] = hd;
		}
	}
}
//===============================================================================================================================
void ProceduralGenerator::AddPerlinNoiseHeightmap(vector<ZShadeSandboxTerrain::HeightData> heightmapInput)
{
	if (mProceduralHeightmap.size() > 0)
		mProceduralHeightmap.clear();

	int index = 0;

	ZMath::RandomSeed();

	mProceduralHeightmap.resize(mProceduralParameters.terrainSize * mProceduralParameters.terrainSize);

	for (int z = 0; z < mProceduralParameters.terrainSize; z++)
	{
		for (int x = 0; x < mProceduralParameters.terrainSize; x++)
		{
			ZShadeSandboxTerrain::HeightData hd;

			hd.x = x;
			hd.y = heightmapInput[(z * mProceduralParameters.terrainSize) + x].y + PerlinValue(x, z, 1000) * 5.0f;
			hd.z = z;

			mProceduralHeightmap[index++] = hd;
		}
	}
}
//===============================================================================================================================
void ProceduralGenerator::AddFieldNoiseHeightmap(vector<ZShadeSandboxTerrain::HeightData> heightmapInput)
{
	if (mProceduralHeightmap.size() > 0)
		mProceduralHeightmap.clear();

	int index = 0;

	ZMath::RandomSeed();

	mProceduralHeightmap.resize(mProceduralParameters.terrainSize * mProceduralParameters.terrainSize);

	for (int z = 0; z < mProceduralParameters.terrainSize; z++)
	{
		for (int x = 0; x < mProceduralParameters.terrainSize; x++)
		{
			ZShadeSandboxTerrain::HeightData hd;

			hd.x = x;
			hd.y = heightmapInput[(z * mProceduralParameters.terrainSize) + x].y + (float)sin(x);
			hd.z = z;

			mProceduralHeightmap[index++] = hd;
		}
	}
}
//===============================================================================================================================
void ProceduralGenerator::AddCustumNoiseHeightmap(vector<ZShadeSandboxTerrain::HeightData> heightmapInput)
{
	
}
//===============================================================================================================================
float ProceduralGenerator::ReadProceduralHeight(int x, int z)
{
	if (x < mProceduralParameters.terrainSize && z < mProceduralParameters.terrainSize && x >= 0 && z >= 0)
	{
		return mProceduralHeightmap[(z * mProceduralParameters.terrainSize) + x].y;
	}

	return 0;
}
//===============================================================================================================================
float ProceduralGenerator::ReadProceduralHeight(int index)
{
	return mProceduralHeightmap[index].y;
}
//===============================================================================================================================
void ProceduralGenerator::UpdateProceduralHeight(int x, int z, float height)
{
	mProceduralHeightmap[(z * mProceduralParameters.terrainSize) + x].y = height;
}
//===============================================================================================================================
float ProceduralGenerator::PerlinValue(int x, int y, int random)
{
	int n = x + y * 57 + (rand() % random) * 131;
	n = (n << 13) ^ n;
	return (1.0f - ((n * (SQR(n) * 15731 + 789221) + 1376312589) & 0x7fffffff) * 0.000000000931322574615478515625f);
}
//===============================================================================================================================
