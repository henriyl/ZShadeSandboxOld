#include "HeightErosion.h"
#include "CGlobal.h"
using ZShadeSandboxTerrain::HeightErosion;
//===============================================================================================================================
//===============================================================================================================================
HeightErosion::HeightErosion(vector<ZShadeSandboxTerrain::HeightData> heightMapInput, int terrainSize, float erosionValue)
:	mTerrainSize(terrainSize)
,	mErosionValue(erosionValue)
{
	int index = 0;
	
	mHeightMap.resize(mTerrainSize * mTerrainSize);
	
	for (int z = 0; z < mTerrainSize; z++)
	{
		for (int x = 0; x < mTerrainSize; x++)
		{
			ZShadeSandboxTerrain::HeightData hd;
			
			hd.x = x;
			hd.y = heightMapInput[(z * mTerrainSize) + x].y;
			hd.z = z;
			
			mHeightMap[index++] = hd;
		}
	}
	
	Erode();
}
//===============================================================================================================================
HeightErosion::~HeightErosion()
{
}
//===============================================================================================================================
void HeightErosion::Erode()
{
	/*
		What you are doing here is basically interpolating between the height in the previous array element with the current array element.
		Whatever percentage the ErosionValue is, you will use that percentage of the PreviousValue and add that to the inverse percentage
		of the current value. So, let's say the erosion value is 0.3. That represents 30%. So the formula becomes "take 30% of the
		previous value and combine that with 70% of the current value". A value of 1.0 would be 100% and you would get a flat plain
		because it would "smear" the first value 100% into all the other values into the row or column. A value of 0.0 would be 0% and
		the result would be 0% of the previous value and 100% of the current value. The final result would be absolutely no change to
		the row or column, and ultimately no change to the shape of the terrain. So, you can hopefully see that this ErosionValue is a
		percentage of how flat you want the terrain to be.

		Believe it or not, that's all there is to this algorithm. You simply process every row in the array from left to right using this
		formula, then do it again from right to left, then do it again from top to bottom using columns instead of rows, and then do it
		again from bottom to top using columns instead of rows. And your terrain will be "eroded" by whatever percentage you used for
		the ErosionValue.
	*/
	
	int index = 0;
	float currHeight;
	float prevHeight;
	float erosion;
	
	if (mErosionMap.size() > 0) mErosionMap.clear();
	mErosionMap.resize(mTerrainSize * mTerrainSize);

	// Process the values from east to west
	for (int z = 0; z < mTerrainSize; z++)
	{
		for (int x = 0; x < mTerrainSize; x++)
		{
			currHeight = ReadHeight(x, z);
			prevHeight = ReadHeight(x - 1, z);
			
			// Could be that the next or prev was not on the map
			if (currHeight == 0) currHeight = ReadHeight(x, z);
			if (prevHeight == 0) prevHeight = ReadHeight(x, z);
			
			erosion = CalculateErosionValue(currHeight, prevHeight);
			
			ZShadeSandboxTerrain::HeightData hd;
			
			hd.x = x;
			hd.y = erosion;
			hd.z = z;
			
			mErosionMap[index++] = hd;
		}
	}
	
	// Rebuild the heightmap from the erosion map that was generated on the prev pass
	RebuildHeightmap();

	index = 0;
	
	// Process the values from west to east
	for (int z = 0; z < mTerrainSize; z++)
	{
		//for (int x = mTerrainSize - 1; x >= 0; x--)
		for (int x = 0; x < mTerrainSize; x++)
		{
			currHeight = ReadHeight(x, z);
			prevHeight = ReadHeight(x + 1, z);
			
			// Could be that the next or prev was not on the map
			if (currHeight == 0) currHeight = ReadHeight(x, z);
			if (prevHeight == 0) prevHeight = ReadHeight(x, z);
			
			erosion = CalculateErosionValue(currHeight, prevHeight);
			
			ZShadeSandboxTerrain::HeightData hd;
			
			hd.x = x;
			hd.y = erosion;
			hd.z = z;
			
			mErosionMap[index++] = hd;
		}
	}
	
	// Rebuild the heightmap from the erosion map that was generated on the prev pass
	RebuildHeightmap();

	index = 0;
	
	// Process the values from north to south
	//for (int x = 0; x < mTerrainSize; x++)
	//{
	//	for (int z = 0; z < mTerrainSize; z++)
	//	{
	for (int z = 0; z < mTerrainSize; z++)
	{
		for (int x = 0; x < mTerrainSize; x++)
		{
			currHeight = ReadHeight(x, z - 1);
			prevHeight = ReadHeight(x, z);
			
			// Could be that the next or prev was not on the map
			if (currHeight == 0) currHeight = ReadHeight(x, z);
			if (prevHeight == 0) prevHeight = ReadHeight(x, z);
			
			erosion = CalculateErosionValue(currHeight, prevHeight);
			
			ZShadeSandboxTerrain::HeightData hd;
			
			hd.x = x;
			hd.y = erosion;
			hd.z = z;
			
			mErosionMap[index++] = hd;
		}
	}
	
	// Rebuild the heightmap from the erosion map that was generated on the prev pass
	RebuildHeightmap();

	index = 0;
	
	// Process the values from south to north
	//for (int x = 0; x < mTerrainSize; x++)
	//{
	//	for (int z = mTerrainSize - 1; z >= 0; z--)
	//	{
	for (int z = 0; z < mTerrainSize; z++)
	{
		for (int x = 0; x < mTerrainSize; x++)
		{
			currHeight = ReadHeight(x, z);
			prevHeight = ReadHeight(x, z + 1);
			
			// Could be that the next or prev was not on the map
			if (currHeight == 0) currHeight = ReadHeight(x, z);
			if (prevHeight == 0) prevHeight = ReadHeight(x, z);
			
			erosion = CalculateErosionValue(currHeight, prevHeight);
			
			ZShadeSandboxTerrain::HeightData hd;
			
			hd.x = x;
			hd.y = erosion;
			hd.z = z;
			
			mErosionMap[index++] = hd;
		}
	}
}
//===============================================================================================================================
void HeightErosion::RebuildHeightmap()
{
	if (mHeightMap.size() > 0) mHeightMap.clear();
	mHeightMap.resize(mTerrainSize * mTerrainSize);
	int index = 0;
	for (int z = 0; z < mTerrainSize; z++)
	{
		for (int x = 0; x < mTerrainSize; x++)
		{
			ZShadeSandboxTerrain::HeightData hd;
			
			hd.x = x;
			hd.y = mErosionMap[(z * mTerrainSize) + x].y;
			hd.z = z;
			
			mHeightMap[index++] = hd;
		}
	}
	if (mErosionMap.size() > 0) mErosionMap.clear();
	mErosionMap.resize(mTerrainSize * mTerrainSize);
}
//===============================================================================================================================
float HeightErosion::ReadErosionHeight(int x, int z)
{
	if (x < mTerrainSize && z < mTerrainSize && x >= 0 && z >= 0)
	{
		return mErosionMap[(z * mTerrainSize) + x].y;
	}
	
	return 0;
}
//===============================================================================================================================
float HeightErosion::ReadHeight(int x, int z)
{
	if (x < mTerrainSize && z < mTerrainSize && x >= 0 && z >= 0)
	{
		return mHeightMap[(z * mTerrainSize) + x].y;
	}
	
	return 0;
}
//===============================================================================================================================
float HeightErosion::CalculateErosionValue(float currHeight, float prevHeight)
{
	return mErosionValue * prevHeight + (1 - mErosionValue) * currHeight;
}
//===============================================================================================================================