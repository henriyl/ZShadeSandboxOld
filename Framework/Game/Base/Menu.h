//*************************************************************************************************************************
// Menu.h
//*************************************************************************************************************************
#ifndef __MENU_H
#define __MENU_H
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//Includes
#include <string>
#include <vector>
#include "Image.h"
#include "Button.h"
#include "Text.h"
using namespace std;
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
namespace ZShadeSandboxEnvironment {
namespace EMenuType
{
	enum Type
	{
		eMainMenu,
		eInGameMenu
	};
}
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
class Menu
{
public:
	Menu();
	Menu(const Menu& o);
	~Menu();

	void RemoveButton(ZShadeSandboxGraphics::Button* b);
	void RemoveText(ZShadeSandboxGraphics::Text* t);

	void ResetComponentsToWindow(D3D* d3d, HWND hwnd, GameDirectory2D* gd, EngineOptions* eo);

	vector<ZShadeSandboxGraphics::Button*> GetButtons() { return m_menu_buttons; }
	vector<ZShadeSandboxGraphics::Text*> GetTexts() { return m_menu_text; }

	bool MenuButtonSelected(ZShadeSandboxGraphics::Button*& button, int x, int y);
	bool MenuTextSelected(ZShadeSandboxGraphics::Text*& text, int x, int y);

	string GetBackgroundName() { return m_background->GetName(); }

	void SetMenuType(EMenuType::Type mt) { m_menu_type = mt; }
	EMenuType::Type GetMenuType() { return m_menu_type; }

	void SetMenuName(string menu) { m_menu_name = menu; }
	string GetMenuName() { return m_menu_name; }

	void SetLinkMenuName(string menu) { m_link_menu_name = menu; }
	string GetLinkMenuName() { return m_link_menu_name; }

	int GetMenuWidth() { return m_menu_width; }
	int GetMenuHeight() { return m_menu_height; }

	void SetMenuWidth(int width) { m_menu_width = width; }
	void SetMenuHeight(int height) { m_menu_height = height; }

	void AddBackground(ZShadeSandboxGraphics::Image* image);
	void AddMenuButton(ZShadeSandboxGraphics::Button* button);
	void AddMenuText(ZShadeSandboxGraphics::Text* text);

	//void UpdateMenu(Keyboard keyboard, int x, int y);

	void UpdateButtonMouseUp(WPARAM btnState, int x, int y);
	void UpdateButtonMouseDown(WPARAM btnState, int x, int y);
	void UpdateButtonMouseOn(WPARAM btnState, int x, int y);
	void UpdateButtonMouseMove(WPARAM btnState, int x, int y);

	//bool Render10(TextureShader* textureShader, TransparentShader* transparentShader, Camera* camera);
	bool Render(Camera* camera);

private:
	int m_menu_width;
	int m_menu_height;

	EMenuType::Type m_menu_type;
	string m_menu_name;
	string m_link_menu_name;

	ZShadeSandboxGraphics::Image* m_background;
	
	vector<ZShadeSandboxGraphics::Button*> m_menu_buttons;

	vector<ZShadeSandboxGraphics::Text*> m_menu_text;
};
}
//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
#endif//__MENU_H