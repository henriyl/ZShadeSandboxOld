//=====================================================================================
//HeightmapTester.h
//=====================================================================================
#ifndef __HEIGHTMAPTESTER_H
#define __HEIGHTMAPTESTER_H
//-------------------------------------------------------------------------------------
#include "Heightmap.h"
//-------------------------------------------------------------------------------------
namespace ZShadeSandboxTerrain {
class HeightmapTester
{
public:
	HeightmapTester(string heightmap, D3D* d3d);
	HeightmapTester(const HeightmapTester& o);
	~HeightmapTester();

	bool InitializeBuffers();
	void ShutdownBuffers();
	void RenderBuffers();

	int GetIndexCount() { return m_index_count; }

private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};

	D3D* m_d3d;

	ZShadeSandboxTerrain::Heightmap* m_heightmap;

	int m_vertex_count, m_index_count, NumFaces;

	ID3D11Buffer* m_vertex_buffer;
	ID3D11Buffer* m_index_buffer;
};
}
//-------------------------------------------------------------------------------------
#endif//__HEIGHTMAPTESTER_H