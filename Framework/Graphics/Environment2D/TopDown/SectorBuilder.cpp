#include "SectorBuilder.h"
#include "InsersionSort.h"
//===============================================================================================================================
//===============================================================================================================================
std::vector<int> SectorBuilder::arrNW6x6;
std::vector<int> SectorBuilder::arrNE6x6;
std::vector<int> SectorBuilder::arrSW6x6;
std::vector<int> SectorBuilder::arrSE6x6;
std::vector<int> SectorBuilder::arrNW8x8;
std::vector<int> SectorBuilder::arrNE8x8;
std::vector<int> SectorBuilder::arrSW8x8;
std::vector<int> SectorBuilder::arrSE8x8;
std::vector<int> SectorBuilder::arrNW10x10;
std::vector<int> SectorBuilder::arrNE10x10;
std::vector<int> SectorBuilder::arrSW10x10;
std::vector<int> SectorBuilder::arrSE10x10;
std::vector<int> SectorBuilder::arrNW20x20;
std::vector<int> SectorBuilder::arrNE20x20;
std::vector<int> SectorBuilder::arrSW20x20;
std::vector<int> SectorBuilder::arrSE20x20;
std::vector<int> SectorBuilder::arrNW30x30;
std::vector<int> SectorBuilder::arrNE30x30;
std::vector<int> SectorBuilder::arrSW30x30;
std::vector<int> SectorBuilder::arrSE30x30;
std::vector<int> SectorBuilder::arrNWSpecialCase6x6;
std::vector<int> SectorBuilder::arrNESpecialCase6x6;
std::vector<int> SectorBuilder::arrSWSpecialCase6x6;
std::vector<int> SectorBuilder::arrNWSpecialCase8x8;
std::vector<int> SectorBuilder::arrNESpecialCase8x8;
std::vector<int> SectorBuilder::arrSWSpecialCase8x8;
std::vector<int> SectorBuilder::arrNWSpecialCase10x10;
std::vector<int> SectorBuilder::arrNESpecialCase10x10;
std::vector<int> SectorBuilder::arrSWSpecialCase10x10;
std::vector<int> SectorBuilder::arrNWSpecialCase20x20;
std::vector<int> SectorBuilder::arrNESpecialCase20x20;
std::vector<int> SectorBuilder::arrSWSpecialCase20x20;
std::vector<int> SectorBuilder::arrNWSpecialCase30x30;
std::vector<int> SectorBuilder::arrNESpecialCase30x30;
std::vector<int> SectorBuilder::arrSWSpecialCase30x30;
//===============================================================================================================================
SectorBuilder::SectorBuilder()
{
	// Build the sectors

	arrNW6x6 = BuildSector(6, 7);
	arrNE6x6 = BuildSector(6, 19);
	arrSW6x6 = BuildSector(6, 9);
	arrSE6x6 = BuildSector(6, 21);
	
	arrNW8x8 = BuildSector(8, 9);
	arrNE8x8 = BuildSector(8, 33);
	arrSW8x8 = BuildSector(8, 12);
	arrSE8x8 = BuildSector(8, 36);

	arrNW10x10 = BuildSector(10, 11);
	arrNE10x10 = BuildSector(10, 51);
	arrSW10x10 = BuildSector(10, 15);
	arrSE10x10 = BuildSector(10, 55);
	
	arrNW20x20 = BuildSector(20, 21);
	arrNE20x20 = BuildSector(20, 201);
	arrSW20x20 = BuildSector(20, 30);
	arrSE20x20 = BuildSector(20, 210);
	
	arrNW30x30 = BuildSector(30, 31);
	arrNE30x30 = BuildSector(30, 451);
	arrSW30x30 = BuildSector(30, 45);
	arrSE30x30 = BuildSector(30, 465);

	arrNWSpecialCase6x6 = BuildSpecialSector(6, ST_NORTH_WEST, 7);
	arrNESpecialCase6x6 = BuildSpecialSector(6, ST_NORTH_EAST, 19);
	arrSWSpecialCase6x6 = BuildSpecialSector(6, ST_SOUTH_WEST, 9);

	arrNWSpecialCase8x8 = BuildSpecialSector(8, ST_NORTH_WEST, 9);
	arrNESpecialCase8x8 = BuildSpecialSector(8, ST_NORTH_EAST, 33);
	arrSWSpecialCase8x8 = BuildSpecialSector(8, ST_SOUTH_WEST, 12);

	arrNWSpecialCase10x10 = BuildSpecialSector(10, ST_NORTH_WEST, 11);
	arrNESpecialCase10x10 = BuildSpecialSector(10, ST_NORTH_EAST, 51);
	arrSWSpecialCase10x10 = BuildSpecialSector(10, ST_SOUTH_WEST, 15);

	arrNWSpecialCase20x20 = BuildSpecialSector(20, ST_NORTH_WEST, 21);
	arrNESpecialCase20x20 = BuildSpecialSector(20, ST_NORTH_EAST, 201);
	arrSWSpecialCase20x20 = BuildSpecialSector(20, ST_SOUTH_WEST, 30);

	arrNWSpecialCase30x30 = BuildSpecialSector(30, ST_NORTH_WEST, 31);
	arrNESpecialCase30x30 = BuildSpecialSector(30, ST_NORTH_EAST, 451);
	arrSWSpecialCase30x30 = BuildSpecialSector(30, ST_SOUTH_WEST, 45);

	// Sort all the sectors

	InsersionSort<int>::Sort(arrNW6x6);
	InsersionSort<int>::Sort(arrNE6x6);
	InsersionSort<int>::Sort(arrSW6x6);
	InsersionSort<int>::Sort(arrSE6x6);

	InsersionSort<int>::Sort(arrNW8x8);
	InsersionSort<int>::Sort(arrNE8x8);
	InsersionSort<int>::Sort(arrSW8x8);
	InsersionSort<int>::Sort(arrSE8x8);

	InsersionSort<int>::Sort(arrNW10x10);
	InsersionSort<int>::Sort(arrNE10x10);
	InsersionSort<int>::Sort(arrSW10x10);
	InsersionSort<int>::Sort(arrSE10x10);

	InsersionSort<int>::Sort(arrNW20x20);
	InsersionSort<int>::Sort(arrNE20x20);
	InsersionSort<int>::Sort(arrSW20x20);
	InsersionSort<int>::Sort(arrSE20x20);

	InsersionSort<int>::Sort(arrNW30x30);
	InsersionSort<int>::Sort(arrNE30x30);
	InsersionSort<int>::Sort(arrSW30x30);
	InsersionSort<int>::Sort(arrSE30x30);

	InsersionSort<int>::Sort(arrNWSpecialCase6x6);
	InsersionSort<int>::Sort(arrNESpecialCase6x6);
	InsersionSort<int>::Sort(arrSWSpecialCase6x6);

	InsersionSort<int>::Sort(arrNWSpecialCase8x8);
	InsersionSort<int>::Sort(arrNESpecialCase8x8);
	InsersionSort<int>::Sort(arrSWSpecialCase8x8);

	InsersionSort<int>::Sort(arrNWSpecialCase10x10);
	InsersionSort<int>::Sort(arrNESpecialCase10x10);
	InsersionSort<int>::Sort(arrSWSpecialCase10x10);

	InsersionSort<int>::Sort(arrNWSpecialCase20x20);
	InsersionSort<int>::Sort(arrNESpecialCase20x20);
	InsersionSort<int>::Sort(arrSWSpecialCase20x20);

	InsersionSort<int>::Sort(arrNWSpecialCase30x30);
	InsersionSort<int>::Sort(arrNESpecialCase30x30);
	InsersionSort<int>::Sort(arrSWSpecialCase30x30);
}
//===============================================================================================================================
std::vector<int> SectorBuilder::BuildSpecialSector(int map_size, SectorType type, int begin)
{
	std::vector<int> vals;

	int perSector = 0;

	if (map_size == 6)
	{
		perSector = 2;
	}

	if (map_size == 8)
	{
		perSector = 3;
	}

	if (map_size == 10)
	{
		perSector = 4;
	}

	if (map_size == 20)
	{
		perSector = 9;
	}

	if (map_size == 30)
	{
		perSector = 14;
	}

	if (type == ST_NORTH_WEST)
	{
		int t = begin;
		int i;
		int stop = 0;

		i = 0;
		stop = t;

		while (i < perSector)
		{
			vals.push_back(stop);

			stop += map_size;

			i++;
		}

		for (i = 0, t = begin + 1; i < perSector - 1; i++, t++)
		{
			vals.push_back(t);
		}
	}

	if (type == ST_NORTH_EAST)
	{
		int t = begin;
		int i;
		int stop = 0;

		i = 0;
		stop = t;

		while (i < perSector)
		{
			vals.push_back(stop);

			stop += map_size;

			i++;
		}
	}

	if (type == ST_SOUTH_WEST)
	{
		int t;
		int i;
		
		for (i = 0, t = begin; i < perSector; i++, t++)
		{
			vals.push_back(t);
		}
	}

	return vals;
}
//===============================================================================================================================
std::vector<int> SectorBuilder::BuildSector(int map_size, int begin)
{
	std::vector<int> vals;

	int perSector = 0;

	if (map_size == 6)
	{
		perSector = 2;
	}

	if (map_size == 8)
	{
		perSector = 3;
	}

	if (map_size == 10)
	{
		perSector = 4;
	}

	if (map_size == 20)
	{
		perSector = 9;
	}

	if (map_size == 30)
	{
		perSector = 14;
	}

	int t = begin;
	int i;
	int stop = 0;

	for (int j = 0; j < perSector; j++)
	{
		i = 0;
		stop = t;
		
		while (i < perSector)
		{
			vals.push_back(stop);

			stop += map_size;
			
			i++;
		}

		t++;
	}

	return vals;
}
//===============================================================================================================================