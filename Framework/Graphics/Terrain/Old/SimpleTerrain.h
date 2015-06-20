//=====================================================================================
//SimpleTerrain.h
//=====================================================================================
#ifndef __SIMPLETERRAIN_H
#define __SIMPLETERRAIN_H
//-------------------------------------------------------------------------------------
#include "Heightmap.h"
//#include "ColorShader.h"
//#include "GerstnerWaveShader.h"
#include "Camera.h"
#include <vector>
#include <algorithm>
using namespace std;
//-------------------------------------------------------------------------------------
#define M_HILLS 50.0
#define M_RADII 1000.0
#define M_SIGMA2X 50.0
#define M_SIGMA2Y 50.0
#define M_A 4.0
//-------------------------------------------------------------------------------------
struct Constants
{
    void initializeConstants();

    int hills;
    float sigma2x;
    float sigma2y;
    float a;

    float clustersPerPatch;

    float perturbationWaveAmplitude;
    float ambientWaveAmplitude;
};
//-------------------------------------------------------------------------------------
class SimpleTerrain
{
public:
	SimpleTerrain(D3D* d3d, int size, bool addBump = false);
	SimpleTerrain(const SimpleTerrain& o);
	~SimpleTerrain();

	bool moreHills();
    bool lessHills();
	float** generateMap();
    void addHill(int x, int z);

	bool InitializeBuffers();
	void ShutdownBuffers();
	void RenderBuffers();
	void ReGenBuffers();

	int GetIndexCount() { return m_index_count; }

	void BuildHeightmapSRV();

	vector<float> GetHeightmap();

	ID3D11ShaderResourceView* GetHeightmapSRV() { return mHeightMapSRV; }

private:
	struct VertexType
	{
		XMFLOAT3 position;
		//XMFLOAT4 color;
		XMFLOAT2 tex_uv;
	};

	XMMATRIX mWorld;

	VertexType* vertices;

	Constants constants;

	float** m_map;

	D3D* m_d3d;

	int map_size;

	ID3D11ShaderResourceView* mHeightMapSRV;

	int m_vertex_count, NumFaces;
	unsigned long m_index_count;

	ID3D11Buffer* m_vertex_buffer;
	ID3D11Buffer* m_index_buffer;
};
//-------------------------------------------------------------------------------------
#endif//__SIMPLETERRAIN_H