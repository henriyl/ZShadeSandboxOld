//===============================================================================================================================
// SectorBuilder
//
//===============================================================================================================================
// History
//
// -Created on 12/27/2014 by Dustin Watson
//===============================================================================================================================
#ifndef __SECTORBUILDER_H
#define __SECTORBUILDER_H
//===============================================================================================================================
//===============================================================================================================================
// Includes
#include <vector>
#include "SectionAdjacency.h"
//===============================================================================================================================
class SectorBuilder
{
	std::vector<int> BuildSpecialSector(int map_size, SectorType type, int begin);
	std::vector<int> BuildSector(int map_size, int begin);

public:

	static std::vector<int> arrNW6x6;
	static std::vector<int> arrNE6x6;
	static std::vector<int> arrSW6x6;
	static std::vector<int> arrSE6x6;

	static std::vector<int> arrNW8x8;
	static std::vector<int> arrNE8x8;
	static std::vector<int> arrSW8x8;
	static std::vector<int> arrSE8x8;

	static std::vector<int> arrNW10x10;
	static std::vector<int> arrNE10x10;
	static std::vector<int> arrSW10x10;
	static std::vector<int> arrSE10x10;

	static std::vector<int> arrNW20x20;
	static std::vector<int> arrNE20x20;
	static std::vector<int> arrSW20x20;
	static std::vector<int> arrSE20x20;

	static std::vector<int> arrNW30x30;
	static std::vector<int> arrNE30x30;
	static std::vector<int> arrSW30x30;
	static std::vector<int> arrSE30x30;

	static std::vector<int> arrNWSpecialCase6x6;
	static std::vector<int> arrNESpecialCase6x6;
	static std::vector<int> arrSWSpecialCase6x6;

	static std::vector<int> arrNWSpecialCase8x8;
	static std::vector<int> arrNESpecialCase8x8;
	static std::vector<int> arrSWSpecialCase8x8;

	static std::vector<int> arrNWSpecialCase10x10;
	static std::vector<int> arrNESpecialCase10x10;
	static std::vector<int> arrSWSpecialCase10x10;

	static std::vector<int> arrNWSpecialCase20x20;
	static std::vector<int> arrNESpecialCase20x20;
	static std::vector<int> arrSWSpecialCase20x20;

	static std::vector<int> arrNWSpecialCase30x30;
	static std::vector<int> arrNESpecialCase30x30;
	static std::vector<int> arrSWSpecialCase30x30;

	SectorBuilder();
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__SECTORBUILDER_H