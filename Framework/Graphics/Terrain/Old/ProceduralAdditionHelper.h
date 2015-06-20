//===============================================================================================================================
// ProceduralAdditionHelper
//
// This class will add things to a heightmap
//===============================================================================================================================
// History
//
// -Created on 6/18/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __PROCEDURALADDITIONHELPER_H
#define __PROCEDURALADDITIONHELPER_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//

#include "D3D.h"
#include "HeightmapContainer.h"

namespace ZShadeSandboxTerrain {
	struct ProceduralParameters;
}

//===============================================================================================================================
namespace ZShadeSandboxTerrain {
class ProceduralAdditionHelper
{
public:
	
	ProceduralAdditionHelper(HeightmapContainer heightmapInput, ZShadeSandboxTerrain::ProceduralParameters pp);
	~ProceduralAdditionHelper();
	
	void AddRandomHeightmap();
	void AddPerlinNoiseHeightmap();
	void AddFieldNoiseHeightmap();
	
	// Updates the heightmap with new values radiating from an origin point
	void RadiateHeightFromOrigin(int centerX, int centerZ, int radius, float minHeight, float maxHeight, bool addToInput);
	
	HeightmapContainer ProceduralHeight() { return mProceduralHeightmap; }
	
private:
	
	ZShadeSandboxTerrain::ProceduralParameters mProceduralParameters;
	
	// Input heightmap to start from
	HeightmapContainer mHeightMapInput;
	
	// The resulting height map after it has been affected
	HeightmapContainer mProceduralHeightmap;
};
}
//===============================================================================================================================
#endif//__PROCEDURALADDITIONHELPER_H