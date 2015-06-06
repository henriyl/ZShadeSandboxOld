#include "HeightmapTester.h"
using ZShadeSandboxTerrain::HeightmapTester;
//-------------------------------------------------------------------------------------
HeightmapTester::HeightmapTester(string heightmap, D3D* d3d) :
	m_d3d(d3d)
{
	m_heightmap = new ZShadeSandboxTerrain::Heightmap(heightmap, ZShadeSandboxTerrain::EHeightExtension::BMP, 0, 0, 0.5f, 50.0f, 64);
	//m_heightmap->LoadElevation(heightmap);
	//m_heightmap->Normalize();

	//Create the terrain
	InitializeBuffers();
}
//-------------------------------------------------------------------------------------
HeightmapTester::HeightmapTester(const HeightmapTester& o)
{

}
//-------------------------------------------------------------------------------------
HeightmapTester::~HeightmapTester()
{
	ShutdownBuffers();
}
//-------------------------------------------------------------------------------------
bool HeightmapTester::InitializeBuffers()
{
	VertexType* vertices;
	unsigned long* indices;
	int index, i, j;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int index1, index2, index3, index4;

	//Calculate the number of vertices in the heightmap mesh
	m_vertex_count = (m_heightmap->Width()) * (m_heightmap->Height());

	m_index_count = (m_heightmap->Width() * 2) * (m_heightmap->Width() - 1) + (m_heightmap->Width() - 2);

	NumFaces  = (m_heightmap->Width()-1)*(m_heightmap->Height()-1)*2;

	// Create the vertex array.
	vertices = new VertexType[m_vertex_count];
	
	if(!vertices) return false;

	// Create the index array.
	indices = new unsigned long[m_index_count];

	if(!indices) return false;

	float height;

	index = 0;

	// Load the vertex and index array with the terrain data.
	int iVertexBuffOffset = 0;
    int terrScale = 1;//Default is 8

	// Center the grid in model space
	float halfWidth = ((float)m_heightmap->Width() - 1.0f) / 2.0f;
	float halfLength = ((float)m_heightmap->Height() - 1.0f) / 2.0f;

	index = 0;

	for (int z = 0; z < m_heightmap->Height(); z++)
	{
		for (int x = 0; x < m_heightmap->Width(); x++)
		{
			index = x + (z * m_heightmap->Width());

			height = m_heightmap->SampleHeight(x, z);
			
			vertices[index].position = XMFLOAT3((x - halfWidth) * terrScale, height, (z - halfLength) * terrScale);
			vertices[index].color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}

	index = 0;

	for (int z = 0; z < m_heightmap->Height() - 1; z++)
	{
		//Even rows move left to right, odd rows right to left
		if (z % 2 == 0)
		{
			//Even row
			int x;
			for (x = 0; x < m_heightmap->Width(); x++)
			{
				indices[index++] = x + (z * m_heightmap->Width());
				indices[index++] = x + (z * m_heightmap->Width()) + m_heightmap->Width();
			}

			//Insert degenerate vertex if this isn't the last row
			if (z != m_heightmap->Height() - 2)
			{
				indices[index++] = --x + (z * m_heightmap->Width());
			}
		}
		else
		{
			//Odd row
			int x;
			for (x = m_heightmap->Width() - 1; x >= 0; x--)
			{
				indices[index++] = x + (z * m_heightmap->Width());
				indices[index++] = x + (z * m_heightmap->Width()) + m_heightmap->Width();
			}

			//Insert degenerate vertex if this isn't the last row
			if (z != m_heightmap->Height() - 2)
			{
				indices[index++] = ++x + (z * m_heightmap->Width());
			}
		}
	}

	//Setup the description of the static vertex buffer
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertex_count;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	//Give the subresource a pointer to the vertex data
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	//Create the vertex buffer
	result = m_d3d->GetDevice11()->CreateBuffer(&vertexBufferDesc, &vertexData, & m_vertex_buffer);

	if (FAILED(result)) return false;

	//Setup the description of the static index buffer
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_index_count;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	//Give the subresource a pointer to the index data
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	//Create the index buffer
	result = m_d3d->GetDevice11()->CreateBuffer(&indexBufferDesc, &indexData, & m_index_buffer);

	if (FAILED(result)) return false;

	//Release the arrays new that the buffers have been created and loaded
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}
//-------------------------------------------------------------------------------------
void HeightmapTester::ShutdownBuffers()
{
	//Release the index buffer
	if (m_index_buffer)
	{
		m_index_buffer->Release();
		m_index_buffer = 0;
	}

	//Release the vertex buffer
	if (m_vertex_buffer)
	{
		m_vertex_buffer->Release();
		m_vertex_buffer = 0;
	}
}
//-------------------------------------------------------------------------------------
void HeightmapTester::RenderBuffers()
{
	unsigned int stride;
	unsigned int offset;

	//Set the vertex buffer stride and offset
	stride = sizeof(VertexType);
	offset = 0;

	//Set the vertex buffer to active in the input assembler so it can be rendered
	m_d3d->GetDeviceContext()->IASetVertexBuffers(0, 1, &m_vertex_buffer, &stride, &offset);

	//Set the index buffer to active in the input assembler so it can be rendered
	m_d3d->GetDeviceContext()->IASetIndexBuffer(m_index_buffer, DXGI_FORMAT_R32_UINT, 0);

	//Set the type of primitive that should be rendered from this vertex buffer
	m_d3d->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
}
//-------------------------------------------------------------------------------------
