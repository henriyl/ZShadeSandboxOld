#include "ProceduralGenerator.h"
#include "ZMath.h"
#include <stdlib.h>
using ZShadeSandboxTerrain::ProceduralGenerator;
//===============================================================================================================================
//===============================================================================================================================
ProceduralGenerator::ProceduralGenerator(float terrainSize, float seaLevel, float maxHeight)
:	fSeaLevel(seaLevel)
,	fTerrainSize(terrainSize)
,	fMaxHeight(maxHeight)
{
}
//===============================================================================================================================
ProceduralGenerator::ProceduralGenerator(float terrainSize, float seaLevel, float maxHeight, vector<ZShadeSandboxTerrain::HeightData> heightMapInput)
:	mHeightmapInput(heightMapInput)
,	fSeaLevel(seaLevel)
,	fTerrainSize(terrainSize)
,	fMaxHeight(maxHeight)
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
	
	XMFLOAT2 point(fSeaLevel, fMaxHeight);
	
	int index = 0;
	
	mProceduralHeightmap.resize(fTerrainSize * fTerrainSize);
	
	srand(0);
	
	for (int z = 0; z < fTerrainSize; z++)
	{
		for (int x = 0; x < fTerrainSize; x++)
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
	
	mProceduralHeightmap.resize(fTerrainSize * fTerrainSize);
	
	srand(0);

	for (int z = 0; z < fTerrainSize; z++)
	{
		for (int x = 0; x < fTerrainSize; x++)
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
	
	mProceduralHeightmap.resize(fTerrainSize * fTerrainSize);
	
	for (int z = 0; z < fTerrainSize; z++)
	{
		for (int x = 0; x < fTerrainSize; x++)
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

	srand(0);

	int index = 0;

	mProceduralHeightmap.resize(fTerrainSize * fTerrainSize);

	int featureSize = fTerrainSize / 2;

	for (int z = 0; z < fTerrainSize; z += featureSize)
	{
		for (int x = 0; x < fTerrainSize; x += featureSize)
		{
			ZShadeSandboxTerrain::HeightData hd;

			hd.x = x;
			hd.y = ZShadeSandboxMath::ZMath::RandF<float>(-1, 1);
			hd.z = z;

			mProceduralHeightmap[index++] = hd;
		}
	}

	float scale = 1.0f;

	while (featureSize > 1)
	{
		int halfStep = featureSize / 2;

		//
		// Perform the Square step
		//

		for (int z = halfStep; z < fTerrainSize + halfStep; z += featureSize)
		{
			for (int x = halfStep; x < fTerrainSize + halfStep; x += featureSize)
			{
				int hs = featureSize / 2;

				// a   b
				//   x
				// c   d

				float a = ReadProceduralHeight(x - hs, z - hs);
				float b = ReadProceduralHeight(x + hs, z - hs);
				float c = ReadProceduralHeight(x - hs, z + hs);
				float d = ReadProceduralHeight(x + hs, z + hs);

				float height = ((a + b + c + d) / 4) + (ZShadeSandboxMath::ZMath::RandF<float>(-1, 1) * scale);

				mProceduralHeightmap[(z * fTerrainSize) + x].y = height;
			}
		}

		//
		// Perform the Diamond step
		//

		for (int z = 0; z < fTerrainSize; z += featureSize)
		{
			for (int x = 0; x < fTerrainSize; x += featureSize)
			{
				int hs = featureSize / 2;

				//   c
				// a x b
				//   d

				float a = ReadProceduralHeight((x + halfStep) - hs, z);
				float b = ReadProceduralHeight((x + halfStep) + hs, z);
				float c = ReadProceduralHeight((x + halfStep), z - hs);
				float d = ReadProceduralHeight((x + halfStep), z + hs);

				float height = ((a + b + c + d) / 4) + (ZShadeSandboxMath::ZMath::RandF<float>(0, 100) * scale);

				mProceduralHeightmap[(z * fTerrainSize) + (x + halfStep)].y = height;

				//   c
				// a x b
				//   d

				a = ReadProceduralHeight(x - hs, (z + halfStep));
				b = ReadProceduralHeight(x + hs, (z + halfStep));
				c = ReadProceduralHeight(x, (z + halfStep) - hs);
				d = ReadProceduralHeight(x, (z + halfStep) + hs);

				height = ((a + b + c + d) / 4) + (ZShadeSandboxMath::ZMath::RandF<float>(0, 100) * scale);

				mProceduralHeightmap[((z + halfStep) * fTerrainSize) + x].y = height;
			}
		}

		featureSize /= 2;
		scale /= 2.0f;
	}
}
//===============================================================================================================================
void ProceduralGenerator::AddRandomHeightmap()
{
	XMFLOAT2 point(fSeaLevel, fMaxHeight);

	int index = 0;

	srand(0);

	for (int z = 0; z < fTerrainSize; z++)
	{
		for (int x = 0; x < fTerrainSize; x++)
		{
			mHeightmapInput[index++].y += ZShadeSandboxMath::ZMath::RandF<float>(point.x, point.y) * 0.01f;
		}
	}

	index = 0;

	mProceduralHeightmap.resize(fTerrainSize * fTerrainSize);

	for (int z = 0; z < fTerrainSize; z++)
	{
		for (int x = 0; x < fTerrainSize; x++)
		{
			ZShadeSandboxTerrain::HeightData hd;

			hd.x = x;
			hd.y = mHeightmapInput[(z * fTerrainSize) + x].y;
			hd.z = z;

			mProceduralHeightmap[index++] = hd;
		}
	}
}
//===============================================================================================================================
void ProceduralGenerator::AddPerlinNoiseHeightmap()
{
	int index = 0;

	srand(0);

	for (int z = 0; z < fTerrainSize; z++)
	{
		for (int x = 0; x < fTerrainSize; x++)
		{
			mHeightmapInput[index++].y += PerlinValue(x, z, 1000) * 5.0f;
		}
	}

	index = 0;

	mProceduralHeightmap.resize(fTerrainSize * fTerrainSize);

	for (int z = 0; z < fTerrainSize; z++)
	{
		for (int x = 0; x < fTerrainSize; x++)
		{
			ZShadeSandboxTerrain::HeightData hd;

			hd.x = x;
			hd.y = mHeightmapInput[(z * fTerrainSize) + x].y;
			hd.z = z;

			mProceduralHeightmap[index++] = hd;
		}
	}
}
//===============================================================================================================================
void ProceduralGenerator::AddFieldNoiseHeightmap()
{
	int index = 0;

	for (int z = 0; z < fTerrainSize; z++)
	{
		for (int x = 0; x < fTerrainSize; x++)
		{
			mHeightmapInput[index++].y += (float)sin(x);
		}
	}

	index = 0;

	mProceduralHeightmap.resize(fTerrainSize * fTerrainSize);

	for (int z = 0; z < fTerrainSize; z++)
	{
		for (int x = 0; x < fTerrainSize; x++)
		{
			ZShadeSandboxTerrain::HeightData hd;

			hd.x = x;
			hd.y = mHeightmapInput[(z * fTerrainSize) + x].y;
			hd.z = z;

			mProceduralHeightmap[index++] = hd;
		}
	}
}
//===============================================================================================================================
void ProceduralGenerator::AddCustumNoiseHeightmap()
{

}
//===============================================================================================================================
void ProceduralGenerator::Smooth(int smoothingPassCount)
{
	if (smoothingPassCount > 0)
	{
		for (int i = 0; i < smoothingPassCount; i++)
		{
			Smooth();
		}
	}
}
//===============================================================================================================================
void ProceduralGenerator::Smooth()
{
	int index = 0;

	vector<HeightData> tempHeightmap;

	tempHeightmap.resize(fTerrainSize * fTerrainSize);

	for (int z = 0; z < fTerrainSize; z++)
	{
		for (int x = 0; x < fTerrainSize; x++)
		{
			float averageHeight = 0;
			float count = 0;

			for (int m = x - 1; m <= x + 1; m++)
			{
				for (int n = z - 1; n <= z + 1; n++)
				{
					if (m >= 0 && m < fTerrainSize && n >= 0 && n < fTerrainSize)
					{
						averageHeight += ReadProceduralHeight(n, m);
						count += 1;
					}
				}
			}

			ZShadeSandboxTerrain::HeightData hd;

			hd.x = x;
			hd.y = averageHeight / count;
			hd.z = z;

			tempHeightmap[index++] = hd;
		}
	}

	if (mProceduralHeightmap.size() > 0) mProceduralHeightmap.clear();
	mProceduralHeightmap.resize(fTerrainSize * fTerrainSize);
	
	index = 0;

	for (int z = 0; z < fTerrainSize; z++)
	{
		for (int x = 0; x < fTerrainSize; x++)
		{
			ZShadeSandboxTerrain::HeightData hd;

			hd.x = x;
			hd.y = tempHeightmap[(z * fTerrainSize) + x].y;
			hd.z = z;

			mProceduralHeightmap[index++] = hd;
		}
	}
}
//===============================================================================================================================
void ProceduralGenerator::Normalize(float normalizeFactor)
{
	int index = 0;

	for (int z = 0; z < fTerrainSize; z++)
	{
		for (int x = 0; x < fTerrainSize; x++)
		{
			mProceduralHeightmap[index++].y /= normalizeFactor;
		}
	}
}
//===============================================================================================================================
void ProceduralGenerator::ErodeHeightmapInput(float erosionValue)
{
	HeightErosion he(mHeightmapInput, fTerrainSize, erosionValue);
	
	int index = 0;

	// Get the resulting erosion map
	mProceduralHeightmap.resize(fTerrainSize * fTerrainSize);
	
	for (int z = 0; z < fTerrainSize; z++)
	{
		for (int x = 0; x < fTerrainSize; x++)
		{
			ZShadeSandboxTerrain::HeightData hd;
			
			hd.x = x;
			hd.y = he.ReadErosionHeight(x, z);
			hd.z = z;
			
			mProceduralHeightmap[index++] = hd;
		}
	}
}
//===============================================================================================================================
void ProceduralGenerator::ErodeProceduralMap(float erosionValue)
{
	HeightErosion he(mProceduralHeightmap, fTerrainSize, erosionValue);
	
	// Get the resulting erosion map
	int index = 0;
	
	if (mProceduralHeightmap.size() > 0) mProceduralHeightmap.clear();
	mProceduralHeightmap.resize(fTerrainSize * fTerrainSize);
	
	for (int z = 0; z < fTerrainSize; z++)
	{
		for (int x = 0; x < fTerrainSize; x++)
		{
			ZShadeSandboxTerrain::HeightData hd;
			
			hd.x = x;
			hd.y = he.ReadErosionHeight(x, z);
			hd.z = z;
			
			mProceduralHeightmap[index++] = hd;
		}
	}
}
//===============================================================================================================================
float ProceduralGenerator::ReadProceduralHeight(int x, int z)
{
	if (x < fTerrainSize && z < fTerrainSize && x >= 0 && z >= 0)
	{
		return mProceduralHeightmap[(z * fTerrainSize) + x].y;
	}

	return 0;
}
//===============================================================================================================================
float ProceduralGenerator::ReadProceduralHeight(int index)
{
	return mProceduralHeightmap[index].y;
}
//===============================================================================================================================
float ProceduralGenerator::PerlinValue(int x, int y, int random)
{
	int n = x + y * 57 + (rand() % random) * 131;
	n = (n << 13) ^ n;
	return (1.0f - ((n * (SQR(n) * 15731 + 789221) + 1376312589) & 0x7fffffff) * 0.000000000931322574615478515625f);
}
//===============================================================================================================================
