//===============================================================================================================================
// SectionAdjacency
//
//===============================================================================================================================
// History
//
// -Created on 12/21/2014 by Dustin Watson
//===============================================================================================================================
#ifndef __SECTIONADJACENCY_H
#define __SECTIONADJACENCY_H
//===============================================================================================================================
//===============================================================================================================================
// Includes
#include <vector>
#include <memory>
//===============================================================================================================================
//===============================================================================================================================
enum NeighborPos
{
	NORTH, // Up
	SOUTH, // Down
	EAST,  // ->
	WEST,  // <-
	NE,
	NW,
	SE,
	SW
};
//===============================================================================================================================
enum SAType
{
	NW_CORNER,
	NE_CORNER,
	SW_CORNER,
	SE_CORNER,
	NORTH_EDGE,
	SOUTH_EDGE,
	WEST_EDGE,
	EAST_EDGE,
	CENTER
};
//===============================================================================================================================
enum SectorType
{
	ST_NORTH_WEST,
	ST_SOUTH_WEST,
	ST_NORTH_EAST,
	ST_SOUTH_EAST
};
//===============================================================================================================================
struct AdjacentSection
{
	int ID;
	NeighborPos neighborPos;
};
//===============================================================================================================================
struct SectionAdjacency
{
	// The adjacent numbers to this tile
	std::vector< AdjacentSection > adjSections;
	
	// The current ID of the tile
	int ID;

	// Is this a corner, edge or center tile
	SAType type;

	SectionAdjacency() {}
	void Add(int id, int adjID, SAType t, NeighborPos adjPos)
	{
		ID = id;
		type = t;

		AdjacentSection at;

		at.ID = adjID;
		at.neighborPos = adjPos;

		adjSections.push_back(at);
	}

	SectionAdjacency* Clone()
	{
		std::unique_ptr<SectionAdjacency> sa;
		sa.reset(new SectionAdjacency());

		for (int i = 0; i < adjSections.size(); i++)
		{
			sa->Add(ID, adjSections[i].ID, type, adjSections[i].neighborPos);
		}

		return sa.get();
	}
};
//===============================================================================================================================
static std::unique_ptr<SectionAdjacency> FindCorner(int id, int inc, int dir)
{
	std::unique_ptr<SectionAdjacency> ta;
	ta.reset(new SectionAdjacency());

	if (dir == 0)
	{
		int eastID = id + inc;
		int southID = id + 1;
		int seID = eastID + 1;

		ta->Add(id, eastID, NW_CORNER, EAST);
		ta->Add(id, southID, NW_CORNER, SOUTH);
		ta->Add(id, seID, NW_CORNER, SE);
	}

	if (dir == 1)
	{
		int northID = id - 1;
		int neID = id + (inc - 1);
		int eastID = neID + 1;

		ta->Add(id, northID, SW_CORNER, NORTH);
		ta->Add(id, neID, SW_CORNER, NE);
		ta->Add(id, eastID, SW_CORNER, EAST);
	}

	if (dir == 2)
	{
		int westID = id - inc;
		int swID = westID + 1;
		int southID = id + 1;

		ta->Add(id, westID, NE_CORNER, WEST);
		ta->Add(id, swID, NE_CORNER, SW);
		ta->Add(id, southID, NE_CORNER, SOUTH);
	}

	if (dir == 3)
	{
		int westID = id - inc;
		int nwID = westID - 1;
		int northID = id - 1;

		ta->Add(id, westID, SE_CORNER, WEST);
		ta->Add(id, nwID, SE_CORNER, NW);
		ta->Add(id, northID, SE_CORNER, NORTH);
	}

	return ta;
}
//===============================================================================================================================
static std::unique_ptr<SectionAdjacency> FindEdgeWest(int id, int inc)
{
	std::unique_ptr<SectionAdjacency> ta;
	ta.reset(new SectionAdjacency());

	int northID = id - 1;
	int neID = id + (inc - 1);
	int eastID = neID + 1;
	int seID = eastID + 1;
	int southID = id + 1;

	ta->Add(id, northID, WEST_EDGE, NORTH);
	ta->Add(id, neID, WEST_EDGE, NE);
	ta->Add(id, eastID, WEST_EDGE, EAST);
	ta->Add(id, seID, WEST_EDGE, SE);
	ta->Add(id, southID, WEST_EDGE, SOUTH);

	return ta;
}
//===============================================================================================================================
static std::unique_ptr<SectionAdjacency> FindEdgeNorth(int id, int inc)
{
	std::unique_ptr<SectionAdjacency> ta;
	ta.reset(new SectionAdjacency());

	int westID = id - inc;
	int swID = westID + 1;
	int southID = id + 1;
	int seID = southID + inc;
	int eastID = seID - 1;

	ta->Add(id, westID, NORTH_EDGE, WEST);
	ta->Add(id, swID, NORTH_EDGE, SW);
	ta->Add(id, southID, NORTH_EDGE, SOUTH);
	ta->Add(id, seID, NORTH_EDGE, SE);
	ta->Add(id, eastID, NORTH_EDGE, EAST);

	return ta;
}
//===============================================================================================================================
static std::unique_ptr<SectionAdjacency> FindEdgeEast(int id, int inc)
{
	std::unique_ptr<SectionAdjacency> ta;
	ta.reset(new SectionAdjacency());

	int northID = id - 1;
	int nwID = northID - inc;
	int westID = nwID + 1;
	int swID = westID + 1;
	int southID = id + 1;

	ta->Add(id, northID, EAST_EDGE, NORTH);
	ta->Add(id, nwID, EAST_EDGE, NW);
	ta->Add(id, westID, EAST_EDGE, WEST);
	ta->Add(id, swID, EAST_EDGE, SW);
	ta->Add(id, southID, EAST_EDGE, SOUTH);

	return ta;
}
//===============================================================================================================================
static std::unique_ptr<SectionAdjacency> FindEdgeSouth(int id, int inc)
{
	std::unique_ptr<SectionAdjacency> ta;
	ta.reset(new SectionAdjacency());

	int westID = id - inc;
	int nwID = westID - 1;
	int northID = id - 1;
	int neID = id + (inc - 1);
	int eastID = neID + 1;

	ta->Add(id, westID, SOUTH_EDGE, WEST);
	ta->Add(id, nwID, SOUTH_EDGE, NW);
	ta->Add(id, northID, SOUTH_EDGE, NORTH);
	ta->Add(id, neID, SOUTH_EDGE, NE);
	ta->Add(id, eastID, SOUTH_EDGE, EAST);

	return ta;
}
//===============================================================================================================================
static std::unique_ptr<SectionAdjacency> FindCenter(int id, int inc)
{
	std::unique_ptr<SectionAdjacency> ta;
	ta.reset(new SectionAdjacency());

	int northID = id - 1;
	int nwID = northID - inc;
	int westID = nwID + 1;
	int swID = westID + 1;
	int southID = id + 1;
	int neID = id + (inc - 1);
	int eastID = neID + 1;
	int seID = eastID + 1;

	ta->Add(id, northID, CENTER, NORTH);
	ta->Add(id, nwID, CENTER, NW);
	ta->Add(id, westID, CENTER, WEST);
	ta->Add(id, swID, CENTER, SW);
	ta->Add(id, southID, CENTER, SOUTH);
	ta->Add(id, neID, CENTER, NE);
	ta->Add(id, eastID, CENTER, EAST);
	ta->Add(id, seID, CENTER, SE);

	return ta;
}
//===============================================================================================================================
/*

3x3

0 3 6
1 4 7
2 5 8

0 -> 1, 4, 3
1 -> 0, 3, 4, 5, 2
2 -> 1, 4, 5
3 -> 0, 1, 4, 7, 6
4 -> 0, 1, 2, 3, 5, 6, 7, 8
5 -> 2, 1, 4, 7, 8
6 -> 3, 4, 7
7 -> 6, 3, 4, 5, 8
8 -> 5, 4, 7
*/
struct TileAdjInfo3x3
{
	static std::unique_ptr<SectionAdjacency> tileAdjInfo[9];
	
	TileAdjInfo3x3()
	{
		/*
		tileAdjInfo[0].Add(0, 1, SOUTH); tileAdjInfo[0].Add(0, 4, SE); tileAdjInfo[0].Add(0, 3, EAST);
		tileAdjInfo[1].Add(1, 0, NORTH); tileAdjInfo[1].Add(1, 3, NE); tileAdjInfo[1].Add(1, 4, EAST); tileAdjInfo[1].Add(1, 5, SE); tileAdjInfo[1].Add(1, 2, SOUTH);
		tileAdjInfo[2].Add(2, 1, NORTH); tileAdjInfo[2].Add(2, 4, NE); tileAdjInfo[2].Add(2, 5, EAST);
		tileAdjInfo[3].Add(3, 0, WEST); tileAdjInfo[3].Add(3, 1, SW); tileAdjInfo[3].Add(3, 4, SOUTH); tileAdjInfo[3].Add(3, 7, SE); tileAdjInfo[3].Add(3, 6, EAST);
		tileAdjInfo[4].Add(4, 0, NW); tileAdjInfo[4].Add(4, 1, WEST); tileAdjInfo[4].Add(4, 2, SW); tileAdjInfo[4].Add(4, 3, NORTH); tileAdjInfo[4].Add(4, 5, SOUTH); tileAdjInfo[4].Add(4, 6, NE); tileAdjInfo[4].Add(4, 7, EAST); tileAdjInfo[4].Add(4, 8, SE);
		tileAdjInfo[5].Add(5, 2, WEST); tileAdjInfo[5].Add(5, 1, NW); tileAdjInfo[5].Add(5, 4, NORTH); tileAdjInfo[5].Add(5, 7, NE); tileAdjInfo[5].Add(5, 8, EAST);
		tileAdjInfo[6].Add(6, 3, WEST); tileAdjInfo[6].Add(6, 4, SW); tileAdjInfo[6].Add(6, 7, SOUTH);
		tileAdjInfo[7].Add(7, 6, NORTH); tileAdjInfo[7].Add(7, 3, NW); tileAdjInfo[7].Add(7, 4, WEST); tileAdjInfo[7].Add(7, 5, SW); tileAdjInfo[7].Add(7, 8, SOUTH);
		tileAdjInfo[8].Add(8, 5, WEST); tileAdjInfo[8].Add(8, 4, NW); tileAdjInfo[8].Add(8, 7, NORTH);
		*/

		tileAdjInfo[0] = FindCorner(0, 3, 0);
		tileAdjInfo[2] = FindCorner(2, 3, 1);
		tileAdjInfo[6] = FindCorner(6, 3, 2);
		tileAdjInfo[8] = FindCorner(8, 3, 3);

		tileAdjInfo[1] = FindEdgeWest(1, 3);
		tileAdjInfo[3] = FindEdgeNorth(3, 3);
		tileAdjInfo[7] = FindEdgeEast(7, 3);
		tileAdjInfo[5] = FindEdgeSouth(5, 3);

		tileAdjInfo[4] = FindCenter(4, 3);
	}
};
//===============================================================================================================================
/*
6x6

00 06 12 18 24 30
01 07 13 19 25 31
02 08 14 20 26 32
03 09 15 21 27 33
04 10 16 22 28 34
05 11 17 23 29 35
*/
struct TileAdjInfo6x6
{
	static std::unique_ptr<SectionAdjacency> tileAdjInfo[36];

	TileAdjInfo6x6()
	{
		tileAdjInfo[ 0] = FindCorner( 0, 6, 0);
		tileAdjInfo[ 5] = FindCorner( 5, 6, 1);
		tileAdjInfo[30] = FindCorner(30, 6, 2);
		tileAdjInfo[35] = FindCorner(35, 6, 3);

		tileAdjInfo[ 1] = FindEdgeWest(1, 6);
		tileAdjInfo[ 2] = FindEdgeWest(2, 6);
		tileAdjInfo[ 3] = FindEdgeWest(3, 6);
		tileAdjInfo[ 4] = FindEdgeWest(4, 6);

		tileAdjInfo[ 6] = FindEdgeNorth(6, 6);
		tileAdjInfo[12] = FindEdgeNorth(12, 6);
		tileAdjInfo[18] = FindEdgeNorth(18, 6);
		tileAdjInfo[24] = FindEdgeNorth(24, 6);

		tileAdjInfo[31] = FindEdgeEast(31, 6);
		tileAdjInfo[32] = FindEdgeEast(32, 6);
		tileAdjInfo[33] = FindEdgeEast(33, 6);
		tileAdjInfo[34] = FindEdgeEast(34, 6);

		tileAdjInfo[11] = FindEdgeSouth(11, 6);
		tileAdjInfo[17] = FindEdgeSouth(17, 6);
		tileAdjInfo[23] = FindEdgeSouth(23, 6);
		tileAdjInfo[29] = FindEdgeSouth(29, 6);

		tileAdjInfo[ 7] = FindCenter(7, 6);
		tileAdjInfo[ 8] = FindCenter(8, 6);
		tileAdjInfo[ 9] = FindCenter(9, 6);
		tileAdjInfo[10] = FindCenter(10, 6);
		tileAdjInfo[13] = FindCenter(13, 6);
		tileAdjInfo[14] = FindCenter(14, 6);
		tileAdjInfo[15] = FindCenter(15, 6);
		tileAdjInfo[16] = FindCenter(16, 6);
		tileAdjInfo[19] = FindCenter(19, 6);
		tileAdjInfo[20] = FindCenter(20, 6);
		tileAdjInfo[21] = FindCenter(21, 6);
		tileAdjInfo[22] = FindCenter(22, 6);
		tileAdjInfo[25] = FindCenter(25, 6);
		tileAdjInfo[26] = FindCenter(26, 6);
		tileAdjInfo[27] = FindCenter(27, 6);
		tileAdjInfo[28] = FindCenter(28, 6);
	}
};
//===============================================================================================================================
/*
8x8

00 08 16 24 32 40 48 56
01 09 17 25 33 41 49 57
02 10 18 26 34 42 50 58
03 11 19 27 35 43 51 59
04 12 20 28 36 44 52 60
05 13 21 29 37 45 53 61
06 14 22 30 38 46 54 62
07 15 23 31 39 47 55 63
*/
struct TileAdjInfo8x8
{
	static std::unique_ptr<SectionAdjacency> tileAdjInfo[64];

	TileAdjInfo8x8()
	{
		tileAdjInfo[0] = FindCorner(0, 8, 0);
		tileAdjInfo[7] = FindCorner(7, 8, 1);
		tileAdjInfo[56] = FindCorner(56, 8, 2);
		tileAdjInfo[63] = FindCorner(63, 8, 3);

		tileAdjInfo[1] = FindEdgeWest(1, 8);
		tileAdjInfo[2] = FindEdgeWest(2, 8);
		tileAdjInfo[3] = FindEdgeWest(3, 8);
		tileAdjInfo[4] = FindEdgeWest(4, 8);
		tileAdjInfo[5] = FindEdgeWest(5, 8);
		tileAdjInfo[6] = FindEdgeWest(6, 8);
		
		tileAdjInfo[ 8] = FindEdgeNorth(8, 8);
		tileAdjInfo[16] = FindEdgeNorth(16, 8);
		tileAdjInfo[24] = FindEdgeNorth(24, 8);
		tileAdjInfo[32] = FindEdgeNorth(32, 8);
		tileAdjInfo[40] = FindEdgeNorth(40, 8);
		tileAdjInfo[48] = FindEdgeNorth(48, 8);

		tileAdjInfo[57] = FindEdgeEast(57, 8);
		tileAdjInfo[58] = FindEdgeEast(58, 8);
		tileAdjInfo[59] = FindEdgeEast(59, 8);
		tileAdjInfo[60] = FindEdgeEast(60, 8);
		tileAdjInfo[61] = FindEdgeEast(61, 8);
		tileAdjInfo[62] = FindEdgeEast(62, 8);

		tileAdjInfo[15] = FindEdgeSouth(15, 8);
		tileAdjInfo[23] = FindEdgeSouth(23, 8);
		tileAdjInfo[31] = FindEdgeSouth(31, 8);
		tileAdjInfo[39] = FindEdgeSouth(39, 8);
		tileAdjInfo[47] = FindEdgeSouth(47, 8);
		tileAdjInfo[55] = FindEdgeSouth(55, 8);

		tileAdjInfo[9] = FindCenter(9, 8);
		tileAdjInfo[10] = FindCenter(10, 8);
		tileAdjInfo[11] = FindCenter(11, 8);
		tileAdjInfo[12] = FindCenter(12, 8);
		tileAdjInfo[13] = FindCenter(13, 8);
		tileAdjInfo[14] = FindCenter(14, 8);
		tileAdjInfo[17] = FindCenter(17, 8);
		tileAdjInfo[18] = FindCenter(18, 8);
		tileAdjInfo[19] = FindCenter(19, 8);
		tileAdjInfo[20] = FindCenter(20, 8);
		tileAdjInfo[21] = FindCenter(21, 8);
		tileAdjInfo[22] = FindCenter(22, 8);
		tileAdjInfo[25] = FindCenter(25, 8);
		tileAdjInfo[26] = FindCenter(26, 8);
		tileAdjInfo[27] = FindCenter(27, 8);
		tileAdjInfo[28] = FindCenter(28, 8);
		tileAdjInfo[29] = FindCenter(29, 8);
		tileAdjInfo[30] = FindCenter(30, 8);
		tileAdjInfo[33] = FindCenter(33, 8);
		tileAdjInfo[34] = FindCenter(34, 8);
		tileAdjInfo[35] = FindCenter(35, 8);
		tileAdjInfo[36] = FindCenter(36, 8);
		tileAdjInfo[37] = FindCenter(37, 8);
		tileAdjInfo[38] = FindCenter(38, 8);
		tileAdjInfo[41] = FindCenter(41, 8);
		tileAdjInfo[42] = FindCenter(42, 8);
		tileAdjInfo[43] = FindCenter(43, 8);
		tileAdjInfo[44] = FindCenter(44, 8);
		tileAdjInfo[45] = FindCenter(45, 8);
		tileAdjInfo[46] = FindCenter(46, 8);
		tileAdjInfo[49] = FindCenter(49, 8);
		tileAdjInfo[50] = FindCenter(50, 8);
		tileAdjInfo[51] = FindCenter(51, 8);
		tileAdjInfo[52] = FindCenter(52, 8);
		tileAdjInfo[53] = FindCenter(53, 8);
		tileAdjInfo[54] = FindCenter(54, 8);
	}
};
//===============================================================================================================================
/*
10x10

00 10 20 30 40 50 60 70 80 90
01 11 21 31 41 51 61 71 81 91
02 12 22 32 42 52 62 72 82 92
03 13 23 33 43 53 63 73 83 93
04 14 24 34 44 54 64 74 84 94
05 15 25 35 45 55 65 75 85 95
06 16 26 36 46 56 66 76 86 96
07 17 27 37 47 57 67 77 87 97
08 18 28 38 48 58 68 78 88 98
09 19 29 39 49 59 69 79 89 99
*/
struct TileAdjInfo10x10
{
	static std::unique_ptr<SectionAdjacency> tileAdjInfo[100];

	TileAdjInfo10x10()
	{
		tileAdjInfo[ 0] = FindCorner(0, 10, 0);
		tileAdjInfo[ 9] = FindCorner(9, 10, 1);
		tileAdjInfo[90] = FindCorner(90, 10, 2);
		tileAdjInfo[99] = FindCorner(99, 10, 3);

		tileAdjInfo[1] = FindEdgeWest(1, 10);
		tileAdjInfo[2] = FindEdgeWest(2, 10);
		tileAdjInfo[3] = FindEdgeWest(3, 10);
		tileAdjInfo[4] = FindEdgeWest(4, 10);
		tileAdjInfo[5] = FindEdgeWest(5, 10);
		tileAdjInfo[6] = FindEdgeWest(6, 10);
		tileAdjInfo[7] = FindEdgeWest(7, 10);
		tileAdjInfo[8] = FindEdgeWest(8, 10);

		tileAdjInfo[10] = FindEdgeNorth(10, 10);
		tileAdjInfo[20] = FindEdgeNorth(20, 10);
		tileAdjInfo[30] = FindEdgeNorth(30, 10);
		tileAdjInfo[40] = FindEdgeNorth(40, 10);
		tileAdjInfo[50] = FindEdgeNorth(50, 10);
		tileAdjInfo[60] = FindEdgeNorth(60, 10);
		tileAdjInfo[70] = FindEdgeNorth(70, 10);
		tileAdjInfo[80] = FindEdgeNorth(80, 10);

		tileAdjInfo[91] = FindEdgeEast(91, 10);
		tileAdjInfo[92] = FindEdgeEast(92, 10);
		tileAdjInfo[93] = FindEdgeEast(93, 10);
		tileAdjInfo[94] = FindEdgeEast(94, 10);
		tileAdjInfo[95] = FindEdgeEast(95, 10);
		tileAdjInfo[96] = FindEdgeEast(96, 10);
		tileAdjInfo[97] = FindEdgeEast(97, 10);
		tileAdjInfo[98] = FindEdgeEast(98, 10);

		tileAdjInfo[19] = FindEdgeSouth(19, 10);
		tileAdjInfo[29] = FindEdgeSouth(29, 10);
		tileAdjInfo[39] = FindEdgeSouth(39, 10);
		tileAdjInfo[49] = FindEdgeSouth(49, 10);
		tileAdjInfo[59] = FindEdgeSouth(59, 10);
		tileAdjInfo[69] = FindEdgeSouth(69, 10);
		tileAdjInfo[79] = FindEdgeSouth(79, 10);
		tileAdjInfo[89] = FindEdgeSouth(89, 10);

		tileAdjInfo[11] = FindCenter(11, 10);
		tileAdjInfo[12] = FindCenter(12, 10);
		tileAdjInfo[13] = FindCenter(13, 10);
		tileAdjInfo[14] = FindCenter(14, 10);
		tileAdjInfo[15] = FindCenter(15, 10);
		tileAdjInfo[16] = FindCenter(16, 10);
		tileAdjInfo[17] = FindCenter(17, 10);
		tileAdjInfo[18] = FindCenter(18, 10);
		tileAdjInfo[21] = FindCenter(21, 10);
		tileAdjInfo[22] = FindCenter(22, 10);
		tileAdjInfo[23] = FindCenter(23, 10);
		tileAdjInfo[24] = FindCenter(24, 10);
		tileAdjInfo[25] = FindCenter(25, 10);
		tileAdjInfo[26] = FindCenter(26, 10);
		tileAdjInfo[27] = FindCenter(27, 10);
		tileAdjInfo[28] = FindCenter(28, 10);
		tileAdjInfo[31] = FindCenter(31, 10);
		tileAdjInfo[32] = FindCenter(32, 10);
		tileAdjInfo[33] = FindCenter(33, 10);
		tileAdjInfo[34] = FindCenter(34, 10);
		tileAdjInfo[35] = FindCenter(35, 10);
		tileAdjInfo[36] = FindCenter(36, 10);
		tileAdjInfo[37] = FindCenter(37, 10);
		tileAdjInfo[38] = FindCenter(38, 10);
		tileAdjInfo[41] = FindCenter(41, 10);
		tileAdjInfo[42] = FindCenter(42, 10);
		tileAdjInfo[43] = FindCenter(43, 10);
		tileAdjInfo[44] = FindCenter(44, 10);
		tileAdjInfo[45] = FindCenter(45, 10);
		tileAdjInfo[46] = FindCenter(46, 10);
		tileAdjInfo[47] = FindCenter(47, 10);
		tileAdjInfo[48] = FindCenter(48, 10);
		tileAdjInfo[51] = FindCenter(51, 10);
		tileAdjInfo[52] = FindCenter(52, 10);
		tileAdjInfo[53] = FindCenter(53, 10);
		tileAdjInfo[54] = FindCenter(54, 10);
		tileAdjInfo[55] = FindCenter(55, 10);
		tileAdjInfo[56] = FindCenter(56, 10);
		tileAdjInfo[57] = FindCenter(57, 10);
		tileAdjInfo[58] = FindCenter(58, 10);
		tileAdjInfo[61] = FindCenter(61, 10);
		tileAdjInfo[62] = FindCenter(62, 10);
		tileAdjInfo[63] = FindCenter(63, 10);
		tileAdjInfo[64] = FindCenter(64, 10);
		tileAdjInfo[65] = FindCenter(65, 10);
		tileAdjInfo[66] = FindCenter(66, 10);
		tileAdjInfo[67] = FindCenter(67, 10);
		tileAdjInfo[68] = FindCenter(68, 10);
		tileAdjInfo[71] = FindCenter(71, 10);
		tileAdjInfo[72] = FindCenter(72, 10);
		tileAdjInfo[73] = FindCenter(73, 10);
		tileAdjInfo[74] = FindCenter(74, 10);
		tileAdjInfo[75] = FindCenter(75, 10);
		tileAdjInfo[76] = FindCenter(76, 10);
		tileAdjInfo[77] = FindCenter(77, 10);
		tileAdjInfo[78] = FindCenter(78, 10);
		tileAdjInfo[81] = FindCenter(81, 10);
		tileAdjInfo[82] = FindCenter(82, 10);
		tileAdjInfo[83] = FindCenter(83, 10);
		tileAdjInfo[84] = FindCenter(84, 10);
		tileAdjInfo[85] = FindCenter(85, 10);
		tileAdjInfo[86] = FindCenter(86, 10);
		tileAdjInfo[87] = FindCenter(87, 10);
		tileAdjInfo[88] = FindCenter(88, 10);
	}
};
//===============================================================================================================================
/*
20x20

000 020 040 060 080 100 120 140 160 180 200 220 240 260 280 300 320 340 360 380
001 021 041 061 081 101 121 141 161 181 201 221 241 261 281 301 321 341 361 381
002 022 042 062 082 102 122 142 162 182 202 222 242 262 282 302 322 342 362 382
003 023 043 063 083 103 123 143 163 183 203 223 243 263 283 303 323 343 363 383
004 024 044 064 084 104 124 144 164 184 204 224 244 264 284 304 324 344 364 384
005 025 045 065 085 105 125 145 165 185 205 225 245 265 285 305 325 345 365 385
006 026 046 066 086 106 126 146 166 186 206 226 246 266 286 306 326 346 366 386
007 027 047 067 087 107 127 147 167 187 207 227 247 267 287 307 327 347 367 387
008 028 048 068 088 108 128 148 168 188 208 228 248 268 288 308 328 348 368 388
009 029 049 069 089 109 129 149 169 189 209 229 249 269 289 309 329 349 369 389
010 030 050 070 090 110 130 150 170 190 210 230 250 270 290 310 330 350 370 390
011 031 051 071 091 111 131 151 171 191 211 231 251 271 291 311 331 351 371 391
012 032 052 072 092 112 132 152 172 192 212 232 252 272 292 312 332 352 372 392
013 033 053 073 093 113 133 153 173 193 213 233 253 273 293 313 333 353 373 393
014 034 054 074 094 114 134 154 174 194 214 234 254 274 294 314 334 354 374 394
015 035 055 075 095 115 135 155 175 195 215 235 255 275 295 315 335 355 375 395
016 036 056 076 096 116 136 156 176 196 216 236 256 276 296 316 336 356 376 396
017 037 057 077 097 117 137 157 177 197 217 237 257 277 297 317 337 357 377 397
018 038 058 078 098 118 138 158 178 198 218 238 258 278 298 318 338 358 378 398
019 039 059 079 099 119 139 159 179 199 219 239 259 279 299 319 339 359 379 399
*/
struct TileAdjInfo20x20
{
	static std::unique_ptr<SectionAdjacency> tileAdjInfo[400];

	TileAdjInfo20x20()
	{
		tileAdjInfo[  0] = FindCorner(0, 20, 0);
		tileAdjInfo[ 19] = FindCorner(19, 20, 1);
		tileAdjInfo[380] = FindCorner(380, 20, 2);
		tileAdjInfo[399] = FindCorner(399, 20, 3);

		for (int i = 1; i < 18; i++)
			tileAdjInfo[i] = FindEdgeWest(i, 20);

		for (int i = 20; i < 360; i+=20)
			tileAdjInfo[i] = FindEdgeNorth(i, 20);

		for (int i = 381; i < 398; i++)
			tileAdjInfo[i] = FindEdgeEast(i, 20);

		for (int i = 39; i < 379; i+=20)
			tileAdjInfo[i] = FindEdgeSouth(i, 20);

		for (int i = 21; i < 38; i++)
			tileAdjInfo[i] = FindCenter(i, 20);

		for (int i = 41; i < 58; i++)
			tileAdjInfo[i] = FindCenter(i, 20);

		for (int i = 61; i < 78; i++)
			tileAdjInfo[i] = FindCenter(i, 20);

		for (int i = 81; i < 98; i++)
			tileAdjInfo[i] = FindCenter(i, 20);

		for (int i = 101; i < 118; i++)
			tileAdjInfo[i] = FindCenter(i, 20);

		for (int i = 121; i < 138; i++)
			tileAdjInfo[i] = FindCenter(i, 20);

		for (int i = 141; i < 158; i++)
			tileAdjInfo[i] = FindCenter(i, 20);

		for (int i = 161; i < 178; i++)
			tileAdjInfo[i] = FindCenter(i, 20);

		for (int i = 181; i < 198; i++)
			tileAdjInfo[i] = FindCenter(i, 20);

		for (int i = 201; i < 218; i++)
			tileAdjInfo[i] = FindCenter(i, 20);

		for (int i = 221; i < 238; i++)
			tileAdjInfo[i] = FindCenter(i, 20);

		for (int i = 241; i < 258; i++)
			tileAdjInfo[i] = FindCenter(i, 20);

		for (int i = 261; i < 278; i++)
			tileAdjInfo[i] = FindCenter(i, 20);

		for (int i = 281; i < 298; i++)
			tileAdjInfo[i] = FindCenter(i, 20);

		for (int i = 301; i < 318; i++)
			tileAdjInfo[i] = FindCenter(i, 20);

		for (int i = 321; i < 338; i++)
			tileAdjInfo[i] = FindCenter(i, 20);

		for (int i = 341; i < 358; i++)
			tileAdjInfo[i] = FindCenter(i, 20);

		for (int i = 361; i < 378; i++)
			tileAdjInfo[i] = FindCenter(i, 20);
	}
};
//===============================================================================================================================
/*
30x30

000 030 060 090 120 150 180 210 240 270 300 330 360 390 420 450 480 510 540 570 600 630 660 690 720 750 780 810 840 870
001 031 061 091 121 151 181 211 241 271 301 331 361 391 421 451 481 511 541 571 601 631 661 691 721 751 781 811 841 871
002 032 062 092 122 152 182 212 242 272 302 332 362 392 422 452 482 512 542 572 602 632 662 692 722 752 782 812 842 872
003 033 063 093 123 153 183 213 243 273 303 333 363 393 423 453 483 513 543 573 603 633 663 693 723 753 783 813 843 873
004 034 064 094 124 154 184 214 244 274 304 334 364 394 424 454 484 514 544 574 604 634 664 694 724 754 784 814 844 874
005 035 065 095 125 155 185 215 245 275 305 335 365 395 425 455 485 515 545 575 605 635 665 695 725 755 785 815 845 875
006 036 066 096 126 156 186 216 246 276 306 336 366 396 426 456 486 516 546 576 606 636 666 696 726 756 786 816 846 876
007 037 067 097 127 157 187 217 247 277 307 337 367 397 427 457 487 517 547 577 607 637 667 697 727 757 787 817 847 877
008 038 068 098 128 158 188 218 248 278 308 338 368 398 428 458 488 518 548 578 608 638 668 698 728 758 788 818 848 878
009 039 069 099 129 159 189 219 249 279 309 339 369 399 429 459 489 519 549 579 609 639 669 699 729 759 789 819 849 879
010 040 070 100 130 160 190 220 250 280 310 340 370 400 430 460 490 520 550 580 610 640 670 700 730 760 790 820 850 880
011 041 071 101 131 161 191 221 251 281 311 341 371 401 431 461 491 521 551 581 611 641 671 701 731 761 791 821 851 881
012 042 072 102 132 162 192 222 252 282 312 342 372 402 432 462 492 522 552 582 612 642 672 702 732 762 792 822 852 882
013 043 073 103 133 163 193 223 253 283 313 343 373 403 433 463 493 523 553 583 613 643 673 703 733 763 793 823 853 883
014 044 074 104 134 164 194 224 254 284 314 344 374 404 434 464 494 524 554 584 614 644 674 704 734 764 794 824 854 884
015 045 075 105 135 165 195 225 255 285 315 345 375 405 435 465 495 525 555 585 615 645 675 705 735 765 795 825 855 885
016 046 076 106 136 166 196 226 256 286 316 346 376 406 436 466 496 526 556 586 616 646 676 706 736 766 796 826 856 886
017 047 077 107 137 167 197 227 257 287 317 347 377 407 437 467 497 527 557 587 617 647 677 707 737 767 797 827 857 887
018 048 078 108 138 168 198 228 258 288 318 348 378 408 438 468 498 528 558 588 618 648 678 708 738 768 798 828 858 888
019 049 079 109 139 169 199 229 259 289 319 349 379 409 439 469 499 529 559 589 619 649 679 709 739 769 799 829 859 889
020 050 080 110 140 170 200 230 260 290 320 350 380 410 440 470 500 530 560 590 620 650 680 710 740 770 800 830 860 890
021 051 081 111 141 171 201 231 261 291 321 351 381 411 441 471 501 531 561 591 621 651 681 711 741 771 801 831 861 891
022 052 082 112 142 172 202 232 262 292 322 352 382 412 442 472 502 532 562 592 622 652 682 712 742 772 802 832 862 892
023 053 083 113 143 173 203 233 263 293 323 353 383 413 443 473 503 533 563 593 623 653 683 713 743 773 803 833 863 893
024 054 084 114 144 174 204 234 264 294 324 354 384 414 444 474 504 534 564 594 624 654 684 714 744 774 804 834 864 894
025 055 085 115 145 175 205 235 265 295 325 355 385 415 445 475 505 535 565 595 625 655 685 715 745 775 805 835 865 895
026 056 086 116 146 176 206 236 266 296 326 356 386 416 446 476 506 536 566 596 626 656 686 716 746 776 806 836 866 896
027 057 087 117 147 177 207 237 267 297 327 357 387 417 447 477 507 537 567 597 627 657 687 717 747 777 807 837 867 897
028 058 088 118 148 178 208 238 268 298 328 358 388 418 448 478 508 538 568 598 628 658 688 718 748 778 808 838 868 898
029 059 089 119 149 179 209 239 269 299 329 359 389 419 449 479 509 539 569 599 629 659 689 719 749 779 809 839 869 899
*/
struct TileAdjInfo30x30
{
	static std::unique_ptr<SectionAdjacency> tileAdjInfo[900];

	TileAdjInfo30x30()
	{
		tileAdjInfo[0] = FindCorner(0, 30, 0);
		tileAdjInfo[29] = FindCorner(29, 30, 1);
		tileAdjInfo[870] = FindCorner(870, 30, 2);
		tileAdjInfo[899] = FindCorner(899, 30, 3);

		for (int i = 1; i < 28; i++)
			tileAdjInfo[i] = FindEdgeWest(i, 30);

		for (int i = 30; i < 840; i += 30)
			tileAdjInfo[i] = FindEdgeNorth(i, 30);

		for (int i = 871; i < 898; i++)
			tileAdjInfo[i] = FindEdgeEast(i, 30);

		for (int i = 59; i < 869; i += 30)
			tileAdjInfo[i] = FindEdgeSouth(i, 30);

		for (int i = 31; i < 58; i++)
			tileAdjInfo[i] = FindCenter(i, 30);
		for (int i = 61; i < 88; i++)
			tileAdjInfo[i] = FindCenter(i, 30);
		for (int i = 91; i < 118; i++)
			tileAdjInfo[i] = FindCenter(i, 30);
		for (int i = 121; i < 148; i++)
			tileAdjInfo[i] = FindCenter(i, 30);
		for (int i = 151; i < 178; i++)
			tileAdjInfo[i] = FindCenter(i, 30);
		for (int i = 181; i < 208; i++)
			tileAdjInfo[i] = FindCenter(i, 30);
		for (int i = 211; i < 238; i++)
			tileAdjInfo[i] = FindCenter(i, 30);
		for (int i = 241; i < 268; i++)
			tileAdjInfo[i] = FindCenter(i, 30);
		for (int i = 271; i < 298; i++)
			tileAdjInfo[i] = FindCenter(i, 30);
		for (int i = 301; i < 328; i++)
			tileAdjInfo[i] = FindCenter(i, 30);
		for (int i = 331; i < 358; i++)
			tileAdjInfo[i] = FindCenter(i, 30);
		for (int i = 361; i < 388; i++)
			tileAdjInfo[i] = FindCenter(i, 30);
		for (int i = 391; i < 418; i++)
			tileAdjInfo[i] = FindCenter(i, 30);
		for (int i = 421; i < 448; i++)
			tileAdjInfo[i] = FindCenter(i, 30);
		for (int i = 451; i < 478; i++)
			tileAdjInfo[i] = FindCenter(i, 30);
		for (int i = 481; i < 508; i++)
			tileAdjInfo[i] = FindCenter(i, 30);
		for (int i = 511; i < 538; i++)
			tileAdjInfo[i] = FindCenter(i, 30);
		for (int i = 541; i < 568; i++)
			tileAdjInfo[i] = FindCenter(i, 30);
		for (int i = 571; i < 598; i++)
			tileAdjInfo[i] = FindCenter(i, 30);
		for (int i = 601; i < 628; i++)
			tileAdjInfo[i] = FindCenter(i, 30);
		for (int i = 631; i < 658; i++)
			tileAdjInfo[i] = FindCenter(i, 30);
		for (int i = 661; i < 688; i++)
			tileAdjInfo[i] = FindCenter(i, 30);
		for (int i = 691; i < 718; i++)
			tileAdjInfo[i] = FindCenter(i, 30);
		for (int i = 721; i < 748; i++)
			tileAdjInfo[i] = FindCenter(i, 30);
		for (int i = 751; i < 778; i++)
			tileAdjInfo[i] = FindCenter(i, 30);
		for (int i = 781; i < 808; i++)
			tileAdjInfo[i] = FindCenter(i, 30);
		for (int i = 811; i < 838; i++)
			tileAdjInfo[i] = FindCenter(i, 30);
		for (int i = 841; i < 868; i++)
			tileAdjInfo[i] = FindCenter(i, 30);
	}
};
//===============================================================================================================================
//===============================================================================================================================
#endif//__SectionAdjacency_H