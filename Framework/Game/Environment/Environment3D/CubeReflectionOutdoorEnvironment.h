//===============================================================================================================================
// CubeReflectionEnvironment
//
//===============================================================================================================================
// History
//
//		-Created on 6/15/2014 by Dustin Watson
//===============================================================================================================================
#ifndef __CUBEREFLECTIONENVIRONMENT_H
#define __CUBEREFLECTIONENVIRONMENT_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//
#pragma region "Includes"
#include "OutdoorEnvironment.h"
#include "Vertex.h"
#include "Water.h"
#include "WaterShader.h"
#include "Sky.h"
#include "SkyShader.h"
#include "CubeMesh.h"
#include "CSDesaturation.h"
#pragma endregion

//
// Globals
//

//===============================================================================================================================
//===============================================================================================================================

//
// Cube Reflection Environment
//
class CubeReflectionOutdoorEnvironment : public OutdoorEnvironment
{
public:
	CubeReflectionOutdoorEnvironment(LPCSTR base_window_name, LPCSTR render_window_name, EngineOptions* eo, bool init_lua);
	~CubeReflectionOutdoorEnvironment();

	bool Initialize();
	void Shutdown();

	void Update();
	void Render();
	void RenderDeferred();

	void RenderReflection(XMFLOAT4 clipplane);
	void RenderRefraction(XMFLOAT4 clipplane);

private:
	
	//
	// Cube
	//
	ZShadeSandboxMesh::CustomMesh* mCube;
	CSDesaturation* pDesaturationCS;
};

//===============================================================================================================================
//===============================================================================================================================
#endif//__CUBEREFLECTIONENVIRONMENT_H