//===============================================================================================================================
// TerrainEnvironment
//
//===============================================================================================================================
// History
//
// -Created on 5/19/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __TERRAINENVIRONMENT_H
#define __TERRAINENVIRONMENT_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//

#include "Environment3D.h"
#include "QuadTree.h"
#include "QuadTreeRenderer.h"
#include "Sky.h"

//===============================================================================================================================
//===============================================================================================================================

//
// Defines what is needed for terrain to be rendered
//
class TerrainEnvironment : public Environment3D
{
public:
	TerrainEnvironment(LPCSTR base_window_name, LPCSTR render_window_name, EngineOptions* eo, bool init_lua);
	~TerrainEnvironment();
	
	void Init();
	void Shutdown();
	
	virtual void Update();
	virtual void Render();
	void RenderDeferred();

	void RenderTerrainShadowSSAO();
	
protected:
	
	//
	// Terrain
	//
	ZShadeSandboxTerrain::QuadTreeMesh* m_pQuadTreeMesh;
	ZShadeSandboxTerrain::QuadTreeRenderer* m_pQuadTreeRenderer;

	//
	// Sky
	//
	Sky* mSky;
	
	float fHeightScale;
	float fTerrSize;
	float fMinDist;
	float fMaxDist;
	float fTerrRenderSize;
	float fMapSize;
	float fLeafWidth;
	
	bool bToggleTerrainSSAO;
	bool bToggleTerrainShadow;
	bool bToggleTerrainNormalMap;
	bool bToggleTerrainDetailMap;
	bool bRenderFullMap;
	bool bRenderExtraRenderDist;
	bool bClipCameraToTerrain;
};

//===============================================================================================================================
//===============================================================================================================================
#endif//__OUTDOORENVIRONMENT_H