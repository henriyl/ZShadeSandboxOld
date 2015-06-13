#include "Image.h"
#include "MaterialManager.h"
using ZShadeSandboxGraphics::Image;
//-----------------------------------------------------------------------------------------
Image::Image(string texture_name, string base_sprite_path, float x, float y, int width, int height,
	D3D* d3d, string base_path_optional, string optionalColorName) :
	mD3D(d3d), mName(texture_name), mXPos(x), mYPos(y), mWantedWidth(width), mWantedHeight(height),
	mBitmap(0), mOptionalBitmap(0)
{
	if (texture_name.compare("") != 0)
	{
		MaterialManager::Instance()->AddDiffuse(d3d, base_sprite_path, texture_name, texture_name);

		ZShadeSandboxMesh::MeshParameters mp;
		//mp.color = XMFLOAT4(0, 0, 0, 0);
		mp.pos = XMFLOAT3(0, 0, 0);
		mp.rot = XMFLOAT3(0, 0, 0);
		mp.scale = XMFLOAT3(1, 1, 1);
		mp.rotationAxisX = false;
		mp.rotationAxisY = false;
		mp.rotationAxisZ = false;
		mp.vertexType = ZShadeSandboxMesh::EVertexType::VT_Tex;
		//mp.tt = PNG;
		mp.useCustomShader = false;
		//mp.textureName = texture_name;
		mp.textureWidth = width;
		mp.textureHeight = height;
		//mp.texturePath = base_sprite_path;
		mp.material = MaterialManager::Instance()->GetMaterial(texture_name);
		mp.in2D = true;

		mBitmap = new ZShadeSandboxMesh::QuadMesh(d3d, mp, true);
	}
	else
	{
		mBitmap = 0;
	}

	if (optionalColorName.compare("") != 0)
	{
		mOptionalColorName = optionalColorName;
		MaterialManager::Instance()->AddDiffuse(d3d, base_path_optional, optionalColorName, optionalColorName);
		
		ZShadeSandboxMesh::MeshParameters mp;
		//mp.color = XMFLOAT4(0, 0, 0, 0);
		mp.pos = XMFLOAT3(0, 0, 0);
		mp.rot = XMFLOAT3(0, 0, 0);
		mp.scale = XMFLOAT3(1, 1, 1);
		mp.rotationAxisX = false;
		mp.rotationAxisY = false;
		mp.rotationAxisZ = false;
		mp.vertexType = ZShadeSandboxMesh::EVertexType::VT_Tex;
		//mp.tt = PNG;
		mp.useCustomShader = false;
		//mp.textureName = optionalColorName;
		mp.textureWidth = width;
		mp.textureHeight = height;
		//mp.texturePath = base_path_optional;
		mp.material = MaterialManager::Instance()->GetMaterial(optionalColorName);
		mp.in2D = true;

		mOptionalBitmap = new ZShadeSandboxMesh::QuadMesh(d3d, mp, true);
	}
}
//-----------------------------------------------------------------------------------------
Image::~Image()
{
	//if (mBitmap)
	//{
		//mBitmap->Shutdown();
		//mBitmap = 0;
	//}
}
//-----------------------------------------------------------------------------------------
RECT Image::GetRect()
{
	RECT P;
	P.top = mXPos;
	P.left = mYPos;
	P.right = P.left + mWantedWidth;
	P.bottom = P.top + mWantedHeight;
	return P;
}
//-----------------------------------------------------------------------------------------
int Image::GetImageWidth()
{
	if (mBitmap == NULL)
	{
		return mWantedWidth;// + 50;
	}

	return GetWidth();
}
//-----------------------------------------------------------------------------------------
int Image::GetImageHeight()
{
	if (mBitmap == NULL)
	{
		return mWantedHeight;// + 20;
	}

	return GetHeight();
}
//-----------------------------------------------------------------------------------------
void Image::MoveImageX(float amount)
{
	mXPos = amount;
}
//-----------------------------------------------------------------------------------------
void Image::MoveImageY(float amount)
{
	mYPos = amount;
}
//-----------------------------------------------------------------------------------------
void Image::Draw(bool useTransparent, Camera* camera)
{
	if (mBitmap)
	{
		mBitmap->UpdateBuffers(mXPos, mYPos);
		ZShadeSandboxMesh::MeshRenderParameters mrp;
		mrp.camera = camera;
		//mrp.bTransparent = useTransparent;
		//mrp.blendAmount = 0;
		mBitmap->Render(mrp);
	}
	else
	{
		mOptionalBitmap->UpdateBuffers(mXPos, mYPos);
		ZShadeSandboxMesh::MeshRenderParameters mrp;
		mrp.camera = camera;
		//mrp.bTransparent = useTransparent;
		//mrp.blendAmount = 0;
		mOptionalBitmap->Render(mrp);
	}
}
//-----------------------------------------------------------------------------------------