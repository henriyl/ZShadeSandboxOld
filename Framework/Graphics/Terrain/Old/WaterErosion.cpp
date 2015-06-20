#include "WaterErosion.h"
using ZShadeSandboxTerrain::WaterErosion;
//===============================================================================================================================
//===============================================================================================================================
WaterErosion::WaterErosion(vector<ZShadeSandboxTerrain::HeightData> heightMapInput, ZShadeSandboxTerrain::WaterErosionParameters wep)
:	mWaterErosionParameters(wep)
{
	mErosionMap.resize(mWaterErosionParameters.terrainSize * mWaterErosionParameters.terrainSize);
	mHeightMapInput.resize(mWaterErosionParameters.terrainSize * mWaterErosionParameters.terrainSize);

	for (int z = 0; z < mWaterErosionParameters.terrainSize; z++)
	{
		for (int x = 0; x < mWaterErosionParameters.terrainSize; x++)
		{
			UpdateHeightMapInput(x, z, heightMapInput[(z * mWaterErosionParameters.terrainSize) + x].y);
		}
	}
	
	mWaterMap.resize(mWaterErosionParameters.terrainSize * mWaterErosionParameters.terrainSize);

	for (int z = 0; z < mWaterErosionParameters.terrainSize; z++)
	{
		for (int x = 0; x < mWaterErosionParameters.terrainSize; x++)
		{
			UpdateWaterMap(x, z, mWaterErosionParameters.seaLevel);
		}
	}
	
	mSedimentationMap.resize(mWaterErosionParameters.terrainSize * mWaterErosionParameters.terrainSize);
	mOutflowFluxMap.resize(mWaterErosionParameters.terrainSize * mWaterErosionParameters.terrainSize);
	mVelocityVectorMap.resize(mWaterErosionParameters.terrainSize * mWaterErosionParameters.terrainSize);
	
	Erode();
}
//===============================================================================================================================
WaterErosion::~WaterErosion()
{
}
//===============================================================================================================================
void WaterErosion::Erode()
{
	for (int z = 0; z < mWaterErosionParameters.terrainSize; z++)
	{
		for (int x = 0; x < mWaterErosionParameters.terrainSize; x++)
		{
			UpdateWaterMap(x, z, 0);
			UpdateOutflowFluxMap(x, z, OutflowValues(0, 0, 0, 0));
			UpdateSedimentationMap(x, z, 0);
		}
	}
	
	int waterAmount = mWaterErosionParameters.waterSourceHeight;

	for (float time = mWaterErosionParameters.erosionDuration; time > 0; time -= mWaterErosionParameters.deltaT)
	{
		mWaterErosionParameters.waterSourceHeight = 0.05f * waterAmount;

		SetWaterSource();

		BuildVelocityVectorMap();

		ApplyErosion();

		ApplyThermalWeathering();

		ApplyEvaporation();

		float maxLength = 0;
		for (int i = 0; i < mWaterErosionParameters.terrainSize; i++)
		{
			float length = sqrt(mVelocityVectorMap[i].x * mVelocityVectorMap[i].x + mVelocityVectorMap[i].y * mVelocityVectorMap[i].y);
			if (length > maxLength)
			{
				maxLength = length;
			}
		}

		mWaterErosionParameters.deltaT = 1 / (1.5 * maxLength);
		mWaterErosionParameters.deltaT = min(mWaterErosionParameters.deltaT, 0.05);
	}

	//SetWaterSource();
	
	//if (mWaterErosionParameters.applyEvaporation)
	//{
	//	ApplyEvaporation();
	//}

	//ApplyErosion();
}
//===============================================================================================================================
void WaterErosion::ThermalWeather()
{
	if (mWaterErosionParameters.applyThermalWeathering)
	{
		ApplyThermalWeathering();
	}
}
//===============================================================================================================================
void WaterErosion::SetWaterSource()
{
	for (int z = 0; z < mWaterErosionParameters.terrainSize; z++)
	{
		for (int x = 0; x < mWaterErosionParameters.terrainSize; x++)
		{
			UpdateWaterMap(x, z, ReadWaterHeight(x, z) + (mWaterErosionParameters.waterSourceHeight * mWaterErosionParameters.deltaT));
		}
	}
}
//===============================================================================================================================
void WaterErosion::ApplyEvaporation()
{
	for (int z = 0; z < mWaterErosionParameters.terrainSize; z++)
	{
		for (int x = 0; x < mWaterErosionParameters.terrainSize; x++)
		{
			UpdateWaterMap(x, z, ReadWaterHeight(x, z) * 0.985f);
		}
	}
}
//===============================================================================================================================
void WaterErosion::BuildVelocityVectorMap()
{
	vector<HeightData> waterMapCopy;
	
	waterMapCopy.resize(mWaterErosionParameters.terrainSize * mWaterErosionParameters.terrainSize);
	
	for (int z = 0; z < mWaterErosionParameters.terrainSize; z++)
	{
		for (int x = 0; x < mWaterErosionParameters.terrainSize; x++)
		{
			ZShadeSandboxTerrain::HeightData hd;
			
			hd.x = x;
			hd.y = ReadWaterHeight(x, z);
			hd.z = z;
			
			waterMapCopy[(z * mWaterErosionParameters.terrainSize) + x] = hd;
		}
	}
	
	float currentHeight;
	int currentIndex;
	
	for (int z = 0; z < mWaterErosionParameters.terrainSize; z++)
	{
		for (int x = 0; x < mWaterErosionParameters.terrainSize; x++)
		{
			currentIndex = (z * mWaterErosionParameters.terrainSize) + x;
			currentHeight = ReadHeightMapInput(x, z);
			
			// Calculate outflow values for individual directions.
			OutflowValues ov;
			
			if (x > 0)
			{
				float heightDifference = currentHeight + waterMapCopy[currentIndex].y - mHeightMapInput[currentIndex - 1].y - waterMapCopy[currentIndex - 1].y;
				float flowValue = FlowValue(ReadOutflowFluxMap(x, z).left, heightDifference);
				ov.left = max(0, flowValue);
			}
			
			if (x + 1 < mWaterErosionParameters.terrainSize)
			{
				float heightDifference = currentHeight + waterMapCopy[currentIndex].y - mHeightMapInput[currentIndex + 1].y - waterMapCopy[currentIndex + 1].y;
				float flowValue = FlowValue(ReadOutflowFluxMap(x, z).right, heightDifference);
				ov.right = max(0, flowValue);
			}

			if (z > 0)
			{
				float heightDifference = currentHeight + waterMapCopy[currentIndex].y - mHeightMapInput[currentIndex - mWaterErosionParameters.terrainSize].y - waterMapCopy[currentIndex - mWaterErosionParameters.terrainSize].y;
				float flowValue = FlowValue(ReadOutflowFluxMap(x, z).top, heightDifference);
				ov.top = max(0, flowValue);
			}

			if (z + 1 < mWaterErosionParameters.terrainSize)
			{
				float heightDifference = currentHeight + waterMapCopy[currentIndex].y - mHeightMapInput[currentIndex + mWaterErosionParameters.terrainSize].y - waterMapCopy[currentIndex + mWaterErosionParameters.terrainSize].y;
				float flowValue = FlowValue(ReadOutflowFluxMap(x, z).bottom, heightDifference);
				ov.bottom = max(0, flowValue);
			}
			
			UpdateOutflowFluxMap(x, z, ov);
			
			// Scale the outflow values so sum(outflow) < amount of water here
			OutflowValues outflow = ReadOutflowFluxMap(x, z);
			float sumOutflow = outflow.left + outflow.right + outflow.top + outflow.bottom;
			
			if (sumOutflow > ReadWaterHeight(x, z))
			{
				float factor = min(1, ReadWaterHeight(x, z) * mWaterErosionParameters.pipeLength * mWaterErosionParameters.pipeLength / (sumOutflow * mWaterErosionParameters.deltaT));
				
				outflow.left *= factor;
				outflow.right *= factor;
				outflow.top *= factor;
				outflow.bottom *= factor;
				
				UpdateOutflowFluxMap(x, z, outflow);
				
				outflow = ReadOutflowFluxMap(x, z);
				sumOutflow = outflow.left + outflow.right + outflow.top + outflow.bottom;
			}
		}
	}
	
	for (int z = 0; z < mWaterErosionParameters.terrainSize; z++)
	{
		for (int x = 0; x < mWaterErosionParameters.terrainSize; x++)
		{
			currentIndex = (z * mWaterErosionParameters.terrainSize) + x;
			
			OutflowValues outflow = ReadOutflowFluxMap(x, z);
			float sumOutflow = outflow.left + outflow.right + outflow.top + outflow.bottom;
			float sumInflow = 0;
			
			if (x > 0)
			{
				sumInflow += mOutflowFluxMap[currentIndex - 1].right;
			}

			if (x + 1 < mWaterErosionParameters.terrainSize)
			{
				sumInflow += mOutflowFluxMap[currentIndex + 1].left;
			}

			if (z > 0)
			{
				sumInflow += mOutflowFluxMap[currentIndex - mWaterErosionParameters.terrainSize].bottom;
			}
			
			if (z + 1 < mWaterErosionParameters.terrainSize)
			{
				sumInflow += mOutflowFluxMap[currentIndex + mWaterErosionParameters.terrainSize].top;
			}
			
			float oldWaterLevel = waterMapCopy[currentIndex].y;
			float newWaterLevel = waterMapCopy[currentIndex].y + (mWaterErosionParameters.deltaT * (sumInflow - sumOutflow)) / SQR(mWaterErosionParameters.pipeLength);
			
			UpdateWaterMap(x, z, max(0, newWaterLevel));
			
			float waterAverage = (oldWaterLevel + ReadWaterHeight(x, z)) / 2;
			
			// The velocity field must be updated
			VelocityVector vvField;
			
			// Horizontal (x-axis) velocity field vector component
			if (x == 0)
			{
				// Left border.
				vvField.x = mOutflowFluxMap[(mWaterErosionParameters.terrainSize * z) + x].right -
							mOutflowFluxMap[(mWaterErosionParameters.terrainSize * z) + x + 1].left;
			}
			else if (x + 1 == mWaterErosionParameters.terrainSize)
			{
				// Right border.
				vvField.x = mOutflowFluxMap[(mWaterErosionParameters.terrainSize * z) + x - 1].right -
							mOutflowFluxMap[(mWaterErosionParameters.terrainSize * z) + x].left;
			}
			else
			{
				// The rest.
				vvField.x = mOutflowFluxMap[(mWaterErosionParameters.terrainSize * z) + x - 1].right -
							mOutflowFluxMap[(mWaterErosionParameters.terrainSize * z) + x].left +
							mOutflowFluxMap[(mWaterErosionParameters.terrainSize * z) + x].right -
							mOutflowFluxMap[(mWaterErosionParameters.terrainSize * z) + x + 1].left;
				
				vvField.x /= 2;
			}
			
			// Vertical (y-axis) velocity field vector component
			if (z == 0)
			{
				// Top border.
				vvField.y = mOutflowFluxMap[(mWaterErosionParameters.terrainSize * z) + x].bottom -
							mOutflowFluxMap[(mWaterErosionParameters.terrainSize * (z + 1)) + x + 1].top;
			}
			else if (z + 1 == mWaterErosionParameters.terrainSize)
			{
				// Bottom border.
				vvField.y = mOutflowFluxMap[(mWaterErosionParameters.terrainSize * (z - 1)) + x].bottom -
							mOutflowFluxMap[(mWaterErosionParameters.terrainSize * (z)) + x].top;
			}
			else
			{
				// The rest.
				vvField.y = mOutflowFluxMap[(mWaterErosionParameters.terrainSize * (z - 1)) + x].bottom -
							mOutflowFluxMap[(mWaterErosionParameters.terrainSize * z) + x].top +
							mOutflowFluxMap[(mWaterErosionParameters.terrainSize * z) + x].bottom -
							mOutflowFluxMap[(mWaterErosionParameters.terrainSize * (z + 1)) + x].top;
				
				vvField.y /= 2;
			}
			
			UpdateVelocityVectorMap(x, z, vvField);
		}
	}
	
	if (waterMapCopy.size() > 0)
		waterMapCopy.clear();
}
//===============================================================================================================================
void WaterErosion::ApplyErosion()
{
	vector<HeightData> sedimentToMoveMap;

	sedimentToMoveMap.resize(mWaterErosionParameters.terrainSize * mWaterErosionParameters.terrainSize);

	int currentIndex;

	for (int z = 0; z < mWaterErosionParameters.terrainSize; z++)
	{
		for (int x = 0; x < mWaterErosionParameters.terrainSize; x++)
		{
			currentIndex = (z * mWaterErosionParameters.terrainSize) + x;

			sedimentToMoveMap[currentIndex].x = x;
			sedimentToMoveMap[currentIndex].y = 0;
			sedimentToMoveMap[currentIndex].z = z;

			VelocityVector currentVelocityVector = ReadVelocityVectorMap(x, z);

			signed int coordinateOffsetX = currentVelocityVector.x < 0 ? -1 : 0;
			signed int coordinateOffsetY = currentVelocityVector.y < 0 ? -1 : 0;
			signed int baseCoordinateX = x + coordinateOffsetX;
			signed int baseCoordinateY = z + coordinateOffsetY;

			float coordinatePartX = currentVelocityVector.x * mWaterErosionParameters.deltaT - coordinateOffsetX;
			float coordinatePartY = currentVelocityVector.y * mWaterErosionParameters.deltaT - coordinateOffsetY;

			if (coordinatePartX < 0)
				coordinatePartX += 1;

			if (coordinatePartY < 0)
				coordinatePartY += 1;

			//
			// Calculate current height of the target point (it will likely have to be interpolated from 4 surrounding points).
			//

			float heightSum = 0;
			float heightWeightSum = 0;

			// Top Left Point
			if (baseCoordinateX < mWaterErosionParameters.terrainSize && x >= -coordinateOffsetX && baseCoordinateY < mWaterErosionParameters.terrainSize && z >= -coordinateOffsetY)
			{
				float currentWeight = (1 - coordinatePartX) * (1 - coordinatePartY);
				heightSum += mHeightMapInput[baseCoordinateX + mWaterErosionParameters.terrainSize * baseCoordinateY].y * currentWeight;
				heightWeightSum += currentWeight;
			}

			// Top Right Point
			if (baseCoordinateX + 1 < mWaterErosionParameters.terrainSize && x + 1 >= -coordinateOffsetX && baseCoordinateY < mWaterErosionParameters.terrainSize && z >= -coordinateOffsetY)
			{
				float currentWeight = coordinatePartX * (1 - coordinatePartY);
				heightSum += mHeightMapInput[baseCoordinateX + 1 + mWaterErosionParameters.terrainSize * baseCoordinateY].y * currentWeight;
				heightWeightSum += currentWeight;
			}

			// Bottom Left Point
			if (baseCoordinateX < mWaterErosionParameters.terrainSize && x >= -coordinateOffsetX && baseCoordinateY + 1 < mWaterErosionParameters.terrainSize && z + 1 >= -coordinateOffsetY)
			{
				float currentWeight = (1 - coordinatePartX) * coordinatePartY;
				heightSum += mHeightMapInput[baseCoordinateX + mWaterErosionParameters.terrainSize * (baseCoordinateY + 1)].y * currentWeight;
				heightWeightSum += currentWeight;
			}

			// Bottom Right Point
			if (baseCoordinateX + 1 < mWaterErosionParameters.terrainSize && x + 1 >= -coordinateOffsetX && baseCoordinateY + 1 < mWaterErosionParameters.terrainSize && z + 1 >= -coordinateOffsetY)
			{
				float currentWeight = coordinatePartX * coordinatePartY;
				heightSum += mHeightMapInput[baseCoordinateX + 1 + mWaterErosionParameters.terrainSize * (baseCoordinateY + 1)].y * currentWeight;
				heightWeightSum += currentWeight;
			}

			float targetPointHeightDiff = heightSum / heightWeightSum - ReadErosionHeight(x, z);

			float velocityVectorLength = sqrt(SQR(ReadVelocityVectorMap(x, z).x) + SQR(ReadVelocityVectorMap(x, z).y));

			float surfaceTilt;

			if (velocityVectorLength > 0)
			{
				surfaceTilt = atan2(targetPointHeightDiff, velocityVectorLength);
			}
			else
			{
				surfaceTilt = 0;
			}

			surfaceTilt = max(0.2f, surfaceTilt);

			float sedimentCapacity = mWaterErosionParameters.sedimentCapacityConstant * surfaceTilt * velocityVectorLength;

			float sedimentToMove = 0;

			if (sedimentCapacity < 0)
			{
				ZShadeMessageCenter::MsgBoxError(NULL, "Erosion error: Invalid sediment capacity.");
				return;
			}

			if (ReadSedimentationHeight(x, z) == 0 && sedimentCapacity <= 0)
			{
				sedimentToMoveMap[currentIndex].y = 0;
				continue;
			}

			float originalHeight = ReadHeightMapInput(x, z);

			if (mSedimentationMap[(z * mWaterErosionParameters.terrainSize) + x].y < 0)
			{
				ZShadeMessageCenter::MsgBoxError(NULL, "Erosion error: Negative sediment before update.");
				return;
			}

			float sedimentChange;

			if (sedimentCapacity > ReadSedimentationHeight(x, z))
			{
				// The water can carry more sediment - some sediment will be picked up
				sedimentChange = mWaterErosionParameters.dissolvingConstant * mWaterErosionParameters.deltaT * (sedimentCapacity - ReadSedimentationHeight(x, z));

				if (sedimentChange > 1 || sedimentChange != sedimentChange)
				{
					ZShadeMessageCenter::MsgBoxError(NULL, "Erosion error: Excessive negative sediment change.");
					return;
				}

				mHeightMapInput[currentIndex].y -= sedimentChange;
				mWaterMap[currentIndex].y += sedimentChange;
				sedimentToMove = ReadSedimentationHeight(x, z) + sedimentChange;
			}
			else if (sedimentCapacity < ReadSedimentationHeight(x, z))
			{
				// The water is over saturated with sediment - some sediment will be deposited
				sedimentChange = mWaterErosionParameters.depositionConstant * mWaterErosionParameters.deltaT * (ReadSedimentationHeight(x, z) - sedimentCapacity);

				if (sedimentChange > 1 || sedimentChange != sedimentChange)
				{
					ZShadeMessageCenter::MsgBoxError(NULL, "Erosion error: Excessive positive sediment change.");
					return;
				}

				mHeightMapInput[currentIndex].y += sedimentChange;
				mWaterMap[currentIndex].y -= sedimentChange;

				if (ReadWaterHeight(x, z) < 0)
				{
					UpdateWaterMap(x, z, 0);
				}

				sedimentToMove = ReadSedimentationHeight(x, z) - sedimentChange;
			}

			if (sedimentToMove < 0)
			{
				ZShadeMessageCenter::MsgBoxError(NULL, "Erosion error: Negative sediment to move.");
				return;
			}

			sedimentToMoveMap[currentIndex].y = sedimentToMove;
		}
	}

	for (int z = 0; z < mWaterErosionParameters.terrainSize; z++)
	{
		for (int x = 0; x < mWaterErosionParameters.terrainSize; x++)
		{
			currentIndex = (z * mWaterErosionParameters.terrainSize) + x;

			VelocityVector currentVelocityVector = ReadVelocityVectorMap(x, z);

			// Move the sediment according to the velocity field map
			if (sedimentToMoveMap[currentIndex].y == 0) continue;

			// we are doing a step backwards in time, so inverse vector has to be used
			currentVelocityVector.x *= -1;
			currentVelocityVector.y *= -1;

			signed int coordinateOffsetX = currentVelocityVector.x < 0 ? -1 : 0;
			signed int coordinateOffsetY = currentVelocityVector.y < 0 ? -1 : 0;
			signed int baseCoordinateX = x + coordinateOffsetX;
			signed int baseCoordinateY = z + coordinateOffsetY;

			float coordinatePartX = currentVelocityVector.x * mWaterErosionParameters.deltaT - coordinateOffsetX;
			float coordinatePartY = currentVelocityVector.y * mWaterErosionParameters.deltaT - coordinateOffsetY;

			if (coordinatePartX > 1 || coordinatePartY > 1 || coordinatePartX < 0 || coordinatePartY < 0)
			{
				//ZShadeMessageCenter::MsgBoxError(NULL, "E");
			}

			//
			// Calculate current height of the target point (it will likely have to be interpolated from 4 surrounding points).
			//

			float sedimentSum = 0;
			float sedimentWeightSum = 0;

			// Top Left Point
			if (baseCoordinateX < mWaterErosionParameters.terrainSize && x >= -coordinateOffsetX && baseCoordinateY < mWaterErosionParameters.terrainSize && z >= -coordinateOffsetY)
			{
				float currentWeight = (1 - coordinatePartX) * (1 - coordinatePartY);
				sedimentSum += sedimentToMoveMap[baseCoordinateX + mWaterErosionParameters.terrainSize * baseCoordinateY].y * currentWeight;
				sedimentWeightSum += currentWeight;
			}

			// Top Right Point
			if (baseCoordinateX + 1 < mWaterErosionParameters.terrainSize && x + 1 >= -coordinateOffsetX && baseCoordinateY < mWaterErosionParameters.terrainSize && z >= -coordinateOffsetY)
			{
				float currentWeight = coordinatePartX * (1 - coordinatePartY);
				sedimentSum += sedimentToMoveMap[baseCoordinateX + 1 + mWaterErosionParameters.terrainSize * baseCoordinateY].y * currentWeight;
				sedimentWeightSum += currentWeight;
			}

			// Bottom Left Point
			if (baseCoordinateX < mWaterErosionParameters.terrainSize && x >= -coordinateOffsetX && baseCoordinateY + 1 < mWaterErosionParameters.terrainSize && z + 1 >= -coordinateOffsetY)
			{
				float currentWeight = (1 - coordinatePartX) * coordinatePartY;
				sedimentSum += sedimentToMoveMap[baseCoordinateX + mWaterErosionParameters.terrainSize * (baseCoordinateY + 1)].y * currentWeight;
				sedimentWeightSum += currentWeight;
			}

			// Bottom Right Point
			if (baseCoordinateX + 1 < mWaterErosionParameters.terrainSize && x + 1 >= -coordinateOffsetX && baseCoordinateY + 1 < mWaterErosionParameters.terrainSize && z + 1 >= -coordinateOffsetY)
			{
				float currentWeight = coordinatePartX * coordinatePartY;
				sedimentSum += sedimentToMoveMap[baseCoordinateX + 1 + mWaterErosionParameters.terrainSize * (baseCoordinateY + 1)].y * currentWeight;
				sedimentWeightSum += currentWeight;
			}

			if (sedimentSum < 0)
			{
				ZShadeMessageCenter::MsgBoxError(NULL, "Erosion error: Negative interpolated sediment value.");
				return;
			}

			UpdateSedimentationMap(x, z, sedimentWeightSum > 0 ? sedimentSum / sedimentWeightSum : 0);
		}
	}

	//
	// Add Sedimentation to erosion map
	//

	for (int z = 0; z < mWaterErosionParameters.terrainSize; z++)
	{
		for (int x = 0; x < mWaterErosionParameters.terrainSize; x++)
		{
			UpdateErosionMap(x, z, ReadSedimentationHeight(x, z));
		}
	}

	if (sedimentToMoveMap.size() > 0)
		sedimentToMoveMap.clear();
}
//===============================================================================================================================
void WaterErosion::ApplyThermalWeathering()
{
	float currentIndex;

	vector<HeightData> heightMapCopy;

	heightMapCopy.resize(mWaterErosionParameters.terrainSize * mWaterErosionParameters.terrainSize);

	for (int z = 0; z < mWaterErosionParameters.terrainSize; z++)
	{
		for (int x = 0; x < mWaterErosionParameters.terrainSize; x++)
		{
			float currentHeight = ReadHeightMapInput(x, z);
			heightMapCopy[(z * mWaterErosionParameters.terrainSize) + x].x = x;
			heightMapCopy[(z * mWaterErosionParameters.terrainSize) + x].y = currentHeight;
			heightMapCopy[(z * mWaterErosionParameters.terrainSize) + x].z = z;
		}
	}

	for (int z = 0; z < mWaterErosionParameters.terrainSize; z++)
	{
		for (int x = 0; x < mWaterErosionParameters.terrainSize; x++)
		{
			currentIndex = (z * mWaterErosionParameters.terrainSize) + x;

			float currentHeight = ReadHeightMapInput(x, z);

			double heightDiffTopLeft = 0;
			double heightDiffTop = 0;
			double heightDiffTopRight = 0;
			double heightDiffRight = 0;
			double heightDiffBottomLeft = 0;
			double heightDiffBottom = 0;
			double heightDiffBottomRight = 0;
			double heightDiffLeft = 0;

			if (z > 0)
			{
				if (x > 0)
				{
					heightDiffTopLeft = currentHeight - heightMapCopy[currentIndex - mWaterErosionParameters.terrainSize - 1].y;
				}

				heightDiffTop = currentHeight - heightMapCopy[currentIndex - mWaterErosionParameters.terrainSize].y;

				if (x < mWaterErosionParameters.terrainSize - 1)
				{
					heightDiffTopRight = currentHeight - heightMapCopy[currentIndex - mWaterErosionParameters.terrainSize + 1].y;
				}
			}

			if (x < mWaterErosionParameters.terrainSize - 1)
			{
				heightDiffRight = currentHeight - heightMapCopy[currentIndex + 1].y;
			}

			if (z < mWaterErosionParameters.terrainSize - 1)
			{
				if (x > 0)
				{
					heightDiffBottomLeft = currentHeight - heightMapCopy[currentIndex + mWaterErosionParameters.terrainSize - 1].y;
				}

				heightDiffBottom = currentHeight - heightMapCopy[currentIndex + mWaterErosionParameters.terrainSize].y;

				if (x < mWaterErosionParameters.terrainSize - 1)
				{
					heightDiffBottomRight = currentHeight - heightMapCopy[currentIndex + mWaterErosionParameters.terrainSize + 1].y;
				}
			}

			if (x > 0)
			{
				heightDiffLeft = currentHeight - heightMapCopy[currentIndex - 1].y;
			}

			double maxHeightDiff = 0;
			maxHeightDiff = max(maxHeightDiff, heightDiffTopLeft);
			maxHeightDiff = max(maxHeightDiff, heightDiffTop);
			maxHeightDiff = max(maxHeightDiff, heightDiffTopRight);
			maxHeightDiff = max(maxHeightDiff, heightDiffRight);
			maxHeightDiff = max(maxHeightDiff, heightDiffBottomLeft);
			maxHeightDiff = max(maxHeightDiff, heightDiffBottom);
			maxHeightDiff = max(maxHeightDiff, heightDiffBottomRight);
			maxHeightDiff = max(maxHeightDiff, heightDiffLeft);

			if (maxHeightDiff < mWaterErosionParameters.talusAngle)
			{
				continue;
			}

			double amountToTransport = maxHeightDiff / 2 * mWaterErosionParameters.deltaT * mWaterErosionParameters.thermalPowerMultiplier;

			double totalTransportableAmount = 0;

			if (heightDiffTopLeft >= mWaterErosionParameters.talusAngle)
				totalTransportableAmount += heightDiffTopLeft;

			if (heightDiffTop >= mWaterErosionParameters.talusAngle)
				totalTransportableAmount += heightDiffTop;

			if (heightDiffTopRight >= mWaterErosionParameters.talusAngle)
				totalTransportableAmount += heightDiffTopRight;

			if (heightDiffRight >= mWaterErosionParameters.talusAngle)
				totalTransportableAmount += heightDiffRight;

			if (heightDiffBottomLeft >= mWaterErosionParameters.talusAngle)
				totalTransportableAmount += heightDiffBottomLeft;

			if (heightDiffBottom >= mWaterErosionParameters.talusAngle)
				totalTransportableAmount += heightDiffBottom;

			if (heightDiffBottomRight >= mWaterErosionParameters.talusAngle)
				totalTransportableAmount += heightDiffBottomRight;

			if (heightDiffLeft >= mWaterErosionParameters.talusAngle)
				totalTransportableAmount += heightDiffLeft;

			mHeightMapInput[currentIndex].y -= amountToTransport;

			if (z > 0)
			{
				if (x > 0)
				{
					if (heightDiffTopLeft >= mWaterErosionParameters.talusAngle)
						mHeightMapInput[currentIndex - mWaterErosionParameters.terrainSize - 1].y += amountToTransport * heightDiffTopLeft / totalTransportableAmount;
				}

				if (heightDiffTop >= mWaterErosionParameters.talusAngle)
					mHeightMapInput[currentIndex - mWaterErosionParameters.terrainSize].y += amountToTransport * heightDiffTop / totalTransportableAmount;

				if (x < mWaterErosionParameters.terrainSize - 1)
				{
					if (heightDiffTopRight >= mWaterErosionParameters.talusAngle)
						mHeightMapInput[currentIndex - mWaterErosionParameters.terrainSize + 1].y += amountToTransport * heightDiffTopRight / totalTransportableAmount;
				}
			}

			if (x < mWaterErosionParameters.terrainSize - 1)
			{
				if (heightDiffRight >= mWaterErosionParameters.talusAngle)
					mHeightMapInput[currentIndex + 1].y += amountToTransport * heightDiffRight / totalTransportableAmount;
			}

			if (z < mWaterErosionParameters.terrainSize - 1)
			{
				if (x > 0)
				{
					if (heightDiffBottomLeft >= mWaterErosionParameters.talusAngle)
						mHeightMapInput[currentIndex + mWaterErosionParameters.terrainSize - 1].y += amountToTransport * heightDiffBottomLeft / totalTransportableAmount;
				}

				if (heightDiffBottom >= mWaterErosionParameters.talusAngle)
					mHeightMapInput[currentIndex + mWaterErosionParameters.terrainSize].y += amountToTransport * heightDiffBottom / totalTransportableAmount;

				if (x < mWaterErosionParameters.terrainSize - 1)
				{
					if (heightDiffBottomRight >= mWaterErosionParameters.talusAngle)
						mHeightMapInput[currentIndex + mWaterErosionParameters.terrainSize + 1].y += amountToTransport * heightDiffBottomRight / totalTransportableAmount;
				}
			}

			if (x > 0)
			{
				if (heightDiffLeft >= mWaterErosionParameters.talusAngle)
					mHeightMapInput[currentIndex - 1].y += amountToTransport * heightDiffLeft / totalTransportableAmount;
			}
		}
	}

	////
	//// Add Thermal map to height map
	////

	//for (int z = 0; z < mWaterErosionParameters.terrainSize; z++)
	//{
	//	for (int x = 0; x < mWaterErosionParameters.terrainSize; x++)
	//	{
	//		UpdateHeightMapInput(x, z, thermalMap[(z * mWaterErosionParameters.terrainSize) + x].y);
	//	}
	//}

	if (heightMapCopy.size() > 0)
		heightMapCopy.clear();
}
//===============================================================================================================================
float WaterErosion::FlowValue(float direction, float heightDifference)
{
	return direction + mWaterErosionParameters.deltaT * mWaterErosionParameters.pipeCrossectionArea * (mWaterErosionParameters.graviationalAcceleration * heightDifference) / mWaterErosionParameters.pipeLength;
}
//===============================================================================================================================
float WaterErosion::ReadWaterHeight(int x, int z)
{
	if (x < mWaterErosionParameters.terrainSize && z < mWaterErosionParameters.terrainSize && x >= 0 && z >= 0)
	{
		return mWaterMap[(z * mWaterErosionParameters.terrainSize) + x].y;
	}
	
	return 0;
}
//===============================================================================================================================
float WaterErosion::ReadSedimentationHeight(int x, int z)
{
	if (x < mWaterErosionParameters.terrainSize && z < mWaterErosionParameters.terrainSize && x >= 0 && z >= 0)
	{
		return mSedimentationMap[(z * mWaterErosionParameters.terrainSize) + x].y;
	}
	
	return 0;
}
//===============================================================================================================================
float WaterErosion::ReadErosionHeight(int x, int z)
{
	if (x < mWaterErosionParameters.terrainSize && z < mWaterErosionParameters.terrainSize && x >= 0 && z >= 0)
	{
		return mErosionMap[(z * mWaterErosionParameters.terrainSize) + x].y;
	}
	
	return 0;
}
//===============================================================================================================================
float WaterErosion::ReadHeightMapInput(int x, int z)
{
	if (x < mWaterErosionParameters.terrainSize && z < mWaterErosionParameters.terrainSize && x >= 0 && z >= 0)
	{
		return mHeightMapInput[(z * mWaterErosionParameters.terrainSize) + x].y;
	}

	return 0;
}
//===============================================================================================================================
ZShadeSandboxTerrain::OutflowValues WaterErosion::ReadOutflowFluxMap(int x, int z)
{
	if (x < mWaterErosionParameters.terrainSize && z < mWaterErosionParameters.terrainSize && x >= 0 && z >= 0)
	{
		return mOutflowFluxMap[(z * mWaterErosionParameters.terrainSize) + x];
	}
	
	OutflowValues ov;
	ov.left = ov.right = ov.top = ov.bottom = 0;
	return ov;
}
//===============================================================================================================================
ZShadeSandboxTerrain::VelocityVector WaterErosion::ReadVelocityVectorMap(int x, int z)
{
	if (x < mWaterErosionParameters.terrainSize && z < mWaterErosionParameters.terrainSize && x >= 0 && z >= 0)
	{
		return mVelocityVectorMap[(z * mWaterErosionParameters.terrainSize) + x];
	}
	
	VelocityVector vv;
	vv.x = vv.y = 0;
	return vv;
}
//===============================================================================================================================
void WaterErosion::UpdateWaterMap(int x, int z, float value)
{
	mWaterMap[(z * mWaterErosionParameters.terrainSize) + x].y = value;
}
//===============================================================================================================================
void WaterErosion::UpdateSedimentationMap(int x, int z, float value)
{
	mSedimentationMap[(z * mWaterErosionParameters.terrainSize) + x].y = value;
}
//===============================================================================================================================
void WaterErosion::UpdateErosionMap(int x, int z, float value)
{
	mErosionMap[(z * mWaterErosionParameters.terrainSize) + x].y = value;
}
//===============================================================================================================================
void WaterErosion::UpdateHeightMapInput(int x, int z, float value)
{
	mHeightMapInput[(z * mWaterErosionParameters.terrainSize) + x].y = value;
}
//===============================================================================================================================
void WaterErosion::UpdateOutflowFluxMap(int x, int z, ZShadeSandboxTerrain::OutflowValues value)
{
	mOutflowFluxMap[(z * mWaterErosionParameters.terrainSize) + x] = value;
}
//===============================================================================================================================
void WaterErosion::UpdateVelocityVectorMap(int x, int z, ZShadeSandboxTerrain::VelocityVector value)
{
	mVelocityVectorMap[(z * mWaterErosionParameters.terrainSize) + x] = value;
}
//===============================================================================================================================