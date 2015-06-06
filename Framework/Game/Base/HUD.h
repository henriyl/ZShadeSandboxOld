//*************************************************************************************************************************
// HUD.h
//*************************************************************************************************************************
#ifndef __HUD_H
#define __HUD_H
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
#include <vector>
#include "Image.h"
#include "Text.h"
//--------------------------------------------------------------------------------------------------------------------
//The HUD will contain images and text
class HUD
{
public:
	HUD(D3D* d3d);
	HUD(const HUD& h);
	~HUD();

	void ResetComponentsToWindow(D3D* d3d, HWND hwnd, GameDirectory2D* gd, EngineOptions* eo);

	void RemoveImage(ZShadeSandboxGraphics::Image* im);
	void RemoveText(ZShadeSandboxGraphics::Text* t);

	vector<ZShadeSandboxGraphics::Image*> GetImages() { return m_images; }
	vector<ZShadeSandboxGraphics::Text*> GetTexts() { return m_texts; }

	void SetWidth(int w) { m_width = w; }
	void SetHeight(int h) { m_height = h; }

	int GetWidth() { return m_width; }
	int GetHeight() { return m_height; }

	bool HUDImageSelected(ZShadeSandboxGraphics::Image*& im, int x, int y);
	bool HUDTextSelected(ZShadeSandboxGraphics::Text*& text, int x, int y);

	void AddImage(ZShadeSandboxGraphics::Image* im);
	void AddText(ZShadeSandboxGraphics::Text* t);

	//void Draw10(bool useTransparent, TextureShader* textureShader, TransparentShader* transparentShader, Camera* camera);
	void Draw(bool useTransparent, Camera* camera);

private:
	D3D* m_D3DSystem;
	
	vector<ZShadeSandboxGraphics::Image*> m_images;
	vector<ZShadeSandboxGraphics::Text*> m_texts;

	int m_width;
	int m_height;
};
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
#endif//__HUD_H