//#include "stdafx.h"
#include "SimpleTerrain.h"
#include <time.h>
//-------------------------------------------------------------------------------------
void Constants::initializeConstants()
{
    hills = M_HILLS;
    sigma2x = M_SIGMA2X;
    sigma2y = M_SIGMA2Y;
    a = M_A;

    perturbationWaveAmplitude = 5.0;
    ambientWaveAmplitude = .4;
}
//-------------------------------------------------------------------------------------
SimpleTerrain::SimpleTerrain(D3D* d3d, int size, bool addBump) :
	m_d3d(d3d)
{
	mWorld = XMMatrixIdentity();

	constants.initializeConstants();

	map_size = size;

	// setup for the heightmap
    m_map = new float*[map_size];
    for( int i=0; i<map_size; i++)
    {
        m_map[i] = new float[map_size];
    }

	for(int i=0; i<map_size; i++)
    {
        for( int j=0; j<map_size; j++)
        {
            m_map[i][j] = 0.0;
        }
    }

	//moreHills();
	
	//Adds bumpness to the map
	if (addBump) m_map = generateMap();

	//Create the terrain
	InitializeBuffers();

	//Build the SRV
	BuildHeightmapSRV();
}
//-------------------------------------------------------------------------------------
SimpleTerrain::SimpleTerrain(const SimpleTerrain& o)
{
}
//-------------------------------------------------------------------------------------
SimpleTerrain::~SimpleTerrain()
{
	ShutdownBuffers();
}
//-------------------------------------------------------------------------------------
/*!
 * Adds more hills to m_map by changing the constants
 * governing the hilliness and spread of the hills
 */
bool SimpleTerrain::moreHills()
{
    if (constants.a < 8.0)
    {
        constants.a += 0.25;
        constants.hills += 2;
        constants.sigma2x += 10.0;
        constants.sigma2y += 10.0;

        return true;
    }

    return false;
}
//-------------------------------------------------------------------------------------
/*!
 * Does the opVertexite of moreHills()
 */
bool SimpleTerrain::lessHills()
{
    if (constants.a > 0.25)
    {
        constants.a -= 0.25;
        constants.hills -= 2;
        constants.sigma2x -= 10.0;
        constants.sigma2y -= 10.0;

        return true;
    }

    return false;
}
//-------------------------------------------------------------------------------------
/*!
 * Randomly find points in the height map and calls
 * addHill on that point (see addHill)
 */
float** SimpleTerrain::generateMap()
{
    srand ( time(NULL) );

    for(int i = 0; i< constants.hills; i++)
    {
        int x = floor( rand()%10 / 10.0 * map_size );
        int z = floor( rand()%10 / 10.0 * map_size );

        addHill(x,z);
    }

    return m_map;
}
//-------------------------------------------------------------------------------------
/*!
 * Effectively adds a hill to the height map by
 * iterating through the points in the map and
 * checking their distance from the hill point.
 * If the point is within the specified hill's range,
 * then a height value is calculated for the point and
 * added to the corresponding element in the height map.
 *
 * @param int x, int z
 * the hill point
 */
void SimpleTerrain::addHill(int x, int z)
{
    int diameter = M_RADII * 2;
    int numPoints = pow( diameter, 2);

    int startRow = max(0.0, z-M_RADII);
    int endRow = min((double)map_size, z+M_RADII);

    int startCol = max(0.0, x-M_RADII);
    int endCol = min((double)map_size, x+M_RADII);

    for(int row = startRow ; row < endRow; row++)
    {
        for(int col = startCol; col<endCol; col++)
        {
            float xOff = col-x;
            float zOff = row-z;
            float dist2 = pow(zOff, 2) + pow(xOff, 2);
            if( dist2 > M_RADII*M_RADII)
                continue;
            else
            {
                float xContrib = pow( xOff, 2) / 2 / constants.sigma2x;
                float zContrib = pow( zOff, 2) / 2 / constants.sigma2y;
                float primary = exp( -(xContrib + zContrib));
                float height = constants.a*primary;
                m_map[row][col] += height;
            }
        }
    }
}
//-------------------------------------------------------------------------------------
void SimpleTerrain::ReGenBuffers()
{
	int index = 0;
	// Load the vertex and index array with the terrain data.
	for (int z = 0-(map_size/2), i = 0; z < 0+(map_size/2), i < map_size; z++, i++)
	{
		for (int x = 0-(map_size/2), j = 0; x < 0+(map_size/2), j < map_size; x++, j++)
		{
			float r = m_map[i][j];
			index = (map_size * i) + j;
			vertices[index].position = XMFLOAT3(x, z, r);
			vertices[index].tex_uv = XMFLOAT2(j * (1.0f / (map_size)), i * (1.0f / (map_size)));
		}
	}

	D3D11_MAPPED_SUBRESOURCE resource;
	m_d3d->GetDeviceContext()->Map(m_vertex_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	memcpy(resource.pData, &vertices[0], sizeof(VertexType) * m_vertex_count);
	m_d3d->GetDeviceContext()->Unmap(m_vertex_buffer, 0);
}
//-------------------------------------------------------------------------------------
bool SimpleTerrain::InitializeBuffers()
{
	unsigned long* indices;
	int index, i, j;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int index1, index2, index3, index4;
	float height;

	//Calculate the number of vertices in the heightmap mesh
	m_vertex_count = (map_size) * (map_size);
	m_index_count = (map_size * 2) * (map_size - 1) + (map_size - 2);

	NumFaces  = (map_size-1)*(map_size-1)*2;

	// Create the vertex array.
	vertices = new VertexType[m_vertex_count];
	if(!vertices) return false;

	// Create the index array.
	indices = new unsigned long[m_index_count];
	if(!indices) return false;

	index = 0;

	// Load the vertex and index array with the terrain data.
	for (int z = 0; z < map_size; z++)
	{
		for (int x = 0; x < map_size; x++)
		{
			float r = m_map[x][z];
			index = (map_size * x) + z;
			vertices[index].position = XMFLOAT3(x, r, z);
			vertices[index].tex_uv = XMFLOAT2(x * (1.0f / (map_size)), z * (1.0f / (map_size)));
		}
	}

	index = 0;

	for (int z = 0; z < map_size - 1; z++)
	{
		//Even rows move left to right, odd rows right to left
		if (z % 2 == 0)
		{
			//Even row
			int x;
			for (x = 0; x < map_size; x++)
			{
				indices[index++] = x + (z * map_size);
				indices[index++] = x + (z * map_size) + map_size;
			}

			//Insert degenerate vertex if this isn't the last row
			if (z != map_size - 2)
			{
				indices[index++] = --x + (z * map_size);
			}
		}
		else
		{
			//Odd row
			int x;
			for (x = map_size - 1; x >= 0; x--)
			{
				indices[index++] = x + (z * map_size);
				indices[index++] = x + (z * map_size) + map_size;
			}

			//Insert degenerate vertex if this isn't the last row
			if (z != map_size - 2)
			{
				indices[index++] = ++x + (z * map_size);
			}
		}
	}

	//Setup the description of the static vertex buffer
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertex_count;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
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
	indexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_index_count;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	//Give the subresource a pointer to the index data
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	//Create the index buffer
	result = m_d3d->GetDevice11()->CreateBuffer(&indexBufferDesc, &indexData, & m_index_buffer);

	if (FAILED(result)) return false;

	return true;
}
//-------------------------------------------------------------------------------------
void SimpleTerrain::ShutdownBuffers()
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
void SimpleTerrain::RenderBuffers()
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
vector<float> SimpleTerrain::GetHeightmap()
{
	vector<float> verts;
	int index;
	verts.resize( map_size * map_size );
	for (int z = 0; z < map_size; z++)
	{
		for (int x = 0; x < map_size; x++)
		{
			index = (map_size * x) + z;
			float r = m_map[x][z];
			verts[index] = r;
		}
	}

	return verts;
}
//-------------------------------------------------------------------------------------
void SimpleTerrain::BuildHeightmapSRV()
{
	D3D11_TEXTURE2D_DESC texDesc;
	texDesc.Width = map_size;
	texDesc.Height = map_size;
    texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format    = DXGI_FORMAT_R16_FLOAT;
	texDesc.SampleDesc.Count   = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	vector<float> verts = GetHeightmap();
	
	// HALF is defined in xnamath.h, for storing 16-bit float.
	std::vector<HALF> hmap(verts.size());
	std::transform(verts.begin(), verts.end(), hmap.begin(), XMConvertFloatToHalf);
	
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = &hmap[0];
    data.SysMemPitch = map_size * sizeof(HALF);
    data.SysMemSlicePitch = 0;

	ID3D11Texture2D* hmapTex = 0;
	m_d3d->GetDevice11()->CreateTexture2D(&texDesc, &data, &hmapTex);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;
	m_d3d->GetDevice11()->CreateShaderResourceView(hmapTex, &srvDesc, &mHeightMapSRV);

	// SRV saves reference.
	SAFE_RELEASE(hmapTex);
}
//-------------------------------------------------------------------------------------