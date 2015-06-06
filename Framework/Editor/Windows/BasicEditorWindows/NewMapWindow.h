//================================================================================================================
// NewMapWindow.h
//
//================================================================================================================
// History
//
// Created on 1/11/2014 by Dustin Watson
//================================================================================================================
#ifndef __NEWMAPWINDOW_H
#define __NEWMAPWINDOW_H
//================================================================================================================
//#include <Windows.h>
//#include <CommCtrl.h>
//================================================================================================================
#include "D3D.h"
#include "WinText.h"
#include "WinCheckbox.h"
#include "WinLabel.h"
#include "WinButton.h"
#include "WinComboBox.h"
#include "WinTextbox.h"
#include "WinRadioGroup.h"
#include "MapEditorSystem2D.h"
class MapEditorSystem2D;
using namespace GUIComponents;
//================================================================================================================
// Need to add in Isometric or Standard maps
//Defines
#define CLOSE_BUTTON 0
#define CREATE_BUTTON 1
#define MAP_SIZE_TEXT 2
#define MAP_SIZE_CB 3
#define FOW_ENABLED_TEXT 4
#define FOW_ENABLED_RC 5
#define YES_RADIO_BUTTON 6
#define NO_RADIO_BUTTON 7
#define FOW_RADIUS_TEXT 8
#define FOW_RADIUS_CB 9
#define ISO_STAND_TEXT 10
#define ISO_STAND_RG 11
#define ISOMETRIC_RADIO_BUTTON 12
#define STANDARD_RADIO_BUTTON 13
#define MAP_NAME_TEXT 14
#define MAP_NAME 15
#define WORLD_NAME_TEXT 16
#define WORLD_NAME_CB 17
#define FOW_FLASHLIGHT 18
//================================================================================================================
class NewMapWindow
{
public:
	NewMapWindow(MapEditorSystem2D* mapEditorSystem, HINSTANCE hInstance, D3D* d3d);
	NewMapWindow(const NewMapWindow& other);
	~NewMapWindow();
	
	int GetXSize() { return m_xSize; }
	int GetYSize() { return m_ySize; }
	bool GetFOWEnabled() { return m_fowEnabled; }
	int GetFOWRadius() { return m_fowRadius; }
	
	bool IsRunning() { return m_running; }
	bool IsCreated() { return m_created; }
	void SetCreated(bool created) { m_created = created; }
	
	void Show(bool show = false);
	void SetPosition(int x, int y);
	void SetEscape(bool bSet) { m_escape = bSet; }
	void Invalidate();
	
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
	LRESULT ColorStatic(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	D3D* m_D3DSystem;
	Window* m_Window;
	
	WinButton m_createButton;
	WinButton m_closeButton;
	
	WinLabel m_mapSizeLabel;
	WinComboBox m_mapSizeCombobox;
	
	WinLabel m_IsoStandLabel;
	WinRadioGroup m_IsoStandRadioContainer;
	
	WinLabel m_MapNameLabel;
	WinTextbox m_MapNameTextbox;
	
	WinLabel m_worldNameLabel;
	WinComboBox m_worldNameComboBox;
	
	WinLabel m_fowEnabledLabel;
	WinRadioGroup m_fowEnabledRadioContainer;
	WinLabel m_fowRadiusLabel;
	WinComboBox m_fowRadiusComboBox;
	WinCheckbox m_fowFlashlightEnabled;
	
	vector<string> m_createdWorlds;
	
	HWND m_hwndProgressBar;
	
	MapEditorSystem2D* m_mapEditorSystem2D;
	
	GameType2D m_GameType;
	
	int m_xSize;
	int m_ySize;
	//int m_playerPosition;
	bool m_fowEnabled;
	int m_fowRadius;
	bool m_fowFlashlight;
	bool bIso;
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
//static LRESULT CALLBACK NewTopdownMapWndProc(
//		HWND hwnd,
//		UINT msg,
//		WPARAM wParam,
//		LPARAM lParam);
//--------------------------------------------------------------------------------------------------------------------
//static NewMapWindow* m_newMapInstance = NULL;
//--------------------------------------------------------------------------------------------------------------------
#endif//__NEWMAPWINDOW_H