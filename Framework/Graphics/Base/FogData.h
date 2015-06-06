//******************************************************************************************************
// FogData.h
//******************************************************************************************************
#ifndef __FOGDATA_H
#define __FOGDATA_H
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//Includes
#include "D3D.h"
#include "QuadMesh.h"
#include "MaterialManager.h"
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
//Defines
#define NULL               0
#define FOW_RADIUS_MIN     3
#define FOW_RADIUS_MAX     9
#define NUM_FOW_RADII      ((FOW_RADIUS_MAX - FOW_RADIUS_MIN) + 1)
#define FOW_MAX_RADIUS_LENGTH ((FOW_RADIUS_MAX * 2) + 1)
//Define the 9 fog bits       Left Shift
#define FOW_BIT_NW (1 << 0) //00000000
#define FOW_BIT_N  (1 << 1) //00000010
#define FOW_BIT_NE (1 << 2) //00000100
#define FOW_BIT_W  (1 << 3) //00000110
#define FOW_BIT_C  (1 << 4) //00001000
#define FOW_BIT_E  (1 << 5) //00001010
#define FOW_BIT_SW (1 << 6) //00001100
#define FOW_BIT_S  (1 << 7) //00001110
#define FOW_BIT_SE (1 << 8) //00010000
//Create the masks that define the bit definitions
#define fow_non    0
#define fow_all    (FOW_BIT_NW | FOW_BIT_N | FOW_BIT_NE | \
					FOW_BIT_W | FOW_BIT_C | FOW_BIT_E | \
					FOW_BIT_SW | FOW_BIT_S | FOW_BIT_SE)
#define NUM_FOW_ENTRIES fow_all
//Straights
#define fow_EEE		(FOW_BIT_SE | FOW_BIT_E | FOW_BIT_NE)
#define fow_NNN		(FOW_BIT_NE | FOW_BIT_N | FOW_BIT_NW)
#define fow_WWW		(FOW_BIT_NW | FOW_BIT_W | FOW_BIT_SW)
#define fow_SSS		(FOW_BIT_SW | FOW_BIT_S | FOW_BIT_SE)
//Corners
#define fow_CNE		(FOW_BIT_E | FOW_BIT_NE | FOW_BIT_N | \
					 FOW_BIT_NW | FOW_BIT_C | FOW_BIT_SE)
#define fow_CNW		(FOW_BIT_N | FOW_BIT_NW | FOW_BIT_W | \
					 FOW_BIT_SW | FOW_BIT_C | FOW_BIT_NE)
#define fow_CSW		(FOW_BIT_W | FOW_BIT_SW | FOW_BIT_S | \
					 FOW_BIT_NW | FOW_BIT_C | FOW_BIT_SE)
#define fow_CSE		(FOW_BIT_S | FOW_BIT_SE | FOW_BIT_E | \
					 FOW_BIT_NE | FOW_BIT_C | FOW_BIT_SW)
//Joins
#define fow_JNE		(FOW_BIT_E | FOW_BIT_NE | FOW_BIT_N)
#define fow_JNW		(FOW_BIT_N | FOW_BIT_NW | FOW_BIT_W)
#define fow_JSW		(FOW_BIT_W | FOW_BIT_SW | FOW_BIT_S)
#define fow_JSE		(FOW_BIT_S | FOW_BIT_SE | FOW_BIT_E)
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
static unsigned short circle_mask[NUM_FOW_RADII][FOW_MAX_RADIUS_LENGTH * FOW_MAX_RADIUS_LENGTH] =
{
	//Radius 3
	{
		fow_all, fow_all, fow_CNW, fow_NNN, fow_CNE, fow_all, fow_all,
		fow_all, fow_CNW, fow_JNW, fow_non, fow_JNE, fow_CNE, fow_all,
		fow_CNW, fow_JNW, fow_non, fow_non, fow_non, fow_JNE, fow_CNE,
		fow_WWW, fow_non, fow_non, fow_non, fow_non, fow_non, fow_EEE,
		fow_CSW, fow_JSW, fow_non, fow_non, fow_non, fow_JSE, fow_CSE,
		fow_all, fow_CSW, fow_JSW, fow_non, fow_JSE, fow_CSE, fow_all,
		fow_all, fow_all, fow_CSW, fow_SSS, fow_CSE, fow_all, fow_all,
	},
	//Radius 4
	{
		fow_all, fow_all, fow_all, fow_CNW, fow_NNN, fow_CNE, fow_all, fow_all, fow_all,
		fow_all, fow_all, fow_CNW, fow_JNW, fow_non, fow_JNE, fow_CNE, fow_all, fow_all,
		fow_all, fow_CNW, fow_JNW, fow_non, fow_non, fow_non, fow_JNE, fow_CNE, fow_all,
		fow_CNW, fow_JNW, fow_non, fow_non, fow_non, fow_non, fow_non, fow_JNE, fow_CNE,
		fow_WWW, fow_non, fow_non, fow_non, fow_non, fow_non, fow_non, fow_non, fow_EEE,
		fow_CSW, fow_JSW, fow_non, fow_non, fow_non, fow_non, fow_non, fow_JSE, fow_CSE,
		fow_all, fow_CSW, fow_JSW, fow_non, fow_non, fow_non, fow_JSE, fow_CSE, fow_all,
		fow_all, fow_all, fow_CSW, fow_JSW, fow_non, fow_JSE, fow_CSE, fow_all, fow_all,
		fow_all, fow_all, fow_all, fow_CSW, fow_SSS, fow_CSE, fow_all, fow_all, fow_all,
	},
	//Radius 5
	{
		fow_all, fow_all, fow_all, fow_CNW, fow_NNN, fow_NNN, fow_NNN, fow_CNE, fow_all, fow_all, fow_all,
		fow_all, fow_all, fow_CNW, fow_JNW, fow_non, fow_non, fow_non, fow_JNE, fow_CNE, fow_all, fow_all,
		fow_all, fow_CNW, fow_JNW, fow_non, fow_non, fow_non, fow_non, fow_non, fow_JNE, fow_CNE, fow_all,
		fow_all, fow_WWW, fow_non, fow_non, fow_non, fow_non, fow_non, fow_non, fow_non, fow_EEE, fow_all,
		fow_CNW, fow_JNW, fow_non, fow_non, fow_non, fow_non, fow_non, fow_non, fow_non, fow_JNE, fow_CNE,
		fow_WWW, fow_non, fow_non, fow_non, fow_non, fow_non, fow_non, fow_non, fow_non, fow_non, fow_EEE,
		fow_CSW, fow_JSW, fow_non, fow_non, fow_non, fow_non, fow_non, fow_non, fow_non, fow_JSE, fow_CSE,
		fow_all, fow_WWW, fow_non, fow_non, fow_non, fow_non, fow_non, fow_non, fow_non, fow_EEE, fow_all,
		fow_all, fow_CSW, fow_JSW, fow_non, fow_non, fow_non, fow_non, fow_non, fow_JSE, fow_CSE, fow_all,
		fow_all, fow_all, fow_CSW, fow_JSW, fow_non, fow_non, fow_non, fow_JSE, fow_CSE, fow_all, fow_all,
		fow_all, fow_all, fow_all, fow_CSW, fow_SSS, fow_SSS, fow_SSS, fow_CSE, fow_all, fow_all, fow_all,
	},
	//Radius 6
	{
		fow_all,fow_all,fow_all,fow_all,fow_CNW,fow_NNN,fow_NNN,fow_NNN,fow_CNE,fow_all,fow_all,fow_all,fow_all,
		fow_all,fow_all,fow_all,fow_CNW,fow_JNW,fow_non,fow_non,fow_non,fow_JNE,fow_CNE,fow_all,fow_all,fow_all,
		fow_all,fow_all,fow_CNW,fow_JNW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_JNE,fow_CNE,fow_all,fow_all,
		fow_all,fow_CNW,fow_JNW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_JNE,fow_CNE,fow_all,
		fow_CNW,fow_JNW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_JNE,fow_CNE,
		fow_WWW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_EEE,
		fow_WWW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_EEE,
		fow_WWW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_EEE,
		fow_CSW,fow_JSW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_JSE,fow_CSE,
		fow_all,fow_CSW,fow_JSW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_JSE,fow_CSE,fow_all,
		fow_all,fow_all,fow_CSW,fow_JSW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_JSE,fow_CSE,fow_all,fow_all,
		fow_all,fow_all,fow_all,fow_CSW,fow_JSW,fow_non,fow_non,fow_non,fow_JSE,fow_CSE,fow_all,fow_all,fow_all,
		fow_all,fow_all,fow_all,fow_all,fow_CSW,fow_SSS,fow_SSS,fow_SSS,fow_CSE,fow_all,fow_all,fow_all,fow_all,
	},
	//Radius 7
	{
		fow_all,fow_all,fow_all,fow_all,fow_all,fow_CNW,fow_NNN,fow_NNN,fow_NNN,fow_CNE,fow_all,fow_all,fow_all,fow_all,fow_all,
		fow_all,fow_all,fow_all,fow_CNW,fow_NNN,fow_JNW,fow_non,fow_non,fow_non,fow_JNE,fow_NNN,fow_CNE,fow_all,fow_all,fow_all,
		fow_all,fow_all,fow_CNW,fow_JNW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_JNE,fow_CNE,fow_all,fow_all,
		fow_all,fow_CNW,fow_JNW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_JNE,fow_CNE,fow_all,
		fow_all,fow_WWW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_EEE,fow_all,
		fow_CNW,fow_JNW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_JNE,fow_CNE,
		fow_WWW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_EEE,
		fow_WWW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_EEE,
		fow_WWW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_EEE,
		fow_CSW,fow_JSW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_JSE,fow_CSE,
		fow_all,fow_WWW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_EEE,fow_all,
		fow_all,fow_CSW,fow_JSW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_JSE,fow_CSE,fow_all,
		fow_all,fow_all,fow_CSW,fow_JSW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_JSE,fow_CSE,fow_all,fow_all,
		fow_all,fow_all,fow_all,fow_CSW,fow_SSS,fow_JSW,fow_non,fow_non,fow_non,fow_JSE,fow_SSS,fow_CSE,fow_all,fow_all,fow_all,
		fow_all,fow_all,fow_all,fow_all,fow_all,fow_CSW,fow_SSS,fow_SSS,fow_SSS,fow_CSE,fow_all,fow_all,fow_all,fow_all,fow_all,
	},
	//Radius 8
	{
		fow_all,fow_all,fow_all,fow_all,fow_all,fow_all,fow_CNW,fow_NNN,fow_NNN,fow_NNN,fow_CNE,fow_all,fow_all,fow_all,fow_all,fow_all,fow_all,
		fow_all,fow_all,fow_all,fow_all,fow_CNW,fow_NNN,fow_JNW,fow_non,fow_non,fow_non,fow_JNE,fow_NNN,fow_CNE,fow_all,fow_all,fow_all,fow_all,
		fow_all,fow_all,fow_all,fow_CNW,fow_JNW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_JNE,fow_CNE,fow_all,fow_all,fow_all,
		fow_all,fow_all,fow_CNW,fow_JNW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_JNE,fow_CNE,fow_all,fow_all,
		fow_all,fow_CNW,fow_JNW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_JNE,fow_CNE,fow_all,
		fow_all,fow_WWW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_EEE,fow_all,
		fow_CNW,fow_JNW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_JNE,fow_CNE,
		fow_WWW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_EEE,
		fow_WWW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_EEE,
		fow_WWW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_EEE,
		fow_CSW,fow_JSW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_JSE,fow_CSE,
		fow_all,fow_WWW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_EEE,fow_all,
		fow_all,fow_CSW,fow_JSW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_JSE,fow_CSE,fow_all,
		fow_all,fow_all,fow_CSW,fow_JSW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_JSE,fow_CSE,fow_all,fow_all,
		fow_all,fow_all,fow_all,fow_CSW,fow_JSW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_JSE,fow_CSE,fow_all,fow_all,fow_all,
		fow_all,fow_all,fow_all,fow_all,fow_CSW,fow_SSS,fow_JSW,fow_non,fow_non,fow_non,fow_JSE,fow_SSS,fow_CSE,fow_all,fow_all,fow_all,fow_all,
		fow_all,fow_all,fow_all,fow_all,fow_all,fow_all,fow_CSW,fow_SSS,fow_SSS,fow_SSS,fow_CSE,fow_all,fow_all,fow_all,fow_all,fow_all,fow_all,
	},
	//Radius 9
	{
		fow_all,fow_all,fow_all,fow_all,fow_all,fow_all,fow_all,fow_CNW,fow_NNN,fow_NNN,fow_NNN,fow_CNE,fow_all,fow_all,fow_all,fow_all,fow_all,fow_all,fow_all,
		fow_all,fow_all,fow_all,fow_all,fow_all,fow_CNW,fow_NNN,fow_JNW,fow_non,fow_non,fow_non,fow_JNE,fow_NNN,fow_CNE,fow_all,fow_all,fow_all,fow_all,fow_all,
		fow_all,fow_all,fow_all,fow_CNW,fow_NNN,fow_JNW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_JNE,fow_NNN,fow_CNE,fow_all,fow_all,fow_all,
		fow_all,fow_all,fow_CNW,fow_JNW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_JNE,fow_CNE,fow_all,fow_all,
		fow_all,fow_all,fow_WWW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_EEE,fow_all,fow_all,
		fow_all,fow_CNW,fow_JNW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_JNE,fow_CNE,fow_all,
		fow_all,fow_WWW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_EEE,fow_all,
		fow_CNW,fow_JNW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_JNE,fow_CNE,
		fow_WWW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_EEE,
		fow_WWW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_EEE,
		fow_WWW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_EEE,
		fow_CSW,fow_JSW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_JSE,fow_CSE,
		fow_all,fow_WWW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_EEE,fow_all,
		fow_all,fow_CSW,fow_JSW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_JSE,fow_CSE,fow_all,
		fow_all,fow_all,fow_WWW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_EEE,fow_all,fow_all,
		fow_all,fow_all,fow_CSW,fow_JSW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_JSE,fow_CSE,fow_all,fow_all,
		fow_all,fow_all,fow_all,fow_CSW,fow_SSS,fow_JSW,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_non,fow_JSE,fow_SSS,fow_CSE,fow_all,fow_all,fow_all,
		fow_all,fow_all,fow_all,fow_all,fow_all,fow_CSW,fow_SSS,fow_JSW,fow_non,fow_non,fow_non,fow_JSE,fow_SSS,fow_CSE,fow_all,fow_all,fow_all,fow_all,fow_all,
		fow_all,fow_all,fow_all,fow_all,fow_all,fow_all,fow_all,fow_CSW,fow_SSS,fow_SSS,fow_SSS,fow_CSE,fow_all,fow_all,fow_all,fow_all,fow_all,fow_all,fow_all,
	}
};
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
class FogData
{
	static D3D* m_D3DSystem;
	static signed char* m_fow_frame_table;
	static FogData* instance;
public:
	static ZShadeSandboxMesh::QuadMesh* fow_all_texture;
	static ZShadeSandboxMesh::QuadMesh* fow_CNW_texture;
	static ZShadeSandboxMesh::QuadMesh* fow_CNE_texture;
	static ZShadeSandboxMesh::QuadMesh* fow_CSW_texture;
	static ZShadeSandboxMesh::QuadMesh* fow_CSE_texture;
	static ZShadeSandboxMesh::QuadMesh* fow_JNW_texture;
	static ZShadeSandboxMesh::QuadMesh* fow_JNE_texture;
	static ZShadeSandboxMesh::QuadMesh* fow_JSW_texture;
	static ZShadeSandboxMesh::QuadMesh* fow_JSE_texture;
	static ZShadeSandboxMesh::QuadMesh* fow_NNN_texture;
	static ZShadeSandboxMesh::QuadMesh* fow_SSS_texture;
	static ZShadeSandboxMesh::QuadMesh* fow_EEE_texture;
	static ZShadeSandboxMesh::QuadMesh* fow_WWW_texture;

	FogData(D3D* d3d);
	FogData(const FogData& other);
	~FogData();

	static FogData* NewInstance(D3D* d3d);
	static FogData* Instance();

	static void InitializeTable()
	{
		//Initialize the lookup table
		m_fow_frame_table = new signed char[NUM_FOW_ENTRIES];

		for (int i = 0; i < NUM_FOW_ENTRIES; i++)
		{
			m_fow_frame_table[i] = -1;
		}

		//                         Frame ID's
		m_fow_frame_table[0x5F]  = 0;
		m_fow_frame_table[0x137] = 1;
		m_fow_frame_table[0x1F4] = 2;
		m_fow_frame_table[0x1D9] = 3;
		m_fow_frame_table[0x7]   = 4;
		m_fow_frame_table[0x1C0] = 5;
		m_fow_frame_table[0x49]  = 6;
		m_fow_frame_table[0x124] = 7;
		m_fow_frame_table[0xB]   = 8;
		m_fow_frame_table[0x26]  = 9;
		m_fow_frame_table[0x1A0] = 10;
		m_fow_frame_table[0xC8]  = 11;
		m_fow_frame_table[0x1FF] = 12;
		m_fow_frame_table[0x0]   = 13;
	}

	static void LoadTextures(string base_path)
	{
		ZShadeSandboxMesh::MeshParameters mp;

		//mp.color = XMFLOAT4(0, 0, 0, 0);
		mp.pos = XMFLOAT3(0, 0, 0);
		mp.rot = XMFLOAT3(0, 0, 0);
		mp.scale = XMFLOAT3(1, 1, 1);
		mp.rotationAxisX = false;
		mp.rotationAxisY = false;
		mp.rotationAxisZ = false;
		mp.vertexType = ZShadeSandboxMesh::EVertexType::VT_Tex;
		mp.useCustomShader = false;
		mp.textureWidth = 64;
		mp.textureHeight = 64;
		mp.in2D = true;
		//mp.texturePath = base_path;

		//mp.textureName = "fow\\fow_all.png";
		mp.material = MaterialManager::Instance()->GetMaterial("FOW ALL");
		fow_all_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp, true);
		
		//Corner Textures
		//mp.textureName = "fow\\fow_CNW.png";
		mp.material = MaterialManager::Instance()->GetMaterial("FOW CNW");
		fow_CNW_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp, true);
		//mp.textureName = "fow\\fow_CNE.png";
		mp.material = MaterialManager::Instance()->GetMaterial("FOW CNE");
		fow_CNE_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp, true);
		//mp.textureName = "fow\\fow_CSW.png";
		mp.material = MaterialManager::Instance()->GetMaterial("FOW CSW");
		fow_CSW_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp, true);
		//mp.textureName = "fow\\fow_CSE.png";
		mp.material = MaterialManager::Instance()->GetMaterial("FOW CSE");
		fow_CSE_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp, true);
		
		//Joint Textures
		//mp.textureName = "fow\\fow_JNW.png";
		mp.material = MaterialManager::Instance()->GetMaterial("FOW JNW");
		fow_JNW_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp, true);
		//mp.textureName = "fow\\fow_JNE.png";
		mp.material = MaterialManager::Instance()->GetMaterial("FOW JNE");
		fow_JNE_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp, true);
		//mp.textureName = "fow\\fow_JSW.png";
		mp.material = MaterialManager::Instance()->GetMaterial("FOW JSW");
		fow_JSW_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp, true);
		//mp.textureName = "fow\\fow_JSE.png";
		mp.material = MaterialManager::Instance()->GetMaterial("FOW JSE");
		fow_JSE_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp, true);
		
		//Straight Textures
		//mp.textureName = "fow\\fow_NNN.png";
		mp.material = MaterialManager::Instance()->GetMaterial("FOW NNN");
		fow_NNN_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp, true);
		//mp.textureName = "fow\\fow_SSS.png";
		mp.material = MaterialManager::Instance()->GetMaterial("FOW SSS");
		fow_SSS_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp, true);
		//mp.textureName = "fow\\fow_WWW.png";
		mp.material = MaterialManager::Instance()->GetMaterial("FOW WWW");
		fow_WWW_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp, true);
		//mp.textureName = "fow\\fow_EEE.png";
		mp.material = MaterialManager::Instance()->GetMaterial("FOW EEE");
		fow_EEE_texture = new ZShadeSandboxMesh::QuadMesh(m_D3DSystem, mp, true);
	}
};
//-----------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------
#endif//__FOGDATA_H