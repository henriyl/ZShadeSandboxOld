//==============================================================================================================================
// QuadTreeMesh.h
//
//==============================================================================================================================
// History
//
// 4/7/2015 - Dustin Watson - Created
// 4/14/2015 - Dustin Watson - Adding QuadTree width for each node in the mesh
// 4/15/2015 - Dustin Watson - Adding support for each node to locate its triangles and node height in the mesh
// 4/19/2015 - Dustin Watson - Fixed AABB collision support and smooth camera height adjustment
// 4/29/2015 - Dustin Watson - Added terrain picking with ray tracing to pick a vertex on the terrain
//==============================================================================================================================
#ifndef __QUADTREEMESH_H
#define __QUADTREEMESH_H
//==============================================================================================================================
//==============================================================================================================================
//Windows includes
#include <Windows.h>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <fstream>
using namespace std;
//==============================================================================================================================
//==============================================================================================================================
//My Includes
#include "ShaderMaterial.h"
#include "QMeshNode.h"
#include "QuadTree.h"
#include "D3D.h"
#include "Heightmap.h"
#include "BetterString.h"
#include "TerrainParameters.h"
#include "TerrainPointCollider.h"
#include "Ray.h"
//==============================================================================================================================
namespace ZShadeSandboxTerrain {
class QuadTreeMesh
{
public:
	
	// Constructors for loading a Quad Tree Mesh
	QuadTreeMesh(D3D* d3d, ZShadeSandboxTerrain::TerrainParameters tp, GameDirectory3D* gd3d);
	QuadTreeMesh(D3D* d3d, ZShadeSandboxTerrain::QuadTree* qtree);
	
	// Copy Constructor
	QuadTreeMesh(const QuadTreeMesh& qtree);
	
	// Destructor
	~QuadTreeMesh();
	
	void Init();
	
	// Returns the height at a given position for a camera or mesh to
	// collide against the Quad Tree
	bool GetHeightAtPosition(XMFLOAT3 position, float& height);
	
	// This function is replacing GetHeightAtPosition
	bool CameraColliding(float unitsPerMeter, float gravity, Camera*& camera);
	bool Colliding(XMFLOAT3 position, vector<XMFLOAT3>& internalVertices, vector<UINT>& internalIndices);

	// Ray tracing to find out if a ray has hit the Mesh
	// Has optional parameters to get the SRV for height of an area for displacement
	void Intersects(ZShadeSandboxMath::Ray ray, bool& hit, XMFLOAT3& hitPoint);
	
	// Creates a square area of height based on a point and size of square
	// and returns a texture based on the height of that area.
	void GenerateHeightQuad(XMFLOAT3 point, int areaSize, ID3D11ShaderResourceView*& heightAreaSRV);
	
	// Performs a mesh regeneration of the entire Quad Tree
	void ReGenMesh(int newTerrainScale);
	
	// Decomposes a point after it has been translated to terrain world position
	void DecomposePoint(XMFLOAT3 pointIn, XMFLOAT3& pointOut);

	// Transforms a point to terrain world position
	void TransformPoint(XMFLOAT3 pointIn, XMFLOAT3& pointOut);

	//
	// Material for the Quad Tree Mesh
	//
	
	void AddMaterialColors(XMFLOAT4 ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
						   XMFLOAT4 diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
						   XMFLOAT4 specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 64.0f));
	
	void AddDiffuseColor(XMFLOAT4 diffuse);
	void AddAmbientColor(XMFLOAT4 ambient);
	void AddSpecularColor(XMFLOAT4 specular);
	
	void AddSpecularPower(float power = 0.8f);
	void AddSpecularIntensity(float intensity = 0.3f);
	
	// Creates the textures used by the terrain
	//void AddMaterialTextures(vector<string> textureNames, string basePath, string blendMapFilename, string normalMapFilename, string detailMapFilename);
	
	//void AddDiffuseLayerMap(vector<string> textureNames);
	
	void ClearLayerMapNames();
	void AddDiffuseLayerMapName(string textureName, int position);
	void CreateDiffuseLayerMap();
	
	void AddBlendMap(string basePath, string blendMapFilename);
	void AddNormalMap(string basePath, string normalMapFilename);
	void AddDetailMap(string basePath, string detailMapFilename);
	
public:
	
	bool IsLoaded() const										{ return m_meshLoaded; }
	bool Tessellate() const										{ return m_QuadTree->Tessellate(); }
	int GetRenderPrimitive() const 								{ return m_QuadTree->GetRenderPrimitive(); }
	int TriangleCount() const									{ return m_QuadTree->TriangleCount(); }
	ZShadeSandboxMesh::CubeMesh* GetRootBoxMesh()				{ return root_box_mesh; }
	ZShadeSandboxTerrain::QMeshNode* GetRootNode()				{ return m_MeshNodes; }
	ZShadeSandboxLighting::ShaderMaterial* GetMaterial()		{ return mMaterial; }
	ID3D11ShaderResourceView* GetHeightMapSRV() 				{ return m_QuadTree->GetHeightMapSRV(); }
	ZShadeSandboxTerrain::QuadTree* GetQuadTree()				{ return m_QuadTree; }
	bool CameraCollided() const									{ return m_cameraCollided; }
	
	// Attributes for updating the height of the mesh
	float& HeightScale()										{ return m_heightScale; }
	float& TerrainZScale()										{ return m_terrainZScale; }
	
	// Update tessellation of the mesh
	bool& Tessellate()											{ return m_QuadTree->Tessellate(); }
	
	vector<Triangle> InternalTriangles()						{ return mInternalTriangles; }

	vector<XMFLOAT3> VertexList()								{ return vertexList; }
	vector<UINT> IndexList()									{ return indexList; }

private:
	
	void LatheCollisionMesh();

	// Find highest point in the node at a position
	bool GetHeightAtPosition(ZShadeSandboxTerrain::QMeshNode* node, XMFLOAT3 position, float& height);
	bool FindNodeHeight(ZShadeSandboxTerrain::QMeshNode* node, XMFLOAT3 position, float& height);
	
	bool CameraColliding(float unitsPerMeter, float gravity, Camera*& camera, ZShadeSandboxTerrain::QMeshNode* node);
	bool Colliding(XMFLOAT3 position, vector<XMFLOAT3>& internalVertices, vector<UINT>& internalIndices, ZShadeSandboxTerrain::QMeshNode* node);
	bool FindInternalTriangles(XMFLOAT3 position, vector<XMFLOAT3>& internalVertices, vector<UINT>& internalIndices, ZShadeSandboxTerrain::QMeshNode* node);

	void Intersects(QMeshNode* node, ZShadeSandboxMath::Ray ray, bool& hit, XMFLOAT3& hitPoint);
	
	// Creates a square area of height based on a point and size of square
	// and returns a texture based on the height of that area.
	void GenerateHeightQuad(QMeshNode* node, XMFLOAT3 point, int areaSize, vector<float>& heightList);
	
	// Generates a box for each node in the Quad Tree
	void CreateNodeBox();
	void CreateNodeBox(ZShadeSandboxTerrain::QMeshNode*& node);
	void ReGenNodeBox();
	void ReGenNodeBox(ZShadeSandboxTerrain::QMeshNode*& node);

	// Generates a sphere for each node in the Quad Tree
	void CreateNodeSphere();
	void CreateNodeSphere(ZShadeSandboxTerrain::QMeshNode*& node);

	// Generates the nodes required for the mesh
	void BuildMeshNodes();
	void BuildMeshNodes(ZShadeSandboxTerrain::QNode* node, float width, ZShadeSandboxTerrain::QMeshNode*& meshNode, ZShadeSandboxTerrain::QMeshNode* parent);

	// Deletes the vertex data and indice data from each node
	void ClearMesh(bool removeIndices);
	void ClearMesh(ZShadeSandboxTerrain::QMeshNode*& node, bool removeIndices);
	
	//
	// Generate the mesh
	//
	
	// Recursively generates the base mesh for each leaf in the Quad Tree
	void GenerateMesh();
	void GenerateMesh(ZShadeSandboxTerrain::QMeshNode*& node);
	
	// Generates a mesh with no tessellation for each leaf in the Quad Tree
	void GenerateMeshNT(ZShadeSandboxTerrain::QMeshNode*& node);
	
	// Generates a mesh with DX11 tessellation for each leaf in the Quad Tree
	void GenerateMesh3PointPatchTessellation(ZShadeSandboxTerrain::QMeshNode*& node);
	void GenerateMesh4PointPatchTessellation(ZShadeSandboxTerrain::QMeshNode*& node);
	
	// Creates the vertex buffer for the mesh
	void BuildVertexBuffer(ZShadeSandboxTerrain::QMeshNode*& node);
	
	// Creates the index buffer for the mesh
	void BuildIndexBuffer(ZShadeSandboxTerrain::QMeshNode*& node);
	
	// Create triangles internal to each node in the Quad Tree
	void LatheInternalTriangles(ZShadeSandboxTerrain::QMeshNode*& node);
	
	//
	// Regenerate the mesh
	//

	// Performs a mesh regeneration of the entire Quad Tree
	void ReGenMesh(ZShadeSandboxTerrain::QMeshNode*& node);

	// Mesh regeneration with no tessellation for each leaf in the Quad Tree
	void ReGenMeshNT(ZShadeSandboxTerrain::QMeshNode*& node);

	// Mesh regeneration with DX11 tessellation for each leaf in the Quad Tree
	void ReGenMesh3PointPatchTessellation(ZShadeSandboxTerrain::QMeshNode*& node);
	void ReGenMesh4PointPatchTessellation(ZShadeSandboxTerrain::QMeshNode*& node);

	// Rebuilds the vertex buffer for the mesh
	void ReBuildVertexBuffer(ZShadeSandboxTerrain::QMeshNode*& node);

	// Rebuilds the index buffer for the mesh
	void ReBuildIndexBuffer(ZShadeSandboxTerrain::QMeshNode*& node);
	
private:

	//
	//Variables
	//
	
	D3D* m_d3d;
	EngineOptions* m_EngineOptions;
	
	// Scaling parameters for the terrain to dynamically scale
	float m_heightScale;
	float m_terrainZScale;
	
	// The absolute maximum width of the quad tree mesh given a list of vertices (Not used)
	float m_maxWidth;
	
	// The default texture that gets applied when there is no texture
	string m_DefaultTextureName;
	
	vector<string> mLayerTextureNames;
	
	// The maximum amount of triangles in the mesh
	//int m_triangleCount;
	
	// Did the camera touch the mesh ??
	bool m_cameraCollided;
	
	bool m_meshLoaded;
	
	// Data container for the mesh
	ZShadeSandboxTerrain::QuadTree* m_QuadTree;

	// The bounding box for entire Quad Tree
	ZShadeSandboxMesh::CubeMesh* root_box_mesh;

	// The material that gets applied to the Quad Tree Mesh (Need to make sure this is used)
	// Create lighting system with material and material contains color and texture
	ZShadeSandboxLighting::ShaderMaterial* mMaterial;
	
	vector<Triangle> mInternalTriangles;

	vector<XMFLOAT3>	vertexList;
	vector<UINT>		indexList;

	// Root mesh node
	ZShadeSandboxTerrain::QMeshNode* m_MeshNodes;
};
}
//==============================================================================================================================
//==============================================================================================================================
#endif//__QUADTREEMESH_H