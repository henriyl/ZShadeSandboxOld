//================================================================================================================
// NewHUDWindow.h
//
//================================================================================================================
// History
//
// Created on 4/3/2015 by Dustin Watson
//================================================================================================================
#ifndef __NEWHUDWINDOW_H
#define __NEWHUDWINDOW_H
//================================================================================================================
//#include <Windows.h>
//#include <CommCtrl.h>
#include "Sprite.h"
#include "Convert.h"
#include "GameDirectory.h"
#include "Menu.h"
#include "WinLabel.h"
#include "WinText.h"
#include "WinTextbox.h"
#include "WinButton.h"
#include "WinRadioGroup.h"
#include "MenuEditorSystem.h"
using namespace GUIComponents;
//================================================================================================================
//Defines
#define HUD_CLOSE_BUTTON          0
#define HUD_SIZE_TEXT             1
#define HUD_SIZE_RADIO_CONTAINER  2
#define HUD_SIZE_RADIO_800X600    3
#define HUD_SIZE_RADIO_1024X768   4
#define HUD_SIZE_RADIO_1366X768   5
#define HUD_SIZE_RADIO_1280X1024  6
#define HUD_SIZE_RADIO_1920X1080  7
#define HUD_CREATE_BUTTON         8
//================================================================================================================
class NewHUDWindow
{
public:
	
	NewHUDWindow(HUDEditorSystem* hudEditorSystem, HINSTANCE hInstance, D3D* d3d);
	NewHUDWindow(const NewHUDWindow& other);
	~NewHUDWindow();
	
	int GetHUDWidth() { return m_hudWidth; }
	int GetHUDHeight() { return m_hudHeight; }
	
	void AddHUDEditorSystem(HUDEditorSystem* hudEditorSystem);
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
	WinLabel m_hudSizeText;
	//WinLabel m_menuNameText;
	//WinLabel m_menuTypeText;
	WinRadioGroup m_hudSizeContainer;
	//WinRadioGroup m_hudTypeContainer;
	//WinTextbox m_hudNameTextbox;
	
	bool m_created;
	bool m_running;
	bool m_escape;
	
	HUDEditorSystem* m_HUDEditorSystem;
	GameDirectory2D* gd;
	
	//vector<string> m_createdMenus;
	
	//HINSTANCE m_hInstance;
	//HWND m_hwnd;
	//HBITMAP m_backgroundImage;
	//HDC m_winDC;
	
	int m_windowWidth;
	int m_windowHeight;
	
	int m_hudWidth;
	int m_hudHeight;
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
#endif//__NEWHUDWINDOW_H