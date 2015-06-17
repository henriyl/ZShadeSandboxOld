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

#include "ZMath.h"
#include "CustomMesh.h"

//===============================================================================================================================
//===============================================================================================================================
namespace ZShadeSandboxMesh {
class QuadMesh : public ZShadeSandboxMesh::CustomMesh
{
	int m_screenWidth, m_screenHeight;
	int m_bitmapWidth, m_bitmapHeight;
	float m_previousPosX, m_previousPosY;
	bool bUpdatable;
	bool bFullScreen;
	ZShadeSandboxMath::EAxis::Type mAxis;
	
	void CreateQuadVertex6Point(XMFLOAT3 positions[6]);
	
public:
	
	// updatable means means that this quad mesh can have its position updated
	// fullscreen means that this quad mesh is a fullscreen quad
	QuadMesh
	(	D3D* d3d
	,	ZShadeSandboxMesh::MeshParameters mp
	,	bool updatable = false
	,	bool fullScreen = false
	,	ZShadeSandboxMath::EAxis::Type axis = ZShadeSandboxMath::EAxis::Type::Axis_Z
	);
	
	~QuadMesh();
	
	//Re-build the dynamic vertex buffer for rendering to a different location on the screen
	void UpdateBuffers(XMFLOAT2 position);
	void UpdateBuffers(XMFLOAT3 position);
	void UpdateBuffers(float positionX, float positionY);
	
	void Initialize();
	
	// Loads 4 Quad Points for tessellation
	// If the mesh parameters is set to use tessellation then this will be used
	void InitTessellation();
	
	void InitFullscreenQuad();
};
}
//===============================================================================================================================
//===============================================================================================================================
#endif//__QUADMESH_H