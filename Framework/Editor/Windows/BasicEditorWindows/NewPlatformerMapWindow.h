//================================================================================================================
// NewMapWindowPlatformer.h
//
//================================================================================================================
// History
//
// Created on 1/11/2014 by Dustin Watson
//================================================================================================================
#ifndef __NEWPLATFORMERMAPWINDOW_H
#define __NEWPLATFORMERMAPWINDOW_H
//================================================================================================================
#include <Windows.h>
#include <CommCtrl.h>
//================================================================================================================
//GUI Component Includes
#include "WinText.h"
#include "WinButton.h"
#include "WinLabel.h"
#include "WinTextbox.h"
#include "WinComboBox.h"
#include "WinRadioGroup.h"
#include "MapEditorSystem2D.h"
class MapEditorSystem2D;
using namespace GUIComponents;
//================================================================================================================
//Defines
#define PLAT_CLOSE_BUTTON		0
#define PLAT_CREATE_BUTTON		1
#define PLAT_MAP_NAME_TEXT		2
#define PLAT_MAP_NAME			3
#define PLAT_WORLD_NAME_TEXT	4
#define PLAT_WORLD_NAME_CB		5
#define PLAT_MAP_WIDTH_TEXT		6
#define PLAT_MAP_WIDTH_TEXTBOX	7
#define PLAT_MAP_HEIGHT_TEXT	8
#define PLAT_MAP_HEIGHT_TEXTBOX	9
//================================================================================================================
class NewMapWindowPlatformer
{
public:
	NewMapWindowPlatformer(MapEditorSystem2D* mapEditorSystem, HINSTANCE hInstance, D3D* d3d);
	NewMapWindowPlatformer(const NewMapWindowPlatformer& other);
	~NewMapWindowPlatformer();

	int GetXSize() { return m_xSize; }
	int GetYSize() { return m_ySize; }

	bool IsRunning() { return m_running; }
	void SetPosition(int x, int y);
	void SetEscape(bool bSet) { m_escape = bSet; }
	void Invalidate();
	bool IsCreated() { return m_created; }
	void SetCreated(bool created) { m_created = created; }
	void Show(bool show = false);
	int Run();
	bool InFocus();
	
	//bool OnCreate();
	
	//void CreateGUIComponents(HWND hwnd);
	
	//void ProcessUpdates(HWND hwnd, WPARAM wParam, LPARAM lParam);
	
	//void BltBackground(HDC hdc);
	
	//LRESULT CALLBACK WinProc(
	//	HWND hwnd,
	//	UINT msg,
	//	WPARAM wParam,
	//	LPARAM lParam);

	void AddWorldName(string name);

	void SetProgressbarWindowHandle(HWND hwnd) { m_hwndProgressBar = hwnd; }

	void AddMapEditorSystem(MapEditorSystem2D* mapEditorSystem) { m_mapEditorSystem2D = mapEditorSystem; }

	void SetMapCreated(bool set) { m_mapCreated = set; }
	bool IsMapCreated() { return m_mapCreated; }

	GameType2D& GameType() { return m_GameType; }
	GameType2D GameType() const { return m_GameType; }

private:
	
	LRESULT Command(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT Paint(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
	D3D* m_D3DSystem;
	Window* m_Window;
	
	MapEditorSystem2D* m_mapEditorSystem2D;
	
	WinButton m_createButton;
	WinButton m_closeButton;
	
	WinLabel m_mapWidthLabel;
	WinTextbox m_MapWidthTextbox;
	WinLabel m_mapHeightLabel;
	WinTextbox m_MapHeightTextbox;
	
	WinLabel m_MapNameLabel;
	WinTextbox m_MapNameTextbox;
	
	WinLabel m_worldNameLabel;
	WinComboBox m_worldNameComboBox;
	
	vector<string> m_createdWorlds;
	
	HWND m_hwndProgressBar;
	
	GameType2D m_GameType;
	
	int m_xSize;
	int m_ySize;
	string m_WorldName;
	bool m_mapCreated;
	
	bool m_escape;
	bool m_created;
	bool m_running;
	
	//HINSTANCE m_hInstance;
	//HWND m_hwnd;
	//HBITMAP m_backgroundImage;
	//HDC m_winDC;
	
	int m_windowWidth;
	int m_windowHeight;
};
//================================================================================================================
//static LRESULT CALLBACK NewPlatformerMapWndProc(
//		HWND hwnd,
//		UINT msg,
//		WPARAM wParam,
//		LPARAM lParam);
//--------------------------------------------------------------------------------------------------------------------
//static NewMapWindowPlatformer* m_newPlatformerMapInstance = NULL;
//--------------------------------------------------------------------------------------------------------------------
#endif//__NEWPLATFORMERMAPWINDOW_H