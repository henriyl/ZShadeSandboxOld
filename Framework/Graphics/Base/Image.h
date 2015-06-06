#ifndef __IMAGE_H
#define __IMAGE_H
//-----------------------------------------------------------------------------------------
#include "D3DIncludes.h"
#include "D3D.h"
#include <string>
#include "CGlobal.h"
#include "QuadMesh.h"
//#include "TextureShader.h"
//#include "TransparentShader.h"
using std::string;
//-----------------------------------------------------------------------------------------
namespace ZShadeSandboxGraphics {
	class Image
	{
	public:
		Image(string texture_name, string base_sprite_path, float x, float y, int width, int height, D3D* d3d, string base_path_optional = "", string optionalColorName = "");
		~Image();

		void Draw(bool useTransparent, Camera* camera);

		string GetName()              { return mName; }
		string GetOptionalColorName() { return mOptionalColorName; }

		void SetOptionalBitmap(ZShadeSandboxMesh::QuadMesh* tex) { mOptionalBitmap = tex; }

		float GetXPos()      { return mXPos; }
		float GetYPos()      { return mYPos; }
		int GetWidth()     { return mWantedWidth; }
		int GetHeight()    { return mWantedHeight; }

		int GetImageWidth();
		int GetImageHeight();

		void MoveImageX(float amount);
		void MoveImageY(float amount);

		RECT GetRect();

	private:
		ZShadeSandboxMesh::QuadMesh*          mBitmap;
		ZShadeSandboxMesh::QuadMesh*          mOptionalBitmap;
		D3D*               mD3D;
		string             mName;
		string             mOptionalColorName;

		float              mXPos;
		float              mYPos;
		int                mWantedWidth;
		int                mWantedHeight;
	};
}
//-----------------------------------------------------------------------------------------
#endif//__IMAGE_H