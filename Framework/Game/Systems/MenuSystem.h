//*************************************************************************************************************************
// MenuSystem.h
//*************************************************************************************************************************
#ifndef __MENUSYSTEM_H
#define __MENUSYSTEM_H
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//Includes
#include <string>
#include <map>
#include "Menu.h"
#include "Button.h"
#include "Text.h"
using namespace std;
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
class MenuSystem
{
public:
	MenuSystem(D3D* d3d);
	MenuSystem(const MenuSystem& o);
	~MenuSystem();

	void GetMenuNames(vector<string>& names);

	map<string, ZShadeSandboxEnvironment::Menu*> GetMenus() { return m_menus; }
	vector<ZShadeSandboxGraphics::Button*> GetButtons(string menu_name);
	vector<ZShadeSandboxGraphics::Text*> GetTexts(string menu_name);

	ZShadeSandboxEnvironment::Menu* GetMenu(string name) { return m_menus[name]; }

	void ResetComponentsToWindow(D3D* d3d, HWND hwnd, GameDirectory2D* gd, EngineOptions* eo);

	void SetMenuToRender(string menu);

	string GetMenuRenderName() { return m_menu_to_render; }

	void RemoveButton(ZShadeSandboxGraphics::Button* b);
	void RemoveText(ZShadeSandboxGraphics::Text* t);

	void RemoveMenu(string menu_name);

	bool MenuButtonSelected(ZShadeSandboxGraphics::Button*& button, int x, int y);
	bool MenuTextSelected(ZShadeSandboxGraphics::Text*& text, int x, int y);

	void AddMenu(ZShadeSandboxEnvironment::Menu* m);

	void AddMenuLinkName(string menu, string link_name);
	void AddMenuBackgroundSprite(string menu, ZShadeSandboxGraphics::Image* image);
	void AddMenuButton(string menu, ZShadeSandboxGraphics::Button* button);
	void AddMenuText(string menu, ZShadeSandboxGraphics::Text* text);

	void UpdateButtonMouseUp(WPARAM btnState, int x, int y);
	void UpdateButtonMouseDown(WPARAM btnState, int x, int y);
	void UpdateButtonMouseOn(WPARAM btnState, int x, int y);
	void UpdateButtonMouseMove(WPARAM btnState, int x, int y);
	//void UpdateMenu(Keyboard keyboard, POINT pt);

	//bool Render10(TextureShader* textureShader, TransparentShader* transparentShader, Camera* camera);
	bool Render(Camera* camera);
	
private:
	D3D* m_d3d;
	string m_menu_to_render;
	map<string, ZShadeSandboxEnvironment::Menu*> m_menus;
};
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
#endif//__MENUSYSTEM_H