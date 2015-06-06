//===============================================================================================================================
// TextureManager
//
//===============================================================================================================================
// History
//
// -Created on 6/14/2014 by Dustin Watson
//===============================================================================================================================
#ifndef __TEXTUREMANAGER_H
#define __TEXTUREMANAGER_H
//===============================================================================================================================
//===============================================================================================================================

#pragma managed(push, off)

//
// Includes
//

#include <comdef.h>
#include "BetterString.h"
#include "D3D.h"
#include <map>
using namespace std;

#define DEBUG

//_COM_SMARTPTR_TYPEDEF(ID3D11Texture2D, __uuidof(ID3D11Texture2D));

enum TextureType
{
	DDS,
	PNG,
	TGA
};

// To flip a texture:
//   In your shader do 1-u to flip horizontally or 1-v to flip vertically

//===============================================================================================================================
//===============================================================================================================================
class TextureManager
{
	D3D* mD3DSystem;
	static TextureManager* m_instance;
	
public:
	
	TextureManager(D3D* d3d);
	~TextureManager();
	
	static void NewInstance(D3D* d3d);
	static TextureManager* Instance();
	
	ID3D11ShaderResourceView* GetTexture(BetterString tex_filename);
	
	bool Exists(BetterString filename, D3D_VERSION d3d);
	
	/*
	*	Load a texture from disc and set it so that we can extract the data into a buffer for the compute shader to use.
	*   To make everything more clear we also save a copy of the texture data in main memory. We will copy from this buffer
	*	the data that we need to feed into the GPU for the compute shader.
	*/
	bool loadTexture(BetterString filename, ID3D11Texture2D** texture, int& textureDataSize, byte*& srcTextureData, TextureType tt);
	
	ID3D11Texture2D* ConvertToTex2D(ID3D11Resource* res);
	
	ID3D11ShaderResourceView* CreateTexture2DArraySRV(std::vector<std::string>& filenames);

	ID3D11ShaderResourceView* CreateRandomTexture1DSRV();

private:
	
	ID3D11ShaderResourceView* GetTexture(BetterString tex_filename, TextureType tt);
	ID3D11ShaderResourceView* LoadSRV(LPCWSTR tex_filename, TextureType tt);
	
	HRESULT CreateTGATextureFromFile(LPCWSTR tex_filename, ID3D11ShaderResourceView** srv);
	HRESULT LoadDDSTextureFromFile(LPCWSTR tex_filename, ID3D11ShaderResourceView** srv);
	HRESULT LoadWICTextureFromFile(LPCWSTR tex_filename, ID3D11ShaderResourceView** srv);
	
	map<string, ID3D11ShaderResourceView*> m_pTextures;
};

#pragma managed(pop)

//===============================================================================================================================
//===============================================================================================================================
#endif//__TEXTUREMANAGER_H