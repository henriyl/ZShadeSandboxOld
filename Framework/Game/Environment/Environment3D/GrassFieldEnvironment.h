//===============================================================================================================================
// GrassFieldEnvironment
//
//===============================================================================================================================
// History
//
//		-Created on 7/12/2014 by Dustin Watson
//===============================================================================================================================
#ifndef __GRASSFIELDENVIRONMENT_H
#define __GRASSFIELDENVIRONMENT_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//
#pragma region "Includes"
#include "OutdoorEnvironment.h"
#include "Grass.h"
#include "SimpleTerrain.h"
#include "MaterialShader.h"
#pragma endregion

//
// Globals
//

//===============================================================================================================================
//===============================================================================================================================

//
// Cube Reflection Environment
//
class GrassFieldEnvironment : public OutdoorEnvironment
{
public:
	GrassFieldEnvironment(LPCSTR base_window_name, LPCSTR render_window_name, EngineOptions* eo, bool init_lua);
	~GrassFieldEnvironment();

	bool Initialize();
	void Shutdown();

	void Update();
	void Render();
	void RenderDeferred();

	void RenderReflection(XMFLOAT4 clipplane);
	void RenderRefraction(XMFLOAT4 clipplane);

private:
	
	SimpleTerrain* m_pTerrain;
	MaterialShader* m_pMaterialShader;
	
	ID3D11ShaderResourceView* tex;
	
	//
	// Grass Field
	//
	GrassField* m_pGrassField;
};

//===============================================================================================================================
//===============================================================================================================================
#endif//__GRASSFIELDENVIRONMENT_H