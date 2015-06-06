#include "QuadTreeMesh.h"
#include "Convert.h"
#include "TextureManager.h"
#include "Triangle.h"
#include "ShapeContact.h"
using ZShadeSandboxTerrain::QuadTreeMesh;
//==============================================================================================================================
//==============================================================================================================================
QuadTreeMesh::QuadTreeMesh(D3D* d3d, ZShadeSandboxTerrain::TerrainParameters tp)
:  m_d3d(d3d)
,  m_cameraCollided(false)
{
	m_EngineOptions = d3d->GetEngineOptions();

	// Create the Quad Tree
	m_QuadTree = new ZShadeSandboxTerrain::QuadTree(d3d, tp);
	
	// Create the mesh for the Quad Tree
	BuildMeshNodes();
	GenerateMesh();

	// Create the collision objects for the mesh
	CreateNodeBox();
	CreateNodeSphere();
	
	mMaterial = new ZShadeSandboxLighting::ShaderMaterial(m_d3d, "Terrain");
}
//==============================================================================================================================
QuadTreeMesh::QuadTreeMesh(D3D* d3d, ZShadeSandboxTerrain::QuadTree* qtree)
:   m_QuadTree(qtree)
,   m_d3d(d3d)
,   m_cameraCollided(false)
{
	m_EngineOptions = d3d->GetEngineOptions();
	
	// Create the mesh for the Quad Tree
	BuildMeshNodes();
	GenerateMesh();

	// Create the collision objects for the mesh
	CreateNodeBox();
	CreateNodeSphere();
	
	mMaterial = new ZShadeSandboxLighting::ShaderMaterial(m_d3d, "Terrain");
}
//==============================================================================================================================
QuadTreeMesh::QuadTreeMesh(const QuadTreeMesh& qtree)
{
}
//==============================================================================================================================
QuadTreeMesh::~QuadTreeMesh()
{
}
//==============================================================================================================================
void QuadTreeMesh::BuildMeshNodes()
{
	m_maxWidth = m_QuadTree->MapSize() * m_QuadTree->LeafWidth();
	BuildMeshNodes(m_QuadTree->GetNodes(), m_maxWidth, m_MeshNodes, 0);
}
//==============================================================================================================================
void QuadTreeMesh::BuildMeshNodes(ZShadeSandboxTerrain::QNode* node, float width, ZShadeSandboxTerrain::QMeshNode*& meshNode, ZShadeSandboxTerrain::QMeshNode* parent)
{
	if (node == 0) return;
	
	meshNode = new ZShadeSandboxTerrain::QMeshNode();
	
	meshNode->children[0] = 0;
	meshNode->children[1] = 0;
	meshNode->children[2] = 0;
	meshNode->children[3] = 0;
	
	meshNode->boundingCoord[0] = node->boundingCoord[0];
	meshNode->boundingCoord[1] = node->boundingCoord[1];
	meshNode->boundingCoord[2] = node->boundingCoord[2];
	meshNode->boundingCoord[3] = node->boundingCoord[3];
	
	meshNode->branches[0] = node->branches[0];
	meshNode->branches[1] = node->branches[1];
	meshNode->branches[2] = node->branches[2];
	meshNode->branches[3] = node->branches[3];
	
	meshNode->center = XMFLOAT3(
		((meshNode->boundingCoord[2].x - meshNode->boundingCoord[1].x) / 2),
		0.0f,
		((meshNode->boundingCoord[3].z - meshNode->boundingCoord[1].z) / 2));
	
	meshNode->center.x = abs(meshNode->center.x);
	meshNode->center.y = abs(meshNode->center.y);
	meshNode->center.z = abs(meshNode->center.z);
	
	meshNode->bContainsCamera = false;
	meshNode->parent = parent;
	meshNode->m_triangle_count = 0;
	meshNode->fWidth = width;
	meshNode->boundary = node->boundary;
	meshNode->type = node->type;
	meshNode->nodeID = node->nodeID;
	meshNode->parentID = node->parentID;
	
	BuildMeshNodes(node->children[0], width / 2, meshNode->children[0], meshNode);
	BuildMeshNodes(node->children[1], width / 2, meshNode->children[1], meshNode);
	BuildMeshNodes(node->children[2], width / 2, meshNode->children[2], meshNode);
	BuildMeshNodes(node->children[3], width / 2, meshNode->children[3], meshNode);
}
//==============================================================================================================================
void QuadTreeMesh::CreateNodeBox()
{
	//
	// Create the cube
	//

	// Center the grid in model space
	int LeafWidth = m_QuadTree->LeafWidth();
	int TerrScale = m_QuadTree->TerrainScale();
	float halfWidth = ((float)LeafWidth - 1.0f) / 2.0f;
	float halfLength = ((float)LeafWidth - 1.0f) / 2.0f;

	XMFLOAT3 boxBase1 = m_MeshNodes->boundingCoord[0];
	XMFLOAT3 boxBase2 = m_MeshNodes->boundingCoord[1];
	XMFLOAT3 boxBase3 = m_MeshNodes->boundingCoord[2];
	XMFLOAT3 boxBase4 = m_MeshNodes->boundingCoord[3];

	// If this is not a leaf node we need to scale the box fit around all it's children
	float fScale = m_QuadTree->MapSize() * LeafWidth;// ((boxBase2.x - boxBase1.x) / 2) * m_QuadTree->LeafWidth();

	ZShadeSandboxMesh::MeshParameters mp;
	//mp.color = XMFLOAT4(0, 0, 0, 1);
	mp.pos = XMFLOAT3((boxBase1.x - halfWidth) * TerrScale, boxBase1.y, (boxBase1.z - halfWidth) * TerrScale);// (boxBase2.x - boxBase1.x) / 2, boxBase1.y, (boxBase3.z - boxBase1.z) / 2);
	mp.rot = XMFLOAT3(0, 0, 0);
	mp.scale = XMFLOAT3(fScale, fScale, fScale);
	mp.rotationAxisX = false;
	mp.rotationAxisY = false;
	mp.rotationAxisZ = false;
	mp.vertexType = ZShadeSandboxMesh::EVertexType::VT_Tex;
	mp.useCustomShader = false;
	//mp.tt = TextureType::DDS;
	//mp.textureName = "stone.dds";
	//mp.texturePath = "";
	mp.in2D = false;
	//mp.bUseTessellation = true;

	root_box_mesh = new ZShadeSandboxMesh::CubeMesh(m_d3d, mp);// boxBase1, boxBase2, boxBase3, boxBase4, 1024, mp);

	CreateNodeBox(m_MeshNodes);
}
//==============================================================================================================================
void QuadTreeMesh::CreateNodeBox(ZShadeSandboxTerrain::QMeshNode*& node)
{
	if (node == 0) return;

	//
	// Create the cube
	//

	// Center the grid in model space
	int LeafWidth = m_QuadTree->LeafWidth();
	int TerrScale = m_QuadTree->TerrainScale();
	float halfWidth = ((float)LeafWidth - 1.0f) / 2.0f;
	float halfLength = ((float)LeafWidth - 1.0f) / 2.0f;

	XMFLOAT3 boxBase1 = node->boundingCoord[0];
	XMFLOAT3 boxBase2 = node->boundingCoord[1];
	XMFLOAT3 boxBase3 = node->boundingCoord[2];
	XMFLOAT3 boxBase4 = node->boundingCoord[3];

	// If this is not a leaf node we need to scale the box fit around all it's children
	float fScale = 20;// ((boxBase2.x - boxBase1.x) / 2);// *m_QuadTree->LeafWidth();

	ZShadeSandboxMesh::MeshParameters mp;
	//mp.color = XMFLOAT4(0, 0, 0, 1);
	mp.pos = XMFLOAT3((boxBase1.x - halfWidth) * TerrScale, boxBase1.y, (boxBase1.z - halfWidth) * TerrScale);// (boxBase2.x - boxBase1.x) / 2, boxBase1.y, (boxBase3.z - boxBase1.z) / 2);
	mp.rot = XMFLOAT3(0, 0, 0);
	mp.scale = XMFLOAT3(fScale, fScale, fScale);
	mp.rotationAxisX = false;
	mp.rotationAxisY = false;
	mp.rotationAxisZ = false;
	mp.vertexType = ZShadeSandboxMesh::EVertexType::VT_Tex;
	mp.useCustomShader = false;
	//mp.tt = TextureType::DDS;
	//mp.textureName = "stone.dds";
	//mp.texturePath = "";
	mp.in2D = false;
	//mp.bUseTessellation = true;

	node->box_mesh = new ZShadeSandboxMesh::CubeMesh(m_d3d, mp);// boxBase1, boxBase2, boxBase3, boxBase4, 512, mp);

	// Generate boxes for the children
	if (node->type != ZShadeSandboxTerrain::EQuadNodeType::LEAF)
	{
		CreateNodeBox(node->children[0]);
		CreateNodeBox(node->children[1]);
		CreateNodeBox(node->children[2]);
		CreateNodeBox(node->children[3]);
	}
}
//==============================================================================================================================
void QuadTreeMesh::ReGenNodeBox()
{
	// Center the grid in model space
	int LeafWidth = m_QuadTree->LeafWidth();
	int TerrScale = m_QuadTree->TerrainScale();
	float halfWidth = ((float)LeafWidth - 1.0f) / 2.0f;
	float halfLength = ((float)LeafWidth - 1.0f) / 2.0f;

	XMFLOAT3 boxBase1 = m_MeshNodes->boundingCoord[0];
	XMFLOAT3 boxBase2 = m_MeshNodes->boundingCoord[1];
	XMFLOAT3 boxBase3 = m_MeshNodes->boundingCoord[2];
	XMFLOAT3 boxBase4 = m_MeshNodes->boundingCoord[3];

	root_box_mesh->Position() = XMFLOAT3((boxBase1.x - halfWidth) * TerrScale, boxBase1.y, (boxBase1.z - halfWidth) * TerrScale);

	ReGenNodeBox(m_MeshNodes);
}
//==============================================================================================================================
void QuadTreeMesh::ReGenNodeBox(ZShadeSandboxTerrain::QMeshNode*& node)
{
	if (node == 0) return;

	// Center the grid in model space
	int LeafWidth = m_QuadTree->LeafWidth();
	int TerrScale = m_QuadTree->TerrainScale();
	float halfWidth = ((float)LeafWidth - 1.0f) / 2.0f;
	float halfLength = ((float)LeafWidth - 1.0f) / 2.0f;

	XMFLOAT3 boxBase1 = node->boundingCoord[0];
	XMFLOAT3 boxBase2 = node->boundingCoord[1];
	XMFLOAT3 boxBase3 = node->boundingCoord[2];
	XMFLOAT3 boxBase4 = node->boundingCoord[3];

	node->box_mesh->Position() = XMFLOAT3((boxBase1.x - halfWidth) * TerrScale, boxBase1.y, (boxBase1.z - halfWidth) * TerrScale);

	// Generate boxes for the children
	if (node->type != ZShadeSandboxTerrain::EQuadNodeType::LEAF)
	{
		ReGenNodeBox(node->children[0]);
		ReGenNodeBox(node->children[1]);
		ReGenNodeBox(node->children[2]);
		ReGenNodeBox(node->children[3]);
	}
}
//==============================================================================================================================
void QuadTreeMesh::CreateNodeSphere()
{
	CreateNodeSphere(m_MeshNodes);
}
//==============================================================================================================================
void QuadTreeMesh::CreateNodeSphere(ZShadeSandboxTerrain::QMeshNode*& node)
{
	if (node == 0) return;

	// Find the center of this node which is the center of the box
	node->sphere.Set(node->center, 512);
	
	// Generate the sphere for the children
	if (node->type != ZShadeSandboxTerrain::EQuadNodeType::LEAF)
	{
		CreateNodeBox(node->children[0]);
		CreateNodeBox(node->children[1]);
		CreateNodeBox(node->children[2]);
		CreateNodeBox(node->children[3]);
	}
}
//==============================================================================================================================
void QuadTreeMesh::ReGenMesh(int newTerrainScale)
{
	m_QuadTree->TerrainScale() = newTerrainScale;

	ReGenNodeBox();

	ReGenMesh(m_MeshNodes);
}
//==============================================================================================================================
void QuadTreeMesh::ReGenMesh(ZShadeSandboxTerrain::QMeshNode*& node)
{
	if (node == 0) return;

	int LeafWidth = m_QuadTree->LeafWidth();
	int TerrScale = m_QuadTree->TerrainScale();
	int MapSize = m_QuadTree->MapSize();

	// Center the grid in model space
	float halfWidth = ((float)LeafWidth - 1.0f) / 2.0f;
	float halfLength = ((float)LeafWidth - 1.0f) / 2.0f;
	
	// Recreate the AABB for the node
	node->boundary.vMin = XMFLOAT3((node->boundingCoord[0].x - halfWidth) * TerrScale, node->boundingCoord[0].y, (node->boundingCoord[0].z - halfWidth) * TerrScale);
	node->boundary.vMax = XMFLOAT3((node->boundingCoord[3].x - halfWidth) * TerrScale, node->boundingCoord[3].y, (node->boundingCoord[3].z - halfWidth) * TerrScale);
	
	m_QuadTree->UpdateHeightValues(m_heightScale, m_terrainZScale);

	switch (GetRenderPrimitive())
	{
		case ZShadeSandboxMesh::ERenderType::e4ControlPointPatchList:
			ReGenMesh4PointPatchTessellation(node);
		break;
		case ZShadeSandboxMesh::ERenderType::e3ControlPointPatchList:
			ReGenMesh3PointPatchTessellation(node);
		break;
		case ZShadeSandboxMesh::ERenderType::eTriangleStrip:
			ReGenMeshNT(node);
		break;
	}
	
	//Progress through the children
	ReGenMesh(node->children[0]);
	ReGenMesh(node->children[1]);
	ReGenMesh(node->children[2]);
	ReGenMesh(node->children[3]);
}
//==============================================================================================================================
void QuadTreeMesh::ReGenMeshNT(ZShadeSandboxTerrain::QMeshNode*& node)
{
	if (node == 0) return;
	if (node->type != ZShadeSandboxTerrain::EQuadNodeType::LEAF) return;

	int index = 0;
	float height = 0;
	int LeafWidth = m_QuadTree->LeafWidth();
	int TerrScale = m_QuadTree->TerrainScale();
	int MapSize = m_QuadTree->MapSize();

	// Center the grid in model space
	float halfWidth = ((float)LeafWidth - 1.0f) / 2.0f;
	float halfLength = ((float)LeafWidth - 1.0f) / 2.0f;
	
	int vert_count = (LeafWidth)* (LeafWidth);
	int index_count = ((LeafWidth)* 2) * (LeafWidth - 1) + (LeafWidth - 2);

#pragma region "Load Vertices"
	node->m_vertex_count = vert_count;

	node->vertices = new ZShadeSandboxMesh::VertexTex[vert_count];

	for (int z = (int)node->boundingCoord[0].z; z <= (int)node->boundingCoord[2].z; z++)
	{
		for (int x = (int)node->boundingCoord[0].x; x <= (int)node->boundingCoord[1].x; x++)
		{
			if (m_QuadTree->UsingHeight())
			{
				if (x < MapSize && z < MapSize)
				{
					if (m_QuadTree->GetMapExt() == EHeightExtension::RAW)
						height = m_QuadTree->SampleHeight(x + (z * MapSize + 1));
					else
						height = m_QuadTree->SampleHeight(x, z);
				}
			}

			node->vertices[index].position = XMFLOAT3(((x - halfWidth) * TerrScale), height, ((z - halfLength) * TerrScale));
			//node->vertices[index].normal = XMFLOAT3(0, 1, 0);
			node->vertices[index].texture = XMFLOAT2(x * (1.0f / (MapSize - 1)), z * (1.0f / (MapSize - 1)));

			index++;
		}
	}
#pragma endregion

	//GenerateBTT(node);

	// Regenerate the vertex buffer for the leaf node
	ReBuildVertexBuffer(node);
}
//==============================================================================================================================
void QuadTreeMesh::ReGenMesh4PointPatchTessellation(ZShadeSandboxTerrain::QMeshNode*& node)
{
	if (node == 0) return;
	if (node->type != ZShadeSandboxTerrain::EQuadNodeType::LEAF) return;

	int index = 0;
	int LeafWidth = m_QuadTree->LeafWidth();
	int TerrScale = m_QuadTree->TerrainScale();
	int MapSize = m_QuadTree->MapSize();

	int vert_count = 4;
	int index_count = 4;

	// Center the grid in model space
	float halfWidth = ((float)LeafWidth - 1.0f) / 2.0f;
	float halfLength = ((float)LeafWidth - 1.0f) / 2.0f;

#pragma region "Load Vertices"
	node->m_vertex_count = vert_count;

	node->vertices = new ZShadeSandboxMesh::VertexTex[vert_count];

	float xPos[4];
	float yPos[4];
	float zPos[4];

	//Looks like I only need four vertices to pass to the tessellation shader
	for (int i = 0; i < 4; i++)
	{
		xPos[i] = (node->boundingCoord[i].x - halfWidth) * TerrScale;
		yPos[i] = node->boundingCoord[i].y;
		zPos[i] = (node->boundingCoord[i].z - halfWidth) * TerrScale;

		node->vertices[i].position = XMFLOAT3(xPos[i], yPos[i], zPos[i]);
		//node->vertices[i].normal = XMFLOAT3(0, 1, 0);
		node->vertices[i].texture = XMFLOAT2(node->boundingCoord[i].x * (1.0f / (MapSize - 1)), node->boundingCoord[i].z * (1.0f / (MapSize - 1)));
	}
#pragma endregion
	
	// Regenerate the vertex buffer for the leaf node
	ReBuildVertexBuffer(node);
}
//==============================================================================================================================
void QuadTreeMesh::ReGenMesh3PointPatchTessellation(ZShadeSandboxTerrain::QMeshNode*& node)
{
	if (node == 0) return;
	if (node->type != ZShadeSandboxTerrain::EQuadNodeType::LEAF) return;

	int index = 0;
	float height = 0;
	int LeafWidth = m_QuadTree->LeafWidth();
	int TerrScale = m_QuadTree->TerrainScale();
	int MapSize = m_QuadTree->MapSize();

	// Center the grid in model space
	float halfWidth = ((float)LeafWidth - 1.0f) / 2.0f;
	float halfLength = ((float)LeafWidth - 1.0f) / 2.0f;

	XMFLOAT3 nodeBounds0 = XMFLOAT3((node->boundingCoord[0].x - halfWidth) * TerrScale, node->boundingCoord[0].y, (node->boundingCoord[0].z - halfWidth) * TerrScale);
	XMFLOAT3 nodeBounds1 = XMFLOAT3((node->boundingCoord[1].x - halfWidth) * TerrScale, node->boundingCoord[1].y, (node->boundingCoord[1].z - halfWidth) * TerrScale);
	XMFLOAT3 nodeBounds2 = XMFLOAT3((node->boundingCoord[2].x - halfWidth) * TerrScale, node->boundingCoord[2].y, (node->boundingCoord[2].z - halfWidth) * TerrScale);
	XMFLOAT3 nodeBounds3 = XMFLOAT3((node->boundingCoord[3].x - halfWidth) * TerrScale, node->boundingCoord[3].y, (node->boundingCoord[3].z - halfWidth) * TerrScale);

	TVertex v1[3], v2[3];

	v1[0].x = nodeBounds0.x;
	v1[0].y = nodeBounds0.y;
	v1[0].z = nodeBounds0.z;

	v1[1].x = nodeBounds3.x;
	v1[1].y = nodeBounds3.y;
	v1[1].z = nodeBounds3.z;

	v1[2].x = nodeBounds1.x;
	v1[2].y = nodeBounds1.y;
	v1[2].z = nodeBounds1.z;

	v2[0].x = nodeBounds0.x;
	v2[0].y = nodeBounds0.y;
	v2[0].z = nodeBounds0.z;

	v2[1].x = nodeBounds2.x;
	v2[1].y = nodeBounds2.y;
	v2[1].z = nodeBounds2.z;

	v2[2].x = nodeBounds3.x;
	v2[2].y = nodeBounds3.y;
	v2[2].z = nodeBounds3.z;

	UINT i1[3], i2[3];

	i1[0] = 0;
	i1[1] = 1;
	i1[2] = 2;

	i2[0] = 0;
	i2[1] = 2;
	i2[2] = 3;

	// Create two triangles based on the bounds for this mesh
	Triangle* tri1 = new Triangle(v1, i1);
	Triangle* tri2 = new Triangle(v2, i2);

	node->m_vertex_count = 6;
	node->vertices = new ZShadeSandboxMesh::VertexTex[node->m_vertex_count];

	node->vertices[0].position = XMFLOAT3(tri1->GetVertex()[0].x, tri1->GetVertex()[0].y, tri1->GetVertex()[0].z);
	node->vertices[0].texture = XMFLOAT2(nodeBounds0.x * (1.0f / (MapSize - 1)), nodeBounds0.z * (1.0f / (MapSize - 1)));

	node->vertices[1].position = XMFLOAT3(tri1->GetVertex()[1].x, tri1->GetVertex()[1].y, tri1->GetVertex()[1].z);
	node->vertices[1].texture = XMFLOAT2(nodeBounds3.x * (1.0f / (MapSize - 1)), nodeBounds3.z * (1.0f / (MapSize - 1)));

	node->vertices[2].position = XMFLOAT3(tri1->GetVertex()[2].x, tri1->GetVertex()[2].y, tri1->GetVertex()[2].z);
	node->vertices[2].texture = XMFLOAT2(nodeBounds1.x * (1.0f / (MapSize - 1)), nodeBounds1.z * (1.0f / (MapSize - 1)));

	node->vertices[3].position = XMFLOAT3(tri2->GetVertex()[0].x, tri2->GetVertex()[0].y, tri2->GetVertex()[0].z);
	node->vertices[3].texture = XMFLOAT2(nodeBounds0.x * (1.0f / (MapSize - 1)), nodeBounds0.z * (1.0f / (MapSize - 1)));

	node->vertices[4].position = XMFLOAT3(tri2->GetVertex()[1].x, tri2->GetVertex()[1].y, tri2->GetVertex()[1].z);
	node->vertices[4].texture = XMFLOAT2(nodeBounds2.x * (1.0f / (MapSize - 1)), nodeBounds2.z * (1.0f / (MapSize - 1)));

	node->vertices[5].position = XMFLOAT3(tri2->GetVertex()[2].x, tri2->GetVertex()[2].y, tri2->GetVertex()[2].z);
	node->vertices[5].texture = XMFLOAT2(nodeBounds3.x * (1.0f / (MapSize - 1)), nodeBounds3.z * (1.0f / (MapSize - 1)));

	// Regenerate the vertex buffer for the leaf node
	ReBuildVertexBuffer(node);
}
//==============================================================================================================================
void QuadTreeMesh::ReBuildVertexBuffer(ZShadeSandboxTerrain::QMeshNode*& node)
{
	D3D11_MAPPED_SUBRESOURCE resource;
	m_d3d->GetDeviceContext()->Map(node->m_vertex_buffer11, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	{
		memcpy(resource.pData, node->vertices, sizeof(ZShadeSandboxMesh::VertexTex) * node->m_vertex_count);
	}
	m_d3d->GetDeviceContext()->Unmap(node->m_vertex_buffer11, 0);
}
//==============================================================================================================================
void QuadTreeMesh::ReBuildIndexBuffer(ZShadeSandboxTerrain::QMeshNode*& node)
{
	D3D11_MAPPED_SUBRESOURCE resource;
	m_d3d->GetDeviceContext()->Map(node->m_index_buffer11, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	{
		memcpy(resource.pData, node->indices, sizeof(unsigned long) * node->m_index_count);
	}
	m_d3d->GetDeviceContext()->Unmap(node->m_index_buffer11, 0);
}
//==============================================================================================================================
void QuadTreeMesh::ClearMesh(bool removeIndices)
{
	ClearMesh(m_MeshNodes, removeIndices);
}
//==============================================================================================================================
void QuadTreeMesh::ClearMesh(ZShadeSandboxTerrain::QMeshNode*& node, bool removeIndices)
{
	if (node == 0) return;

	if (node->type == ZShadeSandboxTerrain::EQuadNodeType::LEAF)
	{
		delete[] node->vertices;

		if (removeIndices)
			delete[] node->indices;
	}
	else
	{
		//Progress through the children
		ClearMesh(node->children[0], removeIndices);
		ClearMesh(node->children[1], removeIndices);
		ClearMesh(node->children[2], removeIndices);
		ClearMesh(node->children[3], removeIndices);
	}
}
//==============================================================================================================================
void QuadTreeMesh::GenerateMesh()
{
	GenerateMesh(m_MeshNodes);
}
//==============================================================================================================================
void QuadTreeMesh::GenerateMesh(ZShadeSandboxTerrain::QMeshNode*& node)
{
	if (node == 0) return;
	
	switch (GetRenderPrimitive())
	{
		case ZShadeSandboxMesh::ERenderType::e4ControlPointPatchList:
			GenerateMesh4PointPatchTessellation(node);
		break;
		case ZShadeSandboxMesh::ERenderType::e3ControlPointPatchList:
			GenerateMesh3PointPatchTessellation(node);
		break;
		case ZShadeSandboxMesh::ERenderType::eTriangleStrip:
			GenerateMeshNT(node);
		break;
	}
	
	//Progress through the children
	GenerateMesh(node->children[0]);
	GenerateMesh(node->children[1]);
	GenerateMesh(node->children[2]);
	GenerateMesh(node->children[3]);
}
//==============================================================================================================================
void QuadTreeMesh::GenerateMeshNT(ZShadeSandboxTerrain::QMeshNode*& node)
{
	if (node == 0) return;
	if (node->type != ZShadeSandboxTerrain::EQuadNodeType::LEAF) return;

	int index = 0;
	float height = 0;
	int LeafWidth = m_QuadTree->LeafWidth();
	int TerrScale = m_QuadTree->TerrainScale();
	int MapSize = m_QuadTree->MapSize();

	// Center the grid in model space
	float halfWidth = ((float)LeafWidth - 1.0f) / 2.0f;
	float halfLength = ((float)LeafWidth - 1.0f) / 2.0f;

	int vert_count = (LeafWidth) * (LeafWidth);
	int index_count = ((LeafWidth) * 2) * (LeafWidth - 1) + (LeafWidth - 2);
	
#pragma region "Load Vertices"
	node->m_vertex_count = vert_count;
	node->m_triangle_count = node->m_vertex_count / 3;
	node->vertices = new ZShadeSandboxMesh::VertexTex[vert_count];

	for (int z = (int)node->boundingCoord[0].z; z <= (int)node->boundingCoord[2].z; z++)
	{
		for (int x = (int)node->boundingCoord[0].x; x <= (int)node->boundingCoord[1].x; x++)
		{
			if (m_QuadTree->UsingHeight())
			{
				if (x < MapSize && z < MapSize)
				{
					if (m_QuadTree->GetMapExt() == EHeightExtension::RAW)
						height = m_QuadTree->SampleHeight(x + (z * MapSize + 1));
					else
						height = m_QuadTree->SampleHeight(x, z);
				}
			}

			node->vertices[index].position = XMFLOAT3(((x - halfWidth) * TerrScale), height, ((z - halfLength) * TerrScale));
			//node->vertices[index].normal = XMFLOAT3(0, 1, 0);
			node->vertices[index].texture = XMFLOAT2(x * (1.0f / (MapSize - 1)), z * (1.0f / (MapSize - 1)));

			index++;
		}
	}
#pragma endregion
#pragma region "Load Indices"
	index = 0;
	
	node->m_index_count = index_count;
	node->indices = new unsigned long[index_count];

	for (int z = 0; z < LeafWidth - 1; z++)
	{
		//Even rows move left to right, odd rows right to left
		if (z % 2 == 0)
		{
			//Even row
			int x;
			for (x = 0; x < LeafWidth; x++)
			{
				node->indices[index++] = x + (z * LeafWidth);
				node->indices[index++] = x + (z * LeafWidth) + LeafWidth;
			}
			//Insert degenerate vertex if this isn't the last row
			if (z != LeafWidth - 2)
			{
				node->indices[index++] = --x + (z * LeafWidth);
			}
		}
		else
		{
			//Odd row
			int x;
			for (x = LeafWidth - 1; x >= 0; x--)
			{
				node->indices[index++] = x + (z * LeafWidth);
				node->indices[index++] = x + (z * LeafWidth) + LeafWidth;
			}
			//Insert degenerate vertex if this isn't the last row
			if (z != LeafWidth - 2)
			{
				node->indices[index++] = ++x + (z * LeafWidth);
			}
		}
	}
#pragma endregion

	// Build the vertex and index buffers for the leaf node
	BuildVertexBuffer( node );
	BuildIndexBuffer( node );
}
//==============================================================================================================================
void QuadTreeMesh::GenerateMesh4PointPatchTessellation(ZShadeSandboxTerrain::QMeshNode*& node)
{
	if (node == 0) return;
	if (node->type != ZShadeSandboxTerrain::EQuadNodeType::LEAF) return;

	int index = 0;
	int LeafWidth = m_QuadTree->LeafWidth();
	int TerrScale = m_QuadTree->TerrainScale();
	int MapSize = m_QuadTree->MapSize();

	int vert_count = 4;
	int index_count = 4;

	// Center the grid in model space
	float halfWidth = ((float)LeafWidth - 1.0f) / 2.0f;
	float halfLength = ((float)LeafWidth - 1.0f) / 2.0f;

#pragma region "Load Vertices"
	node->m_vertex_count = vert_count;
	node->m_triangle_count = node->m_vertex_count / 3;
	node->vertices = new ZShadeSandboxMesh::VertexTex[vert_count];
	
	float xPos[4];
	float yPos[4];
	float zPos[4];

	//Looks like I only need four vertices to pass to the tessellation shader
	for (int i = 0; i < 4; i++)
	{
		xPos[i] = (node->boundingCoord[i].x - halfWidth) * TerrScale;
		yPos[i] = node->boundingCoord[i].y;
		zPos[i] = (node->boundingCoord[i].z - halfWidth) * TerrScale;

		node->vertices[i].position = XMFLOAT3(xPos[i], yPos[i], zPos[i]);
		node->vertices[i].texture = XMFLOAT2(node->boundingCoord[i].x * (1.0f / (MapSize - 1)), node->boundingCoord[i].z * (1.0f / (MapSize - 1)));
	}
#pragma endregion
#pragma region "Load Indices"
	node->m_index_count = index_count;
	node->indices = new unsigned long[index_count];
	node->indices[0] = 1;
	node->indices[1] = 0;
	node->indices[2] = 3;
	node->indices[3] = 2;
#pragma endregion

	// Build the vertex and index buffers for the leaf node
	BuildVertexBuffer( node );
	BuildIndexBuffer( node );
}
//==============================================================================================================================
void QuadTreeMesh::GenerateMesh3PointPatchTessellation(ZShadeSandboxTerrain::QMeshNode*& node)
{
	if (node == 0) return;
	if (node->type != ZShadeSandboxTerrain::EQuadNodeType::LEAF) return;

	int index = 0;
	float height = 0;
	int LeafWidth = m_QuadTree->LeafWidth();
	int TerrScale = m_QuadTree->TerrainScale();
	int MapSize = m_QuadTree->MapSize();

	// Center the grid in model space
	float halfWidth = ((float)LeafWidth - 1.0f) / 2.0f;
	float halfLength = ((float)LeafWidth - 1.0f) / 2.0f;

	XMFLOAT3 nodeBounds0 = XMFLOAT3((node->boundingCoord[0].x - halfWidth) * TerrScale, node->boundingCoord[0].y, (node->boundingCoord[0].z - halfWidth) * TerrScale);
	XMFLOAT3 nodeBounds1 = XMFLOAT3((node->boundingCoord[1].x - halfWidth) * TerrScale, node->boundingCoord[1].y, (node->boundingCoord[1].z - halfWidth) * TerrScale);
	XMFLOAT3 nodeBounds2 = XMFLOAT3((node->boundingCoord[2].x - halfWidth) * TerrScale, node->boundingCoord[2].y, (node->boundingCoord[2].z - halfWidth) * TerrScale);
	XMFLOAT3 nodeBounds3 = XMFLOAT3((node->boundingCoord[3].x - halfWidth) * TerrScale, node->boundingCoord[3].y, (node->boundingCoord[3].z - halfWidth) * TerrScale);

	TVertex v1[3], v2[3];

	v1[0].x = nodeBounds0.x;
	v1[0].y = nodeBounds0.y;
	v1[0].z = nodeBounds0.z;

	v1[1].x = nodeBounds3.x;
	v1[1].y = nodeBounds3.y;
	v1[1].z = nodeBounds3.z;

	v1[2].x = nodeBounds1.x;
	v1[2].y = nodeBounds1.y;
	v1[2].z = nodeBounds1.z;

	v2[0].x = nodeBounds0.x;
	v2[0].y = nodeBounds0.y;
	v2[0].z = nodeBounds0.z;

	v2[1].x = nodeBounds2.x;
	v2[1].y = nodeBounds2.y;
	v2[1].z = nodeBounds2.z;

	v2[2].x = nodeBounds3.x;
	v2[2].y = nodeBounds3.y;
	v2[2].z = nodeBounds3.z;

	UINT i1[3], i2[3];

	i1[0] = 0;
	i1[1] = 1;
	i1[2] = 2;

	i2[0] = 3;
	i2[1] = 4;
	i2[2] = 5;

	// Create two triangles based on the bounds for this mesh
	Triangle* tri1 = new Triangle(v1, i1);
	Triangle* tri2 = new Triangle(v2, i2);

	// There are two triangles in the vertex list
	node->m_triangle_count = 2;

	node->m_vertex_count = 6;
	node->vertices = new ZShadeSandboxMesh::VertexTex[node->m_vertex_count];

	node->vertices[0].position = XMFLOAT3(tri1->GetVertex()[0].x, tri1->GetVertex()[0].y, tri1->GetVertex()[0].z);
	node->vertices[0].texture = XMFLOAT2(nodeBounds0.x * (1.0f / (MapSize - 1)), nodeBounds0.z * (1.0f / (MapSize - 1)));

	node->vertices[1].position = XMFLOAT3(tri1->GetVertex()[1].x, tri1->GetVertex()[1].y, tri1->GetVertex()[1].z);
	node->vertices[1].texture = XMFLOAT2(nodeBounds3.x * (1.0f / (MapSize - 1)), nodeBounds3.z * (1.0f / (MapSize - 1)));

	node->vertices[2].position = XMFLOAT3(tri1->GetVertex()[2].x, tri1->GetVertex()[2].y, tri1->GetVertex()[2].z);
	node->vertices[2].texture = XMFLOAT2(nodeBounds1.x * (1.0f / (MapSize - 1)), nodeBounds1.z * (1.0f / (MapSize - 1)));

	node->vertices[3].position = XMFLOAT3(tri2->GetVertex()[0].x, tri2->GetVertex()[0].y, tri2->GetVertex()[0].z);
	node->vertices[3].texture = XMFLOAT2(nodeBounds0.x * (1.0f / (MapSize - 1)), nodeBounds0.z * (1.0f / (MapSize - 1)));

	node->vertices[4].position = XMFLOAT3(tri2->GetVertex()[1].x, tri2->GetVertex()[1].y, tri2->GetVertex()[1].z);
	node->vertices[4].texture = XMFLOAT2(nodeBounds2.x * (1.0f / (MapSize - 1)), nodeBounds2.z * (1.0f / (MapSize - 1)));

	node->vertices[5].position = XMFLOAT3(tri2->GetVertex()[2].x, tri2->GetVertex()[2].y, tri2->GetVertex()[2].z);
	node->vertices[5].texture = XMFLOAT2(nodeBounds3.x * (1.0f / (MapSize - 1)), nodeBounds3.z * (1.0f / (MapSize - 1)));

	node->m_index_count = 6;
	node->indices = new unsigned long[node->m_index_count];

	node->indices[0] = tri1->Indices()[0];
	node->indices[1] = tri1->Indices()[1];
	node->indices[2] = tri1->Indices()[2];

	node->indices[3] = tri2->Indices()[0];
	node->indices[4] = tri2->Indices()[1];
	node->indices[5] = tri2->Indices()[2];

	// Build the vertex and index buffers for the leaf node
	BuildVertexBuffer(node);
	BuildIndexBuffer(node);
}
//==============================================================================================================================
void QuadTreeMesh::BuildVertexBuffer(ZShadeSandboxTerrain::QMeshNode*& node)
{
	if (m_d3d == 0) return;
	if (m_EngineOptions == 0) return;

	HRESULT result;
	D3D11_BUFFER_DESC vertexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData;

	//Setup the description of the static vertex buffer
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(ZShadeSandboxMesh::VertexTex) * node->m_vertex_count;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	//Give the subresource a pointer to the vertex data
	vertexData.pSysMem = node->vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//Create the vertex buffer
	result = m_d3d->GetDevice11()->CreateBuffer(&vertexBufferDesc, &vertexData, &node->m_vertex_buffer11);
	if (FAILED(result)) return;
}
//==============================================================================================================================
void QuadTreeMesh::BuildIndexBuffer(ZShadeSandboxTerrain::QMeshNode*& node)
{
	if (m_d3d == 0) return;
	if (m_EngineOptions == 0) return;

	HRESULT result;
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA indexData;

	//Setup the description of the static index buffer
	indexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * node->m_index_count;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	//Give the subresource a pointer to the index data
	indexData.pSysMem = node->indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	//Create the index buffer
	result = m_d3d->GetDevice11()->CreateBuffer(&indexBufferDesc, &indexData, &node->m_index_buffer11);
	if (FAILED(result)) return;
}
//==============================================================================================================================
bool QuadTreeMesh::GetHeightAtPosition(XMFLOAT3 position, float& height)
{
	float meshMinX, meshMaxX, meshMinZ, meshMaxZ;
	
	meshMinX = m_MeshNodes->boundary.vMin.x;
	meshMaxX = m_MeshNodes->boundary.vMax.x;

	meshMinZ = m_MeshNodes->boundary.vMin.z;
	meshMaxZ = m_MeshNodes->boundary.vMax.z;
	
	// Make sure the coordinates are actually over a polygon.
	if((position.x < meshMinX) || (position.x > meshMaxX) || (position.z < meshMinZ) || (position.z > meshMaxZ))
	{
		return false;
	}
	
	// Find the node which contains the polygon for this position.
	return GetHeightAtPosition(m_MeshNodes, position, height);
}
//==============================================================================================================================
bool QuadTreeMesh::GetHeightAtPosition(ZShadeSandboxTerrain::QMeshNode* node, XMFLOAT3 position, float& height)
{
	if (node == 0) return false;

	if (node->type == ZShadeSandboxTerrain::EQuadNodeType::LEAF)
	{
		float meshMinX, meshMaxX, meshMinZ, meshMaxZ;

		meshMinX = node->boundary.vMin.x;
		meshMaxX = node->boundary.vMax.x;

		meshMinZ = node->boundary.vMin.z;
		meshMaxZ = node->boundary.vMax.z;

		// Make sure the coordinates are actually over a polygon.
		if ((position.x < meshMinX) || (position.x > meshMaxX) || (position.z < meshMinZ) || (position.z > meshMaxZ))
		{
			return false;
		}

		// Find the node which contains the polygon for this position.
		return FindNodeHeight(node, position, height);
	}
	
	//
	// Search the children to find the height
	//
	
	bool foundHeight = false;
	
	foundHeight = GetHeightAtPosition(node->children[0], position, height);

	if (!foundHeight)
		foundHeight = GetHeightAtPosition(node->children[1], position, height);
	
	if (!foundHeight)
		foundHeight = GetHeightAtPosition(node->children[2], position, height);
	
	if (!foundHeight)
		foundHeight = GetHeightAtPosition(node->children[3], position, height);

	return foundHeight;
}
//==============================================================================================================================
bool QuadTreeMesh::FindNodeHeight(ZShadeSandboxTerrain::QMeshNode* node, XMFLOAT3 position, float& height)
{
	float xMin, xMax, zMin, zMax;
	int count, index;
	float vertex1[3], vertex2[3], vertex3[3];
	bool foundHeight = false;
	
	// Calculate the dimensions of this node.
	xMin = node->boundary.vMin.x;
	xMax = node->boundary.vMax.x;

	zMin = node->boundary.vMin.z;
	zMax = node->boundary.vMax.z;

	// See if the x and z coordinate are in this node, if not then stop traversing this part of the tree.
	if ((position.x < xMin) || (position.x > xMax) || (position.z < zMin) || (position.z > zMax))
	{
		return false;
	}
	
	// If the coordinates are in this node then check first to see if children nodes exist.
	count = 0;
	
	for(int i = 0; i < 4; i++)
	{
		if(node->children[i] != 0)
		{
			count++;
			FindNodeHeight(node->children[i], position, height);
		}
	}
	
	// If there were children nodes then return since the polygon will be in one of the children.
	if(count > 0)
	{
		return false;
	}
	
	// If there were no children then the polygon must be in this node.  Check all the polygons in this node to find 
	// the height of which one the polygon we are looking for.
	
	// If the height is not found interpolate between node vertices the camera is on and the camera position
	if (node->boundary.ContainsPoint3DOmitY(XMFLOAT3(position.x, 0, position.z)))
	{
		// Interpolate the real height values around the camera and smooth the camera movement

		int LeafWidth = m_QuadTree->LeafWidth();
		int TerrScale = m_QuadTree->TerrainScale();

		// Center the grid in model space
		float halfWidth = ((float)LeafWidth - 1.0f) / 2.0f;
		float halfLength = ((float)LeafWidth - 1.0f) / 2.0f;

		float realHeight = position.y;
		float biggest = 0.0f;
		float heightAverage = 0.0f;
		int size = 0;
		float h;
		
		for (int z = (int)node->boundingCoord[0].z; z <= (int)node->boundingCoord[2].z; z++)
		{
			for (int x = (int)node->boundingCoord[0].x; x <= (int)node->boundingCoord[1].x; x++)
			{
				if (m_QuadTree->UsingHeight())
				{
					h = m_QuadTree->GetHeight(x, z);

					h = ((h * m_heightScale * 100.0f) / 255.0) / ((m_terrainZScale * 2) + 1);

					if (h > biggest)
					{
						biggest = h;
					}

					heightAverage += h;

					size++;
				}
			}
		}

		heightAverage /= (size);

		float t = 0.05f;

		realHeight = ZShadeSandboxMath::ZMath::lerp(position.y, biggest, t);
		realHeight = ZShadeSandboxMath::ZMath::lerp(realHeight, heightAverage, t);

		if (realHeight < biggest)
		{
			realHeight = ZShadeSandboxMath::ZMath::lerp(biggest, realHeight, t);
		}

		height = realHeight;

		foundHeight = true;
	}
	else
	{
		foundHeight = false;
	}

	m_cameraCollided = foundHeight;

	return foundHeight;
}
//==============================================================================================================================
void QuadTreeMesh::Intersects(ZShadeSandboxMath::Ray ray, bool& hit, XMFLOAT3& hitPoint)
{
	Intersects(m_MeshNodes, ray, hit, hitPoint);
	
	// If an intersection was found get the height at the intersection point
	//if (hit)
	//{
		//int LeafWidth = m_QuadTree->LeafWidth();
		//int TerrScale = m_QuadTree->TerrainScale();

		//// Center the grid in model space
		//float halfWidth = ((float)LeafWidth - 1.0f) / 2.0f;
		//float halfLength = ((float)LeafWidth - 1.0f) / 2.0f;
		//
		//hitPoint.x = (hitPoint.x + halfWidth) / TerrScale;
		//hitPoint.z = (hitPoint.z + halfWidth) / TerrScale;

	//	hitPoint.y = m_QuadTree->GetHeight(hitPoint.x, hitPoint.z);
	//}
}
//==============================================================================================================================
void QuadTreeMesh::Intersects(ZShadeSandboxTerrain::QMeshNode* node, ZShadeSandboxMath::Ray ray, bool& hit, XMFLOAT3& hitPoint)
{
	hitPoint = XMFLOAT3(FLT_MAX, FLT_MAX, FLT_MAX);
	
	if (node == 0) return;
	
	// This is our terminating condition
	if (node->type == ZShadeSandboxTerrain::EQuadNodeType::LEAF)
	{
		float d;

		// Check if the ray intersects this leaf node's AABB
		//if (!node->boundary.Intersects(ray, d))
		if (!ShapeContact::RayIntersectsAABB(ray, node->boundary, d))
		{
			// No Intersection
			hit = false;

			return;
		}

		// Return the center point of the leaf's bounding box
		hitPoint.x = (node->boundary.vMin.x + node->boundary.vMax.x) / 2;
		hitPoint.y = (node->boundary.vMin.y + node->boundary.vMax.y) / 2;
		hitPoint.z = (node->boundary.vMin.z + node->boundary.vMax.z) / 2;

		int LeafWidth = m_QuadTree->LeafWidth();
		int TerrScale = m_QuadTree->TerrainScale();

		// Center the grid in model space
		float halfWidth = ((float)LeafWidth - 1.0f) / 2.0f;
		float halfLength = ((float)LeafWidth - 1.0f) / 2.0f;

		// Locate the contact point on this node
		float h = 0;
		for (int z = (int)node->boundingCoord[0].z; z <= (int)node->boundingCoord[2].z; z++)
		{
			for (int x = (int)node->boundingCoord[0].x; x <= (int)node->boundingCoord[1].x; x++)
			{
				if (m_QuadTree->UsingHeight())
				{
					h = m_QuadTree->GetHeight(x, z);
					h = ((h * m_heightScale * 100.0f) / 255.0) / ((m_terrainZScale * 2) + 1);
				}

				XMFLOAT3 point((x - halfWidth) * TerrScale, h, (z - halfWidth) * TerrScale);
				ZShadeSandboxMath::SpherePrimitive sphere(point, 2);

				if (ShapeContact::RayIntersectsSphere(ray, sphere, d))
				{
					hitPoint = point;
					hit = true;
					return;
				}
			}
		}

		hit = true;
		return;
	}

	// If the node has children intersect each child
	std::map<float, ZShadeSandboxTerrain::QMeshNode*> pq;
	for (int i = 0; i < 4; i++)
	{
		float cd;

		ZShadeSandboxTerrain::QMeshNode* n = node->children[i];
		
		if (n != NULL)
		{
			//if (ray.Intersects(n->boundary, cd))
			if (ShapeContact::RayIntersectsAABB(ray, n->boundary, cd))
			{
				while (pq.find(cd) != pq.end())
				{
					// Perturb things slightly so that we don't have duplicate keys
					cd += ZShadeSandboxMath::ZMath::RandF<float>(-0.001f, 0.001f);
				}

				pq.insert(make_pair(cd, n));
			}
		}
	}
	
	// There were no child intersections
	if (pq.size() <= 0)
	{
		hit = false;
		
		return;
	}

	// check the child intersections for the nearest intersection
	bool intersect = false;

	// setup a very-far away intersection point to compare against
	XMFLOAT3 bestHit;

	bestHit.x = ray.position.x + ray.direction.x * 1000;
	bestHit.y = ray.position.y + ray.direction.y * 1000;
	bestHit.z = ray.position.z + ray.direction.z * 1000;

	std::map<float, ZShadeSandboxTerrain::QMeshNode*>::iterator pq_iter = pq.begin();

	for (; pq_iter != pq.end(); pq_iter++)
	{
		ZShadeSandboxTerrain::QMeshNode* n = (*pq_iter).second;

		XMFLOAT3 thisHit;

		// Intersect each child recursively
		bool wasHit = false;
		
		Intersects(n, ray, wasHit, thisHit);

		if (!wasHit)
		{
			// No intersection, continue and intersect other children
			continue;
		}
		
		// Make sure that the intersection point is in front of the ray's world-space origin
		ZShadeSandboxMath::XMMath3 thisHitXM(thisHit);
		ZShadeSandboxMath::XMMath3 bestHitXM(bestHit);
		ZShadeSandboxMath::XMMath3 rayPosXM(ray.position);
		ZShadeSandboxMath::XMMath3 rayDirXM(ray.direction);
		
		ZShadeSandboxMath::XMMath3 dotPos(thisHitXM - rayPosXM);
		dotPos.NormalizeMe();

		if (!(dotPos.Dot(ray.direction) > 0.9f))
			continue;

		// Check that the intersection is closer than the nearest intersection found thus far
		if (!((rayPosXM - thisHitXM).LengthSquared() < (rayPosXM - bestHitXM).LengthSquared()))
		{
			continue;
		}

		// If we found a closer intersection store the new closest intersection
		bestHit = thisHit;

		intersect = true;
	}

	// Best hit now contains the closest intersection found, or the distant sentinel value
	hitPoint = bestHit;

	hit = intersect;
}
//==============================================================================================================================
void QuadTreeMesh::AddMaterialColors(XMFLOAT4 ambient, XMFLOAT4 diffuse, XMFLOAT4 specular)
{
	AddDiffuseColor(diffuse);
	AddAmbientColor(ambient);
	AddSpecularColor(specular);
}
//==============================================================================================================================
void QuadTreeMesh::AddDiffuseColor(XMFLOAT4 diffuse)
{
	mMaterial->DiffuseColor() = diffuse;
}
//==============================================================================================================================
void QuadTreeMesh::AddAmbientColor(XMFLOAT4 ambient)
{
	mMaterial->AmbientColor() = ambient;
}
//==============================================================================================================================
void QuadTreeMesh::AddSpecularColor(XMFLOAT4 specular)
{
	mMaterial->SpecularColor() = specular;
}
//==============================================================================================================================
void QuadTreeMesh::AddSpecularPower(float power)
{
	mMaterial->SpecularPower() = power;
}
//==============================================================================================================================
void QuadTreeMesh::AddSpecularIntensity(float intensity)
{
	mMaterial->SpecularIntensity() = intensity;
}
//==============================================================================================================================
void QuadTreeMesh::AddMaterialTextures(vector<string> textureNames, string basePath, string blendMapFilename, string normalMapFilename, string detailMapFilename)
{
	AddDiffuseLayerMap(textureNames);
	AddBlendMap(basePath, blendMapFilename);
	AddNormalMap(basePath, normalMapFilename);
	AddDetailMap(basePath, detailMapFilename);
}
//================================================================================================================
void QuadTreeMesh::AddDiffuseLayerMap(vector<string> textureNames)
{
	mMaterial->CreateTexture2DArray(textureNames);
}
//================================================================================================================
void QuadTreeMesh::AddBlendMap(string basePath, string blendMapFilename)
{
	mMaterial->AddBlendMapTexture(basePath, blendMapFilename);
}
//================================================================================================================
void QuadTreeMesh::AddNormalMap(string basePath, string normalMapFilename)
{
	mMaterial->AddNormalMapTexture(basePath, normalMapFilename);
}
//================================================================================================================
void QuadTreeMesh::AddDetailMap(string basePath, string detailMapFilename)
{
	mMaterial->AddDetailMapTexture(basePath, detailMapFilename);
}
//================================================================================================================