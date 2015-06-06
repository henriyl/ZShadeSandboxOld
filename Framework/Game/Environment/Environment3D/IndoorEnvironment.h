//===============================================================================================================================
// IndoorEnvironment
//
//===============================================================================================================================
// History
//
//	-Created on 5/4/2015 by Dustin Watson
//===============================================================================================================================
#ifndef __INDOORENVIRONMENT_H
#define __INDOORENVIRONMENT_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//
#pragma region "Includes"
#include "Environment3D.h"
#include "CubeMesh.h"
#pragma endregion

//===============================================================================================================================
//===============================================================================================================================

// Start out rendering a large cube with the camera inside it. The camera is clipped to only be inside the cube and not leave it.
// It would be cool to add a mirror into the scene. (use the depth stencil to add mirror)
// Create a room class that contains a cube with 6 planes which represent the AABB collision plane for the walls.

//
// Indoor Environment
//
class IndoorEnvironment : public Environment3D
{
public:
	IndoorEnvironment(LPCSTR base_window_name, LPCSTR render_window_name, EngineOptions* eo, bool init_lua);
	~IndoorEnvironment();

	bool Initialize();
	void Shutdown();

	void Update();
	void Render();
	void RenderDeferred();

private:
	
	ZShadeSandboxMesh::CubeMesh* mCube;
};

//===============================================================================================================================
//===============================================================================================================================
#endif//__INDOORENVIRONMENT_H