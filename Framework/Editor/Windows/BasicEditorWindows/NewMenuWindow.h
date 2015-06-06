//================================================================================================================
// NewMenuWindow.h
//
//================================================================================================================
// History
//
// Created on 1/11/2014 by Dustin Watson
//================================================================================================================
#ifndef __NEWMENUWINDOW_H
#define __NEWMENUWINDOW_H
//================================================================================================================
//#include <Windows.h>
//#include <CommCtrl.h>
#include "Sprite.h"
#include "Convert.h"
#include "GameDirectory.h"
#include "Menu.h"
#include "WinText.h"
#include "WinLabel.h"
#include "WinTextbox.h"
#include "WinButton.h"
#include "WinRadioGroup.h"
#include "MenuEditorSystem.h"
using namespace GUIComponents;
//================================================================================================================
//Defines
#define CLOSE_BUTTON               0
#define MENU_NAME_TEXT             1
#define MENU_NAME_TEXTBOX          2
#define MENU_SIZE_TEXT             3
#define MENU_SIZE_RADIO_CONTAINER  4
#define MENU_SIZE_RADIO_800X600    5
#define MENU_SIZE_RADIO_1024X768   6
#define MENU_SIZE_RADIO_1366X768   7
#define MENU_SIZE_RADIO_1280X1024  8
#define MENU_SIZE_RADIO_1920X1080  9
#define MENU_TYPE_TEXT             10
#define MENU_TYPE_RADIO_CONTAINER  11
#define MENU_TYPE_RADIO_MAINMENU   12
#define MENU_TYPE_RADIO_INGAMEMENU 13
#define CREATE_BUTTON              14
//================================================================================================================
class NewMenuWindow
{
public:
	
	NewMenuWindow(MenuEditorSystem* menuEditorSystem, HINSTANCE hInstance, D3D* d3d);
	NewMenuWindow(const NewMenuWindow& other);
	~NewMenuWindow();
	
	int GetMenuWidth() { return m_menuWidth; }
	int GetMenuHeight() { return m_menuHeight; }
	string GetMenuName() { return m_menuName; }
	ZShadeSandboxEnvironment::EMenuType::Type GetMenuType() { return m_menuType; }
	
	void AddMenuEditorSystem(MenuEditorSystem* menuEditorSystem);
	//void AddMenuName(string name);
	bool IsRunning() { return m_running; }
	bool InFocus();
	bool IsCreated() { return m_created; }
	void SetCreated(bool created) { m_created = created; }
	int Run();
	void Show(bool show = false);
	void SetGD(GameDirectory2D* gd);
	void SetPosition(int x, int y);
	void SetEscape(bool bSet) { m_escape = bSet; }
	void Invalidate();
	
	//bool OnCreate();
	
	//void CreateGUIComponents(HWND hwnd);
	
	//void ProcessUpdates(WPARAM wParam, LPARAM lParam);
	
	//void BltBackground(HDC hdc);
	
	//LRESULT CALLBACK WinProc(
	//	HWND hwnd,
	//	UINT msg,
	//	WPARAM wParam,
	//	LPARAM lParam);
	
private:
	
	LRESULT Command(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT Paint(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
	D3D* m_D3DSystem;
	Window* m_Window;
	
	WinButton m_closeButton;
	WinButton m_createButton;
	WinLabel m_menuSizeText;
	WinLabel m_menuNameText;
	WinLabel m_menuTypeText;
	WinRadioGroup m_menuSizeContainer;
	WinRadioGroup m_menuTypeContainer;
	WinTextbox m_menuNameTextbox;
	
	bool m_created;
	bool m_running;
	bool m_escape;
	
	MenuEditorSystem* m_MenuEditorSystem;
	GameDirectory2D* gd;
	
	//vector<string> m_createdMenus;
	
	//HINSTANCE m_hInstance;
	//HWND m_hwnd;
	//HBITMAP m_backgroundImage;
	//HDC m_winDC;
	
	int m_windowWidth;
	int m_windowHeight;
	
	int m_menuWidth;
	int m_menuHeight;
	string m_menuName;
	ZShadeSandboxEnvironment::EMenuType::Type m_menuType;
};
//================================================================================================================
//static LRESULT CALLBACK CreateMenuWindowWndProc(
//	HWND hwnd,
//	UINT msg,
//	WPARAM wParam,
//	LPARAM lParam);
//--------------------------------------------------------------------------------------------------------------------
//static NewMenuWindow* m_createMenuWindowInstance = NULL;
//--------------------------------------------------------------------------------------------------------------------
#endif//__NEWMENUWINDOW_H