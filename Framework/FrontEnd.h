//==================================================================================================================================
// FrontEnd.h
//
// Front End window that will decide if we are entering the sandbox editor or game
//==================================================================================================================================
// History
//
// -Created on 3/6/2015 by Dustin Watson
//==================================================================================================================================
#ifndef __FRONTEND_H
#define __FRONTEND_H
//==================================================================================================================================
//==================================================================================================================================
#include "Window.h"
#include "WinLabel.h"
#include "WinButton.h"
#include "WinTextbox.h"
#include "WinCheckbox.h"
#include "EngineOptions.h"
using namespace GUIComponents;
//==================================================================================================================================
#define EDITOR_MODE_BUTTON		0
#define GAME_MODE_BUTTON		1
#define EXIT_BUTTON				2
#define MODE_LABEL				3
#define GAME_FOLDER_LABEL		4
#define GAME_FOLDER_TEXT		5
#define REMEMBER_CHECKBOX		6
//==================================================================================================================================
class FrontEnd
{
	Window* m_Window;
	//HINSTANCE m_Instance;
	
	WinLabel    m_Label;
	WinButton   m_EditorModeButton;
	WinButton   m_GameModeButton;
	WinButton   m_ExitButton;
	WinLabel    m_GameFolderLabel;
	WinTextbox  m_GameFolderText;
	WinCheckbox m_RememberCheckbox;
	
	EngineOptions* m_EngineOptions;
	bool m_remember;
	
public:
	
	FrontEnd(EngineOptions* eo);
	
	bool Init();
	
	int Run();
	
	LRESULT Command(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	
	bool InitEditorMode();
	bool InitGameMode();
};
//==================================================================================================================================
//==================================================================================================================================
#endif//__FRONTEND_H