//===============================================================================================================================
// QuadMesh
//
//===============================================================================================================================
// History
//
// -Created on 7/8/2014 by Dustin Watson
//===============================================================================================================================
#ifndef __QUADMESH_H
#define __QUADMESH_H
//===============================================================================================================================
//===============================================================================================================================

//
// Includes
//
#include "CustomMesh.h"

//===============================================================================================================================
//===============================================================================================================================
namespace ZShadeSandboxMesh {
//
// Globals
//
const int NUM_QUAD_VERTS   = 6;
const int NUM_QUAD_INDICES = 6;

//===============================================================================================================================
//===============================================================================================================================
class QuadMesh : public ZShadeSandboxMesh::CustomMesh
{
	int m_screenWidth, m_screenHeight;
	int m_bitmapWidth, m_bitmapHeight;
	float m_previousPosX, m_previousPosY;
	bool bUpdatable;
	bool bFullScreen; // Is this a fullscreen quad
	
public:
	
	// updatable means means that this quad mesh can have its position updated
	QuadMesh(D3D* d3d, ZShadeSandboxMesh::MeshParameters mp, bool updatable = false, bool fullScreen = false);
	~QuadMesh();
	
	//Re-build the dynamic vertex buffer for rendering to a different location on the screen
	void UpdateBuffers(float positionX, float positionY);
	
	void Initialize();
	
	// Loads 4 Quad Points for tessellation
	void InitTessellation();
	
	void InitFullscreenQuad();
};
}
//===============================================================================================================================
//===============================================================================================================================
#endif//__QUADMESH_H