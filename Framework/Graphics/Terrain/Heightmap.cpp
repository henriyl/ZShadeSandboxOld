#include "Heightmap.h"
#include "BetterString.h"
#include "CGlobal.h"
#include "HeightErosion.h"
#include "WaterErosion.h"
#include <fstream>
using namespace std;
using ZShadeSandboxTerrain::Heightmap;
//===================================================================================================================
Heightmap::Heightmap(string heightmap, int width, int height, float heightScale, float seaLevel, float maxHeight)
{
	m_heightmap_width = width;
	m_heightmap_height = height;
	m_height_scale = heightScale;
	m_seaLevel = seaLevel;
	m_maxHeight = maxHeight;
	
	ZShadeSandboxTerrain::ProceduralParameters pp;
	mProcGen = new ProceduralGenerator(pp);
	
	// Auto detect the height extension
	//m_ext = extension;
	string fileExt = CGlobal::GetFileExt(heightmap);
	
	if (fileExt == "bmp")
	{
		m_ext = EHeightExtension::Type::eBmp;
	}
	else if (fileExt == "raw")
	{
		m_ext = EHeightExtension::Type::eRaw;
	}
	else
	{
		m_ext = EHeightExtension::Type::eNone;
	}
	
	LoadElevation(heightmap);
}
//===================================================================================================================
Heightmap::Heightmap(ZShadeSandboxTerrain::ProceduralParameters pp, int width, int height, float heightScale, float seaLevel, float maxHeight)
{
	m_heightmap_width = width;
	m_heightmap_height = height;
	m_height_scale = heightScale;
	m_seaLevel = seaLevel;
	m_maxHeight = maxHeight;
	
	m_heightmap.resize(m_heightmap_width * m_heightmap_height);

	mProcGen = new ProceduralGenerator(pp);
	
	LoadProceduralElevation(pp);
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
void Heightmap::LoadProceduralElevation(ZShadeSandboxTerrain::ProceduralParameters pp)
{
	// Heightmap Erosion
	// Google: simple heightmap erosion
	// http://ranmantaru.com/blog/2011/10/08/water-erosion-on-heightmap-terrain/
	// http://www.dreamincode.net/forums/blog/2250/entry-4550-terrain-erosion/
	// http://www.gamedev.net/topic/334604-erosion-on-a-heightmap/
	// Procedurally load a height map
	
	switch (pp.proceduralType)
	{
		case EProceduralType::Type::eRandom:
		{
			BuildRandomHeightmap();
		}
		break;
		case EProceduralType::Type::ePerlinNoise:
		{
			BuildPerlinNoiseHeightmap();
		}
		break;
		case EProceduralType::Type::eFieldNoise:
		{
			BuildFieldNoiseHeightmap();
		}
		break;
		case EProceduralType::Type::eDiamondSquare:
		{
			BuildDiamondSquare();
		}
		break;
	}
	
	if (pp.useErosion)
	{
		switch (pp.erosionType)
		{
			case EErosionType::Type::eHeight:
			{
				ErodeHeight(pp.erosionValue);
			}
			break;
			case EErosionType::Type::eWater:
			{
				ErodeWater(pp.waterErosionParameters);
				//ErodeHeight(pp.erosionValue);
			}
			break;
		}
	}
	
	if (pp.useSmoothing)
	{
		Smooth(pp.smoothingPassCount);
	}
	
	if (pp.normalize)
	{
		Normalize(pp.normalizeFactor);
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
		case EHeightExtension::Type::eBmp:
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

			HeightData _data;

			m_heightmap.resize(m_heightmap_width * m_heightmap_height);
			
			//Initialize the position in the image data buffer
			k = 0;
			index = 0;

			//Read the image data into the heightmap
			for (j = 0; j < m_heightmap_height; j++)
			{
				for (i = 0; i < m_heightmap_width; i++)
				{
					//height = (bitmapImage[k] / 255.0f) * m_height_scale;
					height = bitmapImage[k] * m_height_scale;

					_data.x = i;
					_data.y = height;
					_data.z = j;

					m_heightmap[index++] = _data;

					k += 3;
				}
			}

			//ZShadeSandboxTerrain::ProceduralGenerator pgen(m_heightmap_width, m_seaLevel, m_maxHeight, m_heightmap);

			////pgen.AddPerlinNoiseHeightmap();
			//pgen.ErodeHeightmapInput(0.3);
			//pgen.Smooth(1);
			////pgen.Normalize(2);

			//if (m_heightmap.size() > 0) m_heightmap.clear();
			//m_heightmap.resize(m_heightmap_width * m_heightmap_width);

			//index = 0;

			//for (int z = 0; z < m_heightmap_width; z++)
			//{
			//	for (int x = 0; x < m_heightmap_width; x++)
			//	{
			//		ZShadeSandboxTerrain::HeightData hd;

			//		hd.x = x;
			//		hd.y = pgen.ReadProceduralHeight(x, z);
			//		hd.z = z;

			//		m_heightmap[index++] = hd;
			//	}
			//}

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
		case EHeightExtension::eRaw:
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

			HeightData _data;

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
void Heightmap::UpdateHeight(int x, int z, float height)
{
	if (InHeightmap(x, z))
	{
		m_heightmap[(z * m_heightmap_width) + x].y = height;
	}
}
//===================================================================================================================
void Heightmap::BuildFromProcGen()
{
	for (int z = 0; z < m_heightmap_width; z++)
	{
		for (int x = 0; x < m_heightmap_width; x++)
		{
			UpdateHeight(x, z, mProcGen->ReadProceduralHeight(x, z));
		}
	}
}
//===================================================================================================================
void Heightmap::BuildRandomHeightmap()
{
	mProcGen->BuildRandomHeightmap();
	BuildFromProcGen();
}
//===================================================================================================================
void Heightmap::BuildPerlinNoiseHeightmap()
{
	mProcGen->BuildPerlinNoiseHeightmap();
	BuildFromProcGen();
}
//===================================================================================================================
void Heightmap::BuildFieldNoiseHeightmap()
{
	mProcGen->BuildFieldNoiseHeightmap();
	BuildFromProcGen();
}
//===================================================================================================================
void Heightmap::BuildDiamondSquare()
{
	mProcGen->BuildDiamondSquare();
	BuildFromProcGen();
}
//===================================================================================================================
void Heightmap::AddRandomHeightmap()
{
	mProcGen->AddRandomHeightmap(m_heightmap);
	BuildFromProcGen();
}
//===================================================================================================================
void Heightmap::AddPerlinNoiseHeightmap()
{
	mProcGen->AddPerlinNoiseHeightmap(m_heightmap);
	BuildFromProcGen();
}
//===================================================================================================================
void Heightmap::AddFieldNoiseHeightmap()
{
	mProcGen->AddFieldNoiseHeightmap(m_heightmap);
	BuildFromProcGen();
}
//===================================================================================================================
void Heightmap::Smooth(int smoothingPassCount)
{
	if (smoothingPassCount > 0)
	{
		for (int i = 0; i < smoothingPassCount; i++)
		{
			Smooth();
		}
	}
}
//===================================================================================================================
void Heightmap::Smooth()
{
	int index = 0;

	for (int z = 0; z < m_heightmap_width; z++)
	{
		for (int x = 0; x < m_heightmap_width; x++)
		{
			float averageHeight = 0;
			float count = 0;

			for (int m = x - 1; m <= x + 1; m++)
			{
				for (int n = z - 1; n <= z + 1; n++)
				{
					if (m >= 0 && m < m_heightmap_width && n >= 0 && n < m_heightmap_width)
					{
						averageHeight += GetHeight(n, m);
						count += 1;
					}
				}
			}

			UpdateHeight(x, z, averageHeight / count);
		}
	}
}
//===================================================================================================================
void Heightmap::Normalize(float normalizeFactor)
{
	int index = 0;

	for (int z = 0; z < m_heightmap_width; z++)
	{
		for (int x = 0; x < m_heightmap_width; x++)
		{
			m_heightmap[index++].y /= normalizeFactor;
		}
	}
}
//===================================================================================================================
void Heightmap::ErodeHeight(float erosionValue)
{
	HeightErosion he(m_heightmap, m_heightmap_width, erosionValue);
	
	for (int z = 0; z < m_heightmap_width; z++)
	{
		for (int x = 0; x < m_heightmap_width; x++)
		{
			UpdateHeight(x, z, he.ReadErosionHeight(x, z));
		}
	}
}
//===================================================================================================================
void Heightmap::ErodeWater(ZShadeSandboxTerrain::WaterErosionParameters wep)
{
	WaterErosion we(m_heightmap, wep);

	for (int z = 0; z < m_heightmap_width; z++)
	{
		for (int x = 0; x < m_heightmap_width; x++)
		{
			UpdateHeight(x, z, we.ReadErosionHeight(x, z));
		}
	}
}
//===================================================================================================================
void Heightmap::Flatten(float flatHeight)
{
	for (int z = 0; z < m_heightmap_width; z++)
	{
		for (int x = 0; x < m_heightmap_width; x++)
		{
			UpdateHeight(x, z, flatHeight);
		}
	}
}
//===================================================================================================================