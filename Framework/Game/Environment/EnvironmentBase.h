//================================================================================================================
// EnvironmentBase.h
//
// This will hold what is needed for a 2D and 3D environment
//================================================================================================================
// History
//
// Created on 3/11/2015 by Dustin Watson
//================================================================================================================
#ifndef __ENVIRONMENTBASE_H
#define __ENVIRONMENTBASE_H
//================================================================================================================
//================================================================================================================
#include <memory>
#include <Windows.h>
#include <string>
#include <windowsx.h>
#include <CommCtrl.h>
#include "Window.h"
#include "Timer.h"
#include "MenuSystem.h"
#include "HUDSystem.h"
#include "D3D.h"
#include "Input.h"
#include "Camera.h"
#include "GameState.h"
#include "GameDirectory.h"
//#include "TextureShader.h"
//#include "TransparentShader.h"
#include "CreateGameDirectory.h"
#include "ZShadeMessageCenter.h"
#include "ZShadeResource.h"
#include "SplitterWindow.h"
using namespace GUIComponents;
//================================================================================================================
class EnvironmentBase
{
public:
	
	EnvironmentBase(EngineOptions* engineOptions, LPCSTR base_window_name, LPCSTR render_window_name);
	~EnvironmentBase();
	
public:
	
	// This must be called before the environment is initialized
	bool InitBase(EngineOptions* engineOptions, LPCSTR base_window_name, LPCSTR render_window_name);
	
	void GoFullscreen(bool bEnable);
	
	void SetGameDirectory(GameDirectory2D* gd);
	void SetGameDirectory(GameDirectory3D* gd);
	
	Window* GetRenderWindow() { return m_RenderWindow; }
	Window* GetBaseWindow() { return m_BaseWindow; }
	D3D* GetD3DSystem() { return m_D3DSystem; }
	EngineOptions* GetEngineOptions() { return m_EngineOptions; }
	GameDirectory2D* GetGD2D() { return m_GameDirectory2D; }
	GameDirectory3D* GetGD3D() { return m_GameDirectory3D; }
	XMFLOAT3 GetCameraPosition() { return m_CameraSystem->Position(); }
	Camera* GetCamera() { return m_CameraSystem; }
	MenuSystem*& GetMenuSystem() { return m_MenuSystem; }
	HUDSystem*& GetHUDSystem() { return m_HUDSystem; }
	Keyboard* GetKeyboard() { return keyboard; }
	Mouse* GetMouse() { return mouse; }
	bool& IsGameInitialized() { return m_GameInitialized; }
	const SplitterWindow* GetBaseEditWindow() const { return m_BaseEditWindow; }
	SplitterWindow*& GetBaseEditWindow() { return m_BaseEditWindow; }

	LRESULT WindowResized(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT WindowMouseDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT WindowMouseUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT WindowMouseMove(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
	virtual void BeforeReset();
	virtual void AfterReset();
	
	virtual void OnWindowResized(WPARAM wParam);
	virtual void OnMouseDown(WPARAM btnState, int x, int y);
	virtual void OnMouseUp(WPARAM btnState, int x, int y);
	virtual void OnMouseMove(WPARAM btnState, int x, int y);
	
protected:
	
	Keyboard* keyboard;
	Mouse* mouse;
	D3D* m_D3DSystem;
	Camera* m_CameraSystem;
	EngineOptions* m_EngineOptions;
	//TextureShader* m_TextureShader;
	//TransparentShader* m_TransparentShader;
	ZShadeSandboxGlobal::Timer m_Timer;
	Window* m_RenderWindow;
	Window* m_BaseWindow;
	SplitterWindow* m_BaseEditWindow;
	GameDirectory2D* m_GameDirectory2D;
	GameDirectory3D* m_GameDirectory3D;
	HUDSystem* m_HUDSystem;
	MenuSystem* m_MenuSystem;
	bool m_GameInitialized;
	HINSTANCE m_HInstance;
};
//================================================================================================================
#endif//__ENVIRONMENTBASE_H