#include "Heightmap.h"
#include "BetterString.h"
#include <fstream>
using namespace std;
using ZShadeSandboxTerrain::Heightmap;
//===================================================================================================================
Heightmap::Heightmap(string heightmap, int extension, int width, int height, float cellSpacing, float heightScale, int leafSize)
{
	m_heightmap_width = width;
	m_heightmap_height = height;
	m_height_scale = heightScale;
	m_cellSpacing = cellSpacing;
	m_ext = extension;

	LoadElevation(heightmap);
}
//===================================================================================================================
Heightmap::Heightmap(const Heightmap& o)
{
}
//===================================================================================================================
Heightmap::~Heightmap()
{
}
//===================================================================================================================
float Heightmap::SampleHeight(int x, int z)
{
	return m_heightmap[(z * m_heightmap_width) + x].y;
}
//===================================================================================================================
float Heightmap::SampleHeight(int index)
{
	return m_heightmap[index].y;
}
//===================================================================================================================
void Heightmap::UpdateHeightValues(float heightScale, float zScale)
{
	for (int z = 0; z < m_heightmap_width; z++)
	{
		for (int x = 0; x < m_heightmap_width; x++)
		{
			m_heightmap[(z * m_heightmap_width) + x].y = ((m_heightmap[(z * m_heightmap_width) + x].y * heightScale * 100.0f) / 255.0) / ((zScale * 2) + 1);
		}
	}
}
//===================================================================================================================
bool Heightmap::LoadElevation(string heightmap)
{
	//Check the extention of the file
	BetterString hmap = heightmap;
	BetterString ext = hmap.Strip('.', hmap.size());
	//Not a valid script
	if (ext != "bmp")
	{
		if (ext != "raw")
		{
			return false;
		}
	}

	switch (m_ext)
	{
		case EHeightExtension::BMP:
		{
			#pragma region "Load BMP"
			FILE* file_ptr;
			int error;
			unsigned int count;
			BITMAPFILEHEADER bitmapFileHeader;
			BITMAPINFOHEADER bitmapInfoHeader;
			int imageSize, i, j, k, index;
			unsigned char* bitmapImage;
			unsigned char height;

			//Open the heightmap file in binary
			error = fopen_s(&file_ptr, heightmap.c_str(), "rb");

			if (error != 0) return false;

			//Read in the file header
			count = fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, file_ptr);

			if (count != 1) return false;

			//Read in the bitmap info header
			count = fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, file_ptr);

			if (count != 1) return false;

			//Stoer the size of the heightmap
			m_heightmap_width = bitmapInfoHeader.biWidth;
			m_heightmap_height = bitmapInfoHeader.biHeight;

			//Calculate the size of the bitmap image data
			imageSize = m_heightmap_width * m_heightmap_height * 3;

			//Allocate memory for the bitmap image data
			bitmapImage = new unsigned char[imageSize];

			if (!bitmapImage) return false;

			//Move to the beginning of the bitmap data
			fseek(file_ptr, bitmapFileHeader.bfOffBits, SEEK_SET);

			//Read in the bitmap image data
			count = fread(bitmapImage, 1, imageSize, file_ptr);

			if (count != imageSize) return false;

			//Close the file
			error = fclose(file_ptr);

			if (error != 0) return false;

			HEIGHT_DATA _data;

			m_heightmap.resize(m_heightmap_width * m_heightmap_height);
			
			//Initialize the position in the image data buffer
			k = 0;
			index = 0;

			//Read the image data into the heightmap
			for (j = 0; j < m_heightmap_height; j++)
			{
				for (i = 0; i < m_heightmap_width; i++)
				{
					height = (bitmapImage[k]/* / 255.0f*/) * m_height_scale;

					_data.x = i;
					_data.y = height;
					_data.z = j;

					m_heightmap[index++] = _data;

					k += 3;
				}
			}

			//Create the structure to hold the raw height data
			//m_height_data = new HEIGHT_DATA[m_heightmap_width * m_heightmap_height];
			
			//m_height_data = new float*[m_heightmap_height];
			//for (i = 0; i < m_heightmap_height; i++)
			//{
			//	m_height_data[i] = new float[m_heightmap_width];
			//}

			//if (!m_height_data) return false;

			////Initialize the position in the image data buffer
			//k = 0;

			//Read the image data into the heightmap
			//for (j = 0; j < m_heightmap_height; j++)
			//{
			//	for (i = 0; i < m_heightmap_width; i++)
			//	{
			//		height = (bitmapImage[k] / 255.0f) * m_height_scale;

			//		m_height_data[i][j] = (float)height;

			//		k += 3;
			//	}
			//}

			//Release the bitmap image data
			delete[] bitmapImage;
			bitmapImage = 0;

			return true;
			#pragma endregion
		}
		break;
		case EHeightExtension::RAW:
		{
			#pragma region "Load RAW"
			vector<unsigned char> in (m_heightmap_width * m_heightmap_height);

			//Open the file
			ifstream inFile(heightmap, ios_base::binary);
			
			if (inFile)
			{
				//Read the RAW bytes
				inFile.read((char*)&in[0], (streamsize)in.size());

				//Done with file
				inFile.close();
			}

			int k = 0, index = 0;
			float height;

			HEIGHT_DATA _data;

			m_heightmap.resize(m_heightmap_width * m_heightmap_height);

			//Copy the array data into a float array and scale it.
			for (int j = 0; j < m_heightmap_height; j++)
			{
				for (int i = 0; i < m_heightmap_width; i++)
				{
					height = (in[k++] / 255.0f) * m_height_scale;

					_data.x = i;
					_data.y = height;
					_data.z = j;

					m_heightmap[index++] = _data;
				}
			}

			//Copy the array data into a float array and scale it.
			//m_heightmap.resize(m_heightmap_width * m_heightmap_height, 0);
			//for (UINT i = 0; i < m_heightmap_width * m_heightmap_height; ++i)
			//{
			//	m_heightmap[i] = (in[i] / 255.0f) * m_height_scale;
			//}
			#pragma endregion
		}
		break;
		default: break;//Do not support anything else currently
	}
}
//===================================================================================================================
bool Heightmap::InBounds(int x, int z)
{
	//True if x and z are valid indices; False otherwise
	return x >= 0 && x < m_heightmap_height && z >= 0 && z < m_heightmap_width;
}
//===================================================================================================================
bool Heightmap::InHeightmap(float x, float z)
{
	if (x < m_heightmap_width && z < m_heightmap_width && x >= 0 && z >= 0)
	{
		return true;
	}

	return false;
}
//===================================================================================================================
float Heightmap::GetHeight(float x, float z)
{
	if (InHeightmap(x, z))
	{
		return SampleHeight(x, z);
	}

	return 0;
}
//===================================================================================================================
/*float Heightmap::GetHeight(float x, float z)
{
	float w = (m_heightmap_width - 1) * m_cellSpacing;
	float depth = (m_heightmap_height - 1) * m_cellSpacing;

	// Transform from terrain local space to "cell" space.
	float c = (x + 0.5f* w) /  m_cellSpacing;
	float d = (z - 0.5f* depth) / -m_cellSpacing;

	// Get the row and column we are in.
	int row = (int)floorf(d);
	int col = (int)floorf(c);

	// Grab the heights of the cell we are in.
	// A*--*B
	//  | /|
	//  |/ |
	// C*--*D
	float A = m_heightmap[row * m_heightmap_width + col];
	float B = m_heightmap[row * m_heightmap_width + col + 1];
	float C = m_heightmap[(row+1) * m_heightmap_height + col];
	float D = m_heightmap[(row+1) * m_heightmap_height + col + 1];

	// Where we are relative to the cell.
	float s = c - (float)col;
	float t = d - (float)row;

	// If upper triangle ABC.
	if( s + t <= 1.0f)
	{
		float uy = B - A;
		float vy = C - A;
		return A + s*uy + t*vy;
	}
	else // lower triangle DCB.
	{
		float uy = C - D;
		float vy = B - D;
		return D + (1.0f-s)*uy + (1.0f-t)*vy;
	}
}
//===================================================================================================================
void Heightmap::Smooth()
{
	vector<float> dest( m_heightmap.size() );

	for(UINT i = 0; i < m_heightmap_height; ++i)
	{
		for(UINT j = 0; j < m_heightmap_width; ++j)
		{
			dest[i * m_heightmap_width + j] = Average(i,j);
		}
	}

	//Replace the old heightmap
	m_heightmap = dest;
}
//===================================================================================================================
float Heightmap::Average(int x, int z)
{
	// Function computes the average height of the ij element.
	// It averages itself with its eight neighbor pixels.  Note
	// that if a pixel is missing neighbor, we just don't include it
	// in the average--that is, edge pixels don't have a neighbor pixel.
	//
	// ----------
	// | 1| 2| 3|
	// ----------
	// |4 |ij| 6|
	// ----------
	// | 7| 8| 9|
	// ----------

	float avg = 0.0f;
	float num = 0.0f;

	// Use int to allow negatives.  If we use UINT, @ i=0, m=i-1=UINT_MAX
	// and no iterations of the outer for loop occur.
	for(int m = x-1; m <= x+1; ++m)
	{
		for(int n = z-1; n <= z+1; ++n)
		{
			if( InBounds(m,n) )
			{
				avg += m_heightmap[m * m_heightmap_width + n];
				num += 1.0f;
			}
		}
	}

	return avg / num;
}
//===================================================================================================================
void Heightmap::Normalize()
{
	int i, j;

	for (j = 0; j < m_heightmap_height; j++)
	{
		for (i = 0; i < m_heightmap_width; i++)
		{
			//m_height_data[ (m_heightmap_height * j) ].y /= 15.0f;
			m_height_data[j][i] /= 5.0f;
		}
	}
}
//===================================================================================================================
*/