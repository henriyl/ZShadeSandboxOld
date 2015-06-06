//*************************************************************************************************************************
// HUDSystem.h
//*************************************************************************************************************************
#ifndef __HUDSYSTEM_H
#define __HUDSYSTEM_H
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//Includes
#include <string>
#include <map>
#include "HUD.h"
class Keyboard;
using namespace std;
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
class HUDSystem
{
public:
	HUDSystem(D3D* d3d);
	HUDSystem(const HUDSystem& o);
	~HUDSystem();

	vector<ZShadeSandboxGraphics::Image*> GetImagess();
	vector<ZShadeSandboxGraphics::Text*> GetTexts();

	HUD* GetHUD() { return m_hud; }

	void SetWidth(int w) { m_hud->SetWidth(w); }
	void SetHeight(int h) { m_hud->SetHeight(h); }

	int GetWidth() { return m_hud->GetWidth(); }
	int GetHeight() { return m_hud->GetHeight(); }

	void ResetComponentsToWindow(D3D* d3d, HWND hwnd, GameDirectory2D* gd, EngineOptions* eo);

	void RemoveImage(ZShadeSandboxGraphics::Image* im);
	void RemoveText(ZShadeSandboxGraphics::Text* t);

	bool HUDImageSelected(ZShadeSandboxGraphics::Image*& im, int x, int y);
	bool HUDTextSelected(ZShadeSandboxGraphics::Text*& text, int x, int y);

	void AddHUD(HUD* hud);

	void AddHUDImage(ZShadeSandboxGraphics::Image* image);
	void AddHUDText(ZShadeSandboxGraphics::Text* text);

	//Might need, do not know
	void UpdateHUD(Keyboard keyboard, POINT pt);

	//void Render10(TextureShader* textureShader, TransparentShader* transparentShader, Camera* camera);
	void Render(Camera* camera);
	
private:
	D3D* m_D3DSystem;
	HUD* m_hud;
};
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
#endif//__HUDSYSTEM_H